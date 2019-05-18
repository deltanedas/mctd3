#include "main/main.h"

std::map<std::string, int> options;
int framerateCap = 60;
std::string Title = "Minecraft Tower Defense III";
int nextFrameTicks = 0;
int lastTime = 0;

int key_moveCameraUp = SDL_SCANCODE_W;
int key_moveCameraDown = SDL_SCANCODE_S;
int key_moveCameraLeft = SDL_SCANCODE_A;
int key_moveCameraRight = SDL_SCANCODE_D;
int key_toggleFullscreen = SDLK_F11;

bool MCTD3_EventLoop_running = false;
bool MCTD3_RenderLoop_running = false;
bool MCTD3_LogicLoop_running = false;