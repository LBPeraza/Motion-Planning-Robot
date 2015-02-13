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

void drawCircle(circle c) {
	for (int angle = 0; a < 360; a += 10) {
		int x = c.x + (int)(round(c.r * cosDegrees(a)));
		int y = c.y + (int)(round(c.r * sinDegrees(a)));
		setPixel(x, y);
	}
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

void drawRect(rect r) {
	point corners[4] = r.corners;
	const int res1, res2;
	int x, y;
	res1 = 6;	res2 = 24;
	for (int i = 0; i <= res1; i++) {
		x = (corners[1].x - corners[0].x) * i / res1 + corners[0].x;
		y = (corners[1].y - corners[0].y) * i / res1 + corners[0].x;
		setPixel(x, y);
		x = (corners[3].x - corners[2].x) * i / res1 + corners[2].x;
		y = (corners[3].y - corners[2].y) * i / res1 + corners[2].y;
		setPixel(x, y);
	}
	for (int i = 0; i <= res2; i++) {
		x = (corners[2].x - corners[1].x) * i / res2 + corners[1].x;
		y = (corners[2].y - corners[1].y) * i / res2 + corners[1].y;
		setPixel(x, y);
		x = (corners[0].x - corners[3].x) * i / res2 + corners[3].x;
		y = (corners[0].y - corners[3].x) * i / res2 + corners[3].y;
		setPixel(x, y);
	}
}
