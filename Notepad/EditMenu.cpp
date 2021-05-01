#include "EditMenu.h"
#include "Identifiers.h"
#include "Controls.h"

#include <time.h>
#include <Richedit.h>
#include <CommCtrl.h>

static void iSwap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void AppendText(LPCWSTR newText, HWND mainhWnd)
{
	HWND hwndOutput = GetDlgItem(mainhWnd, IDC_TEXT_EDIT);
	DWORD startPos, endPos;

	SendMessage(hwndOutput, EM_GETSEL, reinterpret_cast<WPARAM>(&startPos), reinterpret_cast<LPARAM>(&endPos));

	int outLength = GetWindowTextLength(hwndOutput);

	SendMessage(hwndOutput, EM_SETSEL, outLength, outLength);
	SendMessage(hwndOutput, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(newText));
	SendMessage(hwndOutput, EM_SETSEL, startPos, endPos);
}

void InsertText(LPCWSTR newText, HWND mainhWnd)
{
	HWND hwndOutput = GetDlgItem(mainhWnd, IDC_TEXT_EDIT);

	int outLength = GetWindowTextLength(hwndOutput);

	SendMessage(hwndOutput, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(newText));
}

static void InsertSystemTime(HWND hWnd)
{
	time_t t = time(NULL);
	struct tm localtm = *localtime(&t);

	WCHAR date[32];
	wsprintf(date, L"%02d:%02d:%02d %d-%02d-%02d", localtm.tm_hour,
		localtm.tm_min, localtm.tm_sec,
		localtm.tm_year + 1900, localtm.tm_mon + 1,
		localtm.tm_mday);
	InsertText(date, hWnd);
}

static void UndoChanges(HWND hWnd)
{
	DecrementChangeCount(hWnd);
	SendMessage(GetDlgItem(hWnd, IDC_TEXT_EDIT), EM_UNDO, NULL, NULL);
}

static void PasteText(HWND hWnd)
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
		return;

	if (!OpenClipboard(hWnd))
		return;

	HGLOBAL hClipboard = GetClipboardData(CF_UNICODETEXT);

	if (hClipboard != NULL)
	{
		LPWSTR data = (LPWSTR)GlobalLock(hClipboard);

		if (data != NULL)
		{
			InsertText(data, hWnd);
			GlobalUnlock(data);
		}
	}

	CloseClipboard();
}

static void DeleteSelectedText(HWND hWnd)
{
	SendMessage(GetDlgItem(hWnd, IDC_TEXT_EDIT), EM_REPLACESEL, TRUE, (LPARAM)L"");
}

static void	CopySelectedText(HWND hWndMain)
{
	INPUT inputs[4];
	for (size_t i = 0; i < 4; ++i)
	{
		ZeroMemory(&inputs[i].ki, sizeof(KEYBDINPUT));
		inputs[i].type = INPUT_KEYBOARD;
	}

	inputs[0].ki.wVk = VK_CONTROL;
	inputs[1].ki.wVk = 0x43; // 'C' Virtual Key Code
	inputs[2].ki.wVk = VK_CONTROL;
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[3].ki.wVk = 0x43;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(sizeof(inputs) / sizeof(inputs[0]), inputs, sizeof(INPUT));
}

static void CutSelectedText(HWND hWnd)
{
	CopySelectedText(hWnd);
	DeleteSelectedText(hWnd);
}

UINT_PTR CALLBACK FRWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

// Don't forget to call free()
static FINDREPLACE GetFindStruct(HWND hWnd)
{
	const BYTE size = 64;
	LPWSTR lpstrFindWhat    = (LPWSTR)calloc(size, sizeof(WCHAR));

	FINDREPLACE fr;
	ZeroMemory(&fr, sizeof(fr));
	fr.lStructSize		= sizeof(fr);
	fr.hwndOwner		= hWnd;
	fr.lpstrFindWhat	= lpstrFindWhat;
	fr.wFindWhatLen		= size * sizeof(WCHAR);
	fr.lpfnHook			= FRWindowProcedure;
	fr.Flags			= FR_ENABLEHOOK;

	return fr;
}

static void HandleFind(HWND hWnd)
{
	FINDREPLACE fr = GetFindStruct(hWnd);

	FindText(&fr);

	free(fr.lpstrFindWhat);
}

void HandleEditMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_EDIT_TIME_DATE:
		InsertSystemTime(hWnd);
		break;

	case IDM_EDIT_UNDO:
		UndoChanges(hWnd);
		break;

	case IDM_EDIT_PASTE:
		PasteText(hWnd);
		break;

	case IDM_EDIT_COPY:
		CopySelectedText(hWnd);
		break;

	case IDM_EDIT_CUT:
		CutSelectedText(hWnd);
		break;

	case IDM_EDIT_DELETE:
		DeleteSelectedText(hWnd);
		break;

	case IDM_EDIT_FIND:
		HandleFind(hWnd);
		break;
	}
}

void EnableTextEditMenuItems(HWND hWnd, BOOL enabled)
{
	HMENU hMenu = GetMenu(hWnd);

	if (hMenu == NULL)
		return;

	UINT uEnable = enabled ? MF_ENABLED : (MF_GRAYED | MF_DISABLED);
	uEnable |= MF_BYCOMMAND;

	EnableMenuItem(hMenu, IDM_EDIT_CUT, uEnable);
	EnableMenuItem(hMenu, IDM_EDIT_COPY, uEnable);
	EnableMenuItem(hMenu, IDM_EDIT_DELETE, uEnable);
	EnableMenuItem(hMenu, IDM_EDIT_SEARCH, uEnable);
}

void HandlePossibleTextSelect(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DWORD cbSelStart, cbSelEnd;
	HWND parent = GetParent(hWnd);

	SendMessage(hWnd, EM_GETSEL, (WPARAM)&cbSelStart, (LPARAM)&cbSelEnd);

	static BOOL areControlsEnabled = FALSE;

	// Has selected some text
	if (cbSelStart != cbSelEnd && !areControlsEnabled)
	{
		areControlsEnabled = TRUE;
		EnableTextEditMenuItems(parent, TRUE);
	}

	else if (cbSelStart == cbSelEnd && areControlsEnabled)
	{
		areControlsEnabled = FALSE;
		EnableTextEditMenuItems(parent, FALSE);
	}
}

void SetLineColumnStatusBar(HWND hWnd)
{
	// because it is called from the wndproc of the edit control
	HWND hControl = hWnd;
	hWnd = GetParent(hWnd);

	CHARRANGE cr;
	SendMessage(hControl, EM_EXGETSEL, NULL, (LPARAM)&cr);

	DWORD dwColumn = 0;// SendMessage(hControl, EM_LINELENGTH, cr.cpMin, NULL);

	LRESULT dwLineIndex = SendMessage(hControl, EM_EXLINEFROMCHAR, 0, cr.cpMax);

	WCHAR buf[32];
	wsprintf(buf, L" Ln %d, Col %d", dwLineIndex + 1, dwColumn + 1);

	HWND hStatusBar = GetDlgItem(hWnd, IDC_STATUS_BAR);
	SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)buf);

	UpdateWindow(hStatusBar);
}

BOOL SelectionHasChanged(UINT message, LPARAM lParam)
{
	if (message == WM_SETCURSOR)
	{
		return HIWORD(lParam) != WM_MOUSEMOVE;
	}

	return TRUE;
}