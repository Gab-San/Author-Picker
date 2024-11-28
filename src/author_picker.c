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

// Basically somewhat of a string: it makes it easier to do operations of copying and writing
typedef struct char_buffer { 
    char* buf;
    int bytes;
} cbuffer_t;

const char datafile[20] = "./out/author_db.txt";

/*
    Creates file and compiles the first line.
*/
FILE* initialize_file(){
    FILE* fp = fopen(datafile, "w+");
    if(fp == NULL) fatal("in initialize_file() while opening file.");
    char first_line[FIRST_LINE_OFFST];

    strncpy(first_line, FIRST_LINE_TEXT, FIRST_LINE_OFFST);
    first_line[FIRST_LINE_OFFST - 1] = '\n'; // Subst \0
    fwrite(first_line, FIRST_LINE_OFFST, 1, fp);
    return fp;
}


/*
    Returns the bytes read from current position of the 
    cursor to the end of the line.

    At the end of this function the cursor will be at its starting position.

    # RETURNS
    Returns the bytes read from the current position to the end of the line (newline excluded)
*/
int estimate_read_length(FILE* fp) {
    int bytes_read = 0;
    char c;
    while( (c = fgetc(fp)) != '\n' && c != (EOF)) bytes_read++;
    fseek(fp, -1 * (bytes_read + 1), SEEK_CUR); // + 1 for '\n'
    return bytes_read;
}

/*
    Moves the cursor to the end of the first line.
    
    The first line is the one that states which name
    was last extracted.

    # RETURNS
    Returns the bytes read on the first line (newline included).
*/
int skip_first_line(FILE* fp){
    fseek(fp, FIRST_LINE_OFFST - 1, SEEK_SET);  // The last char in the offset is either the '\n' (discarded by estimate_read_length)
                                                // or is the first letter of the first-line formatted write 
    int bytes_read = estimate_read_length(fp);
    fseek(fp, bytes_read + 1, SEEK_CUR); // + 1 counting for '\n'
    return bytes_read + FIRST_LINE_OFFST;
}

/*
    Moves the file cursor to the start of the requested line.
    The first line is always skipped, this means that there's an offset of 1.
    
    # INPUT
    Line to reach offset by one. Line 0 is effectively line 1.

    # RETURNS
    The value of bytes read (including newline) 
*/
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

/*
    Starting from the from_line moves the cursor to the start of the to_line.

    # INPUT
    from_line: the line from which the cursor will start.
    to_line: the line from which the cursor will be positioned at (start).
    
    ## PREREQUISITE
    from_line <= to_line

    # RETURNS
    Returns the number of bytes read during this operation (including newlines)
*/
int skip_to_line_from(int from_line, int to_line, FILE* fp){
    int count = from_line;
    int bytes_read = 0;
    char c;
    while(count < to_line){
        while((c = fgetc(fp)) != '\n'){
            if(c == EOF) break;
            bytes_read++;
        }
        bytes_read++; // Counting newlines
        count++;
    }
    return bytes_read;
}

/*
    Appends the author's name to the end of the file.
*/
void append_author(FILE* fp, char* auth_name){
    fseek(fp, 0, SEEK_END);
    fwrite(auth_name, len_of_str(auth_name,AUTHOR_LEN), 1, fp);
    fwrite("\n", 1, 1, fp);
    printf("%s inserted!\n", auth_name);
}

void read_extracted_auth(FILE* fp,char* rbuf){
    fseek(fp, FIRST_LINE_OFFST - 1, SEEK_SET);
    int b = estimate_read_length(fp);
    fread(rbuf, b + 1, 1, fp);
    rbuf[b] = '\0';
    printf("[DEBUG:read_extracted_auth()] INPUT READ: %s\n", rbuf);
}

/*
    Searches for an equal author name.

    # RETURNS
    Returns 0 if there's no match, otherwise 1.
*/
int search_author(FILE* fp, char* auth){
    char input_read[AUTHOR_LEN] = {'\0'};
    int bytes_read;
    read_extracted_auth(fp, input_read);

    if( len_of_str(input_read, AUTHOR_LEN) == len_of_str(auth,AUTHOR_LEN) && 
        strncmp(input_read, auth, len_of_str(auth, AUTHOR_LEN)) == 0) return 1;

    skip_first_line(fp);

    do{
        bytes_read = estimate_read_length(fp);
        fread(input_read, bytes_read + 1, 1, fp);
        input_read[bytes_read] = '\0'; // SUBSTITUTE '\n' with '\0'
        printf("INPUT: %s\n", input_read);
        // ALL OF THE SUBSTRING OF INPUT_READ ARE CONSIDERED EQUAL aaa == aa
        if( len_of_str(input_read, AUTHOR_LEN) == len_of_str(auth, AUTHOR_LEN) && 
            strncmp(input_read, auth, len_of_str(auth,AUTHOR_LEN)) == 0 ) return 1;
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

    assert( strlen(author_name) != 0 ); // This has to be changed into an print to screen and early return

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
        fatal("in insert_author() while closing file");
    }
}

void remove_author(char* author_name){

}

// void read_authors(){}
// void find_author(char* author_name) {}

/*
    Evaluates the number of authors inserted in the file.

    # RETURNS
    Number of inserted authors.
*/
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
        // The number of authors depends on the number of 
        // not empty lines in the file. 
        counter += bytes_read == 0 ? 0 : 1;
    }while(bytes_read != 0);

    return counter;
}

/*
    Writes the first line in the correct format
*/
void write_first_line(FILE* fp, char* buf){
    fseek(fp, FIRST_LINE_OFFST-1, SEEK_SET);
    fwrite(buf, len_of_str(buf, AUTHOR_LEN), 1, fp);
    fwrite("\n", 1, 1, fp);
    int written_bytes = FIRST_LINE_OFFST + len_of_str(buf,AUTHOR_LEN);
    truncate(datafile, written_bytes);
}

/*
    Saves the chunk of text from the desired lines.

    # RETURNS
    Returns a copy-buffer that contains information about
    the chunk of text read.
*/
cbuffer_t* copy_text(FILE* fp, int starting_line, int n_line){
    go_to_line(starting_line, fp);

    // At this point the program is in extracted-line + 1
    // The program must copy all of the remaining lines
    // The number of bytes read from the starting line to the end line 
    // is the return value of the called func
    int bytes_read = skip_to_line_from(starting_line, n_line, fp);
    //GETTING BACK TO THE CORRECT LINE
    fseek(fp, -1 * bytes_read, SEEK_CUR);
    cbuffer_t* cp_buf = (cbuffer_t*) malloc(sizeof(cbuffer_t));
    cp_buf->buf = (char*) malloc(bytes_read);
    cp_buf->bytes = bytes_read;
    fread(cp_buf->buf, cp_buf->bytes, 1, fp);
    return cp_buf;
}

/*
    Writes back the information stored into the copy-buffers.
*/
void copy_back_txt(cbuffer_t* lines_before, cbuffer_t* lines_after, FILE* fp){
    skip_first_line(fp);
    fwrite(lines_before->buf, lines_before->bytes, 1, fp);
    fwrite(lines_after->buf, lines_after->bytes, 1, fp);
}

void extract_author() {
    // At this point the file should exist
    FILE* fp = fopen(datafile, "r+");
    if (fp == NULL) {
        fatal("in extract_author() while opening file");
    }

    int reads = num_of_authors(fp); 

    assert(reads != 0); // This has to be changed into a print to screen and early return

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
    // Now the lines before and after the extracted lines are copied
    cbuffer_t* cp_before_line = copy_text(fp, 0, r);
    cbuffer_t* cp_after_line = copy_text(fp, r + 1, reads);
    // Writes the first line and truncates
    write_first_line(fp, extracted);
    // Writes back copied text
    copy_back_txt(cp_before_line, cp_after_line, fp);
    
    free(cp_before_line->buf);
    free(cp_before_line);
    free(cp_after_line->buf);
    free(cp_after_line);

    printf("%s extracted!\n", extracted);
    free(extracted);
    if(fclose(fp) != 0) fatal("in extract_author() while closing file");
}