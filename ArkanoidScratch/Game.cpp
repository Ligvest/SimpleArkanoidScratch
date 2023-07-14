#include "ArkanoidScratch.h"
#include <stdlib.h>
#include <memory.h>
#include "Drawer.h"
#include "Character.h"
#include "Ball.h"
#include "Brick.h"
#include <vector>
#include <memory>

std::shared_ptr<AActor> MainCharacter;
std::shared_ptr<AActor> Ball;
std::vector<std::shared_ptr<AActor>> PoolActors;

void InitCharacter()
{
	// Create main char and add to the actors pool
	FSize CharSize{ 100, 20 };
	// FLeftTopPos CharPos{ ( SCREEN_WIDTH / 2.f ) - ( CharSize.Width / 2.f ), SCREEN_HEIGHT - 50.f };
	FLeftTopPos CharPos{ ( SCREEN_WIDTH / 2.f ) - ( CharSize.Width / 2.f ), SCREEN_HEIGHT / 2.f };
	MainCharacter = std::make_shared<ACharacter>( ScreenBuffer, CharPos, CharSize, FRGB{ 50, 50, 250 } );
	// PoolActors.emplace_back( MainCharacter );
}

void InitBall()
{
	// Create a ball and add to the actors pool
	FSize BallSize{ 10, 10 };
	FLeftTopPos BallPos{ ( SCREEN_WIDTH / 3.f ), SCREEN_HEIGHT / 1.5f };
	// Init ball to fly bottom right
	FVector Direction{ 1.f, 1.f };
	float Speed{ 0.3f };
	Ball = std::make_shared<ABall>( MainCharacter, PoolActors, ScreenBuffer, BallPos, BallSize, FRGB{ 200, 200, 200 }, Speed, Direction );
	// PoolActors.emplace_back( Ball );
}

void InitMap()
{
	// Create bricks and add to the actors pool
	uint32_t BricksAmount = 36;
	uint32_t RowsAmount = static_cast<uint32_t>( sqrt( BricksAmount ) );
	uint32_t ColsAmount = RowsAmount;

	uint32_t HorizontalIndent = 30;
	uint32_t VerticalIndent = 30;

	// How much space horizontal indents will take ( including outside indents )
	uint32_t HorizontalSpaceOcupiedByIndents = HorizontalIndent * ( RowsAmount + 1 );
	// Subtract indent and calculate a size of each break
	uint32_t BrickWidth = ( SCREEN_WIDTH - HorizontalSpaceOcupiedByIndents ) / RowsAmount;

	// How much space vertical indents will take ( including outside indents )
	uint32_t VerticalSpaceOcupiedByIndents = VerticalIndent * ( ColsAmount + 1 );
	// Subtract indent and calculate a size of each break
	uint32_t BrickHeight = ( ( SCREEN_HEIGHT / 2 ) - VerticalSpaceOcupiedByIndents ) / ColsAmount;

	FSize BrickSize = FSize{ BrickWidth, BrickHeight };

	uint32_t FirstBrickX = HorizontalIndent;
	uint32_t FirstBrickY = VerticalIndent;
	std::shared_ptr<ABrick> Brick;

	for ( uint32_t Row = 0; Row < RowsAmount; ++Row )
	{
		for ( uint32_t Col = 0; Col < ColsAmount; ++Col )
		{
			FLeftTopPos BrickPos{ static_cast<float>( FirstBrickX + ( HorizontalIndent * Col ) + ( BrickSize.Width * Col ) ),
			                      static_cast<float>( FirstBrickY + ( VerticalIndent * Row ) + ( BrickSize.Height * Row ) ) };
			Brick = std::make_shared<ABrick>( ScreenBuffer, BrickPos, BrickSize, FRGB{ 200, 50, 200 } );
			PoolActors.emplace_back( Brick );
		}
	}
}

void Initialize()
{
	InitCharacter();
	InitBall();
	InitMap();
}

void ProcessInput( float DeltaTime )
{
	// Convert seconds to milliseconds
	DeltaTime *= 1000;

	if ( Ball )
	{
		Ball->Move( DeltaTime );
	}
	if ( IsKeyPressed( VK_ESCAPE ) )
	{
		ScheduleQuitGame();
	}

	if ( IsKeyPressed( VK_LEFT ) )
	{
		if ( MainCharacter )
		{
			MainCharacter->MoveLeft( DeltaTime );
		}
	}

	if ( IsKeyPressed( VK_UP ) )
	{
		if ( MainCharacter )
		{
			MainCharacter->MoveUp( DeltaTime );
		}
	}

	if ( IsKeyPressed( VK_DOWN ) )
	{
		if ( MainCharacter )
		{
			MainCharacter->MoveDown( DeltaTime );
		}
	}

	if ( IsKeyPressed( VK_RIGHT ) )
	{
		if ( MainCharacter )
		{
			MainCharacter->MoveRight( DeltaTime );
		}
	}
}

void Tick( float DeltaTimeSeconds )
{
	ProcessInput( DeltaTimeSeconds );
}

// uint32_t ScreenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] - array of pixels (32-bit colors (8 bits per each of R, G and B))
void Draw()
{
	ClearScreenBuffer();

	Drawer::DrawActor( *MainCharacter );

	// Can change from one pointer to array if want to add more balls
	Drawer::DrawActor( *Ball );

	// Draw all actors:
	for ( const std::shared_ptr<AActor> Actor : PoolActors )
	{
		Drawer::DrawActor( *Actor );
	}
}

void Finalize()
{
}
