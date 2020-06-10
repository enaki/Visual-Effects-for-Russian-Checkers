#ifndef _GAME_
#define _GAME_

//definirea constantelor
#define R				8
#define C				4
#define BLACK_CHECKER	1
#define WHITE_CHECKER	2
#define NO_CHECKER		0
#define KING			1
#define CHECKER			0

//definirea variabilelor globale
int WIN;
int MOUSEX = 0;
int MOUSEY = 0;
int PRESSED = 0;
int TYPE1 = NO_CHECKER;
int TYPE2 = NO_CHECKER;
int SIDE_COEF;
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
	unsigned int check:2;
	unsigned int type:1;
} _board[R][C], _undoBoard[R][C];

//structura unei locatii pe tabla
struct location {
	int i;
	int j;
} sel, to;

//prototipurile functiilor
void boardInit();
void display();
void mouse(int, int, int, int);
void motion(int, int);
void timer(int);
void createMenu();
void actionMenu(int);
void keyboard(unsigned char, int, int);
void passiveMotion(int, int);
void putChecker();
void undo();
void copyArray(struct square from[R][C], struct square to[R][C]);
void sleep(unsigned int);
struct location coordsToIndex(int, int);
int moveIsLegal(int);
int max(int, int);
int min(int, int);
int areIdentic(struct square a[R][C], struct square b[R][C]);
void listOfMoves();
void listOfJumpes();
void saveToFile();
void initFromFile();
int noMoreMoves(int);
int countCheckers(int);
void drawPossibleMoves();

#endif
