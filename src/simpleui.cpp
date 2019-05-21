#include "simpleui.h"

int clamp8Bit(int i) {
	return std::clamp(i, 0, 255);
}
// Vec2

Vec2::Vec2(int x, int y) {
	X = static_cast<double>(x);
	Y = static_cast<double>(y);
}

Vec2::Vec2(double x, double y) {
	X = x;
	Y = y;
}

bool Vec2::operator==(const Vec2 compare) {
	return X == compare.X && Y == compare.Y;
}

Vec2 Vec2::operator+(const Vec2 add) {
	return Vec2(X + add.X, Y + add.Y);
}

Vec2 Vec2::operator-(const Vec2 sub) {
	return Vec2(X - sub.X, Y - sub.Y);
}

Vec2 Vec2::operator*(const Vec2 mul) {
	return Vec2(X * mul.X, Y * mul.Y);
}

Vec2 Vec2::operator*(const double mul) {
	return Vec2(X * mul, Y * mul);
}

Vec2 Vec2::operator/(const Vec2 div) {
	return Vec2(X / div.X, Y / div.Y);
}

Vec2 Vec2::operator/(const double div) {
	return Vec2(X / div, Y / div);
}

void Vec2::operator+=(const Vec2 add) {
	Vec2 result = operator+(add);
	X = result.X;
	Y = result.Y;
}

void Vec2::operator-=(const Vec2 sub) {
	Vec2 result = operator-(sub);
	X = result.X;
	Y = result.Y;
}

void Vec2::operator*=(const Vec2 mul) {
	Vec2 result = operator*(mul);
	X = result.X;
	Y = result.Y;
}

void Vec2::operator*=(const double mul) {
	Vec2 result = operator*(mul);
	X = result.X;
	Y = result.Y;
}

void Vec2::operator/=(const Vec2 div) {
	Vec2 result = operator/(div);
	X = result.X;
	Y = result.Y;
}

void Vec2::operator/=(const double div) {
	Vec2 result = operator/(div);
	X = result.X;
	Y = result.Y;
}

std::string Vec2::to_string() {
	return "X: " + std::to_string(X) + ", Y: " + std::to_string(Y);
}

// TextureType

TextureType::TextureType(std::string path, Vec2 scale, bool clip) {
	setClip(clip);
	setScale(scale);
	setPath(path);
}

TextureType::~TextureType() {
	if (Texture) {
		SDL_DestroyTexture(Texture);
	}
}

void TextureType::setPath(std::string path) {
	if (!path.empty()) {
		SDL_ClearError();
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (surface == nullptr) {
			throw std::runtime_error("Image failed to load: " + std::string(SDL_GetError()));
		}
		if (Clip) {
			SDL_SetClipRect(surface, NULL);
		}

		Texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		Path = path;
	}
}

void TextureType::setScale(Vec2 scale) {
	if (scale.X == 0.0) scale.X = 1;
	if (scale.Y == 0.0) scale.Y = 1;
	Scale = scale;
	setPath(Path);
}

void TextureType::setClip(bool clip) {
	Clip = clip;
	setPath(Path);
}

SDL_Texture* TextureType::getTexture() {
	return Texture;
}

std::string TextureType::TextureType::getPath() {
	return Path;
}

Vec2 TextureType::getScale() {
	return Scale;
}

bool TextureType::getClip() {
	return Clip;
}

// TextType

TextType::TextType(FC_Font* font, std::string text) {
	setFont(font);
	setText(text);
}

TextType::~TextType() {
	if (Font) {
		FC_FreeFont(Font);
	}
	if (Colour) {
		delete Colour;
	}
}

void TextType::setText(std::string text) {
	Text = text;
	updateSize();
}

void TextType::setFont(FC_Font* font) {
	Font = font;
}

void TextType::setColour(ColourType* colour) {
	Colour = colour;
	if (Font != NULL) {
		FC_SetDefaultColor(Font, FC_MakeColor(colour->getR(), colour->getG(), colour->getB(), colour->getA()));
	}
}

void TextType::setWrap(bool wrap) {
	Wrap = wrap;
}

void TextType::setScaleX(bool scaleX) {
	ScaleX = scaleX;
}

void TextType::setScaleY(bool scaleY) {
	ScaleY = scaleY;
}

void TextType::updateSize() {
	if (Font != NULL) {
		int w = FC_GetWidth(Font, Text);
		int h = FC_GetHeight(Font, Text);
		Size = Vec2(w, h);
	}
}

std::string TextType::getText() {
	return Text;
}

FC_Font* TextType::getFont() {
	return Font;
}

ColourType* TextType::getColour() {
	return Colour;
}

bool TextType::getWrap() {
	return Wrap;
}

bool TextType::getScaleX() {
	return ScaleX;
}

bool TextType::getScaleY() {
	return ScaleY;
}

Vec2 TextType::getSize() {
	return Size;
}

// SizeType

SizeType::SizeType(Vec2 scale, Vec2 offset) {
	setScale(scale);
	setOffset(offset);
}

void SizeType::setScale(Vec2 scale) {
	Scale = scale;
}

void SizeType::setOffset(Vec2 offset) {
	Offset = offset;
}

std::string SizeType::to_string() {
	return "Scale: {X: " + std::to_string(Scale.X) + ", Y: " + std::to_string(Scale.Y) + "}, Offset: {X: " + std::to_string((int) Offset.X) + ", Y: " + std::to_string((int) Offset.Y) + "}";
}

// ColourType

ColourType::ColourType(int r, int g, int b, int a) {
	setR(r);
	setG(g);
	setB(b);
	setA(a);
}

void ColourType::setR(int r) { R = clamp8Bit(r); }
void ColourType::setG(int g) { G = clamp8Bit(g); }
void ColourType::setB(int b) { B = clamp8Bit(b); }
void ColourType::setA(int a) { A = clamp8Bit(a); }

int ColourType::getR() { return R; }
int ColourType::getG() { return G; }
int ColourType::getB() { return B; }
int ColourType::getA() { return A; }


void ColourType::invert(bool invertAlpha) {
	R = 255 - R;
	G = 255 - G;
	B = 255 - B;
	if (invertAlpha) {
		A = 255 - A;
	}
}

void ColourType::greyScale() {
	int average = (R + G + B) / 3;
	R = average;
	G = average;
	B = average;
}

// EventType

EventType::EventType(Frame* caller) {
	Caller = caller;
}

// Frame

Frame::Frame() {
	Parent = NULL;
	Texture = NULL;
	Text = NULL;
	//Children = {};
	Frames.insert(this);
	EventCallbacks.resize(10);
}

void Frame::setSize(SizeType size) {
	Size = size;
	int totalX = Size.Offset.X;
	int totalY = Size.Offset.Y;

	if (Size.Scale.X > 0.0 || Size.Scale.Y > 0.0) {
		if (Parent == NULL) {
			if (window) {
				totalX += static_cast<int>(windowSize.X * Size.Scale.X);
				totalY += static_cast<int>(windowSize.Y * Size.Scale.Y);
			} else {
				throw std::runtime_error("SDL Window must be created before scaling with it.");
			}
		} else {
			Vec2 parentSize = Parent->getAbsoluteSize();
			int parentX = parentSize.X;
			int parentY = parentSize.Y;

			totalX += static_cast<int>(parentX * Size.Scale.X);
			totalY += static_cast<int>(parentY * Size.Scale.Y);
		}
	}
	AbsoluteSize = Vec2(totalX, totalY);
}

void Frame::setPosition(SizeType position) {
	Position = position;
	int totalX = Position.Offset.X - 2;
	int totalY = Position.Offset.Y - 2;

	if (Position.Scale.X > 0.0 || Position.Scale.Y > 0.0) {
		if (Parent == NULL) {
			int screenX = 0;
			int screenY = 0;

			if (window) {
				SDL_GetWindowSize(window, &screenX, &screenY);
				totalX += static_cast<int>(screenX * Position.Scale.X);
				totalY += static_cast<int>(screenY * Position.Scale.Y);
			} else {
				loggerf("SDL Window must be created before scaling with it.", Level::WARN);
			}
		} else {
			Vec2 parentSize = Parent->getAbsoluteSize();
			Vec2 parentPosition = Parent->getAbsolutePosition();
			int parentX = parentSize.X;
			int parentY = parentSize.Y;
			totalX += parentPosition.X;
			totalY += parentPosition.Y;

			totalX += static_cast<int>(parentX * Position.Scale.X);
			totalY += static_cast<int>(parentY * Position.Scale.Y);
		}
	}

	AbsolutePosition = Vec2(totalX, totalY);
}

void Frame::setVisible(bool visible, bool recursive) {
	Visible = visible;
	if (visible) {
		VisibleFrames.insert(this);
	} else {
		VisibleFrames.erase(this);
	}
	setParent(Parent);
	if (recursive) {
		for (Frame* frame : Children) {
			frame->setVisible(visible, true);
		}
	}
}

void Frame::setAnchored(bool anchored, bool recursive) {
	Anchored = anchored;
	if (recursive) {
		for (Frame* frame : Children) {
			frame->setAnchored(anchored, true);
		}
	}
}

void Frame::setColour(ColourType* colour) {
	Colour = colour;
}

void Frame::setParent(Frame* parent) {
	if (parent == this) {
		throw std::runtime_error("Frame cannot be its own parent.");
	}
	if (Parent != NULL) {
		Parent->Children.erase(this);
	}
	if (parent != NULL) {
		parent->Children.insert(this);
	}
	setPosition(Position);
	setSize(Size);
	Parent = parent;
}

void Frame::setTexture(TextureType* texture) {
	Texture = texture;
}

void Frame::setText(TextType* text) {
	Text = text;
}

void Frame::setEventCallback(EventEnum enumEvent, EventCallback eventCallback) {
	EventCallbacks.at((int) enumEvent) = eventCallback;
}


void Frame::toggleVisible(bool recursive) {
	if (Visible) {
		setVisible(false, recursive);
	} else {
		setVisible(true, recursive);
	}
}

void Frame::toggleAnchored(bool recursive) {
	Anchored = Anchored ? false : true;
	if (recursive) {
		for (Frame* frame : Children) {
			frame->toggleAnchored(true);
		}
	}
}

bool Frame::isPointInBounds(Vec2 point, bool absolute) {
	if (absolute) {
		Vec2 cameraPos = Vec2(camera.X, camera.Y);
		Vec2 pos = AbsolutePosition + cameraPos;
		Vec2 size = AbsoluteSize + AbsolutePosition + cameraPos;
		return (pos.X < point.X) && (pos.Y < point.Y) && (size.X > point.X) && (size.Y > point.Y);
	}
	Vec2 size = AbsoluteSize;
	return (size.X > point.X) && (size.Y > point.Y);
}

bool Frame::isPointInBounds(int pointX, int pointY, bool absolute) {
	return isPointInBounds(Vec2(pointX, pointY), absolute);
}


SizeType Frame::getSize() {
	return Size;
}

SizeType Frame::getPosition() {
	return Position;
}

Vec2 Frame::getAbsoluteSize() {
	return AbsoluteSize;
}

Vec2 Frame::getAbsolutePosition() {
	return AbsolutePosition;
}

bool Frame::getVisible() {
	return Visible;
}

bool Frame::getAnchored() {
	return Anchored;
}

ColourType* Frame::getColour() {
	return Colour;
}

Frame* Frame::getParent() {
	return Parent;
}

std::set<Frame*> Frame::getChildren() {
	return Children;
}

TextureType* Frame::getTexture() {
	return Texture;
}

TextType* Frame::getText() {
	return Text;
}

std::set<EventEnum> Frame::getEventTypes() {
	return EventTypes;
}

std::vector<EventCallback> Frame::getEventCallbacks() {
	return EventCallbacks;
}


void cleanUpFrame(Frame* frame) {
	if (frame) {
		for (Frame* child : frame->getChildren()) {
			cleanUpFrame(child);
			child->setParent();
		}
	}
	loggerf("\tRemoving frame from sets.", Level::DEBUG);
	VisibleFrames.erase(frame);
	Frames.erase(frame);
}

int cleanUpFrames() {
	int ret = 0;
	std::set<Frame*> frames = Frames;
	for (Frame* frame : frames) {
		loggerf("\tCleaning up frame children.", Level::DEBUG);
		cleanUpFrame(frame);
		/*loggerf("\tCleaning up frame colour.", Level::DEBUG);
		if (frame->getColour()) {
			delete frame->getColour();
		}
		loggerf("\tCleaning up frame texture.", Level::DEBUG);
		if (frame->getTexture()) {
			delete frame->getTexture();
		}
		loggerf("\tCleaning up frame text.", Level::DEBUG);
		if (frame->getText()) {
			delete frame->getText();
		}*/
		loggerf("\tDeleting frame.", Level::DEBUG);
		//delete frame;
		ret++;
	}
	return ret;
}

int updateFrames() {
	int ret = 0;
	for (Frame* frame : Reverse(VisibleFrames)) {
		frame->setParent(frame->getParent());
		ret++;
	}
	return ret;
}

bool mousePressed;

void updateEvents() {
	switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			mousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			for (Frame* frame : VisibleFrames) {
				if (mousePressed) {
					EventCallback callback = frame->getEventCallbacks()[(int) EventEnum::MOUSE_CLICK];
					if (callback != NULL) {
						EventType callbackEvent(frame);
						callbackEvent.MouseButtonEvent = event.button;
						callback(callbackEvent);
					}
				}
			}
			mousePressed = false;
			break;
		case SDL_MOUSEMOTION:
			for (Frame* frame : VisibleFrames) {
				EventCallback callback = frame->getEventCallbacks()[(int) EventEnum::MOUSE_MOVED];
				if (callback != NULL) {
					EventType callbackEvent(frame);
					callbackEvent.MouseMotionEvent = event.motion;
					callback(callbackEvent);
				}
			}
			break;
		case SDL_KEYDOWN:
			for (Frame* frame : VisibleFrames) {
				EventCallback callback = frame->getEventCallbacks()[(int) EventEnum::KEY_DOWN];
				if (callback != NULL && event.key.repeat == 0) {
					EventType callbackEvent(frame);
					callbackEvent.KeyboardEvent = event.key;
					callback(callbackEvent);
				}
			}
			break;
		case SDL_KEYUP:
			for (Frame* frame : VisibleFrames) {
				EventCallback callback = frame->getEventCallbacks()[(int) EventEnum::KEY_UP];
				if (callback != NULL && event.key.repeat == 0) {
					EventType callbackEvent(frame);
					callbackEvent.KeyboardEvent = event.key;
					callback(callbackEvent);
				}
			}
			break;
		/*case SDL_MOUSEWHEEL:
			for (Frame* frame : VisibleFrames) {
				EventCallback callback = frame->getEventCallbacks()[(int) EventEnum::SCROLL];
				if (callback != NULL) {
					EventType callbackEvent(frame);
					callbackEvent.MouseWheelEvent = event.scroll;
					callback(callbackEvent);
				}
			}
			break;*/
	}
	for (Frame* frame : VisibleFrames) {
		if (mousePressed) {
			EventCallback callback = frame->getEventCallbacks()[(int) EventEnum::MOUSE_DOWN];
			if (callback != NULL) {
				EventType callbackEvent(frame);
				callbackEvent.MouseButtonEvent = event.button;
				callback(callbackEvent);
			}
		} else {
			EventCallback callback = frame->getEventCallbacks()[(int) EventEnum::MOUSE_UP];
			if (callback != NULL) {
				EventType callbackEvent(frame);
				callbackEvent.MouseButtonEvent = event.button;
				callback(callbackEvent);
			}
		}
	}
}

std::set<Frame*> Frames;
std::set<Frame*> VisibleFrames;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Surface* screenSurface = nullptr;
SDL_Event event;

bool MCTD3_closing = false;
bool MCTD3_restarting = false;
bool MCTD3_paused = false;
bool MCTD3_debug = false;
bool MCTD3_fullscreen = false;

Vec2 screenSize;
double aspectRatio;
int tickCount = 0;
Vec2 windowSize;
Vec2 cameraBounds;

void Camera::Move(int x, int y) {
	Move((double) x, (double) y);
}

void Camera::Move(double x, double y) {
	X -= x;
	Y -= y;
	if (X > cameraBounds.X) {
		X = cameraBounds.X;
	} else if (X < 0) {
		X = 0;
	}
	if (Y > cameraBounds.Y) {
		Y = cameraBounds.Y;
	} else if (Y < 0) {
		Y = 0;
	}
}

Camera camera;
