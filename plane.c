#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>
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

Hit
planehit(Obj *plane, Point3 e, Point3 d)
{
	double denom, intersect;
	Point3 dist;
	Hit hit;
	int x;

	denom = dot3(d, plane->n);
	if(denom == 0)
		goto nohit;

	dist = sub3(plane->p, e);
	intersect = dot3(dist, plane->n)/denom;

	if(intersect <= 0)
		goto nohit;

	hit.d = intersect;
	hit.p = add3(e, mul3(d, intersect));
	hit.n = plane->n;
	hit.ie = e;
	hit.id = mul3(d, hit.d);
	hit.o = plane;

	x = dist3(plane->p, hit.p) * 100;
	if(x % 20 > 10)
		hit.c = (Colour){1, 0, 0};
	else
		hit.c = (Colour){0, 0, 1};

	return hit;
nohit:
	hit.d = 0;
	return hit;
}
