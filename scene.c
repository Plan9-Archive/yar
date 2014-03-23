#include <u.h>
#include <libc.h>
#include <draw.h>
#include <geometry.h>

#include "scene.h"

Scene *
newscene(int w, int h)
{
	Scene *scene;

	scene = malloc(sizeof(Scene));
	if(scene == nil)
		exits("malloc");

	scene->objs = nil;
	scene->e = (Point3){0, 0, 1, 0};
	scene->u = (Point3){1, 0, 0, 0};
	scene->v = (Point3){0, 1, 0, 0};
	scene->w = (Point3){0, 0, 1, 0};
	scene->d = 1.0;
	scene->l = -1;
	scene->r = 1;
	scene->t = 1;
	scene->b = -1;
	scene->s = (Point){w, h};

	return scene;
}

Point3
eyeray(Scene *scene, int x, int y)
{
	double u, v;
	Point3 ray;

	u = scene->l + (scene->r - scene->l) * ((double)x + 0.5) / (double)scene->s.x;
	v = scene->b + (scene->t - scene->b) * ((double)y + 0.5) / (double)scene->s.y;

	ray = mul3(scene->w, -scene->d);
	ray = add3(ray, mul3(scene->u, u));
	ray = add3(ray, mul3(scene->v, v));

	return ray;
}
