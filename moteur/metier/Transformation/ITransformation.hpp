#ifndef __I_TRANSFORMATION_HPP__
#define __I_TRANSFORMATION_HPP__
#include <glm/glm.hpp>

class ITransformation
{
public:
	virtual ITransformation *translate(glm::vec3 t) = 0;
	virtual ITransformation *setTranslate(glm::vec3 pos) = 0;

	virtual ITransformation *rotate(glm::vec3 r) = 0;
	virtual ITransformation *setRotation(glm::vec3 r) = 0;

	virtual glm::vec3 getRotation() = 0;
	virtual glm::vec3 getTranslation() = 0;

	virtual glm::mat4 getMatrix(glm::mat4 globalParent = glm::mat4(1.0f)) = 0;
	virtual glm::mat4 getGlobalMatrix(glm::mat4 globalParent = glm::mat4(1.0f)) = 0;

	static glm::mat4 generateMatrix(glm::vec3 pos, glm::vec3 rotation = glm::vec3(0.0f), float scale = 1.0f)
	{
		glm::mat4 matrix = glm::mat4(1.0f);

		matrix = glm::scale(matrix, glm::vec3(scale));

		matrix = glm::rotate(matrix, rotation[1], vec3(0, 1, 0));
		matrix = glm::rotate(matrix, rotation[0], vec3(1, 0, 0));
		matrix = glm::rotate(matrix, rotation[2], vec3(0, 0, 1));

		matrix = glm::translate(matrix, pos);

		return matrix;
	}
};

#endif // !__I_TRANSFORMATION_HPP__
