#ifndef __LOADER_OBJ_HPP__
#define __LOADER_OBJ_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

using namespace std;

void splitString(const std::string &str, std::vector<std::string> &output)
{
    std::string::size_type start = 0;
    std::string::size_type last = str.find_first_of(" ");

    while (last != std::string::npos)
    {
        if (last > start)
        {
            output.push_back(str.substr(start, last - start));
        }
        start = ++last;
        last = str.find_first_of(" ", last);
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

        // Read file
        while (getline(fileOBJ, readLine))
        {
            // Get First Word
            std::string input = readLine.c_str();
            std::vector<std::string> allWordsLine;
            splitString(input, allWordsLine);

            // What am i reading?
            if (allWordsLine[0].compare("v") == 0)
            {
                res.vertices.push_back(glm::vec3(stof(allWordsLine[1]), stof(allWordsLine[2]), stof(allWordsLine[3])));
            }
            if (allWordsLine[0].compare("vn") == 0)
            {
                res.normals.push_back(glm::vec3(stof(allWordsLine[1]), stof(allWordsLine[2]), stof(allWordsLine[3])));
            }
            if (allWordsLine[0].compare("vt") == 0)
            {
                res.textures.push_back(glm::vec2(stof(allWordsLine[1]), stof(allWordsLine[2])));
            }
            if (allWordsLine[0].compare("f") == 0)
            {
                // Triangle Faces
                if (allWordsLine.size() == 4)
                { // 4 car je compte le "f" en plus
                    res.faces.push_back((unsigned int)(allWordsLine[1][0] - '0'));
                    res.faces.push_back((unsigned int)(allWordsLine[2][0] - '0'));
                    res.faces.push_back((unsigned int)(allWordsLine[3][0] - '0'));
                }
                // Quad Faces
                if (allWordsLine.size() == 5)
                { // 5 car je compte le "f" en plus
                    res.faces.push_back((unsigned int)(allWordsLine[1][0] - '0'));
                    res.faces.push_back((unsigned int)(allWordsLine[2][0] - '0'));
                    res.faces.push_back((unsigned int)(allWordsLine[3][0] - '0'));
                    res.faces.push_back((unsigned int)(allWordsLine[4][0] - '0'));
                    res.faces.push_back((unsigned int)(allWordsLine[2][0] - '0'));
                    res.faces.push_back((unsigned int)(allWordsLine[3][0] - '0'));
                }
            }
        }

        // Close file
        fileOBJ.close();

        return res;
    }
};

#endif;