#include "_scene.h"
#include<_lightSetting.h>
#include<_model.h>

_lightSetting *mylight = new _lightSetting();
_model *myModel = new _model(); // creating instance for model

_scene::_scene()
{
    //ctor
}

_scene::~_scene()
{
    //dtor
}

GLint _scene::initGL()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    mylight->setLight(GL_LIGHT0);

    return true;

}

GLvoid _scene::reSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
    // keep track of the resize wwindow
    glViewport(0,0,width,height); // adjusting the viewport
    glMatrixMode(GL_PROJECTION);// perspective projection settings
    glLoadIdentity(); // identity matrix
    gluPerspective(45,aspectRatio,0.1,100.0); // projection settings
    glMatrixMode(GL_MODELVIEW); // camera and model settings
    glLoadIdentity(); // identity matrix

}
void _scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear bits in each iteration
    glLoadIdentity();

    glPushMatrix();

    glColor3f(1.0,0,0);
    glTranslatef(0.0,0.0,-8.0);
    glRotatef(30,0,1,0);
    glutSolidTeapot(1.5);

    glPopMatrix();

    glPushMatrix();

    glColor3f(0.0,1,0);
    glTranslatef(8.0,0.0,-15.0);
    glRotatef(30,1,0,0);
    glutSolidCube(2.0);

    glPopMatrix();
}
