#include "run_module.h"
#include "lib/author_picker.h"
#include "lib/ap_config.h"
#include "lib/gen_func.h"
#include "lib/string_helper.h"

#include <stdio.h>

/*
   # Read author name
   Reads the author name and inserts it into the buffer.

   ## Input
   buf -> character buffer to fill with the user input. 
*/
void read_auth_name(char* buf){
    printf("Insert the name of the author: ");
    read_line(buf, AUTHOR_LEN);
    str_to_lower(buf);
}


#define MENUTIZE_STR(X) "-. " #X
#define MENUTIZE(X) MENUTIZE_STR(X)
#define MENU ("\nWelcome to Author Picker:")
#define MENU_INSERT [I]nsert an author
#define MENU_EXTRACT [E]xtract an author
#define MENU_VIEW [V]iew
#define MENU_REMOVE [R]emove
#define MENU_FIND [F]ind
#define MENU_QUIT [Q]uit

/*
    # Print menu

    Prints the menu of the program.     
*/
void print_menu(){
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n",  MENU, 
                                        MENUTIZE(MENU_INSERT), 
                                        MENUTIZE(MENU_EXTRACT),
                                        MENUTIZE(MENU_FIND), 
                                        MENUTIZE(MENU_REMOVE), 
                                        MENUTIZE(MENU_VIEW), 
                                        MENUTIZE(MENU_QUIT));
}


/*
    # Parse input

    Parses the user input and calls the program functions.

    ## Input
    action -> user input character

    ## Returns
    An integer to evaluate whether to continue running or not.
*/
int parse_input(char action){
    // This should not be an assertion if an if is not implemented before calling this func
    if('A' > action && action > 'z'){
        printf("Selected action not valid!\n");
        return 1;
    }

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

void run(){
    check_config();
    char action;
    int running = 1;
    while (running) {
        print_menu();
        printf(INPUT);
        action = getchar() + '\0';
        if(action == '\n') continue;
        flush_stdin();
        running = parse_input(action);
    }
}