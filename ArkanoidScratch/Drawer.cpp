#include "Drawer.h"
#include <bit>
#include "Actor.h"

uint32_t Drawer::RgbToTColor( FRGB RgbColor )
{
	return std::bit_cast<TColor>( RgbColor );
}

void Drawer::DrawRectangle( const FLeftTopPos Pos, const FSize Size, FRGB RgbColor /** 8 bits per R, G, B */ )
{
	TColor Color = RgbToTColor( RgbColor );
	for ( uint32_t i = static_cast<uint32_t>( Pos.Top ); i < static_cast<uint32_t>( Pos.Top ) + Size.Height; ++i )
	{
		for ( uint32_t j = static_cast<uint32_t>( Pos.Left ); j < static_cast<uint32_t>( Pos.Left ) + static_cast<uint32_t>( Size.Width ); ++j )
		{
			ScreenBuffer[i][j] = Color;
		}
	}
}

void Drawer::DrawActor( const AActor& Actor )
{
	DrawRectangle( Actor.GetLeftTopPos(), Actor.GetSize(), Actor.GetColor() );
}
