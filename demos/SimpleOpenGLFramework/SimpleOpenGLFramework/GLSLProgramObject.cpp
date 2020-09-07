//
// GLSLProgramObject.cpp - Wrapper for GLSL program objects
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#define NV_REPORT_COMPILE_ERRORS
//#define NV_REPORT_UNIFORM_ERRORS

#include "GLSLProgramObject.h"
#include <iostream>

GLSLProgramObject::GLSLProgramObject() :
	progId_(0)
{
}

GLSLProgramObject::~GLSLProgramObject()
{
	destroy();
}

void GLSLProgramObject::destroy()
{
	for (unsigned i = 0; i < vertexShaders_.size(); i++) {
		glDeleteShader(vertexShaders_[i]);
	}
	for (unsigned i = 0; i < fragmentShaders_.size(); i++) {
		glDeleteShader(fragmentShaders_[i]);
	}
	if (progId_ != 0) {
		glDeleteProgram(progId_);
	}
}

//void GLSLProgramObject::attachVertexShader(std::string filename)
//{
//	//std::cerr << filename << std::endl;
//	//std::string resolved_path;
//	//
//	//if (sdkPath.getFilePath( filename.c_str(), resolved_path)) {
//	//    GLuint shaderId = nv::CompileGLSLShaderFromFile(GL_VERTEX_SHADER, resolved_path.c_str());
//	//    if (shaderId == 0) {
//	//	    std::cerr << "Error: Vertex shader failed to compile" << std::endl;
//	//	    exit(1);
//	//    }
//	//    _vertexShaders.push_back(shaderId);
//	//}
//	//else {
//	//    std::cerr << "Error: Failed to find vertex shader" << std::endl;
//	//	exit(1);
//	//}
//}
//
void GLSLProgramObject::attachVertexShader(const char *VShaderFile)
{
	//创建着色器对象：顶点着色器    
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	//错误检测    
	if (0 == vShader)
	{
		std::cerr << "ERROR : Create vertex shader failed" << std::endl;
		exit(1);
	}

	//把着色器源代码和着色器对象相关联    
	const GLchar *vShaderCode = textFileRead(VShaderFile);
	const GLchar *vCodeArray[1] = { vShaderCode };

	//将字符数组绑定到对应的着色器对象上  
	glShaderSource(vShader, 1, vCodeArray, NULL);

	//编译着色器对象    
	glCompileShader(vShader);

	//检查编译是否成功    
	GLint compileResult;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度    
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出    
			glGetShaderInfoLog(vShader, logLen, &written, log);
			std::cerr << "vertex shader compile log : " << std::endl;
			std::cerr << log << std ::endl;
			free(log);//释放空间    
		}
	}
	else
	{
		vertexShaders_.push_back(vShader);
	}
	//add to _vertexShaders
}

//void GLSLProgramObject::attachFragmentShader(std::string filename)
//{
//	////std::cerr << filename << std::endl;
//    //std::string resolved_path;
//	//
//    //if (sdkPath.getFilePath( filename.c_str(), resolved_path)) {
//	//    GLuint shaderId = nv::CompileGLSLShaderFromFile(GL_FRAGMENT_SHADER, resolved_path.c_str());
//	//    if (shaderId == 0) {
//	//	    std::cerr << "Error: Fragment shader failed to compile" << std::endl;
//	//	    exit(1);
//	//    }
//	//    _fragmentShaders.push_back(shaderId);
//    //}
//    //else {
//    //    std::cerr << "Error: Failed to find fragment shader" << std::endl;
//	//	exit(1);
//    //}
//}


void GLSLProgramObject::attachFragmentShader(const char *FShaderFile)
{
	//创建着色器对象：片断着色器    
	GLint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//错误检测    
	if (0 == fShader)
	{
		std::cerr << "ERROR : Create fragment shader failed" << std::endl;
		exit(1);
	}

	//把着色器源代码和着色器对象相关联    
	const GLchar *fShaderCode = textFileRead(FShaderFile);
	const GLchar *fCodeArray[1] = { fShaderCode };
	glShaderSource(fShader, 1, fCodeArray, NULL);

	//编译着色器对象    
	glCompileShader(fShader);

	//检查编译是否成功  
	GLint compileResult;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度    
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出    
			glGetShaderInfoLog(fShader, logLen, &written, log);
			std::cerr << "fragment shader compile log : " << std::endl;
			std::cerr << log << std::endl;
			free(log);//释放空间    
		}
	}
	else
	{
		fragmentShaders_.push_back(fShader);
	}
}




void GLSLProgramObject::link()
{
	////3、链接着色器对象    
	////创建着色器程序    
	//GLuint programHandle = glCreateProgram();
	//if (!programHandle)
	//{
	//	cerr << "ERROR : create program failed" << endl;
	//	exit(1);
	//}
	////将着色器程序链接到所创建的程序中    
	//glAttachShader(programHandle, vShader);
	//glAttachShader(programHandle, fShader);
	////将这些对象链接成一个可执行程序    
	//glLinkProgram(programHandle);
	////查询链接的结果    
	//GLint linkStatus;
	//glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
	//if (GL_FALSE == linkStatus)
	//{
	//	cerr << "ERROR : link shader program failed" << endl;
	//	GLint logLen;
	//	glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
	//		&logLen);
	//	if (logLen > 0)
	//	{
	//		char *log = (char *)malloc(logLen);
	//		GLsizei written;
	//		glGetProgramInfoLog(programHandle, logLen,
	//			&written, log);
	//		cerr << "Program log : " << endl;
	//		cerr << log << endl;
	//	}
	//}
	//else//链接成功，在OpenGL管线中使用渲染程序    
	//{
	//	glUseProgram(programHandle);
	//}

	/*---------------------------------------*/

	//步骤基本一样

	progId_ = glCreateProgram();

    for (unsigned i = 0; i < vertexShaders_.size(); i++) {
        glAttachShader(progId_, vertexShaders_[i]);
    }

    for (unsigned i = 0; i < fragmentShaders_.size(); i++) {
        glAttachShader(progId_, fragmentShaders_[i]);
    }

    glLinkProgram(progId_);

    GLint success = 0;
    glGetProgramiv(progId_, GL_LINK_STATUS, &success);

    if (!success) {
        char temp[1024];
        glGetProgramInfoLog(progId_, 1024, NULL, temp);
        printf("Failed to link program:\n%s\n", temp);
		exit(1);
    }
}

void GLSLProgramObject::bind()
{
	glUseProgram(progId_);
}

void GLSLProgramObject::release()
{
	glUseProgram(0);
}

void GLSLProgramObject::setUniform(std::string name, GLfloat* val, int count)
{
	GLint id = glGetUniformLocation(progId_, name.c_str());
	if (id == -1) {
#ifdef NV_REPORT_UNIFORM_ERRORS
		std::cerr << m_vName << std::endl << m_fName << ":" << std::endl;
		std::cerr << "Warning: Invalid uniform parameter " << name << std::endl;
#endif
		return;
	}
	switch (count) {
		case 1:
			glUniform1fv(id, 1, val);
			break;
		case 2:
			glUniform2fv(id, 1, val);
			break;
		case 3:
			glUniform3fv(id, 1, val);
			break;
		case 4:
			glUniform4fv(id, 1, val);
			break;
	}
}

void GLSLProgramObject::setUniformMatrix4(std::string name, GLfloat* val, int count)
{
	GLint id = glGetUniformLocation(progId_, name.c_str());
	if (id == -1)
	{
		return;
	}
	
	glUniformMatrix4fv(id,1,GL_FALSE,val);

}

void GLSLProgramObject::setTextureUnit(std::string texname, int texunit)
{
	GLint linked;
	glGetProgramiv(progId_, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE) {
		std::cerr << "Error: setTextureUnit needs program to be linked." << std::endl;
		exit(1);
	}
	GLint id = glGetUniformLocation(progId_, texname.c_str());
	if (id == -1) {
#ifdef NV_REPORT_UNIFORM_ERRORS
		std::cerr << "Warning: Invalid texture " << texname << std::endl;
#endif
		return;
	}
	glUniform1i(id, texunit);
}

void GLSLProgramObject::bindTexture(GLenum target, std::string texname, GLuint texid, int texunit)
{
	glActiveTexture(GL_TEXTURE0 + texunit);

	glBindTexture(target, texid);
	setTextureUnit(texname, texunit);
	glActiveTexture(GL_TEXTURE0);
}


