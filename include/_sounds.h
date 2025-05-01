#ifndef _SOUNDS_H
#define _SOUNDS_H
#include <SNDS/irrKlang.h>

using namespace irrklang;
using namespace std;

class _sounds
{
    public:
        _sounds();
        virtual ~_sounds();
// play background music
        void playMusic(char *);//passing file name directory give music path
       // action sounds
        void playSounds(char *);//pass file name
      //pause or STOP
        void pauseSounds(char *);
        int initSounds();

        ISoundEngine *sndEng = createIrrKlangDevice();// creates sounds instance \



    protected:

    private:
};

#endif // _SOUNDS_H
