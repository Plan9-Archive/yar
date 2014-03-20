#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

Bbox
spherebb(Point3 pos, double r)
{
	Bbox b;

	b.min.x = pos.x - r;
	b.min.y = pos.y - r;
	b.min.z = pos.z - r;
	b.min.w = 1;

	b.max.x = pos.x + r;
	b.max.y = pos.y + r;
	b.max.z = pos.z + r;
	b.max.w = 1;

	return b;
}

Obj *
newsphere(Point3 pos, double r)
{
	Obj *sphere;

	sphere = malloc(sizeof(Obj));
	if(sphere == nil)
		exits("malloc");

	sphere->type = SPHERE;
	sphere->p = pos;
	sphere->n = (Point3){0, 0, 0, 1};
	sphere->r = r;
	sphere->next = nil;
	sphere->chld = nil;
	sphere->Bbox = spherebb(pos, r);

	return sphere;
}
