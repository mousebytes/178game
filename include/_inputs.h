#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_player.h>

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_player *);
        void keyUP(_player*);

        void mouseEventUp();

        double prev_msX;
        double prev_msY;

        bool isMsTranslate;
        bool isMsRotation;
        bool key_left_down, key_right_down;

        WPARAM wParam;
        WPARAM temp_wParam;

    protected:

    private:
};

#endif // _INPUTS_H
