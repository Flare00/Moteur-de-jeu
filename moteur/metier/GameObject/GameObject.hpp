#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "../Transformation.hpp"
#include "../World/Camera.hpp"

class GameObject {
private:

	std::string identifier;
	Transformation* transform;
	GameObject* parent;
	std::vector<GameObject*> childs;

public:
	GameObject(std::string id, GameObject* parent = NULL) {
		this->identifier = id;
		this->parent = parent;
		this->transform = new Transformation();

	}

	GameObject(std::string id, GameObject* parent, std::vector<GameObject*> childs) {
		this->identifier = id;
		this->parent = parent;
		this->childs = childs;
		this->transform = new Transformation();

	}

	~GameObject() {
		free(this->parent);
		for (int i = 0, max = this->childs.size(); i < max; i++) {
			free(this->childs[i]);
		}
		free(this->transform);
	}

	glm::mat4 getGlobalMatrixRecursive() {
		if (this->parent != NULL) {
			return this->transform->getGlobalMatrix(this->parent->getGlobalMatrixRecursive());

		}
		return this->transform->getGlobalMatrix();
	}

	glm::mat4 getTransformMatrix() {
		if (this->parent != NULL) {
			return this->transform->getMatrix(this->parent->getGlobalMatrixRecursive());
		}
		return this->transform->getMatrix();
	}

	void drawDFS() {
		for (int i = 0, max = this->childs.size(); i < max; i++) {
			this->childs[i]->draw(true);
		}
	}

	void drawBFS() {
		std::vector<GameObject*> parcours;
		parcours.push_back(this);

		while (parcours.size() > 0) {
			GameObject* current = parcours[0];
			parcours.erase(parcours.begin());
			if (current != this) {
				current->draw(false);
			}
			std::vector<GameObject*> currentChilds = current->getChilds();
			for (int i = 0, max = currentChilds.size(); i < max; i++) {
				parcours.push_back(currentChilds[i]);
			}
		}

	}

	virtual void draw(bool dfs) {
		if (dfs) {
			this->drawDFS();
		}
		else {
			this->drawBFS();
		}
	}

	//Retourne un enfant selon sont identifiant, seulement si enfant directe de l'objet courant.
	GameObject* findDirectChild(std::string identifier) {
		GameObject* res = NULL;
		for (int i = 0, max = this->childs.size(); i < max && res == NULL; i++) {
			if (this->childs[i]->getIdentifier().compare(identifier) == 0) {
				res = this->childs[i];
			}
		}
		return res;
	}

	//Retourne un enfant selon sont identifiant si l'enfant se trouve dans la g�n�alogie. Parcours DFS
	GameObject* findChild(std::string identifier) {
		GameObject* res = NULL;
		for (int i = 0, max = this->childs.size(); i < max && res == NULL; i++) {
			if (this->childs[i]->getIdentifier().compare(identifier) == 0) {
				res = this->childs[i];
			}
			else {
				res = this->childs[i]->findChild(identifier);
			}
		}
		return res;
	}

	std::string getIdentifier() {
		return this->identifier;
	}

	GameObject* getChild(int i) {
		return this->childs[i];
	}

	std::vector<GameObject*> getChilds() {
		return this->childs;
	}

	void addChild(GameObject* child) {
		child->setParent(this);
		this->childs.push_back(child);
	}

	void removeChild(int index) {
		this->childs[index]->setParent(NULL);
		this->childs.erase(this->childs.begin() + index);
	}

	Transformation* getTransform() {
		return this->transform;
	}

	void setTransform(Transformation* transform) {
		this->transform = transform;
	}

	GameObject* getParent() {
		return this->parent;
	}

	void setParent(GameObject* parent) {
		this->parent = parent;
	}

};

#endif