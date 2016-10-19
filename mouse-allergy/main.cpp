#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <math.h>

static POINT cursor = {};
static const auto THRESHOLD = 16.0f;

int RunAway(const long& cursor, const long& base, const long &center)
{
	auto dist = abs(cursor - center);
	auto window_center = center - base;

	auto m = (window_center - dist) / (float)window_center;
	if (cursor >= center)
		m = -m;

	return (int)(THRESHOLD * m);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);

	if (cursor.x < rect.left || cursor.x > rect.right
		|| cursor.y < rect.top || cursor.y > rect.bottom)
	{
		return true;
	}

	int x = rect.left;
	int y = rect.top;

	if (cursor.x >= rect.left && cursor.x <= rect.right)
	{
		auto center = rect.left + (rect.right - rect.left) / 2;
		x += RunAway(cursor.x, rect.left, center);
	}

	if (cursor.y >= rect.top && cursor.y <= rect.bottom)
	{
		auto center = rect.top + (rect.bottom - rect.top) / 2;
		y += RunAway(cursor.y, rect.top, center);
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