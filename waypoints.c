#ifndef MAIN

#pragma config(Motor,  motorA,          InMotor,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl, encoder)

task main() {
	motor[motorB] = 100;
	motor[motorC] = -100;
	while (nNxtButtonPressed != kExitButton) {}
}

typedef struct pt {
	float x;
	float y;
} point;

bool safe_path(point wp1, point wp2) {
	return true;
}

#endif

typedef point waypoint;

#define WPCOUNT 6

bool waypoint_defined[WPCOUNT];
float edges[WPCOUNT][WPCOUNT];
waypoint waypoints[WPCOUNT];

void initWaypoint() {
	for (int i = 0; i < WPCOUNT; i++) {
		waypoint_defined[i] = false;
		for (int j = 0; j < WPCOUNT; j++)
			edges[i][j] = -1.0;
	}
}

int new_waypoint(float x, float y, int wp) {
	if (wp < WPCOUNT && !waypoint_defined[wp]) {
		waypoints[wp].x = x;
		waypoints[wp].y = y;
		waypoint_defined[wp] = true;
		return wp;
	}
	return -1;
}

float dist(waypoint wp1, waypoint wp2) {
	float xdiff, ydiff;
	xdiff = wp1.x - wp2.x;
	ydiff = wp1.y - wp2.y;
	return sqrt(xdiff*xdiff + ydiff*ydiff);
}

bool _connect_waypoints(int wp1, int wp2) {
	if (waypoint_defined[wp1] && waypoint_defined[wp2] &&
			safe_path(waypoints[wp1], waypoints[wp2]) &&
			edges[wp1][wp2] < 0) {
		float d = dist(waypoints[wp1], waypoints[wp2]);
		edges[wp1][wp2] = d;
		edges[wp2][wp1] = d;
		return true;
	}
	return false;
}

int connect_waypoints() {
	int edge_count = 0;
	for (int i = 0; i < WPCOUNT; i++)
		for (int j = i+1; j < WPCOUNT; j++)
			if (_connect_waypoints(i, j))
				edge_count++;
	return edge_count;
}

bool waypoint_location(point *p, int wp) {
	if (waypoint_defined[wp]) {
		p->x = waypoints[wp].x;
		p->y = waypoints[wp].y;
		return true;
	}
	return false;
}

int closest_waypoint(point p) {
	float d;
	float minDist = -1.0;
	int minInd = -1;
	for (int i = 0; i < WPCOUNT; i++) {
		if (waypoint_defined[i] && safe_path(waypoints[i], p)) {
			d = dist(p, waypoints[i]);
			if (d < minDist || minDist < 0) {
				minDist = d;
				minInd = i;
			}
		}
	}
	return minInd;
}

void draw_paths() {
	for (int i = 0; i < WPCOUNT; i++) {
		if (waypoint_defined[i])
			drawCircle(waypoints[i].x - 2, waypoints[i].y + 2, 4);
		for (int j = i+1; j < WPCOUNT; j++) {
			if (edges[i][j] >= 0) {
				drawLine(waypoints[i].x, waypoints[i].y, waypoints[j].x, waypoints[j].y);
			}
		}
	}
}
