#pragma once
#include "Actor.h"

class ACharacter : public AActor
{
	// Ctors, Dtors
public:
	ACharacter( uint32_t ( &NewScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH], FLeftTopPos NewPos, FSize NewSize, FRGB NewColor, float NewSpeed = 1.f, FVector NewDirection = FVector{ 0, 0 } );
};
