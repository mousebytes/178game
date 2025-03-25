#ifndef _MODEL_H
#define _MODEL_H

#include<_common.h>

class _model
{
    public:
        _model();
        virtual ~_model();

        vec3 rotation; // use for model rotation
        vec3 pos; // to place the model
        vec3 scale; // resize the model

        void drawModel();






    protected:

    private:
};



#endif // _MODEL_H
