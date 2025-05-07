#include "_explosion.h"

_explosion::_explosion()
{
    //ctor
    tex = new _textureLoader();
    frameTimer=new _timer();
}

_explosion::~_explosion()
{
    //dtor
    delete tex;
    delete frameTimer;
}


void _explosion::initExpl(const char* file, vec3 p, float s, int fx, int fy)
{
    pos = p;
    scale = s;
    framesX = fx;
    framesY = fy;
    curFrame = 0;
    frameTimer->reset();
    tex->loadTexture(file);
    xMin = yMin = 0;
    xMax = 1.0f / framesX;
    yMax = 1.0f / framesY;
}

void _explosion::update()
{
    if (frameTimer->getTicks() > 50) {            // 50 ms per frame
      ++curFrame;
      if (curFrame < framesX * framesY) {
        int cx = curFrame % framesX;
        int cy = curFrame / framesX;
        xMin = cx * (1.0f/framesX);
        xMax = xMin+(1.0f/framesX);
        yMin = cy* (1.0f/framesY);
        yMax = yMin + (1.0f/framesY);
      }
      frameTimer->reset();
    }
}

void _explosion::draw()
{
    if (curFrame >= framesX*framesY) return;
    glBindTexture(GL_TEXTURE_2D, tex->tex);
    glPushMatrix();
      glTranslatef(pos.x, pos.y, pos.z);
      glScalef(scale, scale, 1);
      glBegin(GL_QUADS);
        glTexCoord2f(xMin, yMin);  glVertex2f( 1,  1);
        glTexCoord2f(xMax, yMin);  glVertex2f(-1,  1);
        glTexCoord2f(xMax, yMax);  glVertex2f(-1, -1);
        glTexCoord2f(xMin, yMax);  glVertex2f( 1, -1);
      glEnd();
    glPopMatrix();
}

bool _explosion::isDone()
{
    return curFrame >= framesX*framesY;
}
