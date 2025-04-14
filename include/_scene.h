#ifndef _SCENE_
#define _SCENE_

#include<_common.h>

class _scene
{
    public:
        _scene();             //constructor
        virtual ~_scene();    //destructor

       GLint initGL();        //initialize game objects
       void reSize(GLint width,GLint Height); // resize window
       void  drawScene();     // Render the Final scene

       int winMsg(HWND	hWnd,			// Handle For This Window
                  UINT	uMsg,			// Message For This Window
                  WPARAM	wParam,			// Additional Message Information
                  LPARAM	lParam);

        void check_platform_collisions();
    vec2 dim;
    protected:

    private:
};

#endif // _SCENE_
