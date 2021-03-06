#ifndef __I_LIGHT_CALLBACK__
#define __I_LIGHT_CALLBACK__

class ILightCallback
{

public:
	enum Direction
	{
		DEVANT,
		DERRIERE,
		GAUCHE,
		DROITE,
		HAUT,
		BAS
	};

	virtual void moveDirection(int id, Direction direction, float deltaTime) = 0;
	virtual void mouseMovement(float x, float y) = 0;
	virtual void toggleTorchMode() = 0;
	virtual void togglePhysicDebug() = 0;
};

#endif