#include "author_picker.h"
#include "ap_config.h"
#include "gen_func.h"
#include "string_helper.h"
#include "file_helper.h"
#include "ap_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h> // truncate



typedef struct remaining_time {
    int pt;
    struct tm* expiration_date;
} rem_time_t;

const char datafile[] = "./db/author_db.txt";

#define FIRST_LINE_TEXT "LAST EXTRACTED: "
#define FIRST_LINE_OFFST 17

/*
    Creates file and compiles the first line.
*/
FILE* initialize_file(){
    FILE* fp = open_file(datafile, "w+", "initialize_file()");
    char first_line[FIRST_LINE_OFFST];
    strncpy(first_line, FIRST_LINE_TEXT, FIRST_LINE_OFFST);
    fwrite(first_line, len_of_str(first_line, FIRST_LINE_OFFST), 1, fp);
    fwrite(FORMATTED_TIME_STR, len_of_str(FORMATTED_TIME_STR, FORMATTED_TIME_LEN), 1, fp);
    fwrite("\n", 1 ,1 ,fp);
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
    int bytes_back = c == EOF ? bytes_read : (bytes_read + 1);
    fseek(fp, -1 * bytes_back, SEEK_CUR); // + 1 for '\n'
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
    b -= FORMATTED_TIME_LEN - 1; // Not counting the space after the name
    b = b == 0 ? 0 : b - 1; // Counting the space 
    fread(rbuf, b, 1, fp);
    rbuf[b] = '\0';
    // printf("[DEBUG:read_extracted_auth()] INPUT READ: %s\n", rbuf);
}

/*
    # Search author
    Searches for an equal author name.

    ## Output

    Returns -1 if there's no match, otherwise returns the line on which the author is located;
    the count starts at 0 being the first line (extraction line).

    ## Side effects
    Cursor is positioned at the start of the match line.
*/
int search_author(FILE* fp, char* auth){
    char input_read[AUTHOR_LEN] = {'\0'};
    int bytes_read;
    int line = 0;

    read_extracted_auth(fp, input_read);
    if( len_of_str(input_read, AUTHOR_LEN) == len_of_str(auth,AUTHOR_LEN) && 
        strncmp(input_read, auth, len_of_str(auth, AUTHOR_LEN)) == 0) return line;
    skip_first_line(fp);

    do{
        line++;
        bytes_read = estimate_read_length(fp);
        fread(input_read, bytes_read + 1, 1, fp);
        input_read[bytes_read] = '\0'; // SUBSTITUTE '\n' with '\0'

        // printf("INPUT: %s\n", input_read);
        if( len_of_str(input_read, AUTHOR_LEN) == len_of_str(auth, AUTHOR_LEN) &&
            strncmp(input_read, auth, len_of_str(auth,AUTHOR_LEN)) == 0 ){ 
                fseek(fp, -1*(bytes_read + 1), SEEK_CUR);
                return line;
            }
    }while(bytes_read != 0);

    return -1;
}   

#define MSG_INPUT_ERROR ("\n[INPUT ERROR]: the input has null length, please consider writing at least one character!\n")

void insert_author(char* author_name) {
    // At this point the buffer contains the name of the author
    // this means that 2 actions are to be performed:
    // 1. the file must be scanned to find if the author was already inserted
    //      A. the author was inserted: the functions notifies the user and returns
    //      B. the author wasn't previously inserted: proceed normally
    // 2. insert the name of the author at the end of the file
    // 
    // THE FIRST LINE OF THE FILE WILL CONTAIN THE AUTHOR THAT WAS LAST EXTRACTED

    char* auth_name = trim(author_name);

    if (len_of_str(auth_name, AUTHOR_LEN) == 0){
        fprintf(stderr, MSG_INPUT_ERROR);
        free(auth_name);
        return;
    }
    
    FILE* fp = fopen(datafile, "r+");
    
    if(fp == NULL)
        fp = initialize_file();    

    if( search_author(fp, auth_name) != -1 )
        printf("%s was previously inserted!\n", auth_name);
    else
        append_author(fp, auth_name);
    
    free(auth_name);
    close_file(fp, "insert_author()");
}

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
    fwrite(" ", 1, 1, fp);

    struct tm* ftime = get_current_time();

    char* str_ftime = ftime_to_string(ftime);
    fwrite(str_ftime, FORMATTED_TIME_LEN - 1 , 1, fp);
    free(str_ftime);
    fwrite("\n", 1, 1, fp);

    int written_bytes = FIRST_LINE_OFFST + len_of_str(buf,AUTHOR_LEN) + FORMATTED_TIME_LEN;
    truncate(datafile, written_bytes);
}

/*
    Saves the chunk of text from the desired lines.

    # INPUT
    starting_line -> line to start copying from (included)
    last_line -> stop copying at this line (excluded) 

    # RETURNS
    Returns a copy-buffer that contains information about
    the chunk of text read.
*/
cbuffer_t* copy_text_from_to(FILE* fp, int starting_line, int last_line){
    int bytes_read = skip_to_line_from(starting_line, last_line, fp);
    //GETTING BACK TO THE CORRECT LINE
    fseek(fp, -1 * bytes_read, SEEK_CUR);
    return copy_text(fp, bytes_read);
}

/*
    Writes back the information stored into the copy-buffers.
*/
void copy_back_txt(cbuffer_t* lines_before, cbuffer_t* lines_after, FILE* fp){
    skip_first_line(fp);
    fwrite(lines_before->buf, lines_before->bytes, 1, fp);
    fwrite(lines_after->buf, lines_after->bytes, 1, fp);
}

char* read_extraction_time(FILE* fp){
    char buf[AUTHOR_LEN];
    read_extracted_auth(fp, buf);
    char* ext_t = (char* ) malloc(FORMATTED_TIME_LEN);
    if(ext_t == NULL) fatal("in read_extraction_time() while allocating memory");
    // After the call to this function the file pointer is at
    // the start of the time string
    char c = fgetc(fp); // SKIPPING SPACE;
    if(c != ' ') fseek(fp, -1, 1); // In the first iteration there is no space to skip
    fread(ext_t, FORMATTED_TIME_LEN - 1, 1, fp);
    ext_t[FORMATTED_TIME_LEN - 1] = '\0';
    return ext_t;
}

rem_time_t* evaluate_time_passed(FILE* fp){
    
    char* ext_time_str = read_extraction_time(fp);
    if(!is_time(ext_time_str)){ 
        free(ext_time_str);
        return NULL;
    }

    struct tm* ftime = str_to_ftime(ext_time_str);
    free(ext_time_str);

    enum passed_time pt = read_config();

    rem_time_t* rem = (rem_time_t*) malloc(sizeof(rem_time_t));
    rem->pt = cmp_ftime(ftime, get_current_time(), pt);
    rem->expiration_date = calculate_expiration(ftime, pt);
    free(ftime);

    return rem;
}

void copy_file(FILE* fp, int rline, int reads, char* extracted){
    // Now the lines before and after the extracted lines must be copied
    cbuffer_t* cp_before_line = copy_text_from_to(fp, 1, rline);
    // Reads is equal to the number of inserted lines. In order to read the last 
    // author is necessary to count one more line after that.
    cbuffer_t* cp_after_line = copy_text_from_to(fp, rline + 1, reads + 1);
    // Writes the first line and truncates
    write_first_line(fp, extracted);
    // Writes back copied text
    copy_back_txt(cp_before_line, cp_after_line, fp);
    
    free(cp_before_line->buf);
    free(cp_before_line);
    free(cp_after_line->buf);
    free(cp_after_line);
}

void extract_author() {
    // At this point the file should exist
    FILE* fp = open_file(datafile, "r+", "extract_author()");

    rem_time_t* rt = evaluate_time_passed(fp);
    if(rt != NULL)
        if(rt->pt == 0){
            char* exp_date = ftime_to_string(rt->expiration_date);
            char curr_auth[AUTHOR_LEN];
            read_extracted_auth(fp, curr_auth);
            printf("Cannot extract another author until %s\nCurrent extracted author: %s\n", exp_date, curr_auth);
            free(rt);
            free(exp_date);
            return;
        }
    free(rt);

    int reads = num_of_authors(fp); 

    if(reads == 0){
        printf("There are no authors to extract.\nInsert some!");
        return;
    } 
    
    // chooses a random line.
    int rline = gen_limited_randint(reads) + 1;
    go_to_line(rline, fp);
    // At this point the file pointer points at the
    // start of the line to extract

    int bytes_read = estimate_read_length(fp);
    char* extracted = (char*) malloc(sizeof(char) * bytes_read + 1);
    if(extracted == NULL) fatal("in extract_author() while trying to allocate memory for extracted");
    fread(extracted, bytes_read + 1, 1, fp);
    extracted[bytes_read] = '\0';

    copy_file(fp, rline, reads, extracted);

    printf("%s extracted!\n", extracted);
    free(extracted);
    close_file(fp, "extract_author()");
}

// The options are "all" and "ext" both 3 letters
#define VIEW_ACTION_LEN 5
#define VIEW_DIALOG ("-. [ALL] AUTHORS\n-. [EXT]RACTED AUTHOR\n-. [BACK]\n")
#define PRINT_INPUT (": ")

typedef enum bool{true = 1, false = 0} boolean;

void print_insauth_loop(FILE* fp) {

    char input_read[AUTHOR_LEN + 1];
    int line = 1;
    int bytes;
    char cont = '\n';

    while(conv_to_lower(cont) != 'q'){
        int count = 0;
        const int max_count = 20;

        do{
            bytes = estimate_read_length(fp);
            fread(input_read, bytes + 1, 1, fp);
            input_read[bytes] = '\0';
            if(len_of_str(input_read, AUTHOR_LEN + 1) > 0) printf("%d: %s\n", line, input_read);
            count++;
            line++;
        }while(bytes != 0 && count < max_count);

        if(bytes == 0) break; // If no more bytes are read it means that the file is at end.
        
        printf(PRINT_INPUT);
        cont = getchar() + '\0';
        if(cont == '\n') continue;
        flush_stdin();
    }
}

void print_inserted_authors(){
    FILE* fp = open_file(datafile, "r", "print_inserted_authors()");

    skip_first_line(fp);
    printf("\n__INSERTED AUTHORS__\n");

    print_insauth_loop(fp);

    close_file(fp, "print_inserted_authors()");
}

void print_extracted_author(){
    FILE* fp = open_file(datafile, "r", "print_extracted_author()");
    char input_read[AUTHOR_LEN + 1];

    read_extracted_auth(fp, input_read);
    
    if(len_of_str(input_read, AUTHOR_LEN) == 0){
        printf("No author extracted!\n");
        return;
    }

    rem_time_t* rt = evaluate_time_passed(fp);
    char* exp_t = ftime_to_string(rt->expiration_date);

    printf("The current extracted author is: %s\nuntil %s\n", input_read, exp_t);

    free(exp_t);
    free(rt);
    close_file(fp, "print_extracted_author()");
}

void view(){
    char view_action[VIEW_ACTION_LEN] = {'\0'};
    boolean valid = true;

    do {
        printf(VIEW_DIALOG);
        printf(INPUT);
        read_line(view_action, VIEW_ACTION_LEN);
        str_to_lower(view_action);

        if(strcmp("all", view_action) == 0){
            print_inserted_authors();
            return;
        } else if (strcmp("ext", view_action) == 0) {
            print_extracted_author();
            return;
        } else if (strcmp("back", view_action) == 0) {
            return;
        } else {
            valid = false;
        }

        if(!valid) printf("ERROR: Input not valid!\n");

    }while(!valid);
}


int bytes_to_end(FILE* fp){
    int bytes_read = 0;
    while(fgetc(fp) != EOF){
        bytes_read++;
    }
    fseek(fp, -1*bytes_read, SEEK_CUR);
    return bytes_read;
}

void remove_author(char* author_name){
    
    char* auth_name = trim(author_name);

    if(len_of_str(auth_name, AUTHOR_LEN) == 0) {
        fprintf(stderr, MSG_INPUT_ERROR);
        free(auth_name);
        return;
    }

    FILE* fp = open_file(datafile, "r+", "remove_author()");
    
    int line;
    if((line = search_author(fp, auth_name)) == -1){
        printf("Author not found!\n\nBe sure to write the name as inserted!\n");
        free(auth_name);
        return;
    }

    if(line == 0){ // Reading extracted author
        printf("Cannot remove the extracted author!\n");
        free(auth_name);
        return;
    }
    
    int auth_bytes = estimate_read_length(fp);
    fseek(fp, auth_bytes + 1, SEEK_CUR);


    int cpy_bytes = bytes_to_end(fp);
    cbuffer_t* lines_after = copy_text(fp, cpy_bytes);

    fseek(fp, 0, SEEK_SET);
    int bytes_file_size = bytes_to_end(fp); // FIRST_LINE_LEN: 17 + 20 + auth_len<=50
    int bytes_to_save =  bytes_file_size - (auth_bytes + 1 + cpy_bytes);
    truncate(datafile, bytes_to_save);
    fseek(fp, bytes_to_save, SEEK_SET);
    fwrite(lines_after->buf, lines_after->bytes, 1, fp);
    free(lines_after->buf);
    free(lines_after);

    free(auth_name);
    
    close_file(fp, "remove_author()");
}

void find_author(char* author_name) {

    char* auth_name = trim(author_name);

    if(len_of_str(auth_name, AUTHOR_LEN) == 0){
        fprintf(stderr, MSG_INPUT_ERROR);
        free(auth_name);
        return;
    }

    FILE* fp = open_file(datafile, "r", "find_author()");

    int line;

    if((line = search_author(fp, auth_name)) != -1)
        printf("Author found at line %d!\n", line);
    else
        printf("Author not found!\n");
    
    free(auth_name);
}