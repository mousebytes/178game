#ifndef _ENEMIES_H
#define _ENEMIES_H
#include<_common.h>
#include<_timer.h>
#include<_textureLoader.h>

class _enemies
{
    public:
        _enemies();
        virtual ~_enemies();

        _timer *timer = new _timer();
        _timer *fall_timer = new _timer();
        _textureLoader *tex = new _textureLoader();
        float xMax,xMin,yMax,yMin;
        float speed;

        bool is_grounded;

        vec3 pos;
        vec2 scale;
        vec3 rot;
        int frames;
        int action_trigger;

        enum{STANDING,WALKLEFT,WALKRIGHT};

        void initEnms(const char*);
        void drawEnms(GLuint);
        void placeEnms(vec3,float);
        void actions();
        bool isEnmsLive = true;

        // for patrol routes
        float patrol_range, init_x_pos;

    protected:

    private:
};

#endif // _ENEMIES_H
