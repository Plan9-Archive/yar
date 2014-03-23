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
	Point3 pn, pp;
	double d;
	Rectangle r;
};

extern Bbox boundpt(Bbox b, Point3 pt);
extern Bbox boundbb(Bbox b, Bbox a);

extern Bbox spherebb(Point3 pos, double r);
extern Obj *newsphere(Point3 pos, double r);
