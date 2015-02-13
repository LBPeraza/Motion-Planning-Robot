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

void new_rect(rect *r, point p0, point p1, point p2, point p3) {
	point corners[4];
	corners[0].x = p0.x;
	corners[0].y = p0.y;
	corners[1].x = p1.x;
	corners[1].y = p1.y;
	corners[2].x = p2.x;
	corners[2].y = p2.y;
	corners[3].x = p3.x;
	corners[3].y = p3.y;
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
