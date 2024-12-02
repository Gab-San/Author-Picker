#ifndef HELPER_LIB_H
#define HELPER_LIB_H

#include <stdio.h>
#define INPUT ("> ")

/*
    Difference between 'a' and 'A'.
*/
#define CHAR_DIFF (97-65)
/*
    This function returns the maximum between the length of the string or the given maximum length. 

    # Input
    max_len <= buffer.length;
    The string should be well formatted, meaning it should terminate with a null terminator character ('\0').

    # Output
    Maximum between the length of the string and the specified max_len
*/
int len_of_str(char* buffer, size_t max_len);

/*
    Reads a line from stdin (a sequence of characters and spaces till newline) of max_len (spaces included).

    It basically is a wrapper of the scanf function that limits the length of the read correctly
    formatting the reading string to read a line.

    # Input
    buffer := to save the input;
    max_len := maximum input length.

    ## Pre-conditions
    max_len <= buffer.length.

    # Output
    Fills the buffer with the input from stdin.
*/
void read_line(char* buffer, size_t max_len);

/*
    Converts a character to the upper-case equivalent.

    # Input
    A lower-case character

    # Output
    The equivalent upper-case character

    ## Edge cases
    If the given character c is not a letter of the alphabet, 
    then it's not converted and the returned value is c.

    If the given character c is already an upper-case character,
    then the returned value is c. 
*/
char conv_to_upper(char c);
/*
    Converts a character to the lower-case equivalent.

    # Input
    An upper-case character

    # Output
    The equivalent lower-case character

    ## Edge cases
    If the given character c is not a letter of the alphabet,
    then it's not converted and the returned value is c.

    If the given character c is already a lower-case character,
    then the returned value is c.
*/
char conv_to_lower(char c);

void str_to_lower(char* buffer);

/*
    Prints the error message and aborts the program.

    # Input
    The error message to be printed along the error
*/
void fatal(const char* msg);

void report_fatal_error(const char* msg);

const char* get_config_file();

void setup_config(const char* optional_dialog);

void flush_stdin();

char* trim(char* mod_str);

#endif