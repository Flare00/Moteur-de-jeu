#ifndef __I_GLOBAL_SCENE_CALLBACK__
#define __I_GLOBAL_SCENE_CALLBACK__

class IGlobalSceneCallback
{
public:
    virtual void askCameraChange(int numero) = 0;
    virtual void askNextCamera(bool next) = 0;
};

#endif