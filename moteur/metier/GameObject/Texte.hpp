#ifndef __TEXTE_H__
#define __TEXTE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include "GameObject.hpp"

#include "../Texture.hpp"
#include "../Collision/BoundingBox.hpp"
#include "../Shader/TexteShader.hpp"

class Texte : public GameObject {
private :
	TexteShader* shader;
    std::string texte;
protected:
    Texte(std::string id, GameObject* parent = NULL) : GameObject(id, parent) {
        this->texte = id;
	}

    Texte(std::string id, std::string texte, GameObject* parent = NULL) : GameObject(id, parent) {
        this->texte = texte;
	}

    void setTexte(std::string texte){
        this->texte = texte;
    }   
    std::string getTexte(){
        return this->texte;
    }

    virtual void draw(bool dfs) {
        if(this->texte != "")
            return;

        this->shader->use();

        
		if (dfs)
			GameObject::draw();
		
	} 
};

#endif