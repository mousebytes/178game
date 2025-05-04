#include "_scene.h"
#include<_lightsetting.h>
#include<_inputs.h>
#include<_player.h>
#include<_camera.h>
#include<_background.h>
#include<_collisionCheck.h>
#include<_platform.h>
#include<_enemies.h>
#include<_hud.h>
#include<_goal.h>
#include<_barrelCannon.h>
#include<_parallax.h>
#include<_buttons.h>
#include <_sounds.h>
#include<_fonts.h>

char *playerTex = "images/Mk.png";
//int player xfrm = 8;
//int player yfrm = 9;

_lightSetting *myLight = new _lightSetting();
_inputs *input = new _inputs();
_player *player = new _player();
_sounds *snds = new _sounds();
_sounds *sndOof = new _sounds();



_camera *camera = new _camera();
//_background *background = new _background();
_collisionCheck *collision = new _collisionCheck();
_hud *hud = new _hud();
_goal *goal = new _goal();

_parallax *p1 = new _parallax();
_parallax *p2 = new _parallax();
_parallax *p3 = new _parallax();
_parallax *background = new _parallax();
_parallax *deathScreen = new _parallax();
_parallax *winBG = new _parallax();
_parallax *startScreenBG = new _parallax();
_parallax *creditsScreenBG = new _parallax();
_parallax *helpScreenBG = new _parallax();


_platform* previewPlat = nullptr;
_enemies* previewEnemy = nullptr;
_collectible* previewCoin = nullptr;
_goal* previewGoal = nullptr;
_barrelCannon* previewBarrel = nullptr;

vector<_platform*> platforms;
vector<_enemies*> enemies;
vector<_collectible*> collectibles;
vector<_barrelCannon*> barrels;

vector<_buttons*> inventoryButtons(7);
vector<_buttons*> platTextureButtons(2);
vector<_buttons*> platAttributeButtons(2);

bool playerWon = false;
int currLevel = 1;

_buttons* startButton = new _buttons();
_buttons *editorButton = new _buttons();
_buttons *exitButton = new _buttons();
_buttons *loadButton = new _buttons();

_buttons *resumeButton = new _buttons();
_buttons *backToMenuButton = new _buttons();
_buttons *startScreenButton = new _buttons();
_buttons *jungleAdventureSS = new _buttons();
_buttons *menuCreditsButton = new _buttons();
_buttons *menuHelpButton = new _buttons();
_buttons *creditsBackButton = new _buttons();
_buttons *helpBackButton = new _buttons();
_buttons *loadSaveButton = new _buttons();
_buttons *loadCustomButton = new _buttons();

_fonts *platAttributeMoving =new _fonts();
_fonts *platAttributeStatic = new _fonts();

_scene::_scene()
{
    //ctor
    SceneDone = false;
    isPaused = false;

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

   //glEnable(GL_LIGHTING);
   //glEnable(GL_LIGHT0);
   glDisable(GL_LIGHTING);

  // glEnable(GL_COLOR_MATERIAL);
   myLight->setLight(GL_LIGHT0);

    player->initPlayer(playerTex);


    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    background->initPrlx("images/new_images/New_Bg.png");
    deathScreen->initPrlx("images/new_images/Game_Over.png");
    startScreenBG->initPrlx("images/new_images/New_Bg.png");
    winBG->initPrlx("images/new_images/You_Win.png");
    creditsScreenBG->initPrlx("images/new_images/Credits_Menu.png");
    helpScreenBG->initPrlx("images/new_images/help menu.png");
    snds->initSounds();
    snds->playMusic("JgMus.mp3");


    load_level_file("levels/level1.txt");

    hud->initHud("images/heart.png",1,1,camera->camPos);

    p1->initPrlx("images/level backgrounds/3.png");
    p1->speed = 0.001;
    p2->initPrlx("images/level backgrounds/2.png");
    p3->initPrlx("images/level backgrounds/1.png");
    p1->speed = 0.005;
    p2->speed = 0.002;
    p3->speed = 0.001;

    initMenuButtons();
    initEditorInventory();

    platAttributeStatic->initFonts("images/fontsheet.png",15,8);
    platAttributeMoving->initFonts("images/fontsheet.png",15,8);
    platAttributeStatic->pos = {-6.2,-3,-2};
    platAttributeMoving->pos = {-6.2,-2,-2};

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


    switch(gs)
    {
    case STARTSCREEN:
        drawstartScreen();
        break;
    case MAINMENU:
        drawMenu();
        break;
    case PLAYING:
        runGame();
        break;
    case GAMEOVER:
        drawGameOver();
        break;
    case LEVELEDITOR:
        drawEditor();
        break;
    case WIN:
        drawWinScreen();
        break;
    case CREDITS:
        drawCreditsScreen();
        break;
    case HELP:
        drawHelpScreen();
        break;
    case SAVESCREEN:
        drawSaveScreen();
        break;
    }






}

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_KEYDOWN:
           input->wParam = wParam;

           if((gs == PLAYING || gs == LEVELEDITOR || gs == SAVESCREEN) && wParam == VK_ESCAPE)
           {
                isPaused = !isPaused;
           }
           if((gs == WIN || gs == CREDITS || gs == HELP)&& wParam == VK_ESCAPE)
                gs = MAINMENU;
        if(!isPaused)
        {


           if(gs == MAINMENU && wParam == VK_RETURN)
           {
                //background->initPrlx("images/marce.png");
                currLevel = 1;
                load_level_file("levels/level1.txt");
                player->initPlayer(playerTex);
                gs = PLAYING;
                playerWon= false;
           }
            else if (gs == GAMEOVER && wParam == VK_ESCAPE)
           {
               //background->initPrlx("images/temp_mainmenu.png");
               gs = MAINMENU;

               player->initPlayer(playerTex);
               load_level_file("levels/level1.txt");
               currLevel = 1;
               playerWon = false;

               player->health = 3;
               player->coins = 0;
           }
            else if(gs == PLAYING && wParam == 'K') // press K to save
            {
                saveGame();
            }
            else if(gs == MAINMENU && wParam == 'L') // Press L to load
                {
                    //background->initPrlx("images/marce.png");
                    loadGame();
                    gs = PLAYING;
                }
            else if(gs == PLAYING)
           {
                input->keyPressed(player);
                input->keyPressedPRLX(p1);
                input->keyPressedPRLX(p2);
                input->keyPressedPRLX(p3);

           //   input->keyPressed(player_can_be_damaged);
           }
           else if(gs==LEVELEDITOR)
           {
               switch(wParam)
               {
                   //case 'M': if(placeObj == PLAT) if(previewPlat->chooseTex == previewPlat->DIRT) previewPlat->chooseTex = previewPlat->STONE; else previewPlat->chooseTex = previewPlat->DIRT; break;
                   case '1': placeObj = PLAT; break;
                   case '2': placeObj = ENEMY; if(!previewEnemy) previewEnemy = new _enemies(); previewEnemy->eT = previewEnemy->WALKER; break;
                   case '3': placeObj = COLLECTIBLE; break;
                   case '4': placeObj = GOAL; break;
                   case '5': placeObj = ENEMY; if(!previewEnemy) previewEnemy = new _enemies(); previewEnemy->eT = previewEnemy->WALKER; break;
                   case '6': placeObj = ENEMY; if(!previewEnemy) previewEnemy = new _enemies(); previewEnemy->eT = previewEnemy->JUMPER; break;
                   case '7': placeObj = ENEMY; if(!previewEnemy) previewEnemy = new _enemies(); previewEnemy->eT = previewEnemy->FLYER; break;
                   case '8': placeObj = BARREL; if(!previewBarrel) previewBarrel = new _barrelCannon(); previewBarrel->initBarrel("images/barrel.png",{mouseX,mouseY},90,true,1); break;
                   //case 'Q': gs = MAINMENU; /*background->initPrlx("images/temp_mainmenu.png");*/ break;
                   case 'S': saveCustomLevel();break;
                   case 'A': player->plPos.x -=0.5; for(int i = 0; i < inventoryButtons.size(); i++) inventoryButtons[i]->pos.x -=0.5; for(int i = 0;i<platTextureButtons.size();++i) platTextureButtons[i]->pos.x-=0.5; for(int i = 0;i<platAttributeButtons.size();++i) platAttributeButtons[i]->pos.x-=0.5; platAttributeMoving->pos.x -= 0.5; platAttributeStatic->pos.x -= 0.5;break;
                   case 'D': player->plPos.x +=0.5; for(int i = 0; i < inventoryButtons.size(); i++) inventoryButtons[i]->pos.x +=0.5; for(int  i= 0;i<platTextureButtons.size();++i) platTextureButtons[i]->pos.x += 0.5; for(int  i= 0;i<platAttributeButtons.size();++i) platAttributeButtons[i]->pos.x += 0.5; platAttributeMoving->pos.x += 0.5; platAttributeStatic->pos.x += 0.5;break;
                   case VK_LEFT:  // Decrease platform X scale
                    if (previewPlat && placeObj == PLAT)
                        previewPlat->scale.x = max(0.1f, previewPlat->scale.x - 0.1f);
                    break;
                    case 'R':
                    if (previewBarrel && placeObj == BARREL)
                        previewBarrel->rotation -= 5;
                    break;

                    case 'E':
                    if (previewBarrel && placeObj == BARREL)
                        previewBarrel->rotation += 5;
                    break;

                    case VK_RIGHT: // Increase platform X scale
                    if (previewPlat && placeObj == PLAT)
                        previewPlat->scale.x += 0.1f;
                    break;
                    case VK_DOWN: // Decrease Y
                    if (previewPlat && placeObj == PLAT)
                        previewPlat->scale.y = max(0.1, previewPlat->scale.y - 0.1);
                    break;
                    case VK_UP:   // Increase Y
                    if (previewPlat && placeObj == PLAT)
                        previewPlat->scale.y += 0.1;
                        break;

                    case VK_OEM_4: // '[' — shrink enemy
                    if (previewEnemy && placeObj == ENEMY)
                    previewEnemy->scale.x = previewEnemy->scale.y = max(0.05, previewEnemy->scale.x - 0.05);

                    if(previewPlat && placeObj == PLAT)
                    {
                        previewPlat->scale.x = max(0.1, previewPlat->scale.y - 0.1);
                        previewPlat->scale.y = max(0.1, previewPlat->scale.y - 0.1);
                    }
                    break;
                    case VK_OEM_6: // ']' — enlarge enemy
                    if (previewEnemy && placeObj == ENEMY)
                    previewEnemy->scale.x = previewEnemy->scale.y += 0.05;

                    if(previewPlat && placeObj == PLAT)
                    {
                        previewPlat->scale.x += 0.1;
                        previewPlat->scale.y +=0.1;
                    }
                    break;
                    case VK_BACK:
                        if (placeObj == PLAT && previewPlat)
                        {
                            previewPlat->scale.x = 1.0;
                            previewPlat->scale.y = 1.0;
                        }
                        else if (placeObj == ENEMY && previewEnemy)
                        {
                            previewEnemy->scale.x = previewEnemy->scale.y = 0.25f;
                        }
                    break;
               }
           }
           else if(gs == MAINMENU && wParam == 'E')
           {
               //background->initPrlx("images/marce.png");
               platforms.clear();
               enemies.clear();
               collectibles.clear();
               player->initPlayer(playerTex);
               load_level_file("levels/custom_level.txt");
               gs = LEVELEDITOR;
           }
           else if(gs == MAINMENU && wParam == 'C')
            {
                //background->initPrlx("images/marce.png");
                load_level_file("levels/custom_level.txt");
                player->initPlayer(playerTex);
                player->plPos = {0, 0, -3};
                player->health = 3;
                player->coins = 0;
                playerWon = false;
                gs = PLAYING;
                currLevel = 0;
                cout << "Loaded custom_level.txt and started game" << endl;
            }
        }
        break;


    case WM_KEYUP:
        input->wParam = wParam;
        input->keyUP(player);
        input->keyPressedPRLX(p1);
        input->keyPressedPRLX(p2);
        input->keyPressedPRLX(p3);
        break;

    case WM_LBUTTONDOWN:

        if(isPaused)
        {
        mouseMapping(LOWORD(lParam), HIWORD(lParam));

        float adjustedMouseX = mouseX - camera->camPos.x;
        float adjustedMouseY = mouseY - camera->camPos.y;

        if(resumeButton->isHovered(adjustedMouseX, adjustedMouseY))
        {
            isPaused = false;
        }
        else if(backToMenuButton->isHovered(adjustedMouseX, adjustedMouseY))
        {
            isPaused = false;
            gs = MAINMENU;
            return 0;
        }

        }
        if(gs == SAVESCREEN)
        {
            if(loadSaveButton->isHovered(mouseX,mouseY))
            {
                gs = PLAYING;
                loadGame();
            }

            if(loadCustomButton->isHovered(mouseX,mouseY))
            {
                load_level_file("levels/custom_level.txt");
                player->initPlayer(playerTex);
                player->plPos = {0, 0, -3};
                player->health = 3;
                player->coins = 0;
                playerWon = false;
                gs = PLAYING;
                currLevel = 0;
                cout << "Loaded custom_level.txt and started game" << endl;
            }

        }

        if(gs == STARTSCREEN)
        {
            if( startScreenButton->isHovered(mouseX,mouseY))
            {
                gs = MAINMENU;
                return 0;
            }

        }
        if(gs == CREDITS || gs == SAVESCREEN)
            if(creditsBackButton->isHovered(mouseX,mouseY))
            {
                gs = MAINMENU;
                return 0;
            }

        if(gs == HELP)
            if(helpBackButton->isHovered(mouseX,mouseY))
            {
                gs = MAINMENU;
                return 0;
            }


        if(!isPaused)
        {


        if(gs==LEVELEDITOR)
        {
            // send mouse coords when in level editor
            mouseMapping(LOWORD(lParam),HIWORD(lParam));

            if(placeObj == PLAT)
            {
                _platform *plat = new _platform();
                //plat->initPlat("images/wall.png",mouseX,mouseY,-3.0,1,1,1,1,1,1,0,0);
                plat->initPlat("images/wall.png", mouseX, mouseY, -2.0,
                    previewPlat ? previewPlat->scale.x : 1.0f,
                    previewPlat ? previewPlat->scale.y : 1.0f,
               1, 1, 1, int(previewPlat->type), 0.01, 1.5, int(previewPlat->chooseTex));
                platforms.push_back(plat);
            }
            else if(placeObj == ENEMY)
            {
                _enemies* enemy = new _enemies();
                //enemy->initEnms("images/wall.png");
                enemy->eT = previewEnemy->eT;
                enemy->initEnms();
                //enemy->placeEnms({mouseX,mouseY,-3},0.25);
                enemy->placeEnms({mouseX,mouseY,-3}, previewEnemy ? previewEnemy->scale.x : 0.25f);
                enemy->isEnmsLive = true;
                enemy->action_trigger = enemy->WALKLEFT;
                enemies.push_back(enemy);
               // if(enemies->Dead){
              //  input->keyPressed(snds,"explosion.mp3");
              //  }
            }
            else if (placeObj == COLLECTIBLE)
            {
                _collectible *c = new _collectible();
                c->initColl("images/banana.png",mouseX,mouseY,-2,0.1,1,1);
                collectibles.push_back(c);
            }
            else if(placeObj == GOAL)
            {
                if(goal) delete goal;
                goal = new _goal();
                goal->initGoal("images/goal.png",mouseX,mouseY,-2,0.2,1,1);
            }
            else if(placeObj == BARREL)
            {
                _barrelCannon* b = new _barrelCannon();
                b->initBarrel("images/barrel.png",{mouseX,mouseY,-2},previewBarrel->rotation, false, previewBarrel->fireDelay);
                barrels.push_back(b);
            }
        }
        }
        if(gs == MAINMENU)
        {

        //mouseMapping(LOWORD(lParam), HIWORD(lParam));

        if(startButton->isHovered(mouseX, mouseY))
        {
            //background->initPrlx("images/marce.png");
            currLevel = 1;
            load_level_file("levels/level1.txt");
            player->initPlayer(playerTex);
            gs = PLAYING;
            playerWon = false;
            return 0;
        }
        else if(loadButton->isHovered(mouseX, mouseY))
        {
            //background->initPrlx("images/marce.png");
            //loadGame();
            gs = SAVESCREEN;
            return 0;
        }
        else if(editorButton->isHovered(mouseX, mouseY))
        {
            //background->initPrlx("images/marce.png");
            platforms.clear();
            enemies.clear();
            collectibles.clear();
            player->initPlayer(playerTex);
            load_level_file("levels/custom_level.txt");
            gs = LEVELEDITOR;

            /*
            for(int i =0;i<2;++i)
            {
                platAttributeButtons[i]->pos.x = -5.5;
            }*/

            initEditorInventory();


            return 0;
        }
        else if(exitButton->isHovered(mouseX, mouseY))
        {
            SceneDone = true;  // exit game
        }

        else if(menuCreditsButton->isHovered(mouseX,mouseY))
        {
            gs = CREDITS;
            return 0;
        }
        else if(menuHelpButton->isHovered(mouseX,mouseY))
        {
            gs = HELP;
            return 0;
        }

        }



        break;
    case WM_RBUTTONDOWN:
        input->wParam = wParam;
        if (gs == LEVELEDITOR)
            {
                mouseMapping(LOWORD(lParam), HIWORD(lParam));
                deleteObjectAtMouseInEditor();
            }
            if(gs == LEVELEDITOR)
                {
                for(int i = 0; i < 7; i++)
                {
                    if(inventoryButtons[i]->isHovered(mouseX, mouseY))
                    {
                    switch(i)
                        {
                        case 0: placeObj = PLAT; break;
                        case 1: placeObj = ENEMY; previewEnemy = new _enemies(); previewEnemy->eT = previewEnemy->WALKER; break;
                        case 2: placeObj = ENEMY; previewEnemy = new _enemies(); previewEnemy->eT = previewEnemy->JUMPER; break;
                        case 3: placeObj = ENEMY; previewEnemy = new _enemies(); previewEnemy->eT = previewEnemy->FLYER; break;
                        case 4: placeObj = COLLECTIBLE; break;
                        case 5: placeObj = GOAL; break;
                        case 6: placeObj = BARREL; break;
                        }
            return 0;
                    }

                }
                for(int i =0;i<2;i++)
                {

                    if(platTextureButtons[i]->isHovered(mouseX,mouseY))
                    {
                        switch(i)
                        {
                            case 0: previewPlat->chooseTex = previewPlat->STONE; break;
                            case 1: previewPlat->chooseTex=previewPlat->DIRT; break;
                        }
                    }
                    if(platAttributeButtons[i]->isHovered(mouseX,mouseY))
                    {
                        previewPlat->type = i==1 ? previewPlat->STATIC : previewPlat->HORIZONTAL;
                    }
                }
                if(previewPlat)
                    if(previewPlat->type != previewPlat->STATIC)
                    {
                        platAttributeButtons[0]->xMin = 1.0f / platAttributeButtons[0]->framesX;
                        platAttributeButtons[0]->xMax = 2.0f / platAttributeButtons[0]->framesX;

                        platAttributeButtons[1]->xMin = 0 / platAttributeButtons[1]->framesX;
                        platAttributeButtons[1]->xMax = 1.0 / platAttributeButtons[1]->framesX;
                    }
                else
                {
                    platAttributeButtons[0]->xMin = 0 / platAttributeButtons[0]->framesX;
                    platAttributeButtons[0]->xMax = 1.0 / platAttributeButtons[0]->framesX;

                    platAttributeButtons[1]->xMin = 1.0f / platAttributeButtons[1]->framesX;
                    platAttributeButtons[1]->xMax = 2.0f / platAttributeButtons[1]->framesX;
                }


                return 0;
                }
    break;
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        input->mouseEventUp();
        break;
    case WM_MOUSEMOVE:

        //todo: check this
        if(gs == LEVELEDITOR)
    {
        mouseMapping(LOWORD(lParam), HIWORD(lParam));
        switch(placeObj)
        {
            case PLAT:
                {


                //if (!previewPlat) previewPlat = new _platform();
                //previewPlat->initPlat("images/wall.png", mouseX, mouseY, -2.0, 1, 1, 1, 1, 1, 1, 0, 0);
                if (!previewPlat)
                    previewPlat = new _platform();

                float scaleX = previewPlat->scale.x;
                float scaleY = previewPlat->scale.y;

                previewPlat->initPlat("images/dirt_plat.png", mouseX, mouseY, -2.0,scaleX, scaleY, 1,1, 1, previewPlat->type, 0, 0,previewPlat->chooseTex);
                break;
                }
            case ENEMY:
                {


                //if (!previewEnemy) previewEnemy = new _enemies();
                //previewEnemy->placeEnms({mouseX, mouseY, -3}, 0.25);
                //previewEnemy->initEnms("images/wall.png");
                if(!previewEnemy)
                    previewEnemy= new _enemies();
                float scale = previewEnemy->scale.x;
                previewEnemy->placeEnms({mouseX,mouseY,-2},scale);
                //previewEnemy->initEnms("images/wall.png");
                previewEnemy->initEnms();
                break;
                }
            case COLLECTIBLE:
                if (!previewCoin) previewCoin = new _collectible();
                previewCoin->initColl("images/banana.png", mouseX, mouseY, -2, 0.1, 1, 1);
                break;
            case GOAL:
                if(!previewGoal) previewGoal = new _goal();
                previewGoal->initGoal("images/goal.png",mouseX,mouseY,-2,0.2,1,1);
                break;
            case BARREL:
                if(!previewBarrel)
                    previewBarrel = new _barrelCannon();
                previewBarrel->initBarrel("images/barrel.png",{mouseX,mouseY,-2},previewBarrel->rotation, previewBarrel->isAuto, previewBarrel->fireDelay);
                break;
        }
    }
        if(gs == MAINMENU || gs == STARTSCREEN || gs == CREDITS || gs == HELP || gs == SAVESCREEN || (gs == PLAYING && isPaused))
            mouseMapping(LOWORD(lParam), HIWORD(lParam));

    break;
        break;
    case WM_MOUSEWHEEL:
        break;
    }
}

void _scene::check_platform_collisions()
{
    bool on_any_plat = false;

    for(auto p : platforms)
    {
        if(collision->isPlayerOnGround(player,p)) on_any_plat = true;
    }
    player->is_grounded=on_any_plat;



    for(auto e : enemies)
    {
        bool enemy_on_ground = false;
        for(auto p : platforms)
        {
            if(collision->isEnemyOnGround(e,p))
            {
                enemy_on_ground = true;
                break;
            }
        }
        e->is_grounded = enemy_on_ground;
    }

}

void _scene::check_enemy_collisions()
{

    for(auto e : enemies)
    {
        if(!e->isEnmsLive) continue;

        bool isTouching = collision->isPlayerTouchingEnemy(player,e);
        bool isAbove = player->plPos.y > e->pos.y + e->scale.y;

        if(isTouching && isAbove)
        {
            e->isEnmsLive=false;
            e->start_respawn_timer = true;

            player->isJumping = true;
            player->is_grounded = false;
            player->height_before_jump = player->plPos.y;

        }
        else if (isTouching && player->player_can_be_damaged)
        {
            sndOof->playSounds("oof.mp3");
            player->health--;
            player->damage_timer->reset();
            player->player_can_be_damaged = false;
            player->blink = true;

            float pushback_strength = 0.6;
            if(player->plPos.x < e->pos.x)
                player->plPos.x -=pushback_strength;
            else
                player->plPos.x += pushback_strength;
        }
    }

}

void _scene::load_level_file(const char* file_name)
{
    player->inBarrel=false;
    player->isBeingDisplacedHorz=false;
    player->isJumping=false;
    for(auto p : platforms) delete p;
    for (auto e : enemies) delete e;
    for(auto c : collectibles) delete c;
    for(auto b : barrels) delete b;

    barrels.clear();
    platforms.clear();
    enemies.clear();
    collectibles.clear();

    ifstream file(file_name);
    string line;

    while(getline(file,line))
    {
        // check for empty lines and our self asserted comment symbol
        if(line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        string type;
        ss>>type;



        if(type == "PLATFORM")
        {
            string img;
            float x,y,z,sx,sy,sz;
            int fx,fy, t, chooseTex;

            ss>>img>>x>>y>>z>>sx>>sy>>sz>>fx>>fy>>t >> chooseTex;

            _platform* plat = new _platform();
            plat->initPlat(img.data(),x,y,z,sx,sy,sz,fx,fy,t,0,0,chooseTex);
            platforms.push_back(plat);
        }
        else if(type == "ENEMY")
        {
            //string img;
            float x,y,z,scl;
            int eType = 0;
            ss>>x>>y>>z>>scl>>eType;

            _enemies* enemy = new _enemies();


            switch(eType)
            {
            case 0:
                enemy->eT = enemy->WALKER;
                break;
            case 1:
                enemy->eT = enemy->JUMPER;
                break;
            case 2:
                enemy->eT = enemy->FLYER;
                //enemy->frames = 4;
                //enemy->xMax = 1.0 / enemy->frames;
                //enemy->xMin = 0;
                break;
            }


            enemy->initEnms();
            enemy->placeEnms({x,y,z},scl);
            enemy->isEnmsLive = true;
            enemy->action_trigger = enemy->WALKLEFT;


            enemies.push_back(enemy);
        }
        else if(type == "MOVING_PLATFORM")
        {
            string img;
            float x,y,z,sx,sy,sz,range,speed;
            int fx,fy, t, texType;

            //ss>>img>>x>>y>>z>>sx>>sy>>sz>>fx>>fy>>t>>speed>>range>>texType;
            ss >> img >> x >> y >> z >> sx >> sy >> sz >> fx >> fy >> t >> range >> speed >> texType;


            _platform* plat = new _platform();
            plat->initPlat(img.data(),x,y,z,sx,sy,sz,fx,fy, t,range,speed,texType);

            //if(direction == 1) plat->type = plat->HORIZONTAL;
            //else if (direction==2) plat->type=plat->VERTICAL;

            platforms.push_back(plat);
        }
        else if(type == "COLLECTIBLE")
        {
            string img;
            float x,y,z,r;
            int framex,framey;

            ss >> img >> x >> y >> z >> r >> framex >> framey;

            _collectible* c = new _collectible();
            c->initColl(img.data(), x, y, z, r, framex, framey);
            collectibles.push_back(c);
        }
        else if(type == "GOAL")
        {
            string img;
            float x,y,z,r;
            int fx,fy;

            ss>>img>>x>>y>>z>>r>>fx>>fy;

            if(goal) delete goal;
            goal = new _goal();
            goal->initGoal(img.data(),x,y,z,r,fx,fy);

        }
        else if (type == "BARREL")
        {
            float x,y,z,rotation;
            int isAuto,fireDelay;
            ss>>x>>y>>z>>rotation>>isAuto>>fireDelay;

            _barrelCannon* b = new _barrelCannon();
            b->initBarrel("images/barrel.png",{x,y,z},rotation,(bool)isAuto,fireDelay);
            barrels.push_back(b);
        }
    }
}

void _scene::checkCollectibles()
{
    for(auto c : collectibles)
    {
        if(c->isCollected) continue;

        if(collision->isPlayerTouchingCollectible(player, c))
        {
            c->isCollected = true;
            player->coins++;
            cout << "Coins: " << player->coins << endl;
        }
    }
    if(player->coins == 20)
        {
            player->health++;
            player->coins = 0;
        }
}

void _scene::checkGoal()
{
    if(playerWon) return;

    float dx = player->plPos.x - goal->pos.x;
    float dy = player->plPos.y - goal->pos.y;
    float dist = sqrt(dx * dx + dy * dy);

    if(dist < (player->plScl.x + goal->scl.x))
    {
        playerWon = true;
        cout << "player won";

        if(currLevel == 0)
        {
            gs = MAINMENU;
            //background->initPrlx("images/temp_mainmenu.png");
            return;
        }

        currLevel++;



        stringstream ss;

        ss << "levels/level"<<currLevel<<".txt";
        ifstream test(ss.str().c_str());
        if(!test.good())
        {
            //background->initPrlx("images/temp_reset.png");
            gs = WIN;
            currLevel=1;
            player->health = 3;
            player->inBarrel = false;
            player->isBeingDisplacedHorz = false;
            return ;
        }

        load_level_file(ss.str().c_str());
        player->initPlayer(playerTex);
        player->plPos={0,0,-3};
        playerWon=false;
    }
}


void _scene::drawMenu()
{
    glLoadIdentity();
    glColor3f(1, 0, 0);
    glDisable(GL_DEPTH_TEST);
    background->drawBackground(dim.x, dim.y,-35,17);
    glEnable(GL_DEPTH_TEST);


    startButton->updateHover(mouseX, mouseY);
    loadButton->updateHover(mouseX, mouseY);
    editorButton->updateHover(mouseX, mouseY);
    exitButton->updateHover(mouseX, mouseY);
    menuCreditsButton->updateHover(mouseX, mouseY);
    menuHelpButton->updateHover(mouseX, mouseY);

    startButton->drawButton();
    loadButton->drawButton();
    editorButton->drawButton();
    exitButton->drawButton();
    menuCreditsButton->drawButton();
    menuHelpButton->drawButton();

    menuCreditsButton->drawButton();
    menuHelpButton->drawButton();

}

void _scene::runGame()
{
    //glDisable(GL_LIGHTING);

    p3->drawBackground(dim.x,dim.y,-29,13);
    p2->drawBackground(dim.x,dim.y,-28,13);
    p1->drawBackground(dim.x,dim.y,-27,13);

    if(!isPaused)
    {




    p3->scroll();
    p2->scroll();
    p1->scroll();

    // don't change the order of these 4 functions -- player texture breaks otherwise
    player->playerActions();
    camera->followPlayer(player);
    camera->updateCamPos();


    player->handle_vertical();
    player->handle_player_damage_timer();
    player->handleHorizontalDisplacement();

    if(player->health < 1)
        {
            gs = GAMEOVER;
            player->health = 3;
            //background->initPrlx("images/temp_reset.png");
        }

    if(!player->blink)
    {
        player->drawPlayer();
    }
    if(!player->player_can_be_damaged)
    {
        player->blink = !player->blink;
    }
    else
        player->blink = false;




    //TODO: add a way for the player's health to matter
    if(player->plPos.y < -4.0)
    {
        //player->initPlayer(1,1,"images/wall.png");
        player->health--;
        player->plPos = {0,0,-3};

        cout << "\n" << player->health;
    }

    check_platform_collisions();
    check_enemy_collisions();
    checkCollectibles();

    for(auto b : barrels)
    {

        if(b->isPlayerInside(player->plPos, {player->plScl.x, player->plScl.y}) && b->cooldownTimer->getTicks() > 500) //&& !player->isBeingDisplacedHorz)
            {
                b->playerInside = true;
                player->inBarrel = true;
            }
        else
            b->playerInside = false;

        if(!player->inBarrel)
        {
            b->manualDelay->reset();
        }



        b->updateB(player);
    }

    for(auto e : enemies)
    {
        e->actions();
    }

    for(auto plat: platforms)
    {
        plat->updatePlat();
    }



    }
    else
    {
        camera->followPlayer(player);
        camera->updateCamPos();
        player->drawPlayer();
    }








    for(auto plat: platforms) plat->drawPlat();
    for(auto b : barrels) b->drawBarrel();
    for(auto e : enemies) e->drawEnms(e->tex->tex);
    //glEnable(GL_LIGHTING);
    for(auto c : collectibles) c->drawColl();
    //glDisable(GL_LIGHTING);

    goal->drawGoal();
    checkGoal();

    hud->drawHearts(player->health,camera->camPos);
    hud->updateBananaCount(player->coins, camera);
    hud->drawBananaIcon(camera);
    //b->updateB(player->plPos, player->velocity, player->isJumping);

    if(isPaused)
    {
        drawPausePopup();
    }
}


void _scene::drawGameOver()
{
    glDisable(GL_DEPTH_TEST);
    deathScreen->drawBackground(dim.x,dim.y,-30,13);
    //background->drawBackground(dim.x, dim.y,-21,13);
    glEnable(GL_DEPTH_TEST);


}

void _scene::saveGame()
{
    ofstream file("saves/save1.txt");
    if(!file.is_open()) return;

    file << "level="<<currLevel << endl;
    file << "health="<<player->health<<endl;
    file<<"coins="<<player->coins<<endl;

    file.close();

    cout <<"\nsaved" << endl;
}

void _scene::loadGame()
{
    isPaused = false;
    ifstream file("saves/save1.txt");
    if(!file.is_open())return;

    string line;

    while(getline(file,line))
    {
        stringstream ss(line);
        string key;
        getline(ss,key,'=');

        if(key=="level") ss >>currLevel;
        else if(key=="health") ss >> player->health;
        else if (key=="coins")ss >> player->coins;
    }

    file.close();

    stringstream ss;
    ss<<"levels/level"<<currLevel<<".txt";
    load_level_file(ss.str().c_str());

    player->initPlayer(playerTex);
    player->plPos = {0,0,-3};

    cout << "\nloaded";
}


void _scene::mouseMapping(int x, int y)
{
    dim.x = GetSystemMetrics(SM_CXSCREEN);  // only for windows
    dim.y = GetSystemMetrics(SM_CYSCREEN);  // only for windows

    GLint viewPort[4];       // store window coord
    GLdouble modelViewM[16]; // model and cam
    GLdouble projectionM[16];// projection
    GLfloat winX,winY,winZ;  // for mapping x,y,z

    glGetDoublev(GL_MODELVIEW_MATRIX,modelViewM);
    glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
    glGetIntegerv(GL_VIEWPORT, viewPort);

    winX = (GLfloat)x;
    winY = (GLfloat)(viewPort[3]-y);

    //glReadPixels(x,(int)winY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);

    //if(winZ == 1.0)
        //winZ = 0.5;

    //gluUnProject(winX,winY,winZ,
                 //modelViewM,projectionM,viewPort,&mouseX,&mouseY,&mouseZ);
                 //mouseZ = -3.0;

        //veryyy hacky fix
        GLdouble nearX, nearY, nearZ;
        GLdouble farX, farY, farZ;

        gluUnProject(winX, winY, 0.0, modelViewM, projectionM, viewPort, &nearX, &nearY, &nearZ);
        gluUnProject(winX, winY, 1.0, modelViewM, projectionM, viewPort, &farX, &farY, &farZ);

    // ray direction
    float dirX = farX - nearX;
    float dirY = farY - nearY;
    float dirZ = farZ - nearZ;

    // find t where z = -3.0 (our depth at which we place everything
    float t = (-3.0 - nearZ) / dirZ;

    mouseX = nearX + t * dirX;
    mouseY = nearY + t * dirY;
    mouseZ = -3.0;
}


void _scene::saveCustomLevel()
{
    ofstream file("levels/custom_level.txt");

    if(!file.is_open())
    {
        cout << "\ncustom level file not loaded";
    }

    for(auto plat:platforms)
    {
        if(int(plat->type) == 0)
        file<<"PLATFORM images/wall.png " << plat->pos.x << " " << plat->pos.y << " " << plat->pos.z << " "<< plat->scale.x << " " << plat->scale.y << " " << plat->scale.z << " " << plat->framesX << " " << plat->framesY << " " << int(plat->type) << " " << int(plat->chooseTex)<< endl;
        else
            file << "MOVING_PLATFORM images/wall.png "
     << plat->pos.x << " " << plat->pos.y << " " << plat->pos.z << " "
     << plat->scale.x << " " << plat->scale.y << " " << plat->scale.z << " "
     << plat->framesX << " " << plat->framesY << " "
     << int(plat->type) << " " << plat->speed << " " << plat->patrol_range << " "
     << int(plat->chooseTex) << "\n";

    }

    for(auto e:enemies)
    {
        file<<"ENEMY " << e->pos.x<< " " << e->pos.y << " " << e->pos.z << " " << e->scale.x << " " << (int)e->eT << endl;
    }

    for(auto c : collectibles)
    {
        file << "COLLECTIBLE images/banana.png "<< c->pos.x << " " << c->pos.y << " " << c->pos.z << " "<< c->radius << " " << c->framesX << " " << c->framesY << endl;
    }

    for(auto b : barrels)
    {
        file << "BARREL " << b->pos.x << " " << b->pos.y << " " << b->pos.z << " " << b->rotation << " " << (int)b->isAuto << " " << b->fireDelay << endl;
    }

    file << "GOAL images/goal.png " << goal->pos.x << " " << goal->pos.y << " " << goal->pos.z
     << " " << goal->scl.x << " " << goal->framesX << " " << goal->framesY << endl;

    file.close();
    cout << "saved to levels/custom_level.txt"<<endl;
}


void _scene::drawEditor()
{
    glLoadIdentity();

    /*if(previewBarrel)
    cout << '\n' << previewBarrel->pos.z << " " << previewBarrel->scale.x << " " << previewBarrel->scale.y;
    */

    //glDisable(GL_LIGHTING);

    glDisable(GL_DEPTH_TEST);
    p3->drawBackground(dim.x,dim.y,-29,13);
    p2->drawBackground(dim.x,dim.y,-28,13);
    p1->drawBackground(dim.x,dim.y,-27,13);



    //background->drawBackground(dim.x, dim.y,-35,13);
    glEnable(GL_DEPTH_TEST);

    camera->followPlayer(player);
    camera->updateCamPos();



    for(auto plat : platforms)
        plat->drawPlat();

    for(auto e : enemies)
        e->drawEnms(e->tex->tex);

    for(auto b : barrels)
        b->drawBarrel();

    for(auto c : collectibles)
        c->drawColl();
    goal->drawGoal();

    if(!isPaused)
    {




    //p3->scroll();
    //p2->scroll();
    //p1->scroll();

        //todo: check this
        glDisable(GL_DEPTH_TEST); // ensure preview is always on top
        glColor4f(1.0, 1.0, 1.0, 0.5); // semi transparent

        if(previewPlat && placeObj == PLAT)
            previewPlat->drawPlat();
        if(previewEnemy && placeObj == ENEMY)
            previewEnemy->drawEnms(previewEnemy->tex->tex);
        if(previewCoin && placeObj == COLLECTIBLE)
            previewCoin->drawColl();
        if(previewGoal && placeObj == GOAL)
            previewGoal->drawGoal();
        if(previewBarrel && placeObj == BARREL)
            previewBarrel->drawBarrel();

        glEnable(GL_DEPTH_TEST);
        glColor4f(1.0, 1.0, 1.0, 1.0); // reset color

        glDisable(GL_DEPTH_TEST);
        for(int i = 0; i < 7; i++)
        {
            inventoryButtons[i]->drawButton();
        }

        if(placeObj==PLAT)
        {
            for(int i =0;i<2;i++)
            {
                platTextureButtons[i]->drawButton();
                platAttributeButtons[i]->drawButton();
            }
            platAttributeStatic->drawText("STATIC");
            platAttributeMoving->drawText("MOVING");
        }



        glEnable(GL_DEPTH_TEST);


    }
        if(isPaused)
            drawPausePopup();
}

void _scene::deleteObjectAtMouseInEditor()
{
    // platform check
    for (int i = 0; i < platforms.size(); i++)
    {
        _platform* p = platforms[i];
        if (mouseX > p->pos.x - p->scale.x &&
            mouseX < p->pos.x + p->scale.x &&
            mouseY > p->pos.y - p->scale.y &&
            mouseY < p->pos.y + p->scale.y)
        {
            delete p;
            platforms.erase(platforms.begin() + i);
            cout << "Deleted platform.\n";
            return;
        }
    }

    // ENEMY check
    for (int i = 0; i < enemies.size(); i++)
    {
        _enemies* e = enemies[i];
        if (mouseX > e->pos.x - e->scale.x &&
            mouseX < e->pos.x + e->scale.x &&
            mouseY > e->pos.y - e->scale.y &&
            mouseY < e->pos.y + e->scale.y)
        {
            delete e;
            enemies.erase(enemies.begin() + i);
            cout << "Deleted enemy.\n";
            return;
        }
    }

    // COLLECTIBLE check
    for (int i = 0; i < collectibles.size(); i++)
    {
        _collectible* c = collectibles[i];
        if (mouseX > c->pos.x - c->radius &&
            mouseX < c->pos.x + c->radius &&
            mouseY > c->pos.y - c->radius &&
            mouseY < c->pos.y + c->radius)
        {
            delete c;
            collectibles.erase(collectibles.begin() + i);
            cout << "Deleted coin.\n";
            return;
        }
    }


    for (int i = 0; i < barrels.size();i++)
    {
        _barrelCannon *b = barrels[i];
        if (mouseX > b->pos.x - b->scale.x &&
            mouseX < b->pos.x + b->scale.x &&
            mouseY > b->pos.y - b->scale.y &&
            mouseY < b->pos.y + b->scale.y)
            {
                delete b;
                barrels.erase(barrels.begin() + i);
                return;
            }
    }
}


void _scene::initMenuButtons()
{
    startButton->initButton("images/new_images/newgame_buttons.png",0,1.2,-3,1.0,0.3,1.0,2,1);
    loadButton->initButton("images/new_images/load_buttons.png",0,0.6,-3,1.0,0.3,1.0,2,1);
    editorButton->initButton("images/new_images/editor_buttons.png",0,-.1,-3,1.0,0.3,1.0,2,1);
    exitButton->initButton("images/new_images/quit_buttons.png",0,-.9,-3,1.0,0.3,1.0,2,1);
    menuCreditsButton->initButton("images/new_images/credits_buttons.png",-2.2,-.9,-3,1.0,0.3,1.0,2,1);
    menuHelpButton->initButton("images/new_images/help_buttons.png",2.2,-.9,-3,1.0,0.3,1.0,2,1);


    resumeButton->initButton("images/new_images/continue_buttons.png", 0, 0.3, -2, 1.0, 0.3, 1.0, 2, 1);
    backToMenuButton->initButton("images/new_images/mainmenu_buttons.png", 0, -0.5, -2, 1.0, 0.3, 1.0, 2, 1);

    startScreenButton->initButton("images/new_images/playnow_buttons.png",0, -0.5, -2, 1.0, 0.3, 1.0, 2, 1);
    jungleAdventureSS->initButton("images/new_images/Game_Title.png", 0, 0.5, -2, 1.0, 1.0, 1.0, 1,1);

    creditsBackButton->initButton("images/new_images/mainmenu_buttons.png",-1.5, -.9, -2, 1.0, .5, 1.0, 2,1);
    helpBackButton->initButton("images/new_images/mainmenu_buttons.png",-1.8, -1.0, -2, .7, .3, 1.0, 2,1);

    loadSaveButton->initButton("images/new_images/saved_button.png",0,0.7,-3,1.0,0.3,1.0,2,1);
    loadCustomButton->initButton("images/new_images/saved_button.png",0,-.1,-3,1.0,0.3,1.0,2,1);



}


void _scene::drawPausePopup()
{
    glDisable(GL_DEPTH_TEST);


    glPushMatrix();

    // Move the popup in front of the camera
    glTranslatef(camera->camPos.x, camera->camPos.y, camera->camPos.z - 4.0f);


    glScalef(2.0f, 1.5f, 1.0f);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(camera->camPos.x, camera->camPos.y, camera->camPos.z - 3.5f);

    //float adjustedMouseX = mouseX - camera->camPos.x;
    //float adjustedMouseY = mouseY - camera->camPos.y;
    resumeButton->updateHover(mouseX, mouseY);
    backToMenuButton->updateHover(mouseX, mouseY);

    //resumeButton->updateHover(mouseX, mouseY);
    //backToMenuButton->updateHover(mouseX, mouseY);

    resumeButton->drawButton();
    backToMenuButton->drawButton();

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}



void _scene::drawWinScreen()
{
    glDisable(GL_DEPTH_TEST);
    winBG->drawBackground(dim.x,dim.y,-30,13);
    glEnable(GL_DEPTH_TEST);

}

void _scene::drawstartScreen()
{
    glDisable(GL_DEPTH_TEST);
    startScreenBG->drawBackground(dim.x,dim.y,-30,13);
    glEnable(GL_DEPTH_TEST);

    jungleAdventureSS->drawButton();
    startScreenButton->drawButton();

    startScreenButton->updateHover(mouseX,mouseY);
}


void _scene::drawHelpScreen()
{
    glDisable(GL_DEPTH_TEST);
    helpScreenBG->drawBackground(dim.x,dim.y,-30,13);
    glEnable(GL_DEPTH_TEST);

    helpBackButton->drawButton();

    helpBackButton->updateHover(mouseX,mouseY);

}

void _scene::drawCreditsScreen()
{
    glDisable(GL_DEPTH_TEST);
    creditsScreenBG->drawBackground(dim.x,dim.y,-30,13);
    glEnable(GL_DEPTH_TEST);

    creditsBackButton->drawButton();
    creditsBackButton->updateHover(mouseX,mouseY);
}

void _scene::initEditorInventory()
{
    float yPos = 3.0;

    inventoryButtons[0] = new _buttons();  // Platform
    inventoryButtons[0]->initButton("images/stone_plat.png", -3.0, yPos, -2, 0.5, 0.5, 1.0, 1, 1);

    inventoryButtons[1] = new _buttons();  // Walker
    inventoryButtons[1]->initButton("images/temp_enemy.png", -2.2, yPos, -2, 0.5, 0.5, 1.0, 4, 1);

    inventoryButtons[2] = new _buttons();  // Jumper
    inventoryButtons[2]->initButton("images/temp_jumper.png", -1.4, yPos, -2, 0.5, 0.5, 1.0, 3, 1);

    inventoryButtons[3] = new _buttons();  // Flyer
    inventoryButtons[3]->initButton("images/temp_flyer.png", -0.6, yPos, -2, 0.5, 0.5, 1.0, 3, 1);

    inventoryButtons[4] = new _buttons();  // Collectible
    inventoryButtons[4]->initButton("images/banana.png", 0.2, yPos, -2, 0.5, 0.5, 1.0, 1, 1);

    inventoryButtons[5] = new _buttons();  // Goal
    inventoryButtons[5]->initButton("images/goal.png", 1.0, yPos, -2, 0.5, 0.5, 1.0, 1, 1);

    inventoryButtons[6] = new _buttons();  // Barrel
    inventoryButtons[6]->initButton("images/barrel.png", 1.8, yPos, -2, 0.5, 0.5, 1.0, 1, 1);


    platTextureButtons[0] = new _buttons();
    platTextureButtons[0]->initButton("images/stone_plat.png",-6.0,0.8,-2,0.5,0.5,1.0,1,1);

    platTextureButtons[1] = new _buttons();
    platTextureButtons[1]->initButton("images/dirt_plat.png",-6.0,-0.5,-2,0.5,0.5,1.0,1,1);

    platAttributeButtons[0] = new _buttons();
    platAttributeButtons[0]->initButton("images/border.png", -5.5,-2,-2,1.2,0.2,1.0,2,1);

    platAttributeButtons[1] = new _buttons();
    platAttributeButtons[1]->initButton("images/border.png", -5.5,-3,-2,1.2,0.2,1.0,2,1);

    platAttributeMoving->pos.x = -6.2;
    platAttributeStatic->pos.x =-6.2;

}

void _scene::drawSaveScreen()
{
    glDisable(GL_DEPTH_TEST);
    background->drawBackground(dim.x,dim.y,-30,13);
    glEnable(GL_DEPTH_TEST);

    creditsBackButton->drawButton();
    loadSaveButton->drawButton();
    loadCustomButton->drawButton();

    creditsBackButton->updateHover(mouseX,mouseY);
    loadSaveButton->updateHover(mouseX,mouseY);
    loadCustomButton->updateHover(mouseX,mouseY);

}


