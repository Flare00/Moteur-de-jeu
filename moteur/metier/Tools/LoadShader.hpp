#ifndef __LOAD_SHADER_H__
#define __LOAD_SHADER_H__

std::string ReadFile(std::string path) {
	std::string retour;
	std::ifstream FragmentShaderStream(path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		retour = sstr.str();
		FragmentShaderStream.close();
		return retour;
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", path);
		getchar();
		return "";
	}
}

GLint CompileShader(GLuint id, std::string code) {
	GLint result = GL_FALSE;
	int infoLogLength;
	//Compile
	char const* VertexSourcePointer = code.c_str();
	glShaderSource(id, 1, &VertexSourcePointer, NULL);
	glCompileShader(id);

	// Check 
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(id, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
	return result;
}

GLuint LoadShaders(std::string vertexPath, std::string fragmentPath, std::string geometryPath = "", std::string tesselationControlPath = "", std::string tesselationEvalPath = "") {
	std::string vertexData;
	std::string fragmentData;
	std::string geometryData = "";
	std::string tesselationControlData = "";
	std::string tesselationEvalData = "";
	GLint ProgramID = 0;

	vertexData = ReadFile(vertexPath);
	if (vertexData.length() > 0) {
		fragmentData = ReadFile(fragmentPath);
		if (fragmentData.length() > 0) {
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			printf("Compiling shader : %s\n", vertexPath);
			if (CompileShader(VertexShaderID, vertexData) == GL_FALSE) return 0;
			printf("Compiling shader : %s\n", fragmentPath);
			if (CompileShader(FragmentShaderID, fragmentData) == GL_FALSE) return 0;

			GLuint GeometryShaderID;
			GLuint TesselationControlShaderID;
			GLuint TesselationEvalShaderID;
			if (geometryPath.length() > 0) {
				geometryData = ReadFile(geometryPath);
				if (geometryData.length() > 0) {
					GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
					printf("Compiling shader : %s\n", geometryPath);
					if (CompileShader(GeometryShaderID, geometryData) == GL_FALSE) return 0;
				}
				else {
					return 0;
				}
			}

			if (tesselationControlPath.length() > 0) {
				tesselationControlData = ReadFile(tesselationControlPath);
				if (tesselationControlData.length() > 0) {
					TesselationControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
					printf("Compiling shader : %s\n", tesselationControlPath);
					if (CompileShader(TesselationControlShaderID, tesselationControlData) == GL_FALSE) return 0;
				}
				else {
					return 0;
				}
			}

			if (tesselationEvalPath.length() > 0) {
				tesselationEvalData = ReadFile(tesselationEvalPath);
				if (tesselationEvalData.length() > 0) {
					TesselationEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
					printf("Compiling shader : %s\n", tesselationEvalPath);
					if (CompileShader(TesselationEvalShaderID, tesselationEvalData) == GL_FALSE) return 0;
				}
				else {
					return 0;
				}
			}

			//Linking
			printf("Linking program\n");
			ProgramID = glCreateProgram();
			glAttachShader(ProgramID, VertexShaderID);
			glAttachShader(ProgramID, FragmentShaderID);
			if (geometryPath.length() > 0) {
				glAttachShader(ProgramID, GeometryShaderID);
			}
			if (tesselationControlPath.length() > 0) {
				glAttachShader(ProgramID, TesselationControlShaderID);
			}
			if (tesselationEvalPath.length() > 0) {
				glAttachShader(ProgramID, TesselationEvalShaderID);
			}
			glLinkProgram(ProgramID);
			GLint result = GL_FALSE;
			int infoLogLength;
			// Check the program
			glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0) {
				std::vector<char> ProgramErrorMessage(infoLogLength + 1);
				glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
				printf("%s\n", &ProgramErrorMessage[0]);
			}


			glDetachShader(ProgramID, VertexShaderID);
			glDetachShader(ProgramID, FragmentShaderID);
			if (geometryPath.length() > 0) {
				glDetachShader(ProgramID, GeometryShaderID);
			}
			if (tesselationControlPath.length() > 0) {
				glDetachShader(ProgramID, TesselationControlShaderID);
			}
			if (tesselationEvalPath.length() > 0) {
				glDetachShader(ProgramID, TesselationEvalShaderID);
			}

			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);
			if (geometryPath.length() > 0) {
				glDeleteShader(GeometryShaderID);
			}
			if (tesselationControlPath.length() > 0) {
				glDeleteShader(TesselationControlShaderID);
			}
			if (tesselationEvalPath.length() > 0) {
				glDeleteShader(TesselationEvalShaderID);
			}
		}
	}

	return ProgramID;
}



#endif