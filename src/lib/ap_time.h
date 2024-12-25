#ifndef AP_TIME_H
#define AP_TIME_H

#define MAX_STR_LEN 32
#define FORMATTED_TIME_LEN 20
#define FORMATTED_TIME_STR "dd/mm/yyyy hh:mm:ss"

/*
    This enumeration quantifies time that has to pass.
*/
enum passed_time {
    HOUR,   // Considering 1 hour
    DAY,    // Counting 1 day
    WEEK,   // Counting 7 days
    MONTH,  // Averaged at 30 days
};

/*
    # Get current time
    Returns the current time in a struct tm*.

    ## Returns
    A pointer to a struct tm containing the current time.

    ## Info
    Check the library time.h to see how a struct tm is structured.

    ## Deallocation
    The value returned by this function must not be deallocated.
*/
struct tm* get_current_time();

/*
    # String to formatted time
    Converts a string telling the time into a struct tm.

    ## Input
    A well-formatted string about time using the format "dd/mm/yyyy hh:mm:ss"

    ## Returns
    A pointer to a struct tm containing the information represented in the string.

    ## Deallocation
    Deallocation of the pointer returned by this function is delegated to the user.
*/
struct tm* str_to_ftime(char* stime);

/*
    # Formatted time to string
    Converts a struct tm into a string formatted as dd/mm/yyyy hh:mm:ss.

    ## Input
    A struct tm that holds the information about the time to convert.

    ## Returns
    A well-formatted string displaying the time in the format of "dd/mm/yyyy hh:mm:ss".

    ## Deallocation
    Deallocation of the pointer returned is delegated to the user.
*/
char* ftime_to_string(struct tm* ftime);

/*
    # Compare formatted time
    Compares the two time structs given and evaluates if the time
    passed between the two is greater then the specified quantity.

    ## Input
    t1 -> first time to compare
    t2 -> second time to compare
    pt -> specified time to pass between the first and the second time

    ## Returns
    Returns 1 if |t1-t2| > pt, otherwise returns 0
*/
int cmp_ftime(struct tm* t1, struct tm* t2, enum passed_time pt);

/*
    # Calculate expiration
    Evaluates the time t2 which will make cmp_ftime return 1 given the same t1 and pt.

    ## Input
    t1 -> starting time
    pt -> time to pass before expiration

    ## Returns
    The time that can be expressed as t1 + pt

    ## Deallocation
    No deallocation needed.
*/
struct tm* calculate_expiration(struct tm* t1, enum passed_time pt);

/*
    # Generate limited random integer
    Generates a random integer limited to the upper_bound

    ## Input 
    upper_bound -> upper limit to the integer generation 

    ## Returns
    An integer between 0 and (upper_bound - 1)
*/
int gen_limited_randint(int upper_bound);

/*
    # Is time
    Checks if the given string is effectively a time formatted string.

    ## Input
    stime -> a string of any kind

    ## Returns
    Returns 1 if stime is a string formatted as "dd/mm/yyyy hh:mm:ss",
    else it returns 0
*/
int is_time(char* stime);

#endif