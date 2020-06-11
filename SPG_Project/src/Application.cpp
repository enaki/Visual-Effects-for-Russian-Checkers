#ifdef __WIN32___
	#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Application.hpp"
#include "utilities.hpp"
#include "grafix.hpp"
#include <queue>
#include <algorithm>
#include <list>
#include "data.hpp"
#include "Game.hpp"


//functia main in care initializam rutina OpenGL si Glut
int main(int argc, char** argv) {
#ifdef __WIN32__
	FreeConsole();
#endif
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(550, 550);
	glutInitWindowPosition(100, 100);
	WIN = glutCreateWindow("Joc de Dame - Curcudel Eugen");
	createMenu();

	keyboard::WIN = WIN;
	glClearColor(0.9, 0.9, 0.9, 0.9);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-275.0, 275.0, -275.0, 275.0, 0.0, 1.0);
	boardInit();

	glutPassiveMotionFunc(passiveMotion);
	glutDisplayFunc(display);
	timer(0);
	glutMouseFunc(mouse::mouse_listener);
	glutMotionFunc(mouse::motion_listener);
	glutKeyboardFunc(keyboard::keyboard_listener);
	glutMainLoop();
}


//este rechemata recursiv pe parcursul programului
//si permite lucrul in timp real
void timer(int s) {
	if (!checkList.empty()) checkList.clear();
	if (!jumpList.empty()) jumpList.clear();
	if (!moveList.empty()) moveList.clear();
	

	listOfJumpes(jumpList, checkList);
	listOfMoves(moveList);

	display();

	int i, j;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			if (mouse::MOUSEX < _board[i][j].x + 30 && mouse::MOUSEX > _board[i][j].x - 30 && mouse::MOUSEY < _board[i][j].y + 30 && mouse::MOUSEY > _board[i][j].y - 30)
				if (mouse::PRESSED) {
					to.first = i;
					to.second = j;
				}


	if (sel.first != -1 && to.first != -1) {
		copyArray(_board, _undoBoard);
		if (moveIsLegal(1))
			putChecker(); //recursie indirecta catre putChecker() care va redirectiona spre 
						//timer() in caz ca jucatorul are o serie de sarituri
	}


	//rechemarea recursiva a functiei timer()
	glutTimerFunc(10, timer, 0);
}


//daca o miscare este posibila, atunci aceasta este efectuata
void putChecker() {
	_board[to.first][to.second].check = _board[sel.first][sel.second].check;
	_board[to.first][to.second].type = _board[sel.first][sel.second].type;
	_board[sel.first][sel.second].check = NO_CHECKER;
	_board[sel.first][sel.second].type = CHECKER;


	if (!jumpList.empty()) jumpList.clear();

	listOfJumpes(jumpList, checkList);

	if (JUMPED && listContainElement(jumpList, to.first, to.second)) {
		JUMPED = 0;
		mouse::PRESSED = 0;

		sel.first = to.first;
		sel.second = to.second;

		timer(0);//recursie indirecta catre timer() in caz ca userul are o serie de sarituri	
	}
	else {

		GO = (GO == WHITE_CHECKER) ? BLACK_CHECKER : WHITE_CHECKER;
		JUMPED = 0;
		mouse::PRESSED = 0;

		display();

		if (ROTIRI) {
			sleep(1);
			glRotatef(180, 0, 0, 1);
			mouse::SIDE_COEF *= -1;
		}
		sel.first = sel.second = -1;
		to.first = to.second = -1;
		mouse::MOUSEX = mouse::MOUSEY = -240;
	}
}


//fixeaza coordonatele cursorului, chiar daca nu se apasa nici un buton
void passiveMotion(int x, int y) {
	int i, j;
	x = (x - 275) * mouse::SIDE_COEF;
	y = (y - 275) * (-1) * mouse::SIDE_COEF;

	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++) {
			//sageata cursorului devine "std::mina" sau "drag" deasupra pieselor
			if (_board[i][j].check != NO_CHECKER && x < _board[i][j].x + 30 && x > _board[i][j].x - 30 && y < _board[i][j].y + 30 && y > _board[i][j].y - 30) {
				glutSetCursor(GLUT_CURSOR_INFO);
				return;
			}
		}
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}



//initializarea tablei de dame -=Joc Nou=-
void boardInit() {
	int i, j, s = -240;
	float x, y;
	mouse::SIDE_COEF = 1;
	mouse::MOUSEX = mouse::MOUSEY = -241;
	sel.first = sel.second = -1;
	to.first = to.second = -1;

	for (i = 0; i < R; i++) {
		for (j = 0; j < C; j++) {

			x = (float)s + j * 120;
			y = (float)-240 + i * 60;

			_board[i][j].x = x + 30;
			_board[i][j].y = y + 30;
			if (i < 3)
				_board[i][j].check = TYPE1; //white checker
			else if (i > 4)
				_board[i][j].check = TYPE2; //black checker
			else
				_board[i][j].check = NO_CHECKER; //empty, no ckecker

			_board[i][j].type = CHECKER; //not King  

		}

		s = (s == -240) ? -180 : -240;
	}
	copyArray(_board, _undoBoard);
}

//da programul inapoi cu o miscare
void undo() {
	copyArray(_undoBoard, _board);
	GO = (GO == WHITE_CHECKER) ? BLACK_CHECKER : WHITE_CHECKER;
	if (ROTIRI) {
		glRotatef(180, 0, 0, 1);
		mouse::SIDE_COEF *= -1;
	}
	glutPostRedisplay();
}

//returneaza true daca piesele de tip "checker" nu mai au nici o miscare
bool noMoreMoves(int checker) {

	for (auto it = moveList.begin(); it != moveList.end(); ++it) {
		if (_board[it->first][it->second].check == checker) {
			return false;
		}
	}

	for (auto it = checkList.begin(); it != checkList.end(); ++it) {
		if (_board[it->first][it->second].check == checker) {
			return false;
		}
	}
	return true;
}



//definirea si structurarea meniului
void createMenu() {
	int options = glutCreateMenu(actionMenu);

	//submeniul "Optiuni"
	glutAddMenuEntry("Rotire 180", 3);
	glutAddMenuEntry("Rotiri on/off", 4);
	glutAddMenuEntry("Miscari Posibile on/off", 7);
	glutAddMenuEntry("Undo", 6);

	int menuId = glutCreateMenu(actionMenu);

	glutAddMenuEntry("Joc Nou", 1);
	glutAddMenuEntry("Continuare Joc", 2);
	glutAddMenuEntry("Salvare Joc", 5);
	glutAddSubMenu("Optiuni", options);
	glutAddMenuEntry("Ajutor", 8);

	glutAddMenuEntry("Iesire", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//actiunile la selectarea optiunilor din meniu
void actionMenu(int option) {
	switch (option) {
	case 0:
		glutDestroyWindow(WIN);
		exit(0);
	case 1:
		if (mouse::SIDE_COEF == -1)
			glRotatef(180, 0, 0, 1);
		TYPE1 = WHITE_CHECKER;
		TYPE2 = BLACK_CHECKER;
		GO = WHITE_CHECKER;
		boardInit();
		break;
	case 2:
		TYPE1 = WHITE_CHECKER;
		TYPE2 = BLACK_CHECKER;
		initFromFile();
		if (mouse::SIDE_COEF == -1)
			glRotatef(180, 0, 0, 1);
		break;
	case 3:
		mouse::SIDE_COEF *= -1;
		mouse::MOUSEX = mouse::MOUSEY = -241;
		sel.first = sel.second = -1;
		glRotatef(180, 0, 0, 1);
		break;
	case 4:
		ROTIRI = ROTIRI ? 0 : 1;
		break;
	case 5:
		saveToFile();
		break;
	case 6:
		if (!areIdentic(_undoBoard, _board))
			undo();
		break;
	case 7:
		POS_MOVES = (POS_MOVES) ? 0 : 1;
		break;
	case 8:
		HELP = 1;
		break;
	}

	glutPostRedisplay();
}


//desenarea grafica a tuturor miscarilor posibile
void drawPossibleMoves() {
	int i, j, ver = 1;
	int a = sel.first, b = sel.second, c = to.first, d = to.second, e = JUMPED;
	
	for (auto it = jumpList.begin(); it != jumpList.end(); ++it) {
		for (i = 0; i < 8; i++)
			for (j = 0; j < 4; j++) {
				sel.first = it->first;
				sel.second = it->second;
				to.first = i;
				to.second = j;
				if (moveIsLegal(0)) {
					if (JUMPED) {
						(GO == WHITE_CHECKER) ? glColor3f(0, 1, 0) : glColor3f(1, 0, 0);
						glBegin(GL_LINES);
						glVertex2f(_board[sel.first][sel.second].x, _board[sel.first][sel.second].y);
						glVertex2f(_board[to.first][to.second].x, _board[to.first][to.second].y);
						glEnd();
						ver = 0;
					}
				}
			}
	}

	if (ver) {
		for (auto it = moveList.begin(); it != moveList.end(); ++it) {
			for (i = 0; i < 8; i++)
				for (j = 0; j < 4; j++) {
					if (_board[it->first][it->second].check == GO) {
						sel.first = it->first;
						sel.second = it->second;
						to.first = i;
						to.second = j;
						if (moveIsLegal(0)) {
							if (!JUMPED) {
								(GO == WHITE_CHECKER) ? glColor3f(0, 1, 0) : glColor3f(1, 0, 0);
								glBegin(GL_LINES);
								glVertex2f(_board[sel.first][sel.second].x, _board[sel.first][sel.second].y);
								glVertex2f(_board[to.first][to.second].x, _board[to.first][to.second].y);
								glEnd();
							}
						}
					}
				}
		}
	}

	sel.first = a;
	sel.second = b;
	to.first = c;
	to.second = d;
	JUMPED = e;
}

//functia de desenare grafica principala
void display() {
	//curatam ecranul
	glClear(GL_COLOR_BUFFER_BIT);

	int i, j, s;
	float x, y;

	for (i = 0; i < R; i++) {
		for (j = 0; j < C; j++) {
			//desenam cite un patrat
			glBegin(GL_QUADS);
			glColor3f(0, 0, 0);
			glVertex2f(_board[i][j].x - 30, _board[i][j].y - 30);
			glVertex2f(_board[i][j].x + 30, _board[i][j].y - 30);
			glVertex2f(_board[i][j].x + 30, _board[i][j].y + 30);
			glVertex2f(_board[i][j].x - 30, _board[i][j].y + 30);
			glEnd();

			//transformam piesele in dame daca ajung in partea opusa
			if (i == 0 && _board[i][j].check == TYPE2)//'B')
				_board[i][j].type = KING;

			if (i == 7 && _board[i][j].check == TYPE1)//'W')
				_board[i][j].type = KING;

			//coloram piesele in dependenta de tip
			if (_board[i][j].check == WHITE_CHECKER)
				glColor3f(1.0, 1.0, 1.0); //Albe
			else if (_board[i][j].check == BLACK_CHECKER)
				glColor3f(0.2, 0.2, 0.8); //Albastre
			else
				continue;

			//selectarea unei piese
			if (_board[i][j].check == GO && mouse::MOUSEX < _board[i][j].x + 30 && mouse::MOUSEX > _board[i][j].x - 30 && mouse::MOUSEY < _board[i][j].y + 30 && mouse::MOUSEY > _board[i][j].y - 30) {
				if (!jumpList.empty()) {
					if (listContainElement(jumpList, i, j) && _board[i][j].check == GO) {
						if (mouse::PRESSED) {
							sel.first = i;
							sel.second = j;
						}

						//culoarea pieselor selectate in dependenta de tip
						if (_board[i][j].check == BLACK_CHECKER)
							glColor3f(0.2, 0.2, 0.6); //violeta
						if (_board[i][j].check == WHITE_CHECKER)
							glColor3f(0.7, 0.7, 0.7); //surie
					}

				}

				else if (!moveList.empty()) {
					if (listContainElement(moveList, i, j)) {
						if (mouse::PRESSED) {
							sel.first = i;
							sel.second = j;
						}

						//culoarea pieselor selectate in dependenta de tip
						if (_board[i][j].check == BLACK_CHECKER)
							glColor3f(0.2, 0.2, 0.6); //violeta
						if (_board[i][j].check == WHITE_CHECKER)
							glColor3f(0.7, 0.7, 0.7); //surie
					}

				}

			}

			//desenam piesele de joc (octagoane)
			//conform formulelor parametrice a cercului
			glBegin(GL_POLYGON);
			for (x = 0; x <= 2 * M_PI; x += 0.77) {
				glVertex2f(25 * cos(x) + (_board[i][j].x - 30 + _board[i][j].x + 30) / 2, 25 * sin(x) + (_board[i][j].y - 30 + _board[i][j].y + 30) / 2);
			}
			glEnd();

			//semnificatii:
			//x-30 x sting
			//x+30 x drept
			//y-30 y jos
			//y+30 y sus

			//desenam coronita la dame
			//coordonatele virfurilor deduse de std::mine
			if (_board[i][j].type == KING) {
				float* mx = (float*)malloc(sizeof(float));
				float* my = (float*)malloc(sizeof(float));
				if (!mx || !my) {
					glutDestroyWindow(WIN);
					exit(1);
				}
				*mx = _board[i][j].x;
				*my = _board[i][j].y;
				glBegin(GL_POLYGON);
				if (_board[i][j].check == WHITE_CHECKER)
					glColor3f(0.0, 0.0, 1.0);
				else if (_board[i][j].check == BLACK_CHECKER)
					glColor3f(0.9, 0.1, 0.1);
				glVertex2f(*mx, *my - 10 * mouse::SIDE_COEF);
				glVertex2f(*mx + 10, *my - 10 * mouse::SIDE_COEF);
				glVertex2f(*mx + 10, *my + 10 * mouse::SIDE_COEF);
				glVertex2f(*mx + 5, *my - 5 * mouse::SIDE_COEF);
				glVertex2f(*mx, *my + 10 * mouse::SIDE_COEF);
				glVertex2f(*mx - 5, *my - 5 * mouse::SIDE_COEF);
				glVertex2f(*mx - 10, *my + 10 * mouse::SIDE_COEF);
				glVertex2f(*mx - 10, *my - 10 * mouse::SIDE_COEF);
				glEnd();

				free(mx);
				free(my);
			}

		}
	}

	//desenam cifrele din stinga tablei, literele de jos
	//si doua linii de contur
	drawAround(mouse::SIDE_COEF);

	//Desenam toate miscarile posibile daca exista
	if (POS_MOVES)
		drawPossibleMoves();

	//desenam output-ul meniului "Ajutor"
	if (HELP) {
		showHelp(mouse::SIDE_COEF);
		if (mouse::PRESSED) {
			if (mouse::MOUSEX * mouse::SIDE_COEF > 190 && mouse::MOUSEY * mouse::SIDE_COEF > 140 && mouse::MOUSEX * mouse::SIDE_COEF < 210 && mouse::MOUSEY * mouse::SIDE_COEF < 160) {
				HELP = 0;
			}
		}
	}
	else {
		if (!checkList.empty()) checkList.clear();
		if (!jumpList.empty()) jumpList.clear();
		if (!moveList.empty()) moveList.clear();

		listOfJumpes(jumpList, checkList);
		listOfMoves(moveList);

		//desenam intro Joc de Dame de Curcudel Eugen, la inceput de joc
		if (TYPE1 == TYPE2)
			showIntro(mouse::SIDE_COEF);
		else {
			//afisam invingatorul sau detinatorul de miscare in timpul dat
			if (countCheckers(WHITE_CHECKER) == 0 || noMoreMoves(WHITE_CHECKER)) {
				showWiner("Negrele au invins!", mouse::SIDE_COEF);
				showTurn("Negrele au invins!", mouse::SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
			}
			else if (countCheckers(BLACK_CHECKER) == 0 || noMoreMoves(BLACK_CHECKER)) {
				showWiner("Albele au invins!", mouse::SIDE_COEF);
				showTurn("Albele au invins!", mouse::SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
			}
			else if (GO == WHITE_CHECKER)
				showTurn("Albele merg", mouse::SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
			else if (GO == BLACK_CHECKER)
				showTurn("Negrele merg", mouse::SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
		}
	}

	//curatam ecranul si schimbam buferele
	glFlush();
	glutSwapBuffers();
}
