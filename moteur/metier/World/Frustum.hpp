#ifndef __FRUSTUM_HPP__
#define __FRUSTUM_HPP__

#include <vector>;
#include <glm/glm.hpp>
#include <Collision/Type/BoundingBox.hpp>
#include <Collision/Type/SphereCollision.hpp>

using namespace glm;
class Frustum {
	mat4 matrix;
public:
	Frustum() {

	}

	void update(mat4 proj, mat4 view) {
		this->matrix = proj * view;
	}

	bool isVisible(vec3 a) {
		vec4 p = this->matrix * vec4(a, 1.0f);
		return	abs(p.x) < p.w &&
			abs(p.y) < p.w &&
			p.z > 0 &&
			p.z < p.w;

	}

	bool isVisible(BoundingBox* a) {
		bool result = false;
		std::vector<vec3> coords = a->getCoords();
		for (size_t i = 0, max = coords.size(); i < max && !result; i++) {
			result = isVisible(coords[i]);
		}
		return result;
	}

	bool isVisible(SphereCollision* a) {
		vec4 center = vec4(a->getCenter(), 1.0f);
		float rayon = a->getRadius();

		float w_rayon = center.w + rayon;

		return abs(center.x) < w_rayon &&
			abs(center.y) < w_rayon &&
			center.z > -rayon &&
			center.z < w_rayon;
	}
};

#endif // !__FRUSTUM_HPP__


