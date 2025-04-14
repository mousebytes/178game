#include "_collisionCheck.h"

_collisionCheck::_collisionCheck()
{
    //ctor
    is_grounded = false;
}

_collisionCheck::~_collisionCheck()
{
    //dtor
}

bool _collisionCheck::isPlayerOnGround(_player* plyr, _platform *ground)
{
    float player_bottom = (plyr->plPos.y - plyr->plScl.y);
    float ground_top = (ground->pos.y + ground->scale.y);

    float player_left  = plyr->plPos.x - plyr->plScl.x * 1.0; //* 0.5f;
    float player_right = plyr->plPos.x + plyr->plScl.x * 1.0; //* 0.5f;
    float ground_left  = ground->pos.x - ground->scale.x;
    float ground_right = ground->pos.x + ground->scale.x;

    bool horizontally_aligned = player_right > ground_left && player_left < ground_right;


    // maybe change the .1 to a function passed threshold later
    return player_bottom <= ground_top && horizontally_aligned && !(ground->pos.y + 0.1 > plyr->plPos.y);
}
