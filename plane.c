#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

Obj *
newplane(Point3 p, Point3 n)
{
	Obj *plane;

	plane = malloc(sizeof(Obj));
	if(plane == nil)
		exits("malloc");

	plane->type = PLANE;
	plane->p = p;
	plane->n = n;
	plane->next = nil;
	plane->chld = nil;

	return plane;
}

double
planehit(Obj *plane, Point3 p0, Point3 v)
{
	double denom;
	Point3 dist;

	denom = dot3(v, plane->n);
	if(denom == 0)
		return 0;

	dist = sub3(plane->p, p0);
	return dot3(dist, plane->n)/denom;
}
