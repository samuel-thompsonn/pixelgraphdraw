#include <stdio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "linear_utils.h"
#include <ctime>

#ifndef UNICODE
#define UNICODE
#endif 

//#include "linear_utils.cpp"


const float PI_SUB = 3.14159265358;
const float rotateRate = PI_SUB * 2 / 30;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const char CLASS_NAME[] = "Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		"Learn to Program Windows pleasekillme",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

int idTimer = -1;

vector3 lightDir(-1.0f, 1.0f, -1.f);
vector3 light = lightDir.scaleVector(1.0 / lightDir.magnitude());

vector3 vertices_default[8] = {
	vector3(12,0,12), //0
	vector3(-12,0,12), //1
	vector3(-12,0,-12), //2
	vector3(12,0,-12), //3
	vector3(12,24,12), //4
	vector3(-12,24,12), //5
	vector3(-12,24,-12), //6
	vector3(12,24,-12) //7
};
//tri tris_default[12] = {
//	//back left
//	tri(&vertices[2],&vertices[6],&vertices[7]),
//	tri(&vertices[2],&vertices[7],&vertices[3]),
//
//	//back right
//	tri(&vertices[7],&vertices[4],&vertices[3]),
//	tri(&vertices[0],&vertices[3],&vertices[4]),
//
//	//right
//	tri(&vertices[4],&vertices[5],&vertices[0]),
//	tri(&vertices[1],&vertices[0],&vertices[5]),
//
//	//top
//	tri(&vertices[6],&vertices[5],&vertices[7]),
//	tri(&vertices[4],&vertices[7],&vertices[5]),
//
//	//left
//	tri(&vertices[5],&vertices[6],&vertices[1]),
//	tri(&vertices[2],&vertices[1],&vertices[6]),
//
//	//bottom
//	tri(&vertices[0],&vertices[1],&vertices[3]),
//	tri(&vertices[2],&vertices[3],&vertices[1])
//};

char names[12][50] = {
	"back left", "back left",
	"back right","back right",
	"right", "right",
	"top", "top",
	"left", "left",
	"bottom", "bottom"
};

mat3x3 matRot = mat3x3(cos(rotateRate), 0, -sin(rotateRate),
				0, 1, 0,
				sin(rotateRate), 0, cos(rotateRate));

HDC hdc;
int drawCount = 0;
COLORREF myPixels[100][100];
int graphSize = 6;
float displaySize = 16.f;

int gridLength = (2 * graphSize) + 1;
vector3** vertices = (vector3 * *)malloc((gridLength) * sizeof(tri*));
//vector3 vertices[3][3];
int numTris = ((2 * graphSize) * (2 * graphSize)) * 2;
tri* tris = (tri*)malloc(numTris * sizeof(tri));
int count = 0;
int initialized = 0;

//magmi bitmapu
HBITMAP myBitmap = NULL;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, idTimer = 1, 300, NULL);
		hdc = GetDC(hwnd);
		return 0L;


	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;


	case WM_TIMER:

		//if (drawCount >= 12 ) {
			//for (int i = 0; i < sizeof(vertices)/sizeof(vertices[0]); i++) {
				//matRot.vecTransf(&vertices[i]);
				//matRot.vecTransf(&vertices[i]);
				//matRot.vecTransf(&vertices[i]);
			//}
		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridLength; j++) {
				matRot.vecTransf(&vertices[i][j]);
			}
		}
			//drawCount = 0;
		//}

		//RedrawWindow(hwnd, NULL, NULL, RDW_INTERNALPAINT);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		return 0L; 
	case WM_PAINT:
	{

		//new magic sorcery for bitmaps
		HDC newHdc = CreateCompatibleDC(hdc);
		myBitmap = CreateCompatibleBitmap(newHdc, 1000, 1000);
		SelectObject(newHdc, myBitmap);

		if (initialized == 0) {
			for (int i = 0; i <= 2 * graphSize; i += 1) {
				vertices[i] = (vector3*)malloc(gridLength * sizeof(vector3));
				for (int j = 0; j <= 2 * graphSize; j += 1) {
					float this_x = ((float)i * (displaySize / graphSize)) - displaySize;
					float this_z = ((float)j * (displaySize / graphSize)) - displaySize;
					float this_y = 0.05f * ((this_z * this_z) - (this_x * this_x));
					//float this_y = 5.0f * (cos(0.5f * this_x) + sin(0.5f * this_z));
					vertices[i][j] = vector3(this_x,this_y,this_z);
					//vertices[i][j].y = 0.10f * ((i * i) - (j * j));
				}
			}
			for (int i = 0; i < 2 * graphSize; i++) {
				for (int j = 0; j < 2 * graphSize; j++) {
					tris[count] = tri((&vertices[i][j]), (&vertices[i][j + 1]), (&vertices[i + 1][j]));
					tris[count + 1] = tri((&vertices[i][j + 1]), (&vertices[i + 1][j + 1]), (&vertices[i + 1][j]));
					count += 2;
				}
			}
			initialized = 1;
		}

		drawCount += 1;
		if (drawCount > 0) {
			PAINTSTRUCT ps;
			hdc = BeginPaint(hwnd, &ps);

			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 100; j++) {
					myPixels[i][j] = blackColor;
				}
			}
			for (int i = 0; i < numTris; i++) {
				tris[i].calcDepth();
			}
			int triDraw;
			COLORREF myColor;
			for (int i = 0; i < numTris; i++) {
				triDraw = maxDepthIndex(tris, numTris);
				tris[triDraw].depth = -100000;

				float lightAmt = 127.f + (dotProduct(light, tris[triDraw].getNormal()) * 127.f);
				myColor = RGB(lightAmt, lightAmt*0.8f, lightAmt*0.0f);

				drawTriangle(*(tris[triDraw].vert1), *(tris[triDraw].vert2), *(tris[triDraw].vert3), hdc, myPixels, myColor);				
			}

			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 100; j++) {
					SetPixel(newHdc, (2 * i), (2 * j), myPixels[i][j]);
					SetPixel(newHdc, (2 * i)+1, (2 * j), myPixels[i][j]);
					SetPixel(newHdc, (2 * i), (2 * j)+1, myPixels[i][j]);
					SetPixel(newHdc, (2 * i)+1, (2 * j)+1, myPixels[i][j]);
				}
			}

			DeleteDC(newHdc);
			
			EndPaint(hwnd, &ps);
		}
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}