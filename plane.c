#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

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
