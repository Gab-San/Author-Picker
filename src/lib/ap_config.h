#ifndef AP_CONFIG_H
#define AP_CONFIG_H

#include "ap_time.h"

/*
    # Check configuration
    Check if the configuration file exists and correctly set up.
*/
void check_config();

/*
    # Setup configuration
    Creates configuration file if not present and sets it up configuration file.

    ## Input
    optional_dialog -> a dialog message to show to the user when setting up configuration file (can be NULL).
*/
void setup_config(const char* optional_dialog);

/*
    # Read configuration file
    Reads the configuration file and extracts information stored.

    ## Returns
    A passed_time enum containing the extracted information from the configuration file.
*/
enum passed_time read_config();

#endif