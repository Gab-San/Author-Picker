#include "string_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int len_of_str(char* buffer, size_t max_len){
    if(buffer == NULL) return 0;

    int counter = 0;
    while(counter < max_len && *(buffer + counter) != '\0') counter++;
    return counter;
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

char* trim(char* org_str){

    if(org_str == NULL) return NULL;
    if (strlen(org_str) == 0) return NULL;

    char* front = org_str;
    char* end = org_str + strlen(org_str);

    while(*front == ' ') front++;
    if(front == end){ // String filled with spaces
        char* trimmed_str = (char*) malloc(1);
        trimmed_str[0] = '\0';
        return trimmed_str;
    }

    end--; // end is now on the last character before termination char;
    while(*end == ' ' && end != front) end--;
    
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

