#ifndef __TEXTE_SHADER_H__
#define __TEXTE_SHADER_H__

#include "Shader.hpp"

class TexteShader : public Shader {
protected:

public:
	TexteShader(std::string vertex, std::string fragment) : Shader(vertex, fragment) {
	}
};


#endif
