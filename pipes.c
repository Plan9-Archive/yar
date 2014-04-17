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
	Obj *o;
	Channel *creq;
};

void
objthread(void *arg)
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
mkobjpipe(Obj *o)
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
	pipe->c = chancreate(sizeof(Hitreq), 8);
	pipe->next = nil;

	pa->o = o;
	pa->creq = pipe->c;
	proccreate(objthread, pa, 8192*1024);

	return pipe;
}
