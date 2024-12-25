FLAGS = -g -Wall -Werror -fsanitize=address --pedantic -o
LIBOUT_DIR = out/libs
LIBSRC_DIR = src/lib
COMPILED_LIBRARIES = $(LIBOUT_DIR)/string_helper $(LIBOUT_DIR)/author_picker $(LIBOUT_DIR)/ap_time \
						$(LIBOUT_DIR)/file_helper $(LIBOUT_DIR)/gen_func $(LIBOUT_DIR)/ap_config $(LIBOUT_DIR)/run_module


out/out : src/main.c out/ $(COMPILED_LIBRARIES)
	gcc $(FLAGS) out/out src/main.c $(COMPILED_LIBRARIES)

$(LIBOUT_DIR)/string_helper : $(LIBSRC_DIR)/string_helper.c 
	gcc -c $(FLAGS) $(LIBOUT_DIR)/string_helper $(LIBSRC_DIR)/string_helper.c

$(LIBOUT_DIR)/author_picker : $(LIBSRC_DIR)/author_picker.c 
	gcc -c $(FLAGS) $(LIBOUT_DIR)/author_picker $(LIBSRC_DIR)/author_picker.c

$(LIBOUT_DIR)/ap_time : $(LIBSRC_DIR)/ap_time.c
	gcc -c $(FLAGS) $(LIBOUT_DIR)/ap_time $(LIBSRC_DIR)/ap_time.c

$(LIBOUT_DIR)/file_helper : $(LIBSRC_DIR)/file_helper.c
	gcc -c $(FLAGS) $(LIBOUT_DIR)/file_helper $(LIBSRC_DIR)/file_helper.c 

$(LIBOUT_DIR)/gen_func : $(LIBSRC_DIR)/gen_func.c
	gcc -c $(FLAGS) $(LIBOUT_DIR)/gen_func $(LIBSRC_DIR)/gen_func.c

$(LIBOUT_DIR)/ap_config : $(LIBSRC_DIR)/ap_config.c
	gcc -c $(FLAGS) $(LIBOUT_DIR)/ap_config $(LIBSRC_DIR)/ap_config.c

$(LIBOUT_DIR)/run_module : src/run_module.c
	gcc -c $(FLAGS) $(LIBOUT_DIR)/run_module src/run_module.c

out/:
	mkdir out/
	mkdir out/libs/
	mkdir out/db/

clean:
	rm -r out/