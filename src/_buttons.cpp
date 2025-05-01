#include "_buttons.h"

_buttons::_buttons()
{
    //ctor
}

_buttons::~_buttons()
{
    //dtor
}

void _buttons::initButton(const char* fileName, float xCoord, float yCoord, float zCoord, float xScl, float yScl, float zScl, int xFrm, int yFrm)
{
    vert[0].x = -1.0; vert[0].y = -1.0; vert[0].z = -1.0;
    vert[1].x =  1.0; vert[1].y = -1.0; vert[1].z = -1.0;
    vert[2].x =  1.0; vert[2].y =  1.0; vert[2].z = -1.0;
    vert[3].x = -1.0; vert[3].y =  1.0; vert[3].z = -1.0;


    pos.x=xCoord;
    pos.y = yCoord;
    pos.z = zCoord;

    scale.x = xScl;
    scale.y = yScl;
    scale.z = zScl;

    framesX=xFrm;
    framesY=yFrm;

    xMin=0;
    xMax=1.0/(float)framesX;
    yMax=1.0/(float)framesY;
    yMin=yMax-(1/(float)framesY);

    tex->loadTexture(fileName);
}

void _buttons::drawButton()
{
    glPushMatrix();
        glTranslatef(pos.x,pos.y,pos.z);
        glScalef(scale.x,scale.y,scale.z);
        glColor3f(1.0,1.0,1.0);

        tex->textureBinder();

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


bool _buttons::isHovered(float mouseX, float mouseY)
{
    return mouseX > pos.x - scale.x &&
           mouseX < pos.x + scale.x &&
           mouseY > pos.y - scale.y &&
           mouseY < pos.y + scale.y;
}

void _buttons::updateHover(float mouseX, float mouseY)
{
    if(isHovered(mouseX, mouseY))
    {
        // wsitch to second frame when hovered
        xMin = 1.0f / framesX;
        xMax = 2.0f / framesX;
    }
    else
    {
        // default to first frame
        xMin = 0.0f;
        xMax = 1.0f / framesX;
    }
}
