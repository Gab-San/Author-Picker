#ifndef HELPER_LIB_H
#define HELPER_LIB_H

#include <stdio.h>

#define CHAR_DIFF (97-65)

int len_of_str(char* buffer, int max_len);
void read_input(char* buffer, int max_len);
char conv_to_upper(char c);
char conv_to_lower(char c);
#endif