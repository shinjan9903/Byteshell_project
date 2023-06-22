# TerminalShell

TerminalShell is a simple shell program implemented in C. It provides a command-line interface where users can execute commands, navigate directories, and perform various operations.

## Features

- Execute external commands
- Change directories (`cd` command)
- View command history (`history` command)
- Print messages to the console (`echo` command)
- Set variables (`set` command)
- Perform arithmetic operations on variables (`result` command)
- Get help on available commands (`help` command)
- Exit the shell (`exit` command)

## Getting Started

To compile and run the TerminalShell program, follow these steps:

1. Open a terminal or command prompt.
2. Navigate to the directory containing the source code files.
3. Compile the code using the following command:
4. Run the program:

## Usage

Once you have launched the TerminalShell, you can use the following commands:

- `help`: Displays a list of available commands and their usage.
- `exit`: Exits the shell program.
- `cd <directory>`: Changes the current working directory to the specified directory.
- `history [n]`: Displays the command history. Optionally, you can specify the number of history entries to show. By default it shows last 5 commands with latest commands first.
- `echo <message>`: Prints the given message to the console.
- `set <variable> = <value>`: Sets a variable with the specified name and value.
- `result <variable1> <operator> <variable2>`: Performs an arithmetic operation on two variables and prints the result. Supported operators are `+`, `-`, `*`, and `/`.

## Examples
TerminalShinjan -> cd /path/to/directory
TerminalShinjan -> echo "Hello, world!"
TerminalShinjan -> set var1 = 10
TerminalShinjan -> result var1 + var2
TerminalShinjan -> history
TerminalShinjan -> help
TerminalShinjan -> exit


