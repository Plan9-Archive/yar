#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

Colour
trace(int depth, Obj *obj, Point3 e, Point3 d)
{
	Obj *o;
	Hit hit, minhit;
	double c;

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

	c = 1-(dot3(d, minhit.n)/(len3(d) * len3(minhit.n)));
	return (Colour){
		minhit.c.r * c,
		minhit.c.g * c,
		minhit.c.b * c
	};
}
