#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <Component.hpp>
#include <Collision/BoundingBox.hpp>

class Collision : public Component {
private:
	BoundingBox* boundingBox = NULL;
	bool actif;
public:
	Collision(bool actif = false) : Component(Component::Type::COLLISION){
		this->actif = actif;
	}

	Collision(std::vector<glm::vec3> indexed_vertices, bool actif = true) : Component(Component::Type::COLLISION) {
		this->boundingBox = new BoundingBox(indexed_vertices);
		this->actif = actif;
	}

	Collision(BoundingBox* b, bool actif = true) : Component(Component::Type::COLLISION) {
		this->boundingBox = b;
		this->actif = actif;
	}


	~Collision() {
		delete this->boundingBox;
	}

	//Methodes
	void computeBoundingBox(std::vector<glm::vec3> indexed_vertices) {
		this->boundingBox = new BoundingBox(indexed_vertices);
	}

	static bool computeCollision(Collision* c1, Collision* c2) {
		bool res = false;
		if (c1->isActif() && c2->isActif()) {
			if (c1->getBoundingBox()->isIntersection(c2->getBoundingBox())) {
				//do phase 2
				res = true;
			}
		}
		return res;
	}


	//Getter ans Setter
	bool isActif() {
		return this->actif;
	}

	void setActif(bool a) {
		this->actif = a;
	}

	BoundingBox* getBoundingBox() {
		return this->boundingBox;
	}

	void setBoundingBox(BoundingBox* b) {
		this->boundingBox = b;
	}

	virtual void apply(glm::mat4 tranformationMatrix) {
		this->boundingBox->applyTransformation(tranformationMatrix);
	}
};

#endif