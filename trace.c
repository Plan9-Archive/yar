#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

Colour
trace(int depth, Obj *obj, Point3 e, Point3 d)
{
	double t;
	int x;

	if(obj->type != PLANE)
		return (Colour){0, 0, 0};

	if(t = planehit(obj, e, d)){
		x = dist3(obj->p, add3(e, mul3(d, t))) * 100;
		if(x % 20 > 10)
			return (Colour){1, 0, 0};
		else
			return (Colour){0, 0, 1};
	}else
		return (Colour){0, 0, 0};
}
