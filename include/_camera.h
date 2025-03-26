#ifndef _CAMERA_H
#define _CAMERA_H
#include<_common.h>
#include<_player.h>


class _camera
{
    public:
        _camera();
        virtual ~_camera();

        void followPlayer(_player*);
        void updateCamPos();

        vec3 camPos;
        vec3 targetPos;

    protected:

    private:
};

#endif // _CAMERA_H
