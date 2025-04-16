#ifndef _COLLISIONCHECK_H
#define _COLLISIONCHECK_H

#include<_common.h>
#include<_player.h>
#include<_platform.h>
#include<_enemies.h>
#include<_collectible.h>

class _collisionCheck
{
    public:
        _collisionCheck();
        virtual ~_collisionCheck();

        bool isPlayerOnGround(_player*,_platform *ground);
        bool isEnemyOnGround(_enemies*,_platform*);
        bool isPlayerTouchingEnemy(_player*,_enemies*);
        bool isPlayerTouchingCollectible(_player* plyr, _collectible* c);

    protected:

    private:
};

#endif // _COLLISIONCHECK_H
