#include "../headers/graphics.h"

void Graphics::init(void) {
	shaderMap["simple_color_shader"] = Shader("../Minecraft-Clone/shaders/simple_color_shader.vs", "../Minecraft-Clone/shaders/simple_color_shader.frag");
	shaderMap["simple_texture_shader"] = Shader("../Minecraft-Clone/shaders/simple_texture_shader.vs", "../Minecraft-Clone/shaders/simple_texture_shader.frag");

	projection_location = glGetUniformLocation(shaderMap["simple_texture_shader"].ID, "projection");
	model_view_location = glGetUniformLocation(shaderMap["simple_texture_shader"].ID, "model_view");

	genCubeColorLines(1, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec3(-1.0, 16.0, -1.0));

	model_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//TODO: Skybox maybe
	glClearColor(0.0f, 0.8f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void Graphics::clean(void) {

}

void Graphics::display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!objects.empty()) {
		for (std::vector<GraphicsObject>::iterator it = objects.begin(); it != objects.end(); ++it) {
			switch (it->type) {
			case 0:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				shaderMap["simple_color_shader"].use();
				glBindVertexArray(it->VAO);
				glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
				glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
				glUniformMatrix4fv(it->ctm_location, 1, GL_FALSE, (GLfloat *)&it->ctm);
				glDrawArrays(GL_TRIANGLES, 0, it->num_vertices);
				break;
			case 1:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				shaderMap["simple_texture_shader"].use();
				glBindVertexArray(it->VAO);
				it->texture.bind();
				glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
				glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
				glUniformMatrix4fv(it->ctm_location, 1, GL_FALSE, (GLfloat *)&it->ctm);
				glUniform1i(it->selected_location, (GLint)&it->selected);
				glDrawArrays(GL_TRIANGLES, 0, it->num_vertices);
				break;
			case 2:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				shaderMap["simple_color_shader"].use();
				glBindVertexArray(it->VAO);
				glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
				glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
				glUniformMatrix4fv(it->ctm_location, 1, GL_FALSE, (GLfloat *)&it->ctm);
				glDrawArrays(GL_LINES, 0, it->num_vertices);
				break;
			}
		}
	}

	glfwSwapBuffers(window);
}

void Graphics::idle(void) {
	for (std::vector<GraphicsObject>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if(it->ID == 1) {
			glm::vec3 highlight = cameraPos + 3.0f * cameraFront;
			highlight.x = (int)highlight.x;
			highlight.y = (int)highlight.y;
			highlight.z = (int)highlight.z;
			it->ctm = glm::translate({ { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } }, highlight);
		}
	}

	display();
}

void Graphics::genCubeColor(int ID, glm::vec4 color, glm::vec3 pos)
{
	GraphicsObject newCube;
	int it;
	int i, j;

	newCube.ID = ID;
	newCube.type = 0;
	newCube.num_vertices = 36;

	glm::vec4 cube[36] = { {-0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, //Front
						{0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, //Right
						{0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //Back
						{-0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Left
						{-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Up
						{-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0} }; //Down

	newCube.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newCube.num_vertices));
	newCube.color_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newCube.num_vertices));

	for (it = 0; it < newCube.num_vertices; it++) {
		newCube.position_array[it] = cube[it];
		newCube.color_array[it] = color;
	}

	newCube.ctm = glm::translate(newCube.ctm, pos);

	glGenVertexArrays(1, &newCube.VAO);
	glBindVertexArray(newCube.VAO);
	glGenBuffers(1, &newCube.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newCube.VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4) * newCube.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newCube.num_vertices, newCube.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newCube.num_vertices, sizeof(glm::vec4) * newCube.num_vertices, newCube.color_array);
	vPosition = glGetAttribLocation(shaderMap["simple_color_shader"].ID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(shaderMap["simple_color_shader"].ID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newCube.num_vertices));

	newCube.ctm_location = glGetUniformLocation(shaderMap["simple_color_shader"].ID, "ctm");

	objects.push_back(newCube);

	return;
}


void Graphics::genCubeColorLines(int ID, glm::vec4 color, glm::vec3 pos)
{
	GraphicsObject newCube;
	int it;
	int i, j;

	newCube.ID = ID;
	newCube.type = 2;
	newCube.num_vertices = 24;

	glm::vec4 cube[24] = { {-0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, //0-1 0-3 0-4
						{0.5, 0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //2-1 2-3 2-6
						{0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //5-1 5-4 5-6
						{-0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0} }; //7-3 7-4 7-6

	newCube.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newCube.num_vertices));
	newCube.color_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newCube.num_vertices));

	for (it = 0; it < newCube.num_vertices; it++) {
		newCube.position_array[it] = cube[it];
		newCube.color_array[it] = color;
	}

	newCube.ctm = glm::translate(newCube.ctm, pos);

	glGenVertexArrays(1, &newCube.VAO);
	glBindVertexArray(newCube.VAO);
	glGenBuffers(1, &newCube.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newCube.VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4) * newCube.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newCube.num_vertices, newCube.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newCube.num_vertices, sizeof(glm::vec4) * newCube.num_vertices, newCube.color_array);
	vPosition = glGetAttribLocation(shaderMap["simple_color_shader"].ID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(shaderMap["simple_color_shader"].ID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newCube.num_vertices));

	newCube.ctm_location = glGetUniformLocation(shaderMap["simple_color_shader"].ID, "ctm");

	objects.push_back(newCube);

	return;
}

void Graphics::genCubeTexture(int ID, std::string textureKey, glm::vec3 pos)
{
	GraphicsObject newCube;
	int it;
	int i, j;

	newCube.ID = ID;
	newCube.type = 1;
	newCube.num_vertices = 36;
	newCube.texture.textureKey = textureKey;

	glm::vec4 cube[36] = { {-0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, //Front
						{0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, //Right
						{0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //Back
						{-0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Left
						{-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Up
						{-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0} }; //Down

	newCube.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newCube.num_vertices));
	newCube.texture_array = (glm::vec2*)malloc(sizeof(glm::vec2) * (newCube.num_vertices));

	for (it = 0; it < newCube.num_vertices; it++) {
		newCube.position_array[it] = cube[it];
	}

	it = 0;
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 3; i++) {
			newCube.texture_array[it] = { (float)i / 3.0, (float)(j + 1) / 2.0 };
			newCube.texture_array[it+1] = { (float)(i + 1) / 3.0, (float)(j + 1) / 2.0 };
			newCube.texture_array[it+2] = { (float)(i + 1) / 3.0, (float)j / 2.0 };
			newCube.texture_array[it+3] = { (float)i / 3.0, (float)(j + 1) / 2.0 };
			newCube.texture_array[it+4] = { (float)(i + 1) / 3.0, (float)j / 2.0 };
			newCube.texture_array[it+5] = { (float)i / 3.0, (float)j / 2.0 };
			it += 6;
		}
	}

	newCube.texture.generate(textureKey);
	newCube.ctm = glm::translate(newCube.ctm, pos);

	glGenVertexArrays(1, &newCube.VAO);
	glBindVertexArray(newCube.VAO);
	glGenBuffers(1, &newCube.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newCube.VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) + sizeof(glm::vec2)) * newCube.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newCube.num_vertices, newCube.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newCube.num_vertices, sizeof(glm::vec2) * newCube.num_vertices, newCube.texture_array);
	vPosition = glGetAttribLocation(shaderMap["simple_texture_shader"].ID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vTexCoords = glGetAttribLocation(shaderMap["simple_texture_shader"].ID, "vTexCoords");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newCube.num_vertices));

	newCube.ctm_location = glGetUniformLocation(shaderMap["simple_texture_shader"].ID, "ctm");

	objects.push_back(newCube);

	return;
}

void Graphics::genBlockSides(int ID, std::string textureKey, std::bitset<6> sides, glm::vec3 pos) {
	int s;
	int sideCount = 0;

	for (s = 0; s < 6; s++) {
		if (sides[s] == true) {
			sideCount++;
		}
	}
	
	if (sideCount == 0) {
		return;
	}

	GraphicsObject newSides;
	int it, i, j;

	newSides.ID = ID;
	newSides.type = 1;
	newSides.num_vertices = 6 * sideCount;
	newSides.texture.textureKey = textureKey;

	glm::vec4 cube[36] = { {-0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, //Front
						{0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, //Right
						{0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //Back
						{-0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Left
						{-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Up
						{-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0} }; //Down

	newSides.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newSides.num_vertices));
	newSides.texture_array = (glm::vec2*)malloc(sizeof(glm::vec2) * (newSides.num_vertices));

	s = 0;
	for (it = 0; it < 6; it++) {
		if (sides[it]) {
			newSides.position_array[6 * s] = cube[6 * it];
			newSides.position_array[6 * s + 1] = cube[6 * it + 1];
			newSides.position_array[6 * s + 2] = cube[6 * it + 2];
			newSides.position_array[6 * s + 3] = cube[6 * it + 3];
			newSides.position_array[6 * s + 4] = cube[6 * it + 4];
			newSides.position_array[6 * s + 5] = cube[6 * it + 5];
			s++;
		}
	}

	s = 0;
	it = 0;
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 3; i++) {
			if (sides[it]) {
				newSides.texture_array[6 * s] = { (float)i / 3.0, (float)(j + 1) / 2.0 };
				newSides.texture_array[6 * s + 1] = { (float)(i + 1) / 3.0, (float)(j + 1) / 2.0 };
				newSides.texture_array[6 * s + 2] = { (float)(i + 1) / 3.0, (float)j / 2.0 };
				newSides.texture_array[6 * s + 3] = { (float)i / 3.0, (float)(j + 1) / 2.0 };
				newSides.texture_array[6 * s + 4] = { (float)(i + 1) / 3.0, (float)j / 2.0 };
				newSides.texture_array[6 * s + 5] = { (float)i / 3.0, (float)j / 2.0 };
				s++;
			}
			it++;
		}
	}

	newSides.texture.generate(textureKey);
	newSides.ctm = glm::translate(newSides.ctm, pos);

	glGenVertexArrays(1, &newSides.VAO);
	glBindVertexArray(newSides.VAO);
	glGenBuffers(1, &newSides.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newSides.VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) + sizeof(glm::vec2)) * newSides.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newSides.num_vertices, newSides.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newSides.num_vertices, sizeof(glm::vec2) * newSides.num_vertices, newSides.texture_array);
	vPosition = glGetAttribLocation(shaderMap["simple_texture_shader"].ID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vTexCoords = glGetAttribLocation(shaderMap["simple_texture_shader"].ID, "vTexCoords");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newSides.num_vertices));

	newSides.ctm_location = glGetUniformLocation(shaderMap["simple_texture_shader"].ID, "ctm");
	newSides.selected_location = glGetUniformLocation(shaderMap["simple_texture_shader"].ID, "selected");

	objects.push_back(newSides);

	return;
}

void Graphics::genWorld(std::vector<std::vector<std::vector<int>>> world, std::map<int, std::string> blockMap)
{
	int i, j, k, s;
	std::bitset<6> sides;

	for(i = 0; i < world.size(); i++) {
		for(j = 0; j < world[i].size(); j++) {
			for(k = 0; k < world[i][j].size(); k++) {
				for (s = 0; s < 6; s++) {
					sides[s] = false;
				}
				
				//Front
				if (k + 1 >= world[i][j].size()) {
					sides[0] = true;
				}
				else if (!world[i][j][k + 1]) { 
					sides[0] = true;
				}

				//Right
				if (i + 1 >= world.size()) {
					sides[1] = true;
				}
				else if (!world[i + 1][j][k]) { 
					sides[1] = true;
				}

				//Back
				if (k - 1 < 0) {
					sides[2] = true;
				}
				else if (!world[i][j][k - 1]) { 
					sides[2] = true;
				}

				//Left
				if (i - 1 < 0) {
					sides[3] = true;
				}
				else if (!world[i - 1][j][k]) { 
					sides[3] = true;
				}

				//Up
				if (j - 1 < 0) {
					sides[4] = true;
				}
				else if (!world[i][j - 1][k]) {
					sides[4] = true;
				}

				//Down
				if (j + 1 >= world[i].size()) {
					sides[5] = true;
				}
				else if (!world[i][j + 1][k]) {
					sides[5] = true;
				}
				
				if (world[i][j][k]) {
					genBlockSides(NULL, blockMap[world[i][j][k]], sides, glm::vec3(i, world[i].size() - j, k));
				}
			}
		}
	}
	return;
}

/*
Camera Movement:
0 = Forward, 1 = Left, 2 = Back, 3 = Right, 4 = Up, 5 = Down
6 = Movement Modifier
*/
void Graphics::moveCamera(int action, GLfloat modifier)
{
	switch (action) {
	case 0:
		cameraPos = cameraPos + camera_move_speed * cameraFront;
		break;
	case 1:
		cameraPos = cameraPos - camera_move_speed * glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	case 2:
		cameraPos = cameraPos - camera_move_speed * cameraFront;
		break;
	case 3:
		cameraPos = cameraPos + camera_move_speed * glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	case 4:
		cameraPos = cameraPos + camera_move_speed * cameraUp;
		break;
	case 5:
		cameraPos = cameraPos - camera_move_speed * cameraUp;
		break;
	case 6:
		camera_move_speed = modifier;
		break;
	}
	model_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Graphics::look(GLfloat xOffset, GLfloat yOffset)
{
	theta_xz += camera_look_speed_vertical * xOffset;
	theta_y += camera_look_speed_horizontal * yOffset;
	if (theta_xz > 360.0f) {
		theta_xz -= 360.0f;
	}
	if (theta_xz < 0.0f) {
		theta_xz += 360.0f;
	}
	if (theta_y > 179.0f) {
		theta_y = 179.0f;
	}
	if (theta_y < 1.0f) {
		theta_y = 1.0f;
	}
	cameraFront = glm::normalize(glm::vec3(glm::sin(glm::radians(theta_xz)) * glm::sin(glm::radians(theta_y)), glm::cos(glm::radians(theta_y)), glm::cos(glm::radians(theta_xz)) * glm::sin(glm::radians(theta_y))));
	model_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}