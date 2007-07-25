#
# makefile header
#

# common

export SRC  := Feelin:Sources
export DST  := Feelin:Libs/Feelin
export CATS := Feelin:Locale/Catalogs/Français
export EXT  := fc

# compiler

export COMPILER := sc
export COMPILER_OPTIONS := RESETOPTS IGNORE 73 IGNORE 306 NOICON NOVERSION NOSTACKCHECK COMMENTNEST \
			ERRREXX STREQ STRMER GST=Include:feelin.gst INCLUDEDIR=SC:Include \
			INCLUDEDIR=Feelin:Include CPU=68020 MATH=IEEE PARAMETERS=REGISTERS MODIFIED \
			MEMORYSIZE=HUGE MULTIPLECHARACTERCONSTANTS
export COMPILER_OPTIONS += OPTIMIZE OPTGO OPTPEEP OPTINLOCAL OPTINL OPTLOOP OPTRDEP=4 OPTDEP=4 OPTCOMP=4
#export COMPILER_CLASS_OPTIONS :=
export COMPILER_OBJECT := OBJNAME

# linker

export LINKER := slink
export LINKER_OPTIONS := NOICONS NODEBUG SC SD QUIET ADDSYM STRIPDEBUG FROM
#export LINKER_CLASSES_OPTIONS :=
export LINKER_LIBS := LIB sc:lib/sc.lib LIB sc:lib/scmieee.lib LIB feelin:include/lib/feelin_68k.lib
export LINKER_OUTPUT := TO

# flush

export FLUSH := avail >NIL: FLUSH

# platform

export PLATFORM := 68k

# create

%.o : %.c
	@ echo ">>"[1m$(<F)[0m $(COMPLIER) $(COMPILER_OPTIONS) $@
	@ $(COMPILER) $(COMPILER_OPTIONS) $(COMPILER_CLASS_OPTIONS) $(COMPILER_OBJECT) $@ $<
