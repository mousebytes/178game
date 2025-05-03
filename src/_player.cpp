#include "_player.h"
#define TIMER_LIMIT 20


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
    coins = 0;
    isBeingDisplacedHorz = false;
    horzDisTimer->reset();
    maxHorzDisplacement = 2.7;
    inBarrel = false;
    animationTimer->reset();
    facingRight = true;

    framesX = 6;
    framesY = 1;

    displacementTraveled = 0;
    barrelAngleDeg=0;

    justExitedBarrel->reset();

}

_player::~_player()
{
    //dtor
}

void _player::initPlayer(char* fileName)
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

    velocity = {0,0,0};

    //get frame count
    //framesX=xfrm;
    //framesY=yfrm;

    xMin=0;
    xMax=1.0/(float)framesX;
    yMax=1.0/(float)framesY;
    yMin=yMax-(1/(float)framesY);

    //yMax = 3.0/framesY;
    //yMin = yMax -(1/(float)framesY);

    pTex->loadTexture(fileName);
}

void _player::drawPlayer()
{
    if(inBarrel) return;

    glPushMatrix();
        glTranslatef(plPos.x,plPos.y,plPos.z);
        if(facingRight)
            glScalef(plScl.x,plScl.y,plScl.z);
        else
            glScalef(-plScl.x,plScl.y,plScl.z);
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
    if(inBarrel) return;
    if(!player_can_be_damaged && justExitedBarrel->getTicks() > 1000)
    {
        xMin = 0.0;
        xMax = 1.0/(float)framesX;
    }
    if(!isBeingDisplacedHorz)
    if(timer->getTicks() > TIMER_LIMIT)
    {
        switch(action_trigger)
        {
        case STANDING:
            if(player_can_be_damaged)
            {
                xMin =1.0/(float)framesX;
                xMax = 2.0/(float)framesX;
                yMax = 1.0/(float)framesY;
                yMin = yMax- (1.0/(float)framesY);
            }

            facingRight = facingRight;
        break;

        case WALKLEFT:

            plPos.x -= speed;
            timer->reset();
            if(player_can_be_damaged)
            if(animationTimer->getTicks() > 120)
            {
                if(xMax >= 4.0/(float)framesX)
                {
                    xMin =1.0/(float)framesX;
                    xMax = 2.0/(float)framesX;
                    yMax = 1.0/(float)framesY;
                    yMin = yMax- (1.0/(float)framesY);
                }
                else
                {
                    xMax +=1.0/(float)framesX;
                    xMin +=1.0/(float)framesX;

                }
                animationTimer->reset();

            }
            facingRight = false;


        break;

    case WALKRIGHT:
            plPos.x += speed;
            timer->reset();
            if(player_can_be_damaged)
            if(animationTimer->getTicks() > 120)
            {
                /*xMax +=1.0/(float)framesX;
                xMin +=1.0/(float)framesX;
                animationTimer->reset();
                facingRight = true;*/

                if(xMax >= 4.0/(float)framesX)
                {
                    xMin =1.0/(float)framesX;
                    xMax = 2.0/(float)framesX;
                    yMax = 1.0/(float)framesY;
                    yMin = yMax- (1.0/(float)framesY);
                }
                else
                {
                    xMax +=1.0/(float)framesX;
                    xMin +=1.0/(float)framesX;

                }

                animationTimer->reset();
            }
            facingRight = true;


        break;
        }
    }
    if(isJumping && player_can_be_damaged)
    {
        if(xMin >=1.0)
        {
            xMax = 5.0/(float)framesX;
            xMax = 4.0/(float)framesX;
        }
        else
        {
            xMax= 1.0;
            xMin = 5.0/(float)framesX;
        }
    }


}

void _player::handle_vertical()
{
    if(inBarrel) return;
    if(isBeingDisplacedHorz) return;

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



void _player::handleHorizontalDisplacement()
{

    if(!isBeingDisplacedHorz) return;

    if(displacementTraveled<maxHorzDisplacement)
    {
        if(horzDisTimer->getTicks()>TIMER_LIMIT)
        {
            float rad = (barrelAngleDeg-90.0) * (3.1415926/180.0);
            float stepX = -cos(rad)*jumping_speed*1.4;
            float stepY = -sin(rad)*jumping_speed*1.4;

            plPos.x+=stepX;
            plPos.y +=stepY;
            displacementTraveled+=jumping_speed;

            horzDisTimer->reset();
        }
    }
    else
        isBeingDisplacedHorz=false;
}

