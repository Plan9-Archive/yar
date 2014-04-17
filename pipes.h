typedef struct Hitpipe Hitpipe;
struct Hitpipe
{
	enum type type;
	Channel *c;
	Hitpipe *next;
};

typedef struct Hitreq Hitreq;
struct Hitreq
{
	int depth;
	Point3 e;
	Point3 d;
	Channel *c;
};

typedef struct Hitresp Hitresp;
struct Hitresp
{
	Hit;
	Channel *oc;
};

extern Hitpipe *mkobjpipe(Obj *o);

extern Hitpipe *mkspherepipe(Point3 pos, double r);
extern Hitpipe *mkplanepipe(Point3 p, Point3 n);
extern Hitpipe *mklightpipe(Point3 p, double r, Point3 n, Colour c);
