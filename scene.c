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

	scene = malloc(sizeof(Scene));
	if(scene == nil)
		exits("malloc");

	scene->objs = newplane((Point3){0, -1, 0, 0}, (Point3){0, 1, 0, 0});
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

Memimage *
render(Scene *scene)
{
	Memimage *img;
	Colour c;
	int i, j;
	uchar *px;

	img = allocmemimage(Rpt(Pt(0, 0), scene->s), RGB24);

	for(j = 0; j < scene->s.y; ++j)
		for(i = 0; i < scene->s.x; ++i){
			c = trace(0, scene->objs, scene->e, eyeray(scene, i, j));
			px = byteaddr(img, (Point){i, j});
			px[0] = c.b * 255;
			px[1] = c.g * 255;
			px[2] = c.r * 255;
		}

	return img;
}
