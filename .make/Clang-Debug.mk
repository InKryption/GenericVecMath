COMPILER:=clang++
EXECUTABLE_SHELL:=

BIN:=bin/Clang
BUILD:=Debug
PROJECT_NAME_PREFIX:=
PROJECT_NAME_POSTFIX:=
ARG:=

CFLAG:=\
-std=c++20 \
-D_DEBUG -Og -g \
-Weverything -Wall -Wextra -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded \

LFLAG:=\
