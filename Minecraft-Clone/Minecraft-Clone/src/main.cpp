#include "../headers/main.h"

// default settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* window = nullptr;
Helper helper;

int main() {
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Minecraft Clone", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_callback);

	graphics.init();
	graphics.moveCamera(6, camera_speed);
	initBlockMap();
	generateWorld(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		graphics.idle();
		glfwPollEvents();
	}

	//TODO: Add way to clean up all of the vertex arrays and buffers
	//graphics.clean();
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		graphics.moveCamera(0, NULL);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		graphics.moveCamera(1, NULL);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		graphics.moveCamera(2, NULL);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		graphics.moveCamera(3, NULL);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		graphics.moveCamera(4, NULL);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		graphics.moveCamera(5, NULL);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		graphics.moveCamera(6, camera_speed_enhanced);
	}
	else {
		graphics.moveCamera(6, camera_speed);
	}
	
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}

	GLfloat xoffset = lastX - xpos;
	GLfloat yoffset = ypos - lastY; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	graphics.look(xoffset, yoffset);
}

void initBlockMap()
{
	helper.loadTexture("../Minecraft-Clone/sprites/test.png", "test");
	blockMap[-1] = "test";
	helper.loadTexture("../Minecraft-Clone/sprites/grass_block.png", "grass");
	blockMap[1] = "grass";
	helper.loadTexture("../Minecraft-Clone/sprites/dirt_block.png", "dirt");
	blockMap[2] = "dirt";
	helper.loadTexture("../Minecraft-Clone/sprites/stone_block.png", "stone");
	blockMap[3] = "stone";
	helper.loadTexture("../Minecraft-Clone/sprites/bedrock_block.png", "bedrock");
	blockMap[4] = "bedrock";
}

void generateWorld(int seed)
{
	int x, y, z;
	for (x = 0; x < 16; x++) {
		std::vector<std::vector<int>> two;
		world.push_back(two);
		for (y = 0; y < MAX_HEIGHT; y++) {
			std::vector<int> one;
			world[x].push_back(one);
			for (z = 0; z < 16; z++) {
				if (y == MAX_HEIGHT - 8) {
					world[x][y].push_back(rand() % 2);
				}
				else if (y == MAX_HEIGHT - 5) {
					world[x][y].push_back(rand() % 2 + 2);
				}
				else if (y == MAX_HEIGHT - 1) {
					world[x][y].push_back(4);
				}
				else if (y > MAX_HEIGHT - 5){
					world[x][y].push_back(3);
				}
				else if (y > MAX_HEIGHT - 8) {
					if (world[x][y - 1][z]) {
						world[x][y].push_back(2);
					}
					else {
						world[x][y].push_back(1);
					}
				}
				else {
					world[x][y].push_back(0);
				}
			}
		}
	}
	graphics.genWorld(world, blockMap);
}