#ifndef _SCENE_
#define _SCENE_

#include<_common.h>
#include<_timer.h>

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
        void check_enemy_collisions();
        void load_level_file(const char*);
        void checkCollectibles();
        void checkGoal();
        void saveGame();
        void loadGame();
        void mouseMapping(int x, int y);

        enum GameState {MAINMENU,PLAYING,GAMEOVER,LEVELEDITOR, PAUSED, WIN, STARTSCREEN, CREDITS, HELP, SAVESCREEN};

        GameState gs = STARTSCREEN;
        GameState previousGS;

        double mouseX, mouseY, mouseZ;

        enum PlaceObjs{PLAT,ENEMY,COLLECTIBLE, GOAL, BARREL,FONT};
        PlaceObjs placeObj = PLAT;

    vec2 dim;

        void drawMenu();
        void runGame();
        void drawGameOver();
        void saveCustomLevel();
        void drawEditor();

        void deleteObjectAtMouseInEditor();

        void initMenuButtons();

        bool SceneDone, isPaused;

        void drawPausePopup();

        void drawWinScreen();
        void drawstartScreen();
        void drawHelpScreen();
        void drawCreditsScreen();
        void drawSaveScreen();

        void initEditorInventory();

    protected:

    private:
};

#endif // _SCENE_
