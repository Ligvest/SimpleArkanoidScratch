#pragma once

// Alien file

#include <stdint.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

// backbuffer
extern uint32_t ScreenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

#ifndef VK_ESCAPE
#define VK_ESCAPE 0x1B
#define VK_LEFT   0x25
#define VK_UP     0x26
#define VK_RIGHT  0x27
#define VK_DOWN   0x28
#endif

// Check if the specified button_vk_code key is pressed.
// Valid code are begin with VK_...
bool IsKeyPressed( int button_vk_code );

// Check if a mouse button pressed down ( 0 - left, 1 - right )
// But still you can use IsKeyPressed with VK_LBUTTON and VK_RBUTTON
bool IsMouseButtonPressed( int button );

// Get cursor screen coordinates
int GetCursorPosX();
int GetCursorPoxY();

// Check if the program window is active
bool IsWindowActive();

// Set all screen buffer to black (0, 0, 0)
void ClearScreenBuffer();

// Before gameloop
void Initialize();

// After gameloop
void Finalize();

// Game Tick function
void Tick( float dt );

// Drawing is here
void Draw();

// Request quit
void ScheduleQuitGame();
