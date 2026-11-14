#include <stdio.h>
#include "types.h"
#include "header.h"


int main(int argc,char *argv[])
{
    MP3 mp3;

    if(check_operation_type(argv[1][1]) == e_view)
    {
        if(argc < 2)
        {
            printf("Invalid input!!!\n");
            printf("Use: \n./a.out -v mp3filename( 'filename.mp3' )\n");
            return 0;
        }
        if(read_and_validate_view_args(argc, argv, &mp3) == e_success)
        {
            if(view_metadata(&mp3) == e_success)
            {
                printf("---------------------- END VIEW ----------------------\n");
            }
            else
            {
                printf("Invalid!!! \nCouldn't view the file!!!\n");
            }
        }
        else
        {
            printf("Invalid viewing arguments!!!\n");
        }
    }
    else if(check_operation_type(argv[1][1]) == e_edit)
    {
        if(argc < 4)
        {
            printf("Invalid input!!!\n");
            printf("Use: \n./a.out -v -t/-a/-A/-m/-y/-c mp3filename( 'filename.mp3' )\n");
            return 0;
        }
        if(read_and_validate_edit_args(argc, argv, &mp3) == e_success)
        {
            if(edit_metadata(&mp3) == e_success)
            {
                printf("---------------------- DONE EDITING ----------------------\n");
            }
            else
            {
                printf("Invalid!!! \nCouldn't edit the file!!!\n");
            }
        }
        else
        {
            printf("Invalid editing arguments!!!\n");
        }
    }   
    else
   {
        printf("Invalid input!!!... \nEntered arguments are invalid!\n");
   }

    return 0;
}

OperationType check_operation_type(char opt)
{
    if(opt == 'v')
    {
        printf("---------------------- VIEWING STARTED ----------------------\n");
        return e_view;
    }
    else if(opt == 'e')
    {
        printf("---------------------- EDITING STARTED ----------------------\n");
        return e_edit;
    }
    else
    {
        printf("Invalid Input!!!\n");
        return e_unsupported;
    }
}