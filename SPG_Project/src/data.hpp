#pragma once

#include <queue>
#include <list>

//definirea constantelor
#define R				8
#define C				4
#define BLACK_CHECKER	1
#define WHITE_CHECKER	2
#define NO_CHECKER		0
#define KING			1
#define CHECKER			0

#define M_PI 3.1415926535897932384626433832795

//definirea variabilelor globale
int WIN;

int TYPE1 = NO_CHECKER;
int TYPE2 = NO_CHECKER;
int ROTIRI = 0;
int GO = NO_CHECKER;
int INDEXING = 0;
int JUMPED = 0;
int POS_MOVES = 0;
int HELP = 0;


std::list<std::pair<int, int>> jumpList;
std::list<std::pair<int, int>> checkList;
std::list<std::pair<int, int>> moveList;

//structura reprezentarii tablei de dame
struct square {
	float x;
	float y;
	unsigned int check : 2;
	bool type : 1;
};
square _board[R][C], _undoBoard[R][C];

std::pair<int, int> sel, to;