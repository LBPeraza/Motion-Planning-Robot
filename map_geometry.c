#define ROBOTR 4

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
		y = (r.corners[1].y - r.corners[0].y) * i / res + r.corners[0].y;
		setPixel(x, y);
		x = (r.corners[3].x - r.corners[2].x) * i / res + r.corners[2].x;
		y = (r.corners[3].y - r.corners[2].y) * i / res + r.corners[2].y;
		setPixel(x, y);
		x = (r.corners[2].x - r.corners[1].x) * i / res + r.corners[1].x;
		y = (r.corners[2].y - r.corners[1].y) * i / res + r.corners[1].y;
		setPixel(x, y);
		x = (r.corners[0].x - r.corners[3].x) * i / res + r.corners[3].x;
		y = (r.corners[0].y - r.corners[3].y) * i / res + r.corners[3].y;
		setPixel(x, y);
	}
}

#ifndef MAIN

#pragma config(Motor,  motorA,          InMotor,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl, encoder)

task main() {
	motor[motorB] = 100;
	motor[motorC] = -100;
	while (1) {}
}

#endif

bool segment_intersect(point s1, point e1, point s2, point e2){
	float x00 = s1.x;
	float y00 = s1.y;
	float x10 = s2.x;
	float y10 = s2.y;
	float x01 = e1.x - s1.x;
	float y01 = e1.y - s1.y;
	float x11 = e2.x - s2.x;
	float y11 = e2.y - s2.y;

	float det = x11*y01 - x01*y11;

	float s = 1/det * ((x00-x10)*y01 - (y00-y10)*x01);
	float t = 1/det * -(-(x00-x10)*y11 + (y00-y10)*x11);

	if(s >= 0 && s <= 1 && t >= 0 && t <= 1)
		return true;

	return false;
}

bool rectangle_intersect(point p1, point p2, rect r){
	for(int i = 0; i<4; i++){
		if(segment_intersect(p1, p2, r.corners[i], r.corners[(i+1)%4]))
			return true;
	}
	return false;
}

bool circle_intersect(point p1, point p2, circle c){
	vector segDir, circDir;
	segDir.x = p2.x - p1.x;
	segDir.y = p2.y - p1.y;
	circDir.x = c.c.x - p1.x;
	circDir.y = c.c.y - p1.y;

	vector projection;
	float segMag = sqrt(pow(p2.x-p1.x,2) + pow(p2.y-p1.y,2));

	float projLen = (circDir.x*segDir.x + circDir.y*segDir.y) / segMag;
	projection.x = projLen * segDir.x / segMag;
	projection.y = projLen * segDir.y / segMag;
	point closest;
	if(projLen < 0){
		closest.x = p1.x;
		closest.y = p1.y;
	}
	else if(projLen > segMag){
		closest.x = p2.x;
		closest.y = p2.y;
	}
	else{
		closest.x = p1.x + projection.x;
		closest.y = p1.y + projection.y;
	}

	if(sqrt(pow(c.c.x-closest.x,2) + pow(c.c.y-closest.y,2)) <= c.r){
		return true;
	}
	return false;
}
