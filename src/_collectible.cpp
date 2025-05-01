#include "_collectible.h"

_collectible::_collectible()
{
    //ctor
    isCollected = false;

}

_collectible::~_collectible()
{
    //dtor
}

void _collectible::initColl(const char* fileName, float x, float y, float z, float r, int xfrm, int yfrm)
{
    // initialize quad
    vert[0].x = -1.0; vert[0].y = -1.0; vert[0].z = -1.0;
    vert[1].x =  1.0; vert[1].y = -1.0; vert[1].z = -1.0;
    vert[2].x =  1.0; vert[2].y =  1.0; vert[2].z = -1.0;
    vert[3].x = -1.0; vert[3].y =  1.0; vert[3].z = -1.0;

    //get frame count
    framesX=xfrm;
    framesY=yfrm;

    xMin=0;
    xMax=1.0/(float)framesX;
    yMax=1.0/(float)framesY;
    yMin=yMax-(1/(float)framesY);

    tex->loadTexture(fileName);

    pos = {x,y,z};

    radius = r;
}

void _collectible::drawColl()
{
    if(isCollected) return;

    tex->textureBinder();

    glPushMatrix();
        glTranslatef(pos.x,pos.y,pos.z);
        glScalef(radius,radius,1.0);
        glColor3f(1.0,1.0,1.0);
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
