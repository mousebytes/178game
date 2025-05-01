#ifndef _BARRELCANNON_H
#define _BARRELCANNON_H
#include<_timer.h>
#include<_textureLoader.h>
#include<_player.h>

class _barrelCannon
{
    public:
        _barrelCannon();
        virtual ~_barrelCannon();

        vec3 pos;
        vec2 scale;

        float rotation;
        bool isAuto; // automatically fired or waits for player to press
        bool playerInside;
        float fireDelay;
        _timer* fireTimer;
        _timer* manualDelay;

        _textureLoader* tex;

        void initBarrel(const char* fileName, vec3 p, float rotDeg, bool autoF, float delay);
        void drawBarrel();
        bool isPlayerInside(vec3 pPos, vec2 pScl);
        void updateB(_player*);

        float xMax,yMax,xMin,yMin;
        vec3 vert[4];
        int framesX, framesY;

    protected:

    private:
};

#endif // _BARRELCANNON_H
