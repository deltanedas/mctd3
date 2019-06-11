#include "luawrapper.h"

using namespace luabridge;

void Wrapped::Wrapped_SimpleUI_Log(std::string text, Level level) {
	SimpleUI_Log(text, level);
}

void MCTD3_WrapSimpleUI() {
	lua_State* L = luaL_newstate();

	Namespace luaNamespace = getGlobalNamespace(L);
	SimpleUI_Log("Nah");
	luaNamespace.beginNamespace("SimpleUI");
		SimpleUI_Log("hmm.");
		luaNamespace.addFunction("SimpleUI_Log", Wrapped::Wrapped_SimpleUI_Log);
	luaNamespace.endNamespace();
	SimpleUI_Log("Odd.");

	if (luaL_loadfile(L, uiScriptPath.c_str())) {
		throw std::runtime_error("Failed to load UI Script '" + uiScriptPath + "'.");
	}
	luaL_openlibs(L);

	int scriptError = lua_pcall(L, 0, 0, 0);
	while (scriptError && lua_gettop(L)) {
		SimpleUI_Log(std::string(lua_tostring(L, -1)), Level::ERROR);
		lua_pop(L, 1);
		scriptError = lua_pcall(L, 0, 0, 0);
	}
	if (scriptError) {
		throw std::runtime_error("Failed to run UI Script.");
	}
}
