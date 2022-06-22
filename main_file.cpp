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
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

float window_width = 1280, window_height = 720;
float lastX = window_width / 2, lastY = window_height / 2, yaw = -90, pitch = 0;
float cameraSpeed = 0.1f;
float tree_growth = 0.0f, tree_growth_speed = 1.5f, tree_max_height = 5.0f;
float branch_growth = 0.0f, branch_growth_speed = 0.1f;
GLuint tex;

glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  15.0f);
glm::vec3 camera_focus 		= glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_upVector   = glm::vec3(0.0f, 1.0f,  0.0f);

float random(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

float max(float a, float b){
	if(a>b) return a;
	return b;
}

float min(float a, float b){
	if(a<b) return a;
	return b;
}

float positive(float a){
	if(a<0) return -a;
	return a;
}

float negative(float a){
	if(a>0) return -a;
	return a;
}

int max_depth = 4;
void draw(int depth, float height, float x, float y, float angle, float radius){
	if (depth > 0){
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(x, y, 0.0f));
		M = glm::rotate(M, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		M = glm::scale(M, glm::vec3(radius, height, radius));

		glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D,tex);
		glUniform1i(spTextured->u("tex"), 0);

		Models::cone.drawWire();
		Models::cone.drawSolid();

		if (depth == max_depth) y = y + (height / 4);
		float prevX = x, prevY = y;

		x = x - (cos(glm::radians(270 + angle)) - cos(glm::radians(270 + angle + 40))) * height / 2;
		y = y - (sin(glm::radians(270 + angle)) - sin(glm::radians(270 + angle + 40))) * height / 2;
		prevX = prevX + (cos(glm::radians(270 + angle)) - cos(glm::radians(270 + angle + 40))) * height / 2;
		prevY = prevY + (sin(glm::radians(270 + angle)) - sin(glm::radians(270 + angle + 40))) * height / 2;

		draw(depth - 1, height / 2, x, y, angle - 40, radius / 2);
		draw(depth - 1, height / 2, prevX, prevY, angle + 40, radius / 2);
	}
}

void draw_scene(GLFWwindow* window){
	// kod rysujący obraz
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6, 0.8, 0.9, 0.5);

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, window_width / window_height, 1.0f, 50.0f);
	glm::mat4 V = glm::lookAt(camera_position, camera_position + camera_focus, camera_upVector);	
	glm::mat4 M = glm::mat4(1.0f);

	spTextured->use();
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));

	draw(max_depth, tree_max_height, 0, 0, 0, 1);

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

    yaw += xoffset;
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

GLuint readTexture(const char* filename){
	GLuint tex;
	glActiveTexture(GL_TEXTURE0); 
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
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
	char texture[] = "kora.png";
	tex = readTexture(texture);
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
		tree_growth += tree_growth_speed * glfwGetTime();
		process_movement(window);
		glfwSetTime(0);
		draw_scene(window);
		glfwPollEvents(); 
	}

	free_opengl_program(window);
	glDeleteTextures(1, &tex);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}