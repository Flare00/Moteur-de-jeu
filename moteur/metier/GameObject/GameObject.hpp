#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include <Component.hpp>
#include <Light/Lightning.hpp>
#include <World/CameraData.hpp>
#include <Transformation/Transformation.hpp>
#include <Transformation/BulletTransformation.hpp>

class GameObject
{
protected:
	std::string identifier;
	GameObject *parent;
	std::vector<GameObject *> childs;
	std::vector<Component *> composants;
	BulletRigidbody *rigidBody = NULL;

	ITransformation *transform = NULL;
	bool isBulletDependent = false;

public:
	GameObject(std::string id, GameObject *parent = NULL, BulletRigidbody *rigidbody = NULL)
	{
		this->identifier = id;
		this->parent = parent;
		this->rigidBody = rigidbody;
		if (this->parent != NULL)
		{
			this->parent->addChild(this);
		}

		if (rigidbody == NULL)
		{
			this->transform = new Transformation();
		}
		else
		{
			this->transform = new BulletTransformation(rigidbody);
			isBulletDependent = true;
		}
	}

	GameObject(std::string id, GameObject *parent, std::vector<GameObject *> childs, BulletRigidbody *rigidbody = NULL)
	{
		this->identifier = id;
		this->parent = parent;
		this->childs = childs;
		this->rigidBody = rigidbody;
		if (this->parent != NULL)
		{
			this->parent->addChild(this);
		}
		if (rigidbody == NULL)
		{
			this->transform = new Transformation();
		}
		else
		{
			this->transform = new BulletTransformation(rigidbody);
			isBulletDependent = true;
		}
	}

	~GameObject()
	{
		for (size_t i = 0, max = this->childs.size(); i < max; i++)
		{
			delete this->childs[i];
		}
		delete this->transform;
	}

	void generateRigidbodyTransform(BulletRigidbody *rigidbody)
	{
		this->rigidBody = rigidbody;
		if (this->transform != NULL)
		{
			delete this->transform;
		}
		this->transform = new BulletTransformation(rigidbody);
		isBulletDependent = true;
	}

	glm::vec3 getPosition()
	{
		return this->transform->getTranslation();
	}

	glm::mat4 getGlobalMatrixRecursive()
	{
		if (!isBulletDependent)
		{
			if (this->parent != NULL)
			{
				return this->transform->getGlobalMatrix(this->parent->getGlobalMatrixRecursive());
			}
			return this->transform->getGlobalMatrix();
		}
		else
		{
			return this->transform->getMatrix();
		}
	}

	glm::mat4 getTransformMatrix()
	{
		if (!isBulletDependent)
		{
			if (this->parent != NULL)
			{
				return this->transform->getMatrix(this->parent->getGlobalMatrixRecursive());
			}
			return this->transform->getMatrix();
		}
		else
		{
			return this->transform->getMatrix();
		}
	}

	virtual void updateTransformation()
	{
	}

	void computeDFS(CameraData *data, Lightning *lights)
	{
		for (size_t i = 0, max = this->childs.size(); i < max; i++)
		{
			this->childs[i]->compute(data, lights, true);
		}
	}

	void computeBFS(CameraData *data, Lightning *lights)
	{
		std::vector<GameObject *> parcours;
		parcours.push_back(this);

		while (parcours.size() > 0)
		{
			GameObject *current = parcours[0];
			parcours.erase(parcours.begin());
			if (current != this)
			{
				current->compute(data, lights, false);
			}
			std::vector<GameObject *> currentChilds = current->getChilds();
			for (size_t i = 0, max = currentChilds.size(); i < max; i++)
			{
				parcours.push_back(currentChilds[i]);
			}
		}
	}

	virtual void compute(CameraData *data, Lightning *lights, bool dfs)
	{
		if (dfs)
		{
			this->computeDFS(data, lights);
		}
		else
		{
			this->computeBFS(data, lights);
		}
	}

	std::string getIdentifier()
	{
		return this->identifier;
	}

	// Retourne un enfant selon sont identifiant, seulement si enfant directe de l'objet courant.
	GameObject *findDirectChild(std::string identifier)
	{
		GameObject *res = NULL;
		for (size_t i = 0, max = this->childs.size(); i < max && res == NULL; i++)
		{
			if (this->childs[i]->getIdentifier().compare(identifier) == 0)
			{
				res = this->childs[i];
			}
		}
		return res;
	}

	// Retourne un enfant selon sont identifiant si l'enfant se trouve dans la g�n�alogie. Parcours DFS
	GameObject *findChild(std::string identifier)
	{
		GameObject *res = NULL;
		for (size_t i = 0, max = this->childs.size(); i < max && res == NULL; i++)
		{
			if (this->childs[i]->getIdentifier().compare(identifier) == 0)
			{
				res = this->childs[i];
			}
			else
			{
				res = this->childs[i]->findChild(identifier);
			}
		}
		return res;
	}

	GameObject *getChild(int i)
	{
		return this->childs[i];
	}

	std::vector<GameObject *> getChilds()
	{
		return this->childs;
	}

	void addChild(GameObject *child)
	{
		child->setParent(this);
		this->childs.push_back(child);
	}

	void removeChild(int index)
	{
		this->childs[index]->setParent(NULL);
		this->childs.erase(this->childs.begin() + index);
	}

	void removeChild(GameObject *obj)
	{
		size_t found = -1;
		for (size_t i = 0, max = this->childs.size(); i < max && found == -1; i++)
		{
			if (this->childs[i] == obj)
			{
				found = i;
			}
		}
		if (found != -1)
		{
			this->childs[found]->setParent(NULL);
			this->childs.erase(this->childs.begin() + found);
		}
	}

	ITransformation *getTransform()
	{
		return this->transform;
	}

	Transformation *getOriginalTransform()
	{
		return (!this->isBulletDependent ? (Transformation *)this->transform : NULL);
	}

	BulletTransformation *getBulletTransform()
	{
		return (this->isBulletDependent ? (BulletTransformation *)this->transform : NULL);
	}

	void setTransform(Transformation *transform)
	{
		this->transform = transform;
	}

	GameObject *getParent()
	{
		return this->parent;
	}

	void setParent(GameObject *parent)
	{
		this->parent = parent;
	}

	void addComponent(Component *component)
	{
		this->composants.push_back(component);
	}
	std::vector<Component *> getComponents()
	{
		return this->composants;
	}
	std::vector<Component *> getComponentsByType(Component::Type type)
	{
		size_t i = 0, max = this->composants.size();
		std::vector<Component *> res;
		while (i < max)
		{
			if (this->composants[i]->getType() == type)
			{
				res.push_back(this->composants[i]);
			}
			i++;
		}
		return res;
	}

	Component *getOneComponentByType(Component::Type type)
	{
		size_t i = 0, max = this->composants.size();
		Component *res = NULL;
		while (i < max && res == NULL)
		{
			if (this->composants[i]->getType() == type)
			{
				res = this->composants[i];
			}
			i++;
		}
		return res;
	}

	bool hasOneComponentByType(Component::Type type)
	{
		size_t i = 0, max = this->composants.size();
		bool res = false;
		while (i < max && !res)
		{
			if (this->composants[i]->getType() == type)
			{
				res = true;
			}
			i++;
		}
		return res;
	}

	std::vector<Component *> getAllComponentsByTypeRecursive(Component::Type type)
	{
		std::vector<Component *> res = this->getComponentsByType(type);
		for (size_t i = 0, max = this->childs.size(); i < max; i++)
		{
			std::vector<Component *> tmp = this->childs[i]->getAllComponentsByTypeRecursive(type);
			res.insert(res.end(), tmp.begin(), tmp.end());
		}
		return res;
	}

	std::vector<GameObject *> getAllGameObjectByComponentType(Component::Type type)
	{
		std::vector<GameObject *> res;
		if (hasOneComponentByType(type))
		{
			res.push_back(this);
		}
		for (size_t i = 0, max = this->childs.size(); i < max; i++)
		{
			std::vector<GameObject *> tmp = this->childs[i]->getAllGameObjectByComponentType(type);
			res.insert(res.end(), tmp.begin(), tmp.end());
		}
		return res;
	}

	BulletRigidbody *getRigidBody()
	{
		return this->rigidBody;
	}
};

#endif