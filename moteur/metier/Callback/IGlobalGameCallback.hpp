#ifndef __I_GLOBAL_GAME_CALLBACK__
#define __I_GLOBAL_GAME_CALLBACK__

class IGlobalGameCallback
{
public:
    virtual void askSceneChange(int numero) = 0;
    virtual void askNextScene(bool next) = 0;
};

#endif