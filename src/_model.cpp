#include "_model.h"

_model::_model()
{
    //ctor

    rotation.x = rotation.y = rotation.z = 0.0;
    pos.x = pos.y = 0.0;
    pos.z =-8.0;
    scale.x=scale.y=scale.z =1.0;
}

_model::~_model()
{
    //dtor
}
void _model::drawModel()
{
    glColor3f(1.0,0.6,0.6);

    glPushMatrix();
    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(rotation.x,1,0,0);
    glRotatef(rotation.y,0,1,0);
    glRotatef(rotation.z,0,0,1);

    glScalef(scale.x,scale.y,scale.z);
    glutSolidTorus(0.5,1.5,40,40); //TODO: subject to be changed by the user
    glPopMatrix();


}
