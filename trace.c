#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"

Colour
trace(int depth, Obj *obj, Point3 e, Point3 d)
{
	Obj *o;
	Hit hit, minhit;
	double c;
	Colour c2;

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

	c2 = trace(depth+1, obj, add3(minhit.p, mul3(minhit.n, 0.01)), sub3(d, mul3(minhit.n, 2*dot3(d, minhit.n))));

	c = 1-(dot3(d, minhit.n)/(len3(d) * len3(minhit.n)));
	return (Colour){
		(c2.r *0.5) + minhit.c.r * c,
		(c2.g * 0.5) + minhit.c.g * c,
		(c2.b * 0.5) + minhit.c.b * c
	};
}
