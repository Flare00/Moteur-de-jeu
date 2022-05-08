#ifndef __COLLISION_FILTER_HPP__
#define __COLLISION_FILTER_HPP__

#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>

class CollisionFilter : public btOverlapFilterCallback
{
public:
    // Group 0 : nothingness
    // Group 1 : Everything
    // Group n : ... obj ... (collide if mask != mask)
    virtual bool needBroadphaseCollision(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) const
    {
        if (proxy0->m_collisionFilterGroup == 0 || proxy1->m_collisionFilterGroup == 0)
        {
            return false;
        }
        if (proxy0->m_collisionFilterGroup == 1 || proxy1->m_collisionFilterGroup == 1)
        {
            return true;
        }
        if (proxy0->m_collisionFilterGroup != proxy1->m_collisionFilterGroup)
        {
            return true;
        }
        if (proxy0->m_collisionFilterGroup == proxy1->m_collisionFilterGroup && proxy0->m_collisionFilterMask == proxy1->m_collisionFilterMask)
        {
            return false;
        }
        return true;
    }
};

#endif