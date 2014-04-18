#include <u.h>
#include <libc.h>
#include <thread.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"
#include "pipes.h"

typedef struct Pipeargs Pipeargs;
struct Pipeargs
{
	int id;
	Obj *o;
	Channel *creq;
};

void
objproc(void *arg)
{
	Pipeargs *pipe;
	Obj *o;
	Channel *creq, *cresp;

	pipe = (Pipeargs *)arg;
	o = pipe->o;
	creq = pipe->creq;
	cresp = chancreate(sizeof(Hitresp), 8);
}

Hitpipe *
mkobjpipe(int id, Obj *o)
{
	Hitpipe *pipe;
	Pipeargs *pa;

	pipe = malloc(sizeof(Hitpipe));
	if(pipe == nil)
		sysfatal("malloc: %r");

	pa = malloc(sizeof(Pipeargs));
	if(pa == nil)
		sysfatal("malloc: %r");

	pipe->type = o->type;
	pipe->id = id;
	pipe->req = chancreate(sizeof(Hitreq), 8);
	pipe->next = nil;

	pa->id = id;
	pa->o = o;
	pa->creq = pipe->req;
	proccreate(objproc, pa, 8192*1024);

	return pipe;
}
