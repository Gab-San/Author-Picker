#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <stdio.h>

/*
    Difference between 'a' and 'A'.
*/
#define CHAR_DIFF (97-65)

/*
    # Length of string
    This function returns the maximum between the length of the string or the given maximum length. 

    ## Input
    buffer -> character buffer = string
    max_len -> maximum length of the string !(max_len <= buffer.length);
    

    ## Returns
    Maximum between the length of the string and the specified max_len
*/
int len_of_str(char* buffer, size_t max_len);

/*
    # Convert to upper case
    Converts a character to the upper-case equivalent.

    ## Input
    c -> lower-case character'.

    ## Returns
    The equivalent upper-case character.

    ## Edge cases
    If the given character c is not a letter of the alphabet, 
    then it's not converted and the returned value is c.

    If the given character c is already an upper-case character,
    then the returned value is c. 
*/
char conv_to_upper(char c);

/*
    # Convert to lower case
    Converts a character to the lower-case equivalent.

    ## Input
    c -> upper-case character.

    ## Returns
    The equivalent lower-case character.

    ## Edge cases
    If the given character c is not a letter of the alphabet,
    then it's not converted and the returned value is c.

    If the given character c is already a lower-case character,
    then the returned value is c.
*/
char conv_to_lower(char c);

/*
    # String to lower case
    Converts a string to lower case.
*/
void str_to_lower(char* buffer);

/*
    # Trim
    Generates a trimmed version of the given string and returns it.

    ## Input
    org_str -> string to trim.

    ## Returns
    A trimmed version of the given string.

    ## Deallocation
    Deallocation of the original string is delegated to the user!
*/
char* trim(char* org_str);

#endif