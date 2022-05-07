#ifndef __BULLET_TRANSFORMATION_HPP__
#define __BULLET_TRANSFORMATION_HPP__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Physique/BulletRigidbody.hpp>
#include <Physique/SwapGLM_BT.hpp>
#include <Transformation/ITransformation.hpp>

class BulletTransformation : public ITransformation {
protected:
	//Transformation et mouvement Bullet Physics
	BulletRigidbody* bulletRigidbody = NULL;
public:

	BulletTransformation(BulletRigidbody* bulletRigidbody) {
		this->bulletRigidbody = bulletRigidbody;
	}

	void applyImpule(glm::vec3 impulse) {
		this->bulletRigidbody->getRigidbody()->applyCentralImpulse(swapGlmBt(impulse));
	}

	void applyForce(glm::vec3 force) {
		this->bulletRigidbody->getRigidbody()->applyCentralForce(swapGlmBt(force));
	}

	BulletRigidbody* getBulletRigidbody() {
		return this->bulletRigidbody;
	}

	virtual ITransformation* translate(glm::vec3 t) {
		//TODO
		return this;
	}
	virtual ITransformation* setTranslate(glm::vec3 pos) {
		//TODO
		//this->bulletRigidbody()->
		return this;
	}

	virtual ITransformation* rotate(glm::vec3 v, float rad) {
		//TODO
		return this;
	}
	virtual ITransformation* setRotation(glm::vec3 r) {
		//TODO
		return this;
	}

	virtual glm::vec3 getRotation() {
		//TODO
		return glm::vec3();
	}
	virtual glm::vec3 getTranslation() {
		return swapGlmBt(this->bulletRigidbody->getRigidbody()->getWorldTransform().getOrigin());
	}

	virtual glm::mat4 getMatrix(glm::mat4 globalParent = glm::mat4(1.0f))
	{
		btTransform transform;
		glm::mat4 matrice;

		this->bulletRigidbody->getRigidbody()->getMotionState()->getWorldTransform(transform);
		transform.getOpenGLMatrix(glm::value_ptr(matrice));

		return matrice;
	}

	virtual glm::mat4 getGlobalMatrix(glm::mat4 globalParent = glm::mat4(1.0f)) {
		return getMatrix();
	}


};

#endif