#ifndef ___MAIN_H___
#define ___MAIN_H___

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "globals.h"
#include "graphics.h"

#define MAX_HEIGHT 256

Graphics graphics;

std::vector<std::vector<std::vector<int>>> world;
std::map<int, std::string> blockMap;

double lastX, lastY;
int firstMouse = 1;
float camera_speed = 0.1f;
float camera_speed_enhanced = 0.25f;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

void initBlockMap();
void generateWorld(int seed);

#endif ___MAIN_H___