#include "_model.h"

_model::_model()
{
    //ctor

    rotation.x = rotation.y = rotation.z= 0.0; // init rotations
    pos.x = pos.y=0.0;                         // init placement
    pos.z = -8.0;
    scale.x=scale.y=scale.z = 1.0;
}

_model::~_model()
{
    //dtor
}
void _model::initModel(char* fileName)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    mytex->loadTexture(fileName);
}


void _model::drawModel()
{
    mytex->textureBinder();
    glPushMatrix();

 //   glColor3f(1.0,0.6,0.6);
    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(rotation.x, 1,0,0);
    glRotatef(rotation.y, 0,1,0);
    glRotatef(rotation.z, 0,0,1);

    glScalef(scale.x,scale.y,scale.z);
    //glutSolidTorus(0.5,1.5,40,40);   //ToDo: subject to be change by the user
    glutSolidTeapot(1.5);
  //   glutSolidCube(2.0);
 //   glutSolidTeapot(2.5);
    glPopMatrix();

}
