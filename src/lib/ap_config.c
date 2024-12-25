#include "ap_config.h"
#include "string_helper.h"
#include "file_helper.h"
#include "gen_func.h"

#include <stdio.h>

#define SETUP_DIALOG ("Setting up config file...\n\nHow much time do you want to wait before extracting a new author's name?\n")
#define DIALOG_OPTIONS ("[H]OUR\n[D]AY\n[W]EEK\n[M]ONTH\n")

const char ap_config_path[] = "./db/config.txt";

/*
    Runs the configuration file setup routine
    with the optional dialog (nullable) stating why to run it.

    # Input
    optional_dialog -> dialog to show on screen when setup is run
                        might state why it is unexpected to run the setup routine.
                        If NULL nothing other than the normal messages will show up.
*/
void setup_config(const char* optional_dialog){
    FILE* config = fopen(ap_config_path, "w");
    if(config == NULL) fatal("in setup_config() while opening config file");
    if(optional_dialog != NULL) printf("%s", optional_dialog);
    printf(SETUP_DIALOG);
    printf(DIALOG_OPTIONS);
    printf(INPUT);
    
    char input = getchar() + '\0';
    flush_stdin();
    char low_in = conv_to_lower(input);
    
    while(low_in != 'w' && low_in != 'h' && low_in != 'd' && low_in != 'm'){
        printf("Inserted option not valid!\nPlease insert a valid option!\n");
        printf(INPUT);
        input = getchar() + '\0';
        flush_stdin();
        low_in = conv_to_lower(input);
    };
    
    fwrite(&low_in, 1, 1, config);
    
    close_file(config, "setup_config()");
}

void check_config(){
    FILE* config = fopen(ap_config_path, "r");
    if(config != NULL) return;
    setup_config(NULL);
}

enum passed_time read_config(){
    FILE* fconfig = open_file(ap_config_path, "r", "read_config()");
    
    char cfg;
    fread(&cfg, 1, 1, fconfig);
    enum passed_time pt;
    switch (cfg){
        case 'h':   pt = HOUR; 
                    break;
        case 'd':   pt = DAY;
                    break;
        case 'w':   pt = WEEK;
                    break;
        case 'm':   pt = MONTH;
                    break;
        default:    close_file(fconfig, "read_config()");
                    setup_config("It looks like there is something wrong with the config file.\nLet's set it up again!\n");
                    return read_config();
    }
    close_file(fconfig, "read_config()");
    return pt;
}
