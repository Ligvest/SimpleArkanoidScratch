#include "Actor.h"

#include <algorithm>

#include "Drawer.h"

AActor::AActor( uint32_t ( &NewScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH], FLeftTopPos NewPos, FSize NewSize, FRGB NewColor, float NewSpeed, FVector NewDirection )
    : ScreenBuffer{ NewScreenBuffer }, LeftTopPos{ NewPos }, Size{ NewSize }, Color{ NewColor }, Speed{ NewSpeed }
{
	SetPos( LeftTopPos );
	SetDirection( NewDirection );
}

void AActor::RelativeMoveTo( FVector Offset )
{
	float Left = std::clamp( GetLeftTopPos().Left + Offset.X, 0.f, static_cast<float>( SCREEN_WIDTH - Size.Width ) );
	float Top = std::clamp( GetLeftTopPos().Top + Offset.Y, 0.f, static_cast<float>( SCREEN_HEIGHT - Size.Height ) );
	SetPos( FLeftTopPos{ Left, Top } );
}

void AActor::AbsoluteMoveTo( FLeftTopPos NewPos )
{
	SetPos( NewPos );
}

void AActor::MoveRight( float DeltaTimeMilliseconds )
{
	// To the right by X
	SetDirection( FVector{ 1, 0 } );
	RelativeMoveTo( GetDirection() * GetSpeed() * DeltaTimeMilliseconds );
}

void AActor::MoveLeft( float DeltaTimeMilliseconds )
{
	// To the left by X
	SetDirection( FVector{ -1, 0 } );
	RelativeMoveTo( GetDirection() * GetSpeed() * DeltaTimeMilliseconds );
}

void AActor::MoveUp( float DeltaTimeMilliseconds )
{
	// Move Up by Y
	SetDirection( FVector{ 0, -1 } );
	RelativeMoveTo( GetDirection() * GetSpeed() * DeltaTimeMilliseconds );
}

void AActor::MoveDown( float DeltaTimeMilliseconds )
{
	// Move Down by Y
	SetDirection( FVector{ 0, 1 } );
	RelativeMoveTo( GetDirection() * GetSpeed() * DeltaTimeMilliseconds );
}

void AActor::MoveInDirection( float DeltaTimeMilliseconds )
{
	RelativeMoveTo( GetDirection() * GetSpeed() * DeltaTimeMilliseconds );
}

void AActor::Move( float DeltaTimeMilliseconds )
{
	MoveInDirection( DeltaTimeMilliseconds );
}

void AActor::SetPos( const FLeftTopPos NewPos )
{
	LeftTopPos = NewPos;
	RightBottomPos.Bottom = LeftTopPos.Top + GetSize().Height;
	RightBottomPos.Right = LeftTopPos.Left + GetSize().Width;

	F2DPoint A;
	F2DPoint B;

	// Set left side segment
	A = { static_cast<uint32_t>( LeftTopPos.Left ), static_cast<uint32_t>( LeftTopPos.Top ) };
	B = { static_cast<uint32_t>( LeftTopPos.Left ), static_cast<uint32_t>( RightBottomPos.Bottom ) };
	SetLeftSide( F2DSegment{ A, B } );

	// Set top side segment
	A = { static_cast<uint32_t>( LeftTopPos.Left ), static_cast<uint32_t>( LeftTopPos.Top ) };
	B = { static_cast<uint32_t>( RightBottomPos.Right ), static_cast<uint32_t>( LeftTopPos.Top ) };
	SetTopSide( F2DSegment{ A, B } );

	// Set right side segment
	A = { static_cast<uint32_t>( RightBottomPos.Right ), static_cast<uint32_t>( LeftTopPos.Top ) };
	B = { static_cast<uint32_t>( RightBottomPos.Right ), static_cast<uint32_t>( RightBottomPos.Bottom ) };
	SetRightSide( F2DSegment{ A, B } );

	// Set bottom side segment
	A = { static_cast<uint32_t>( LeftTopPos.Left ), static_cast<uint32_t>( RightBottomPos.Bottom ) };
	B = { static_cast<uint32_t>( RightBottomPos.Right ), static_cast<uint32_t>( RightBottomPos.Bottom ) };
	SetBottomSide( F2DSegment{ A, B } );
}
