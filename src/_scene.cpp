#include "_scene.h"
#include<_lightsetting.h>
#include<_model.h>
#include<_inputs.h>
#include<_player.h>
#include<_camera.h>
#include<_background.h>
#include<_collisionCheck.h>


_lightSetting *myLight = new _lightSetting();
_model *myModel = new _model();    // creating instance for model
_inputs *input = new _inputs();
_player *player = new _player();
_camera *camera = new _camera();
_background *background = new _background();
_collisionCheck *collision = new _collisionCheck();

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

   glEnable(GL_BLEND); // for transparent bg
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   //glDisable(GL_LIGHTING);

  // glEnable(GL_COLOR_MATERIAL);
   myLight->setLight(GL_LIGHT0);

    player->initPlayer(1,1,"images/wall.png");


    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    background->initBG("images/marce.png");

   return true;
}

void _scene::reSize(GLint width, GLint height)
{
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
    // keep track of the resize window
    glViewport(0,0,width,height); // adjusting the viewport
    glMatrixMode(GL_PROJECTION);  // perspective projection settings
    glLoadIdentity();             // identity matrix
    gluPerspective(45,aspectRatio,0.1,100.0); // projection settings
    glMatrixMode(GL_MODELVIEW);   // camera and model settings
    glLoadIdentity();             // identity matrix

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);
}

void _scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //gluLookAt(0,0,1,0,0,0,0,1,0);

    // don't change the order of these 4 functions -- player texture breaks otherwise
    player->playerActions();
    camera->followPlayer(player);
    camera->updateCamPos();
    player->drawPlayer();

    player->handle_vertical();

    background->drawBackground(dim.x,dim.y);

    cout << "\nLEFT AND RIGHT: " << input->key_left_down << " " << input->key_right_down;
}

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_KEYDOWN:
           input->wParam = wParam;
           input->keyPressed(player);
        break;


    case WM_KEYUP:
        input->wParam = wParam;
        input->keyUP(player);
        break;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        input->wParam = wParam;
        input->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        input->mouseEventUp();
        break;
    case WM_MOUSEMOVE:
        input->mouseMove(myModel,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_MOUSEWHEEL:
        input->mouseWheel(myModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    }
}
