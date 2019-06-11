#pragma once

#include "main/main.h"
#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>
#include <LuaBridge/LuaBridge.h>

namespace Wrapped {
	void Wrapped_SimpleUI_Log(std::string text, Level level);
}

void MCTD3_WrapSimpleUI();
