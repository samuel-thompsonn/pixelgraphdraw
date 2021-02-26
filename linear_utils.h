#pragma once
#include "settings.h"

static COLORREF redColor = RGB(255, 0, 0);
static COLORREF blackColor = RGB(0, 0, 0);
static COLORREF darkColor = RGB(50, 50, 50);
static COLORREF lightColor = RGB(130, 130, 130);
static COLORREF whiteColor = RGB(255, 255, 255);
static COLORREF yellowColor = RGB(255, 170, 0);
static COLORREF blueColor = RGB(0, 0, 255);
static COLORREF greenColor = RGB(43, 164, 24);

class vector3 {

public:
	float x, y, z;
	vector3();
	vector3(float, float, float);
	float magnitude();
	vector3 addVector(vector3);
	vector3 subVector(vector3);
	vector3 scaleVector(float);
	vector3 vec3CrossProduct(vector3);
};

float dotProduct(vector3, vector3);

class vector2 {
public:
	float x, y;
	vector2();
	vector2(float, float);
};

class mat3x2 {
public:
	vector3 firstRow, secondRow;
	mat3x2(	float, float, float,
			float, float, float);
	mat3x2(vector3, vector3);
	vector2 vecMult(vector3);
};

class mat3x3 {
public:
	vector3 firstRow, secondRow, thirdRow;
	mat3x3(float, float, float,
		float, float, float,
		float, float, float);
	mat3x3(vector3, vector3, vector3);
	vector3 vecMult(vector3);
	void vecTransf(vector3*);
};

float getDepth(vector3, vector3, vector3);
vector3 getVertsNormal(vector3, vector3, vector3);

class triOld {
public:
	vector3 vert1, vert2, vert3;
	float depth;
	triOld(vector3, vector3, vector3);
	void calcDepth();
	vector3 getNormal();
};

//out with the old, in with the new!
class tri {
public:
	vector3* vert1;
	vector3* vert2;
	vector3* vert3;
	float depth;
	tri();
	tri(vector3*, vector3*, vector3*);
	void calcDepth();
	vector3 getNormal();
};

float vecSlope(vector2, vector2);
float vecIntercept(vector2, float);

int maxDepthIndex(tri[], int);

void drawTriangle(vector3, vector3, vector3, HDC hdc, COLORREF[][windowSize],COLORREF,mat3x2);

void drawTriangleFrame(vector3, vector3, vector3, HDC hdc, COLORREF[][100], COLORREF);

void setPixels(COLORREF[][100]);