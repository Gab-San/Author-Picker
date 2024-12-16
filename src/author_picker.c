#include "author_picker.h"
#include "helper_lib.h"
#include "ap_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

typedef enum bool{true = 1, false = 0} boolean;

#define FIRST_LINE_TEXT "LAST EXTRACTED: "
#define FIRST_LINE_OFFST 17

typedef struct remaining_time {
    int pt;
    struct tm* expiration_date;
} rem_time_t;

// Basically somewhat of a string: it makes it easier to do operations of copying and writing
typedef struct char_buffer { 
    char* buf;
    int bytes;
} cbuffer_t;

const char datafile[] = "./db/author_db.txt";

/*
    Creates file and compiles the first line.
*/
FILE* initialize_file(){
    FILE* fp = fopen(datafile, "w+");
    if(fp == NULL) fatal("in initialize_file() while opening file.");
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
    b -= FORMATTED_TIME_LEN - 1; // Not counting the space after the name
    b = b == 0 ? 0 : b - 1; // Counting the space 
    fread(rbuf, b, 1, fp);
    rbuf[b] = '\0';
    // printf("[DEBUG:read_extracted_auth()] INPUT READ: %s\n", rbuf);
}

/*
    Searches for an equal author name.

    # RETURNS
    Returns 0 if there's no match, otherwise 1.
*/
boolean search_author(FILE* fp, char* auth){
    char input_read[AUTHOR_LEN] = {'\0'};
    int bytes_read;
    read_extracted_auth(fp, input_read);

    if( len_of_str(input_read, AUTHOR_LEN) == len_of_str(auth,AUTHOR_LEN) && 
        strncmp(input_read, auth, len_of_str(auth, AUTHOR_LEN)) == 0) return true;

    skip_first_line(fp);

    do{
        bytes_read = estimate_read_length(fp);
        fread(input_read, bytes_read + 1, 1, fp);
        input_read[bytes_read] = '\0'; // SUBSTITUTE '\n' with '\0'
        // printf("INPUT: %s\n", input_read);
        // ALL OF THE SUBSTRING OF INPUT_READ ARE CONSIDERED EQUAL aaa == aa
        if( len_of_str(input_read, AUTHOR_LEN) == len_of_str(auth, AUTHOR_LEN) && 
            strncmp(input_read, auth, len_of_str(auth,AUTHOR_LEN)) == 0 ) return true;
    }while(bytes_read != 0);

    return false;
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
    char* auth_name = trim(author_name);
    
    FILE* fp = fopen(datafile, "r+");
    if(fp == NULL){
        fp = initialize_file();
    }

    

    if( search_author(fp, auth_name) == true ){
        printf("%s was previously inserted!\n", auth_name);
        free(auth_name);
        return;
    }

    append_author(fp, auth_name);
    free(auth_name);

    if(fclose(fp) != 0) {
        fatal("in insert_author() while closing file");
    }
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
    fwrite(str_ftime, len_of_str(str_ftime, MAX_STR_LEN) , 1, fp);
    free(str_ftime);
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
    if(cp_buf == NULL) fatal("in copy_text() while trying to allocate memory for cp_buf");
    cp_buf->buf = (char*) malloc(bytes_read);
    if(cp_buf->buf == NULL) fatal("in copy_text() while trying to allocate memory for cp_buf->buf");
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

enum passed_time read_config(){
    FILE* fconfig = fopen(get_config_file(), "r");
    if(fconfig == NULL) fatal("in evaluate_time_passed() while opening config file");
    
    char cfg;
    fread(&cfg, 1, 1, fconfig);
    enum passed_time pt;
    switch (cfg){
        case 'h':   pt = HOUR; 
                    break;
        case 'd':   pt = DAY;
                    break;
        case 'w':   pt = WEEK;
                    break;
        case 'm':   pt = MONTH;
                    break;
        default:    fclose(fconfig);
                    setup_config("It looks like there is something wrong with the config file.\nLet's set it up again!\n");
                    return read_config();
    }
    if( fclose(fconfig) != 0 ) fatal("in read_config() while closing config file");
    return pt;
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


void extract_author() {
    // At this point the file should exist
    FILE* fp = fopen(datafile, "r+");
    if (fp == NULL) {
        fatal("in extract_author() while opening file");
    }

    rem_time_t* rt = evaluate_time_passed(fp);
    if(rt != NULL)
        if(rt->pt == 0){
            char* exp_date = ftime_to_string(rt->expiration_date);
            printf("Cannot extract another author until %s", exp_date);
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
    int rline = gen_limited_randint(reads);
    go_to_line(rline, fp);
    // At this point the file pointer points at the
    // start of the line to extract

    int bytes_read = estimate_read_length(fp);
    char* extracted = (char*) malloc(sizeof(char) * bytes_read + 1);
    if(extracted == NULL) fatal("in extract_author() while trying to allocate memory for extracted");
    fread(extracted, bytes_read + 1, 1, fp);
    extracted[bytes_read] = '\0';

    // Now the lines before and after the extracted lines are copied
    cbuffer_t* cp_before_line = copy_text(fp, 0, rline);
    cbuffer_t* cp_after_line = copy_text(fp, rline + 1, reads);
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

// The options are "all" and "ext" both 3 letters
#define VIEW_ACTION_LEN 5
#define VIEW_DIALOG ("-. [ALL] AUTHORS\n-. [EXT]RACTED AUTHOR\n-. [BACK]\n")
#define PRINT_INPUT (": ")


void print_inserted_authors(){
    FILE* fp = fopen(datafile, "r");
    if (fp == NULL){
        fatal("in print_inserted_authors() while opening file");
    }

    char input_read[AUTHOR_LEN + 1];
    int bytes = 1;
    char cont = '\n';
    skip_first_line(fp);
    printf("\n__INSERTED AUTHORS__\n");
    while(conv_to_lower(cont) != 'q' && bytes != 0){
        int count = 0;
        const int max_count = 20;
        do{
            bytes = estimate_read_length(fp);
            fread(input_read, bytes + 1, 1, fp);
            input_read[bytes] = '\0';
            if(len_of_str(input_read, AUTHOR_LEN + 1) > 0) printf("%s\n", input_read);
            count++;
        }while(bytes != 0 && count < max_count);

        if(bytes == 0) continue;
        
        printf(PRINT_INPUT);
        cont = getchar() + '\0';
        if(cont == '\n') continue;
        flush_stdin();
    }

    if( fclose(fp) != 0) fatal("in print_inserted_authors() while closing file");
}

void print_extracted_author(){
    FILE* fp = fopen(datafile, "r");
    if(fp == NULL) fatal("in print_extracted_author() while opening file");
    char input_read[AUTHOR_LEN + 1];
    read_extracted_auth(fp, input_read);
    rem_time_t* rt = evaluate_time_passed(fp);
    char* exp_t = ftime_to_string(rt->expiration_date);
    printf("The current extracted author is: %s\nuntil %s\n", input_read, exp_t);
    free(exp_t);
    free(rt);
    if( fclose(fp) != 0 ) fatal("in print_extracted_author() while closing file"); 
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