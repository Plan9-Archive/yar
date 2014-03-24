enum type
{
	PLANE,
	SPHERE,
	AGGR,
};

typedef struct Bbox Bbox;
struct Bbox
{
	Point3 min, max;
};

typedef struct Obj Obj;
struct Obj
{
	enum type type;
	Point3 p, n;
	double r;
	Obj *next, *chld;
	Bbox;
};

typedef struct Scene Scene;
struct Scene
{
	Obj *objs;
	Point3 e;
	Point3 u, v, w;
	double d;
	double l, r, t, b;
	Point s;
};

typedef struct Colour Colour;
struct Colour
{
	double r, g, b;
};

extern Scene *newscene(int w, int h);
extern Point3 eyeray(Scene *scene, int x, int y);

extern Colour trace(int depth, Obj *obj, Point3 e, Point3 d);

extern Bbox boundpt(Bbox b, Point3 pt);
extern Bbox boundbb(Bbox b, Bbox a);

extern Bbox spherebb(Point3 pos, double r);
extern Obj *newsphere(Point3 pos, double r);
extern double spherehit(Obj *sphere, Point3 e, Point3 d);

extern Obj *newplane(Point3 p, Point3 n);
extern double planehit(Obj *plane, Point3 p0, Point3 v);
