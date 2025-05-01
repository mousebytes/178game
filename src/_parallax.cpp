#include "_parallax.h"

_parallax::_parallax()
{
    //ctor

    xMax = 1.0;
    xMin = 0.0;
    yMax = 1.0;
    yMin = 0.0;
    speed = 0.05;
    tmr->reset();
}

_parallax::~_parallax()
{
    //dtor
}
void _parallax::drawBackground(float width, float height,float z, float s)
{
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0,1.0,1.0);
    background->textureBinder();

    glBegin(GL_POLYGON);
      glTexCoord2f(xMin,yMax);
      glVertex3f(-width/height * s,-1*s,z);

      glTexCoord2f(xMax,yMax);
      glVertex3f(width/height * s,-1*s,z);

      glTexCoord2f(xMax,yMin);
      glVertex3f(width/height *s,1*s,z);

      glTexCoord2f(xMin,yMin);
      glVertex3f(-width/height*s,1*s,z);
    glEnd();
    glEnable(GL_DEPTH_TEST);
}

void _parallax::initPrlx(char* fileName)
{
    background->loadTexture(fileName);
}



void _parallax::scroll()
{
 //  if(Auto)

       if(tmr->getTicks()>50)
       {
       if(state == RIGHT)
       {
           xMin +=speed;
           xMax +=speed;
       }
       if(state == LEFT)
       {
           xMin -=speed;
           xMax -=speed;
       }
       else
       {
           xMin=xMin;
           xMax=xMax;
       }

       tmr->reset();
       }// end timer check

}
