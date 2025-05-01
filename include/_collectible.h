#ifndef _COLLECTIBLE_H
#define _COLLECTIBLE_H

#include<_textureLoader.h>


class _collectible
{
    public:
        _collectible();
        virtual ~_collectible();

        float radius;
        bool isCollected;
        vec3 pos;

        _textureLoader* tex = new _textureLoader();

        void initColl(const char*,float,float,float,float,int,int);
        void drawColl();

        int framesX,framesY;
        float xMax,yMax,xMin,yMin;
        vec3 vert[4];


    protected:

    private:
};

#endif // _COLLECTIBLE_H
