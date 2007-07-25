#
#  Use this makefile to create feelin.library
#

include feelin:_make/makefile-header

OBJECTS := \
	Class1 \
	Class2 \
	ColorCube \
	CrazyGauges \
	Dialog \
	DnD \
	dosnotify \
	FWidgetPublic \
	lines \
	listview \
	SignalHandler \
	Strings \
	thread \
	timer \
	XMLApplication

all : $(OBJECTS)

#
#
#

% : %.c
	@ echo ">>"[1m$(@F)[0m
	@ $(COMPILER) $(COMPILER_OPTIONS) $(COMPILER_OBJECT) $@ $(COMPILER_SOURCE) $^ $(LINKER_OPTIONS) $(LINKER_LIBS)

clean :
	delete quiet $(OBJECTS)
