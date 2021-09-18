#include<windows.h>

#include<tchar.h>

#include<math.h>

#include<iostream>

#ifdef _DEBUG

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

#endif

static double spd = 0;

static bool gravend = FALSE;

static int x = 400, y = 40;

int cnt = 4;

void gravity(int* y) {

	static RECT rectview;

	*y = *y + spd * 9.8;

	spd = spd + 0.05;

}

void jump(int* y) {

	static RECT rectview;

	*y = *y - (spd * 7);

	spd = spd - 0.05;

	if (spd < 0) gravend = FALSE;

}

void CALLBACK KeyTimer(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)

{

	static RECT rectview;

	if (GetKeyState(VK_LEFT) & 0x8000) {

		x -= 5;

	}

	if (x == rectview.left + 20) { for (int i = 0; i < 10; i++) { x = x + 5; Sleep(1); } }

	if (GetKeyState(VK_RIGHT) & 0x8000) {

		x += 5;

	}

	if (x == 585 - 20) { x = x - 5; if (x == 585 - 25) { x = x - 5; if (x == 585 - 30) { x = x - 5; } } }

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	HDC hdc;

	PAINTSTRUCT ps;

	static RECT rectview;

	static bool flag;

	static int cnt = 10;

	SetTimer(hwnd, 1, 1, NULL);

	SetTimer(hwnd, 2, 1, KeyTimer);

	switch (iMsg) {

	case WM_CREATE:

		GetClientRect(hwnd, &rectview);

		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		if (flag)

			SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));

		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);

		EndPaint(hwnd, &ps);

		break;

	case WM_KEYUP:

		flag = false;



	case WM_TIMER:

		if (y > rectview.bottom) { y = rectview.bottom - 20; gravend = TRUE; }

		if (gravend == FALSE)gravity(&y);

		if (gravend == TRUE)jump(&y);

		std::cout << rectview.right << " " << x << " " << gravend << "\n";

		InvalidateRgn(hwnd, NULL, TRUE);

		break;

	case WM_DESTROY:

		KillTimer(hwnd, 1);

		KillTimer(hwnd, 2);

		PostQuitMessage(0);

		break;

	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, PSTR szCmdLine, int nCmdShow) {

	HWND hwnd;

	MSG msg;

	WNDCLASS win;

	win.style = CS_HREDRAW | CS_VREDRAW;

	win.lpfnWndProc = WndProc;

	win.cbClsExtra = 0;

	win.cbWndExtra = 0;

	win.hInstance = hInstance;

	win.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	win.hCursor = LoadCursor(NULL, IDC_CROSS);

	win.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	win.lpszMenuName = NULL;

	win.lpszClassName = _T("winclass");

	RegisterClass(&win);

	hwnd = CreateWindow(_T("winclass"), _T("TITLE"), WS_OVERLAPPEDWINDOW, 200, 300, 601, 400, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);

		DispatchMessage(&msg);

	}

	return (int)msg.wParam;

}

int main() {



}