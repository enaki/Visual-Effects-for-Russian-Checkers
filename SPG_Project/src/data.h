#pragma once

#include <list>

//definirea constantelor
#define ROWS			8
#define COLUMNS			4

#define NO_CHECKER		0
#define BLACK_CHECKER	1
#define WHITE_CHECKER	2

#define CHECKER			0
#define KING			1

#define M_PI 3.1415926535897932384626433832795

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