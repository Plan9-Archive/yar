#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"

Bbox
boundpt(Bbox b, Point3 pt)
{
	if(pt.x < b.min.x)
		b.min.x = pt.x;
	if(pt.y < b.min.y)
		b.min.y = pt.y;
	if(pt.z < b.min.z)
		b.min.z = pt.z;
	if(pt.w < b.min.w)
		b.min.w = pt.w;

	if(pt.x > b.max.x)
		b.max.x = pt.x;
	if(pt.y > b.max.y)
		b.max.y = pt.y;
	if(pt.z > b.max.z)
		b.max.z = pt.z;
	if(pt.w > b.max.w)
		b.max.w = pt.w;

	return b;
}

#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)

Bbox
boundbb(Bbox b, Bbox a)
{
	b.min.x = MIN(b.min.x, a.min.x);
	b.min.y = MIN(b.min.y, a.min.y);
	b.min.z = MIN(b.min.z, a.min.z);
	b.min.w = MIN(b.min.w, a.min.w);

	b.max.x = MAX(b.max.x, a.max.x);
	b.max.y = MAX(b.max.y, a.max.y);
	b.max.z = MAX(b.max.z, a.max.z);
	b.max.w = MAX(b.max.w, a.max.w);

	return b;
}
