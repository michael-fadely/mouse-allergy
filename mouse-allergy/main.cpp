#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <unordered_map>

constexpr int THRESHOLD = 32;
constexpr int SPEED     = 16;

static std::unordered_map<HWND, RECT> backup;
static POINT cursor = {};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if (hwnd == GetDesktopWindow() || hwnd == GetShellWindow())
	{
		return true;
	}

	const auto style = GetWindowLong(hwnd, GWL_STYLE);

	if (!(style & WS_VISIBLE))
	{
		return true;
	}

	RECT rect;
	GetWindowRect(hwnd, &rect);

	const auto it = backup.find(hwnd);
	if (it == backup.end())
	{
		backup[hwnd] = rect;
	}

	auto x = rect.left;
	auto y = rect.top;

	rect.left   -= THRESHOLD;
	rect.right  += THRESHOLD;
	rect.top    -= THRESHOLD;
	rect.bottom += THRESHOLD;

	if (cursor.x < rect.left || cursor.x > rect.right ||
	    cursor.y < rect.top || cursor.y > rect.bottom)
	{
		return true;
	}

	const auto center_x = rect.left + (rect.right - rect.left) / 2;
	const auto center_y = rect.top + (rect.bottom - rect.top) / 2;

	auto dir_x = static_cast<float>(cursor.x - center_x);
	auto dir_y = static_cast<float>(cursor.y - center_y);

	const auto length = static_cast<float>(sqrt(dir_x * dir_x + dir_y * dir_y));

	dir_x /= length;
	dir_y /= length;

	x += static_cast<int>(SPEED * -dir_x);
	y += static_cast<int>(SPEED * -dir_y);

	if (x != rect.left || y != rect.top)
	{
		SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_ASYNCWINDOWPOS);
	}

	return true;
}

void main()
{
	while (true)
	{
		// ABORT ABORT ABORT
		const auto modifiers = GetAsyncKeyState(VK_SHIFT) | GetAsyncKeyState(VK_MENU);

		if (modifiers & (1 << 16))
		{
			const auto key = GetAsyncKeyState('W');
			if (key & (1 << 16))
			{
				for (auto& it : backup)
				{
					const auto& rect = it.second;
					SetWindowPos(it.first, nullptr, rect.left, rect.top,
					             rect.right - rect.left, rect.bottom - rect.top, SWP_ASYNCWINDOWPOS);
				}

				return;
			}
		}

		GetCursorPos(&cursor);
		EnumWindows(EnumWindowsProc, 0);
		Sleep(1);
	}
}
