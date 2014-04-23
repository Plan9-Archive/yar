typedef struct Hitpipe Hitpipe;
struct Hitpipe
{
	enum type type;
	int id;
	Channel *req;
	Hitpipe *next;
};

typedef struct Hitreq Hitreq;
struct Hitreq
{
	int depth;
	Point3 e;
	Point3 d;
	Channel *resp;
};

typedef struct Hitresp Hitresp;
struct Hitresp
{
	Hit;
	Channel *oc;
};

typedef struct Pipescene Pipescene;
struct Pipescene
{
	Scene *scene;
	Hitpipe *pipes;
};

extern Hitpipe *mkobjpipe(int id, Obj *o);
extern Hitpipe *conspipe(Hitpipe *car, Hitpipe *cdr);

extern Hitpipe *mkspherepipe(int id, Point3 pos, double r);
extern Hitpipe *mkplanepipe(int id, Point3 p, Point3 n);
extern Hitpipe *mklightpipe(int id, Point3 p, double r, Point3 n, Colour c);

extern Pipescene *pipescene(int w, int h);
