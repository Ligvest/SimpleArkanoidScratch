#pragma once
#include <cstdint>

/************************************************
 * Debug Utilities
 ************************************************/

// Debug LOG - BEGIN
#include <sstream>
#include <iostream>
#ifdef _MSC_VER
#include "Windows.h"
#endif

#if !defined( NDEBUG ) && defined( _MSC_VER )
#define LOG( args )                                                              \
	{                                                                        \
		std::stringstream _ss;                                           \
		_ss << __FILE__ << "@" << __LINE__ << ": " << args << std::endl; \
		OutputDebugString( _ss.str().c_str() );                          \
	}
#else
#define LOG( args )
#endif
// Debug LOG - END

/************************************************
 * Structures
 ************************************************/
struct F2DPoint
{
	uint32_t X;
	uint32_t Y;
};

struct F2DSegment
{
	F2DPoint A;
	F2DPoint B;
};

struct FVector
{
	float X;
	float Y;

	void Normalize();
	inline FVector operator*( const float& Factor ) { return FVector{ X * Factor, Y * Factor }; }
};

struct FLeftTopPos
{
	float Left;
	float Top;
};

struct FRightBottomPos
{
	float Right;
	float Bottom;
};

struct FSize
{
	uint32_t Width;
	uint32_t Height;
};

/************************************************
 * Functions
 ************************************************/

bool IsSegmentsIntersectByProjections( const F2DSegment& FirstSegment, const F2DSegment& SecondSegment );
