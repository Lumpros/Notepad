#include "Controls.h"
#include "Identifiers.h"
#include "EditMenu.h"
#include "ViewMenu.h"

#include <CommCtrl.h>
#include <Richedit.h>

#define SIZEOF_ARR(arr)	sizeof(arr) / sizeof(arr[0])

WNDPROC   oldEditProcedure;
int	      change_count       = 0;
BOOL      isStatusBarEnabled = TRUE;
HFONT     hCurrentFont       = NULL;
const int dxStatusWidths[]   = { 0, 174, 64, 150, 150, -1 };

int       iStatusWidths[SIZEOF_ARR(dxStatusWidths)];

// The first part is the only one that changes size as the window changes size
// If the window is so small that the first part's with is 0, then the rest of
// the parts start becoming smaller
static void SetStatusWidths(HWND hWnd)
{
	int sum = 0;

	// Get the width in pixels of all parts (after the first)
	// So we can deduce how big the first part should be
	for (unsigned int i = 1; i < SIZEOF_ARR(dxStatusWidths); ++i)
		sum += dxStatusWidths[i];

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	// Calculate first part width
	int part_width = clientRect.right - sum;

	iStatusWidths[0] = part_width > 0 ? part_width : 0;

	int previousWidthsSum = 0;

	// Apply changes to each part based on the previous calculations
	for (unsigned int i = 1; i < SIZEOF_ARR(dxStatusWidths); ++i)
	{
		previousWidthsSum += dxStatusWidths[i - 1];
		iStatusWidths[i] = dxStatusWidths[i] + part_width + previousWidthsSum;
	}
}

void SaveHFont(HFONT hFont)
{
	hCurrentFont = hFont;
}

HFONT RetrieveHFont(void)
{
	return hCurrentFont;
}

BOOL HasChangedOriginalText(void)
{
	return change_count > 0;
}

static void HandleUndoButtonActivation(HWND hWnd, UINT message, WPARAM wParam)
{
	if (message == WM_CHAR)
	{
		++change_count;

		if (change_count == 1)
		{
			HMENU hMenu = GetMenu(GetParent(hWnd));
			EnableMenuItem(hMenu, IDM_EDIT_UNDO, MF_BYCOMMAND | MF_ENABLED);
		}
	}
}

void DecrementChangeCount(HWND hWnd)
{
	if (change_count > 0)
	{
		--change_count;
	}
}

UINT GetStatusBarHeight(HWND hWnd)
{
	RECT sbRect;
	GetWindowRect(GetDlgItem(hWnd, IDC_STATUS_BAR), &sbRect);

	return sbRect.bottom - sbRect.top;
}

static LPCWSTR LoadRichControlDLL(void)
{
	LPCWSTR editclass;

	if (LoadLibrary(L"Msftedit.dll") != NULL)
	{
		editclass = MSFTEDIT_CLASS;
	}

	else
	{
		MessageBox(
			NULL,
			L"Failed to load Msftedit.dll! Zooming in/out will not be available!",
			L"Warning",
			MB_OK | MB_ICONEXCLAMATION
		);

		editclass = L"Edit";
	}

	return editclass;
}

static void HandlePossibleMouseZoom(HWND hWnd, WPARAM wParam)
{
	if ((LOWORD(wParam) & MK_CONTROL) == MK_CONTROL)
	{
		SHORT scroll = GET_WHEEL_DELTA_WPARAM(wParam);
		
		UpdateZoom(GetParent(hWnd), scroll);
	}
}


LRESULT CALLBACK EditControlProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAR:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case EM_SETSEL:
	case EM_EXSETSEL:
		HandleUndoButtonActivation(hWnd, message, wParam);

		if (SelectionHasChanged(message, lParam))
		{
			HandlePossibleTextSelect(hWnd, message, wParam, lParam);
			SetLineColumnStatusBar(hWnd);
		}
		break;

	case WM_MOUSEWHEEL:
		CallWindowProc(oldEditProcedure, hWnd, message, wParam, lParam);
		HandlePossibleMouseZoom(hWnd, wParam);
		return 0;
	}

	return CallWindowProc(oldEditProcedure, hWnd, message, wParam, lParam);
}

void SetEditControlWindProc(HWND hEditControl)
{
	oldEditProcedure = (WNDPROC)SetWindowLongPtr(hEditControl, GWLP_WNDPROC, (LONG_PTR)EditControlProcedure);
}

static HWND CreateMainEditControl(HWND hWnd, HINSTANCE hInstance)
{
	LPCWSTR editclass = LoadRichControlDLL();

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	HWND editControlHandle = CreateWindowEx(
		ES_EX_ZOOMABLE,
		editclass,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT |
		ES_MULTILINE | ES_AUTOVSCROLL | WS_HSCROLL |
		ES_AUTOHSCROLL | ES_DISABLENOSCROLL,
		0, 0, 
		clientRect.right, 
		clientRect.bottom - GetStatusBarHeight(hWnd),
		hWnd,
		(HMENU)IDC_TEXT_EDIT,
		hInstance,
		NULL
	);

	SendMessage(editControlHandle, EM_SETLIMITTEXT, 0, 0);

	SetEditControlWindProc(editControlHandle);

	if (lstrcmpW(editclass, MSFTEDIT_CLASS) == 0)
		SendMessage(editControlHandle, EM_SETZOOM, 10, 9);

	return editControlHandle;
}

static HWND CreateStatusBar(HWND hWnd, HINSTANCE hInstance)
{
	HWND statusBarHandle = CreateWindow(
		STATUSCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		0, 0, 0, 0,
		hWnd,
		(HMENU)IDC_STATUS_BAR,
		hInstance, NULL
	);

	SetStatusWidths(hWnd);

	SendMessage(statusBarHandle, SB_SETPARTS, SIZEOF_ARR(iStatusWidths), (LPARAM)iStatusWidths);
	SendMessage(statusBarHandle, SB_SETTEXT, 1, (LPARAM)L" Ln 1, Col 1");
	SendMessage(statusBarHandle, SB_SETTEXT, 2, (LPARAM)L" 100%");
	SendMessage(statusBarHandle, SB_SETTEXT, 3, (LPARAM)L" Windows (CRLF)");
	SendMessage(statusBarHandle, SB_SETTEXT, 4, (LPARAM)L" UTF-8");

	return statusBarHandle;
}

TEXTMETRIC GetSystemFontMetrics(HWND hWnd)
{
	HDC hDC = GetDC(hWnd);

	SelectObject(hDC, GetStockObject(SYSTEM_FONT));

	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);

	ReleaseDC(hWnd, hDC);

	return tm;
}

void CreateControls(HWND hWnd)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	TEXTMETRIC tm = GetSystemFontMetrics(hWnd);
	HFONT hFont = CreateFont(tm.tmHeight + tm.tmExternalLeading, tm.tmAveCharWidth, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, DEFAULT_FONT_STR);
	SaveHFont(hFont);
	
	HWND status_bar_handle = CreateStatusBar(hWnd, hInstance);

	// Must be created after status bar (see GetStatusBarHeight)
	HWND text_edit_handle = CreateMainEditControl(hWnd, hInstance);
	SendMessage(text_edit_handle, WM_SETFONT, (WPARAM)hFont, NULL);
}

static POINT GetClientDimensions(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	POINT point;
	point.x = clientRect.right;
	point.y = clientRect.bottom;

	return point;
}

void ResizeMainEditControl(HWND hWnd, int width, int height)
{
	if (isStatusBarEnabled)
		height -= GetStatusBarHeight(hWnd);

	SetWindowPos(GetDlgItem(hWnd, IDC_TEXT_EDIT), HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
}

void EnableStatusBar(HWND hWnd)
{
	if (!isStatusBarEnabled)
	{
		isStatusBarEnabled = TRUE;
		ShowWindow(GetDlgItem(hWnd, IDC_STATUS_BAR), SW_SHOW);
		
		POINT clientDimensions = GetClientDimensions(hWnd);
		ResizeMainEditControl(hWnd, clientDimensions.x, clientDimensions.y);
	}
}

void DisableStatusBar(HWND hWnd)
{
	if (isStatusBarEnabled)
	{
		isStatusBarEnabled = FALSE;
		ShowWindow(GetDlgItem(hWnd, IDC_STATUS_BAR), SW_HIDE);

		POINT clientDimensions = GetClientDimensions(hWnd);
		ResizeMainEditControl(hWnd, clientDimensions.x, clientDimensions.y);
	}
}

void HandleStatusBarResize(HWND hWnd)
{
	HWND sbhandle = GetDlgItem(hWnd, IDC_STATUS_BAR);

	SetStatusWidths(hWnd);
	SendMessage(sbhandle, SB_SETPARTS, SIZEOF_ARR(iStatusWidths), (LPARAM)iStatusWidths);
	SendMessage(sbhandle, WM_SIZE, 0, 0);
}

void HandleLineColumnTextOnResize(HWND hWnd)
{
	HMENU hMenu   = GetMenu(hWnd);
	DWORD dwState = CheckMenuItem(hMenu, IDM_FORMAT_WORDWRAP, MF_BYCOMMAND);

	if (dwState == MF_CHECKED)
	{
		HWND hEditControl = GetDlgItem(hWnd, IDC_TEXT_EDIT);
		SetLineColumnStatusBar(hEditControl);
	}
}

void HandleWindowResize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ResizeMainEditControl(hWnd, LOWORD(lParam), HIWORD(lParam));
	HandleStatusBarResize(hWnd);
	HandleLineColumnTextOnResize(hWnd);
}