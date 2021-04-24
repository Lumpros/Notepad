#include "EditMenu.h"
#include "Resource.h"
#include "Controls.h"

#include <time.h>

void AppendText(LPCWSTR newText)
{
	HWND hwndOutput = text_edit_handle;
	DWORD StartPos, EndPos;

	SendMessage(hwndOutput, EM_GETSEL, reinterpret_cast<WPARAM>(&StartPos), reinterpret_cast<WPARAM>(&EndPos));

	int outLength = GetWindowTextLength(hwndOutput);

	SendMessage(hwndOutput, EM_SETSEL, outLength, outLength);
	SendMessage(hwndOutput, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(newText));
}

static void AppendSystemTime(void)
{
	time_t t = time(NULL);
	struct tm localtm = *localtime(&t);

	WCHAR date[32];
	wsprintf(date, L"%02d:%02d:%02d %d-%02d-%02d", localtm.tm_hour,
		localtm.tm_min, localtm.tm_sec,
		localtm.tm_year + 1900, localtm.tm_mon + 1,
		localtm.tm_mday);
	AppendText(date);
}

void HandleEditMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_EDIT_TIME_DATE:
		AppendSystemTime();
		break;
	}
}