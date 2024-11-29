#include "ap_time.h"
#include "helper_lib.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

int gen_limited_randint(int upper_bound){
    srand(time(NULL));
    int r = rand() % upper_bound;
    return r;
}

struct tm* str_to_ftime(char* stime){
    int dd, mm, yy, h, m, s;
    sscanf(stime, "%02d/%02d/%04d %02d:%02d:%02d",  &dd, &mm, &yy, 
                                                    &h, &m, &s);
    
    printf("%d %d %d %d %d %d\n", dd, mm, yy, h ,m, s);
    assert(yy >= 1970);
    yy -= 1970;
    time_t sec = 3600 *24 * ((int) (0.25* yy) *366 + ((int)(0.75 *yy) + yy%4) *365);
    mm -= 1;
    int thr_mm = 0;
    if(mm >= 11)        thr_mm = 4;
    else if(mm >= 9)    thr_mm = 3;
    else if(mm >= 6)    thr_mm = 2;
    else if(mm >= 4)    thr_mm = 1; 
    int feb = (mm >= 2) ? 1 : 0;

    sec += (mm - thr_mm - feb) * 31 *24 *3600 + thr_mm * 30 * 24 *3600 + 
            feb * 24 * 3600 * ((yy + 1)%4 == 0 ? 29 : 28); 
    sec += (dd - 1) *24*3600;
    sec += (h - 1) * 3600;
    sec += m * 60;
    sec += s;
    
    return localtime(&sec);
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

int cmp_ftime(struct tm* t1, struct tm* t2, enum Passed_Time pt){
    assert(t1 != NULL && t2 != NULL);
    difftime(timegm(t1), timegm(t2));
    return 0;
}
