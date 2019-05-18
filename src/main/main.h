#pragma once

#include <iterator>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <random>
#include "mctd3.h"

// Functions

extern std::function<void(void)> MCTD3_RawTick;
extern std::function<void(void)> MCTD3_Tick;

int RandomInt(int min, int max);

bool MCTD3_LoadOptions();
bool MCTD3_SaveOptions();

bool MCTD3_ProcessEvents();

bool MCTD3_InitSDL();
bool MCTD3_InitUI();

bool MCTD3_EventLoop();
bool MCTD3_RenderLoop();
bool MCTD3_LogicLoop();
bool MCTD3_StartLoops();

bool MCTD3_Init();
bool MCTD3_Close();

// Fields

extern std::map<std::string, int> options;
extern int framerateCap;
extern std::string Title;
extern int nextFrameTicks;
extern int lastTime;

extern bool MCTD3_EventLoop_running;
extern bool MCTD3_RenderLoop_running;
extern bool MCTD3_LogicLoop_running;

extern std::string defaultLabelText;
extern Frame* label;

// Keys

extern int key_moveCameraUp;
extern int key_moveCameraDown;
extern int key_moveCameraLeft;
extern int key_moveCameraRight;
extern int key_toggleFullscreen;