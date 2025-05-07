#include "_inputs.h"

_inputs::_inputs()
{
    //ctor
    isMsRotation = false;
    isMsTranslate = false;
    key_left_down = key_right_down = false;
}

_inputs::~_inputs()
{
    //dtor
}

void _inputs::keyPressed(_player* player)
{



    switch(wParam)
    {
    case VK_LEFT:
    case 'A':
        temp_wParam = wParam;
        key_left_down = true;
        player->action_trigger = player->WALKLEFT;

        break;
    case VK_RIGHT:
    case 'D':
        temp_wParam = wParam;
        key_right_down = true;
        player->action_trigger = player->WALKRIGHT;

        break;
    case VK_UP:
    case 'W':
        //player->plPos.y += 0.05;
        break;
    case VK_DOWN:
    case 'S':
        //player->plPos.y -=0.05;
        break;
    case VK_SPACE:
        if(!player->isJumping && player->is_grounded)
        {
            player->isJumping = true;
            player->is_grounded = false;
            player->height_before_jump = player->plPos.y;
        }
        break;
    }




}
void _inputs::keyUP(_player *player)
{

        switch(wParam)
        {
        case VK_LEFT:
        case 'A':
            key_left_down = false;
            if(!key_right_down)
                player->action_trigger = player->STANDING;
            break;
        case VK_RIGHT:
        case 'D':
            key_right_down = false;
            if(!key_left_down)
                player->action_trigger = player->STANDING;
            break;

        case VK_SPACE:
            wParam = temp_wParam;
            if(!player->isBeingDisplacedHorz && player->justBounced->getTicks() > 1000)
                player->isJumping = false;
            break;

    }


}

void _inputs::mouseEventUp()
{
   isMsRotation = false;
   isMsTranslate = false;
}


void _inputs::keyPressedPRLX(_parallax* PLRX)
{
        if(key_left_down)
            PLRX->state = PLRX->LEFT;
        else if(key_right_down)
            PLRX->state = PLRX->RIGHT;
        else
            PLRX->state = PLRX->STATIC;

}


