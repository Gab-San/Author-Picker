FLAGS = -g -Wall -Werror -fsanitize=address --pedantic -o

out/out : src/main.c libraries out/
	gcc $(FLAGS) out/out src/main.c lib/author_picker_lib lib/helper_lib lib/ap_time_lib

libraries: lib/ lib/helper_lib lib/author_picker_lib lib/ap_time_lib

lib/helper_lib : src/helper_lib.c 
	gcc -c $(FLAGS) lib/helper_lib src/helper_lib.c

lib/author_picker_lib : src/author_picker.c 
	gcc -c $(FLAGS) lib/author_picker_lib src/author_picker.c

lib/ap_time_lib : src/ap_time.c
	gcc -c $(FLAGS) lib/ap_time_lib src/ap_time.c

out/:
	mkdir out/
	mkdir out/db/

lib/:
	mkdir lib/

clean:
	rm -r out/ 
	rm -r lib/