#include <Windows.h>

#include "Controls.h"
#include "Menu.h"
#include "Resource.h"
#include "Identifiers.h"

#include <CommCtrl.h>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' " \
	"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	InitCommonControls();
	
	LPCWSTR class_name = L"MainWindow";
	WNDCLASS wc = { 0 };
	
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WindowProcedure;
	wc.lpszClassName	= class_name;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.lpszMenuName     = MAKEINTRESOURCE(IDR_MENU1);

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"Failed to register window class", L"Error", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	HWND hWnd = CreateWindow(
		class_name,
		L"Untitled - Notepad",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480,
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		CreateControls(hWnd);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;

	case WM_COMMAND:
		HandleMenuCommand(hWnd, wParam, lParam);
		return 0;

	case WM_SIZE:
		HandleWindowResize(hWnd, wParam, lParam);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}