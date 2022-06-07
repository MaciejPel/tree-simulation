#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

float window_width = 1280, window_height = 720;
float lastX = window_width / 2, lastY = window_height / 2, yaw = -90, pitch = 0;
float obj_speed_x = 0, obj_speed_y = 0, obj_angle_x = 0, obj_angle_y = 0;
float cameraSpeed = 0.1f;

glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  10.0f);
glm::vec3 camera_focus 		= glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_upVector   = glm::vec3(0.0f, 1.0f,  0.0f);

void draw_scene(GLFWwindow* window){
	// kod rysujący obraz
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6, 0.8, 0.9, 0.5);
		
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, window_width / window_height, 1.0f, 50.0f);
	glm::mat4 V = glm::lookAt(camera_position, camera_position + camera_focus, camera_upVector);	
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::rotate(M, obj_angle_x, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, obj_angle_y, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(1.0f, 1.0f, 1.0f));

	spLambert->use();
	glUniform4f(spLambert->u("color"), 0.992, 0.984, 0.827, 1);
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));

	Models::cube.drawWire();
	Models::cube.drawSolid();

	glfwSwapBuffers(window);
}

void window_resize_callback(GLFWwindow * window, int width, int height){
	glViewport(0, 0, width, height);
	draw_scene(window);
}

void error_callback(int error, const char* description){
	fputs(description, stderr);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

	glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_focus = glm::normalize(direction);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS){
		if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
		if(key == GLFW_KEY_LEFT) obj_speed_x= -PI;
		if(key == GLFW_KEY_RIGHT) obj_speed_x = PI;
		if(key == GLFW_KEY_UP) obj_speed_y = PI;
		if(key == GLFW_KEY_DOWN) obj_speed_y = -PI;
	}
	if(action == GLFW_RELEASE){
		if(key == GLFW_KEY_LEFT) obj_speed_x= 0;
		if(key == GLFW_KEY_RIGHT) obj_speed_x = 0;
		if(key == GLFW_KEY_UP) obj_speed_y = 0;
		if(key == GLFW_KEY_DOWN) obj_speed_y = 0;
	}
}

void process_movement(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_focus;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_focus;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_position -= glm::normalize(glm::cross(camera_focus, camera_upVector)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_position += glm::normalize(glm::cross(camera_focus, camera_upVector)) * cameraSpeed;
}

void free_opengl_program(GLFWwindow* window){
    // kod, który należy wykonać po zakończeniu pętli głównej
    freeShaders();
}

void init_opengl_program(GLFWwindow* window){
	// kod, który należy wykonać raz, na początku programu
	glfwSetWindowAspectRatio(window, 16, 9);
	glfwSetWindowSizeLimits(window, 1024, 576, 1920, 1080);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, lastX, lastY);
    initShaders();
	glfwSetFramebufferSizeCallback(window, window_resize_callback);
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetCursorPosCallback(window, mouse_callback);  
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
}

int main(void){
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()){
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(window_width, window_height, "OpenGL", NULL, NULL);
	if (!window){
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK){
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	init_opengl_program(window);
	while (!glfwWindowShouldClose(window)){
		obj_angle_x += obj_speed_x * glfwGetTime();
		obj_angle_y += obj_speed_y * glfwGetTime();
		process_movement(window);
		glfwSetTime(0);
		draw_scene(window);
		glfwPollEvents(); 
	}

	free_opengl_program(window);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}