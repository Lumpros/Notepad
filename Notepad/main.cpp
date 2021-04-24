#include <Windows.h>

#include "Controls.h"
#include "Menu.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' " \
	"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	LPCWSTR class_name = L"MainWindow";
	WNDCLASS wc = { 0 };
	
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WindowProcedure;
	wc.lpszClassName	= class_name;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"Failed to register window class", L"Error", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	HWND hWnd = CreateWindow(
		class_name,
		L"Notepad",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1000, 800,
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
		CreateControls(hWnd);
		InitializeMenu(hWnd);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;

	case WM_COMMAND:
		HandleMenuCommand(hWnd, wParam, lParam);
		return 0;

	case WM_SIZE:
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		SetWindowPos(GetDlgItem(hWnd, IDC_TEXT_EDIT), HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}