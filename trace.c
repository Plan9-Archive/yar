#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"

Colour
tracelight(int depth, Obj *obj, Hit pos)
{
	double c;

	c = 1 - (dot3(pos.id, pos.n) / (len3(pos.id) *len3(pos.n)));

	return (Colour){
		pos.c.r * c,
		pos.c.g * c,
		pos.c.b *c
	};
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

	if(depth > 2)
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

	return (Colour){
		(c2.r *0.5) + c1.r,
		(c2.g * 0.5) + c1.g, 
		(c2.b * 0.5) + c1.b
	};
}
