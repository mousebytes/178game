#ifndef _PLATFORM_H
#define _PLATFORM_H

#include<_textureLoader.h>

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

    void initPlat(char*,float,float,float,float,float,float,int,int);
    void drawPlat();

    protected:

    private:
};

#endif // _PLATFORM_H
