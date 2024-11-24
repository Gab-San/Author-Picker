#include "author_picker.h"
#include "helper_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define ERR_MSG_LEN 100

void fatal(const char* msg){
    char error_message[ERR_MSG_LEN];
    strcat(error_message, "[!!] Fatal error:");
    strncat(error_message, msg, ERR_MSG_LEN);
    perror(error_message);
    exit(-1);
}

void append_author(FILE* fp, char* auth_name){
    fseek(fp, 0, SEEK_END);
    
    char formatted_auth[AUTHOR_LEN];
    strncpy(formatted_auth, auth_name, AUTHOR_LEN);
    formatted_auth[len_of_str(formatted_auth, AUTHOR_LEN)] = '\n';
    fwrite(formatted_auth, AUTHOR_LEN, 1, fp);
    printf("%s inserted!\n", auth_name);
}

int search_author(FILE* fp, char* auth){
    char input_read[100];
    int items_read;
    fseek(fp, 0, SEEK_SET);
    do{
        input_read[0] = '\0';
        items_read = fread(input_read, AUTHOR_LEN, 1, fp);
        printf("READ IT: %d. INPUT: %s\n", items_read, input_read);
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
    const char* datafile = "./out/author_db.txt";
    FILE* fp = fopen(datafile, "a+");
    if(fp == NULL){
        fatal("in insert_author() while opening file.");
    }

    if( search_author(fp, author_name) == 1 ){
        printf("%s was previously inserted!\n", author_name);
        return;
    }

    append_author(fp, author_name);

    if( fclose(fp) != 0) {
        fatal("in insert_author() while closing file.");
    }
}

void remove_author(char* author_name){

}

// void read_authors(){}
// void find_author(char* author_name) {}

void extract_author() {

}
