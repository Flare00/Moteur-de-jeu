#ifndef __PRIMITIVE_MESH_HPP__
#define __PRIMITIVE_MESH_HPP__
#define _USE_MATH_DEFINES

#include <math.h>
#include <glm/glm.hpp>
#include <vector>

#include <GameObject/ModeleComponent.hpp>

class PrimitiveMesh
{
public:
	static void generate_plane(ModeleComponent *modele, int res_x, int res_y, float size_x, float size_y)
	{
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		std::vector<glm::vec2> texCoords;

		for (int i = 0; i < res_x; i++)
		{
			float pointX = (float)i * (size_x / (float)res_x);
			for (int j = 0; j < res_y; j++)
			{
				float pointY = (float)j * (size_y / (float)res_y);
				indexed_vertices.push_back(glm::vec3(pointX - (size_x / 2.0), 0.0f, pointY - (size_y / 2.0)));
				normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
				texCoords.push_back(glm::vec2(((float)i) / ((float)res_x - 1), ((float)j) / ((float)res_y - 1)));
			}
		}

		for (int i = 0; i < res_x - 1; i++)
		{
			for (int j = 0; j < res_y - 1; j++)
			{
				indices.push_back((i * res_y) + j);
				indices.push_back((i * res_y) + j + 1);
				indices.push_back(((i + 1) * res_y) + j);

				indices.push_back((i * res_y) + j + 1);
				indices.push_back(((i + 1) * res_y) + j + 1);
				indices.push_back(((i + 1) * res_y) + j);
			}
		}

		modele->setData(indexed_vertices, indices, texCoords, normals);
	}

	static void generate_plane_terrain(ModeleComponent *modele, int res_x, int res_y, float size_x, float size_y)
	{
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		std::vector<glm::vec2> texCoords;

		float subSizeX = (size_x / (float)res_x);
		float subSizeY = (size_y / (float)res_y);
		float centerX = size_x / 2.0f;
		float centerY = size_y / 2.0f;

		for (int i = 0; i < res_x; i++)
		{
			float pointX = subSizeX * i;
			float pointX_1 = subSizeX * (i + 1);
			for (int j = 0; j < res_y; j++)
			{
				float pointY = subSizeY * j;
				float pointY_1 = subSizeY * (j + 1);

				indexed_vertices.push_back(glm::vec3(pointX - centerX, 0.0f, pointY - centerY));
				indexed_vertices.push_back(glm::vec3(pointX_1 - centerX, 0.0f, pointY - centerY));
				indexed_vertices.push_back(glm::vec3(pointX - centerX, 0.0f, pointY_1 - centerY));
				indexed_vertices.push_back(glm::vec3(pointX_1 - centerX, 0.0f, pointY_1 - centerY));

				texCoords.push_back(glm::vec2(i / (float)res_x, j / (float)res_y));
				texCoords.push_back(glm::vec2((i + 1) / (float)res_x, j / (float)res_y));
				texCoords.push_back(glm::vec2(i / (float)res_x, (j + 1) / (float)res_y));
				texCoords.push_back(glm::vec2((i + 1) / (float)res_x, (j + 1) / (float)res_y));

				normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
				normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
				normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
				normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
			}
		}

		for (size_t i = 0, max = indexed_vertices.size(); i < max; i += 4)
		{
			indices.push_back((unsigned int)i);
			indices.push_back((unsigned int)i + 1);
			indices.push_back((unsigned int)i + 2);
			indices.push_back((unsigned int)i + 3);
		}
		modele->setData(indexed_vertices, indices, texCoords, normals);
	}

	static void generate_plane_terrain_test(ModeleComponent *modele, float size_x, float size_y)
	{
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> texCoords;
		float halfSizeX = size_x / 2;
		float halfSizeY = size_y / 2;

		indexed_vertices.push_back(glm::vec3(-halfSizeX, 0.0f, halfSizeY));
		indexed_vertices.push_back(glm::vec3(halfSizeX, 0.0f, halfSizeY));
		indexed_vertices.push_back(glm::vec3(halfSizeX, 0.0f, -halfSizeY));
		indexed_vertices.push_back(glm::vec3(-halfSizeX, 0.0f, -halfSizeY));

		texCoords.push_back(glm::vec2(0, 0));
		texCoords.push_back(glm::vec2(1, 0));
		texCoords.push_back(glm::vec2(0, 1));
		texCoords.push_back(glm::vec2(1, 1));

		normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
		normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
		normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));
		normals.push_back(glm::normalize(glm::vec3(0, 1, 0)));

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		modele->setData(indexed_vertices, indices, texCoords, normals);
	}

	static void generate_plane_terrain_collision(ModeleComponent *modele, int res_x, int res_y, float size_x, float size_y)
	{
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		std::vector<glm::vec2> texCoords;

		float subSizeX = (size_x / (float)res_x);
		float subSizeY = (size_x / (float)res_x);
		float centerX = size_x / 2.0f;
		float centerY = size_y / 2.0f;

		for (int i = 0; i <= res_x; i++)
		{
			float pointX = subSizeX * i;
			for (int j = 0; j <= res_y; j++)
			{
				float pointY = subSizeY * j;
				indexed_vertices.push_back(glm::vec3(pointX - centerX, 0.0f, pointY - centerY));
				texCoords.push_back(glm::vec2(i / (float)res_x, j / (float)res_y));
			}
		}
		modele->setData(indexed_vertices, texCoords);
	}

	static void generate_uv_sphere(ModeleComponent *modele, int nbMeridien, int nbParalleles, float scale = 1.0f)
	{
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> texCoords;
		for (int j = 0; j <= nbParalleles; j++)
		{
			float parallele = (float)(M_PI * (float)j) / ((float)nbParalleles);
			for (int i = 0; i <= nbMeridien; i++)
			{
				// Coordinate
				float meridien = (float)(2.0f * M_PI * (float)i) / ((float)nbMeridien);
				float x = sin(parallele) * cos(meridien);
				float y = cos(parallele);
				float z = sin(parallele) * sin(meridien);
				indexed_vertices.push_back(glm::vec3(x, y, z) * scale);
				normals.push_back(glm::normalize(glm::vec3(x, y, z)));

				// Texture Coordinate
				texCoords.push_back(glm::vec2(meridien / (2.0f * M_PI), parallele / (M_PI)));

				if (j < nbParalleles && i < nbMeridien)
				{
					// Triangles
					int p1 = (j * (nbMeridien + 1)) + i;
					int p2 = p1 + (nbMeridien + 1);

					indices.push_back(p1);
					indices.push_back(p1 + 1);
					indices.push_back(p2);

					indices.push_back(p1 + 1);
					indices.push_back(p2 + 1);
					indices.push_back(p2);
				}
			}
		}
		modele->setData(indexed_vertices, indices, texCoords, normals);
	}

	static void generate_cube(ModeleComponent *modele)
	{
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		// Vertices
		indexed_vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
		indexed_vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

		// Textures
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));

		// Normals
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(-1, 0, 0));
		normals.push_back(glm::vec3(-1, 0, 0));
		normals.push_back(glm::vec3(-1, 0, 0));
		normals.push_back(glm::vec3(-1, 0, 0));
		normals.push_back(glm::vec3(0, -1, 0));
		normals.push_back(glm::vec3(0, -1, 0));
		normals.push_back(glm::vec3(0, -1, 0));
		normals.push_back(glm::vec3(0, -1, 0));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));

		// Indices
		indices.push_back((unsigned int)0);
		indices.push_back((unsigned int)1);
		indices.push_back((unsigned int)2);
		indices.push_back((unsigned int)0);
		indices.push_back((unsigned int)2);
		indices.push_back((unsigned int)3);
		indices.push_back((unsigned int)4);
		indices.push_back((unsigned int)5);
		indices.push_back((unsigned int)6);
		indices.push_back((unsigned int)4);
		indices.push_back((unsigned int)6);
		indices.push_back((unsigned int)7);
		indices.push_back((unsigned int)8);
		indices.push_back((unsigned int)9);
		indices.push_back((unsigned int)10);
		indices.push_back((unsigned int)8);
		indices.push_back((unsigned int)10);
		indices.push_back((unsigned int)11);
		indices.push_back((unsigned int)12);
		indices.push_back((unsigned int)13);
		indices.push_back((unsigned int)14);
		indices.push_back((unsigned int)12);
		indices.push_back((unsigned int)14);
		indices.push_back((unsigned int)15);
		indices.push_back((unsigned int)16);
		indices.push_back((unsigned int)17);
		indices.push_back((unsigned int)18);
		indices.push_back((unsigned int)16);
		indices.push_back((unsigned int)18);
		indices.push_back((unsigned int)19);
		indices.push_back((unsigned int)20);
		indices.push_back((unsigned int)21);
		indices.push_back((unsigned int)22);
		indices.push_back((unsigned int)20);
		indices.push_back((unsigned int)22);
		indices.push_back((unsigned int)23);

		modele->setData(indexed_vertices, indices, texCoords, normals);
	}
};

#endif // !_PRIMITIVE_MESH_H__
