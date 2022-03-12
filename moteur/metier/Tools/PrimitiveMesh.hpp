#ifndef __PRIMITIVE_MESH_H__
#define __PRIMITIVE_MESH_H__

#include <glm/glm.hpp>
#include <vector>

#include "../GameObject/Modele.hpp"

class PrimitiveMesh {
public :
	static void generate_plane(Modele* modele, int res_x, int res_y, float size_x, float size_y) {
		std::vector<glm::vec3> indexed_vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> texCoords;

		for (int i = 0; i < res_x; i++) {
			float pointX = (float)i * (size_x / (float)res_x);
			for (int j = 0; j < res_y; j++) {
				float pointY = (float)j * (size_y / (float)res_y);
				indexed_vertices.push_back(glm::vec3(pointX - (size_x / 2.0), 0.0f, pointY - (size_y / 2.0)));
				texCoords.push_back(glm::vec2(((float)i) / ((float)res_x - 1), ((float)j) / ((float)res_y - 1)));
			}
		}


		for (int i = 0; i < res_x - 1; i++) {
			for (int j = 0; j < res_y - 1; j++) {
				indices.push_back((i * res_y) + j);
				indices.push_back((i * res_y) + j + 1);
				indices.push_back(((i + 1) * res_y) + j);

				indices.push_back((i * res_y) + j + 1);
				indices.push_back(((i + 1) * res_y) + j + 1);
				indices.push_back(((i + 1) * res_y) + j);
			}
		}

		modele->setData(indexed_vertices, indices, texCoords);
	}

	static void generate_uv_sphere(Modele* modele, int nbMeridien, int nbParalleles) {
		std::vector<glm::vec3> indexed_vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> texCoords;
		for (int j = 0; j <= nbParalleles; j++) {
			float parallele = (M_PI * (float)j) / ((float)nbParalleles);
			for (int i = 0; i <= nbMeridien; i++) {
				//Coordinate
				float meridien = (2.0f * M_PI * (float)i) / ((float)nbMeridien);
				float x = sin(parallele) * cos(meridien);
				float y = cos(parallele);
				float z = sin(parallele) * sin(meridien);
				indexed_vertices.push_back(glm::vec3(x, y, z));

				//Texture Coordinate
				texCoords.push_back(glm::vec2(meridien / (2.0f * M_PI), parallele / (M_PI)));

				if (j < nbParalleles && i < nbMeridien) {
					//Triangles
					int p1 = (j * (nbMeridien + 1)) + i;
					int p2 = p1 + (nbMeridien + 1);

					indices.push_back(p1);
					indices.push_back(p2);
					indices.push_back(p1 + 1);

					indices.push_back(p1 + 1);
					indices.push_back(p2);
					indices.push_back(p2 + 1);
				}
			}
		}
		modele->setData(indexed_vertices, indices, texCoords);
	}
};

#endif // !_PRIMITIVE_MESH_H__
