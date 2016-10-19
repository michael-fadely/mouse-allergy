#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <math.h>

static POINT cursor = {};
static const int THRESHOLD = 16;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);

	if (cursor.x < rect.left || cursor.x > rect.right
		|| cursor.y < rect.top || cursor.y > rect.bottom)
	{
		return true;
	}

	auto x = rect.left;
	auto y = rect.top;

	auto dir_x = (float)(cursor.x - (rect.left + (rect.right - rect.left) / 2));
	auto dir_y = (float)(cursor.y - (rect.top + (rect.bottom - rect.top) /2));
	auto m = (float)sqrt((dir_x * dir_x) + (dir_y * dir_y));

	dir_x /= m;
	dir_y /= m;

	x += (int)(THRESHOLD * -dir_x);
	y += (int)(THRESHOLD * -dir_y);

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
		// ABORT ABORT ABORT
		auto helpme = GetAsyncKeyState(VK_ESCAPE);
		if ((1 << 16) & helpme)
		{
			return;
		}

		GetCursorPos(&cursor);
		EnumWindows(EnumWindowsProc, 0);
		Sleep(1);
	}
}