#ifndef AP_CONFIG_H
#define AP_CONFIG_H

#include "ap_time.h"

void check_config();
void setup_config(const char* optional_dialog);
enum passed_time read_config();

#endif