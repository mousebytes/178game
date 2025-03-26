#include "_background.h"


_background::_background()
{
    //ctor
    xMax = 1.0;
    xMin = 0.0;
    yMax = 1.0;
    yMin = 0.0;
}

_background::~_background()
{
    //dtor
}

void _background::drawBackground(float width, float height)
{
    glColor3f(1.0,1.0,1.0);
    background->textureBinder();

    glBegin(GL_POLYGON);
        glTexCoord2f(xMin,yMax);
      glVertex3f(-width/height,-1,-30);

      glTexCoord2f(xMax,yMax);
      glVertex3f(width/height,-1,-30);

      glTexCoord2f(xMax,yMin);
      glVertex3f(width/height,1,-30);

      glTexCoord2f(xMin,yMin);
      glVertex3f(-width/height,1,-30);
    glEnd();
}

void _background::initBG(char* fileName)
{
    background->loadTexture(fileName);
}
