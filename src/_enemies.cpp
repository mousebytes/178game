#include "_enemies.h"

_enemies::_enemies()
{
    //ctor
    pos.x =0.0; pos.y =-0.65; pos.z = -3;
    scale.x =0.25;
    scale.y =0.25;

    rot.x =rot.y=rot.z =0;
    action_trigger =1;
    speed = 0.01;

    xMin= 0;
    yMin= 0;
    frames =4;  //depends on the file

    xMax = 1.0/frames;
    yMax = 1.0;

    timer->reset();
    fall_timer->reset();

    is_grounded = false;

    patrol_range = 1.0;

    start_respawn_timer = false;

    //eT = WALKER;

    jumpT->reset();
    pTmer->reset();

    facingRight = true;
}

_enemies::~_enemies()
{
    //dtor
}

void _enemies::initEnms()
{
    //tex->loadTexture(file_name);
    switch (eT)
    {
        case WALKER: tex->loadTexture("images/temp_enemy.png"); break;
        case JUMPER: tex->loadTexture("images/wall.png"); break;
        case FLYER:  tex->loadTexture("images/temp_flyer.png"); frames = 4; break;
    }
    xMin = 0;
    xMax = 1.0/frames;
    yMin = 0;
    yMax = 1.0;
}

void _enemies::drawEnms(GLuint tx)
{


    if(isEnmsLive)
   {
      glBindTexture(GL_TEXTURE_2D, tx);
      glColor3f(1.0,1.0,1.0);
      //myTex->textureBinder();
    //  glBindTexture(GL_TEXTURE_2D, myTx);

      glPushMatrix();
        glTranslatef(pos.x,pos.y,pos.z);
        glRotatef(rot.x,1,0,0);
        glRotatef(rot.y,0,1,0);
        glRotatef(rot.z,0,0,1);

        if(facingRight)
            glScalef(-scale.x,scale.y,1.0);
        else
            glScalef(scale.x,scale.y,1.0);

         glBegin(GL_POLYGON);

          glTexCoord2f(xMin,yMin);
          glVertex3f(1.0,1.0,0);

          glTexCoord2f(xMax,yMin);
          glVertex3f(-1.0,1.0,0);

          glTexCoord2f(xMax,yMax);
          glVertex3f(-1.0,-1.0,0);

          glTexCoord2f(xMin,yMax);
          glVertex3f(1.0,-1.0,0);

         glEnd();

      glPopMatrix();
   }
}

void _enemies::placeEnms(vec3 p, float scl)
{
    pos.x = p.x;
    pos.y = p.y;
    //pos.z = p.z;
    pos.z = -3.0;
    init_x_pos = p.x;

    scale.x = scale.y = scl;
}

void _enemies::actions()
{
    if(!isEnmsLive)
    {
        if(start_respawn_timer)
        {
            respawn_timer->reset();
            start_respawn_timer = false;
        }
        else if(respawn_timer->getTicks() > 6000)
        {
            isEnmsLive = true;
            respawn_timer->reset();
        }
    }

    switch(eT)
    {
    case WALKER:
        handleWalker();
        break;

    case JUMPER:
        handleJumper();
        break;

    case FLYER:
        handleFlyer();
        break;
    }


}


void _enemies::handleWalker()
{
    if (!is_grounded)
    {
        if (fall_timer->getTicks() > 15)
        {
            pos.y -= speed; // falling speed
            fall_timer->reset();
        }
    }

    if(timer->getTicks() > 20)
    {
        switch(action_trigger)
        {
        case STANDING:
            xMin =0;
            xMax = 1.0/(float)frames;
            yMax = 1.0;
            yMin = yMax - 1.0;

            break;
        case WALKRIGHT:
            facingRight = true;
            if(patrol_range + init_x_pos <= pos.x)
            {
                action_trigger = WALKLEFT;
                if(pTmer->getTicks() >70)
                {
                    xMax +=1.0/(float)frames;
                    xMin +=1.0/(float)frames;
                    pTmer->reset();
                }

            }
            else
            {
                pos.x += speed;
                if(pTmer->getTicks() >70)
                {
                    xMax +=1.0/(float)frames;
                    xMin +=1.0/(float)frames;
                    pTmer->reset();
                }
            }

            break;
        case WALKLEFT:
            facingRight = false;
            if(init_x_pos - patrol_range>= pos.x)
            {
                action_trigger = WALKRIGHT;
                if(pTmer->getTicks() >70)
                {
                    xMax +=1.0/(float)frames;
                    xMin +=1.0/(float)frames;
                    pTmer->reset();
                }
            }
            else
            {
                pos.x -= speed;

                if(pTmer->getTicks() >70)
                {
                    xMax +=1.0/(float)frames;
                    xMin +=1.0/(float)frames;
                    pTmer->reset();
                }
            }

            break;
        }
        timer->reset();
    }
}

void _enemies::handleJumper()
{
    if(is_grounded && jumpT->getTicks() > 3000)
    {
        pos.y += 0.7;
        is_grounded = false;
        jumpT->reset();
    }

    //keep patrol same as walker
    handleWalker();
}

void _enemies::handleFlyer()
{
    if (timer->getTicks() > 20)
    {
        switch(action_trigger)
        {
        case STANDING:
            break;
        case WALKRIGHT:
            if (patrol_range + init_x_pos <= pos.x)
                action_trigger = WALKLEFT;
            else
            {
                pos.x += speed;
                xMin +=1.0/frames;
                xMax +=1.0/frames;
            }

            break;
        case WALKLEFT:
            if (init_x_pos - patrol_range >= pos.x)
                action_trigger = WALKRIGHT;
            else
                {
                    pos.x -= speed;
                    xMin +=1.0/frames;
                    xMax +=1.0/frames;
                }
            break;
        }
        timer->reset();
    }
}

