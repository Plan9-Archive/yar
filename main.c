#include <u.h>
#include <libc.h>
#include <thread.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"

int nnodes, node, nproc;
int nsamples;
int maxdepth;

Scene *scene;
Channel *c;

void
thread(void *arg)
{
	int id;

	id = (int)arg;
	render(scene, id);
	sendul(c, id);
}

void
usage(char *s)
{
	fprint(2, "%s [-d maxdepth] [-s samples]\n", s);
	exits("usage");
}

void
threadmain(int argc, char **argv)
{
	char *NPROC, *s;
	int i;

	NPROC= getenv("NPROC");
	if(NPROC != nil){
		nproc = atoi(NPROC);
		if(nproc < 1)
			nproc = 1;
	}else
		nproc = 1;

	nsamples = 4;
	maxdepth = 16;

	ARGBEGIN {
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
	default:
		usage(argv0);
	} ARGEND

	srand(time(0));

	scene = newscene(1024, 768);
	c = chancreate(sizeof(ulong), 1);
	for(i = 0; i < nproc; ++i)
		proccreate(thread, (void *)i, 8192*1024);

	for(i = 0; i < nproc; ++i)
		recvul(c);

	writememimage(1, scene->img);

	exits("");
}
