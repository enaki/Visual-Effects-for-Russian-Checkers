#pragma once
//definirea constantelor
#define R				8
#define C				4
#define BLACK_CHECKER	1
#define WHITE_CHECKER	2
#define NO_CHECKER		0
#define KING			1
#define CHECKER			0

#include <queue>
#include <list>

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

//structura reprezentarii tablei de dame
struct square {
	float x;
	float y;
	unsigned int check : 2;
	bool type : 1;
} _board[R][C], _undoBoard[R][C];

std::pair<int, int> sel, to;
//prototipurile functiilor
void boardInit();
void display();
void timer(int);
void createMenu();
void actionMenu(int);
void passiveMotion(int, int);
void putChecker();
void undo();
void copyArray(struct square from[R][C], struct square to[R][C]);
void sleep(unsigned int);
std::pair<int, int> coordsToIndex(int, int);
int moveIsLegal(int);
int areIdentic(struct square a[R][C], struct square b[R][C]);
void listOfJumpes(std::list<std::pair<int, int>>& jumpList, std::list<std::pair<int, int>>& checkList);
void listOfMoves(std::list<std::pair<int, int>>& moveList);
void saveToFile();
void initFromFile();
bool noMoreMoves(int);
int countCheckers(int);
void drawPossibleMoves();

