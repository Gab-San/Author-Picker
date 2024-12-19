# <img src="./other/images/author-picker-logo.png" alt="Description of image" style="width:40px;height:40px;"/> Author-Picker

This simple program lets you write down authors or albums you want to listen to and then, whenever you feel like it, you can extract a new author as a suggestion for your next music session!

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)

## Installation

> WARNING! C compiler is needed to install this software.

> This software was not tested on Mac or Windows (only on wsl).

### Ubuntu (or WSL)

1. Clone this repository: `git clone https://github.com/Gab-San/Author-Picker.git`
2. Get into the folder: `cd author_picker/`
3. Compile the program:
    - Automatically: the makefile in the project will compile everything that is needed, just write the command `make`
    - Manually (as the makefile would do it):
        1. You will need to compile the libraries first:
            - Make a library folder named *lib* (`mkdir lib/`)
            - `gcc -c -g -o lib/helper_lib src/helper_lib.c`
            - `gcc -c -g -o lib/author_picker_lib src/author_picker.c`;
            - `gcc -c -g -o lib/ap_time_lib src/ap_time.c`
        2. Then you can compile and link the main:
            - Make an output folder named *out* (`mkdir out/`), inside of it make a folder for the files named *db* (`mkdir out/db/` or `cd out/;mkdir db/;cd ..`)
            - `gcc -g -o out/out src/main.c lib/author_picker_lib lib/helper_lib lib/ap_time_lib`
4. Run the program:
    1. Enter into the output folder: `cd out/`
    2. Run the program `./out`

#### Runnable bash file

> To facilitate step 4 I wrote a little bash program that will automatically run the program.
Copy paste the following code in a 'file_name'.sh file:

```bash
#! bin/bash
cd out/
./out
cd ..
```

Then you can simply run `./'file_name'.sh`

## Usage

To run:

1. Enter the folder `out/` with `cd out/`;
2. Run the program by typing `./out`.

or if you wrote the [runnable bash file](#runnable-bash-file) simply run `./file_name.sh`

> Disclaimer! Most of the inputs can be deduced from the menus. The character(s) in between square brackets show what should be the input.

On the first start-up, the first screen that will appear will lets you choose how much time you want to wait before the next extraction command will be accepted:

![Choose-Limit](./images/tutorial/choose_limit.png)

After which the menu appears, check out the [user manual](./other/user_manual.md) to understand what the actions do. A list
of the available commands is displayed below here👇

| Action | Description |
|:------:| ----------- |
| `insert` | insert a new author into the database |
| `extract` | extracts an author from the database and sets expiration date |
| `view` | print to screen the inserted authors or the extracted one |
| `remove` | removes author from the database |
| `find` | search for an author in the database |


Check [what's next](#whats-next) for more info about coming patches.

## Contributing

This repository is completely open to contribution but I might not be able to completely support you. Docs will be completed as soon as possible.

1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Make your changes.
4. Push your branch: `git push origin feature-name`.
5. Create a pull request.

## Implemented features

View: 00-view-all

## What's Next

My intentions are not to develop further this software with new functionalities. I consider this program finished and working like I intended it to.

I will try to add some other actions like:

- changing the config file in order to have more flexible limits;
- make the programs prints and outputs be more verbose;

---

Since I'm proud of my little work:
<p align="center"> <img src="./other/images/author-picker-logo.png" alt="Logo"> </p>

Be sure to check the [wiki](https://github.com/Gab-San/Author-Picker/wiki)!
