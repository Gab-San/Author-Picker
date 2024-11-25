#include <stdio.h>
#include <assert.h>

#include "author_picker.h"
#include "helper_lib.h"

#define A 2 
#define MENU ("\nWelcome to Author Picker:\n1.[I]nsert an author\n2.[E]xtract an author\n3.[Q]uit\n")
#define INPUT ("> ")

void flush_stdin() {
    while(getchar() != '\n' && getchar() != EOF);
}

int parse_input(char action, char* buf){
    assert('A' <= action && action <= 'z');

    switch (conv_to_lower(action))
    {
    case 'i':
        printf("Insert the name of the author: ");
        read_line(buf, AUTHOR_LEN);
        flush_stdin();
        printf("Inserting %s...\n", buf);
        insert_author(buf);
        return 1;
    case 'e':
        printf("Extracting...\n");
        extract_author();
        return 1;
    case 'q': return 0;
    default:
        printf("Selected action not valid: %c\n", action);
        return 1;
    }
}


int main(int argc, char** argv) {
    char action;
    char buffer[AUTHOR_LEN] = {'\0'};
    int running = 1;
    while (running) {
        printf(MENU);
        printf(INPUT);
        action = getchar() + '\0';
        flush_stdin();
        running = parse_input(action, buffer);
    }
    return 0;
}