#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static POINT cursor = {};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);

	int x = rect.left;
	int y = rect.top;

	if (cursor.x > rect.left && cursor.x < rect.right)
	{
		auto center = rect.left + (rect.right - rect.left) / 2;

		if (cursor.x < center)
		{
			x += 8;
		}
		else
		{
			x -= 8;
		}
	}

	if (cursor.y > rect.top && cursor.y < rect.bottom)
	{
		auto center = rect.top + ((rect.bottom - rect.top) / 2);
		if (cursor.y < center)
		{
			y += 8;
		}
		else
		{
			y -= 8;
		}
	}

	if (x != rect.left || y != rect.top)
	{
		SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE);
	}

	return true;
}

void main()
{
	while (true)
	{
		GetCursorPos(&cursor);
		EnumWindows(EnumWindowsProc, 0);

		auto whatever = GetAsyncKeyState(VK_ESCAPE);
		if ((1 << 16) & whatever)
		{
			return;
		}

		Sleep(1);
	}
}