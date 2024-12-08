# <img src="/images/author-picker-logo.png" alt="Description of image" style="width:40px;height:40px;"/> Author-Picker
This simple program lets you write down authors or albums you want to listen to and then, whenever you feel like it, you can extract a new author as a suggestion for your next music session!

# Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)

# Installation

> WARNING! C compiler is needed to install this software.

> This software was not tested on Mac or Windows (only on wsl).

## Ubuntu (or WSL)
1. Clone this repository: `git clone https://github.com/Gab-San/Author-Picker.git`
2. Get into the folder: `cd author_picker/`
3. Compile the program:
    - Automatically: the makefile in the project will compile everything that is needed, just write the command `make` 
    - Manually: 
        1. You will need to compile the libraries first:
            - Make a library folder named *lib* (`mkdir lib/`)
            - `gcc -c -g -o lib/helper_lib src/helper_lib.c`
            - `gcc -c -g -o lib/author_picker_lib src/author_picker.c`;
            - `gcc -c -g -o lib/ap_time_lib src/ap_time.c`
        2. Then you can compile and link the main:
            - Make an output folder named *out* (`mkdir out/`)
            - `gcc -g -o out/out src/main.c lib/author_picker_lib lib/helper_lib lib/ap_time_lib`
4. Run the program: `./out/out`

### Runnable bash file
> To facilitate step 4 I wrote a little bash program that will automatically run the program.
Copy paste the following code in a file_name.sh file:
```bash
#! bin/bash
./out/out
```
Then you can simply run `./file_name.sh`

# Usage

- [Insert](#insert)
- [Extract](#extract)
- [View](#view)

To run use  `./out/out` or if you wrote the [runnable bash file](#runnable-bash-file) simply run `./file_name.sh`

> Disclaimer! Most of the inputs can be deduced from the menus. The character(s) in between square brackets show what should be the input.

On the first start-up, the first screen that will appear will lets you choose how much time you want to wait before the next extraction command will be accepted:

![Choose-Limit](./images/tutorial/choose_limit.png)

After which the menu appears, right now there are only a couple actions that you can do. (Check [what's next](#whats-next) for more info)
## Insert
*Insert* lets you insert an author into the database:

![Insert](./images/tutorial/insert_example.png)


## Extract
*Extract* lets you extract an author and set an expiration date to wait for your next extraction:

![Appr-Extract](./images/tutorial/approved_extraction.png)

If you request another extraction before the time limit has expired then your request will be denied:

![Deny-Extract](./images/tutorial/denied_extraction.png)

## View
You can view the extracted author or the other inserted ones by using the *view* command:

![View-All](./images/tutorial/view_all.png)

![View-Extracted](./images/tutorial/view_extracted.png)

If the input command is incorrect the following screen will be shown:

![Incorrect-View-Input](./images/tutorial/incorrect_view_input.png)

When in _view all_, in order to stop scrolling authors input `q` when the input appears:

![Quit-View](./images/tutorial/quit_view.png) 

# Contributing
This repository is completely open to contribution but I might not be able to completely support you. Docs will be completed as soon as possible.
1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Make your changes.
4. Push your branch: `git push origin feature-name`.
5. Create a pull request.

# Implemented features

View: 00-view-all

# What's Next

My intentions are not to develop further this software with new functionalities. I consider this program finished and working like I intended it to.

I will try to add some other actions like:
- changing the config file in order to have more flexible limits;
- make the programs prints and outputs be more verbose;

-----
Since I'm proud of my little work:
<h1 align="center">
    <img src="/images/author-picker-logo.png" alt="">
</h1>

Be sure to check the [wiki](https://github.com/Gab-San/Author-Picker/wiki)!
