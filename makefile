# makefile for the ask program
##############################################################################

SHELL = /bin/sh

# installation directories
prefix = /usr/local
home = ~
srcdir = ./

CFLAGS = -O3 -Wall -g -std=c17
INCLUDES = -I. -I /usr/include -I $(prefix)/include
SOURCES = ask.c
ASMS = ask.s ask_funcs.s
OBJS = ask.o ask_funcs.o
EXECUTABLE = ask

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.s : %.c
	$(CC) $(CFLAGS) $(INCLUDES) -S -fverbose-asm -c $< -o $@

##############################################################################

all : clean build

build : clean lib ask

targets :
	@echo Targets... && grep -A 1 '^[\.a-zA-Z1-9_ ]*:' ./makefile

variables :
	@echo Variables... && grep '^[a-zA-Z1-9_ ]*=' ./makefile

clean :
	-rm -v $(ASMS) 
	-rm -v $(OBJS) 
	-rm -v $(EXECUTABLE) 

test : clean lib ask
	./test_ask

# Library module

lib : ask_funcs.o makefile

ask_funcs.o : ask_funcs.c ask_funcs.h makefile

# Utility program

ask : $(OBJS) makefile
	$(CC) $(CFLAGS) -o ask $(OBJS)

asm : ask_funcs.s ask.s makefile

ask.o : ask.c ask_funcs.h makefile

# Install/uninstall

install : ask
	cp -pv ask $(home)/bin
	sudo cp -pv ask_funcs.o $(prefix)/lib
	sudo cp -pv ask_funcs.h $(prefix)/include


uninstall : $(home)/bin/ask
	rm -v $(home)/bin/ask
	sudo rm -v $(prefix)/lib/ask_funcs.o
	sudo rm -v $(prefix)/include/ask_funcs.h

##############################################################################
