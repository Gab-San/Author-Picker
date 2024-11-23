#include "helper_lib.h"
#include <stdio.h>
#include <stdlib.h>

int len_of_str(char* buffer, int max_len){
    int counter = 0;
    while(counter < max_len && *(buffer + counter) != '\0') counter++;
    return counter;
}

void read_input(char *buffer, int max_len) {
    char format[32]; // Max int (10) + '%' '[' 'A' '-' 'z' '\s' ']' (7) rounded up to the closest power of two (thanks Daniel Trugman)
    snprintf(format, sizeof(format), "%%%d[A-z ]", max_len - 1);
    // printf("FORMAT: %s\n", format);
    scanf(format, buffer);
}

int cmp_eq_char(char c, char extr_min, char extr_max){
    return extr_min <= c && c <= extr_max;
}

char conv_to_upper(char c){
    // if it's already an upper letter it doesn't get converted
    return cmp_eq_char(c, 'A', 'Z') || !cmp_eq_char(c, 'A', 'z') ? c : c - CHAR_DIFF;
}

char conv_to_lower(char c) {
    // if it's already a lower letter it doesn't get converted
    return cmp_eq_char(c, 'a', 'z') || !cmp_eq_char(c, 'A', 'z') ? c : c + CHAR_DIFF;
}