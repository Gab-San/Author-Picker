#include <stdio.h>
#include <assert.h>

#include "author_picker.h"
#include "helper_lib.h"

#define N 50
#define A 2 
#define DIFF (97-65)
#define MENU ("Welcome to Author Picker:\n1.[I]nsert an author\n2.[E]xtract an author\n3.[Q]uit\n")
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
        read_line(buf, N);
        flush_stdin();
        printf("LEN: %d\tBUF: %s\n", len_of_str(buf, N), buf);
        printf("Inserting...\n");
        insert(buf);
        return 1;
    case 'e':
        printf("Extracting...");
        extract();
        return 1;
    case 'q': return 0;
    default:
        printf("Selected action not valid: %c\n", action);
        return 1;
    }
}


int main(int argc, char** argv) {

    char action;
    char buffer[N];
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