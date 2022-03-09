#ifndef _COMPRESSEUR_H_
#define _COMPRESSEUR_H_

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "./GameObject/Modele.hpp"

class Compresseur{
public:
	static float quantification(float c, float range, float qp, float BB_min_c){
		return (((c-BB_min_c) * pow(2, qp)) / range);
	}
	static float dequantification(float c, float range, float qp, float BB_min_c){
		return ((c *  range) / pow(2, qp))  + BB_min_c;
	}


	Compresseur(){}

	static Modele * compresser(Modele * m, float qp){
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec3> indexed_verticesModele = m->getIndexedVertices();
		BoundingBox * bb = m->getBoundingBox();
		for(int i = 0, max = indexed_verticesModele.size(); i < max; i++){
			glm::vec3 coord = indexed_verticesModele[i];
			indexed_vertices.push_back(
				glm::vec3(
					Compresseur::quantification(coord.x, bb->getSize().x, qp, bb->getMin().x),
					Compresseur::quantification(coord.y, bb->getSize().y, qp, bb->getMin().y),
					Compresseur::quantification(coord.z, bb->getSize().z, qp, bb->getMin().z)
				)
			);
		}
		//std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GLuint programID, GameObject* parent = NULL
		return new Modele(m->getIdentifier() + "_Compressed", indexed_vertices, m->getIndices(), m->getTexCoords(), (GlobalShader*) m->getShader(), m->getParent());
	}

	static Modele* decompresser(Modele* m, float qp) {
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec3> indexed_verticesModele = m->getIndexedVertices();
		BoundingBox* bb = m->getBoundingBox();
		for (int i = 0, max = indexed_verticesModele.size(); i < max; i++) {
			glm::vec3 coord = indexed_verticesModele[i];
			indexed_vertices.push_back(
				glm::vec3(
					Compresseur::dequantification(coord.x, bb->getSize().x, qp, bb->getMin().x),
					Compresseur::dequantification(coord.y, bb->getSize().y, qp, bb->getMin().y),
					Compresseur::dequantification(coord.z, bb->getSize().z, qp, bb->getMin().z)
				)
			);
		}
		//std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GLuint programID, GameObject* parent = NULL
		return new Modele(m->getIdentifier() + "_Uncompressed", indexed_vertices, m->getIndices(), m->getTexCoords(), (GlobalShader*) m->getShader(), m->getParent());
	}

};

#endif