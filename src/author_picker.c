#include "author_picker.h"
#include "helper_lib.h"
#include "ap_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#define READ_LEN 100
#define FIRST_LINE_TEXT "LAST EXTRACTED: "
#define FIRST_LINE_OFFST 17

struct first_line_cnt { //CNT SHORT FOR CONTENT
    char* auth_name;
    time_format_t time;
};

const char datafile[20] = "./out/author_db.txt";

int estimate_read_length(FILE* fp) {
    int bytes_read = 0;
    char c;
    while( (c = fgetc(fp)) != '\n' && c != (EOF)) bytes_read++;
    fseek(fp, -1 * (bytes_read + 1), SEEK_CUR); // + 1 for '\n'
    return bytes_read;
}

int skip_first_line(FILE* fp){
    fseek(fp, FIRST_LINE_OFFST - 1, SEEK_SET);  // The last char in the offset is either the '\n' (discarded by estimate_read_length)
                                                // or is the first letter of the first-line formatted write 
    int bytes_read = estimate_read_length(fp);
    fseek(fp, bytes_read + 1, SEEK_CUR); // + 1 counting for '\n'
    return bytes_read + FIRST_LINE_OFFST;
}

int go_to_line(int line, FILE* fp) {
    int total_bytes_read = skip_first_line(fp);
    int count = 0;
    char c;

    while(count < line){

        while((c = fgetc(fp)) != '\n'){
            if(c == EOF) break;

            total_bytes_read++;
        }

        total_bytes_read++; // Counting for '\n'
        count++;
    }

    return total_bytes_read;
}

int skip_to_line_from(int from_line, int to_line, FILE* fp){
    int bytes_read = 0;
    int count = 0;
    char c;
    while(count < (to_line - from_line)) {

        while((c = fgetc(fp)) != '\n'){
            if (c == EOF) break;
            printf("%c\t", c);
            bytes_read++;
        }
        printf("%c\n", c);
        bytes_read++;
        count++;
    }

    return bytes_read;
}

FILE* initialize_file(){
    FILE* fp = fopen(datafile, "w+");
    if(fp == NULL) fatal("in initialize_file() while opening file.");
    char first_line[FIRST_LINE_OFFST];

    strncpy(first_line, FIRST_LINE_TEXT, FIRST_LINE_OFFST);
    first_line[FIRST_LINE_OFFST - 1] = '\n'; // Subst \0
    fwrite(first_line, FIRST_LINE_OFFST, 1, fp);
    return fp;
}

void append_author(FILE* fp, char* auth_name){
    fseek(fp, 0, SEEK_END);
    fwrite(auth_name, len_of_str(auth_name,AUTHOR_LEN), 1, fp);
    fwrite("\n", 1, 1, fp);
    printf("%s inserted!\n", auth_name);
}

int search_author(FILE* fp, char* auth){
    char input_read[AUTHOR_LEN + 1] = {'\0'};
    int bytes_read;
    // OFFSET OF THE FIRST LINE
    skip_first_line(fp);

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

    FILE* fp = fopen(datafile, "r+");
    if(fp == NULL){
        fp = initialize_file();
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
    skip_first_line(fp);

    int counter = 0;
    int bytes_read;
    char input_read[AUTHOR_LEN + 1];

    do{
        bytes_read = estimate_read_length(fp);
        fread(input_read, bytes_read + 1, 1, fp);
        input_read[bytes_read] = '\0';
        counter += bytes_read == 0 ? 0 : 1;

    }while(bytes_read != 0);

    return counter;
}

void write_first_line(FILE* fp, char* buf){
    fseek(fp, FIRST_LINE_OFFST-1, SEEK_SET);
    fwrite(buf, len_of_str(buf, AUTHOR_LEN), 1, fp);
    fwrite("\n", 1, 1, fp);
}

void replace_line(FILE* fp, int line, int replaced_line_bytes){
    int first_line_bytes = skip_first_line(fp);
    int tot_bytes_read = go_to_line(line, fp);
    int bytes_read_to_line = tot_bytes_read - first_line_bytes;
    // GETTING BACK TO LINE 1
    fseek(fp, -1 * bytes_read_to_line, SEEK_CUR);
    char* buf = (char* ) malloc(bytes_read_to_line);
    fread(buf, bytes_read_to_line, 1, fp);
    // GETTING BACK TO LINE 1
    fseek(fp, -1 * bytes_read_to_line, SEEK_CUR);
    // OFFSETTING BY THE BYTES THAT WILL BE OCCUPIED IN THE FIRST LINE
    fseek(fp, replaced_line_bytes, SEEK_CUR);
    fwrite(buf, bytes_read_to_line, 1, fp);
    free(buf);
}

void copy_text(FILE* fp, int starting_line, int n_line){
    go_to_line(starting_line, fp);

    // At this point the program is in extracted-line + 1
    // The program must copy all of the remaining lines
    // The number of bytes read from the starting line to the end line 
    // is the return value of the called func
    int bytes_read = skip_to_line_from(starting_line, n_line, fp);
    //GETTING BACK TO THE CORRECT LINE
    fseek(fp, -1 * bytes_read, SEEK_CUR);

    char* cp_buf = (char*) malloc(bytes_read);
    fread(cp_buf, bytes_read, 1, fp);

    go_to_line(starting_line - 1, fp);
    
    fwrite(cp_buf, bytes_read, 1, fp);
    free(cp_buf);
    bytes_read = go_to_line(n_line, fp);

    truncate(datafile, bytes_read - 1); // should cancel the last '\n' meaning a byte
}

void extract_author() {
    // At this point the file should exist
    FILE* fp = fopen(datafile, "r+");
    if (fp == NULL) {
        fatal("in extract_author() while opening file");
    }

    int reads = num_of_authors(fp);

    assert(reads != 0);

    // chooses a random line.
    srand(time(NULL));
    int r = rand() % reads;
    go_to_line(r, fp);
    // At this point the file pointer points at the
    // start of the line to extract

    int bytes_read = estimate_read_length(fp);
    char* extracted = (char*) malloc(sizeof(char) * bytes_read + 1);
    fread(extracted, bytes_read + 1, 1, fp);
    extracted[bytes_read] = '\0';
    replace_line(fp, r, bytes_read);
    write_first_line(fp, extracted);
    copy_text(fp, r + 1, reads);
    free(extracted);

    fclose(fp);
}