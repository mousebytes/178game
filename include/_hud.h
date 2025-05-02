#ifndef _HUD_H
#define _HUD_H

#include<_common.h>
#include<_textureLoader.h>
#include<_fonts.h>
#include<_camera.h>
#include<_buttons.h>

class _hud
{
    public:
        _hud();
        virtual ~_hud();

    _textureLoader* heartTex = new _textureLoader();

    void initHud(char* heartFile,int,int,vec3);
    void drawHearts(int health, vec3 camPos);

    vec3 vert[4];
    float xMin,xMax,yMin,yMax;
    vec3 pos;
    vec3 scl;

    int framesX;
    int framesY;

    _fonts *bananaCount = new _fonts();

    void updateBananaCount(int bananas, _camera*);

    _buttons *bananaIcon = new _buttons();

    void drawBananaIcon(_camera*);

    protected:

    private:
};

#endif // _HUD_H
