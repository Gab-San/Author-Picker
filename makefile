out/out: src/main.c src/author_picker.c
	gcc -c -g -Wall -Werror -o src/author_picker_lib src/author_picker.c
	gcc -g -Wall -Werror -o out/out src/main.c src/author_picker_lib