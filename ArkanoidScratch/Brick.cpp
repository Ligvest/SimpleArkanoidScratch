#include "Brick.h"

ABrick::ABrick( uint32_t ( &NewScreenBuffer )[SCREEN_HEIGHT][SCREEN_WIDTH], FLeftTopPos NewPos, FSize NewSize, FRGB NewColor, float NewSpeed, FVector NewDirection )
    : AActor( NewScreenBuffer, NewPos, NewSize, NewColor, NewSpeed, NewDirection )
{
}
