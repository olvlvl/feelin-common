# create all executables

include Feelin:Sources/_template/makefile.header.68k

all : Prefs Setup
	
% : %.c
	SC $<
	
Prefs : Prefs.c
Setup : Setup.c
