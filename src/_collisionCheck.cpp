#include "_collisionCheck.h"

_collisionCheck::_collisionCheck()
{
    //ctor
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

bool _collisionCheck::isEnemyOnGround(_enemies*e , _platform* plat)
{
    float enemy_bottom = e->pos.y - e->scale.y;
    float plat_top = plat->pos.y + plat->scale.y;

    float enemy_left = e->pos.x - e->scale.x;
    float enemy_right = e->pos.x + e->scale.x;

    float plat_left = plat->pos.x - plat->scale.x;
    float plat_right = plat->pos.x + plat->scale.x;

    bool horizontally_aligned = enemy_right > plat_left && enemy_left < plat_right;

    return (enemy_bottom <= plat_top && horizontally_aligned && !(plat->pos.y + 0.1 > e->pos.y));

}

bool _collisionCheck::isPlayerTouchingEnemy(_player* plyr, _enemies* enm)
{
    // we want the player to be able to jump on top of enemies but not run into enemies
    // this function checks if the player is running into the enemy from the side but not from above

    float player_bottom = plyr->plPos.y - plyr->plScl.y;
    float player_top = plyr->plPos.y + plyr->plScl.y;
    float player_left = plyr->plPos.x - plyr->plScl.x;
    float player_right = plyr->plPos.x + plyr->plScl.x;

    float enemy_bottom = enm->pos.y - enm->scale.y;
    float enemy_top = enm->pos.y + enm->scale.y;
    float enemy_left = enm->pos.x - enm->scale.x;
    float enemy_right = enm->pos.x + enm->scale.x;

    bool horizontal_overlap = player_right > enemy_left && player_left < enemy_right;
    bool vertical_overlap = player_bottom < enemy_top && player_top > enemy_bottom;

    bool is_above_enemy = player_bottom >= enemy_top - 0.1f; // slight forgiveness for contact
    bool fell_on_enemy = horizontal_overlap && is_above_enemy;

    return vertical_overlap && horizontal_overlap && !fell_on_enemy;
}

