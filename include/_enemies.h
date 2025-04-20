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

        void initEnms();
        void drawEnms(GLuint);
        void placeEnms(vec3,float);
        void actions();
        bool isEnmsLive = true;

        void handleWalker();
        void handleJumper();
        void handleFlyer();

        // for patrol routes
        float patrol_range, init_x_pos;
        bool start_respawn_timer;
        _timer *respawn_timer = new _timer();


        // walker does typical patrol
        // jumper jumps every once in a while
        // flyer is unaffected by gravity but has the same patrol route as walker
        enum EnemyType {WALKER, JUMPER, FLYER};

        // default to walker in const
        EnemyType eT;

        _timer *jumpT = new _timer();

        bool facingRight;

        //animatino timer, just lazy
        _timer *pTmer = new _timer();

    protected:

    private:
};

#endif // _ENEMIES_H
