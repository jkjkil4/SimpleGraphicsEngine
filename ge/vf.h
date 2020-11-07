#pragma once

#include <mutex>

#define VAR_NL_GET_FUNC(mutexName, FuncName, varName, Type)\
    Type get##FuncName() {\
		mutexName.lock();\
		Type temp = varName;\
		mutexName.unlock();\
		return temp;\
	}

#define VAR_NL_SET_FUNC(mutexName, FuncName, varName, Type, Const, Quote)\
    void set##FuncName(Const Type Quote __##varName##__){\
		mutexName.lock();\
		varName = __##varName##__;\
		mutexName.unlock();\
	}

#define VAR_NL_FUNC(FuncName, varName, Type, Const, Quote)\
	std::mutex mutex##FuncName;\
	VAR_NL_GET_FUNC(mutex##FuncName, FuncName, varName, Type)\
    VAR_NL_SET_FUNC(mutex##FuncName, FuncName, varName, Type, Const, Quote)


#define VAR_GET_FUNC(FuncName, varName, Type)\
	Type get##FuncName() const { return varName; }

#define VAR_SET_FUNC(FuncName, varName, Type, Const, Quote)\
	void set##FuncName(Const Type Quote __##varName##__) { varName = __##varName##__; }

#define VAR_FUNC(FuncName, varName, Type, Const, Quote)\
	VAR_GET_FUNC(FuncName, varName, Type)\
	VAR_SET_FUNC(FuncName, varName, Type, Const, Quote)
