#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include<_common.h>
#include<_textureLoader.h>


class _background
{
    public:
        _background();
        virtual ~_background();
        _textureLoader *background = new _textureLoader();

        void drawBackground(float,float);
        void initBG(char*);

        float xMax,xMin,yMax,yMin;



    protected:

    private:
};

#endif // _BACKGROUND_H
