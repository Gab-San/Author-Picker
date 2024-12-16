#include <stdio.h>
#include <assert.h>

#include "author_picker.h"
#include "helper_lib.h"

#define A 2
#define MENU ("\nWelcome to Author Picker:\n-. [I]nsert an author\n-. [E]xtract an author\n-. [V]iew\n-. [R]emove\n-. [Q]uit\n")

int parse_input(char action){
    assert('A' <= action && action <= 'z');

    char buffer[AUTHOR_LEN] = {'\0'};

    switch (conv_to_lower(action))
    {
    case 'i':
        printf("Insert the name of the author: ");
        read_line(buffer, AUTHOR_LEN);
        str_to_lower(buffer);
        printf("Inserting %s...\n", buffer);
        insert_author(buffer);
        return 1;
    case 'e':
        printf("Extracting...\n");
        extract_author();
        return 1;
    case 'v':
        view();
        return 1;
    case 'r':
    printf("Insert the name of the author: ");
        read_line(buffer, AUTHOR_LEN);
        str_to_lower(buffer);
        printf("Remove %s...\n", buffer);
        remove_author(buffer);
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
    int running = 1;
    while (running) {
        printf(MENU);
        printf(INPUT);
        action = getchar() + '\0';
        flush_stdin();
        running = parse_input(action);
    }
    return 0;
}