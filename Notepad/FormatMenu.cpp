#include "FormatMenu.h"
#include "Controls.h"
#include "Identifiers.h"
#include "Controls.h"
#include "ViewMenu.h"
#include "Resource.h"

#include <CommCtrl.h>
#include <Richedit.h>

CHOOSEFONT GetChooseFontFromDialog(HWND hWnd, BOOL* success)
{
	CHOOSEFONT cf;
	LOGFONT lf;

	static LOGFONT oldlf;
	static bool initialized = false;

	ZeroMemory(&cf, sizeof(cf));

	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hWnd;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.Flags = CF_INITTOLOGFONTSTRUCT;
	cf.rgbColors = RGB(0, 0, 0);

	/* Do all this dumb stuff to save previous choices */
	if (initialized)
	{
		cf.lpLogFont = &oldlf;
		ChooseFont(&cf);
	}

	else
	{
		initialized = true;

		TEXTMETRIC tm = GetSystemFontMetrics(hWnd);

		cf.lpLogFont = &lf;
		cf.lpLogFont->lfHeight = tm.tmHeight + tm.tmExternalLeading;
		lstrcpyW(cf.lpLogFont->lfFaceName, DEFAULT_FONT_STR);

		*success = ChooseFont(&cf);

		oldlf = *cf.lpLogFont;
	}

	return cf;
}

static void SetTextEditFont(HWND hWnd)
{
	BOOL bSucces;
	CHOOSEFONT cf = GetChooseFontFromDialog(hWnd, &bSucces);

	if (bSucces)
	{
		HFONT hFont = CreateFontIndirect(cf.lpLogFont);
		SaveHFont(hFont);
		SendMessage(GetDlgItem(hWnd, IDC_TEXT_EDIT), WM_SETFONT, (WPARAM)hFont, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

static LPWSTR GetControlEditTextCopy(HWND hEditControl)
{
	INT iTextLength = GetWindowTextLength(hEditControl) + 1;
	LPWSTR lpszCopy = (LPWSTR)malloc(iTextLength * sizeof(WCHAR));

	GetWindowText(hEditControl, lpszCopy, iTextLength);
	return lpszCopy;
}

static UINT DecideCheckBasedOnState(DWORD checkState)
{
	return checkState == MF_CHECKED ? MF_UNCHECKED : MF_CHECKED;
}

static DWORD GetExtraCreationFlags(DWORD checkState)
{
	DWORD dwFlags = NULL;

	if (checkState == MF_CHECKED)
	{
		dwFlags = WS_HSCROLL | ES_AUTOHSCROLL;
	}

	return dwFlags;
}

static HWND CreateNewControlEdit(HWND hWnd, DWORD checkState)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	RECT  clientRect;
	GetClientRect(hWnd, &clientRect);

	DWORD dwExtraFlags = GetExtraCreationFlags(checkState);

	HWND hEdit = CreateWindowEx(
		ES_EX_ZOOMABLE,
		MSFTEDIT_CLASS,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT |
		ES_MULTILINE | ES_AUTOVSCROLL |
		ES_DISABLENOSCROLL | dwExtraFlags,
		0, 0,
		clientRect.right,
		clientRect.bottom - GetStatusBarHeight(hWnd),
		hWnd,
		(HMENU)IDC_TEXT_EDIT,
		hInstance,
		NULL
	);

	SendMessage(hEdit, EM_SETLIMITTEXT, 0, 0);

	return hEdit;
}

static void HandleWordWrap(HWND hWnd)
{
	HMENU  hMenu      = GetMenu(hWnd);
	HWND   hEdit      = GetDlgItem(hWnd, IDC_TEXT_EDIT);
	LPWSTR lpszCopy   = GetControlEditTextCopy(hEdit);
	DWORD  checkState = CheckMenuItem(hMenu, IDM_FORMAT_WORDWRAP, MF_BYCOMMAND);
	HFONT  hFont      = RetrieveHFont();
	RECT   clientRect;
	GetClientRect(hWnd, &clientRect);

	DestroyWindow(hEdit);

	hEdit = CreateNewControlEdit(hWnd, checkState);
	SetEditControlWindProc(hEdit);
	SetWindowText(hEdit, lpszCopy);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, NULL);
	free(lpszCopy);
	RefreshZoom(hWnd);
	ResizeMainEditControl(hWnd, clientRect.right, clientRect.bottom);
	CheckMenuItem(hMenu, IDM_FORMAT_WORDWRAP, DecideCheckBasedOnState(checkState));
}

void HandleFormatMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FORMAT_FONT:
		SetTextEditFont(hWnd);
		break;

	case IDM_FORMAT_WORDWRAP:
		HandleWordWrap(hWnd);
		break;
	}
}