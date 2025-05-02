#include "_hud.h"

_hud::_hud()
{
    //ctor
}

_hud::~_hud()
{
    //dtor
}

void _hud::initHud(char* heartFile, int xfrm, int yfrm, vec3 camPos)
{
    vert[0].x = -1.0; vert[0].y = -1.0; vert[0].z = -1.0;
    vert[1].x =  1.0; vert[1].y = -1.0; vert[1].z = -1.0;
    vert[2].x =  1.0; vert[2].y =  1.0; vert[2].z = -1.0;
    vert[3].x = -1.0; vert[3].y =  1.0; vert[3].z = -1.0;


    pos = camPos;

    scl.x=scl.y=scl.z=0.05;

    //get frame count
    framesX=xfrm;
    framesY=yfrm;

    xMin=0;
    xMax=1.0/(float)framesX;
    yMax=1.0/(float)framesY;
    yMin=yMax-(1/(float)framesY);

    heartTex->loadTexture(heartFile);

    bananaCount->initFonts("images/fontsheet.png",15,8);
    bananaCount->setPosition(6,3.5,-2);
    bananaCount->setSize(0.1,0.1);

    bananaIcon->initButton("images/banana.png",0,3.05,-2,0.1,0.1,1.0,1,1);
}

void _hud::drawHearts(int health, vec3 camPos)
{


        heartTex->textureBinder();

        for(int i = 0;i<health;++i)
        {
            float offsetX = i * 0.1f;
            pos.x = camPos.x - 1.8f + offsetX;
            pos.y = camPos.y + 0.9f;
            pos.z = camPos.z - 2.5f;

            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glScalef(scl.x, scl.y, scl.z);

            glBegin(GL_QUADS);
                glTexCoord2f(xMin, yMax); glVertex3f(vert[0].x, vert[0].y, vert[0].z);
                glTexCoord2f(xMax, yMax); glVertex3f(vert[1].x, vert[1].y, vert[1].z);
                glTexCoord2f(xMax, yMin); glVertex3f(vert[2].x, vert[2].y, vert[2].z);
                glTexCoord2f(xMin, yMin); glVertex3f(vert[3].x, vert[3].y, vert[3].z);
            glEnd();

            glPopMatrix();

        }


}

void _hud::updateBananaCount(int bananas, _camera* cam)
{
    bananaCount->setPosition(cam->camPos.x + 6,3, bananaCount->pos.z);

    glDisable(GL_DEPTH_TEST);
    bananaCount->drawText("x" + to_string(bananas));
    glEnable(GL_DEPTH_TEST);
}

void _hud::drawBananaIcon(_camera* cam)
{
    bananaIcon->pos.x = cam->camPos.x + 5.8;
    glDisable(GL_DEPTH_TEST);
    bananaIcon->drawButton();
    glEnable(GL_DEPTH_TEST);


}

