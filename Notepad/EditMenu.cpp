#include "EditMenu.h"
#include "Resource.h"
#include "Controls.h"

#include <time.h>

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

static void Undo(HWND hWnd)
{
	SendMessage(GetDlgItem(hWnd, IDC_TEXT_EDIT), EM_UNDO, NULL, NULL);
}

static void Paste(HWND hWnd)
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

void HandleEditMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_EDIT_TIME_DATE:
		InsertSystemTime(hWnd);
		break;

	case IDM_EDIT_UNDO:
		Undo(hWnd);
		break;

	case IDM_EDIT_PASTE:
		Paste(hWnd);
		break;
	}
}