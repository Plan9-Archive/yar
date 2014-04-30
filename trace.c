#include <u.h>
#include <libc.h>
#include <thread.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"
#include "pipes.h"

Hit
traceshadow(int depth, Obj *obj, Hit pos, Hit light)
{
	Obj *o;
	Hit hit, minhit;

	minhit.d = 0;
	minhit.o = nil;
	for(o = obj; o != nil; o = o->next){
		if(o == pos.o)
			continue;
		switch(o->type){
		case SPHERE:
			hit = spherehit(o, pos.p, unit3(light.id));
			break;
		case PLANE:
			hit = planehit(o, pos.p, unit3(light.id));
			break;
		case LIGHT:
			hit = lighthit(o, pos.p, unit3(light.id));
			break;
		default:
			hit.d = 0;
			break;
		}
		if(minhit.d == 0 || (hit.d > 0 && hit.d < minhit.d))
			minhit = hit;
	}

	return minhit;
}

Colour
blinnphong(Hit pos, Hit light)
{
	Point3 h;
	double specdot, bp;

	h = unit3(add3(neg3(unit3(pos.id)), unit3(light.id)));

	specdot = dot3(unit3(pos.n), h);
	if(specdot < 0)
		specdot = 0;

	bp = pow(specdot, 99);

	return cscale(light.c, bp);
}

Colour
tracelight(int depth, Obj *obj, Hit pos)
{
	Obj *o;
	Hit hit, shadow;
	Colour bp, bpt, c;
	double ct;

	bp = (Colour){0, 0, 0};
	c = (Colour){0, 0, 0};
	for(o = obj; o != nil; o = o->next){
		if(o->type != LIGHT)
			continue;
		hit = lighthit(o, pos.p, sub3(o->p, pos.p));
		shadow = traceshadow(depth+1, obj, pos, hit);
		if(shadow.d == 0 || shadow.o != hit.o)
			continue;

		ct = dot3(unit3(hit.id), unit3(pos.n));
		bpt = blinnphong(pos, hit);
		if(ct < 0)
			ct = 0;
		c = csum(c, cscale(hit.c, ct));
		bp = csum(bp, bpt);
	}

	return csum(cmul(pos.c, c), bp);
}

Colour
tracerefl(int depth, Obj *obj, Hit pos)
{
	Point3 peps, refldir;

	peps = add3(pos.p, mul3(pos.n, 0.01));
	refldir = sub3(pos.id, mul3(pos.n, 2*dot3(pos.id, pos.n)));

	return trace(depth, obj, peps, refldir);
}

Colour
trace(int depth, Obj *obj, Point3 e, Point3 d)
{
	Obj *o;
	Hit hit, minhit;
	Colour c1, c2;

	if(depth > maxdepth)
		return (Colour){0, 0, 0};

	if(obj == nil)
		return (Colour){0, 0, 0};

	minhit.d = -1;
	for(o = obj; o != nil; o = o->next){
		if(o->type == PLANE)
			hit = planehit(o, e, d);
		else if(o->type == SPHERE)
			hit = spherehit(o, e, d);
		else
			hit.d = -1;

		if(hit.d > 0)
			if(hit.d < minhit.d || minhit.d <= 0){
				minhit = hit;
			}
	}

	if(minhit.d <= 0)
		return (Colour){0, 0, 0};

	c1 = tracelight(depth+1, obj, minhit);

	c2 = tracerefl(depth+1, obj, minhit);

	return csum(cscale(c2, 0.5), c1);
}

Hitresp
tracepipe(int depth, Hitpipe *pipes, Point3 e, Point3 d)
{
	Hitresp hit, minhit;
	Hitreq req;
	int npipes;

	if(depth < 1){
		minhit.d = -1;
		return minhit;
	}

	req.depth = depth;
	req.e = e;
	req.d = d;
	req.resp = chancreate(sizeof(Hitresp), 0);

	for(npipes = 0; pipes != nil; pipes = pipes->next){
		npipes += 1;
		req.tag = pipes->req;
		send(pipes->req, &req);
	}

	while(npipes-- > 0){
		recv(req.resp, &hit);

		if(minhit.d <= 0 || (hit.d < minhit.d && hit.d > 0))
			minhit = hit;
	}

	chanfree(req.resp);
	return minhit;
}
