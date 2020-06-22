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


//definirea variabilelor globale
int WIN;

int TYPE1 = NO_CHECKER;
int TYPE2 = NO_CHECKER;
bool ROTIRI = false;
int GO = NO_CHECKER;
bool JUMPED = false;
bool POS_MOVES = false;
bool HELP = false;

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

glm::mat4 projectionMatrix, viewMatrix, modelMatrix;

glm::vec3 light_pos(0, 1, 5);
glm::vec3 view_pos(0, 0, 1);

glm::vec3 type1_color(1, 1, 1);	//white
glm::vec3 type1_selected_color(0.7, 0.7, 0.7);	//gray

glm::vec3 type2_color(0.1, 0.1, 1);	//blue
glm::vec3 type2_selected_color(0.2, 0.2, 0.8);	//blue pale

glm::vec3 board_square_color(0.5, 0.0, 0.0);	//dark red

GLint enable_lighting = 1;
GLint enable_texture = 0;
GLint enableNormal = 1;
