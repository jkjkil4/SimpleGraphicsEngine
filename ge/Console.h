#pragma once

#include "utility.h"
#include <iostream>

#define CONSOLE

namespace ge {
	class Console
	{
	public:
		static inline void alloc() {
			AllocConsole();
			//hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			FILE* tempFile = nullptr;
			freopen_s(&tempFile, "conout$", "w+t", stdout);
		}
		static inline void free() {
			FreeConsole();
		}
	};
}
