# create all executables

all : Alpha Class1 Class2 \
		ColorCube CrazyGauges Dialog DnD fonts \
		FWidgetPublic Gradient Lines Listview \
		SignalHandler Strings Thread XMLApplication

% : %.c
	SC $<
	@ delete quiet *.(o|lnk)
#    vc $< -o $@ -IFeelin:Include -LFeelin:Include/lib -lfeelin

clean :

	Delete QUIET *.(o|lnk)

delete :

	Delete QUIET ~(*.e|*.c|*.xml|*.css|*.info|*makefile*|scoptions)
