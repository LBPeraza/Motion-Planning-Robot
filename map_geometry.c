#define ROBOTR 3

typedef struct coord {
	float x;
	float y;
} point;

typedef point vector;

typedef struct circle {
	point c;
	int r;
} circle;

typedef struct rect {
	point corners[4];
} rect;

void new_circle(circle *circ, point center, int radius) {
	circ->c.x = center.x;
	circ->c.y = center.y;
	circ->r = radius + ROBOTR;
}

void draw_circle(circle circ) {
	for (int a = 0; a < 360; a += 10) {
		int x = circ.c.x + (int)(round(circ.r * cosDegrees(a)));
		int y = circ.c.y + (int)(round(circ.r * sinDegrees(a)));
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

		//nxtDisplayTextLine(i+4,"(%.1f, %.1f)", norm.x, norm.y);

	  r->corners[i].x += norm.x * ROBOTR;
	  r->corners[i].y += norm.y * ROBOTR;
	  r->corners[(i+1)%4].x += norm.x * ROBOTR;
	  r->corners[(i+1)%4].y += norm.y * ROBOTR;
	}
}

void draw_rect(rect r) {
	const int res = 100;
	int x, y;
	for (int i = 0; i <= res; i++) {
		x = (r.corners[1].x - r.corners[0].x) * i / res + r.corners[0].x;
		y = (r.corners[1].y - r.corners[0].y) * i / res + r.corners[0].x;
		setPixel(x, y);
		x = (r.corners[3].x - r.corners[2].x) * i / res + r.corners[2].x;
		y = (r.corners[3].y - r.corners[2].y) * i / res + r.corners[2].y;
		setPixel(x, y);
		x = (r.corners[2].x - r.corners[1].x) * i / res + r.corners[1].x;
		y = (r.corners[2].y - r.corners[1].y) * i / res + r.corners[1].y;
		setPixel(x, y);
		x = (r.corners[0].x - r.corners[3].x) * i / res + r.corners[3].x;
		y = (r.corners[0].y - r.corners[3].x) * i / res + r.corners[3].y;
		setPixel(x, y);
	}
}
