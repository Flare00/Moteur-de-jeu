#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "GameObject/GameObject.hpp"
class GameObject;

class Component {
public :
	enum Type {
		Collision,
		Gravity
	};
private :
	Type type;

public :
	Component(Type type) {
		this->type = type;
	}

	Type getType() {
		return this->type;
	}
};

#endif // !__COMPONENT_H__
