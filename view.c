#include <stdio.h>
#include "types.h"
#include "header.h"
#include <string.h>

Status read_and_validate_view_args(int argc, char *argv[], MP3 *mp3)
{

    printf("Validating viewing arguments!!!\n");

    if(argv[2] == NULL)
    {
        printf("Error -> Soure file name not found.\n");
        return e_failure;
    }
    else if (strstr(argv[2], ".mp3") == NULL)
    {
        printf("Error -> Source file must have a .mp3 extension.\n");
        return e_failure;
    }
    else if(strlen(argv[2]) < 4 || strcmp(argv[2] + strlen(argv[2]) - 4, ".mp3") != 0)
    {
        printf("Error -> Invalid source file name.\n");
        return e_failure;
    }
    else
    {
        mp3->mp3_fname = argv[2];
    }

    mp3->fptr_mp3=fopen(argv[2],"rb");
    if(mp3->fptr_mp3 == NULL)
    {
        printf("Error -> Unable to open Source file.\n");
        return e_failure;
    }
    
}