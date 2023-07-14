#pragma once
#include <cstdint>
#include "ArkanoidScratch.h"
#include "Utilities.h"

class AActor;

// backbuffer
extern uint32_t ScreenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

using TColor = uint32_t;

struct FRGB
{
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;

	// Unused variable
	uint8_t Alpha;

	FRGB( uint8_t NewRed, uint8_t NewGreen, uint8_t NewBlue, uint8_t NewAlpha = 0 ) : Red{ NewRed }, Green{ NewGreen }, Blue{ NewBlue }, Alpha{ NewAlpha } {}
};

class Drawer
{
	// Methods
public:
	static uint32_t RgbToTColor( const FRGB RgbColor );
	static void DrawRectangle( const FLeftTopPos Pos, const FSize Size, FRGB RgbColor /** 8 bits per R, G, B */ );
	static void DrawActor( const AActor& Actor );
};
