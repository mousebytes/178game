#ifndef _COLLISIONCHECK_H
#define _COLLISIONCHECK_H

#include<_common.h>


class _collisionCheck
{
    public:
        _collisionCheck();
        virtual ~_collisionCheck();

        bool isPlayerOnGround(vec2,vec2);

    protected:

    private:
};

#endif // _COLLISIONCHECK_H
