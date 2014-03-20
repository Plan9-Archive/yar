</$objtype/mkfile

OBJS = main.$O bbox.$O

%.$O: %.c
	$CC $stem.c

$O.yar: $OBJS
	$LD -o $O.yar $OBJS

show:V: $O.yar
	window $O.yar ^'|page'

clean:V:
	rm *.$O $O.*
