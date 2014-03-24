</$objtype/mkfile

OBJS = main.$O bbox.$O sphere.$O scene.$O plane.$O
HDRS = scene.h

%.$O: %.c
	$CC $stem.c

%.$O: $HDRS

$O.yar: $OBJS
	$LD -o $O.yar $OBJS

show:V: $O.yar
	window $O.yar ^'|page'

clean:V:
	rm *.$O $O.*
