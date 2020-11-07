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
