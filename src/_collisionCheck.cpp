#include "_collisionCheck.h"

_collisionCheck::_collisionCheck()
{
    //ctor
}

_collisionCheck::~_collisionCheck()
{
    //dtor
}

bool _collisionCheck::isPlayerOnGround(vec2 playerPos, vec2 groundPos)
{
    return playerPos.y<=groundPos.y; // check if the player is on the ground (T if player on ground)
    /*
    playerPos.y = 3.0;
    groundPos.y = 0.0;

    3.0 <= 0.0 -> F -> player not on ground
    */
}
