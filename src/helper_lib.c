#include "helper_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_MSG_LEN 100
#define SETUP_DIALOG ("Setting up config file...\n\nHow much time do you want to wait before extracting a new author's name?\n")
#define DIALOG_OPTIONS ("[H]OUR\n[D]AY\n[W]EEK\n[M]ONTH\n")

const char auth_config_path[] = "./out/config.txt";

void flush_stdin() {
    while(getchar() != '\n' && getchar() != EOF);
}

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

void str_to_lower(char* buffer){
    char* c = buffer;
    while(*c != '\0'){
        *c = conv_to_lower(*c);
        c++;
    }
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

const char* get_config_file(){
    return auth_config_path;
}

void setup_config(const char* optional_dialog){
    FILE* config = fopen(get_config_file(), "w");
    if(config == NULL) fatal("in setup_config() while opening config file");
    if(optional_dialog != NULL) printf("%s", optional_dialog);
    printf(SETUP_DIALOG);
    printf(DIALOG_OPTIONS);
    printf(INPUT);
    
    char input = getchar() + '\0';
    flush_stdin();
    char low_in = conv_to_lower(input);
    
    while(low_in != 'w' && low_in != 'h' && low_in != 'd' && low_in != 'm'){
        printf("Inserted option not valid!\nPlease insert a valid option!\n");
        printf(INPUT);
        input = getchar() + '\0';
        flush_stdin();
        low_in = conv_to_lower(input);
    };
    
    fwrite(&low_in, 1, 1, config);
    
    fclose(config);
}

int unsafe_len_of_str(char* buf){
    int count = 0;
    while(buf[count] != '\0') count++;
    return count;
}

char* trim(char* mod_str){
    char* front = mod_str;
    char* end = mod_str + unsafe_len_of_str(mod_str) - 1;
    
    if(mod_str == NULL) return NULL;
    if (unsafe_len_of_str(mod_str) == 0) return NULL;
    if (*front == '\0'){
        char* trimmed_str = (char*) malloc(1);
        trimmed_str[0] = '\0';
        return trimmed_str;
    }

    while(*front == ' ') front++;
    if(front == end){ 
        char* trimmed_str = (char*) malloc(1);
        trimmed_str[0] = '\0';
        return trimmed_str;
    }
    while(*end == ' ' && front != end) end--;

    if(front == end){
        char* trimmed_str = (char*) malloc(1);
        trimmed_str[0] = '\0';
        return trimmed_str;
    }
    
    char* p = front;
    int len = 0;
    while(p != (end + 1)){
        len++;
        p++;
    }

    char* trimmed_str = (char*) malloc(len + 1);
    for(int i = 0; front != (end + 1); front++, i++){
        trimmed_str[i] = *front;
    }

    trimmed_str[len] = '\0';
    return trimmed_str;
}