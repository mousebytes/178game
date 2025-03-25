#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>


class _scene
{
    public:
        _scene(); //constructor
        virtual ~_scene(); // destructor

        GLint initGL(); // GLint instead of int because this will give you platform independency :) // initialize game objects
        void reSize(GLint width, GLint Height); // resize window
        void drawScene(); // render the final scene


    protected:

    private:
};

#endif // _SCENE_H
