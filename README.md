# Spccesh

#### *Sometimes great things emerge out of a simple typo.*
> spcce >>>> space

A basic shell that executes bash commands. It can create and run Foreground and Background processes. It can also display history. It is fully written in C.

## Instructions
- `cd spccesh && make`
- Run `./spccesh` to start the shell

## Files
- `cd.c` : Implements *cd* command.
- `echo.c` : Implements *echo* command.
- `exec_proc.c` : Creates and executes foreground and background processes.
- `exit.c` : Implements *exit* command.
- `history.c` : Implements *history* command.
- `ls.c` : Implements *ls* command.
- `main.c` : The main component which calls other components which contains the main loop.
- `nightswatch.c` : Implements a feature similar to *watch*. Accepts [interrupt/newborn] as the arguement.
- `pinfo.c` : Provides information about a process. PID should be passed the argument.
- `prompt.c` : Prints Hostname and Username.
- `pwd.c` : Implements *pwd* command.
- `setname.c` : Extracts Hostname and Username.

## Procedure
Execute `make` in the cloned directory. Spccesh can be run by executing `./spccesh`.

## Implementation
- `history` : History can display maximum of 10 commands and it displays only the **previous** commands (excluding the current one like ZSH). Continuously repeated commands are stored only once in the history.

### The shell was developed in Linux(Manjaro), some features may not work in other distros or other OS(s).
