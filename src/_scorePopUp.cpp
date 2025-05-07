#include "_scorePopUp.h"

_scorePopUp::_scorePopUp(string t, vec3 p)
{
    //ctor
    pos = {p.x,p.y,-2};
    text = t;
    riseSpeed = 0.01;
    lifeTime = 800.0;
    scale = .1;

    lifeTimer = new _timer();
    font = new _fonts();
    tmr = new _timer();

    tmr->reset();
    lifeTimer->reset();

    font->initFonts("images/fontsheet.png",15,8);
    font->setSize(scale,scale);
    font->setPosition(pos.x,pos.y,pos.z);
}

_scorePopUp::~_scorePopUp()
{
    //dtor
    delete lifeTimer;
    delete font;
    delete tmr;
}


bool _scorePopUp::update()
{
    float elapsed =(float)lifeTimer->getTicks();
    if(elapsed > lifeTime) return false;

    if(tmr->getTicks() > 30)
    {
        pos.y += riseSpeed;
        font->setPosition(pos.x,pos.y,pos.z);
        tmr->reset();
    }

    float alpha = 1.0-(elapsed/lifeTime);
    glColor4f(1,1,1,alpha);

    return true;
}
void _scorePopUp::draw()
{
    glDisable(GL_DEPTH_TEST);
    font->drawText(text);
    glEnable(GL_DEPTH_TEST);
}
