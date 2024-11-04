#include <iostream>
#include <windows.h>
#include "./block.h"

void Block::block_mouse() {
	ShowCursor(FALSE);
	POINT point;
	if (GetCursorPos(&point)) {
		RECT rect;
		rect.left = point.x;
		rect.top = point.y;
		rect.right = point.x + 1;
		rect.bottom = point.y + 1;
		ClipCursor(&rect);
	} else {
		std::cerr << "Failed to get cursor position." << std::endl;
	}
}

void Block::unblock_mouse() {
    ShowCursor(TRUE);
    ClipCursor(NULL);
}

void Block::mouse(bool flag) {
	if (flag) block_mouse();
	else unblock_mouse();
}
