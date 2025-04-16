#ifndef _HUD_H
#define _HUD_H

#include<_common.h>
#include<_textureLoader.h>

class _hud
{
    public:
        _hud();
        virtual ~_hud();

    _textureLoader* heartTex = new _textureLoader();

    void initHud(char* heartFile,int,int,vec3);
    void drawHearts(int health, vec3 camPos);

    vec3 pos;
    vec3 scl;

    int framesX;
    int framesY;

    float xMax,yMax,xMin,yMin;
    vec3 vert[4];

    protected:

    private:
};

#endif // _HUD_H
