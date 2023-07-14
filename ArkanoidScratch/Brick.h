#pragma once
#include "Actor.h"
class ABrick : public AActor
{
	// Ctors, Dtors
public:
	ABrick() = delete;
	ABrick( uint32_t ( &NewScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH], FLeftTopPos NewPos, FSize NewSize, FRGB NewColor, float NewSpeed = 0.f, FVector NewDirection = FVector{ 0, 0 } );

	virtual ~ABrick() override {}
};
