extern int nnodes, node, nproc;
extern int nsamples;
extern int maxdepth;

enum type
{
	PLANE,
	SPHERE,
	AGGR,
	LIGHT,
};

typedef struct Bbox Bbox;
struct Bbox
{
	Point3 min, max;
};

typedef struct Colour Colour;
struct Colour
{
	double r, g, b;
};

typedef struct Obj Obj;
struct Obj
{
	enum type type;
	Point3 p, n;
	double r;
	Colour c;
	Obj *next, *chld;
	Bbox;
};

typedef struct Scene Scene;
struct Scene
{
	Memimage *img;
	Obj *objs;
	Point3 e;
	Point3 u, v, w;
	double d;
	double l, r, t, b;
	Point s;
};

typedef struct Hit Hit;
struct Hit
{
	double d;
	Point3 p;
	Point3 n;
	Point3 ie, id;
	Obj *o;
	Colour c;
};

extern Scene *newscene(int w, int h);
extern Point3 eyeray(Scene *scene, int x, int y);
extern Memimage *render(Scene *scene, int id);

extern Colour trace(int depth, Obj *obj, Point3 e, Point3 d);

extern Bbox boundpt(Bbox b, Point3 pt);
extern Bbox boundbb(Bbox b, Bbox a);

extern Bbox spherebb(Point3 pos, double r);
extern Obj *newsphere(Point3 pos, double r);
extern Hit spherehit(Obj *sphere, Point3 e, Point3 d);

extern Obj *newplane(Point3 p, Point3 n);
extern Hit planehit(Obj *plane, Point3 p0, Point3 v);

extern Obj *newlight(Point3 p, double r, Point3 n, Colour c);
extern Hit lighthit(Obj *light, Point3 e, Point3 d);

extern Colour csum(Colour a, Colour b);
extern Colour cscale(Colour a, double b);
extern Colour cmul(Colour a, Colour b);
