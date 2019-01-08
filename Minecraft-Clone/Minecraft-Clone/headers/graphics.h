#ifndef ___GRAPHICS_H___
#define ___GRAPHICS_H___

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "globals.h"
#include "shader.h"
#include "helper.h"
#include "graphicsObject.h"
#include "texture.h"

#include <map>
#include <time.h>
#include <vector>
#include <bitset>
#include <string>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

class Graphics
{
public:
	void init(void);
	void clean(void);
	void display(void);
	void idle(void);

	int genCubeTexture(std::string textureKey, glm::vec3 pos);
	int genBlockSides(std::string textureKey, std::bitset<6> sides, glm::vec3 pos);
	int genWorld(std::vector<std::vector<std::vector<int>>> world, std::map<int, std::string> blockMap);

	void moveCamera(int action, GLfloat modifier);
	void look(GLfloat xOffset, GLfloat yOffset);
private:
	int object_counter = 0;
	std::map<std::string, Shader> shaderMap;
	std::vector<GraphicsObject> objects;

	GLfloat camera_move_speed = 0.1f;
	GLfloat camera_look_speed_vertical = 0.1f;
	GLfloat camera_look_speed_horizontal = 0.1f;

	GLfloat fov = 90.0f;
	GLfloat theta_xz = 45.0f;
	GLfloat theta_y = 90.0f;
	glm::vec3 cameraPos = { 0.0, 8.0, 0.0 };
	glm::vec3 cameraFront = glm::vec3(glm::sin(glm::radians(theta_xz)) * glm::sin(glm::radians(theta_y)), glm::cos(glm::radians(theta_y)), glm::cos(glm::radians(theta_xz)) * glm::sin(glm::radians(theta_y)));
	glm::vec3 cameraUp = { 0.0, 1.0, 0.0 };
	
	GLuint projection_location;
	glm::mat4 projection = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	GLuint model_view_location;
	glm::mat4 model_view = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	
	GLuint vPosition;
	GLuint vColor;
	GLuint vTexCoords;
};

#endif