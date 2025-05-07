#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include<_common.h>
#include<_textureLoader.h>
#include<_timer.h>
class _explosion
{
    public:
        _explosion();
        virtual ~_explosion();

    vec3 pos;
    float scale;
    int framesX, framesY;
    float xMin,xMax,yMin,yMax;
    int curFrame;
    _timer* frameTimer;
    _textureLoader* tex;

    void initExpl(const char* file, vec3 p, float s, int fx, int fy);

    void update();
    void draw();
    bool isDone();

    protected:

    private:
};

#endif // _EXPLOSION_H
