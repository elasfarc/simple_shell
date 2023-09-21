# Simple Shell Project - 0x16

## Overview

The 0x16 project, titled "C - Simple Shell," is a significant milestone project undertaken as part of ALX Africa Software Engineering. This project delves into the intricacies of creating a shell program and involves advanced concepts such as process management, system calls, bit manipulation, file handling, and error handling.

This shell is designed to emulate the functionalities of a simple UNIX command interpreter (similar to "sh") and is implemented entirely in the C programming language.

## Description

This shell is a C-based implementation inspired by the [Thompson Shell](https://en.wikipedia.org/wiki/Thompson_shell). It provides a command-line interface where users can input commands. Each command line is expected to terminate with a new line character.

One of the key features of this shell is its ability to handle various scenarios effectively, such as:

- Locating and executing executable files.
- Providing clear error messages if a command is not found.
- Handling errors gracefully.
- Managing the "end of file" condition (Ctrl+D).
- Processing command lines with arguments.
- Managing the system PATH.
- Supporting the 'exit' command along with exit status.
- Preventing unintended shell termination via Ctrl-C.
- Managing command separators (';').
- Handling logical operators ('&&' and '||').

## Environment

The shell has been built and rigorously tested on Ubuntu 14.04 LTS.

## Built-in Commands

This shell supports several built-in commands that enhance its functionality:

- **exit [n]**: Exit the shell, with an optional exit status, 'n'.
- **env**: Display the environment variables.
- **setenv [var][value]**: Set an environment variable and its value. If the variable exists, the value will be updated.
- **unsetenv [var]**: Remove an environment variable.
- **cd [dir]**: Change the current working directory.

## Contributors


- [Mohammed Al-Asfar](https://github.com/elasfarc)

This project represents a significant achievement in the exploration of shell programming and its associated complexities.
