#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

Colour
trace(int depth, Obj *obj, Point3 e, Point3 d)
{
	Obj *o, *hitobj;
	double hit, minhit;
	int x;

	if(obj == nil)
		return (Colour){0, 0, 0};

	minhit = -1;
	hitobj = nil;
	for(o = obj; o != nil; o = o->next){
		if(o->type == PLANE)
			hit = planehit(o, e, d);
		else if(o->type == SPHERE)
			hit = spherehit(o, e, d);
		else
			hit = -1;

		if(hit > 0)
			if(hit < minhit || minhit < 0){
				minhit = hit;
				hitobj = o;
			}
	}

	if(hitobj == nil)
		return (Colour){0, 0, 0};
		
	if(hitobj->type == SPHERE)
		return (Colour){0, 1, 0};
	else if(hitobj->type == PLANE){
		x = dist3(hitobj->p, add3(e, mul3(d, minhit))) * 100;
		if(x % 20 > 10)
			return (Colour){1, 0, 0};
		else
			return (Colour){0, 0, 1};
	}else
		return (Colour){0, 0, 0};
}
