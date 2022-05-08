#ifndef __I_PROJECTILE_CALLBACK__
#define __I_PROJECTILE_CALLBACK__

class IProjectileCallback
{
public:
    virtual void actionFireBall() = 0;
    virtual void actionGoUp() = 0;
};

#endif