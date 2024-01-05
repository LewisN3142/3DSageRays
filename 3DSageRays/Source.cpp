// When starting OpenGl project add to Project->Properties->Linker->Input the libraries opengl32.lib, glut32.lib, glu32.lib (remove 32 for x64 apps)

#include <iostream>
#include "glut.h"
#include <cmath>

#include "./Textures/DoomWall.ppm"

const float pi = 3.14159265358979323846f;
const float degRad = 0.017453292519943295;

// Maths Functions -----------------

float modulo(float a, float b) { return fmodf(fmodf(a,b)+b,b); }
float fixAng(float a) { return modulo(a,2*pi); } // Fix angle to mod 2pi
float degToRad(float a) { return a*degRad; } // Switch from degrees to radians
float dist(float ax, float ay, float bx, float by) { return(sqrt((bx - ax)*(bx-ax) + (by - ay)*(by - ay))); } // Compute distance

float px, py, pdx, pdy, pa;  // Player position, change in position, and angle
float f1, f2, fps;
float pSpeed = 0.2F; // Player base movement speed

int screenWidth = 480;
int screenHeight = 320; 
int threeOffset = 528;  

// Store possible key inputs --------

typedef struct
{
	int w, a, s, d, slash, e;
}ButtonKeys; ButtonKeys Keys;


// Checkerboard textures

int All_Textures[] =               //all 32x32 textures
{
	//Checkerboard
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	//Brick
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	//Window
	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,

	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,

	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
	//Door
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,

	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,1, 1,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,1,1,1,1,0,1, 1,0,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
};



// Wall map (floors and ceilings to be added) --------

int mapX = 8, mapY = 8, mapS = 64; // 8x8 grid of squares where each square is 64x64 units in size 
int mapW[] = // Sample map as a row of 64 elements
{
1,1,1,1,1,1,1,1,
1,0,0,0,4,0,0,1,
1,0,0,0,1,0,0,1,
1,0,0,0,1,1,1,1,
1,0,0,0,0,4,0,1,
1,0,0,0,0,2,0,1,
1,0,0,0,0,2,0,1,
1,1,1,1,1,1,1,1
}; // Could easily change to an array of characters so W wall, D door etc

int mapFloor[] = // Sample map as a row of 64 elements
{
1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,2,2,2,1,
1,0,0,0,0,0,1,1,
1,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1
}; // Could easily change to an array of characters so W wall, D door etc

int mapCeil[] = // Sample map as a row of 64 elements
{
1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,2,2,2,1,
1,0,0,0,0,0,1,1,
1,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1
}; // Could easily change to an array of characters so W wall, D door etc



// Draw the map using openGL

void drawMap2D()
{
	int x, y, xo, yo; // Indices for entry in cube and pixel in grid
	for (y = 0; y < mapY; y++)
	{
		for (x = 0; x < mapX; x++)
		{
			if (mapW[y * mapX + x] == 0) { glColor3f(0, 0, 0); } // If the map data in position (x,y) is 1, colour set map colour black
			else { glColor3f(1, 1, 1); } // Otherwise set map colour to zero
			xo = x * mapS; yo = y * mapS; // Create offset positions for x and y based on size of a block, to get the x and y coordinates of the top left vertex
			glBegin(GL_QUADS); // Begin rendering vertices as corners of quads, starting with bottom left and ending on bottom right, and fill the quad with map colour
			glVertex2i(0 + xo + 1, 0 + yo + 1);
			glVertex2i(0 + xo + 1, yo + mapS - 1);
			glVertex2i(xo + mapS - 1, yo + mapS - 1);
			glVertex2i(xo + mapS - 1, 0 + yo + 1);
			glEnd();
		}
	}
}


// Draw the player using openGL

void drawPlayer2D()
{
	glColor3f(1, 1, 0); // Set colour of created vertex, no alpha channel
	glPointSize(8); // Create vertices with diameter 8 pixels
	glBegin(GL_POINTS); // Begin rendering of enclosed vertices as points, as described above
	glVertex2i(px, py); // Specify position of the point to be rendered
	glEnd();

	glLineWidth(3); // Draw a normal pointing line of length pSpeed*(pdx^2 +pdy^2)
	glBegin(GL_LINES);
	glVertex2i(px, py);
	glVertex2i(px + 50*pSpeed * pdx, py + 50*pSpeed * pdy);
	glEnd();
}

// Function to draw rays ---------------------------------

void drawRays2D()
{
	glColor3f(0, 1, 1); glBegin(GL_QUADS); glVertex2i(528, 0); glVertex2i(528+screenWidth, 0); glVertex2i(528+screenWidth, 160); glVertex2i(528, 160); glEnd();
	glColor3f(0, 0, 1); glBegin(GL_QUADS); glVertex2i(528, 160); glVertex2i(528+screenWidth, 160); glVertex2i(528+screenWidth, 320); glVertex2i(528, 320); glEnd();


	int fov = 60; float rayRes = 0.5;  // Field of view in degrees and the proximity of the rays (ratio is the number of rays)
	int r, mx, my, mp, dof; float vx, vy, rx, ry, ra, xo, yo, distV, distH;
	// int side;
	ra = fixAng(pa + fov * degRad / 2); // Ray starts fov*rayRes/2 degrees acw of player normal
                                                             

	for (r = 0; r < fov/rayRes; r++)
	{
		int mv = 0, mh = 0;                                                              //vertical and horizontal map texture number 
		//---Vertical--- 
		dof = 0; distV = 100000;
		// side =0;
		float raTan = tan(ra);
		if (cos(ra) > 0.001) { rx = (((int)px >> 6) << 6) + 64;      ry = (px - rx) * raTan + py; xo = 64; yo = -xo * raTan; }//looking left
		else if (cos(ra) < -0.001) { rx = (((int)px >> 6) << 6) - 0.0001; ry = (px - rx) * raTan + py; xo = -64; yo = -xo * raTan; }//looking right
		else { rx = px; ry = py; dof = 8; }                                                  //looking up or down. no hit  

		while (dof < 8)
		{
			mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx; // Divide ray end positions by 64 to get the location in the array
			if (mp > 0 && mp < mapX * mapY && mapW[mp]>0) { mv = mapW[mp]; dof = 8; distV = dist(px, py, rx, ry); }//hit         
			else { rx += xo; ry += yo; dof += 1; }                                               //check next horizontal
		}

		vx = rx; vy = ry;

		//---Horizontal---
		dof = 0; distH = 100000;
		raTan = 1.0 / raTan;
		if (sin(ra) > 0.001) { ry = (((int)py >> 6) << 6)-0.0001; rx = (py - ry) *raTan + px; yo = -64; xo = -yo *raTan; }//looking up 
		else if (sin(ra) < -0.001) { ry = (((int)py >> 6) << 6) + 64;      rx = (py - ry) *raTan + px; yo = 64; xo = -yo*raTan; }//looking down
		else { rx = px; ry = py; dof = 8; }                                                   //looking straight left or right

		while (dof < 8)
		{
			mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && mapW[mp]>0) { mh = mapW[mp]; dof = 8; distH = dist(px, py, rx, ry); }//hit         
			else { rx += xo; ry += yo; dof += 1; }                                               //check next horizontal
		}

		// Render 2D rays -------------------

		float horiShade = 0.40F;
		float shade = horiShade;
		float vertShade = 0.9F;
		glColor3f(0, 1*horiShade, 0);
		if (distV >= distH && mh == 2) { glColor3f(1 * horiShade, 0, 0);}
		if (distV >= distH && mh == 4) { glColor3f(1 * horiShade, 1 * horiShade, 0); }
		if (distV < distH) { mh = mv; rx = vx; ry = vy; distH = distV; glColor3f(0, 1 * vertShade, 0); shade = vertShade; if (mh == 2) { glColor3f(1 * vertShade, 0, 0); } if (mh == 4) { glColor3f(1 * vertShade, 1 * vertShade, 0); } }//horizontal hit first
		glLineWidth(3); glBegin(GL_LINES); glVertex2i(px, py); glVertex2i(rx, ry); glEnd();//draw 2D ray


	   // Draw the 3D portion of the scene

		float ca = modulo(ra - pa, 2 * pi); distH = distH * cos(ca);
		float lineH = (mapS * screenHeight) / distH; // Formula is cube size times max line height divided by ray length

		float tyStep = 32.0 / lineH; // Step size for texture renderer - number of colourable surfaces on a line of height lineH
		float tyOff = 0; // Offset to account for clipping of line height when moving off screen

		if (lineH > screenHeight) { tyOff = (lineH - screenHeight) / 2.0; lineH = screenHeight; } // Update offset to half the difference between the screen and line height to shift texture back down. Clip line height 
		float lineOff = (screenHeight - lineH) / 2; // Offset lines so the ray points to the centre, centering the rays on the screen (half of (line height + screen height))

		int y;
		float ty = tyOff * tyStep; //+ 32 * (mh-1);
		float tx;
		
		if(shade == horiShade){ tx = (int)(rx / 2.0)%32 ; if (ra >= pi) { tx = 31 - tx; }}
		// Flip the textures when we are facing south so they are not mirrored
		else { tx = (int)(ry / 2.0)%32; if (ra > pi / 2 && ra <= 3 * pi / 2) { tx = 31 - tx; }}
		 // Flip the left facing textures


		// Get warning in VS here, nothing wrong, works fine

		for (y = 0; y < lineH; y++)
		{
			float c = All_Textures[(int)(ty) * 32 + (int)(tx)] * shade; // 32 here is the wall height in texture pixel cells - each texture is 4x4 blocks of 8x8
			glColor3f(c, c, c);
			if (mh == 1) { glColor3f(0, c, 0); }
			if (mh == 2) { glColor3f(c, 0, 0); }
			if (mh == 4) { glColor3f(c, c, 0); }
			glPointSize((screenWidth / fov) * rayRes); glBegin(GL_POINTS); glVertex2i((r + 0.5) * (screenWidth / fov) * rayRes + threeOffset, y + lineOff); glEnd();
			
			// Draw lines of width 8 for each ray cast - offset horizontally by 530 to plot next to 2d map. The line is the height dictated above

			//int pixel = (ty * 32 + tx) * 3;
			//int red = T1[pixel + 0]*shade;
			//int green = T1[pixel + 1]*shade;
			//int blue = T1[pixel + 2]*shade;
			//glPointSize((screenWidth / fov) * rayRes); glColor3ub(red, green, blue); glBegin(GL_POINTS); glVertex2i((r + 0.5) * (screenWidth / fov) * rayRes + threeOffset, y + lineOff); glEnd();
			ty += tyStep;
			
		}

		// Drawing floors

		for (y = lineOff + lineH; y < screenHeight; y++)
		{
			float dy = y - (screenHeight / 2.0), raNew = cos(fixAng(pa - ra));
			tx =  px/2 + cos(ra) * 158 * 32 / dy / raNew;
			ty = py/2 - sin(ra) * 158 *  32 / dy/ raNew;
			int mp = mapFloor[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32; // Choose new texture based on floor array
			float c = All_Textures[((int)(ty) & 31) * 32 + ((int)(tx) & 31)+mp-1] * 0.7;
			glColor3f(c/1.3 , c/1.3 , c); glPointSize((screenWidth/fov)*rayRes); glBegin(GL_POINTS); glVertex2i((r+0.5) * (screenWidth/fov)*rayRes + threeOffset, y); glEnd();

			mp = mapCeil[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32; // Choose new texture based on floor array
			c = All_Textures[((int)(ty) & 31) * 32 + ((int)(tx) & 31) + mp - 1] * 0.7;
			glColor3f(c / 1.2, c / 1.2, c/2.0); glPointSize((screenWidth / fov) * rayRes); glBegin(GL_POINTS); glVertex2i((r + 0.5) * (screenWidth / fov) * rayRes + threeOffset, screenHeight - y); glEnd();
		}

		ra = fixAng(ra - rayRes * degRad); //go to next ray, fov/rayRes total
	}
}//-----------------------------------------------------------------------------



// Initialise draw parameters ---------------

void init()
{
	glClearColor(0.3, 0.3, 0.3, 1); // Set default color to dark grey (RGBa between 0 and 1 not 0 255)
	gluOrtho2D(0, 1024, 512, 0); // Creates 2D orthographic/ isometric projection matrix (defines limits of visible area for rendering - maps 0 to left and top, 1024 to right, 512 to bottom)
	px = 150; py = 150; pdx = cos(pa); pdy = sin(pa); // Initialise player position
}

// Display function ------------------------

void display()
{

	f2 = glutGet(GLUT_ELAPSED_TIME); fps = f2 - f1; f1 = glutGet(GLUT_ELAPSED_TIME); // Calculate the running fps and compensate for it later
	float turnSen = 0.002F; // Sensitivity of turning (add to settings manager)

	if (Keys.a == 1 && Keys.slash == 0) { pa = fixAng(pa + turnSen * fps); pdx = cos(pa); pdy = -sin(pa); }  // If not pressing \ and pressing side keys then turn (may be why can clip through walls moving sideways)
	if (Keys.d == 1 && Keys.slash == 0) { pa = fixAng(pa - turnSen * fps); pdx = cos(pa); pdy = -sin(pa); } // !!! could add in sprinting too 

	int offVal = 20;

	int xo = 0; if (pdx < 0) { xo = -offVal; }
	else { xo = offVal; }                                    //x offset to check map
	int yo = 0; if (pdy < 0) { yo = -offVal; }
	else { yo = offVal; }                                    //y offset to check map

	int* PTRxo = &xo;
	int* PTRyo = &yo;

	int ipx = px / 64.0, ipxOffPlus = (px + *PTRxo) / 64.0, ipxOffMinus = (px - *PTRxo) / 64.0;             // works out what x block the player is in plus what block is offsetVal either side
	int ipy = py / 64.0, ipyOffPlus = (py + *PTRyo) / 64.0, ipyOffMinus = (py - *PTRyo) / 64.0;             // works out what y block the player is in plus what block is offsetVal either side
	// Update these to use pointers to xo and yo so that one does not need to recall them later

	if (Keys.w == 1)                                                                  //move forward
	{
		if (mapW[ipy * mapX + ipxOffPlus ] == 0) { px += pdx * pSpeed * fps; }
		if (mapW[ipyOffPlus *mapX + ipx] == 0) { py += pdy * pSpeed * fps; }
	}
	if (Keys.s == 1)                                                                  //move backward
	{
		if (mapW[ipy * mapX + ipxOffMinus] == 0) { px -= pdx * pSpeed * fps; }
		if (mapW[ipyOffMinus * mapX + ipx] == 0) { py -= pdy * pSpeed * fps; }
	}
	if (Keys.a == 1 && Keys.slash == 1) // Strafe left
	{
		if (mapW[ipy * mapX + ipxOffPlus] == 0) { px += pSpeed * pdy*fps; }
		if (mapW[ipyOffMinus * mapX + ipx] == 0) { py -= pSpeed * pdx*fps; }
	}
	if (Keys.d == 1 && Keys.slash == 1) // Strafe right
	{
		if (mapW[ipy * mapX + ipxOffMinus] == 0) { px -= pSpeed * pdy*fps; }
		if (mapW[ipyOffPlus * mapX + ipx] == 0) { py += pSpeed * pdx*fps; }
	}
	// Could modify the above to push the player away from the wall when near it, rather than not letting player move when near wall !!!

	
	if (Keys.e == 1)
	{
		offVal += 10;
		if (pdx < 0) { xo = -offVal; }
		else { xo = offVal; }                                    
		if (pdy < 0) { yo = -offVal; }
		else { yo = offVal; }

		if (mapW[ipyOffPlus * mapX + ipxOffPlus] == 4) { mapW[ipyOffPlus * mapX + ipxOffPlus] = 0; } 
		// Work out how to close door behind player (pointer global variable set to door position - if player position is not that or offset,then close door in this method or ray
	}



	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawRays2D();
	drawPlayer2D(); // !!! Issue here, player can, at present, spawn in a wall
	glutSwapBuffers();
}


// Register whether a key is pressed -------------

void buttonDown(unsigned char key, int x, int y) // Process player inputs as movement // Modify this to allow custom keybinds/ arrow keys - add strafing on shift/ alt key or turning on slash and c keys
{
	if (key == '\\') { Keys.slash = 1; }
	if (key == 'w') { Keys.w = 1; }
	if (key == 's') { Keys.s = 1; }
	if (key == 'd') { Keys.d = 1; }
	if (key == 'a') { Keys.a = 1; }
	if (key == 'e') { Keys.e = 1; }
	glutPostRedisplay(); // Marks window as needing to be redisplayed for renderer
}

void buttonUp(unsigned char key, int x, int y) // Process player inputs as movement // Modify this to allow custom keybinds/ arrow keys - add strafing on shift/ alt key or turning on slash and c keys
{
	if (key == '\\') { Keys.slash = 0; }
	if (key == 'w') { Keys.w = 0; }
	if (key == 's') { Keys.s = 0; }
	if (key == 'd') { Keys.d = 0; }
	if (key == 'a') { Keys.a = 0; }
	if (key == 'e') { Keys.e = 0; }
	glutPostRedisplay(); // Marks window as needing to be redisplayed for renderer
}

// Resize the window to default if stretched -------------

void resize(int w, int h) // Replace with window size options
{
	glutReshapeWindow(1024, 512);
}

// Main function calls the parts of openGL that are necessary

int main(int argc, char* argv[]) // main takes in an integer and a vector of characters (as a pointer to memory)
{

	glutInit(&argc, argv); // Initialise the glut library using variables from main
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // Initialise the display mode to have a double buffer (draw to back buffer, clear front and push back to front) and RGB with alpha
	glutInitWindowSize(1024, 512); // Set the size of the window - we want this to be at most the range of gluOrtho2D
	// !!! make this auto fullscreen/ set-able (also make a save file for the settings)
	glutInitWindowPosition(200, 200);
	glutCreateWindow("RayStation RS"); // Create a window with name as provided in the argument
	init();
	glutDisplayFunc(display); // Set function to be used to redisplay the window, here 'display'
	glutReshapeFunc(resize);
	glutKeyboardFunc(buttonDown); // Set function to be used to process button input
	glutKeyboardUpFunc(buttonUp);
	glutMainLoop(); // Enters the main loop of the Glut system (call at most once as does not return)

	return 0;

}