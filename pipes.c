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

typedef struct Threadargs Threadargs;
struct Threadargs
{
	Hitreq;
	Obj *o;
};

void
objthread(void *arg)
{
	Threadargs *req;
	Hitresp hit;

	req = arg;
	hit.oc = req->tag;

	switch(req->o->type){
	case PLANE:
		hit.Hit = planehit(req->o, req->e, req->d);
		break;
	case SPHERE:
		hit.Hit = spherehit(req->o, req->e, req->d);
		break;
	case LIGHT:
		hit.Hit = lighthit(req->o, req->e, req->d);
		break;
	default:
		hit.d = -1;
		break;
	}

	send(req->resp, &hit);
	free(req);
}

void
objproc(void *arg)
{
	Pipeargs *pipe;
	Obj *o;
	Channel *creq;
	Threadargs *req;

	pipe = (Pipeargs *)arg;
	o = pipe->o;
	creq = pipe->creq;

	for(;;){
		req = malloc(sizeof(Threadargs));
		recv(creq, &req->Hitreq);
		req->o = o;
		if(req->depth < 1)
			objthread(req);
		else
			threadcreate(objthread, req, 1024 * 128);
	}
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

Hitpipe *
conspipe(Hitpipe *car, Hitpipe *cdr)
{
	if(car == nil)
		return cdr;

	car->next = cdr;
	if(cdr == nil)
		car->id = 0;
	else
		car->id = cdr->id + 1;

	return car;
}
