COMPILER:=x86_64-w64-mingw32-g++-posix
EXECUTABLE_SHELL:=powershell.exe

BIN:=bin/MinGW
BUILD:=Debug
PROJECT_NAME_PREFIX:=
PROJECT_NAME_POSTFIX:=.exe
ARG:=

CFLAG:=\
-std=c++2a \
-D_DEBUG -Og -g \
-Wall -Wextra -Wpedantic -pedantic -pedantic-errors \

LFLAG:=\
