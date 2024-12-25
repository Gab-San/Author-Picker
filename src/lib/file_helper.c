#include "file_helper.h"
#include "gen_func.h"

#include <string.h>
#include <stdlib.h>


#define MAX_FUNC_LEN 70

FILE* open_file(const char* path, const char* access_mode, char* calling_func) {
    
    char msg[ERR_MSG_LEN] = "in ";

    if(calling_func == NULL)
        strcat(msg, "unknown func");
    else 
        strncat(msg, calling_func, MAX_FUNC_LEN);

    int error_offset = strlen(msg);
    strncat(msg, " while opening file", ERR_MSG_LEN - error_offset);

    FILE* fp = fopen(path, access_mode);
    if(!fp) fatal(msg);

    return fp;
}

void close_file(FILE* fp, char* calling_func) {
    char msg[ERR_MSG_LEN] = "in ";
        if(calling_func == NULL)
        strcat(msg, "unknown func");
    else 
        strncat(msg, calling_func, MAX_FUNC_LEN);

    int error_offset = strlen(msg);
    strncat(msg, " while closing file", ERR_MSG_LEN - error_offset);
    
    if(fclose(fp) != 0) fatal(msg);
}

/*
    # GO TO
    Moves the file cursor to the start of the requested line.
    
    ## INPUT
    Line to go to. The cursor will be positioned at the start of the line.

    ## RETURNS
    The value of bytes read (including newline) 
*/
int go_to_line(int line, FILE* fp) {
    fseek(fp, 0, SEEK_SET);
    int total_bytes_read = 0;
    int count = 0;
    char c;

    while(count < line){

        while((c = fgetc(fp)) != '\n'){
            if(c == EOF) break;

            total_bytes_read++;
        }

        total_bytes_read++; // Counting for '\n'
        count++;
    }

    return total_bytes_read;
}

/*
    # SKIP TO FROM
    Starting from the from_line moves the cursor to the start of the to_line.

    ## INPUT
    from_line: the line from which the cursor will start.
    to_line: the line from which the cursor will be positioned at (start).
    
    ### PRE-CONDITIONS
    from_line <= to_line

    ## RETURNS
    Returns the number of bytes read during this operation (including newlines)
*/
int skip_to_line_from(int from_line, int to_line, FILE* fp){
    go_to_line(from_line, fp);
    int count = from_line;
    int bytes_read = 0;
    char c;
    while(count < to_line){
        while((c = fgetc(fp)) != '\n'){
            if(c == EOF) break;
            bytes_read++;
        }
        bytes_read++; // Counting newlines
        count++;
    }
    return bytes_read;
}

cbuffer_t* copy_text(FILE*fp, int bytes_read){
    cbuffer_t* cp_buf = (cbuffer_t*) malloc(sizeof(cbuffer_t));
    
    if(cp_buf == NULL) fatal("in copy_text() while trying to allocate memory for cp_buf");
    cp_buf->buf = (char*) malloc(bytes_read);
    
    if(cp_buf->buf == NULL) fatal("in copy_text() while trying to allocate memory for cp_buf->buf");
    cp_buf->bytes = bytes_read;

    fread(cp_buf->buf, cp_buf->bytes, 1, fp);
    return cp_buf;
}