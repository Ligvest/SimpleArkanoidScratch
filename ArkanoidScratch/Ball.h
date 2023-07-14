#pragma once
#include "Actor.h"
#include <vector>
class ABall : public AActor
{
	// Ctors, Dtors
public:
	ABall() = delete;
	ABall( std::shared_ptr<AActor> NewCharacterPtr,
	       std::vector<std::shared_ptr<AActor>>& NewPoolActorsRef,
	       uint32_t ( &NewScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH],
	       FLeftTopPos NewPos,
	       FSize NewSize,
	       FRGB NewColor,
	       float NewSpeed = 1.f,
	       FVector NewDirection = FVector{ 0, 0 },
	       ECollidedSide NewCollidedSides = ECollidedSide::None );

	virtual ~ABall() override {}

	// Open Methods
public:
	virtual void Move( float DeltaTimeMilliseconds ) override;

	// Internal Methods
protected:
	bool IsCollidedWith( const AActor* ActorToCheck );
	inline void AddCollisionSide( const ECollidedSide& NewCollisionSide ) { CollidedSides = CollidedSides | NewCollisionSide; }
	inline void ClearCollisionStatus() { CollidedSides = ECollidedSide::None; }
	bool ProcessWallCollision();
	bool ProcessActorsCollision();
	void ProcessCollisions();
	void PerformBounce();

	// Internal Variables
protected:
	// Which side has collided
	ECollidedSide CollidedSides;

	// To process collisions with character
	std::shared_ptr<AActor> CharacterPtr;

	// To process collisions with bricks
	std::vector<std::shared_ptr<AActor>>& PoolActorsRef;
};
