#pragma once

#include "header.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define _T TEXT

inline RECT mkRect(LONG x, LONG y, LONG w, LONG h) {
	return RECT{ x, y, x + w, y + h };
}
