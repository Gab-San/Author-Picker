#define _XOPEN_SOURCE 700
#include "ap_time.h"
#include "helper_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SECS_IN_HOUR 3600
#define SECS_IN_DAY SECS_IN_HOUR * 24

int gen_limited_randint(int upper_bound){
    srand(time(NULL));
    int r = rand() % upper_bound;
    return r;
}

struct tm* str_to_ftime(char* stime){
    int dd, mm, yy, h, m, s;
    sscanf(stime, "%02d/%02d/%04d %02d:%02d:%02d",  &dd, &mm, &yy, 
                                                    &h, &m, &s);
    
    printf("[DEBUG:str_to_ftime()] %d %d %d %d %d %d\n", dd, mm, yy, h ,m, s);
    struct tm* timeinfo = (struct tm*) malloc(sizeof(struct tm));
    strptime(stime, "%d/%m/%Y %H:%M:%S", timeinfo);
    return timeinfo;
}

char* ftime_to_string(struct tm* ftime){
    char* format = (char*) malloc(MAX_STR_LEN);
    if(format == NULL) fatal("in ftime_to_string() while allocating memory");
    snprintf(format, MAX_STR_LEN, "%02d/%02d/%04d %02d:%02d:%02d",  ftime->tm_mday, ftime->tm_mon + 1, ftime->tm_year + 1900,
                                                                    ftime->tm_hour, ftime->tm_min, ftime->tm_sec);
    return format;
}

struct tm* get_current_time() {
    time_t now = time(NULL);
    return localtime(&now);
}

int cmp_ftime(struct tm* t1, struct tm* t2, enum passed_time pt){
    assert(t1 != NULL && t2 != NULL);
    time_t tt1 = mktime(t1);
    time_t tt2 = mktime(t2);
    time_t t_last = (tt1 >= tt2) ? tt1 : tt2;
    time_t t_first = (tt1 >= tt2) ? tt2 : tt1;

    double diff = difftime(t_last, t_first);

    switch (pt)
    {
    case HOUR:
        return diff >= SECS_IN_HOUR;
    case DAY:
        return diff >= SECS_IN_DAY;
    case WEEK:
        return diff >= SECS_IN_DAY * 7;
    case MONTH:
        return diff >= SECS_IN_DAY * 30;
    default:
        report_fatal_error("in cmp_ftime(): no value for passed time was set or an incorrect value was set");
        return 0;
    }
}

struct tm* calculate_expiration(struct tm* textr, enum passed_time pt){
    assert(textr != NULL);
    time_t ttextr = mktime(textr);
    
    switch (pt)
    {
    case HOUR:
        ttextr += SECS_IN_HOUR;
        break;
    case DAY:
        ttextr += SECS_IN_DAY;
        break;
    case WEEK:
        ttextr += SECS_IN_DAY * 7;
        break;
    case MONTH:
        ttextr += SECS_IN_DAY * 30;
        break;
    }

    return localtime(&ttextr);
}

int is_time(char* stime){
    // The way time is formatted it's only numbers
    if(stime[0] < '0' || stime[0] > '9') return 0;
    return 1;
}
