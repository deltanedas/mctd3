#include "luawrapper.h"

using namespace luabridge;

void Wrapped::Wrapped_SimpleUI_Log(std::string text, int level) {
	SimpleUI_Log(text, (Level) level);
}

int Wrapped::Wrapped_Level(LuaRef ignore, std::string text) {
	if (text.compare("DEBUG") == 0) {
		return (int) Level::DEBUG;
	}
	if (text.compare("INFO") == 0) {
		return (int) Level::INFO;
	}
	if (text.compare("WARN") == 0) {
		return (int) Level::WARN;
	}
	if (text.compare("ERROR") == 0) {
		return (int) Level::ERROR;
	}
	return -1;
}

FC_Font* Wrapped::Wrapped_Fonts(LuaRef ignore, std::string text) {
	return fonts[text];
}

bool Wrapped::Wrapped_Frame_isPointInBounds(Frame* frame, Vec2 point, bool absolute) {
	return frame->isPointInBounds(point, absolute);
}

lua_State* L;

void MCTD3_WrapSimpleUI() {
	L = luaL_newstate();
	luaL_openlibs(L);

	if (L == NULL) {
		throw std::runtime_error("Failed to allocate memory for Lua state.");
	}

	getGlobalNamespace(L)
		.beginNamespace("SimpleUI")
			.addFunction("SimpleUI_Log", Wrapped::Wrapped_SimpleUI_Log)
			.beginNamespace("Level")
				.addFunction("__index", Wrapped::Wrapped_Level)
			.endNamespace()
			.beginNamespace("Fonts")
				.addFunction("__index", Wrapped::Wrapped_Fonts)
			.endNamespace()
			.beginClass<ColourType>("Colour")
				.addConstructor<void(*) (void)>()
				.addConstructor<void(*) (Uint8, Uint8, Uint8, Uint8)>()

				.addFunction("to_string", &ColourType::to_string)
				.addFunction("invert", &ColourType::invert)
				.addFunction("greyScale", &ColourType::greyScale)

				.addProperty("R", &ColourType::R)
				.addProperty("G", &ColourType::G)
				.addProperty("B", &ColourType::B)
				.addProperty("A", &ColourType::A)
			.endClass()
			.beginClass<Frame>("Frame")
				.addConstructor<void(*) (void)>()
				.addFunction("setSize", &Frame::setSize)
				.addFunction("setPosition", &Frame::setPosition)
				.addFunction("setVisible", &Frame::setVisible)
				.addFunction("setAnchored", &Frame::setAnchored)
				.addFunction("setParent", &Frame::setParent)
				.addFunction("setTexture", &Frame::setTexture)
				.addFunction("setText", &Frame::setText)
				//.addFunction("setEventCallback", &Frame::setEventCallback) // doesnt work idk
				.addFunction("setAnimation", &Frame::setAnimation)
				.addFunction("setAnimationFrame", &Frame::setAnimationFrame)
				.addFunction("setPivot", &Frame::setPivot)
				.addFunction("setRotation", &Frame::setRotation)

				.addFunction("getSize", &Frame::getSize)
				.addFunction("getPosition", &Frame::getPosition)
				.addFunction("getAbsoluteSize", &Frame::getAbsoluteSize)
				.addFunction("getAbsolutePosition", &Frame::getAbsolutePosition)
				.addFunction("getVisible", &Frame::getVisible)
				.addFunction("getAnchored", &Frame::getAnchored)
				.addFunction("getParent", &Frame::getParent)
				.addFunction("getChildren", &Frame::getChildren)
				.addFunction("getTexture", &Frame::getTexture)
				.addFunction("getText", &Frame::getText)
				.addFunction("getEventTypes", &Frame::getEventTypes)
				.addFunction("getEventCallbacks", &Frame::getEventCallbacks)
				.addFunction("getAnimation", &Frame::getAnimation)
				.addFunction("getAnimationFrame", &Frame::getAnimationFrame)
				.addFunction("getPivot", &Frame::getPivot)
				.addFunction("getAbsolutePivot", &Frame::getAbsolutePivot)
				.addFunction("getRotation", &Frame::getRotation)

				.addFunction("toggleVisible", &Frame::toggleVisible)
				.addFunction("toggleAnchored", &Frame::toggleAnchored)

				.addFunction("isPointInBounds", &Wrapped::Wrapped_Frame_isPointInBounds)

				.addFunction("nextAnimationFrame", &Frame::nextAnimationFrame)
				.addFunction("prevAnimationFrame", &Frame::prevAnimationFrame)
			.endClass()
			.beginClass<SizeType>("Size")
				.addConstructor<void(*) (void)>()
				//.addConstructor<void(*) (Vec2, Vec2)>()
				.addFunction("setScale", &SizeType::setScale)
				.addFunction("setOffset", &SizeType::setOffset)
			.endClass()
			.beginClass<TextType>("Text")
				.addConstructor<void(*) (void)>()
				.addConstructor<void(*) (FC_Font*, std::string)>()

				.addFunction("setText", &TextType::setText)
				.addFunction("setFont", &TextType::setFont)
				.addFunction("setColour", &TextType::setColour)
				.addFunction("setWrap", &TextType::setWrap)
				.addFunction("setScaleX", &TextType::setScaleX)
				.addFunction("setScaleY", &TextType::setScaleY)
				.addFunction("updateSize", &TextType::updateSize)

				.addFunction("getText", &TextType::getText)
				.addFunction("getFont", &TextType::getFont)
				.addFunction("getColour", &TextType::getColour)
				.addFunction("getWrap", &TextType::getWrap)
				.addFunction("getScaleX", &TextType::getScaleX)
				.addFunction("getScaleY", &TextType::getScaleY)
				.addFunction("getSize", &TextType::getSize)
			.endClass()
			.beginClass<TextureType>("Texture")
				.addConstructor<void(*) (void)>()
				//.addConstructor<void(*) (std::string, Vec2, bool, ColourType*)>()

				.addFunction("setPath", &TextureType::setPath)
				.addFunction("setScale", &TextureType::setScale)
				.addFunction("setClip", &TextureType::setClip)
				.addFunction("setColour", &TextureType::setColour)

				.addFunction("getTexture", &TextureType::getTexture)
				.addFunction("getPath", &TextureType::getPath)
				.addFunction("getScale", &TextureType::getScale)
				.addFunction("getClip", &TextureType::getClip)
				.addFunction("getColour", &TextureType::getColour)
			.endClass()
			.beginClass<Vec2>("Vec2")
				.addConstructor<void(*) (void)>()
				.addConstructor<void(*) (int, int)>()
				.addConstructor<void(*) (double, double)>()

				.addFunction("to_string", &Vec2::to_string)

				.addProperty("X", &Vec2::X)
				.addProperty("Y", &Vec2::Y)
			.endClass()
			.beginClass<FC_Font>("FC_Font")
			.endClass()
		.endNamespace();

	if (luaL_loadfile(L, uiScriptPath.c_str())) {
		throw std::runtime_error("Failed to load UI Script '" + uiScriptPath + "'.");
	}

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
