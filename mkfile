</$objtype/mkfile

OBJS = main.$O bbox.$O sphere.$O scene.$O plane.$O trace.$O light.$O colour.$O pipes.$O
HDRS = scene.h pipes.h

%.$O: %.c
	$CC $stem.c

%.$O: $HDRS

$O.yar: $OBJS
	$LD -o $O.yar $OBJS

show:V: what.png

what.png: $O.yar
	$O.yar | topng > what.png

clean:V:
	rm *.$O $O.*
