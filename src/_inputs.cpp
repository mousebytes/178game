#include "_inputs.h"

_inputs::_inputs()
{
    //ctor
    isMsRotation = false;
    isMsTranslate = false;
}

_inputs::~_inputs()
{
    //dtor
}

void _inputs::keyPressed(_model* mdl)
{
    switch(wParam)
    {
    case VK_LEFT:
        mdl->rotation.y +=1.0;
        break;

    case VK_RIGHT:
        mdl->rotation.y -=1.0;
        break;

    case VK_UP:
        mdl->rotation.x +=1.0;
        break;

    case VK_DOWN:
        mdl->rotation.x -=1.0;
        break;
    }
}

void _inputs::keyPressed(_player* player)
{
    switch(wParam)
    {
    case VK_LEFT:
        //player->plPos.x -= 0.05; //legacy
        player->applyVelocity(-1,0);
        break;
    case VK_RIGHT:
        //player->plPos.x += 0.05; //legacy
        player->applyVelocity(1,0);
        break;
    case VK_UP:
        //player->plPos.y += 0.05; //legacy
        player->applyVelocity(0,1);
        break;
    case VK_DOWN:
        //player->plPos.y -=0.05; //legacy
        player->applyVelocity(0,-1);
        break;

    }
}


void _inputs::keyUP(_player *player)
{
    switch(wParam)
    {
    case VK_LEFT:
        player->velocity.x = 0;
        cout << "\nLEFT RELEASED"; // debugging
        break;
    case VK_RIGHT:
        player->velocity.x = 0;
        break;
    case VK_UP:
        player->velocity.y = 0;
        break;
    case VK_DOWN:
        player->velocity.y = 0;
        break;
        //default: break;
    }

}

void _inputs::mouseEventDown(_model* mdl, double x, double y)
{
    prev_msX = x;
    prev_msY = y;

        switch(wParam)
        {
        case MK_LBUTTON:
            isMsRotation = true;
            break;

        case MK_RBUTTON:
            isMsTranslate = true;
            break;
         case MK_MBUTTON: break;

         default: break;
        }
}

void _inputs::mouseEventUp()
{
   isMsRotation = false;
   isMsTranslate = false;
}

void _inputs::mouseMove(_model* mdl, double x, double y)
{
       if(isMsRotation){
            mdl->rotation.y += (x-prev_msX)/3.0;   //increase x alngle
            mdl->rotation.x += (y-prev_msY)/3.0;   //increase y angle
       }

       if(isMsTranslate){
            mdl->pos.x +=(x-prev_msX)/100.0;      //increase x position
            mdl->pos.y -=(y-prev_msY)/100.0;      //decrease y position
       }
       prev_msX =x;
       prev_msY =y;

}

void _inputs::mouseWheel(_model* mdl, double delta)
{
    mdl->pos.z +=delta/100;
}
