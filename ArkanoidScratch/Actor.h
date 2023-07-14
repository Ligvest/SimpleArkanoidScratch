#pragma once

#include "ArkanoidScratch.h"
#include "Utilities.h"
#include "Drawer.h"

enum class ECollidedSide
{
	None = 0,
	Left = 1,
	Top = 2,
	Right = 4,
	Bottom = 8
};

inline ECollidedSide operator|( ECollidedSide LeftOperand, ECollidedSide RightOperand )
{
	return static_cast<ECollidedSide>( static_cast<int>( LeftOperand ) | static_cast<int>( RightOperand ) );
}

inline bool IsCollisionSideIncluded( const ECollidedSide& LeftOperand, const ECollidedSide& RightOperand )
{
	return static_cast<int>( LeftOperand ) & static_cast<int>( RightOperand );
}

class AActor
{
	// Ctors, Dtors
public:
	AActor() = delete;
	AActor( uint32_t ( &NewScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH], FLeftTopPos NewPos, FSize NewSize, FRGB NewColor, float NewSpeed = 1.f, FVector NewDirection = FVector{ 0, 0 } );

	virtual ~AActor() {}

	// Methods
public:
	void SetInitialCoordinates( uint32_t NewLeftCoord, uint32_t NewTopCoord );
	void SetSize( uint32_t NewHeight, uint32_t NewWidth );

	void RelativeMoveTo( FVector Offset );
	void AbsoluteMoveTo( FLeftTopPos NewPos );

	void MoveRight( float DeltaTimeMilliseconds );
	void MoveLeft( float DeltaTimeMilliseconds );
	void MoveUp( float DeltaTimeMilliseconds );
	void MoveDown( float DeltaTimeMilliseconds );

	void MoveInDirection( float DeltaTimeMilliseconds );

	virtual void Move( float DeltaTimeMilliseconds );

	// Setters, Getters
public:
	inline FLeftTopPos GetLeftTopPos() const { return LeftTopPos; }
	inline FRightBottomPos GetRightBottomPos() const { return RightBottomPos; }
	void SetPos( const FLeftTopPos NewPos );

	inline FSize GetSize() const { return Size; }
	inline void SetSize( const FSize NewSize ) { Size = NewSize; }

	inline FRGB GetColor() const { return Color; }
	inline void SetColor( const FRGB NewColor ) { Color = NewColor; }

	inline FVector GetDirection() const { return Direction; }
	inline void SetDirection( const FVector& NewDirection )
	{
		Direction = NewDirection;
		Direction.Normalize();
	}

	inline float GetSpeed() const { return Speed; }
	inline void SetSpeed( float NewSpeed ) { Speed = NewSpeed; }

	inline const F2DSegment& GetLeftSideRef() const { return LeftSide; }
	inline const F2DSegment& GetTopSideRef() const { return TopSide; }
	inline const F2DSegment& GetRightSideRef() const { return RightSide; }
	inline const F2DSegment& GetBottomSideRef() const { return BottomSide; }
	inline void SetLeftSide( const F2DSegment& NewLeftSide ) { LeftSide = NewLeftSide; }
	inline void SetTopSide( const F2DSegment& NewTopSide ) { TopSide = NewTopSide; }
	inline void SetRightSide( const F2DSegment& NewRightSide ) { RightSide = NewRightSide; }
	inline void SetBottomSide( const F2DSegment& NewBottomSide ) { BottomSide = NewBottomSide; }

	// Variables
protected:
	// A screen buffer to draw the actor on
	uint32_t ( &ScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH];

	// Actor position
	FLeftTopPos LeftTopPos;
	FRightBottomPos RightBottomPos;

	// Actor size
	FSize Size;

	// Actor color
	FRGB Color;

	// Actor Speed
	float Speed;

	// Actor Direction
	FVector Direction;

	F2DSegment LeftSide;
	F2DSegment TopSide;
	F2DSegment RightSide;
	F2DSegment BottomSide;
};
