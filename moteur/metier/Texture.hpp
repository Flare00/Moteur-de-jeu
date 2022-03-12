#ifndef __H_TEXTURE__
#define __H_TEXTURE__

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Texture
{
private:
	std::string path;
	int width, height, nbChan;

	unsigned int texture_index;
	unsigned char *texture_data;

	bool loaded;

	bool powerOfTwo(int x)
	{
		return (x & (x - 1)) == 0;
	}

public:
	Texture(std::string path)
	{
		this->path = path;

		glGenTextures(1, &this->texture_index);
		glBindTexture(GL_TEXTURE_2D, this->texture_index);

		int w, h, nbC;
		this->texture_data = stbi_load(path.c_str(), &w, &h, &nbC, 4);
		this->width = w;
		this->height = h;
		this->nbChan = nbC;

		if (this->texture_data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->texture_data);
			glBindTexture(GL_TEXTURE_2D, this->texture_index);

			if (powerOfTwo(w) && powerOfTwo(h))
			{
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			this->loaded = true;
		}
		else
		{
			this->loaded = false;
		}
		stbi_image_free(this->texture_data);
	}

	void draw(GLuint u_texture, int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, this->texture_index);
		glUniform1i(u_texture, index);
	}
};

#endif