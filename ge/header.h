#pragma once

#include <mutex>

#define VAR_NL_GET_FUNC(mtxName, FuncName, varName, Type)\
    Type get##FuncName() {\
		mtxName.lock();\
		Type temp = varName;\
		mtxName.unlock();\
		return temp;\
	}

#define VAR_NL_SET_FUNC(mtxName, FuncName, varName, Type, Const, Quote)\
    void set##FuncName(Const Type Quote __##varName##__){\
		mtxName.lock();\
		varName = __##varName##__;\
		mtxName.unlock();\
	}

#define VAR_NL_FUNC(FuncName, varName, Type, Const, Quote)\
	std::mutex mtx##FuncName;\
	VAR_NL_GET_FUNC(mtx##FuncName, FuncName, varName, Type)\
    VAR_NL_SET_FUNC(mtx##FuncName, FuncName, varName, Type, Const, Quote)


#define VAR_GET_FUNC(FuncName, varName, Type)\
	Type get##FuncName() const { return varName; }

#define VAR_SET_FUNC(FuncName, varName, Type, Const, Quote)\
	void set##FuncName(Const Type Quote __##varName##__) { varName = __##varName##__; }

#define VAR_FUNC(FuncName, varName, Type, Const, Quote)\
	VAR_GET_FUNC(FuncName, varName, Type)\
	VAR_SET_FUNC(FuncName, varName, Type, Const, Quote)


#define GE_NAMESPACE using namespace ge; using namespace std
#define repeat(i, n) for(int i = 0; i < n; i++)

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