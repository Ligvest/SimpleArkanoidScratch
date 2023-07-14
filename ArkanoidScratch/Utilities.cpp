#include "Utilities.h"
#include <cmath>
#include <algorithm>

void FVector::Normalize()
{
	{
		float Length = static_cast<float>( sqrt( pow( X, 2.f ) + pow( Y, 2.f ) ) );
		X /= Length;
		Y /= Length;
	}
}

bool IsSegmentsIntersectByProjections( const F2DSegment& FirstSegment, const F2DSegment& SecondSegment )
{
	bool IsXProjectionsIntersect = false;
	bool IsYProjectionsIntersect = false;

	// Here we set X and Y so that lesser digit in the var name means lesser value
	// ( X11 is to the left of X12 and Y11 is upper than Y12 )
	uint32_t X11 = ( FirstSegment.A.X > FirstSegment.B.X ) ? FirstSegment.B.X : FirstSegment.A.X;
	uint32_t X12 = ( FirstSegment.A.X < FirstSegment.B.X ) ? FirstSegment.B.X : FirstSegment.A.X;
	uint32_t Y11 = ( FirstSegment.A.Y > FirstSegment.B.Y ) ? FirstSegment.B.Y : FirstSegment.A.Y;
	uint32_t Y12 = ( FirstSegment.A.Y < FirstSegment.B.Y ) ? FirstSegment.B.Y : FirstSegment.A.Y;

	uint32_t X21 = ( SecondSegment.A.X > SecondSegment.B.X ) ? SecondSegment.B.X : SecondSegment.A.X;
	uint32_t X22 = ( SecondSegment.A.X < SecondSegment.B.X ) ? SecondSegment.B.X : SecondSegment.A.X;
	uint32_t Y21 = ( SecondSegment.A.Y > SecondSegment.B.Y ) ? SecondSegment.B.Y : SecondSegment.A.Y;
	uint32_t Y22 = ( SecondSegment.A.Y < SecondSegment.B.Y ) ? SecondSegment.B.Y : SecondSegment.A.Y;

	IsXProjectionsIntersect = max( X11, X21 ) <= min( X12, X22 );
	if ( !IsXProjectionsIntersect )
	{
		return false;
	}

	IsYProjectionsIntersect = max( Y11, Y21 ) <= min( Y12, Y22 );

	return IsXProjectionsIntersect && IsYProjectionsIntersect;
}
