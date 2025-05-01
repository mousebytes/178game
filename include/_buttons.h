#ifndef _BUTTONS_H
#define _BUTTONS_H
#include<_textureLoader.h>


class _buttons
{
    public:

        _buttons();
        virtual ~_buttons();

        _textureLoader *tex = new _textureLoader();

        vec3 pos, scale;
        float xMax,yMax,xMin,yMin;
        vec3 vert[4];
        int framesX, framesY;

        void initButton(const char*,float,float,float,float,float,float,int,int);
        void drawButton();

        bool isHovered(float,float);
        void updateHover(float,float);

    protected:

    private:
};

#endif // _BUTTONS_H
