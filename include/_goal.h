#ifndef _GOAL_H
#define _GOAL_H

#include<_common.h>
#include<_textureLoader.h>

class _goal
{
    public:
        _goal();
        virtual ~_goal();

        _textureLoader *tex = new _textureLoader();

        int framesX;
        int framesY;

        float xMax,yMax,xMin,yMin;
        vec3 vert[4];
        vec3 pos;
        vec3 scl;


        void initGoal(const char*,float,float,float,float,int,int);
        void drawGoal();

    protected:

    private:
};

#endif // _GOAL_H
