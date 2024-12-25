#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <stdio.h>

// A structure to handle in an easier way copy operations.
typedef struct char_buffer { 
    char* buf;
    int bytes;
} cbuffer_t;


/*
    # Open file
    Tries to open the specified file in path and returns the associated stream.
    It iş a wrapper around the fopen function that checks for an error.

    ## Input
    path -> path to the file to open;
    access_mode -> "r" read, "w" truncate and write, "a" append; using a plus adds the remaining access mode (e.g "r+" read-write)
    calling_func -> name of the calling function, so that if an error occurs it will state the function trying to open the file 
        (set to NULL to omit).

    ## Returns
    The file pointer to the stream associated with the opened file

    ## Input modification
    The 'calling_func' length is stripped after 70 characters to make the copy of the value safe.

    ## Error
    If an error occurs while opening the file this function will exit the program with an error message.
*/
FILE* open_file(const char* path, const char* access_mode, char* calling_func);

/*
    # Close file
    Tries to close the stream of the file pointer 'fp'.

    ## Input
    - fp -> file pointer;
    - calling_func -> name of the calling_function, so that if an error occurs it will state the function trying to open the file.

    ## Input modification
    The 'calling_func' length is stripped after 70 characters to make the copy of the value safe.

    ## Error
    If an error occurs while closing the file this function will exit the program with an error message.

*/
void close_file(FILE* fp, char* calling_func);

/*
    # Go to
    Moves the file cursor to the start of the requested line.
    
    ## Input
    Line to go to. The cursor will be positioned at the start of the line.

    ## Returns
    The value of bytes read (including newline) 
*/
int go_to_line(int line, FILE* fp);

/*
    # Skip to from
    Starting from the from_line moves the cursor to the start of the to_line.

    ## Input
    from_line: the line from which the cursor will start.
    to_line: the line from which the cursor will be positioned at (start).
    
    ### Pre-conditions
    from_line <= to_line

    ## Returns
    Returns the number of bytes read during this operation (including newlines)
*/
int skip_to_line_from(int from_line, int to_line, FILE* fp);

/*
    # Copy text
    Copies a chunck of bytes from the file into memory.

    ## Input
    fp -> file to copy from
    bytes -> bytes to copy

    ## Returns
    A cbuffer pointer to the copied bytes.
*/
cbuffer_t* copy_text(FILE* fp, int bytes);

#endif