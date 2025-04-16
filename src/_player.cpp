#include "_player.h"
#define TIMER_LIMIT 15


_player::_player()
{
    //ctor
    speed =0.07; // change velocity factor here
    isJumping = false;
    action_trigger = 0;
    timer->reset();
    max_jump_height = 2.7;
    jump_timer->reset();
    jumping_speed = 0.09;
    is_grounded = true;
    health = 3;
    damage_timer->reset();
    player_can_be_damaged = true;
    blink = false;
}

_player::~_player()
{
    //dtor
}

void _player::initPlayer(int xfrm, int yfrm, char* fileName)
{
    // initialize quad
    vert[0].x = -1.0; vert[0].y = -1.0; vert[0].z = -1.0;
    vert[1].x =  1.0; vert[1].y = -1.0; vert[1].z = -1.0;
    vert[2].x =  1.0; vert[2].y =  1.0; vert[2].z = -1.0;
    vert[3].x = -1.0; vert[3].y =  1.0; vert[3].z = -1.0;


    //init player pos
    plPos.x=0;
    //plPos.y = -0.55;
    plPos.y = 0.0;
    plPos.z = -3.0;

    // init player size
    plScl.x=plScl.y=plScl.z=0.5;

    //get frame count
    framesX=xfrm;
    framesY=yfrm;

    xMin=0;
    xMax=1.0/(float)framesX;
    yMax=1.0/(float)framesY;
    yMin=yMax-(1/(float)framesY);

    pTex->loadTexture(fileName);
}

void _player::drawPlayer()
{
    glPushMatrix();
        glTranslatef(plPos.x,plPos.y,plPos.z);
        glScalef(plScl.x,plScl.y,plScl.z);
        glColor3f(1.0,1.0,1.0);

        pTex->textureBinder();

        glBegin(GL_QUADS);
        glTexCoord2f(xMin,yMax);
        glVertex3f(vert[0].x,vert[0].y,vert[0].z);

        glTexCoord2f(xMax,yMax);
        glVertex3f(vert[1].x,vert[1].y,vert[1].z);

        glTexCoord2f(xMax,yMin);
        glVertex3f(vert[2].x,vert[2].y,vert[2].z);

        glTexCoord2f(xMin,yMin);
        glVertex3f(vert[3].x,vert[3].y,vert[3].z);
        glEnd();
    glPopMatrix();
}

void _player::playerActions()
{
    if(timer->getTicks() > TIMER_LIMIT)
    {
        switch(action_trigger)
        {
        case STANDING:
        break;

        case WALKLEFT:
            plPos.x -= speed;
            timer->reset();
        break;

    case WALKRIGHT:
            plPos.x += speed;
            timer->reset();
        break;
        }
    }


}

void _player::handle_vertical()
{
    if(isJumping)
    {

        if(plPos.y < (height_before_jump + max_jump_height))
        {
            if(jump_timer->getTicks() > TIMER_LIMIT)
            {
                plPos.y += jumping_speed; // TODO: change speed to jump speed later
                jump_timer->reset();
            }
        }
        else
            isJumping = false;
    }

    if(!isJumping && !is_grounded)
    {
        if(jump_timer->getTicks() > TIMER_LIMIT)
            {
                plPos.y -=jumping_speed;
                jump_timer->reset();
            }
    }
}

void _player::handle_player_damage_timer()
{
    // i think this is 1 second
    if(damage_timer->getTicks() > 1000 && !player_can_be_damaged)
        player_can_be_damaged = true;
}

