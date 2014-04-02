#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"

Scene *
newscene(int w, int h)
{
	Scene *scene;
	Obj *o;

	scene = malloc(sizeof(Scene));
	if(scene == nil)
		exits("malloc");

	scene->img = allocmemimage(Rect(0, 0, w, h), RGB24);
	if(scene->img == nil)
		exits("malloc");

	scene->objs = newplane((Point3){0, -1, 0, 0}, (Point3){0, 1, 0, 0});
	o = newsphere((Point3){0, -0.5, -1, 0}, 0.5);
	scene->objs->next = o;
	o->next = newsphere((Point3){1.5, 0, -2, 0}, 0.5);
	o = o->next;
	o->next = newsphere((Point3){-2, 2, -3, 0}, 1);
	o = o->next;
	o->next = newlight((Point3){3, 3, 0, 0}, 1, (Point3){-3, -3, -1, 0}, (Colour){0.40, 0.6, 0.6});
	o = o->next;
	o->next = newlight((Point3){-2, 4, 2, 0}, 1, (Point3){1, -3, -1, 0}, (Colour){0.4, 0.3, 0.40});
	o = o->next;
	o->next = newlight((Point3){-4, 6, -10, 0}, 1, (Point3){2, -4, 1, 0}, (Colour){0.5, 0.7, 0.4});
	scene->e = (Point3){0, 0, 1, 0};
	scene->u = (Point3){1, 0, 0, 0};
	scene->v = (Point3){0, -1, 0, 0};
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

double
clamp(double x)
{
	if(x > 1) return 1;
	if(x < 0) return 0;
	return x;
}

Memimage *
render(Scene *scene, int id)
{
	Colour c;
	int i, j;
	uchar *px;

	for(j = 0; j < scene->s.y; ++j){
		if((j % nproc) != id)
			continue;
		for(i = 0; i < scene->s.x; ++i){
			c = trace(0, scene->objs, scene->e, eyeray(scene, i, j));
			px = byteaddr(scene->img, (Point){i, j});
			px[0] = clamp(c.b) * 255;
			px[1] = clamp(c.g) * 255;
			px[2] = clamp(c.r) * 255;
		}
	}

	return scene->img;
}
