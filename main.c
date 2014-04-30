#include <u.h>
#include <libc.h>
#include <thread.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"
#include "pipes.h"

int nnodes, node, nproc;
int nsamples;
int maxdepth;

int mainstacksize = 1024 * 8192;

void
usage(char *s)
{
	fprint(2, "%s [-c scnid] [-d maxdepth] [-s samples] [-x xres] [-y yres]\n", s);
	exits("usage");
}

void
threadmain(int argc, char **argv)
{
	char *NPROC, *s;
	Pipescene *ps;
	int xres, yres;
	int scnid;
	int i;

	nproc = 1;
	nsamples = 4;
	maxdepth = 16;
	xres = 1024;
	yres = 768;
	scnid = 1;

	ARGBEGIN {
	case 'c':
		s = EARGF(usage(argv0));
		scnid = atoi(s);
		break;
	case 'd':
		s = EARGF(usage(argv0));
		maxdepth = atoi(s);
		if(maxdepth < 1)
			maxdepth = 1;
		break;
	case 's':
		s = EARGF(usage(argv0));
		nsamples = atoi(s);
		if(nsamples < 1)
			nsamples = 1;
		break;
	case 'x':
		s = EARGF(usage(argv0));
		xres = atoi(s);
		if(xres < 64)
			xres = 64;
		break;
	case 'y':
		s = EARGF(usage(argv0));
		yres = atoi(s);
		if(yres < 64)
			yres = 64;
		break;
	default:
		usage(argv0);
	} ARGEND

	srand(time(0));

	ps = pipescene(xres, yres);
	piperender(ps);
	writememimage(1, ps->scene->img);

	threadexitsall("");
}
