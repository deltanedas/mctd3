#pragma once

#include "main/main.h"
#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>
#include <LuaBridge/LuaBridge.h>

namespace Wrapped {
	void Wrapped_SimpleUI_Log(std::string text, int level);
	int Wrapped_Level(luabridge::LuaRef ignore, std::string text);
	FC_Font* Wrapped_Fonts(luabridge::LuaRef ignore, std::string text);
	bool Wrapped_Frame_isPointInBounds(Frame* frame, Vec2 point, bool absolute = false);
}

extern lua_State* L;

void MCTD3_WrapSimpleUI();
