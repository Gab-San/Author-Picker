#ifndef AP_TIME_H
#define AP_TIME_H

typedef struct time_str {
    int day;
    int month;
    int year;
    int hour;
    int min;
    int sec;
} time_format_t;

time_format_t get_current_time();

#endif