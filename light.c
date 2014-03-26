#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

Obj *
newlight(Point3 p, double r, Point3 n, Colour c)
{
	Obj *light;

	light = malloc(sizeof(Obj));
	if(light == nil)
		exits("malloc");

	light->p = p;
	light->r = r;
	light->n = n;
	light->c = c;

	return light;
}

Hit
lighthit(Obj *light, Point3 e, Point3 d)
{
	double denom, intersect;
	Point3 dist;
	Hit hit;

	denom = dot3(d, light->n);
	if(denom == 0)
		goto nohit;

	dist = sub3(light->p, e);
	intersect = dot3(dist, light->n)/denom;

	if(intersect <= 0)
		goto nohit;

	hit.d = intersect;
	hit.p = add3(e, mul3(d, intersect));
	hit.n = light->n;
	hit.o = light;
	hit.c = light->c;

	if(dist3(hit.p, light->p) > light->r)
		goto nohit;

	return hit;
nohit:
	hit.d = 0;
	return hit;
}
