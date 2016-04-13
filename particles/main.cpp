#include "GL\glew.h"

#include "glfwContext.h"
#include "Shader.h"
#include "MatrixStack.h"

#include "Camera.h"


#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::make:mat4
#include <glm\gtx\rotate_vector.hpp> // rotate vector

#include <iostream>

void inputHandler(GLFWwindow* _window, double _dT);
void cameraHandler(GLFWwindow* _window, double _dT, Camera* _cam);
void GLcalls();

int main(){

	glm::mat4 test = glm::mat4(1);

	glfwContext glfw;
	GLFWwindow* currentWindow = nullptr;

	glfw.init(1280, 720, "particles");
	glfw.getCurrentWindow(currentWindow);
	glfwSetCursorPos(currentWindow, 640, 360);

	//start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum l_GlewResult = glewInit();
	if (l_GlewResult != GLEW_OK) 
		std::cout << "glewInit() error." << std::endl;
	
	// Print some info about the OpenGL context...
	glfw.printGLInfo();

	//Shader emitShader;
	//emitShader.createTransformShader("shaders/emit.vert", "shaders/emit.frag", "shaders/emit.geom");

	//GLint locationP = glGetUniformLocation(emitShader.programID, "P"); //perspective matrix
	//GLint locationMV = glGetUniformLocation(emitShader.programID, "MV"); //modelview matrix
	//GLint locationM = glGetUniformLocation(emitShader.programID, "M"); //modelview matrix

	MatrixStack MVstack; MVstack.init();

	//scene objects

	//TODO: do this properly
	glm::vec4 LP = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);// glm::vec4(lightOne.getPosition()[0], lightOne.getPosition()[1], lightOne.getPosition()[2], 1.0f);
	glm::mat4 lightT = glm::mat4(1.0f);

	Camera mCamera;
	mCamera.setPosition(&glm::vec3(0.0f, 0.0f, -1.0f));
	mCamera.update();


	double lastTime = glfwGetTime() - 0.001;
	double dT = 0.0;
	while (!glfwWindowShouldClose(currentWindow))
	{
		dT = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		//glfw input handler
		inputHandler(currentWindow, dT);
		cameraHandler(currentWindow, dT, &mCamera);

		GLcalls();

		//glUseProgram(sceneLight.programID);
		
		
		MVstack.push();//Camera transforms --<
			//glUniformMatrix4fv(locationP, 1, GL_FALSE, mCamera.getPerspective());
			//MVstack.multiply(mCamera.getTransformM());


		MVstack.pop(); //Camera transforms >--

		glfwSwapBuffers(currentWindow);
		glfwPollEvents();
	}
	
	return 0;
}

void inputHandler(GLFWwindow* _window, double _dT)
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(_window, GL_TRUE);
	}
	
}
void cameraHandler(GLFWwindow* _window, double _dT, Camera* _cam)
{
	glm::vec3 translation;
	glm::mat4* T = _cam->getTransformM();
	glm::vec3* cDirection = _cam->getDirection();
	glm::vec3 cRight = glm::vec3((*T)[0][0], (*T)[1][0], (*T)[2][0]);
	glm::vec3* cUp = _cam->getUpDirection();
	float movementSpeed = 0.0f;
	
	if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT)){
		movementSpeed = 10.0f;
	}
	else{
		movementSpeed = 1.0f;
	}
	if (glfwGetKey(_window, GLFW_KEY_W)) {
		translation = *cDirection*movementSpeed*(float)_dT;
		_cam->translate(&translation);
	}
	if (glfwGetKey(_window, GLFW_KEY_S)){
		translation = *cDirection*-movementSpeed*(float)_dT;
		_cam->translate(&translation);
	}
	if (glfwGetKey(_window, GLFW_KEY_A)) {
		translation = cRight*-movementSpeed*(float)_dT;
		_cam->translate(&translation);
	}
	if (glfwGetKey(_window, GLFW_KEY_D)) {
		translation = cRight*movementSpeed*(float)_dT;
		_cam->translate(&translation);
	}

	double X, Y, dX, dY;
	glfwGetCursorPos(_window, &X, &Y);

	int width, height;
	glfwGetWindowSize(_window, &width, &height);

	_cam->yaw -= (X - (width / 2)) / width;
	_cam->pitch -= (Y - (height / 2)) / height;

	_cam->update();

	glfwSetCursorPos(_window, (width / 2), (height / 2));
}

void GLcalls()
{
	glClearColor(0.01f, 0.01f, 0.01f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	//glEnable(GL_FLAT);
	//glShadeModel(GL_FLAT);
	glCullFace(GL_BACK);
	//glDisable(GL_TEXTURE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glFrontFace(GL_CCW);
}
