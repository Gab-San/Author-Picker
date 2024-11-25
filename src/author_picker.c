#include "author_picker.h"
#include "helper_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>


#define ERR_MSG_LEN 100

const char* datafile = "./out/author_db.txt";

void fatal(const char* msg){
    char error_message[ERR_MSG_LEN];
    strcat(error_message, "[!!] Fatal error:");
    strncat(error_message, msg, ERR_MSG_LEN);
    perror(error_message);
    exit(-1);
}

char* format_data(char* author_name){
    // COPY MANUALLY
    char* buf = (char*) malloc(sizeof(char)* (AUTHOR_LEN + 1));
    char fill[AUTHOR_LEN] = {' '};
    strcpy(buf, author_name);
    printf("%s\n", buf);
    strncat(buf, fill, AUTHOR_LEN - len_of_str(author_name, AUTHOR_LEN));
    buf[AUTHOR_LEN] = '\n';
    printf("%s\n", buf);
    for(int i = 0; i < AUTHOR_LEN +1; i++){
        printf("0x%x\n", buf[i]);
    }
    return buf;
}

void append_author(FILE* fp, char* auth_name){
    fseek(fp, 0, SEEK_END);
    char* formatted_data = format_data(auth_name);
    fwrite(formatted_data, AUTHOR_LEN + 1, 1, fp);
    free(formatted_data);
    printf("%s inserted!\n", auth_name);
}

int search_author(FILE* fp, char* auth){
    // MISSING OFFSET OF THE FIRST LINE

    char input_read[AUTHOR_LEN + 1];
    int items_read;
    fseek(fp, 0, SEEK_SET);
    do{
        input_read[0] = '\0';
        items_read = fread(input_read, AUTHOR_LEN + 1, 1, fp);
        printf("READ IT: %d. INPUT: %s\n", items_read, input_read);
        // ALL OF THE SUBSTRING OF INPUT_READ ARE CONSIDERED EQUAL aaa == aa
        if( strncmp(input_read, auth, len_of_str(auth,AUTHOR_LEN) - 1) == 0 ){
            return 1;
        }
    }while(items_read != 0);

    return 0;
}   

void insert_author(char* author_name) {
    // At this point the buffer contains the name of the author
    // this means that 2 actions are to be performed:
    // 1. the file must be scanned to find if the author was already inserted
    //      A. the author was inserted: the functions notifies the user and returns
    //      B. the author wasn't previously inserted: proceed normally
    // 2. insert the name of the author at the end of the file
    // 
    // THE FIRST LINE OF THE FILE WILL CONTAIN THE AUTHOR THAT WAS LAST EXTRACTED

    assert( strlen(author_name) != 0 ); 

    FILE* fp = fopen(datafile, "a+");
    if(fp == NULL){
        fatal("in insert_author() while opening file.");
    }

    if( search_author(fp, author_name) == 1 ){
        printf("%s was previously inserted!\n", author_name);
        return;
    }

    append_author(fp, author_name);

    if(fclose(fp) != 0) {
        fatal("in insert_author() while closing file.");
    }
}

void remove_author(char* author_name){

}

// void read_authors(){}
// void find_author(char* author_name) {}

int num_of_authors(FILE* fp) {
    // SHOULD ADD THE OFFSET OF THE EXTRACTED GUY
    int counter = 0;
    int item_read = 0;
    char input_read[AUTHOR_LEN + 1];

    do{
        input_read[0] = '\0';
        item_read = fread(input_read, AUTHOR_LEN + 1, 1, fp);
        counter += item_read;
        printf("[DEBUG] Items read: %d\t Input: %s\n", counter, input_read);
    }while(item_read != 0);

    return counter;
}

void extract_author() {
    // At this point the file should exist
    FILE* fp = fopen(datafile, "r+");
    if (fp == NULL) {
        fatal("in extract_author() while opening file");
    }

    int reads = num_of_authors(fp);

    assert(reads != 0);
    
    // srand(time(NULL));
    // int r = rand() % reads;
    // char extracted[AUTHOR_LEN];

}
