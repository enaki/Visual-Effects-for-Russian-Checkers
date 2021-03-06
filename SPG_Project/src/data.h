#pragma once

#include <list>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec3.hpp>

//definirea constantelor
#define ROWS			8
#define COLUMNS			4

#define NO_CHECKER		0
#define BLACK_CHECKER	1
#define WHITE_CHECKER	2

#define CHECKER			0
#define KING			1

#define M_PI glm::pi<float>()

//uiManager
int MOUSEX = 0;
int MOUSEY = 0;
int PRESSED = 0;
int SIDE_COEF;

//definirea variabilelor globale
int WIN;

int TYPE1 = NO_CHECKER;
int TYPE2 = NO_CHECKER;
int ROTIRI = 0;
int GO = NO_CHECKER;
int JUMPED = 0;
int POS_MOVES = 0;
int HELP = 0;

std::list<std::pair<int, int>> jump_list;
std::list<std::pair<int, int>> check_list;
std::list<std::pair<int, int>> move_list;

//structura reprezentarii tablei de dame
struct square {
	float x;
	float y;
	unsigned int check : 2;
	bool type;
};
square board[ROWS][COLUMNS], undo_board[ROWS][COLUMNS];

std::pair<int, int> sel, to;

glm::mat4 projection_matrix, view_matrix, model_matrix;

glm::vec3 light_pos(0, 0, 5);
glm::vec3 view_pos(0, 0, 1);

glm::vec3 type1_color(1, 1, 1);	//white
glm::vec3 type1_selected_color(0.7, 0.7, 0.7);	//gray

glm::vec3 type2_color(0.1, 0.1, 0.5);	//blue
glm::vec3 type2_selected_color(0.2, 0.2, 0.8);	//blue pale

glm::vec3 board_square_color(0.5, 0.0, 0.0);	//dark red

GLint enable_lighting = 0;
GLint enable_texture = 0;
GLint enable_normal = 0;
GLint light_type = 1;
GLfloat spec_power = 32;

GLuint lighting_shader_programme, texture_shader_programme;
GLuint board_texture, board_texture_normal;

float board_squares[ROWS][COLUMNS][12];
float full_board[32] = {
	// positions							// colors			// texture coords
	-240.0f,	-240.0f,	0.0f,		1.0f, 0.0f, 1.0f,	0.0f, 1.0f,
	 240.0f,	-240.0f,	0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
	 240.0f,	 240.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	-240.0f,	 240.0f,	0.0f,		0.0f, 1.0f, 1.0f,	0.0f, 0.0f
};

float board_normalized[] = { -240.0f / 275,	-240.0f / 275,
							-240.0f / 275,	240.0f / 275,
							 240.0f / 275,	240.0f / 275,
							 240.0f / 275,	-240.0f / 275 };