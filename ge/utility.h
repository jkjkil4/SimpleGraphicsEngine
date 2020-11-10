#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GE_NAMESPACE using namespace ge; using namespace std
#define _T TEXT

namespace ge {
	template<typename T>inline void SafeDelete(T*& t) {
		if (t) {
			delete t;
			t = nullptr;
		}
	}
	template<typename T>inline void SafeDeleteArray(T*& t) {
		if (t) {
			delete[] t;
			t = nullptr;
		}
	}

}