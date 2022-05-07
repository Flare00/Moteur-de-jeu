#ifndef __FRUSTUM_HPP__
#define __FRUSTUM_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GameObject/BoundingBox.hpp>

using namespace glm;
class Frustum {
	mat4 matrix = mat4(1.0f);
	float margin = 1.0f;
	vec3 front, up, right;
public:
	class Plane {
	public:
		glm::vec3 pos;
		glm::vec3 normal;
		Plane(glm::vec3 pos, glm::vec3 normal) {
			this->pos = pos;
			this->normal = normal;
		}
	};
	Frustum() {
	}

	void init(mat4 proj, mat4 view) {
		this->matrix = proj * view;
	}

	void update(mat4 proj, mat4 view, vec3 front, vec3 up, vec3 right) {
		this->matrix = proj * view;
		this->front = front;
		this->up = up;
		this->right = right;
	}

	bool isVisible(vec3 a) {
		vec4 p = this->matrix * vec4(a, 1.0f);
		return	abs(p.x) < p.w + margin &&
			abs(p.y) < p.w + margin &&
			p.z > -margin &&
			p.z < p.w + margin;

	}

	bool isVisible(BoundingBox* a) {
		return true;
		/*bool resultat = true;
		vec3 min = a->getMin();
		vec3 max = a->getMax();

		std::vector<Plane> plan;

		plan.push_back(Plane(vec3(this->matrix[0][3] + this->matrix[0][0]), -this->right)); // left
		plan.push_back(Plane(vec3(this->matrix[0][3] - this->matrix[0][0]), this->right)); //right
		plan.push_back(Plane(vec3(this->matrix[0][3] + this->matrix[0][1]), -this->up)); //bottom
		plan.push_back(Plane(vec3(this->matrix[0][3] - this->matrix[0][1]), this->up)); //up
		plan.push_back(Plane(vec3(this->matrix[0][3] + this->matrix[0][2]), this->front)); //near
		plan.push_back(Plane(vec3(this->matrix[0][3] - this->matrix[0][2]), -this->front)); //far

		vec3 rMin, rMax;
		for (int i = 0; i < 6 && resultat; i++) {
			for (int j = 0; j < 3; j++) {
				if (plan[i].normal[j] > 0) {
					rMin[j] = min[j];
					rMax[j] = max[j];
				}
			}
			if (glm::dot(plan[i].normal, rMin) + plan[i].pos > 0) resultat = false;
			if (glm::dot(plan[i].normal, rMin) + plan[i].pos > 0) resultat = true;
		}



		return resultat;*/
	}
};

#endif // !__FRUSTUM_HPP__


