#include "Ball.h"

ABall::ABall( std::shared_ptr<AActor> NewCharacterPtr,
              std::vector<std::shared_ptr<AActor>>& NewPoolActorsRef,
              uint32_t ( &NewScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH],
              FLeftTopPos NewPos,
              FSize NewSize,
              FRGB NewColor,
              float NewSpeed,
              FVector NewDirection,
              ECollidedSide NewCollidedSides )
    : CharacterPtr{ NewCharacterPtr }, AActor( NewScreenBuffer, NewPos, NewSize, NewColor, NewSpeed, NewDirection ), CollidedSides{ NewCollidedSides }, PoolActorsRef{ NewPoolActorsRef }
{
}

bool ABall::ProcessWallCollision()
{
	bool IsCollided{ false };

	// Bottom Y coordinate
	bool IsCollidedByBottomSide = static_cast<uint32_t>( LeftTopPos.Top + Size.Height ) == SCREEN_HEIGHT;
	if ( IsCollidedByBottomSide )
	{
		AddCollisionSide( ECollidedSide::Bottom );
		IsCollided = true;
	}

	// Top Y Coordinate
	bool IsCollidedByTopSide = static_cast<uint32_t>( LeftTopPos.Top ) == 0;
	if ( IsCollidedByTopSide )
	{
		AddCollisionSide( ECollidedSide::Top );
		IsCollided = true;
	}

	// The most Right side by X coordinate
	bool IsCollidedByRightSide = static_cast<uint32_t>( LeftTopPos.Left + Size.Width ) == SCREEN_WIDTH;
	if ( IsCollidedByRightSide )
	{
		AddCollisionSide( ECollidedSide::Right );
		IsCollided = true;
	}

	// The most Left side by X coordinate
	bool IsCollidedByLeftSide = static_cast<uint32_t>( LeftTopPos.Left ) == 0;
	if ( IsCollidedByLeftSide )
	{
		AddCollisionSide( ECollidedSide::Left );
		IsCollided = true;
	}

	return IsCollided;
}

bool ABall::ProcessActorsCollision()
{
	bool IsCollided{ false };
	IsCollided = IsCollidedWith( CharacterPtr.get() );
	if ( IsCollided )
	{
		return IsCollided;
	}

	for ( auto ActorIter = PoolActorsRef.begin(); ActorIter != PoolActorsRef.end(); ++ActorIter )
	{
		std::shared_ptr<AActor>& ActorSharedPtr = *ActorIter;
		IsCollided = IsCollidedWith( ActorSharedPtr.get() );
		if ( IsCollided )
		{
			PoolActorsRef.erase( ActorIter );
			return IsCollided;
		}
	}

	// ToImplement
	// if ( IsCollided )
	//{
	//	FVector PrevDirection{ GetDirection() };
	//	SetDirection( FVector{ PrevDirection.X, PrevDirection.Y * ( -1.f ) } );
	//}
	// F2DPoint FirstSegmentA Get
	//{
	// if (IsSegmentsIntersectByProjections(F2DSegment{))
	return IsCollided;
}

/* Very primitive collisions
 * ALWAYS CHECK THE COLLISION IN RELEASE BUILD as in debug it's bugging
 * But still it's bugging even in release but very rare. ( hard to catch as this is hard to repro )
 * We check for projection intersection and if
 * find it then just change direction horizontally or vertically.
 * When you reflect the ball with your left or right side
 * the behavior is not well as your speed is greater than the speed of the ball.
 * But there is no protection from overlapping between main character and the ball.
 * To implement such thing we shouldn't allow the ball or the character move further when they collided
 * Or change the ball speed to equal or greater than character's one
 *
 * The problem could be in the calculations of positions. As I calculate them with deltatime
 * and it could happen that next position will be overlapped with another object. But still not sure about it...
 */
void ABall::ProcessCollisions()
{
	ClearCollisionStatus();
	bool IsCollidedWithWall = ProcessWallCollision();
	bool IsCollidedWithActor = ProcessActorsCollision();

	if ( IsCollidedWithWall || IsCollidedWithActor )
	{
		PerformBounce();
	}
}

void ABall::PerformBounce()
{
	// Mirror direction vertically
	if ( IsCollisionSideIncluded( CollidedSides, ECollidedSide::Top ) || IsCollisionSideIncluded( CollidedSides, ECollidedSide::Bottom ) )
	{
		FVector PrevDirection{ GetDirection() };
		SetDirection( FVector{ PrevDirection.X, PrevDirection.Y * ( -1.f ) } );
	}

	// Mirror direction horizontally
	if ( IsCollisionSideIncluded( CollidedSides, ECollidedSide::Left ) || IsCollisionSideIncluded( CollidedSides, ECollidedSide::Right ) )
	{
		FVector PrevDirection{ GetDirection() };
		SetDirection( FVector{ PrevDirection.X * ( -1.f ), PrevDirection.Y } );
	}
}

void ABall::Move( float DeltaTimeMilliseconds )
{
	MoveInDirection( DeltaTimeMilliseconds );

	/* Very primitive collisions
	 * ALWAYS CHECK THE COLLISION IN RELEASE BUILD as in debug it's bugging
	 * But still it's bugging even in release but very rare. ( hard to catch as this is hard to repro )
	 * We check for projection intersection and if
	 * find it then just change direction horizontally or vertically.
	 * When you reflect the ball with your left or right side
	 * the behavior is not well as your speed is greater than the speed of the ball.
	 * But there is no protection from overlapping between main character and the ball.
	 * To implement such thing we shouldn't allow the ball or the character move further when they collided
	 * Or change the ball speed to equal or greater than character's one
	 *
	 * The problem could be in the calculations of positions. As I calculate them with deltatime
	 * and it could happen that next position will be overlapped with another object. But still not sure about it...
	 */
	ProcessCollisions();
}

bool ABall::IsCollidedWith( const AActor* Other )
{
	if ( !Other )
	{
		return false;
	}

	// LOG( "Ball.BottomY = " << GetRightBottomPos().Bottomm );
	// LOG( "Char.TopY = " << Other->GetLeftTopPos().Top );
	LOG( "Ball.RightX = " << GetRightBottomPos().Right );
	LOG( "Char.LeftX = " << Other->GetLeftTopPos().Left );
	if ( static_cast<int>( GetRightBottomPos().Bottom ) == static_cast<int>( Other->GetLeftTopPos().Top ) )
	{
		std::cout << "Debug stop";
	}
	if ( static_cast<int>( GetRightBottomPos().Right ) == static_cast<int>( Other->GetLeftTopPos().Left ) )
	{
		std::cout << "Debug stop";
	}
	bool IsCollided{ false };

	bool IsCollidedByLeftSide = IsSegmentsIntersectByProjections( GetLeftSideRef(), Other->GetRightSideRef() );
	if ( IsCollidedByLeftSide )
	{
		AddCollisionSide( ECollidedSide::Left );
		IsCollided = true;
	}

	bool IsCollidedByTopSide = IsSegmentsIntersectByProjections( GetTopSideRef(), Other->GetBottomSideRef() );
	if ( IsCollidedByTopSide )
	{
		AddCollisionSide( ECollidedSide::Top );
		IsCollided = true;
	}

	bool IsCollidedByRightSide = IsSegmentsIntersectByProjections( GetRightSideRef(), Other->GetLeftSideRef() );
	if ( IsCollidedByRightSide )
	{
		AddCollisionSide( ECollidedSide::Right );
		IsCollided = true;
	}

	bool IsCollidedByBottomSide = IsSegmentsIntersectByProjections( GetBottomSideRef(), Other->GetTopSideRef() );
	if ( IsCollidedByBottomSide )
	{
		AddCollisionSide( ECollidedSide::Bottom );
		IsCollided = true;
	}

	return IsCollided;
}
