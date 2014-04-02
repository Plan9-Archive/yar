#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>
#include <geometry.h>

#include "scene.h"

Colour
csum(Colour a, Colour b)
{
	Colour c;

	c.r = a.r + b.r;
	c.g = a.g + b.g;
	c.b = a.b + b.b;

	return c;
}

Colour
cscale(Colour a, double b)
{
	Colour c;

	c.r = a.r * b;
	c.g = a.g * b;
	c.b = a.b * b;

	return c;
}

Colour
cmul(Colour a, Colour b)
{
	Colour c;

	c.r = a.r * b.r;
	c.g = a.g * b.g;
	c.b = a.b * b.b;

	return c;
}
