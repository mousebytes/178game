#ifndef _COLLISIONCHECK_H
#define _COLLISIONCHECK_H

#include<_common.h>
#include<_player.h>
#include<_platform.h>


class _collisionCheck
{
    public:
        _collisionCheck();
        virtual ~_collisionCheck();

        bool isPlayerOnGround(_player*,_platform *ground);
        bool is_grounded;
    protected:

    private:
};

#endif // _COLLISIONCHECK_H
