#include "_scene.h"
#include<_lightsetting.h>
#include<_model.h>
#include<_inputs.h>
#include<_player.h>
#include<_camera.h>
#include<_background.h>
#include<_collisionCheck.h>
#include<_platform.h>
#include<_enemies.h>
#include<_hud.h>


_lightSetting *myLight = new _lightSetting();
_model *myModel = new _model();    // creating instance for model
_inputs *input = new _inputs();
_player *player = new _player();
_camera *camera = new _camera();
_background *background = new _background();
_collisionCheck *collision = new _collisionCheck();
_platform *level1_floor = new _platform();
_platform *test_plat = new _platform();
_platform *test_plat2 = new _platform();
_enemies *test_enemy = new _enemies();
_hud *hud = new _hud();

vector<_platform*> platforms;
vector<_enemies*> enemies;

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

    //level1_floor->initPlat("images/wall.png",0,-2,-2,5.0,1.0,1.0,1,1);
    //test_plat->initPlat("images/wall.png",9.0,-1,-2,3.0,1.0,1.0,1,1);
    //test_plat2->initPlat("images/wall.png",18,-1,-2,4.0,1.0,1.0,1,1);


    //test_enemy->initEnms("images/wall.png");
    //test_enemy->placeEnms({18,2,-3}, .6);
    //test_enemy->isEnmsLive = true;
    //test_enemy->action_trigger = test_enemy->WALKRIGHT;

    load_level_file("levels/testLevel.txt");

    hud->initHud("images/heart.png",1,1,camera->camPos);

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

    player->handle_vertical();
    player->handle_player_damage_timer();


    //TODO: add a way for the player's health to matter
    if(player->plPos.y < -4.0)
    {
        player->initPlayer(1,1,"images/wall.png");
        player->health--;
        if(player->health < 1)
            player->health = 3;
        cout << "\n" << player->health;
    }

    background->drawBackground(dim.x,dim.y);

    //level1_floor->drawPlat();
    //test_plat->drawPlat();
    //test_plat2->drawPlat();

    for(auto plat: platforms)
        plat->drawPlat();

    check_platform_collisions();

    //test_enemy->drawEnms(test_enemy->tex->tex);
    //test_enemy->actions();
    for(auto e : enemies)
    {
        e->drawEnms(enemies[0]->tex->tex);
        e->actions();
    }


    check_enemy_collisions();

    hud->drawHearts(player->health,camera->camPos);

    //TODO: Knock the player back when they collide with enemies

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

void _scene::check_platform_collisions()
{
    bool on_any_plat = false;

    // TODO: change this to for loop for all plats
    //if(collision->isPlayerOnGround(player,level1_floor)) on_any_plat = true;
    //if(collision->isPlayerOnGround(player,test_plat)) on_any_plat = true;
    //if(collision->isPlayerOnGround(player,test_plat2)) on_any_plat = true;

    //player->is_grounded = on_any_plat;

    for(auto p : platforms)
        if(collision->isPlayerOnGround(player,p)) on_any_plat = true;
    player->is_grounded=on_any_plat;

    // can change this to a for loop for all enemies and plats in the future
    //bool enemy_on_ground = false;
    //if (collision->isEnemyOnGround(test_enemy, level1_floor)) enemy_on_ground = true;
    //if (collision->isEnemyOnGround(test_enemy, test_plat)) enemy_on_ground = true;
    //if(collision->isEnemyOnGround(test_enemy,test_plat2)) enemy_on_ground = true;

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

    //test_enemy->is_grounded = enemy_on_ground;
}

void _scene::check_enemy_collisions()
{
    // TODO: change this to a for loop for all enemies later
    /*
    if(test_enemy->isEnmsLive)
    {
        // is the player overlapping the enemy in both axes?
        bool isTouching = collision->isPlayerTouchingEnemy(player, test_enemy);

        // ss player falling downward and above enemys head?
        bool isAbove = player->plPos.y > test_enemy->pos.y + test_enemy->scale.y;

        if (isTouching && isAbove)
        {
            // kill the enemy
            test_enemy->isEnmsLive = false;

            // bounce player upward
            player->isJumping = true;
            player->is_grounded = false;
            player->height_before_jump = player->plPos.y;

            std::cout << "Enemy stomped!\n";
        }
        else if (isTouching && player->player_can_be_damaged)
        {
            // damage player
            player->health--;
            player->damage_timer->reset();
            player->player_can_be_damaged = false;
            player->blink = true;

            if(player->health < 1)
            {
                // maybe create a reset player function later
                player->initPlayer(1,1,"images/wall.png");
                player->health = 3;
            }

            float pushback_strength = 0.6;

            if (player->plPos.x < test_enemy->pos.x)
            {
                player->plPos.x -= pushback_strength;
            }
            // If player is on the right side of the enemy, push right
            else
            {
                player->plPos.x += pushback_strength;
            }

            std::cout << "Player hit! Health: " << player->health << std::endl;
        }
    }*/

    for(auto e : enemies)
    {
        if(!e->isEnmsLive) continue;

        bool isTouching = collision->isPlayerTouchingEnemy(player,e);
        bool isAbove = player->plPos.y > e->pos.y + e->scale.y;

        if(isTouching && isAbove)
        {
            e->isEnmsLive=false;

            player->isJumping = true;
            player->is_grounded = false;
            player->height_before_jump = player->plPos.y;
        }
        else if (isTouching && player->player_can_be_damaged)
        {
            player->health--;
            player->damage_timer->reset();
            player->player_can_be_damaged = false;
            player->blink = true;

            if(player->health < 1)
            {
                player->initPlayer(1,1,"images/wall.png");
                player->health = 3;
            }

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
    for(auto p : platforms) delete p;
    for (auto e : enemies) delete e;
    platforms.clear();
    enemies.clear();

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
            int fx,fy;

            ss>>img>>x>>y>>z>>sx>>sy>>sz>>fx>>fy;

            _platform* plat = new _platform();
            plat->initPlat(img.data(),x,y,z,sx,sy,sz,fx,fy);
            platforms.push_back(plat);
        }
        else if(type == "ENEMY")
        {
            string img;
            float x,y,z,scl;
            ss>>img>>x>>y>>z>>scl;

            _enemies* enemy = new _enemies();
            enemy->initEnms(img.data());
            enemy->placeEnms({x,y,z},scl);
            enemy->isEnmsLive = true;
            enemy->action_trigger = enemy->WALKLEFT;
            enemies.push_back(enemy);
        }
    }
}


