enum type
{
	PLANE,
	SPHERE,
	AGGR
};

typedef struct Bbox Bbox;
struct Bbox
{
	Point3 min, max;
};

extern Bbox boundpt(Bbox b, Point3 pt);
extern Bbox boundbb(Bbox b, Bbox a);
