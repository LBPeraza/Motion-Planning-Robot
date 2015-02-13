#define ROBOTR 3

typedef struct coord {
	float x;
	float y;
} point;

typedef struct circ {
	point c;
	int r;
} circle;

typedef struct rect {
	point p1;
	point p2;
	point p3;
	point p4;
} rect;

void new_circle(circle *c, point center, int radius) {
	c->c.x = center.x;
	c->c.y = center.y;
	c->r = radius + ROBOTR;
}

void new_rect(rect *r, point p1, point p2, point p3, point p4) {

}
