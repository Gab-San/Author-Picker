#ifndef GEN_FUNC_H
#define GEN_FUNC_H

#include <stdio.h>
#define ERR_MSG_LEN 100
/* The string defining the start of an input: "> "*/
#define INPUT ("> ")

/*
    # Read line
    Reads a line from stdin (a sequence of characters and spaces till newline) of max_len (spaces included).

    It basically is a wrapper of the scanf function that limits the length of the read
    formatting the reading string to read a line.

    ## Input
    buffer -> to save the input;
    max_len -> maximum input length !(max_len <= buffer_length).

    ## Side Effect
    Fills the buffer with the input from stdin.
*/
void read_line(char* buffer, size_t max_len);

/*
    # Fatal error
    Prints the error message and aborts the program.

    ## Input
    msg -> optional error message to be printed along the error (can be NULL)
*/
void fatal(const char* msg);

/*
    # Report fatal error
    Reports to screen a fatal error with a completely custom message
    and then crashes the program.

    ## Input
    msg -> optional error message to be printed along the notification of the error (can be NULL)
*/
void report_fatal_error(const char* msg);

/*
    # Flush stdin
    Catches all of the newline and eof characters in stdin
*/
void flush_stdin();

#endif