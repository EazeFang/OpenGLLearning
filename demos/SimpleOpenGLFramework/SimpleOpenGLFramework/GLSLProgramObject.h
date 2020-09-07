//
// GLSLProgramObject.h - Wrapper for GLSL program objects
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//
// modified by Eaze Fang 2018-03-08 
// ADD
// void attachVertexShader(const char *VShaderFile);
// void attachFragmentShader(const char *FShaderFile); 
// attach shader by glew  lib
////////////////////////////////////////////////////////////////////////////////

#ifndef GLSL_PROGRAM_OBJECT_H
#define GLSL_PROGRAM_OBJECT_H

#include "Dependencies\glew\glew.h"
#include "TextFile.h"
#include <string>
#include <iostream>
#include <vector>

class GLSLProgramObject
{
public:
	GLSLProgramObject();
	virtual ~GLSLProgramObject();

	void destroy();

	void bind();

	void release();

	void setUniform(std::string name, GLfloat* val, int count);

	void setUniformMatrix4(std::string name, GLfloat* val, int count);

	void setTextureUnit(std::string texname, int texunit);

	void bindTexture(GLenum target, std::string texname, GLuint texid, int texunit);

	void bindTexture2D(std::string texname, GLuint texid, int texunit) {
		bindTexture(GL_TEXTURE_2D, texname, texid, texunit);
	}

	void bindTexture3D(std::string texname, GLuint texid, int texunit) {
		bindTexture(GL_TEXTURE_3D, texname, texid, texunit);
	}

	void bindTextureRECT(std::string texname, GLuint texid, int texunit) {
		bindTexture(GL_TEXTURE_RECTANGLE_ARB, texname, texid, texunit);
	}

	//void attachVertexShader(std::string filename);
	//
	void attachVertexShader(const char *VShaderFile);

	//void attachFragmentShader(std::string filename);

	void attachFragmentShader(const char *FShaderFile);

	void link();

	inline GLuint getProgId() { return progId_; }
	
protected:
	std::vector<GLuint>		vertexShaders_;
	std::vector<GLuint>		fragmentShaders_;
	GLuint					progId_;
};

#endif
