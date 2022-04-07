#ifndef __SIMPLIFY_MESH_H__
#define __SIMPLIFY_MESH_H__

#include <glm/glm.hpp>
#include <vector>

class CubeSimplify
{
private:
    glm::vec3 c_vecMin;
    glm::vec3 c_vecMax;
    glm::vec3 c_coords;
    glm::vec3 c_normal;
    glm::vec2 c_uv;
    bool gotR = false;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<int> pointsId;

public:
    // Construct && Destruct
    CubeSimplify(glm::vec3 min, glm::vec3 max) : c_vecMin(min), c_vecMax(max) {}
    ~CubeSimplify() {}

    // Getters
    glm::vec3 getRcoords()
    {
        return this->c_coords;
    }
    glm::vec3 getRnormal()
    {
        return this->c_normal;
    }
    glm::vec2 getRuv()
    {
        return this->c_uv;
    }
    bool getIsSet()
    {
        return this->gotR;
    }

    // Method
    bool pointInCube(const glm::vec3 &p)
    {
        if (p.x > this->c_vecMin.x && p.x < this->c_vecMax.x &&
            p.y > this->c_vecMin.y && p.y < this->c_vecMax.y &&
            p.z > this->c_vecMin.z && p.z < this->c_vecMax.z)
        {
            return true;
        }
        return false;
    }

    bool isIdIn(int id)
    {
        bool res = false;
        for (int i = 0, max = this->pointsId.size(); i < max && !res; i++)
        {
            if (this->pointsId[i] == id)
            {
                res = true;
            }
        }
        return res;
    }

    bool addVerticeIfIn(glm::vec3 v, glm::vec3 n, glm::vec2 uv, int id)
    {
        if (pointInCube(v))
        {
            points.push_back(v);
            normals.push_back(n);
            texCoords.push_back(uv);
            pointsId.push_back(id);
            return true;
        }
        return false;
    }

    void calculRepresentant()
    {
        int nbPoints = points.size();
        if (nbPoints > 0)
        {
            glm::vec3 moyenne(0);
            glm::vec3 moyNorm(0);
            glm::vec2 moyUV(0);
            for (int i = 0; i < nbPoints; i++)
            {
                moyenne += points[i];
                moyNorm += normals[i];
                moyUV += texCoords[i];
            }
            moyenne /= nbPoints;
            moyNorm /= nbPoints;
            moyUV /= nbPoints;
            moyNorm = glm::normalize(moyNorm);
            this->c_coords = moyenne;
            this->c_normal = moyNorm;
            this->c_uv = moyUV;
            this->gotR = true;
        }
    }
};

Modele *simplify(Modele *m, int gridResolution)
{
    // Init Model info
    glm::vec3 m_Min = m->getRigidBody()->getCollision()->getBoundingBox()->getMin() - glm::vec3(0.0001f);
    glm::vec3 m_Max = m->getRigidBody()->getCollision()->getBoundingBox()->getMax() + glm::vec3(0.0001f);
    glm::vec3 range = m->getRigidBody()->getCollision()->getBoundingBox()->getSize();

    // Cube Generation
    std::vector<CubeSimplify> cubeDecimation;
    float xStep = (m_Max.x - m_Min.x) / gridResolution;
    float yStep = (m_Max.y - m_Min.y) / gridResolution;
    float zStep = (m_Max.z - m_Min.z) / gridResolution;

    for (int res_x = 0; res_x < gridResolution - 1; res_x++)
    {
        float minX = m_Min.x + (xStep * res_x);
        float maxX = m_Max.x + (xStep * (res_x + 1));
        for (int res_y = 0; res_y < gridResolution - 1; res_y++)
        {
            float minY = m_Min.y + (yStep * res_y);
            float maxY = m_Max.y + (yStep * (res_y + 1));
            for (int res_z = 0; res_z < gridResolution - 1; res_z++)
            {
                float minZ = m_Min.z + (zStep * res_z);
                float maxZ = m_Max.z + (zStep * (res_z + 1));
                cubeDecimation.push_back(CubeSimplify(glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ)));
            }
        }
    }

    // Count vertices in cube
    std::vector<int> corresp(m->getIndexedVertices().size());

    for (int v = 0, maxVert = m->getIndexedVertices().size(); v < maxVert; v++)
    {
        bool foundCube = false;
        std::vector<glm::vec3> v_model = m->getIndexedVertices();
        std::vector<glm::vec3> n_model = m->getNormals();
        std::vector<glm::vec2> uv_model = m->getTexCoords();
        
        for (int c = 0, maxCube = cubeDecimation.size(); c < maxCube && !foundCube; c++)
        {
            if (cubeDecimation[c].addVerticeIfIn(v_model[v], n_model[v], uv_model[v], v))
            {
                corresp[v] = c;
            }
        }
    }

    // Corresp alignement
    for (int i = 0, maxSize = corresp.size(); i < maxSize; i++)
    {
        int next = 0;
        for (int j = 0; j < maxSize && (next > i); j++)
        {
            if ((i - 1) != corresp[j] && next > corresp[j])
            {
                next = corresp[j];
            }
        }
        if (next > i)
        {
            for (int j = 0; j < maxSize; j++)
            {
                if (next == corresp[j])
                {
                    corresp[j] = i;
                }
            }
        }
    }

    // Representant
    for (int c = 0, maxCube = cubeDecimation.size(); c < maxCube; c++)
    {
        cubeDecimation[c].calculRepresentant();
    }

    // Init dirty
    std::vector<glm::vec3> v_Decimate_dirty;      // tab vertices
    std::vector<glm::vec3> n_Decimate_dirty;      // tab normals
    std::vector<glm::vec2> tex_Decimate_dirty;    // tab texcoords
    std::vector<unsigned int> triangles = m->getIndices();
    std::vector<unsigned int> tri_Decimate_dirty(triangles.size()); // tab triangles

    for (int c = 0, maxCube = cubeDecimation.size(); c < maxCube; c++)
    {
        if (cubeDecimation[c].getIsSet())
        {
            v_Decimate_dirty.push_back(cubeDecimation[c].getRcoords());
            n_Decimate_dirty.push_back(cubeDecimation[c].getRnormal());
            tex_Decimate_dirty.push_back(cubeDecimation[c].getRuv());
        }
    }
    for (int i = 0; i < m->getIndices().size(); i++)
    {
        tri_Decimate_dirty[i] = triangles[corresp[m->getIndices()[i]]];
    }

    // Clean triangles
    std::vector<glm::vec3> v_Decimate_cleaned;      // tab vertices
    std::vector<glm::vec3> n_Decimate_cleaned;      // tab normals
    std::vector<glm::vec2> tex_Decimate_cleaned;    // tab texcoords
    std::vector<unsigned int> tri_Decimate_cleaned; // tab triangles

    for (int i = 0, maxSize = tri_Decimate_dirty.size(); i < maxSize; i += 3)
    {
        unsigned int tempTriangle[3];
        bool isClean = true;
        tempTriangle[0] = tri_Decimate_dirty[i];
        tempTriangle[1] = tri_Decimate_dirty[i + 1];
        tempTriangle[2] = tri_Decimate_dirty[i + 2];

        isClean = !(tempTriangle[0] == tempTriangle[1] || tempTriangle[1] == tempTriangle[2] || tempTriangle[0] == tempTriangle[2]);

        for (int j = 0, maxClean = tri_Decimate_cleaned.size(); j < maxClean && isClean; j += 3)
        {
            isClean = !((tempTriangle[0] == tri_Decimate_cleaned[0] && tempTriangle[1] == tri_Decimate_cleaned[1] && tempTriangle[2] == tri_Decimate_cleaned[2]) ||
                        (tempTriangle[1] == tri_Decimate_cleaned[0] && tempTriangle[2] == tri_Decimate_cleaned[1] && tempTriangle[0] == tri_Decimate_cleaned[2]) ||
                        (tempTriangle[2] == tri_Decimate_cleaned[0] && tempTriangle[0] == tri_Decimate_cleaned[1] && tempTriangle[1] == tri_Decimate_cleaned[2]));
        }

        if (isClean)
        {
            tri_Decimate_cleaned.push_back(tempTriangle[0]);
            tri_Decimate_cleaned.push_back(tempTriangle[1]);
            tri_Decimate_cleaned.push_back(tempTriangle[2]);
        }
    }

    // Clean Vertex
    for (int i = 0, maxSize = v_Decimate_dirty.size(); i < maxSize; i++)
    {
        unsigned int p = corresp[i];
        for (int j = 0, maxSize2 = tri_Decimate_cleaned.size(); j < maxSize2; j += 3)
        {
            if (p == tri_Decimate_cleaned[j] || p == tri_Decimate_cleaned[j+1] || p == tri_Decimate_cleaned[j+2])
            {
                v_Decimate_cleaned.push_back(v_Decimate_dirty[i]);
                n_Decimate_cleaned.push_back(n_Decimate_dirty[i]);
                tex_Decimate_cleaned.push_back(tex_Decimate_dirty[i]);
            }
        }
    }

    Modele *res = new Modele("", v_Decimate_cleaned, n_Decimate_cleaned, tri_Decimate_cleaned, tex_Decimate_cleaned, m->getShader());
    
    res->setTexture(m->getTexture(), false);

    return res;
}

#endif // !_SIMPLIFY_MESH_H__