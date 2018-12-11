/*
* project2.c
*
*      Author: Mitchell Leng
*/

#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "../Headers/initShader.h"
#include "glm/glm.hpp"

#include <stdio.h>
#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef struct {
	int north_wall;
	int east_wall;
	int south_wall;
	int west_wall;
} cell;

void idle(void);
void display(void);
void init(void);

void generate_maze();
void one_path(int x, int y, int w, int h);
void readable_maze();
void print_maze();
void generate_3d_maze(vec4 *v, vec4 *c, vec2 *t);

mat4 look_at(vec4 eyePoint, vec4 atPoint, vec4 upVector);
mat4 frustum(GLfloat fleft, GLfloat fright, GLfloat fbottom, GLfloat ftop, GLfloat fnear, GLfloat ffar);

mat4 scale(GLfloat x, GLfloat y, GLfloat z);
mat4 translate(GLfloat x, GLfloat y, GLfloat z);

mat4 rotate_x(GLfloat theta);
mat4 rotate_y(GLfloat theta);
mat4 rotate_z(GLfloat theta);

vec2 brick_tex_coords[6] = { { 0.0, 0.5 },{ 0.5, 0.5 },{ 0.5, 0.0 },{ 0.0, 0.5 },{ 0.5, 0.0 },{ 0.0, 0.0 } };
vec2 stone_tex_coords[6] = { { 0.5, 0.5 },{ 1.0, 0.5 },{ 1.0, 0.0 },{ 0.5, 0.5 },{ 1.0, 0.0 },{ 0.5, 0.0 } };
vec2 grass_tex_coords[6] = { { 0.0, 1.0 },{ 0.5, 1.0 },{ 0.5, 0.5 },{ 0.0, 1.0 },{ 0.5, 0.5 },{ 0.0, 0.5 } };
vec2 corn_tex_coords[6] = { { 0.5, 1.0 },{ 1.0, 1.0 },{ 1.0, 0.5 },{ 0.5, 1.0 },{ 1.0, 0.5 },{ 0.5, 0.5 } };

int num_vertices = 0;
int wall_count = 79;
int pillar_count = 81;
cell maze[8][8];
int maze_horizontal[8][9];
int maze_vertical[9][8];
vec4 maze_easy_move[10][10];

int cube_num_vertices = 36;
vec4 cube_vertices[36] = { {-0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, //Front
						{0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, //Right
						{0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //Back
						{-0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Left
						{-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Up
						{-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0} }; //Down

GLuint model_view_location;
GLuint projection_location;
GLuint ctm_location;

mat4 model_view = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 projection = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };

int animation_phase = 0;
GLfloat alpha_max = 0;
GLfloat alpha = 0.001;
vec4 currentPoint;
vec4 targetPoint;
vec4 pointPath;
vec4 currentEye;
vec4 targetEye;
vec4 eyePath;

int maze_solved = 0;
int move_x;
int move_z;
int move_dir; //North = 0; East = 1; South = 2; West = 3;
int cell_x;
int cell_z;
int next_step;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Project2");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}

void init(void)
{
	int width = 800;
	int height = 800;
	GLubyte* my_texels = (GLubyte*)malloc(sizeof(GLubyte) * width * height * 3);

	FILE *fp = fopen("p2texture04.raw", "r");
	fread(my_texels, width * height * 3, 1, fp);
	fclose(fp);

	generate_maze();
	num_vertices = (wall_count + pillar_count + 1) * cube_num_vertices;
	vec4 *vertices = (vec4 *)malloc(sizeof(vec4) * num_vertices);
	vec4 *colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
	vec2 *textures = (vec2 *)malloc(sizeof(vec2) * num_vertices);
	generate_3d_maze(vertices, colors, textures);

	GLuint program = initShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint mytex;
	glGenTextures(1, &mytex);
	glBindTexture(GL_TEXTURE_2D, mytex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int param;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (2 * sizeof(vec4) + sizeof(vec2)) * num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(vec4) * num_vertices, sizeof(vec2) * num_vertices, textures);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(0 + sizeof(vec4) * num_vertices));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0 + 2 * sizeof(vec4) * num_vertices));

	GLuint texture_location = glGetUniformLocation(program, "texture");
	glUniform1i(texture_location, 0);

	model_view_location = glGetUniformLocation(program, "model_view_matrix");
	projection_location = glGetUniformLocation(program, "projection_matrix");
	ctm_location = glGetUniformLocation(program, "ctm");

	model_view = look_at(v4(1, 1.5, 1, 1), v4(0, 0, 0, 1), v4(0, 1, 0, 0));
	projection = frustum(1, -1, 1, -1, -1, 0.01);
	//ctm = mat4_mat4_multiplication(translate(0, 0, -2), mat4_mat4_multiplication(rotate_x(-45), mat4_mat4_multiplication(rotate_z(180), ctm)));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glClearColor(0.0, 0.8, 1.0, 1.0);
	glDepthRange(1, 0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm);

	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	glutSwapBuffers();
}

void idle(void)
{
	switch (animation_phase) {
		case 0: //Fly around
			if (alpha_max == 0.0) {
				currentPoint = v4(1.5*cos((3 * M_PI) / 2), 1.5, 1.5*sin((3 * M_PI) / 2), 1.0);
				currentEye = v4(0.0, 0.0, 0.0, 1.0);
				model_view = look_at(currentPoint, currentEye, v4(0, 1, 0, 0));
				alpha_max = (3*M_PI)/2;
			}
			if(alpha_max < (7*M_PI)/2) {
				currentPoint = v4(1.5*cos(alpha_max), 1.5, 1.5*sin(alpha_max), 1.0);
				model_view = look_at(currentPoint, currentEye, v4(0, 1, 0, 0));
				alpha_max += alpha;
			} else {
				animation_phase = 1;
				alpha_max = 0.0;
			}
			break;
		case 1: //Fly down
			if (alpha_max == 0.0) {
				targetPoint = maze_easy_move[1][0];
				targetEye = maze_easy_move[1][1];
				pointPath = vec4_subtraction(targetPoint, currentPoint);
				eyePath = vec4_subtraction(targetEye, currentEye);
			}
			if (alpha_max < 1.0) {
				currentEye = vec4_addition(currentEye, vec4_scalar_multiplication(alpha, eyePath));
				currentPoint = vec4_addition(currentPoint, vec4_scalar_multiplication(alpha, pointPath)); 
				model_view = look_at(currentPoint, currentEye, v4(0, 1, 0, 0));
				alpha_max += alpha;
			} else {
				animation_phase = 2;
				alpha_max = 0.0;
			}
			break;
		case 2: //Enter Maze
			if (alpha_max == 0.0) {
				alpha *= 2;
				targetPoint = maze_easy_move[1][1];
				targetEye = maze_easy_move[1][2];
				pointPath = vec4_subtraction(targetPoint, currentPoint);
				eyePath = vec4_subtraction(targetEye, currentEye);
			}
			if (alpha_max < 1.0) {
				currentEye = vec4_addition(currentEye, vec4_scalar_multiplication(alpha, eyePath));
				currentPoint = vec4_addition(currentPoint, vec4_scalar_multiplication(alpha, pointPath));
				model_view = look_at(currentPoint, currentEye, v4(0, 1, 0, 0));
				alpha_max += alpha;
			}
			else {
				animation_phase = 3;
				alpha_max = 0.0;
			}
			break;
		case 3: //Solve Maze
			if (maze_solved == 0) {
				cell_x = 0;
				cell_z = 0;
				move_x = 1;
				move_z = 1;
				move_dir = 2; //North = 0; East = 1; South = 2; West = 3;
				maze_solved = 1;
			}
			if (alpha_max == 0.0 && maze_solved == 1) {
				if (move_dir == 0) { //North
					//Move Forward
					if ((maze[cell_x][cell_z].east_wall && !maze[cell_x][cell_z].north_wall) || next_step) {
						targetPoint = maze_easy_move[move_x][move_z - 1];
						targetEye = maze_easy_move[move_x][move_z - 2];
						move_z--;
						cell_z--;
						if (next_step) { next_step = 0; }
					}
					//Turn Left
					else if (maze[cell_x][cell_z].east_wall && maze[cell_x][cell_z].north_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x - 1][move_z];
						move_dir = 3;
					}
					//Turn Right
					else if (!maze[cell_x][cell_z].east_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x + 1][move_z];
						move_dir = 1;
						next_step = 1;
					}
				}
				else if (move_dir == 1) { //East
					//Move Forward
					if ((maze[cell_x][cell_z].south_wall && !maze[cell_x][cell_z].east_wall) || next_step) {
						targetPoint = maze_easy_move[move_x + 1][move_z];
						targetEye = maze_easy_move[move_x + 2][move_z];
						move_x++;
						cell_x++;
						if (next_step) { next_step = 0; }
					}
					//Turn Left
					else if (maze[cell_x][cell_z].south_wall && maze[cell_x][cell_z].east_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x][move_z - 1];
						move_dir = 0;
					}
					//Turn Right
					else if (!maze[cell_x][cell_z].south_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x][move_z + 1];
						move_dir = 2;
						next_step = 1;
					}

				}
				else if (move_dir == 2) { //South
					//Move Forward
					if ((maze[cell_x][cell_z].west_wall && !maze[cell_x][cell_z].south_wall) || next_step) {
						targetPoint = maze_easy_move[move_x][move_z + 1];
						targetEye = maze_easy_move[move_x][move_z + 2];
						move_z++;
						cell_z++;
						if (next_step) { next_step = 0; }
					}
					//Turn Left
					else if (maze[cell_x][cell_z].west_wall && maze[cell_x][cell_z].south_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x + 1][move_z];
						move_dir = 1;
					}
					//Turn Right
					else if (!maze[cell_x][cell_z].west_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x - 1][move_z];
						move_dir = 3;
						next_step = 1;
					}
				}
				else if (move_dir == 3) { //West
					//Move Forward
					if ((maze[cell_x][cell_z].north_wall && !maze[cell_x][cell_z].west_wall) || next_step) {
						targetPoint = maze_easy_move[move_x - 1][move_z];
						targetEye = maze_easy_move[move_x - 2][move_z];
						move_x--;
						cell_x--;
						if (next_step) { next_step = 0; }
					}
					//Turn Left
					else if (maze[cell_x][cell_z].north_wall && maze[cell_x][cell_z].west_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x][move_z + 1];
						move_dir = 2;
					}
					//Turn Right
					else if (!maze[cell_x][cell_z].north_wall) {
						targetPoint = maze_easy_move[move_x][move_z];
						targetEye = maze_easy_move[move_x][move_z - 1];
						move_dir = 0;
						next_step = 1;
					}
				}
				pointPath = vec4_subtraction(targetPoint, currentPoint);
				eyePath = vec4_subtraction(targetEye, currentEye);
				maze_solved = 2;
			}
			if (alpha_max < 1.0 && maze_solved == 2) {
				currentEye = vec4_addition(currentEye, vec4_scalar_multiplication(alpha, eyePath));
				currentPoint = vec4_addition(currentPoint, vec4_scalar_multiplication(alpha, pointPath));
				model_view = look_at(currentPoint, currentEye, v4(0, 1, 0, 0));
				alpha_max += alpha;
			}
			else {
				currentEye = targetEye;
				currentPoint = targetPoint;
				alpha_max = 0.0;
				maze_solved = 1;
				if (cell_x == 7 && cell_z == 7 && move_dir == 2) {
					maze_solved = 3;
					animation_phase = 4;
				}
			}
			break;

	}
	glutPostRedisplay();
}

void generate_maze()
{
	int i, j;
	//Initialize
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			maze[i][j].north_wall = 0;
			maze[i][j].east_wall = 0;
			maze[i][j].south_wall = 0;
			maze[i][j].west_wall = 0;
		}
	}
	//Set Borders
	for (i = 0; i < 8; i++) {
		maze[i][0].north_wall = 1;
		maze[7][i].east_wall = 1;
		maze[i][7].south_wall = 1;
		maze[0][i].west_wall = 1;
	}
	maze[0][0].north_wall = 0;
	maze[7][7].south_wall = 0;
	//Generate Maze
	srand(time(0));
	one_path(0, 0, 8, 8);
	readable_maze();
	print_maze();
}

void one_path(int x, int y, int w, int h)
{
	if (w < 2 || h < 2)
		return;
	
	int random_side = rand() % 4;
	int random_wall;
	int i;

	//Split Horizontal
	for (i = x; i < x + w; i++) {
		maze[i][y + h / 2].north_wall = 1;
		maze[i][y + h / 2 - 1].south_wall = 1;
	}
	//Split Vertical
	for (i = y; i < y + h; i++) {
		maze[x + w / 2][i].west_wall = 1;
		maze[x + w / 2 - 1][i].east_wall = 1;
	}
	//Remove 3 random walls from 3 different sides
	switch (random_side) {
		case 0: 
			//random_wall = x + rand() % (w / 2);
			//maze[random_wall][y + h / 2].north_wall = 0;
			//maze[random_wall][y + h / 2 - 1].south_wall = 0;
			random_wall = y + rand() % (h / 2);
			maze[x + w / 2][random_wall].west_wall = 0;
			maze[x + w / 2 - 1][random_wall].east_wall = 0;
			random_wall = x + w / 2 + rand() % (w / 2);
			maze[random_wall][y + h / 2].north_wall = 0;
			maze[random_wall][y + h / 2 - 1].south_wall = 0;
			random_wall = y + h / 2 + rand() % (h / 2);
			maze[x + w / 2][random_wall].west_wall = 0;
			maze[x + w / 2 - 1][random_wall].east_wall = 0;
			break;
		case 1:
			random_wall = x + rand() % (w / 2);
			maze[random_wall][y + h / 2].north_wall = 0;
			maze[random_wall][y + h / 2 - 1].south_wall = 0;
			//random_wall = y + rand() % (h / 2);
			//maze[x + w / 2][random_wall].west_wall = 0;
			//maze[x + w / 2 - 1][random_wall].east_wall = 0;
			random_wall = x + w / 2 + rand() % (w / 2);
			maze[random_wall][y + h / 2].north_wall = 0;
			maze[random_wall][y + h / 2 - 1].south_wall = 0;
			random_wall = y + h / 2 + rand() % (h / 2);
			maze[x + w / 2][random_wall].west_wall = 0;
			maze[x + w / 2 - 1][random_wall].east_wall = 0;
			break;
		case 2:
			random_wall = x + rand() % (w / 2);
			maze[random_wall][y + h / 2].north_wall = 0;
			maze[random_wall][y + h / 2 - 1].south_wall = 0;
			random_wall = y + rand() % (h / 2);
			maze[x + w / 2][random_wall].west_wall = 0;
			maze[x + w / 2 - 1][random_wall].east_wall = 0;
			//random_wall = x + w / 2 + rand() % (w / 2);
			//maze[random_wall][y + h / 2].north_wall = 0;
			//maze[random_wall][y + h / 2 - 1].south_wall = 0;
			random_wall = y + h / 2 + rand() % (h / 2);
			maze[x + w / 2][random_wall].west_wall = 0;
			maze[x + w / 2 - 1][random_wall].east_wall = 0;
			break;
		case 3:
			random_wall = x + rand() % (w / 2);
			maze[random_wall][y + h / 2].north_wall = 0;
			maze[random_wall][y + h / 2 - 1].south_wall = 0;
			random_wall = y + rand() % (h / 2);
			maze[x + w / 2][random_wall].west_wall = 0;
			maze[x + w / 2 - 1][random_wall].east_wall = 0;
			random_wall = x + w / 2 + rand() % (w / 2);
			maze[random_wall][y + h / 2].north_wall = 0;
			maze[random_wall][y + h / 2 - 1].south_wall = 0;
			//random_wall = y + h / 2 + rand() % (h / 2);
			//maze[x + w / 2][random_wall].west_wall = 0;
			//maze[x + w / 2 - 1][random_wall].east_wall = 0;
			break;
	}
	one_path(x, y, w / 2, h / 2);
	one_path(x + w / 2, y, w / 2, h / 2);
	one_path(x, y + h / 2, w / 2, h / 2);
	one_path(x + w / 2, y + h / 2, w / 2, h / 2);
}

void readable_maze()
{
	int i, j;
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			if (maze[i][j].north_wall) {
				maze_horizontal[i][j] = 1;
				//wall_count++;
			} else {
				maze_horizontal[i][j] = 0;
			}
		}
		for (i = 0; i < 8; i++) {
			if (maze[i][j].west_wall) {
				maze_vertical[i][j] = 1;
				//wall_count++;
			} else {
				maze_vertical[i][j] = 0;
			}
			if (i == 7) {
				if (maze[i][j].east_wall) {
					maze_vertical[i+1][j] = 1;
					//wall_count++;
				} else {
					maze_vertical[i+1][j] = 0;
				}
			}
		}
		if (j == 7) {
			for (int i = 0; i < 8; i++) {
				if (maze[i][j].south_wall) {
					maze_horizontal[i][j+1] = 1;
					//wall_count++;
				} else {
					maze_horizontal[i][j+1] = 0;
				}
			}
		}
	}
}

void print_maze()
{
	int i, j;
	for (j = 0; j < 9; j++) {
		for (i = 0; i < 8; i++) {
			printf("+");
			if (maze_horizontal[i][j])
				printf("---");
			else
				printf("   ");
		}
		printf("+\n");
		if(j < 8) {
			for (i = 0; i < 9; i++) {
				if (maze_vertical[i][j])
					printf("|");
				else
					printf(" ");
				printf("   ");
			}
			printf("\n");
		}
	}
}

void generate_3d_maze(vec4 *v, vec4 *c, vec2 *t)
{
	GLfloat grass_sizing = 2.0f;
	GLfloat maze_sizing = 1.5f;
	GLfloat maze_x = -0.75f;
	GLfloat maze_z = -0.75f;
	GLfloat wall_length = maze_sizing / 8.0f;
	GLfloat pillar_length = wall_length / 4.0f;
	GLfloat pillar_width = pillar_length;
	GLfloat wall_width = 0.75f * pillar_width;
	GLfloat pillar_height = wall_length;
	GLfloat wall_height = 0.95f * pillar_height;
	
	int i, j, k;
	int it = 0;

	//Maze Easy Move
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			maze_easy_move[i][j] = v4(maze_x + wall_length * (i - 0.5), wall_height / 2, maze_z + wall_length * (j - 0.5), 1.0);
		}
	}

	//Pillars
	for(i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			for (k = 0; k < cube_num_vertices; k++) {
				v[it] = mat4_vec4_multiplication(translate(maze_x + (i * wall_length), pillar_height / 2, maze_z + (j * wall_length)), mat4_vec4_multiplication(scale(pillar_length, pillar_height, pillar_width), cube_vertices[k]));
				c[it] = v4(0.0, 0.0, 0.0, 1.0);
				t[it] = stone_tex_coords[k % 6];
				it++;
			}
		}
	}
	
	//Horizontal Walls
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 9; j++) {
			if (maze_horizontal[i][j]) {
				for (k = 0; k < cube_num_vertices; k++) {
					v[it] = mat4_vec4_multiplication(translate(maze_x + (i * wall_length) + wall_length / 2, wall_height / 2, maze_z + (j * wall_length)), mat4_vec4_multiplication(scale(wall_length, wall_height, wall_width), cube_vertices[k]));
					c[it] = v4(1.0, 0.0, 0.0, 1.0);
					t[it] = brick_tex_coords[k % 6];
					it++;
				}
			}
		}
	}

	//Vertical Walls
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 8; j++) {
			if (maze_vertical[i][j]) {
				for (k = 0; k < cube_num_vertices; k++) {
					v[it] = mat4_vec4_multiplication(translate(maze_x + (i * wall_length), wall_height / 2, maze_z + (j * wall_length) + wall_length / 2), mat4_vec4_multiplication(scale(wall_width, wall_height, wall_length), cube_vertices[k]));
					c[it] = v4(1.0, 0.0, 0.0, 1.0);
					t[it] = brick_tex_coords[k % 6];
					it++;
				}
			}
		}
	}

	// Grass base
	for (i = 0; i < cube_num_vertices; i++) {
		v[it] = mat4_vec4_multiplication(translate(0.0, 0.0, 0.0), mat4_vec4_multiplication(scale(grass_sizing, 0.0, grass_sizing), cube_vertices[i]));
		c[it] = v4(0.0, 1.0, 0.0, 1.0);
		t[it] = grass_tex_coords[i % 6];
		it++;
	}
}

mat4 look_at(vec4 eye, vec4 target, vec4 upVector)
{
	vec4 forward = vec4_norm(vec4_subtraction(eye, target));
	vec4 left = vec4_norm(vec4_cross_product(upVector, forward));
	vec4 up = vec4_cross_product(forward, left);

	mat4 result;
	result.x.x = left.x;	result.y.x = left.y;	result.z.x = left.z;	result.w.x = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
	result.x.y = up.x;		result.y.y = up.y;		result.z.y = up.z;		result.w.y = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
	result.x.z = forward.x; result.y.z = forward.y;	result.z.z = forward.z; result.w.z = -forward.x * eye.x - forward.y * eye.y - forward.z * eye.z;
	result.x.w = 0.0f;		result.y.w = 0.0f;		result.z.w = 0.0f;		result.w.w = 1.0f;
	return result;
}

mat4 frustum(GLfloat fleft, GLfloat fright, GLfloat fbottom, GLfloat ftop, GLfloat fnear, GLfloat ffar)
{
	mat4 result;
	result.x.x = (2 * fnear) / (fright - fleft);	result.y.x = 0.0f;								result.z.x = (fright + fleft) / (fright - fleft);	result.w.x = 0.0f;
	result.x.y = 0.0f;								result.y.y = (2 * fnear) / (ftop - fbottom);	result.z.y = (ftop + fbottom) / (ftop - fbottom);	result.w.y = 0.0f;
	result.x.z = 0.0f;								result.y.z = 0.0f;								result.z.z = -((fnear + ffar) / (ffar - fnear));	result.w.z = -((2 * ffar * fnear) / (ffar - fnear));
	result.x.w = 0.0f;								result.y.w = 0.0f;								result.z.w = -1.0f;									result.w.w = 0.0f;
	return result;
}

mat4 scale(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 result = { { x, 0.0, 0.0, 0.0 },{ 0.0, y, 0.0, 0.0 },{ 0.0, 0.0, z, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	return result;
}

mat4 translate(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 result = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	result.w.x = x;
	result.w.y = y;
	result.w.z = z;
	return result;
}

mat4 rotate_x(GLfloat theta)
{
	mat4 result;
	float s = (M_PI / 180.0f)*theta;
	result.x.x = 1.0f; result.y.x = 0.0f;	result.z.x = 0.0f;		result.w.x = 0.0f;
	result.x.y = 0.0f; result.y.y = cos(s); result.z.y = -sin(s);	result.w.y = 0.0f;
	result.x.z = 0.0f; result.y.z = sin(s); result.z.z = cos(s);	result.w.z = 0.0f;
	result.x.w = 0.0f; result.y.w = 0.0f;	result.z.w = 0.0f;		result.w.w = 1.0f;
	return result;
}

mat4 rotate_y(GLfloat theta)
{
	mat4 result;
	float s = (M_PI / 180.0f)*theta;
	result.x.x = cos(s);	result.y.x = 0.0f; result.z.x = sin(s); result.w.x = 0.0f;
	result.x.y = 0.0f;		result.y.y = 1.0f; result.z.y = 0.0f;	result.w.y = 0.0f;
	result.x.z = -sin(s);	result.y.z = 0.0f; result.z.z = cos(s); result.w.z = 0.0f;
	result.x.w = 0.0f;		result.y.w = 0.0f; result.z.w = 0.0f;	result.w.w = 1.0f;
	return result;
}

mat4 rotate_z(GLfloat theta)
{
	mat4 result;
	float s = (M_PI / 180.0f)*theta;
	result.x.x = cos(s);	result.y.x = -sin(s);	result.z.x = 0.0f; result.w.x = 0.0f;
	result.x.y = sin(s);	result.y.y = cos(s);	result.z.y = 0.0f; result.w.y = 0.0f;
	result.x.z = 0.0f;		result.y.z = 0.0f;		result.z.z = 1.0f; result.w.z = 0.0f;
	result.x.w = 0.0f;		result.y.w = 0.0f;		result.z.w = 0.0f; result.w.w = 1.0f;
	return result;
}


