#include <stdio.h>
#include "types.h"
#include "header.h"
#include <string.h>
#include <stdlib.h>

Status read_and_validate_edit_args(int argc, char *argv[], MP3 *mp3)
{
    printf("Validating editing arguments!!!\n");

    if (argc != 5)
    {
        printf("Error -> Invalid argument count\n");
        return e_failure;
    }

    if (strcmp(argv[1], "-e") != 0)
    {
        printf("Error -> Invalid operation\n");
        return e_failure;
    }

    if (strlen(argv[2]) != 2 || argv[2][0] != '-')
    {
        printf("Invalid editing option\n");
        return e_failure;
    }

    if (argv[2][1] != 't' && argv[2][1] != 'a' && argv[2][1] != 'A' && argv[2][1] != 'm' && argv[2][1] != 'y' && argv[2][1] != 'c')
    {
        printf("Invalid Input -> Editing options are : -t/-a/-A/-m/-y/-c\n");
        return e_failure;
    }

    mp3->edit_opt = argv[2];

    if (argv[3] == NULL || argv[4] == NULL)
    {
        printf("Error -> Missing editing data or filename\n");
        return e_failure;
    }

    mp3->new_data = argv[3];

    if (strstr(argv[4], ".mp3") == NULL)
    {
        printf("Error -> Source file must have a .mp3 extension.\n");
        return e_failure;
    }
    else if (strlen(argv[4]) < 4 || strcmp(argv[4] + strlen(argv[4]) - 4, ".mp3") != 0)
    {
        printf("Error -> Invalid source file name.\n");
        return e_failure;
    }
    else
    {
        mp3->edit_fname = argv[4];
    }

    mp3->fptr_edit = fopen(argv[4], "rb");
    if (mp3->fptr_edit == NULL)
    {
        printf("Error -> Unable to open the file\n");
        return e_failure;
    }

    mp3->fptr_temp = fopen("temp.mp3", "wb");
    if (mp3->fptr_temp == NULL)
    {
        printf("Error -> Unable to open the file\n");
        return e_failure;
    }

    return e_success;
}

char *get_frame_id(char option)
{
    switch (option)
    {
    case 't':
        return "TIT2";
        break;

    case 'a':
        return "TPE1";
        break;

    case 'A':
        return "TALB";
        break;

    case 'c':
        return "COMM";
        break;
    
    case 'm':
        return "TCOM";
        break;

    case 'y':
        return "TYER";
        break;

    default:
        return NULL;
        break;
    }
}

Status copy_mp3_header(FILE *src, FILE *dest)
{
    char buff[10];

    // read 10 bytes of mp3 header
    fread(buff, 10, 1, src);

    // write 10 bytes of mp3 header
    fwrite(buff, 10, 1, dest);

    return e_success;
}

Status edit_metadata(MP3 *mp3)
{
    if (copy_mp3_header(mp3->fptr_edit, mp3->fptr_temp) == e_success)
    {
        printf("ID3 header copied successfully!\n");
    }

    char buff[5];
    int i, size,new_size;

    printf("Before getting frame ID\n");
    // get frame ID
    char *tag = get_frame_id(mp3->edit_opt[1]);
    printf("Frame ID: %s\n", tag);

    if (tag == NULL)
    {
        printf("Invalid editing option\n");
        return e_failure;
    }

    // read 4 bytes of tag from mp3 file
    fread(buff, 4, 1, mp3->fptr_edit);
    buff[4] = '\0';

    // copy 4 bytes of tag to temp
    fwrite(buff, 4, 1, mp3->fptr_temp);

    // read size & endianness conversion from big to little
    edit_endianess(mp3, &size);

    if (size <= 0)
    {
        printf("Invalid frame size\n");
        return e_failure;
    }
    char flag[3];
    char *data;

    // skip 2 flag bytes and 1 encoding byte
    fread(flag, 1, 3, mp3->fptr_edit);

    if(strcmp(buff, tag) == 0)
    {
        // calculate new data size
        new_size = strlen(mp3->new_data) + 1;
        printf("The new_frame size is %d\n",new_size);

        // convert new size into big endian
        unsigned char size_buff[4];

        convert_to_big_endian(new_size, size_buff);

        // write new size into temp file
        fwrite(size_buff, 1, 4, mp3->fptr_temp);

        // copy 3 bytes of flag and encoding
        fwrite(flag, 1, 3, mp3->fptr_temp); 

        // write new data
        fwrite(mp3->new_data, 1, strlen(mp3->new_data), mp3->fptr_temp);

        // Skip old frame data in source file
        fseek(mp3->fptr_edit, size-1, SEEK_CUR);
        printf("Metadata updated successfully!\n");
    }
    else
    {
        // convert old size into big endian
        unsigned char size_buff[4];

        convert_to_big_endian(size, size_buff);
        // write old size
        fwrite(size_buff, 1, 4, mp3->fptr_temp);

        // copy flags and encoding
        fwrite(flag, 1, 3, mp3->fptr_temp);

        // allocate memory for old data
        data = malloc(size);

        if(data == NULL)
        {
            printf("Memory allocation failed\n");
            return e_failure;
        }       

        // read old data
        fread(data, 1, size-1, mp3->fptr_edit);

        // copy old data into temp
        fwrite(data, 1, size-1, mp3->fptr_temp);

        free(data);
    }

    // copy remaining audio data
    while((i = fgetc(mp3->fptr_edit)) != EOF)
    {
        fputc(i, mp3->fptr_temp);
    }

    fclose(mp3->fptr_edit);
    fclose(mp3->fptr_temp);

    // replace original file
    remove(mp3->edit_fname);
    rename("temp.mp3", mp3->edit_fname);

    printf("Metadata editing completed successfully!\n");

    return e_success;
}

Status edit_endianess(MP3 *mp3, int *size)
{
    unsigned char buff[4];
    unsigned char temp;

    // read 4 bytes of size
    fread(buff,1,4,mp3->fptr_edit);
    
    //converting from big endian to little endian by swapping  
    temp = buff[0];
    buff[0] = buff[3];
    buff[3] = temp;

    temp=buff[1];
    buff[1] = buff[2];
    buff[2] = temp;

    //convert reversed byte into integer
    *size= *(int  *)buff;

    return e_success;
}

Status convert_to_big_endian(int size, unsigned char *buff)
{
    unsigned char *ptr = (unsigned char *)&size;

    buff[0] = ptr[3];
    buff[1] = ptr[2];
    buff[2] = ptr[1];
    buff[3] = ptr[0];

    return e_success;
}
