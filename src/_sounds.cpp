#include "_sounds.h"
#include "_common.h"
_sounds::_sounds()
{
    //ctor
}

_sounds::~_sounds()
{
    //dtor
    sndEng->drop();
}
void _sounds::playMusic(char* fileName)
{
    sndEng->stopAllSounds();
    sndEng->play2D(fileName,true);//true means runs on loop
}


void _sounds::playSounds(char* fileName)
{// unmute iff and plays sound
  //  if(!sndEng->isCurrentlyPlaying(fileName)) //currently not playing
        sndEng->play2D(fileName,false,false); //plays once and stops

}

void _sounds::pauseSounds(char* fileName)
{
    sndEng->play2D(fileName, true, false);//pause sound
}

int _sounds::initSounds()
{
    if(!sndEng){
     return 0; //if no music stop playing
    }

    return 1;//means good
}
