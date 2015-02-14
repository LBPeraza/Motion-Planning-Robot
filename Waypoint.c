#pragma config(Motor,  motorA,          InMotor,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define MAIN 1

#include "map_geometry.c"
#include "waypoints.c"

#define SOUND_ON 0
#define CHOOSE_K 0

#define R 2.76
#define L 12.7
#define F 7.3

#define K 7.0

#define PI 3.1415926536
#define numPoints 73

#define MAPWIDTH 96
#define MAPHEIGHT 48

float robot_TH = 0.0;
point robot_c, marker_c, marker_d;
float marker_vd, marker_wd;
int oldL = 0, oldR = 0, oldT;
int velocityUpdateInterval = 20;
int PIDUpdateInterval = 2;

float kp;
float l;

float max(float a, float b){
	if(a > b)
		return a;
	return b;
}

/*****************************************
 * Initiate obstacles
 *****************************************/

void initObstacles() {
	// Circles
	point center;
	center.x = 30; center.y = 30;
	new_circle(&circleObstacles[0], center, 3);

	center.x = 60; center.y = 12;
	new_circle(&circleObstacles[1], center, 3);

	center.x = 66; center.y = 39;
	new_circle(&circleObstacles[2], center, 3);

	// Rectangles
	point corners[4];
	corners[0].x = 0; corners[0].y = 24;
	corners[1].x = 22; corners[1].y = 14;
	corners[2].x = 24; corners[2].y = 20;
	corners[3].x = 2; corners[3].y = 30;
	new_rect(&rectObstacles[0], corners);

	corners[0].x = 33; corners[0].y = 18;
	corners[1].x = 37; corners[1].y = 13;
	corners[2].x = 55; corners[2].y = 28;
	corners[3].x = 52; corners[3].y = 33;
	new_rect(&rectObstacles[1], corners);

	corners[0].x = 78; corners[0].y = 24;
	corners[1].x = 90; corners[1].y = 46;
	corners[2].x = 84; corners[2].y = 48;
	corners[3].x = 72; corners[3].y = 27;
	new_rect(&rectObstacles[2], corners);

	// Walls
	corners[0].x = -1; corners[0].y = 0;
	corners[1].x = 0; corners[1].y = 0;
	corners[2].x = 0; corners[2].y = MAPHEIGHT;
	corners[3].x = -1; corners[3].y = MAPHEIGHT;
	new_rect(&rectObstacles[3], corners);

	corners[0].x = 0; corners[0].y = -1;
	corners[1].x = MAPWIDTH; corners[1].y = -1;
	corners[2].x = MAPWIDTH; corners[2].y = 0;
	corners[3].x = 0; corners[3].y = 0;
	new_rect(&rectObstacles[4], corners);

	corners[0].x = MAPWIDTH; corners[0].y = 0;
	corners[1].x = MAPWIDTH+1; corners[1].y = 0;
	corners[2].x = MAPWIDTH+1; corners[2].y = MAPHEIGHT;
	corners[3].x = MAPWIDTH; corners[3].y = MAPHEIGHT;
	new_rect(&rectObstacles[5], corners);

	corners[0].x = 0; corners[0].y = MAPHEIGHT;
	corners[1].x = MAPWIDTH; corners[1].y = MAPHEIGHT;
	corners[2].x = MAPWIDTH; corners[2].y = MAPHEIGHT+1;
	corners[3].x = 0; corners[3].y = MAPHEIGHT+1;
	new_rect(&rectObstacles[6], corners);
}

/*****************************************
 * Initiate waypoints
 *****************************************/

void initWaypoints() {
	initWaypoint();

	new_waypoint(12, 6, 0);
	new_waypoint(42, 4, 1);
	new_waypoint(84, 4, 2);
	new_waypoint(65, 25, 3);
	new_waypoint(54, 42, 4);
	new_waypoint(12, 42, 5);

	connect_waypoints();

	draw_paths();
}

/*****************************************
 * Complete this function so that it
 * continuously updates the robot's position
 *****************************************/
task trajectory_task()
{
	float t = 0.0;
	oldL = nMotorEncoder[LeftMotor];
	oldR = nMotorEncoder[RightMotor];
	oldT = nPgmTime;
	int loops = 0;
	while(1)
	{
		int curTime = nPgmTime;
		int leftEnc = nMotorEncoder[LeftMotor];
		int rightEnc = nMotorEncoder[RightMotor];
		float dt = (curTime - oldT) / 1000.0;
		t += dt;
		if(dt == 0.0){
			continue;
		}
		loops++;
		float vl = (leftEnc - oldL) / dt * PI / 180.0 * R;
		float vr = (rightEnc - oldR) / dt * PI / 180.0 * R;

		float v = (vr + vl) / 2.0;

		float w = (vr - vl) / l;

		float k00 = v*cos(robot_TH);
		float k01 = v*sin(robot_TH);
		float k02 = w;
		float k10 = v*cos(robot_TH + k02*.5*dt);
		float k11 = v*sin(robot_TH + k02*.5*dt);
		float k12 = w;
		float k20 = v*cos(robot_TH + k12*.5*dt);
		float k21 = v*sin(robot_TH + k12*.5*dt);
		float k22 = w;
		float k30 = v*cos(robot_TH + k22*dt);
		float k31 = v*sin(robot_TH + k22*dt);
		float k32 = w;

		robot_c.x += dt/6.0 * (k00 + 2*(k10 + k20) + k30);
		robot_c.y += dt/6.0 * (k01 + 2*(k11 + k21) + k31);
		robot_TH += dt/6.0 * (k02 + 2*(k12 + k22) + k32);

		marker_c.x = robot_c.x + F * cos(robot_TH);
		marker_c.y = robot_c.y + F * sin(robot_TH);

		float xErr = marker_d.x - marker_c.x;
		float yErr = marker_d.y - marker_c.y;

		marker_vd = kp*(xErr*cos(robot_TH) + yErr*sin(robot_TH));
		marker_wd = kp*(xErr * -sin(robot_TH)/F + yErr * cos(robot_TH)/F);

		float vld = 90.0 / PI * (2.0*marker_vd - L*marker_wd) / R;
		float vrd = 90.0 / PI * (2.0*marker_vd + L*marker_wd) / R;

		motor[motorB] = 0.1 * vld;
		motor[motorC] = 0.1 * vrd;

		//Code that plots the robot's current position and also prints it out as text
		setPixel(50 + (int)(100.0 * marker_c.x), 32 + (int)(100.0 * marker_c.y));
		displayTextLine(0, "X: %f", marker_c.x);
		displayTextLine(1, "Y: %f", marker_c.y);
		displayTextLine(2, "xd: %f", marker_d.x);
		displayTextLine(3, "yd: %f", marker_d.y);
		displayTextLine(4, "t: %f", robot_TH * 180.0 / PI);
		displayTextLine(5, "x: %f", xErr);
		displayTextLine(6, "y: %f", yErr);

		wait1Msec(velocityUpdateInterval);
		oldL = leftEnc;
		oldR = rightEnc;
		oldT = curTime;
	}
}

float clamp (float x, float minimum, float maximum)
{
	if (x < minimum)
		return minimum;
	else if (x > maximum)
		return maximum;
	else
		return x;
}


task speedSounds(){
	while(1){
		float power = (abs(motor[motorB]) + abs(motor[motorC])) / 2.0;
		displayTextLine(7, "motor: %f", motor[motorB]);
		float freq = power/100.0 * (800 - 400);
		freq += 400;
		playTone((int)freq,5);
		sleep(3);
	}
}

void clearExtraPixels() {
	for (int i = MAPHEIGHT; i < 64; i++)
		for (int j = 0; j < 100; j++)
			clearPixel(j, i);
	for (int i = 0; i < 64; i++)
		for (int j = MAPWIDTH; j < 100; j++)
			clearPixel(j, i);
}

void drawMap() {
	for (int i = 0; i < CCOUNT; i++) {
		draw_circle(circleObstacles[i]);
	}
	for (int i = 0; i < RCOUNT; i++) {
		draw_rect(rectObstacles[i]);
	}
}

task closeWaypoint() {
	int leftEnc = nMotorEncoder[LeftMotor];
	int rightEnc = nMotorEncoder[RightMotor];
	int oldL = leftEnc;
	int oldR = rightEnc;
	point c;
	c.x = 6; c.y = 6;
	point e;
	e.x = 6; e.y = 6;
	while (1) {
		eraseLine(c.x, c.y, e.x, e.y);
		leftEnc = nMotorEncoder[LeftMotor];
		rightEnc = nMotorEncoder[RightMotor];
		int xdiff = leftEnc - oldL;
		int ydiff = rightEnc - oldR;
		c.x += xdiff/5;
		c.y += ydiff/5;
		displayTextLine(0, "%d, %d", c.x, c.y);
		int cw = closest_waypoint(c);
		if (cw >= 0) {
			if (waypoint_location(&e, cw)) {
				drawLine(c.x, c.y, e.x, e.y);
			}
		}
		oldL = leftEnc;
		oldR = rightEnc;
		wait1Msec(50);
	}
}

/*****************************************
 * Main function - it is not necessary to
 * modify this
 *****************************************/
task main()
{
  /* Reset encoders and turn on PID control */
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	nMotorPIDSpeedCtrl[motorB] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[motorC] = mtrSpeedReg;
	nPidUpdateInterval = PIDUpdateInterval;

  motor[motorB] = 0;
	motor[motorC] = 0;

	initObstacles();
	initWaypoints();

	drawMap();

	clearExtraPixels();

	if (SOUND_ON)
		startTask(speedSounds);

	//startTask(closeWaypoint);
	int path[6];
	int pathLen = get_path(path,0,5);
	while(nNxtButtonPressed != kExitButton) {}
}
