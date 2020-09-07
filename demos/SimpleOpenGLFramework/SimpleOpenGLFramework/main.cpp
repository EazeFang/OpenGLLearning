#include "Dependencies\glew\glew.h"  
#include "Dependencies\freeglut\freeglut.h" 
#include "GLSLProgramObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <TriMesh.h>
#include <TriMesh_algo.h>
#include <iostream>


GLsizei screenWidth = 800;
GLsizei screenHeight = 600;

//camera transform variables
int state = 0, oldX = 0, oldY = 0;
float rX = 0, rY = 300, dist = -10;

//modelview projection and rotation matrices
glm::mat4 MV, P, R;

GLSLProgramObject testShader;

void checkGLError()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			//printf("GL Error: GL_INVALID_ENUM %s : %d \n", file, line);
			std::cout << "GL Error: GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			//printf("GL Error: GL_INVALID_VALUE %s : %d \n", file, line);
			std::cout << "GL Error: GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			//printf("GL Error: GL_INVALID_OPERATION %s : %d \n", file, line);
			std::cout << "GL Error: GL_INVALID_OPERATION" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			//printf("GL Error: GL_STACK_OVERFLOW %s : %d \n", file, line);
			std::cout << "GL Error: GL_STACK_OVERFLOW" << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			//printf("GL Error: GL_STACK_UNDERFLOW %s : %d \n", file, line);
			std::cout << "GL Error: GL_STACK_UNDERFLOW" << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			//printf("GL Error: GL_OUT_OF_MEMORY %s : %d \n", file, line);
			std::cout << "GL Error: GL_OUT_OF_MEMORY" << std::endl;
			break;
		default:
			printf("GL Error: 0x%x \n", error);
			//std::cout << "GL Error: GL_INVALID_VALUE" << std::endl;
			break;
		}
	}
}
//if you want to do off-screen render, you may need to set FBO to storage the results 
void initialFBO()
{

}

// 3D VAO handle
GLuint vaoHandle3D;
// index count
GLuint indexCount;
// self define vertexes data
// we draw a cube for instance
GLuint cubeVAOID;
GLuint cubeVBOID;
GLuint cubeIndicesID;
void initialVAO()
{
	//setup unit cube vertex array and vertex buffer objects
	glGenVertexArrays(1, &cubeVAOID);
	glGenBuffers(1, &cubeVBOID);
	glGenBuffers(1, &cubeIndicesID);

	//unit cube vertices 
	glm::vec3 vertices[8] =
	{
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f)
	};

	//unit cube indices
	GLushort cubeIndices[36] =
	{
		0, 5, 4,
		5, 0, 1,
		3, 7, 6,
		3, 6, 2,
		7, 4, 6,
		6, 4, 5,
		2, 1, 3,
		3, 1, 0,
		3, 0, 7,
		7, 0, 4,
		6, 5, 2,
		2, 5, 1
	};
	glBindVertexArray(cubeVAOID);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBOID);
	//pass cube vertices to buffer object memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &(vertices[0].x), GL_STATIC_DRAW);

	//GL_CHECK_ERRORS

	//enable vertex attributre array for position
	//对应顶点着色器中Location = 0 的变量传值 ，这里就对应vVertex ，定点着色器处理阶段 shader.bind() 使用VAO传入定点数据
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//pass cube indices to element array  buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}
// set VAO from mesh data
void loadMeshAndSetVAO()
{
	//input mesh file name
	trimesh::TriMesh* mainMesh = trimesh::TriMesh::readOBJ("Rabbit.obj");
	if (mainMesh)
	{
		/*--------------------------归一化处理--------------------------*/
		mainMesh->need_bbox();
		trimesh::point centralO = (mainMesh->bbox.min + mainMesh->bbox.max)*0.5f;
		float boundingBoxR = trimesh::len(mainMesh->bbox.max - mainMesh->bbox.min) / 2;
		for (int i = 0; i < mainMesh->vertices.size(); i++)
		{
			mainMesh->vertices[i] = (mainMesh->vertices[i] - centralO) / boundingBoxR;
		}
		/*--------------------------归一化处理--------------------------*/
		indexCount = mainMesh->faces.size() * 3;
		////申请VAO


		// Create and populate the buffer objects   
		GLuint vboHandles;
		glGenBuffers(1, &vboHandles);
		GLuint positionBufferHandle = vboHandles;
		//GLuint colorBufferHandle = vboHandles[1];
		//GLuint normalBufferHandle = vboHandles[2];

		//绑定VBO以供使用    
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		//加载数据到VBO    
		glBufferData(GL_ARRAY_BUFFER, mainMesh->vertices.size()*sizeof(trimesh::point), mainMesh->vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Generate an ID for the index buffer.  
		GLuint indexBufferId;
		glGenBuffers(1, &indexBufferId);

		// Bind the index buffer and load the index data into it.  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(int), mainMesh->faces.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &vaoHandle3D);

		glBindVertexArray(vaoHandle3D);

		//着色器只有一个顶点变量，所以颜色和法线都不用传
		glEnableVertexAttribArray(0);  // Vertex position.  
		//将positionBufferHandle的点坐标数据绑定到VAO的0号绑定点(location == 0的位置)，以供shader中读取
		// Specify the location and format of the position portion of the vertex buffer.  
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	}
}
//attach shaders and link
void buildShaders()
{
	testShader.attachVertexShader("shader/TestVertexShader.vert");
	testShader.attachFragmentShader("shader/TestFragmentShader.frag");
	testShader.link();
}

void InitGL()
{
	//set VAO
	loadMeshAndSetVAO();//or initialVAO
	//set FBO
	initialFBO();
	//build shaders
	buildShaders();
	//set some states
	//glEnable(GL_DEPTH_TEST);
}

void render()
{
	// set clear color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// 执行clear 清除的颜色和深度缓存区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//enable alpha blending with over compositing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//camera transformation
	glm::mat4 Tr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
	glm::mat4 Rx = glm::rotate(Tr, rX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MV = glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MVP = P*MV;
	testShader.bind();
	float color[4] = { 0.8, 0.6, 0.4, 0.6 };
	testShader.setUniform("uniformColor", color, 4);
	std::string parName = "MVP";
	GLint progId = testShader.getProgId();
	GLint id = glGetUniformLocation(progId, parName.c_str());
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(MVP*R));


	glBindVertexArray(vaoHandle3D);
	glDrawElements(GL_TRIANGLES,indexCount,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
	testShader.release();
	//启用了双缓冲，那么默认画在back 
	glutSwapBuffers();
}

void display()
{
	render();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	//setup the projection matrix
	P = glm::perspective(25.0f, (float)w / h, 0.1f, 1000.0f);
}

void mouseFunc(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{
		if (GLUT_RIGHT_BUTTON)
		{
			oldX = x;
			oldY = y;
		}
	}

	if (button == GLUT_LEFT_BUTTON)
		state = 0;
	else
		state = 1;
}

void motionFunc(int x, int y)
{
	if (state == 0) {
		dist += (y - oldY) / 10.0f;
	}
	else {
		rX += (y - oldY) / 80.0f;
		rY += (x - oldX) / 80.0f;
	}
	oldX = x;
	oldY = y;

	glutPostRedisplay();
}

void idle()
{
	//create a new rotation matrix for rotation on the Y axis
	R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
	//recall the display callback
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);								//GULT
	/*-------------2D------------*/
	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);		//set display mode 
	/*-------------3D------------*/
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	//glutInitWindowPosition(100, 100);					//set window up left conner screen pos
	glutInitWindowSize(screenWidth, screenHeight);		//set window size
	glutCreateWindow("hello glsl !");	//title
	
	//初始化glew扩展库    
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
	}
	else
	{
		if (GLEW_VERSION_3_3)
		{
			std::cout << "Driver supports OpenGL 3.3\nDetails:" << std::endl;
		}
	}
	//this is to ignore INVALID ENUM error 1282
	err = glGetError(); 
	InitGL();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutIdleFunc(idle);
	//main loop call
	glutMainLoop();

	return 0;
}