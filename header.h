#ifndef HEADER_H
#define HEADER_H

#include "types.h" 

typedef struct _MP3
{
    char *mp3_fname; 
    FILE *fptr_view;  

    char *edit_fname;
    char *edit_opt;
    char *new_data;
    FILE *fptr_edit;
    FILE *fptr_temp;

}MP3;

/* prototypes*/

/* Check operation type */
OperationType check_operation_type(char opt);

//OperationType check_edit_operation_type(char opt);

/* Read and validate args from argv */
Status read_and_validate_view_args(int argc, char *argv[], MP3 *mp3);

Status read_and_validate_edit_args(int argc, char *argv[], MP3 *mp3);

/* check mp3 header*/
Status check_mp3_header(FILE *fptr);

Status copy_mp3_header(FILE *src,FILE *dest);

/* Perform the view operation */
Status view_metadata(MP3 *mp3);

/* Perform the edit operation */
Status edit_metadata(MP3 *mp3);

/* Convert editing option into its ID3 frame ID */
char *get_frame_id(char option);

/* check Endianess*/
Status endianess(MP3 *mp3, int *size);

#endif
