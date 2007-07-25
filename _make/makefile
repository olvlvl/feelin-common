#
# This makefile is used to compile classes.
# It's usually included in classes makefile.
#
# The same makefile should be used regardless of the
# machine and system used. Relative options are defined
# in the makefile-header file.
#
# Make sure to overwrite the file to match your
# system and configuration. e.g. A WinUAE user, developping
# using CubicIDE should copy the 'makefile-header.68k.gcc' to
# 'makefile-header'.
#

include Feelin:_make/makefile-header

objs := $(patsubst %.c,%.o,$(wildcard *.c))

$(DST)/$(NAME).$(EXT) : $(objs)
	@ $(FLUSH)
	$(LINKER) $(LINKER_OPTIONS) $(LINKER_CLASSES_OPTIONS) Project.o $(filter-out Project.o, $^) $(LINKER_OUTPUT) $(DST)/$(NAME).$(EXT) $(LINKER_LIBS) $(LINKER_LIBS_APPEND)
	@ echo ">>" [1m$(DST)/$(NAME).$(EXT)[0m Version [1m$(VER).$(REV)[0m

clean :
	@ echo "delete>>" $(objs)
	@ delete QUIET $(objs)
