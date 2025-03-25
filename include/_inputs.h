#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_model *); // handling given model by key strokes
        void keyUP();

        void mouseEventDown(_model *, double, double);
        void mouseEventUp();
        void mouseMove(_model*, double,double);
        void mouseWheel(_model *, double);

        double prev_msX;
        double prev_msY;

        bool isMsTranslate;
        bool isMsRotation;

        WPARAM wParam;

    protected:

    private:
};

#endif // _INPUTS_H
