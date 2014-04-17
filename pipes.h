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
