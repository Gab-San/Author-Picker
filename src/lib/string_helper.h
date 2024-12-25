#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <stdio.h>

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

/*
    Converts a string to lower case.
*/
void str_to_lower(char* buffer);

/*
    Generates a trimmed version of the given string and returns it.

    # Input
    mod_str -> string to trim

    # Output
    A trimmed version of the given string

    # Warning
    Deallocation of the original string is delegated to the user!
*/
char* trim(char* mod_str);

#endif