#include "helper_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_MSG_LEN 100

int len_of_str(char* buffer, size_t max_len){
    int counter = 0;
    while(counter < max_len && *(buffer + counter) != '\0') counter++;
    return counter;
}

void read_line(char *buffer, size_t max_len) {
    char format[32]; // Max int (10) + '%' '[' 'A' '-' 'z' '\s' ']' (7) rounded up to the closest power of two (thanks Daniel Trugman)
    snprintf(format, sizeof(format), "%%%ld[0-9A-z ]", max_len - 1);
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

void fatal(const char* msg){
    char error_message[ERR_MSG_LEN];
    strcpy(error_message, "[!!] Fatal error:");
    const int error_offst = 17;
    strncat(error_message, msg, ERR_MSG_LEN - error_offst); // considers the length of the starting string
    perror(error_message);
    exit(-1);
}

void report_fatal_error(const char* msg){
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(-1);
}