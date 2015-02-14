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

#define WPCOUNT 8

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

int new_waypoint(const float x, const float y, const int wp) {
	if (wp < WPCOUNT) {
		waypoints[wp].x = x;
		waypoints[wp].y = y;
		waypoint_defined[wp] = true;
		return wp;
	}
	return -1;
}

float dist(const waypoint wp1, const waypoint wp2) {
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

bool waypoint_location(point *p, const int wp) {
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

int min_score(const float *scores, const int *indices, const int count) {
	float min_score = -1.0;
	int index = -1;
	for (int i = 0; i < count; i++) {
		if (index == -1 || scores[indices[i]] < min_score) {
			min_score = scores[indices[i]];
			index = indices[i];
		}
	}
	return index;
}

int get_neighbors(int *buf, const int wp) {
	for (int i = 0; i < WPCOUNT; i++)
		buf[i] = -1;
	if (!waypoint_defined[wp])
		return 0;
	int count = 0;
	for (int i = 0; i < WPCOUNT; i++)
		if (waypoint_defined[i] && i != wp &&	edges[wp][i] >= 0.0)
			buf[count++] = i;
	return count;
}

int get_path(int *path, const int wps, const int wpe) {
	if(wps < 0 || wps >= WPCOUNT || wpe < 0 || wpe >= WPCOUNT){
		return -1;
	}
	if(!waypoint_defined[wps] || !waypoint_defined[wpe]){
		return -1;
	}
	float scores[WPCOUNT];
	bool visited[WPCOUNT];
	int parent[WPCOUNT];
	for (int i = 0; i < WPCOUNT; i++){
		scores[i] = -1.0;
		visited[i] = false;
		parent[i] = -1;
	}
	scores[wpe] = 0.0;
	int current = wpe;
	int buf[WPCOUNT];
	int n;
	while (!visited[wps]) {
		n = get_neighbors(buf, current);
		float score = scores[current];
		for (int j = 0; j < n; j++) {
			float newScore = score + edges[current][buf[j]];
			if(!visited[buf[j]] && (scores[buf[j]] < 0 || newScore < scores[buf[j]])){
				scores[buf[j]] = newScore;
				parent[buf[j]] = current;
			}
		}
		visited[current] = true;

		int shortestInd = -1;
		float shortestDist = -1.0;
		for(int j = 0; j < WPCOUNT; j++){
			if(scores[j] != -1 && !visited[j] && (shortestDist < 0 || scores[j] < shortestDist)){
				shortestDist = scores[j];
				shortestInd = j;
			}
		}
		current = shortestInd;
	}

	int i = wps;
	int count = 0;
	while (i != wpe) {
		path[count++] = i;
		i = parent[i];
	}
	path[count++] = i;
	return count;
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
