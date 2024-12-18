#include <stdio.h>
#include <assert.h>

#include "author_picker.h"
#include "helper_lib.h"

#define A 2

#define MENUTIZE_STR(X) "-. " #X
#define MENUTIZE(X) MENUTIZE_STR(X)
#define MENU ("\nWelcome to Author Picker:")
#define MENU_INSERT [I]nsert an author
#define MENU_EXTRACT [E]xtract an author
#define MENU_VIEW [V]iew
#define MENU_REMOVE [R]emove
#define MENU_FIND [F]ind
#define MENU_QUIT [Q]uit


void read_auth_name(char* buf){
    printf("Insert the name of the author: ");
    read_line(buf, AUTHOR_LEN);
    str_to_lower(buf);
}


int parse_input(char action){
    assert('A' <= action && action <= 'z');

    char buf[AUTHOR_LEN] = {'\0'};

    switch (conv_to_lower(action)){
        case 'i':
            read_auth_name(buf);
            printf("Inserting %s...\n", buf);
            insert_author(buf);
            return 1;

        case 'e':
            printf("Extracting...\n");
            extract_author();
            return 1;

        case 'r':
            read_auth_name(buf);
            printf("Remove %s...\n", buf);
            remove_author(buf);
            return 1;

        case 'f':
            read_auth_name(buf);
            printf("Searching for %s...\n", buf);
            find_author(buf);
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

void print_menu(){
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n",  MENU, 
                                        MENUTIZE(MENU_INSERT), 
                                        MENUTIZE(MENU_EXTRACT),
                                        MENUTIZE(MENU_FIND), 
                                        MENUTIZE(MENU_REMOVE), 
                                        MENUTIZE(MENU_VIEW), 
                                        MENUTIZE(MENU_QUIT));
}

int main(int argc, char** argv) {
    check_config();
    char action;
    int running = 1;
    while (running) {
        print_menu();
        printf(INPUT);
        action = getchar() + '\0';
        flush_stdin();
        running = parse_input(action);
    }
    return 0;
}