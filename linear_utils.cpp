#include <math.h>
#include <Windows.h>
#include "linear_utils.h"
#include "settings.h"
#include <fstream>

using namespace std;



vector3::vector3() {
	x = 0.f;
	y = 0.f;
	z = 0.f;
}
vector3::vector3(float x1,float x2, float x3) {
	x = x1;
	y = x2; 
	z = x3;
}

float vector3::magnitude() {
	return sqrtf((x * x) + (y * y) + (z * z));
}

vector3 vector3::addVector(vector3 vecAdd) {
	vector3 newVec(0.0f, 0.0f, 0.0f);
	newVec.x = x + vecAdd.x;
	newVec.y = y + vecAdd.y;
	newVec.z = z + vecAdd.z;
	return newVec;
}

vector3 vector3::subVector(vector3 vecSub) {
	//Since friday I have been using a subVec function that adds vectors together and I haven't noticed...
	//But I'm fixing it as of 4:50 p.m. on 11/12/2019
	vector3 newVec(0.0f, 0.0f, 0.0f);
	newVec.x = x - vecSub.x;
	newVec.y = y - vecSub.y;
	newVec.z = z - vecSub.z;
	return newVec;
}
vector3 vector3::scaleVector(float scalar) {
	vector3 newVec(0.0f, 0.0f, 0.0f);
	newVec.x = x * scalar;
	newVec.y = y * scalar;
	newVec.z = z * scalar;
	return newVec;
}
vector3 vector3::vec3CrossProduct(vector3 v2) {
	//(a,b,c) x (d,e,f) = ((bf-ce),(cd-af),(ae-bd))
	return vector3((y * v2.z) - (z * v2.y), (z * v2.x) - (x * v2.z), (x * v2.y) - (y * v2.x));

}

float dotProduct(vector3 a, vector3 b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

vector2::vector2() {
	x = 0.f;
	y = 0.f;
}
vector2::vector2(float x1, float x2) {
	x = x1;
	y = x2;
}

mat3x2::mat3x2(float x1, float x2, float x3,
	float y1, float y2, float y3) {
	firstRow = vector3(x1, x2, x3);
	secondRow = vector3(y1, y2, y3);
}
mat3x2::mat3x2(vector3 vec1, vector3 vec2) {
	firstRow = vec1;
	secondRow = vec2;
}

vector2 mat3x2::vecMult(vector3 inVec) {
	vector2 retVec;
	retVec.x = (inVec.x * firstRow.x) + (inVec.y * firstRow.y) + (inVec.z * firstRow.z);
	retVec.y = (inVec.x * secondRow.x) + (inVec.y * secondRow.y) + (inVec.z * secondRow.z);
	return retVec;
}

mat3x3::mat3x3(	float x1, float x2, float x3,
				float y1, float y2, float y3,
				float z1, float z2, float z3) {
	firstRow = vector3 (x1, x2, x3);
	secondRow = vector3(y1, y2, y3);
	thirdRow = vector3(z1, z2, z3);
}
mat3x3::mat3x3(vector3 row1, vector3 row2, vector3 row3) {
	firstRow = row1;
	secondRow = row2;
	thirdRow = row3;
}

vector3 mat3x3::vecMult(vector3 inVec) {
	vector3 retVec;
	retVec.x = (inVec.x * firstRow.x) + (inVec.y * firstRow.y) + (inVec.z * firstRow.z);
	retVec.y = (inVec.x * secondRow.x) + (inVec.y * secondRow.y) + (inVec.z * secondRow.z);
	retVec.z = (inVec.x * thirdRow.x) + (inVec.y * thirdRow.y) + (inVec.z * thirdRow.z);
	return retVec;
}
void mat3x3::vecTransf(vector3* inVec) {
	vector3 tempVec;
	tempVec.x = (inVec->x * firstRow.x)		+ (inVec->y * firstRow.y)	+ (inVec->z * firstRow.z);
	tempVec.y = (inVec->x * secondRow.x)	+ (inVec->y * secondRow.y)	+ (inVec->z * secondRow.z);
	tempVec.z = (inVec->x * thirdRow.x)		+ (inVec->y * thirdRow.y)	+ (inVec->z * thirdRow.z);
	inVec->x = tempVec.x;
	inVec->y = tempVec.y;
	inVec->z = tempVec.z;
}

float getDepth(vector3 v1, vector3 v2, vector3 v3) {
	//The problem was that I was calculating things s.t. the triangle was anchored at the origin.
	//The answer is to add the base vertex to the midpoint.
	/*std::ofstream depthops;
	depthops.open("depthOps.txt");
	depthops << "\tVertex 1: (" << v1.x << ", " << v1.y << ", " << v1.z << ", )\n";
	depthops << "\tVertex 2 (" << v2.x << ", " << v2.y << ", " << v2.z << ", )\n";
	depthops << "\tVertex 3: (" << v3.x << ", " << v3.y << ", " << v3.z << ", )\n";*/

	vector3 dir1 = v2.subVector(v1);
	vector3 dir2 = v3.subVector(v1);
	/*depthops << "\tDir 1->2: (" << dir1.x << ", " << dir1.y << ", " << dir1.z << ", )\n";
	depthops << "\tDir 1->3: (" << dir2.x << ", " << dir2.y << ", " << dir2.z << ", )\n";*/
	float scaleD1 = 1.0 / 3;
	float scaleD2 = 1.0 / 3;
	vector3 dir1Scaled = dir1.scaleVector(scaleD1);
	vector3 dir2Scaled = dir2.scaleVector(scaleD2);
	/*depthops << "\tDir 1->2 Scaled: (" << dir1Scaled.x << ", " << dir1Scaled.y << ", " << dir1Scaled.z << ", )\n";
	depthops << "\tDir 1->3 Scaled: (" << dir2Scaled.x << ", " << dir2Scaled.y << ", " << dir2Scaled.z << ", )\n";*/
	vector3 MidPtRelative = dir1Scaled.addVector(dir2Scaled);
	//depthops << "\tRelative Midpt: (" << MidPtRelative.x << ", " << MidPtRelative.y << ", " << MidPtRelative.z << ", )\n";
	vector3 midPtFinal = v1.addVector(MidPtRelative);
	//depthops << "\tAbsolute Midpt: (" << midPtFinal.x << ", " << midPtFinal.y << ", " << midPtFinal.z << ", )\n";
	vector3 ortho = vector3(1, -1, -1);
	float dotRet = dotProduct(ortho, midPtFinal);
	/*depthops << "\tFinal Depth: " << dotRet << "\n";
	depthops.close();*/
	return dotRet;
}

vector3 getVertsNormal(vector3 v1, vector3 v2, vector3 v3) {
	vector3 dir1 = v2.subVector(v1);
	vector3 dir2 = v3.subVector(v1);
	vector3 normal = dir1.vec3CrossProduct(dir2);
	vector3 normalNormalized = normal.scaleVector(1.0 / normal.magnitude());
	return normalNormalized;
}

triOld::triOld(vector3 v1, vector3 v2, vector3 v3) {
	vert1 = v1;
	vert2 = v2;
	vert3 = v3;
	depth = 0;
	depth = getDepth(v1, v2, v3);
}
void triOld::calcDepth() {
	depth = getDepth(vert1, vert2, vert3);
}

vector3 triOld::getNormal() {
	return getVertsNormal(vert1, vert2, vert3);
}

tri::tri() {
	vert1 = NULL;
	vert2 = NULL;
	vert3 = NULL;
	depth = 0;
}

tri::tri(vector3* v1, vector3* v2, vector3* v3) {
	vert1 = v1;
	vert2 = v2;
	vert3 = v3;
	depth = 0;
	depth = getDepth(*v1, *v2, *v3);
}

void tri::calcDepth() {
	depth = getDepth(*vert1, *vert2, *vert3);
}

vector3 tri::getNormal() {
	return getVertsNormal(*vert1, *vert2, *vert3);
}

int maxbound(float x1, float x2, float x3) {
	float xmax = x1;
	if (x2 > xmax) { xmax = x2; }
	if (x3 > xmax) { xmax = x3; }
	return (int)xmax;
}

int minbound(float x1, float x2, float x3) {
	float xmin = x1;
	if (x2 < xmin) { xmin = x2; }
	if (x3 < xmin) { xmin = x3; }
	return (int)xmin;
}

float vecSlope(vector2 v1, vector2 v2) {
	if (v1.x == v2.x) { return -1000000; }
	return ((v2.y - v1.y) / (v2.x - v1.x));
}

float vecIntercept(vector2 vec, float slope) {
	return ((-1 * slope * vec.x) + vec.y);
}

int maxDepthIndex(tri tris[], int len) {
	int maxIndex = 0;
	float maxDepth = -1000000; //placeholder
	for (int i = 0; i < len; i++) {
		if (tris[i].depth > maxDepth) {
		//if (tris[i].depth - maxDepth > 0) {
		//if ((int)tris[i].depth > (int)maxDepth) {
			maxDepth = tris[i].depth;
			maxIndex = i;
		}
	}
	return maxIndex;
}


void drawTriangle(vector3 vertex1, vector3 vertex2, vector3 vertex3, HDC hdc,COLORREF pixels[][windowSize],COLORREF color, mat3x2 matProj) {
	vector2 origin(originX, originY);
	int minX = 0;
	int maxX = windowSize;
	int minY = 0;
	int maxY = windowSize;

	/*log << "Incoming vertices:\n";
	log << "\tVert 1: (" << vertex1.x << ", " << vertex1.y << ", " << vertex1.z << ")\n";
	log << "\tVert 2: (" << vertex2.x << ", " << vertex2.y << ", " << vertex2.z << ")\n";
	log << "\tVert 3: (" << vertex3.x << ", " << vertex3.y << ", " << vertex3.z << ")\n";*/

	vector2 vert1 = matProj.vecMult(vertex1);
	vector2 vert2 = matProj.vecMult(vertex2);
	vector2 vert3 = matProj.vecMult(vertex3);

	/*log << "2d vertices: \n";
	log << "\tVert 1: (" << vert1.x << ", " << vert1.y << ")\n";
	log << "\tVert 2: (" << vert2.x << ", " << vert2.y << ")\n";
	log << "\tVert 3: (" << vert3.x << ", " << vert3.y << ")\n";*/

	vert1.x += origin.x;
	vert1.y += origin.y;
	vert2.x += origin.x;
	vert2.y += origin.y;
	vert3.x += origin.x;
	vert3.y += origin.y;

	/*log << "2d vertices after origin: \n";
	log << "\tVert 1: (" << vert1.x << ", " << vert1.y << ")\n";
	log << "\tVert 2: (" << vert2.x << ", " << vert2.y << ")\n";
	log << "\tVert 3: (" << vert3.x << ", " << vert3.y << ")\n";*/


	float slope12 = vecSlope(vert1, vert2);
	float intercept12 = vecIntercept(vert1, slope12);
	//make sure that you know whether the third vertex is greater or less than the line between the others.
	int dir12 = 1;
	if ((slope12 * vert3.x) + intercept12 > vert3.y) {
		dir12 = -1;
	}
	float slope13 = vecSlope(vert1, vert3);
	float intercept13 = vecIntercept(vert1, slope13);
	int dir13 = 1;
	if ((slope13 * vert2.x) + intercept13 > vert2.y) {
		dir13 = -1;
	}
	float slope23 = vecSlope(vert2, vert3);
	float intercept23 = vecIntercept(vert2, slope23);
	int dir23 = 1;
	if ((slope23 * vert1.x) + intercept23 > vert1.y) {
		dir23 = -1;
	}
	float minVertX = min(min(vert1.x, vert2.x), vert3.x);
	float maxVertX = max(max(vert1.x, vert2.x), vert3.x);
	float minVertY = min(min(vert1.y, vert2.y), vert3.y);
	float maxVertY = max(max(vert1.y, vert2.y), vert3.y);

	/* << "Min Vert x: " << minVertX << ", Max Vert x: " << maxVertX << "\n";
	log << "Min Vert y: " << minVertY << ", Max Vert y: " << maxVertY << "\n";
	log.close();*/
	if (dir12 == -1000000 && dir13 == -1000000 && dir23 == -1000000) {
		for (int i = min(vert1.x, vert2.x, vert3.x); i < max(vert1.x, vert2.x, vert3.x); i++) {
			//pixels[(int)vert1.x][i] = color;
		//	pixels[i][(int)vert1.x] = color;
		}
		//return;
	}
	for (int i = max(0,minVertX); i < maxVertX; i++) {
		for (int j = max(0,minVertY); j < maxVertY; j++) {
			if (dir12 * j >= dir12 * ((slope12 * i) + intercept12)
				&& dir13 * j >= dir13 * ((slope13 * i) + intercept13)
				&& dir23 * j >= dir23 * ((slope23 * i) + intercept23)
				) {
				//I flipped (transposed) the array to account for the bitmap having my image be 90 degrees
				//rotated counterclockwise. I also changed the special 'thin triangle' case above in the
				//same way.
				pixels[j][i] = color;
			}
			else {
				//pixels[i][j] = darkColor;
			}
			//else {lines[j][i] = 'X';}
		}
	}
}

void drawTriangleFrame(vector3 vertex1, vector3 vertex2, vector3 vertex3, HDC hdc, COLORREF pixels[][100], COLORREF color) {
	std::ofstream log;
	log.open("logDraw.txt");
	vector2 origin(50.f, 50.0f);
	float pi_sub = 3.14159265358;
	int minX = 0;
	int maxX = 100;
	int minY = 0;
	int maxY = 100;
	mat3x2 matProj(cos(pi_sub / 6), 0, cos(pi_sub / 6),
		-sin(pi_sub / 6), -1, sin(pi_sub / 6));

	log << "Incoming vertices:\n";
	log << "\tVert 1: (" << vertex1.x << ", " << vertex1.y << ", " << vertex1.z << ")\n";
	log << "\tVert 2: (" << vertex2.x << ", " << vertex2.y << ", " << vertex2.z << ")\n";
	log << "\tVert 3: (" << vertex3.x << ", " << vertex3.y << ", " << vertex3.z << ")\n";

	vector2 vert1 = matProj.vecMult(vertex1);
	vector2 vert2 = matProj.vecMult(vertex2);
	vector2 vert3 = matProj.vecMult(vertex3);

	log << "2d vertices: \n";
	log << "\tVert 1: (" << vert1.x << ", " << vert1.y << ")\n";
	log << "\tVert 2: (" << vert2.x << ", " << vert2.y << ")\n";
	log << "\tVert 3: (" << vert3.x << ", " << vert3.y << ")\n";

	vert1.x += origin.x;
	vert1.y += origin.y;
	vert2.x += origin.x;
	vert2.y += origin.y;
	vert3.x += origin.x;
	vert3.y += origin.y;

	log << "2d vertices after origin: \n";
	log << "\tVert 1: (" << vert1.x << ", " << vert1.y << ")\n";
	log << "\tVert 2: (" << vert2.x << ", " << vert2.y << ")\n";
	log << "\tVert 3: (" << vert3.x << ", " << vert3.y << ")\n";


	float slope12 = vecSlope(vert1, vert2);
	float intercept12 = vecIntercept(vert1, slope12);
	//make sure that you know whether the third vertex is greater or less than the line between the others.
	int dir12 = 1;
	if ((slope12 * vert3.x) + intercept12 > vert3.y) {
		dir12 = -1;
	}
	float slope13 = vecSlope(vert1, vert3);
	float intercept13 = vecIntercept(vert1, slope13);
	int dir13 = 1;
	if ((slope13 * vert2.x) + intercept13 > vert2.y) {
		dir13 = -1;
	}
	float slope23 = vecSlope(vert2, vert3);
	float intercept23 = vecIntercept(vert2, slope23);
	int dir23 = 1;
	if ((slope23 * vert1.x) + intercept23 > vert1.y) {
		dir23 = -1;
	}
	float minVertX = min(min(vert1.x, vert2.x), vert3.x);
	float maxVertX = max(max(vert1.x, vert2.x), vert3.x);
	float minVertY = min(min(vert1.y, vert2.y), vert3.y);
	float maxVertY = max(max(vert1.y, vert2.y), vert3.y);

	log << "Min Vert x: " << minVertX << ", Max Vert x: " << maxVertX << "\n";
	log << "Min Vert y: " << minVertY << ", Max Vert y: " << maxVertY << "\n";
	log.close();
	if (dir12 == -1000000 && dir13 == -1000000 && dir23 == -1000000) {
		for (int i = min(vert1.x, vert2.x, vert3.x); i < max(vert1.x, vert2.x, vert3.x); i++) {
			pixels[(int)vert1.x][i] = color;
		}
		return;
	}
	for (int i = minVertX; i < maxVertX; i++) {
		for (int j = minVertY; j < maxVertY; j++) {
			if (abs(dir12 * j - dir12 * ((slope12 * i) + intercept12)) <= 1.f
				|| abs(dir13 * j - dir13 * ((slope13 * i) + intercept13)) <= 1.f
				|| abs(dir23 * j - dir23 * ((slope23 * i) + intercept23)) <= 1.f
				) {
				pixels[i][j] = color;
			}
			else {
				//pixels[i][j] = darkColor;
			}
			//else {lines[j][i] = 'X';}
		}
	}
}

void setPixels(COLORREF pixels[][100]) {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			pixels[i][j] = blackColor;
		}
	}
}