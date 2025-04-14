#include "_enemies.h"

_enemies::_enemies()
{
    //ctor
    pos.x =0.0; pos.y =-0.65; pos.z = -5;
    scale.x =0.25;
    scale.y =0.25;

    rot.x =rot.y=rot.z =0;
    action_trigger =1;
    speed = 0.01;

    xMin= 0;
    yMin= 0;
    frames =1;  //depends on the file

    xMax = 1.0/frames;
    yMax = 1.0;

    timer->reset();
    fall_timer->reset();

    is_grounded = false;

    patrol_range = 1.0;

}

_enemies::~_enemies()
{
    //dtor
}

void _enemies::initEnms(char* file_name)
{
    tex->loadTexture(file_name);
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

void _enemies::placeEnms(vec3 p)
{
    pos.x = p.x;
    pos.y = p.y;
    pos.z = p.z;
    init_x_pos = p.x;
}

void _enemies::actions()
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
            break;
        case WALKRIGHT:
            if(patrol_range + init_x_pos <= pos.x)
            {
                action_trigger = WALKLEFT;
                init_x_pos = pos.x;
            }
            else
            {
                pos.x += speed;
            }

            break;
        case WALKLEFT:
            if(patrol_range - init_x_pos >= pos.x)
            {
                action_trigger = WALKRIGHT;
                init_x_pos = pos.x;
            }
            else
            {
                pos.x -= speed;
            }

            break;
        }
        timer->reset();
    }

}
