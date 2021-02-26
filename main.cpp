#include <stdio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "linear_utils.h"
#include "settings.h"
#include <ctime>

#ifndef UNICODE
#define UNICODE
#endif 

#define APPLICATION_NAME "Graph Visualizer"

//#include "linear_utils.cpp"


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
		APPLICATION_NAME,    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, windowSize, windowSize,

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

mat3x2 matProj(cos(PI_SUB / 6), 0, cos(PI_SUB / 6),
	-sin(PI_SUB / 6), -1, sin(PI_SUB / 6));

mat3x3 matScale = mat3x3(	graphScale, 0,			0,
							0,			graphScale, 0,
							0,			0,			graphScale);

HDC hdc;
int drawCount = 0;
COLORREF myPixels[windowSize][windowSize];
//COLORREF** myPixels = (COLORREF**)malloc(windowSize*sizeof(COLORREF*));
//int graphSize = 16;
//float displaySize = 50.f;

int gridLength = (2 * graphSize) + 1;
vector3** vertices = (vector3 * *)malloc((gridLength) * sizeof(tri*));
//vector3 vertices[3][3];
int numTris = ((2 * graphSize) * (2 * graphSize)) * 2;
tri* tris = (tri*)malloc(numTris * sizeof(tri));
int count = 0;
int initialized = 0;

//magmi bitmapu
HBITMAP myBitmap = NULL;
BITMAPINFO bmi;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (uMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, idTimer = 1, timerSpeed, NULL);
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
		//matRot.vecTransf(&light);
			//drawCount = 0;
		//}

		//RedrawWindow(hwnd, NULL, NULL, RDW_INTERNALPAINT);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		return 0L; 
	case WM_PAINT:
	{
		if (initialized == 0) {
			for (int i = 0; i <= 2 * graphSize; i += 1) {
				vertices[i] = (vector3*)malloc(gridLength * sizeof(vector3));
				for (int j = 0; j <= 2 * graphSize; j += 1) {
					float this_x = ((float)i * (displaySize / graphSize)) - displaySize;
					float this_z = ((float)j * (displaySize / graphSize)) - displaySize;
					float this_y;
					this_y = 0.010f * ((this_z * this_z) - (this_x * this_x));
					//this_y = 7.0f * (cos(0.15f * this_x) + sin(0.15f * this_z));
					//this_y = 7.0f * (cos(0.15f * this_x) * sin(0.15f * this_z));
					//this_y += 5 + 0.005 * -1.0f * ((this_x * this_x) - (this_z * this_z));
					//float this_y = (0.3f * this_x) + (5.0f * sin(this_x*0.1)) + (sin(this_x * 0.5f));
					// (this_x > -51 && this_z > -51) {
					//	this_y = 44.f * pow(2.7182818284f, -0.1f * (this_x + 51.f)) + 44.f * pow(2.7182818284f,-.1f *  (this_z + 51.f));
					//}
					//else {
					//	this_y = 0;
					//}
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
			//To make sure that myPixels is a 2d thing
			for (int i = 0; i < windowSize; i++) {
			//	myPixels[i] = (COLORREF*)malloc(windowSize * sizeof(COLORREF));
			}
			initialized = 1;
		}

		drawCount += 1;
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < windowSize; i++) {
			for (int j = 0; j < windowSize; j++) {
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
			//myColor = RGB(0.f*lightAmt, lightAmt*0.8f, lightAmt*1.f); //The bitmap thinks it's BGR format
			myColor = RGB(1.f * lightAmt, lightAmt * 1.f, lightAmt * 1.f); //The bitmap thinks it's BGR format

			drawTriangle(*(tris[triDraw].vert1), *(tris[triDraw].vert2), *(tris[triDraw].vert3), hdc, myPixels, myColor, matProj);				
		}

		//new stuff

		BITMAPINFO bmi;

		ZeroMemory(&bmi, sizeof(BITMAPINFO));

		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biHeight = -windowSize;
		bmi.bmiHeader.biWidth = windowSize;
		bmi.bmiHeader.biPlanes = 1;
		HDC memDC = CreateCompatibleDC(NULL);
		HBITMAP map = CreateCompatibleBitmap(hdc, windowSize, windowSize);
		SetDIBits(memDC,map,0,windowSize,(void*)myPixels,&bmi,DIB_RGB_COLORS);
		SelectObject(memDC, map);
		//for (int i = 0; i < 100; i++) {
		//	for (int j = 0; j < 100; j++) {
		//		SetPixel(memDC, i, j, myPixels[i][j]);
		//	}
		//}
		BitBlt(hdc, 0, 0, windowSize, windowSize, memDC, 0, 0, SRCCOPY);
		DeleteObject(map);
		DeleteDC(memDC);
		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}