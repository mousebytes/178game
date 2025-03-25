#include "_player.h"

_player::_player()
{
    //ctor
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
    plPos.x=0; plPos.y = -0.55; plPos.z = -2;

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

}
