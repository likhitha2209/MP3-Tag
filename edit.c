#include <stdio.h>
#include "types.h"
#include "header.h"
#include <string.h>


Status read_and_validate_edit_args(int argc, char *argv[], MP3 *mp3)
{
    printf("Validating editing arguments!!!\n");

    if(argv[2]==NULL)
    {
        printf("Invalid Input -> unable to fetch the editing operation\n");
        return e_failure;
    }

    if(argv[2][1] != 't' || argv[2][1] != 'a' ||argv[2][1] != 'A' ||argv[2][1] != 'm' ||argv[2][1] != 'y' ||argv[2][1] != 'c')
    {
        printf("Invalid Input -> Editing options are : -t/-a/-A/-m/-y/-c\n");
        return e_failure;
    }

    if(argv[4]==NULL)
    {
        printf("Invalid Input -> unable to fetch the edit file name\n");
        return e_failure;
    }

    else if (strstr(argv[4], ".mp3") == NULL)
    {
        printf("Error -> Source file must have a .mp3 extension.\n");
        return e_failure;
    }
    else if(strlen(argv[4]) < 4 || strcmp(argv[4] + strlen(argv[4]) - 4, ".mp3") != 0)
    {
        printf("Error -> Invalid source file name.\n");
        return e_failure;
    }
    else
    {
        mp3->fptr_edit = argv[4];
    }

    mp3->fptr_temp=fopen("temp.mp3","wb");
    if(mp3->fptr_temp==NULL)
    {
        printf("Error -> Unable to open the file\n");
        return e_failure;
    }

    return e_success;
}


Status copy_mp3_header(FILE *fptr)
{

}
Status edit_metadata(MP3 *mp3)
{
    if(copy_mp3_header(mp3->fptr_edit) == e_failure)
    {
        printf("Error -> Failed to skip 10 bytes of mp3 header\n");
        return e_failure;
    }
    
    
    return e_success;
}
