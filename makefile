FLAGS = -g -Wall -Werror -fsanitize=address --pedantic -o

out/out : src/main.c lib/helper_lib lib/author_picker_lib lib/ap_time_lib out/
	gcc $(FLAGS) out/out src/main.c lib/author_picker_lib lib/helper_lib lib/ap_time_lib

lib/helper_lib : src/helper_lib.c lib/
	gcc -c $(FLAGS) lib/helper_lib src/helper_lib.c

lib/author_picker_lib : src/author_picker.c lib/
	gcc -c $(FLAGS) lib/author_picker_lib src/author_picker.c

lib/ap_time_lib : src/ap_time.c lib/
	gcc -c $(FLAGS) lib/ap_time_lib src/ap_time.c

out/:
	mkdir out/

lib/:
	mkdir lib/

clean:
	rm out/out lib/helper_lib lib/author_picker_lib lib/ap_time_lib out/author_db.txt out/config.txt
	rmdir out/ lib/