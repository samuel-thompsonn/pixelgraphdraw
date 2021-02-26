#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "linear_utils.h"

#ifndef UNICODE
#define UNICODE
#endif 

//#include "linear_utils.cpp"

static COLORREF redColor = RGB(255, 0, 0);
static COLORREF blackColor = RGB(0, 0, 0);

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
		"Learn to Program Windows",    // Window text
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);


		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 21));
		vector3 iVec(40.0f, 0.f, 0.f);
		for (float i = 0; i < 100; i ++) {
			iVec.x = i;
			for (float j = 0; j < 100; j ++ ) {
		//		SetPixel(hdc, iVec.x, 40 + j, redColor);
			}
		}

		vector2 origin(100.0f, 100.0f);
		float pi_sub = 3.14159265358;
		mat3x2 matProj(cos(pi_sub / 6), 0, cos(pi_sub / 6),
			-sin(pi_sub / 6), -1, sin(pi_sub / 6));

		vector2 vert1(0.f, 0.f);
		vector2 vert2(100.f, 0.f);
		vector2 vert3(100.f, 100.f);

		float slope12 = vecSlope(vert1, vert2);
		float intercept12 = vecIntercept(vert1, vert2);
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
		for (int i = minX; i <= maxX; i++) {
			for (int j = minY; j <= maxY; j++) {
				if (dir12 * j >= dir12 * floor((slope12 * i) + intercept12)
					&& dir13 * j >= dir13 * floor((slope13 * i) + intercept13)
					&& dir23 * j >= dir23 * floor((slope23 * i) + intercept23)
					) {
					//lines[j][i] = tile;
					SetPixel(hdc, j,i, redColor);
				}
				//else {lines[j][i] = 'X';}
			}
		}

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}