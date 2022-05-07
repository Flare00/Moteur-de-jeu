#ifndef __BULLET_TRANSFORMATION_HPP__
#define __BULLET_TRANSFORMATION_HPP__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Physique/BulletRigidbody.hpp>
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
		this->bulletRigidbody->getRigidbody()->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
	}

	void applyForce(glm::vec3 force) {
		this->bulletRigidbody->getRigidbody()->applyCentralForce(btVector3(force.x, force.y, force.z));
	}

	BulletRigidbody* getBulletRigidbody() {
		return this->bulletRigidbody;
	}

	virtual ITransformation* translate(glm::vec3 t) {
		this->bulletRigidbody->getRigidbody()->getWorldTransform().setOrigin(this->bulletRigidbody->getRigidbody()->getWorldTransform().getOrigin() + btVector3(t.x, t.y, t.z));
		return this;
	}
	virtual ITransformation* setTranslate(glm::vec3 pos) {
		this->bulletRigidbody->getRigidbody()->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
		return this;
	}

	virtual ITransformation* rotate(glm::vec3 r) {
		r += getRotation();
		btQuaternion quatBt;
		quatBt.setEulerZYX(r.z, r.y, r.x);		
		this->bulletRigidbody->getRigidbody()->getWorldTransform().setRotation(quatBt);
		return this;
	}
	virtual ITransformation* setRotation(glm::vec3 r) {
		btQuaternion quatBt;
		quatBt.setEulerZYX(r.z, r.y, r.x);
		this->bulletRigidbody->getRigidbody()->getWorldTransform().setRotation(quatBt);
		return this;
	}

	virtual glm::vec3 getRotation() {
		btScalar z, y, x;
		this->bulletRigidbody->getRigidbody()->getWorldTransform().getRotation().getEulerZYX(z, y, x);
		return glm::vec3(x, y, z);
	}
	virtual glm::vec3 getTranslation() {
		btVector3 trans = this->bulletRigidbody->getRigidbody()->getWorldTransform().getOrigin();
		return glm::vec3(trans.getX(), trans.getY(), trans.getZ());
	}

	virtual glm::mat4 getMatrix(glm::mat4 globalParent = glm::mat4(1.0f))
	{
		btTransform transform;
		glm::mat4 matrice;

		transform = this->bulletRigidbody->getRigidbody()->getWorldTransform();
		transform.getOpenGLMatrix(glm::value_ptr(matrice));

		return matrice;
	}

	virtual glm::mat4 getGlobalMatrix(glm::mat4 globalParent = glm::mat4(1.0f)) {
		return getMatrix();
	}


};

#endif