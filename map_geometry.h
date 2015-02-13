#define ROBOTR 3

typedef struct coord {
	float x;
	float y;
} point;

typedef point vector;

typedef struct circ {
	point c;
	int r;
} circle;

typedef struct rect {
	point corners[4];
} rect;

void new_circle(circle *c, point center, int radius) {
	c->c.x = center.x;
	c->c.y = center.y;
	c->r = radius + ROBOTR;
}

void new_rect(rect *r, point *ps) {
	point corners[4];
	for (int i = 0; i < 4; i++) {
		corners[i].x = ps[i].x;
		corners[i].y = ps[i].y;
	}
	for(int i = 0; i<4; i++){
		r->corners[i].x = corners[i].x;
		r->corners[i].y = corners[i].y;
	}
	for(int i = 0; i<4; i++){
		point c0, c1, c2;
		c0.x = corners[i].x;
		c0.y = corners[i].y;
		c1.x = corners[(i+1)%4].x;
		c1.y = corners[(i+1)%4].y;

		float dist = sqrt(pow(c0.x-c1.x, 2) + pow(c1.y-c0.y, 2));
		vector norm;
		norm.y = (c0.x - c1.x) / dist;
		norm.x = (c1.y - c0.y) / dist;

		nxtDisplayTextLine(i+4,"(%.1f, %.1f)", norm.x, norm.y);

	  r->corners[i].x += norm.x * ROBOTR;
	  r->corners[i].y += norm.y * ROBOTR;
	  r->corners[(i+1)%4].x += norm.x * ROBOTR;
	  r->corners[(i+1)%4].y += norm.y * ROBOTR;
	}
}
