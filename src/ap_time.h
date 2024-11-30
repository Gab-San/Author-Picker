#ifndef AP_TIME_H
#define AP_TIME_H

#include <time.h>

#define MAX_STR_LEN 32
#define FORMATTED_TIME_LEN 19

enum Passed_Time {
    HOUR,   // Considering 1 hour
    DAY,    // Counting 1 day
    WEEK,   // Counting 7 days
    MONTH,  // Averaged at 30 days
};

struct tm* get_current_time();
struct tm* str_to_ftime(char* stime);
int cmp_ftime(struct tm* t1, struct tm* t2, enum Passed_Time pt);
char* ftime_to_string(struct tm* ftime);
int gen_limited_randint(int upper_bound);

#endif