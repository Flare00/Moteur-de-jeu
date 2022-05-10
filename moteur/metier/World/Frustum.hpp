#ifndef __FRUSTUM_HPP__
#define __FRUSTUM_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GameObject/BoundingBox.hpp>

using namespace glm;
class Frustum
{
public:
	class Plane
	{
	public:
		vec3 normal;
		vec3 point;
		Plane(glm::vec3 normal, vec3 point)
		{
			this->normal = glm::normalize(normal);
			this->point = point;
		}
	};

private:
	mat4 matrix = mat4(1.0f);
	float margin = 1.0f;
	vec3 front, up, right;

	float fov, aspect, size, zNear, zFar, halfVSide, halfHSide;
	std::vector<Plane> plan;
	bool ortho = false;

public:
	Frustum()
	{
	}

	void init(mat4 proj, mat4 view, float fov, float aspect, float zNear, float zFar)
	{
		this->matrix = proj * view;
		this->fov = fov;
		this->aspect = aspect;
		this->zNear = zNear;
		this->zFar = zFar;
		this->halfHSide = this->zFar * (float)glm::tan(this->fov * 0.5);
		this->halfVSide = halfHSide * aspect;
	}

	void init(mat4 proj, mat4 view, float size, float zNear, float zFar)
	{
		this->matrix = proj * view;
		this->size = size;
		this->zNear = zNear;
		this->zFar = zFar;
		this->halfHSide = this->zFar * (float)glm::tan(this->fov * 0.5);
		this->halfVSide = halfHSide * aspect;
		ortho = true;
	}

	void update(mat4 proj, mat4 view, vec3 front, vec3 up, vec3 right, vec3 position)
	{
		this->matrix = proj * view;
		this->front = front;
		this->up = up;
		this->right = right;
		if (!ortho) {
			plan.clear();

			plan.push_back(Plane(cross((zFar * front) - (right * halfHSide), up), position)); // Left
			plan.push_back(Plane(cross(up, (zFar * front) + (right * halfHSide)), position)); // Right

			plan.push_back(Plane(cross((zFar * front) + (up * halfVSide), right), position)); // Bottom
			plan.push_back(Plane(cross(right, (zFar * front) - (up * halfVSide)), position)); // Top

			plan.push_back(Plane(front, position + (front * zNear))); // Near
			plan.push_back(Plane(-front, position + (front * zFar))); // Far
		}
	}

	bool isVisible(vec3 a)
	{
		vec4 p = this->matrix * vec4(a, 1.0f);
		return abs(p.x) < p.w + margin &&
			abs(p.y) < p.w + margin &&
			p.z > -margin &&
			p.z < p.w + margin;
	}

	bool isVisible(BoundingBox* a)
	{
		bool resultat = true;

		if (!ortho) {
			vec3 min = a->getMin();
			vec3 max = a->getMax();

			vec3 rMin, rMax;

			for (int i = 0; i < 6 && resultat; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (plan[i].normal[j] > 0)
					{
						rMin[j] = min[j];
						rMax[j] = max[j];
					}
					else
					{
						rMin[j] = max[j];
						rMax[j] = min[j];
					}
				}

				if (i % 2 == 0)
				{
					if (glm::dot(plan[i].normal, rMax - plan[i].point) < 0)
					{
						resultat = false;
					}
				}
				else
				{
					if (glm::dot(plan[i].normal, rMax - plan[i].point) < 0)
					{
						resultat = false;
					}
				}
			}
		}
		else {
			//Todo OBB to AABB
		}
		return resultat;
	}


};

#endif // !__FRUSTUM_HPP__
