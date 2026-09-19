# Customised Virtual File System (CVFS)

A C-based virtual file system project that provides basic file management operations through a custom command-line interface.

## Overview

The Customised Virtual File System (CVFS) is designed to simulate basic file system operations using C.

The project maintains internal structures for file information, permissions, file descriptors and open file handling. It provides a command-line interface through which users can perform different file operations.

## Features

- Create new files
- Open files
- Read data from files
- Write data into files
- Display file information
- Delete files
- List files
- Display command help
- Display manual information for commands
- File permission handling
- File descriptor management
- Error handling for invalid operations

## Available Commands

| Command | Description |
|---|---|
| `help` | Displays available commands |
| `man` | Displays the manual page for a command |
| `clear` | Clears the terminal screen |
| `creat` | Creates a new regular file |
| `write` | Writes data into a file |
| `read` | Reads data from a file |
| `open` | Opens a file |
| `stat` | Displays file information |
| `unlink` | Deletes a file |
| `ls` | Lists files |
| `exit` | Terminates the application |

## File Permissions

The project supports different file permissions:

1 - Read
2 - Write
3 - Read + Write

## Internal Components

The project uses internal data structures to manage the virtual file system, including:

UAREA
Super Block
DILB
File information
File descriptors
User File Descriptor Table (UFDT)
File buffers

The initialization process prepares these structures before the user starts interacting with the virtual file system.

## Technologies Used
C
Data Structures
File System Concepts
Operating System Concepts
System Programming

## Project Structure
Customised-Virtual-File-System/
│
├── CVFS.c
├── README.md
└── .gitignore

## How to Run
Compile

Using GCC:
gcc CVFS.c -o CVFS

Run:
./CVFS

On Windows:
CVFS.exe

## Example

After starting the application, the user can enter commands such as:

help
ls
creat Demo.txt 3
open Demo.txt
stat Demo.txt
write
read
unlink Demo.txt
exit

## Concepts Demonstrated
File system design concepts
File descriptors
File permissions
Open file handling
Read and write operations
Command-line interfaces
Structures in C
Pointers
Dynamic memory and data management
Error handling

## Author

Tarannum Jakirhusen Shaikh
