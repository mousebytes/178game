#ifndef _SCOREPOPUP_H
#define _SCOREPOPUP_H

#include<_timer.h>
#include<_common.h>
#include<_fonts.h>

class _scorePopUp
{
    public:
        _scorePopUp(string t, vec3 p);
        virtual ~_scorePopUp();

    vec3 pos;
    string text;
    float riseSpeed;
    float lifeTime;
    _timer* lifeTimer;
    _fonts* font;
    float scale;
    _timer *tmr;

    scorePopUp(string t, vec3 p);

    bool update();
    void draw();

    protected:

    private:
};

#endif // _SCOREPOPUP_H
