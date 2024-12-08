#include <stdio.h>
#include <assert.h>

#include "author_picker.h"
#include "helper_lib.h"

#define A 2
#define MENU ("\nWelcome to Author Picker:\n-. [I]nsert an author\n-. [E]xtract an author\n-. [V]iew\n-. [Q]uit\n")

int parse_input(char action, char* buf){
    assert('A' <= action && action <= 'z');

    switch (conv_to_lower(action))
    {
    case 'i':
        printf("Insert the name of the author: ");
        read_line(buf, AUTHOR_LEN);
        str_to_lower(buf);
        printf("Inserting %s...\n", buf);
        insert_author(buf);
        return 1;
    case 'e':
        printf("Extracting...\n");
        extract_author();
        return 1;
    case 'v':
        view();
        return 1;
    case 'q': return 0;
    default:
        printf("Selected action not valid: %c\n", action);
        return 1;
    }
}

void check_config(){
    FILE* config = fopen(get_config_file(), "r");
    if(config != NULL) return;
    setup_config(NULL);
}

int main(int argc, char** argv) {
    check_config();
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