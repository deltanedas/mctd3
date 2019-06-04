#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_fontcache.h>
#include <thread>
#include <set>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <type_traits>
#include <climits>
#include "logger.h"

/* USER GUIDE
 *
 * 1. Frame* frame = new Frame();
 * 2. cleanUpFrames(); // at end of code
 * 3. glhf
 *
 * - look at my example (NOT EXEMPLARY BY ANY MEANS) code if youre stuck
 * - -lSDL2 -lSDL2_image -lSDL2_ttf -l SDL2_fontcache -lpthread
 * - use them for linking
 * - i highly recommend splitting your loops for input, logic, rendering and something else into different threads.
 * - when in doubt, THING != NULL
 */
class Frame;
class EventType;
typedef void (*EventCallback)(EventType);

// for (Class c : Reverse(classes))
template<class Rng> //Use this if i fuck up with render order
class Reverse
{
	Rng const& rng;
public:
	Reverse(Rng const& r) noexcept
	:
	rng(r)
	{}

	auto begin() const noexcept { using std::end; return std::make_reverse_iterator(end(rng)); }
	auto end()   const noexcept { using std::begin; return std::make_reverse_iterator(begin(rng)); }
};

// Clamps int 'i' between 0 and 255.
int clamp8Bit(int i);

class Vec2 {
	public:
		Vec2() {}
		Vec2(int x, int y);
		Vec2(double x, double y);
		bool operator==(const Vec2 compare);
		// Add X2, Y2 to X1, Y1
		Vec2 operator+(const Vec2 add);
		// Take X2, Y2 from X1, Y1
		Vec2 operator-(const Vec2 sub);
		// Multiply X1, Y1 by X2, Y2
		Vec2 operator*(const Vec2 mul);
		// Multiply X1, Y1 by mul
		Vec2 operator*(const double mul);
		// Divide X1, Y1 by X2, Y2
		Vec2 operator/(const Vec2 div);
		// Divide X1, Y1 by div
		Vec2 operator/(const double div);

		// Set value to result of addition
		void operator+=(const Vec2 add);
		// Set value to result of subtraction
		void operator-=(const Vec2 sub);
		// Set value to result of multiplication by Vec2
		void operator*=(const Vec2 add);
		// Set value to result of multiplication by double
		void operator*=(const double add);
		// Set value to result of division by Vec2
		void operator/=(const Vec2 div);
		// Set value to result of division by double
		void operator/=(const double div);

		// Returns "X: <x>, Y: <y>", useful for debugging.
		std::string to_string();
		double X;
		double Y;
};

class ColourType {
public:
	ColourType() {}
	ColourType(int r, int g, int b, int a = 255);
	
	// Setters
	void setR(int r);
	void setG(int g);
	void setB(int b);
	void setA(int a);
	
	// Getters
	int getR();
	int getG();
	int getB();
	int getA();
	
	// Transforming
	void invert(bool invertAlpha = false);
	void greyScale();
private:
	int R;
	int G;
	int B;
	int A;
};

class TextureType {
	public:
		TextureType();
		TextureType(std::string path, Vec2 scale = Vec2(1.0, 1.0), bool clip = false, ColourType* colour = new ColourType(255, 255, 255));
		~TextureType();

		// Setters
		void setPath(std::string path);
		void setScale(Vec2 scale);
		void setClip(bool clip);
		void setColour(ColourType* colour);

		// Getters
		SDL_Texture* getTexture();
		std::string getPath();
		Vec2 getScale();
		bool getClip();
		ColourType* getColour();
	private:
		SDL_Texture* Texture;
		std::string Path;
		Vec2 Scale;
		bool Clip;
		ColourType* Colour;
};

enum class EventEnum {
	MOUSE_MOVED,
	MOUSE_CLICK,
	MOUSE_UP,
	MOUSE_DOWN,
	SCROLL,
	SCROLL_UP,
	SCROLL_DOWN,
	KEY_PRESS,
	KEY_UP,
	KEY_DOWN
};

class EventType {
public:
	EventType() {}
	EventType(Frame* caller);

	Frame* Caller;
	SDL_MouseMotionEvent MouseMotionEvent;
	SDL_MouseButtonEvent MouseButtonEvent;
	SDL_MouseWheelEvent MouseWheelEvent;
	SDL_KeyboardEvent KeyboardEvent;
};

class TextType {
	public:
		TextType();
		TextType(FC_Font* font, std::string text = "");
		~TextType();

		// Setters
		void setText(std::string text);
		void setFont(FC_Font* font);
		void setColour(ColourType* colour);
		void setWrap(bool wrap = true);
		void setScaleX(bool scaleX = true);
		void setScaleY(bool scaleY = true);
		void updateSize();

		// Getters
		std::string getText();
		FC_Font* getFont();
		ColourType* getColour();
		bool getWrap();
		bool getScaleX();
		bool getScaleY();
		Vec2 getSize();

		// Togglers
		void toggleWrap();
		void toggleScaleX();
		void toggleScaleY();
	private:
		std::string Text = "";       // R/W
		FC_Font* Font = NULL;        // R/W
		ColourType* Colour;          // R/W
		bool Wrap = false;           // R/W
		bool ScaleX = false;         // R/W
		bool ScaleY = false;         // R/W
		Vec2 Size;                   // R/(W)
};

class SizeType {
	public:
		SizeType();
		SizeType(Vec2 scale, Vec2 offset);

		void setScale(Vec2 scale);
		void setOffset(Vec2 offset);

		//Returns a string "Scale: {X: <sX>, Y: <sY>}, Offset: {X: <oX>, Y: <oY>}", useful for debugging.
		std::string to_string();

		Vec2 Scale;
		Vec2 Offset;
};

// Known bug:
// 1. create frames X long
// 2. switch to last frame
// 3. set frames to be X - Y long
// 4. segfault
// 5. dont modify animations on the go?

class AnimationType {
	public:
		AnimationType(std::vector<TextureType*> frames = {});

		//void setFrames(std::vector<TextureType*> frames = {});

		std::vector<TextureType*> getFrames();

		bool operator() ();
	private:
		std::vector<TextureType*> Frames = {};
};

class Frame {
	public:
		Frame();

		// Setters
		void setSize(SizeType size);
		void setPosition(SizeType position);
		void setVisible(bool visible = true, bool recursive = false);
		void setAnchored(bool anchored = true, bool recursive = false);
		void setParent(Frame* parent = NULL);
		void setTexture(TextureType* texture = NULL);
		void setText(TextType* text = NULL);
		void setEventCallback(EventEnum enumEvent, EventCallback eventCallback = NULL);
		void setAnimation(AnimationType* animation);
		void setAnimationFrame(unsigned int frame);
		void setPivot(Vec2 pivot);
		void setRotation(double rotation);

		// Getters
		SizeType getSize();
		SizeType getPosition();
		Vec2 getAbsoluteSize();
		Vec2 getAbsolutePosition();
		bool getVisible();
		bool getAnchored();
		Frame* getParent();
		std::set<Frame*> getChildren();
		TextureType* getTexture();
		TextType* getText();
		std::set<EventEnum> getEventTypes();
		std::vector<EventCallback> getEventCallbacks();
		AnimationType* getAnimation();
		unsigned int getAnimationFrame();
		Vec2 getPivot();
		double getRotation();

		// Togglers
		void toggleVisible(bool recursive = false);
		void toggleAnchored(bool recursive = false);

		// Logic
		bool isPointInBounds(Vec2 point, bool absolute = false);
		bool isPointInBounds(int pointX, int pointY, bool absolute = false);

		// Other
		void nextAnimationFrame();
		void prevAnimationFrame();
private:
	SizeType Size;
	SizeType Position;
	Vec2 AbsoluteSize;
	Vec2 AbsolutePosition;
	bool Visible = false;
	bool Anchored = false;
	Frame* Parent;
	std::set<Frame*> Children;
	TextureType* Texture;
	TextType* Text;
	std::set<EventEnum> EventTypes;
	std::vector<EventCallback> EventCallbacks;
	AnimationType* Animation;
	unsigned int CurrentFrame = 0;
	Vec2 Pivot;
	int Rotation;
};


void cleanUpFrame(Frame* frame);

int cleanUpTexts();
int cleanUpColours();
int cleanUpTextures();
int cleanUpAnimations();
int cleanUpFrames();

void cleanUpSimpleUI();

int updateFrames();
void updateEvents();

extern std::set<Frame*> FrameInstances;
extern std::set<Frame*> VisibleFrameInstances;
extern std::set<TextType*> TextInstances;
extern std::set<ColourType*> ColourInstances;
extern std::set<TextureType*> TextureInstances;
extern std::set<AnimationType*> AnimationInstances;


extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Surface* screenSurface;
extern SDL_Event event;

extern bool SimpleUI_closing;
extern bool SimpleUI_restarting;
extern bool SimpleUI_paused;
extern bool SimpleUI_debug;
extern bool SimpleUI_fullscreen;

extern Vec2 screenSize;
extern double aspectRatio;
extern int tickCount;
extern Vec2 windowSize;
extern Vec2 cameraBounds;

class Camera {
public:
	Camera() {}
	void Move(int x, int y);
	void Move(double x, double y);
	double X;
	double Y;
};

extern Camera camera;
