#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>

int nnodes, node, nproc;

Memimage *img;

void
main(int argc, char **argv)
{
	char *NPROC;

	NPROC= getenv("NPROC");
	if(NPROC != nil){
		nproc = atoi(NPROC);
		if(nproc < 1)
			nproc = 1;
	}else
		nproc = 1;

	ARGBEGIN {

	} ARGEND

	/* RGB24 is actually laid out B, G, R, B, G, R, ... */
	img = allocmemimage(Rect(0, 0, 320, 240), RGB24);
	memfillcolor(img, DBlack);

	{
		int i, j;
		for(i = 0; i < 240; ++i)
			for(j = 0; j < 10; ++j)
				*(byteaddr(img, (Point){i, i+j}) + 1) = 0xFF;
	}

	writememimage(1, img);

	exits("");
}
