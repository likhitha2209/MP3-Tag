#include <stdio.h>
#include "types.h"
#include "header.h"
#include <string.h>


Status read_and_validate_edit_args(int argc, char *argv[], MP3 *mp3)
{
    printf("Validating editing arguments!!!\n");

    
    {
        printf("Invalid editing arguments!!!\n");
            printf("Use: \n./a.out -v -t/-a/-A/-m/-y/-c 'New_data' mp3filename( 'filename.mp3' )\n");
        return e_failure;
    }

    return e_success;
}

Status edit_metadata(MP3 *mp3)
{

    
    return e_success;
}
