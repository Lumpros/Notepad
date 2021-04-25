#include "AboutMenu.h"
#include "Identifiers.h"
#include "Resource.h"

void PaintWindowsLogo(HDC hDC, HBITMAP winBitmap)
{
	BITMAP bitmap;
	HDC hdcMem;
	HGDIOBJ oldBitmap;
	BLENDFUNCTION bf;

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 0xFF;
	bf.AlphaFormat = AC_SRC_ALPHA;

	hdcMem = CreateCompatibleDC(hDC);
	oldBitmap = SelectObject(hdcMem, winBitmap);

	GetObject(winBitmap, sizeof(bitmap), &bitmap);

	// if the map mode isnt MM_TEXT im screwed lol
	TransparentBlt(
		hDC,
		90,
		20,
		bitmap.bmWidth,
		bitmap.bmHeight,
		hdcMem,
		0,
		0,
		bitmap.bmWidth,
		bitmap.bmHeight,
		RGB(0xFF, 0xFF, 0xFF)
	);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}

INT_PTR CALLBACK AboutMenuProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Because visual studio hates me
	static HBITMAP windows_bitmap = (HBITMAP)INVALID_HANDLE_VALUE;

	PAINTSTRUCT ps;
	HDC hDC;

	switch (message)
	{
	case WM_INITDIALOG:
		if (windows_bitmap == (HBITMAP)INVALID_HANDLE_VALUE)
		{
			windows_bitmap = (HBITMAP)LoadImage(
				GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDB_BITMAP1), 
				IMAGE_BITMAP,
				0, 0,
				LR_CREATEDIBSECTION
			);
		}
		break;
		
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		PaintWindowsLogo(hDC, windows_bitmap);
		EndPaint(hWnd, &ps);
		break;

	case WM_CLOSE:
		EndDialog(hWnd, EXIT_SUCCESS);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			EndDialog(hWnd, IDOK);
			break;
		}
		break;
	}

	return 0;
}

void HandleAboutMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_HELP_ABOUT:
		DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUT_DIALOG), hWnd, AboutMenuProcedure);
		break;
	}
}