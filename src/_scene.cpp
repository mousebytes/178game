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

_lightSetting *myLight = new _lightSetting();
_inputs *input = new _inputs();
_player *player = new _player();
_camera *camera = new _camera();
_background *background = new _background();
_collisionCheck *collision = new _collisionCheck();
_hud *hud = new _hud();
_goal *goal = new _goal();

vector<_platform*> platforms;
vector<_enemies*> enemies;
vector<_collectible*> collectibles;

bool playerWon = false;
int currLevel = 1;


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

    background->initBG("images/temp_mainmenu.png");

    load_level_file("levels/level1.txt");

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

    glDisable(GL_DEPTH_TEST);

    background->drawBackground(dim.x, dim.y);
    glEnable(GL_DEPTH_TEST);


    switch(gs)
    {
    case MAINMENU:
        drawMenu();
        break;
    case PLAYING:
        runGame();
        break;
    case GAMEOVER:
        drawGameOver();
        break;
    }






}

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_KEYDOWN:
           input->wParam = wParam;

           if(gs == MAINMENU && wParam == VK_RETURN)
           {
                background->initBG("images/marce.png");
                currLevel = 1;
                load_level_file("levels/level1.txt");
                player->initPlayer(1,1,"images/wall.png");
                gs = PLAYING;
                playerWon= false;
           }
            else if (gs == GAMEOVER && wParam == 'R')
           {
               background->initBG("images/temp_mainmenu.png");
               gs = MAINMENU;

               player->initPlayer(1,1,"images/wall.png");
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
                    background->initBG("images/marce.png");
                    loadGame();
                    gs = PLAYING;
                }
            else if(gs == PLAYING)
           {
                input->keyPressed(player);
           }
        break;


    case WM_KEYUP:
        input->wParam = wParam;
        input->keyUP(player);
        break;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        input->wParam = wParam;
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        input->mouseEventUp();
        break;
    case WM_MOUSEMOVE:
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

    for(auto p : platforms) delete p;
    for (auto e : enemies) delete e;
    for(auto c : collectibles) delete c;
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
            int fx,fy, t;

            ss>>img>>x>>y>>z>>sx>>sy>>sz>>fx>>fy>>t;

            _platform* plat = new _platform();
            plat->initPlat(img.data(),x,y,z,sx,sy,sz,fx,fy,t,0,0);
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
        else if(type == "MOVING_PLATFORM")
        {
            string img;
            float x,y,z,sx,sy,sz,range,speed;
            int fx,fy, t;

            ss>>img>>x>>y>>z>>sx>>sy>>sz>>fx>>fy>>t>>speed>>range;

            _platform* plat = new _platform();
            plat->initPlat(img.data(),x,y,z,sx,sy,sz,fx,fy, t,range,speed);

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

        currLevel++;

        stringstream ss;

        ss << "levels/level"<<currLevel<<".txt";
        ifstream test(ss.str().c_str());
        if(!test.good())
        {
            background->initBG("images/temp_reset.png");
            gs = GAMEOVER;
            currLevel=1;
            player->health = 3;
            return ;
        }

        load_level_file(ss.str().c_str());
        player->initPlayer(1,1,"images/wall.png");
        player->plPos={0,0,-3};
        playerWon=false;
    }
}


void _scene::drawMenu()
{
    glLoadIdentity();
    glColor3f(1, 0, 0);
}

void _scene::runGame()
{
    // don't change the order of these 4 functions -- player texture breaks otherwise
    player->playerActions();
    camera->followPlayer(player);
    camera->updateCamPos();

    if(player->health < 1)
        {
            gs = GAMEOVER;
            player->health = 3;
            background->initBG("images/temp_reset.png");
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

    player->handle_vertical();
    player->handle_player_damage_timer();


    //TODO: add a way for the player's health to matter
    if(player->plPos.y < -4.0)
    {
        //player->initPlayer(1,1,"images/wall.png");
        player->health--;
        player->plPos = {0,0,-3};

        cout << "\n" << player->health;
    }



    for(auto plat: platforms)
    {
        plat->drawPlat();
        plat->updatePlat();
    }

    for(auto e : enemies)
    {
        e->drawEnms(enemies[0]->tex->tex);
        e->actions();
    }

    for(auto c : collectibles)
        c->drawColl();

    check_platform_collisions();
    check_enemy_collisions();
    checkCollectibles();

    goal->drawGoal();
    checkGoal();

    hud->drawHearts(player->health,camera->camPos);
}

void _scene::drawGameOver()
{

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

    player->initPlayer(1,1,"images/wall.png");
    player->plPos = {0,0,-3};

    cout << "\nloaded";
}

