#include <u.h>
#include <libc.h>
#include <thread.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"
#include "pipes.h"

Obj *
scene0(void)
{
	Obj *objs, *o;

	objs = newplane((Point3){0, -1, 0, 0}, (Point3){0, 1, 0, 0});
	o = newsphere((Point3){0, -0.5, -1, 0}, 0.5);
	objs->next = o;
	o->next = newsphere((Point3){1.5, 0, -2, 0}, 0.5);
	o = o->next;
	o->next = newsphere((Point3){-2, 2, -3, 0}, 1);
	o = o->next;
	o->next = newlight((Point3){3, 3, 0, 0}, 1, (Point3){-3, -3, -1, 0}, (Colour){0.40, 0.6, 0.6});
	o = o->next;
	o->next = newlight((Point3){-2, 4, 2, 0}, 1, (Point3){1, -3, -1, 0}, (Colour){0.4, 0.3, 0.40});
	o = o->next;
	o->next = newlight((Point3){-4, 6, -10, 0}, 1, (Point3){2, -4, 1, 0}, (Colour){0.5, 0.7, 0.4});

	return objs;
}

Obj *
scene1(void)
{
	Obj *objs, *o;

	objs = newplane((Point3){0, -1, 0, 0}, (Point3){0, 1, 0, 0});
	o = newplane((Point3){-5, 0, 0, 0}, (Point3){1, 0, 0, 0});
	objs->next = o;
	o->next = newsphere((Point3){-3, 3, -5, 0}, 0.75);
	o = o->next;
	o->next = newlight((Point3){-2, 10, -5, 0}, 1, (Point3){0.5, -2, 1, 0}, (Colour){0.6, 0.8, 0.8});
	o = o->next;
	o->next = newlight((Point3){2, 10, 5, 0}, 1, (Point3){-0.5, -1, -1, 0}, (Colour){0.8, 0.8, 0.6});
	o = o->next;
	o->next = newsphere((Point3){1, 0.5, -6, 0}, 4);

	return objs;
}

Obj *scene2(void)
{
	Obj *objs, *o;
	int i;

	objs = newplane((Point3){0, -1, -2, 0}, (Point3){0, 1, 0, 0});
	objs->next = newlight((Point3){3, 3, 3, 0}, 1, (Point3){-3, -3, -3, 0}, (Colour){0.8, 0.7, 0.8});
	o = objs->next;
	o->next = newlight((Point3){-3, 3, 3, 0}, 1, (Point3){3, -3, -3, 0}, (Colour){0.9, 0.9, 0.7});
	o = o->next;
	for(i = 0; i < 10; ++i){
		o->next = newsphere((Point3){i-3, 1, -5+((double)i/2.0), 0}, 0.5);
		o = o->next;
	}

	return objs;
}

Scene *
newscene(int scnid, int w, int h)
{
	Scene *scene;
	Obj *o;

	scene = malloc(sizeof(Scene));
	if(scene == nil)
		exits("malloc");

	scene->img = allocmemimage(Rect(0, 0, w, h), RGB24);
	if(scene->img == nil)
		exits("malloc");

	if(scnid == 1)
		scene->objs = scene1();
	else if(scnid == 2)
		scene->objs = scene2();
	else if(scnid == 3)
		scene->objs = nil;
	else
		scene->objs = scene0();

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

Pipescene *
pipescene(int w, int h)
{
	Pipescene *psc;
	int id;

	psc = malloc(sizeof(Pipescene));
	if(psc == nil)
		sysfatal("malloc: %r");

	id = 1;

	psc->scene = newscene(3, w, h);
	psc->pipes = conspipe(mkplanepipe(id++, (Point3){0, -1, 0, 0}, (Point3){0, 1, 0, 0}), nil);
	psc->pipes = conspipe(mkspherepipe(id++, (Point3){0, -0.5, -1, 0}, 0.5), psc->pipes);
	psc->pipes = conspipe(mkspherepipe(id++, (Point3){1.5, 0, -2, 0}, 0.5), psc->pipes);
	psc->pipes = conspipe(mkspherepipe(id++, (Point3){-2, 2, -3, 0}, 1), psc->pipes);
	psc->pipes = conspipe(mklightpipe(id++, (Point3){3, 3, 0, 0}, 1, (Point3){-3, -3, -1, 0},
		(Colour){0.40, 0.6, 0.6}), psc->pipes);
	psc->pipes = conspipe(mklightpipe(id++, (Point3){-2, 4, 2, 0}, 1, (Point3){1, -3, -1, 0},
		(Colour){0.4, 0.3, 0.40}), psc->pipes);
	psc->pipes = conspipe(mklightpipe(id++, (Point3){-4, 6, -10, 0}, 1, (Point3){2, -4, 1, 0},
		(Colour){0.5, 0.7, 0.4}), psc->pipes);

	return psc;
}

Point3
eyeray(Scene *scene, int x, int y)
{
	double u, v;
	Point3 ray;

	u = scene->l + (scene->r - scene->l) * ((double)x + 0.5) / (double)scene->s.x;
	v = scene->b + (scene->t - scene->b) * ((double)y + 0.5) / (double)scene->s.y;

	if(nsamples > 1){
		u += (0.5 - frand()) / 100.0;
		v += (0.5 - frand()) / 100.0;
	}

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
	int i, j, s;
	uchar *px;

	for(j = 0; j < scene->s.y; ++j){
		if((j % nproc) != id)
			continue;
		for(i = 0; i < scene->s.x; ++i){
			c = trace(0, scene->objs, scene->e, eyeray(scene, i, j));
			for(s = 1; s < nsamples; ++s){
				c = csum(c, trace(0, scene->objs, scene->e, eyeray(scene, i, j)));
			}
			c = cscale(c, 1.0/(double)nsamples);
			px = byteaddr(scene->img, (Point){i, j});
			px[0] = clamp(c.b) * 255;
			px[1] = clamp(c.g) * 255;
			px[2] = clamp(c.r) * 255;
		}
	}

	return scene->img;
}
