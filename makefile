FLAGS = -g -Wall -Werror -fsanitize=address --pedantic -o

out/out : src/main.c src/helper_lib src/author_picker_lib
	gcc $(FLAGS) out/out src/main.c src/author_picker_lib src/helper_lib

src/helper_lib : src/helper_lib.c
	gcc -c $(FLAGS) src/helper_lib src/helper_lib.c

src/author_picker_lib : src/author_picker.c
	gcc -c $(FLAGS) src/author_picker_lib src/author_picker.c

clean:
	rm out/out src/helper_lib src/author_picker_lib
