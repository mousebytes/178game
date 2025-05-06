#ifndef _FONTS_H
#define _FONTS_H

#include<_textureLoader.h>
#include<_common.h>

class _fonts
{
    public:
        _fonts();
        virtual ~_fonts();

        _textureLoader *tex = new _textureLoader();

        vec3 pos;
        vec2 scale;
        int framesX, framesY;

        float xMin, xMax, yMin, yMax;
        vec3 vert[4];

        void initFonts(const char* fileName, int fx, int fy);
        void setPosition(float x, float y, float z);
        void setSize(float sx,float sy);
        void drawText(string text);
        bool getFontFrame(char c,int& fx, int& fy);

    protected:

    private:
};

#endif // _FONTS_H
