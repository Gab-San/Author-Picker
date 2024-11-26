#include "author_picker.h"
#include "helper_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>


#define ERR_MSG_LEN 100
#define READ_LEN 100

const char* datafile = "./out/author_db.txt";

void fatal(const char* msg){
    char error_message[ERR_MSG_LEN];
    strcat(error_message, "[!!] Fatal error:");
    strncat(error_message, msg, ERR_MSG_LEN);
    perror(error_message);
    exit(-1);
}

void append_author(FILE* fp, char* auth_name){
    fseek(fp, 0, SEEK_END);
    fwrite(auth_name, len_of_str(auth_name,AUTHOR_LEN), 1, fp);
    fwrite("\n", 1, 1, fp);
    printf("%s inserted!\n", auth_name);
}

int estimate_read_length(FILE* fp) {
    int bytes_read = 0;
    char c;
    while( (c = fgetc(fp)) != '\n' && c != (EOF)) bytes_read++;
    printf("[DEBUG:estimate_read_length()] bytes_read: %d\n", bytes_read);
    fseek(fp, -1 * (bytes_read + 1), SEEK_CUR); // + 1 for '\n'
    return bytes_read;
}

void debug_string(char* buf){
    int i = 0;
    for(; buf[i] != '\0' && buf[i] != '\n'; i++) printf("%d\t", buf[i]);
    printf("%d\n", buf[i]);
}

int search_author(FILE* fp, char* auth){
    // MISSING OFFSET OF THE FIRST LINE

    char input_read[AUTHOR_LEN + 1] = {'\0'};
    int bytes_read;
    fseek(fp, 0, SEEK_SET);
    do{
        bytes_read = estimate_read_length(fp);
        fread(input_read, bytes_read + 1, 1, fp);
        input_read[bytes_read] = '\0'; // SUBSTITUTE '\n' with '\0'
        printf("INPUT: %s\n", input_read);
        // ALL OF THE SUBSTRING OF INPUT_READ ARE CONSIDERED EQUAL aaa == aa
        if( len_of_str(input_read, AUTHOR_LEN + 1) == len_of_str(auth, AUTHOR_LEN) && strncmp(input_read, auth, len_of_str(auth,AUTHOR_LEN)) == 0 ){
            return 1;
        }
    }while(bytes_read != 0);

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
    int bytes_read;
    char input_read[AUTHOR_LEN + 1];

    do{
        input_read[0] = '\0';
        bytes_read = estimate_read_length(fp);
        fread(input_read, bytes_read + 1, 1, fp);
        input_read[bytes_read] = '\0';
        counter += bytes_read == 0 ? 0 : 1;
        printf("[DEBUG] Items read: %d\t Input: %s\n", counter, input_read);
    }while(bytes_read != 0);

    return counter;
}

void skip_to_line(int line, FILE* fp) {
    fseek(fp, 0, SEEK_SET);
    int count = 0;
    char c;
    while(count < line){
        while((c = fgetc(fp)) != '\n' && c != EOF);
        count++;
    }
}

void overwrite_blank(FILE* fp, int bytes){
    char* blanks = (char* ) malloc(bytes + 1);
    int i;
    for(i = 0; i < bytes + 1; i++){
        blanks[i] = ' ';
    }
    blanks[bytes] = '\0';
    fwrite(blanks, bytes, 1, fp);
    free(blanks);
    fseek(fp, -1 * (bytes + 1), SEEK_CUR);
}

void extract_author() {
    // At this point the file should exist
    FILE* fp = fopen(datafile, "r+");
    if (fp == NULL) {
        fatal("in extract_author() while opening file");
    }

    int reads = num_of_authors(fp);

    assert(reads != 0);
    

    srand(time(NULL));
    int r = rand() % reads;

    skip_to_line(r, fp);

    int bytes_read = estimate_read_length(fp);
    char* extracted = (char*) malloc(sizeof(char) * bytes_read + 1);
    fread(extracted, bytes_read + 1, 1, fp);
    extracted[bytes_read] = '\0';

    printf("[DEBUG:extract_author()] EXTRACTED %s FROM LINE %d\n", extracted, r + 1);
    // WHAT IS WRITTEN ON LINE 0 MUST BE SAVED THE FIRST ITERATION (WILL BE FIXED)

    skip_to_line(0, fp);
    bytes_read = estimate_read_length(fp);
    char* save = (char*) malloc(bytes_read + 1);
    fread(save, bytes_read+1, 1, fp);
    save[bytes_read] = '\0';
    fseek(fp, -1 * (bytes_read + 1), SEEK_CUR);

    overwrite_blank(fp, bytes_read);

    char formatted_write[100];
    strcpy(formatted_write, "LAST EXTRACTED: ");
    strncat(formatted_write, extracted, len_of_str(extracted, AUTHOR_LEN));
    printf("%s", formatted_write);
    fwrite(formatted_write, len_of_str(formatted_write, READ_LEN), 1, fp);
    free(extracted);

    skip_to_line(r,fp);
    fwrite(save, len_of_str(save, AUTHOR_LEN), 1, fp);
    fwrite("\n", 2, 1, fp);
    free(save);
    fclose(fp);
}

void overwrite(){
    FILE* fp = fopen(datafile, "r+");
    fwrite("\0", 1, 1, fp);
    fclose(fp);
}