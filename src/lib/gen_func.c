#include "gen_func.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_line(char *buffer, size_t max_len) {
    char format[32]; // Max int (10) + '%' '[' 'A' '-' 'z' '\s' ']' (7) rounded up to the closest power of two (thanks Daniel Trugman)
    snprintf(format, sizeof(format), "%%%ld[0-9A-z ]", max_len - 1);
    // printf("FORMAT: %s\n", format);
    scanf(format, buffer);
    flush_stdin();
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

void flush_stdin() {
    while(getchar() != '\n' && getchar() != EOF);
}
