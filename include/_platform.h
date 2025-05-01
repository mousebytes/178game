#ifndef _PLATFORM_H
#define _PLATFORM_H

#include<_textureLoader.h>
#include<_timer.h>

class _platform
{
    public:
        _platform();
        virtual ~_platform();

    _textureLoader *tex = new _textureLoader();

    vec3 pos, scale;
    float xMax,yMax,xMin,yMin;
    vec3 vert[4];
    int framesX, framesY;

    void initPlat(const char*,float,float,float,float,float,float,int,int,int,float,float,int);
    void drawPlat();

    enum PlatformType {STATIC, HORIZONTAL, VERTICAL};

    PlatformType type;

    enum PlatTex {STONE, DIRT};

    PlatTex chooseTex;

    float speed;
    float patrol_range, start_pos;
    bool moving_forward;

    void updatePlat();

    _timer *timer = new _timer();

    protected:

    private:
};

#endif // _PLATFORM_H
