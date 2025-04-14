#ifndef _PLAYER_H
#define _PLAYER_H
#include<_textureLoader.h>

#include<_timer.h>

class _player
{
    public:
        _player();
        virtual ~_player();

        _textureLoader *pTex = new _textureLoader();

        void initPlayer(int, int, char*); // x frames, y frames, file name
        void drawPlayer();
        void playerActions();
        void handle_vertical();

        enum{STANDING,WALKLEFT,WALKRIGHT,JUMP};
        int action_trigger;

        vec3 plPos;
        vec3 plScl;

        int framesX;
        int framesY;

        float xMax,yMax,xMin,yMin;
        vec3 vert[4];

        float speed;
        bool isJumping, is_grounded;
        float height_before_jump, max_jump_height;
        float jumping_speed;

        _timer *timer = new _timer();
        _timer *jump_timer = new _timer();

    protected:

    private:
};

#endif // _PLAYER_H
