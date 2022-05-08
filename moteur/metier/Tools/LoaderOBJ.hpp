#ifndef __LOADER_OBJ_HPP__
#define __LOADER_OBJ_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

using namespace std;

void splitString(const std::string &str, std::vector<std::string> &output, char separator)
{
    std::string::size_type start = 0;
    std::string::size_type last = str.find_first_of(separator);

    while (last != std::string::npos)
    {
        if (last > start)
        {
            output.push_back(str.substr(start, last - start));
        }
        start = ++last;
        last = str.find_first_of(separator, last);
    }
    output.push_back(str.substr(start));
}

class LoaderOBJ
{
public:
    struct ObjModelInfo
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textures;
        std::vector<unsigned int> faces;
    };

    static ObjModelInfo load(std::string path_file)
    {
        ObjModelInfo res;

        // Open File
        ifstream fileOBJ(path_file);
        if (fileOBJ.fail())
        {
            cout << "OBJ File does not exist!" << endl;
        }

        // Init Reader
        std::string readLine;

        std::vector<glm::vec2> tmpOrderTextures;
        std::vector<glm::vec3> tmpOrderNormals;
        std::vector<glm::vec3> tmpVertex;

        // Read file
        while (getline(fileOBJ, readLine))
        {
            // Get First Word
            std::string input = readLine.c_str();
            std::vector<std::string> allWordsLine;
            splitString(input, allWordsLine, (char)0x20);

            // What am i reading?
            if (allWordsLine[0].compare("v") == 0)
            {
                tmpVertex.push_back(glm::vec3(stof(allWordsLine[1]), stof(allWordsLine[2]), stof(allWordsLine[3])));
            }
            if (allWordsLine[0].compare("vn") == 0)
            {
                tmpOrderNormals.push_back(glm::vec3(stof(allWordsLine[1]), stof(allWordsLine[2]), stof(allWordsLine[3])));
            }
            if (allWordsLine[0].compare("vt") == 0)
            {
                tmpOrderTextures.push_back(glm::vec2(stof(allWordsLine[1]), stof(allWordsLine[2])));
            }
            if (allWordsLine[0].compare("f") == 0)
            {
                // Triangle Faces
                if (allWordsLine.size() == 4)
                { // 4 car je compte le "f" en plus
                    for (int i = 1; i < 4; i++)
                    {
                        std::vector<std::string> tmpString;
                        splitString(allWordsLine[i], tmpString, (char)0x2f);

                        res.faces.push_back(std::stoi(tmpString[0]) - 1);
                        res.textures.push_back(tmpOrderTextures[std::stoi(tmpString[1]) - 1]);
                        res.normals.push_back(tmpOrderNormals[std::stoi(tmpString[2]) - 1]);
                    }
                }
                // Quad Faces
                if (allWordsLine.size() == 5)
                { // 5 car je compte le "f" en plus
                    unsigned int faceTmp[4];
                    /*
                    std::vector<glm::vec3> *normalTmp = new std::vector<glm::vec3>[res.vertices.size()];
                    std::vector<glm::vec2> *textureTmp = new std::vector<glm::vec2>[res.vertices.size()];
                    */
                    for (size_t i = 1; i < 5; i++)
                    {
                        std::vector<std::string> tmpString;
                        splitString(allWordsLine[i], tmpString, (char)0x2f);

                        faceTmp[i - 1] = std::stoi(tmpString[0]) - 1;
                        res.vertices.push_back(tmpVertex[faceTmp[i - 1]]);
                        res.textures.push_back(tmpOrderTextures[std::stoi(tmpString[1]) - 1]);
                        res.normals.push_back(tmpOrderNormals[std::stoi(tmpString[2]) - 1]);
                    }
                    /*
                    for (size_t i = 0, maxSizeI = tmpVertex.size(); i < maxSizeI; i++)
                    {
                        for (size_t j = 0, maxSizeJ = normalTmp[i].size(); j < maxSizeJ; j++)
                        {
                            res.normals.push_back(normalTmp[i][j]);
                        }
                        for (size_t j = 0, maxSizeJ = textureTmp[i].size(); j < maxSizeJ; j++)
                        {
                            res.textures.push_back(textureTmp[i][j]);
                        }
                    }
                    */
                    res.faces.push_back(faceTmp[0]);
                    res.faces.push_back(faceTmp[1]);
                    res.faces.push_back(faceTmp[2]);
                    res.faces.push_back(faceTmp[0]);
                    res.faces.push_back(faceTmp[2]);
                    res.faces.push_back(faceTmp[3]);
                }
            }
        }

        // Close file
        fileOBJ.close();

        // DEBUG

        // Print Vertex ("v X Y Z")
        for (size_t i = 0, max = res.vertices.size(); i < max; i++)
        {
            std::cout << "v " << res.vertices[i].x << " " << res.vertices[i].y << " " << res.vertices[i].z << std::endl;
        }
        // Print Texture ("vt X Y")
        for (size_t i = 0, max = res.textures.size(); i < max; i++)
        {
            std::cout << "vt " << res.textures[i].x << " " << res.textures[i].y << std::endl;
        }
        // Print Normal ("vn X Y Z")
        for (size_t i = 0, max = res.normals.size(); i < max; i++)
        {
            std::cout << "vn " << res.normals[i].x << " " << res.normals[i].y << " " << res.normals[i].z << std::endl;
        }
        // Print Triangle ("f v1 v2 v3")
        for (size_t i = 0, max = res.faces.size(); i < max; i += 3)
        {
            std::cout << "f " << res.faces[i] << " " << res.faces[i + 1] << " " << res.faces[i + 2] << std::endl;
        }

        return res;
    }
};

#endif;