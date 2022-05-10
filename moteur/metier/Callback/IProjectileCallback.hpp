#ifndef __I_PROJECTILE_CALLBACK__
#define __I_PROJECTILE_CALLBACK__

class IProjectileCallback
{
public:

	enum Direction {
		DEVANT,
		DERRIERE,
		GAUCHE,
		DROITE,
		HAUT,
		BAS
	};
    virtual void actionFireBall() = 0;

	virtual void moveDirection(int id, Direction direction, float deltaTime) = 0;
	virtual void mouseMovement(float x, float y) = 0;
	virtual void togglePhysicDebug() = 0;
};

#endif