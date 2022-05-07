#ifndef __I_TRANSFORMATION_HPP__
#define __I_TRANSFORMATION_HPP__
#include <glm/glm.hpp>

class ITransformation {
public:
	
	virtual ITransformation* translate(glm::vec3 t) = 0;
	virtual ITransformation* setTranslate(glm::vec3 pos) = 0;

	virtual ITransformation* rotate(glm::vec3 v, float rad) = 0;
	virtual ITransformation* setRotation(glm::vec3 r) = 0;

	virtual glm::vec3 getRotation() = 0;
	virtual glm::vec3 getTranslation() = 0;

	virtual glm::mat4 getMatrix(glm::mat4 globalParent = glm::mat4(1.0f)) = 0;
	virtual glm::mat4 getGlobalMatrix(glm::mat4 globalParent = glm::mat4(1.0f)) = 0;
};

#endif // !__I_TRANSFORMATION_HPP__
