#include "_camera.h"

_camera::_camera()
{
    //ctor
    camPos.x = 0;
    camPos.y = 0;
    camPos.z = 4;

    targetPos.x = 0;
    targetPos.y = 0;
    targetPos.z = 4;
}

_camera::~_camera()
{
    //dtor
}


void _camera::followPlayer(_player* player)
{
    float lerp = 0.05; // camera smoothing :) -- closer to 1 means follows more quickly

    camPos.x += (player->plPos.x - camPos.x)*lerp;
    camPos.y += (player->plPos.y - camPos.y)*lerp;
    camPos.z = 4.0;

    targetPos = player->plPos;
    targetPos.z = -2.0;
}

void _camera::updateCamPos()
{
    gluLookAt(camPos.x, camPos.y, camPos.z,
              targetPos.x, targetPos.y, targetPos.z,
              0,1,0);
}


