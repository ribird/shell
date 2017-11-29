—To run program go to the directory with
Makefile and shellfinal.c program, type make in terminal,
and then type ./sh in command line.

——FUNCTIONS—-
parse_str -> parses basic args
parse_mult_cmds -> parses multiple cmds
parse_pipe -> parses pipe cmds
add_to_history -> adds args to history
recent_history -> takes care of !! and !d cmds
main -> takes care of flow of execution for the program (see comments)

-—MACROS-—
MAX_LINE -> the max length that a cmd can be (80).

—-DESIGN—
I tried to break my shell up into different functions. For instance, I parse user
Input based on whether there are basic, multiple, or pipe cmds. Flags were useful
For this purpose as well as the ‘&’ cmd. I put ‘!!’ and ‘!d’ cmds into their own
Function because I thought it cleaned up my code nicely and it made sense to do so.
Note: I add the corresponding history cmd that is run with ‘!!’ Or ‘!d’ here to history
 rather than ‘!!’ Or ‘!d’. 