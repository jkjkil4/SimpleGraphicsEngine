#pragma once

#include <string>

struct Error
{
	enum Type { ER_UNKNOWN, ER_RESETFAILED } type = ER_UNKNOWN;
	std::wstring what;
	Error(Type type, const std::wstring& what) : type(type), what(what) {}
};