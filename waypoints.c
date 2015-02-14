#ifndef MAIN

#pragma config(Motor,  motorA,          InMotor,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl, encoder)

task main() {
	motor[motorB] = 100;
	motor[motorC] = -100;
	while (1) {}
}

typedef struct pt {
	float x;
	float y;
} point;

#endif

typedef point waypoint;

#define WPCOUNT 10

float edges[WPCOUNT][WPCOUNT];
waypoint waypoints[WPCOUNT];

void initWaypointGraph() {
	waypointCount = 0;
	for (int i = 0; i < WPCOUNT; i++)
		for (int j = 0; j < WPCOUNT; j++)
			edges[i][j] = -1.0;
}

int new_waypoint(float x, float y, int wp) {
	if (wp < WPCOUNT && waypoints[wp] < 0) {
		waypoints[wp].x = x;
		waypoints[wp].y = y;
	}
	return -1;
}

float dist(waypoint wp1, waypoint wp2) {
	float xdiff, ydiff;
	xdiff = wp1.x - wp2.x;
	ydiff = wp1.y - wp2.y;
	return sqrt(xdiff*xdiff + ydiff*ydiff);
}

void connect_waypoints(int wp1, int wp2) {
	float d = dist(waypoints[wp1], waypoints[wp2]);
	edges[wp1][wp2] = d;
	edges[wp2][wp1] = d;
}
