#include "_barrelCannon.h"

_barrelCannon::_barrelCannon()
{
    //ctor
    tex = new _textureLoader();
    fireTimer = new _timer();
    playerInside = false;
    isAuto = false;
    fireDelay = 1.0;

    framesX=framesY = 1;
}

_barrelCannon::~_barrelCannon()
{
    //dtor
}

void _barrelCannon::initBarrel(const char* fileName, vec3 p, float rotDeg, bool autoF, float delay)
{
    vert[0].x = -1.0; vert[0].y = -1.0; vert[0].z = -1.0;
    vert[1].x =  1.0; vert[1].y = -1.0; vert[1].z = -1.0;
    vert[2].x =  1.0; vert[2].y =  1.0; vert[2].z = -1.0;
    vert[3].x = -1.0; vert[3].y =  1.0; vert[3].z = -1.0;

    pos = p;
    scale = {0.6,0.6};
    rotation = rotDeg;
    isAuto = autoF;
    fireDelay = delay;
    tex->loadTexture(fileName);
    fireTimer->reset();

    xMin=0;
    xMax=1.0/(float)framesX;
    yMax=1.0/(float)framesY;
    yMin=yMax-(1/(float)framesY);

    tex->loadTexture(fileName);
}

void _barrelCannon::drawBarrel()
{
    glPushMatrix();

    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(rotation,0,0,1);
    glScalef(scale.x,scale.y,1.0);
    tex->textureBinder();

    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(xMin, yMax); glVertex3f(vert[0].x, vert[0].y, vert[0].z);
        glTexCoord2f(xMax, yMax); glVertex3f(vert[1].x, vert[1].y, vert[1].z);
        glTexCoord2f(xMax, yMin); glVertex3f(vert[2].x, vert[2].y, vert[2].z);
        glTexCoord2f(xMin, yMin); glVertex3f(vert[3].x, vert[3].y, vert[3].z);
    glEnd();

    glPopMatrix();


}

bool _barrelCannon::isPlayerInside(vec3 pPos, vec2 pScl)
{
    return pPos.x + pScl.x > pos.x - scale.x &&
            pPos.x - pScl.x < pos.x + scale.x &&
            pPos.y + pScl.y > pos.y - scale.y &&
            pPos.y - pScl.y < pos.y + scale.y;
}

void _barrelCannon::updateB(vec3& pPos, vec3& playerVel, bool& isJumping)
{
    if(!playerInside) return;

    if(isAuto && fireTimer->getTicks() > fireDelay * 1000)
    {
        float radians = rotation * 3.14159 / 180.0;

        playerVel.x = cos(radians) * 0.4;
        playerVel.y = sin(radians)*0.4;
        isJumping = true;
        playerInside = false;
    }

    if (!isAuto && GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        float radians = rotation * 3.14159f / 180.0f;
        playerVel.x = cos(radians) * 0.4f;
        playerVel.y = sin(radians) * 0.4f;
        isJumping = true;
        playerInside = false;
    }
}
