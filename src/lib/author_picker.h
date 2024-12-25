#ifndef AUTHOR_PICKER_H
#define AUTHOR_PICKER_H

#define AUTHOR_LEN 50


/*
    # Insert
    Appends the given author's name to the file.

    ## Input
    author_name -> string to insert

    If the passed string has a null lenght (=0), this function
    notifies the user and returns without performing any further action.

    ## Deallocation
    This function doesn't deallocate the input as it cannot determine if it
    is heap or stack allocated
*/
void insert_author(char* author_name);

/*
    # Extract

    Highlights a random author's name from those inserted into the file
    rewriting the name at the first line.
    The first line is formatted with the name of the author and the time and day 
    at which it was extracted.
*/
void extract_author();

/*
    # Remove
    Searches and removes selected author's name.

    ## Input
    author_name -> string to remove

    If the given string has a null length (=0) then the function
    notifies the user and returns without performing any further action.

    If the author is not present the user is notified an the database remains unchanged.
*/
void remove_author(char* author_name);

/*
    # Find
    Searches the file for an entry matching 'author_name'.
    If a line matching the input is found then a check message will be displayed, otherwise a not found message will be displayed

    ## Input
    author_name -> name of the author to search
*/
void find_author(char* author_name);

/*
    # View

    Prints out all of the authors currently inserted
*/
void view();

#endif