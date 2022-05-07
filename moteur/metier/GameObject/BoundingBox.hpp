#ifndef __BOUNDING_BOX_HPP__
#define __BOUNDING_BOX_HPP__

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cmath>

class BoundingBox
{
private:
	glm::mat4 transformation;

	glm::vec3 max;
	glm::vec3 min;
	glm::vec3 transformedMax;
	glm::vec3 transformedMin;
	glm::vec3 position;

public:
	BoundingBox(std::vector<glm::vec3> indexed_vertices)
	{
		if (indexed_vertices.size() > 0)
		{
			this->max = (glm::vec3)indexed_vertices[0];
			this->min = (glm::vec3)indexed_vertices[0];
		}
		for (size_t i = 1, max = indexed_vertices.size(); i < max; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (this->max[j] < indexed_vertices[i][j])
				{
					this->max[j] = indexed_vertices[i][j];
				}
				if (this->min[j] > indexed_vertices[i][j])
				{
					this->min[j] = indexed_vertices[i][j];
				}
			}
		}

		this->transformedMax = this->max;
		this->transformedMin = this->min;
	}

	glm::vec3 getSize()
	{
		return glm::vec3(abs(this->max.x - this->min.x), abs(this->max.y - this->min.y), abs(this->max.z - this->min.z));
	}

	void applyTransformation(glm::mat4 t)
	{
		bool update = false;
		for (int i = 0; i < 4 && !update; i++)
		{
			for (int j = 0; j < 4 && !update; j++)
			{
				if (this->transformation[i][j] != t[i][j])
				{
					this->transformation[i][j] = t[i][j];
					update = true;
				}
			}
		}
		if (update)
		{
			computeTransformedMinMax();
		}
	}

	glm::vec3 getMax()
	{
		return this->transformedMax;
	}

	glm::vec3 getMin()
	{
		return this->transformedMin;
	}

	bool isPointIn(vec3 point)
	{
		vec3 max = getMax();
		vec3 min = getMin();
		return point.x <= max.x && point.y <= max.y && point.z <= max.z &&
			   point.x >= min.x && min.y >= max.y && min.z >= max.z;
	}

	float inCollision(glm::vec3 v)
	{
		float res = -1.0f;
		glm::vec3 p = v - this->getMin();
		glm::vec3 max = this->getMax() - this->getMin();

		if (glm::all(glm::lessThanEqual(p, max)) && glm::all(glm::greaterThanEqual(p, vec3(0))))
		{
			glm::vec3 pmax = max - p;
			float tmp = p[0], tmpMax = pmax[0];
			for (int i = 0; i < 3; i++)
			{
				res = glm::max(glm::min(p[i], pmax[i]), res);
			}
		}

		return res + 0.001f;
	}

	glm::vec3 getCenter()
	{
		return getMin() + (getSize() / 2.0f);
	}

	std::vector<glm::vec3> getCoords()
	{
		std::vector<glm::vec3> coords(8);
		glm::vec3 size = getSize();
		for (int i = 0; i < 8; i++)
		{
			coords[i] = min;
		}
		coords[1].x += size.x;

		coords[2].x += size.x;
		coords[2].z += size.z;

		coords[3].z += size.z;

		coords[4].x += size.x;
		coords[4].y += size.y;

		coords[5].x += size.x;
		coords[5].y += size.y;
		coords[5].z += size.z;

		coords[6].y += size.y;
		coords[6].z += size.z;

		coords[7].y += size.y;

		for (int i = 0; i < 8; i++)
		{
			glm::vec4 tmpCoord = this->transformation * glm::vec4(coords[i], 1.0f);
			coords[i].x = tmpCoord.x;
			coords[i].y = tmpCoord.y;
			coords[i].z = tmpCoord.z;
		}
		return coords;
	}

private:
	void computeTransformedMinMax()
	{
		std::vector<glm::vec3> coords = getCoords();
		if (coords.size() > 0)
		{
			this->transformedMin = coords[0];
			this->transformedMax = coords[0];

			for (size_t i = 1, max = coords.size(); i < max; i++)
			{
				if (coords[i].x < transformedMin.x)
				{
					transformedMin.x = coords[i].x;
				}
				if (coords[i].y < transformedMin.y)
				{
					transformedMin.y = coords[i].y;
				}
				if (coords[i].z < transformedMin.z)
				{
					transformedMin.z = coords[i].z;
				}
				if (coords[i].x > transformedMax.x)
				{
					transformedMax.x = coords[i].x;
				}
				if (coords[i].y > transformedMax.y)
				{
					transformedMax.y = coords[i].y;
				}
				if (coords[i].z > transformedMax.z)
				{
					transformedMax.z = coords[i].z;
				}
			}
		}
	}
};

#endif