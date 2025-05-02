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

        void initPlayer(char*); // x frames, y frames, file name
        void drawPlayer();
        void playerActions();
        void handle_vertical();

        enum{STANDING,WALKLEFT,WALKRIGHT,JUMP};
        int action_trigger;
        int health;

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
        _timer *damage_timer = new _timer();
        bool player_can_be_damaged;
        bool blink;

        void handle_player_damage_timer();

        int coins;

        vec3 velocity;

        void handleHorizontalDisplacement();
        bool isBeingDisplacedHorz;
        float xBeforeHorzDisplacement;
        float maxHorzDisplacement;

        _timer *horzDisTimer = new _timer();

        bool inBarrel;
        _timer *animationTimer = new _timer();

        bool facingRight;

        float barrelAngleDeg;
        float displacementTraveled;

    protected:

    private:
};

#endif // _PLAYER_H
