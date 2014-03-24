#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"

extern Memimage *render(Scene *scene);

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

	img = render(newscene(320, 240));
	writememimage(1, img);

	exits("");
}
