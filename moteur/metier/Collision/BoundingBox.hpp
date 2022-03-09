#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cmath>

#include "../GameObject/Modele.hpp"

class BoundingBox{
private:
	glm::vec3 max;
	glm::vec3 min;
public:
	BoundingBox(std::vector<glm::vec3> indexed_vertices){
		if(indexed_vertices.size() > 0){
			this->max = indexed_vertices[0];
			this->min = indexed_vertices[0];
		}
		for(int i = 1, max = indexed_vertices.size(); i < max; i++){
			for(int j = 0; j < 3; j++){
				if(this->max[j] < indexed_vertices[i][j]){
					this->max[j] = indexed_vertices[i][j];
				}
				if(this->min[j] > indexed_vertices[i][j]){
					this->min[j] = indexed_vertices[i][j];
				}
			}
		}
	}

	glm::vec3 getSize(){
		return glm::vec3(abs(this->max.x - this->min.x), abs(this->max.y - this->min.y),abs(this->max.z - this->min.z));
	}

	glm::vec3 getMax(){
		return this->max;
	}
	
	glm::vec3 getMin(){
		return this->min;
	}
	

};

#endif