#include "Controls.h"

#include <CommCtrl.h>

void CreateControls(HWND hWnd)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	HFONT hFont = CreateFontW(21, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, DEFAULT_FONT_STR);

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	
	HWND text_edit_handle = CreateWindowEx(
		ES_EX_ZOOMABLE,
		L"Edit",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT |
		ES_MULTILINE | ES_AUTOVSCROLL | WS_HSCROLL,
		0, 0, clientRect.right, clientRect.bottom,
		hWnd,
		(HMENU)IDC_TEXT_EDIT,
		hInstance,
		NULL
	);

	SendMessage(text_edit_handle, WM_SETFONT, (WPARAM)hFont, NULL);

}