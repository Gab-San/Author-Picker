#ifndef AUTHOR_PICKER_H
#define AUTHOR_PICKER_H

#define AUTHOR_LEN 50

/*
    Appends the given author's name to the file.

    # INPUT

    The author's name read from the user input.
*/
void insert_author(char* author_name);

/*
    Highlights a random author's name from those inserted into the file
    rewriting the name at the first line.
    The first line is formatted with the name of the author and the time and day 
    at which it was extracted.
*/
void extract_author();

// /*
//     Searches and removes from the file the selected author's name.
// */
// void remove_author(char* author_name);
// void read_authors();
// void find_author(char* author_name);

#endif