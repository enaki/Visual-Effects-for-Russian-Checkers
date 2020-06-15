#ifdef __WIN32___
	#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "Application.h"
#include "utilities.h"
#include "grafix.hpp"
#include <queue>
#include <algorithm>
#include <list>
#include "data.h"
#include "Game.h"


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
	create_menu();

	uimanager::WIN = WIN;
	glClearColor(0.9, 0.9, 0.9, 0.9);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-275.0, 275.0, -275.0, 275.0, 0.0, 1.0);
	board_init();

	glutPassiveMotionFunc(passive_motion);
	glutDisplayFunc(display);
	timer(0);
	glutMouseFunc(uimanager::mouse_listener);
	glutMotionFunc(uimanager::motion_listener);
	glutKeyboardFunc(uimanager::keyboard_listener);
	glutMainLoop();
}


//recursive call during the program
void timer(int s) {
	if (!check_list.empty()) check_list.clear();
	if (!jump_list.empty()) jump_list.clear();
	if (!move_list.empty()) move_list.clear();
	

	list_of_jumpes(jump_list, check_list);
	list_of_moves(move_list);

	display();

	for (auto i = 0; i < ROWS; i++)
		for (auto j = 0; j < COLUMNS; j++)
			if (uimanager::MOUSEX < board[i][j].x + 30 && uimanager::MOUSEX > board[i][j].x - 30 && uimanager::MOUSEY < board[i][j].y + 30 && uimanager::MOUSEY > board[i][j].y - 30)
				if (uimanager::PRESSED) {
					to.first = i;
					to.second = j;
				}


	if (sel.first != -1 && to.first != -1) {
		copy_array(board, undo_board);
		if (move_is_legal(1))
			put_checker(); 
	}


	//rechemarea recursiva a functiei timer()
	glutTimerFunc(10, timer, 0);
}


//daca o miscare este posibila, atunci aceasta este efectuata
void put_checker() {
	board[to.first][to.second].check = board[sel.first][sel.second].check;
	board[to.first][to.second].type = board[sel.first][sel.second].type;
	board[sel.first][sel.second].check = NO_CHECKER;
	board[sel.first][sel.second].type = CHECKER;


	if (!jump_list.empty()) jump_list.clear();

	list_of_jumpes(jump_list, check_list);

	if (JUMPED && list_contain_element(jump_list, to.first, to.second)) {
		JUMPED = 0;
		uimanager::PRESSED = 0;

		sel.first = to.first;
		sel.second = to.second;

		timer(0);//recursie indirecta catre timer() in caz ca userul are o serie de sarituri	
	}
	else {

		GO = (GO == WHITE_CHECKER) ? BLACK_CHECKER : WHITE_CHECKER;
		JUMPED = 0;
		uimanager::PRESSED = 0;

		display();

		if (ROTIRI) {
			sleep(1);
			glRotatef(180, 0, 0, 1);
			uimanager::SIDE_COEF *= -1;
		}
		sel.first = sel.second = -1;
		to.first = to.second = -1;
		uimanager::MOUSEX = uimanager::MOUSEY = -240;
	}
}


//fixeaza coordonatele cursorului, chiar daca nu se apasa nici un buton
void passive_motion(int x, int y) {
	int i, j;
	x = (x - 275) * uimanager::SIDE_COEF;
	y = (y - 275) * (-1) * uimanager::SIDE_COEF;

	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++) {
			//sageata cursorului devine "std::mina" sau "drag" deasupra pieselor
			if (board[i][j].check != NO_CHECKER && x < board[i][j].x + 30 && x > board[i][j].x - 30 && y < board[i][j].y + 30 && y > board[i][j].y - 30) {
				glutSetCursor(GLUT_CURSOR_INFO);
				return;
			}
		}
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}



//initializarea tablei de dame -=Joc Nou=-
void board_init() {
	int s = -240;
	uimanager::SIDE_COEF = 1;
	uimanager::MOUSEX = uimanager::MOUSEY = -241;
	sel.first = sel.second = -1;
	to.first = to.second = -1;

	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLUMNS; j++) {
			auto x = static_cast<float>(s) + j * 120;
			auto y = static_cast<float>(-240) + i * 60;

			board[i][j].x = x + 30;
			board[i][j].y = y + 30;
			if (i < 3)
				board[i][j].check = TYPE1; //white checker
			else if (i > 4)
				board[i][j].check = TYPE2; //black checker
			else
				board[i][j].check = NO_CHECKER; //empty, no ckecker

			board[i][j].type = CHECKER; //not King  
		}

		s = (s == -240) ? -180 : -240;
	}
	copy_array(board, undo_board);
}

//da programul inapoi cu o miscare
void undo() {
	copy_array(undo_board, board);
	GO = (GO == WHITE_CHECKER) ? BLACK_CHECKER : WHITE_CHECKER;
	if (ROTIRI) {
		glRotatef(180, 0, 0, 1);
		uimanager::SIDE_COEF *= -1;
	}
	glutPostRedisplay();
}



//definirea si structurarea meniului
void create_menu() {
	int options = glutCreateMenu(action_menu);

	//submeniul "Optiuni"
	glutAddMenuEntry("Rotire 180", 3);
	glutAddMenuEntry("Rotiri on/off", 4);
	glutAddMenuEntry("Miscari Posibile on/off", 7);
	glutAddMenuEntry("Undo", 6);

	int menuId = glutCreateMenu(action_menu);

	glutAddMenuEntry("Joc Nou", 1);
	glutAddMenuEntry("Continuare Joc", 2);
	glutAddMenuEntry("Salvare Joc", 5);
	glutAddSubMenu("Optiuni", options);
	glutAddMenuEntry("Ajutor", 8);

	glutAddMenuEntry("Iesire", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//actiunile la selectarea optiunilor din meniu
void action_menu(int option) {
	switch (option) {
	case 0:
		glutDestroyWindow(WIN);
		exit(0);
	case 1:
		if (uimanager::SIDE_COEF == -1)
			glRotatef(180, 0, 0, 1);
		TYPE1 = WHITE_CHECKER;
		TYPE2 = BLACK_CHECKER;
		GO = WHITE_CHECKER;
		board_init();
		break;
	case 2:
		TYPE1 = WHITE_CHECKER;
		TYPE2 = BLACK_CHECKER;
		init_from_file();
		if (uimanager::SIDE_COEF == -1)
			glRotatef(180, 0, 0, 1);
		break;
	case 3:
		uimanager::SIDE_COEF *= -1;
		uimanager::MOUSEX = uimanager::MOUSEY = -241;
		sel.first = sel.second = -1;
		glRotatef(180, 0, 0, 1);
		break;
	case 4:
		ROTIRI = ROTIRI ? 0 : 1;
		break;
	case 5:
		save_to_file();
		break;
	case 6:
		if (!are_identic(undo_board, board))
			undo();
		break;
	case 7:
		POS_MOVES = (POS_MOVES) ? 0 : 1;
		break;
	case 8:
		HELP = 1;
		break;
	default: break;
	}

	glutPostRedisplay();
}


//desenarea grafica a tuturor miscarilor posibile
void draw_possible_moves() {
	int i, j, ver = 1;
	int a = sel.first, b = sel.second, c = to.first, d = to.second, e = JUMPED;
	
	for (auto& it : jump_list)
	{
		for (i = 0; i < 8; i++)
			for (j = 0; j < 4; j++) {
				sel.first = it.first;
				sel.second = it.second;
				to.first = i;
				to.second = j;
				if (move_is_legal(0)) {
					if (JUMPED) {
						(GO == WHITE_CHECKER) ? glColor3f(0, 1, 0) : glColor3f(1, 0, 0);
						glBegin(GL_LINES);
						glVertex2f(board[sel.first][sel.second].x, board[sel.first][sel.second].y);
						glVertex2f(board[to.first][to.second].x, board[to.first][to.second].y);
						glEnd();
						ver = 0;
					}
				}
			}
	}

	if (ver) {
		for (auto& it : move_list)
		{
			for (i = 0; i < 8; i++)
				for (j = 0; j < 4; j++) {
					if (board[it.first][it.second].check == GO) {
						sel.first = it.first;
						sel.second = it.second;
						to.first = i;
						to.second = j;
						if (move_is_legal(0)) {
							if (!JUMPED) {
								(GO == WHITE_CHECKER) ? glColor3f(0, 1, 0) : glColor3f(1, 0, 0);
								glBegin(GL_LINES);
								glVertex2f(board[sel.first][sel.second].x, board[sel.first][sel.second].y);
								glVertex2f(board[to.first][to.second].x, board[to.first][to.second].y);
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

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLUMNS; j++) {
			//desenam cite un patrat
			glBegin(GL_QUADS);
			glColor3f(0, 0, 0);
			glVertex2f(board[i][j].x - 30, board[i][j].y - 30);
			glVertex2f(board[i][j].x + 30, board[i][j].y - 30);
			glVertex2f(board[i][j].x + 30, board[i][j].y + 30);
			glVertex2f(board[i][j].x - 30, board[i][j].y + 30);
			glEnd();

			//transformam piesele in dame daca ajung in partea opusa
			if (i == 0 && board[i][j].check == TYPE2)//'B')
				board[i][j].type = KING;

			if (i == 7 && board[i][j].check == TYPE1)//'W')
				board[i][j].type = KING;

			//coloram piesele in dependenta de tip
			if (board[i][j].check == WHITE_CHECKER)
				glColor3f(1.0, 1.0, 1.0); //Albe
			else if (board[i][j].check == BLACK_CHECKER)
				glColor3f(0.2, 0.2, 0.8); //Albastre
			else
				continue;

			//selectarea unei piese
			if (board[i][j].check == GO && uimanager::MOUSEX < board[i][j].x + 30 && uimanager::MOUSEX > board[i][j].x - 30 && uimanager::MOUSEY < board[i][j].y + 30 && uimanager::MOUSEY > board[i][j].y - 30) {
				if (!jump_list.empty()) {
					if (list_contain_element(jump_list, i, j) && board[i][j].check == GO) {
						if (uimanager::PRESSED) {
							sel.first = i;
							sel.second = j;
						}

						//culoarea pieselor selectate in dependenta de tip
						if (board[i][j].check == BLACK_CHECKER)
							glColor3f(0.2, 0.2, 0.6); //violeta
						if (board[i][j].check == WHITE_CHECKER)
							glColor3f(0.7, 0.7, 0.7); //surie
					}

				}

				else if (!move_list.empty()) {
					if (list_contain_element(move_list, i, j)) {
						if (uimanager::PRESSED) {
							sel.first = i;
							sel.second = j;
						}

						//culoarea pieselor selectate in dependenta de tip
						if (board[i][j].check == BLACK_CHECKER)
							glColor3f(0.2, 0.2, 0.6); //violeta
						if (board[i][j].check == WHITE_CHECKER)
							glColor3f(0.7, 0.7, 0.7); //surie
					}

				}

			}

			//desenam piesele de joc (octagoane)
			//conform formulelor parametrice a cercului
			glBegin(GL_POLYGON);
			for (x = 0; x <= 2 * M_PI; x += 0.77) {
				glVertex2f(25 * cos(x) + (board[i][j].x - 30 + board[i][j].x + 30) / 2, 25 * sin(x) + (board[i][j].y - 30 + board[i][j].y + 30) / 2);
			}
			glEnd();

			//semnificatii:
			//x-30 x sting
			//x+30 x drept
			//y-30 y jos
			//y+30 y sus

			//desenam coronita la dame
			//coordonatele virfurilor deduse de std::mine
			if (board[i][j].type == KING) {
				float* mx = (float*)malloc(sizeof(float));
				float* my = (float*)malloc(sizeof(float));
				if (!mx || !my) {
					glutDestroyWindow(WIN);
					exit(1);
				}
				*mx = board[i][j].x;
				*my = board[i][j].y;
				glBegin(GL_POLYGON);
				if (board[i][j].check == WHITE_CHECKER)
					glColor3f(0.0, 0.0, 1.0);
				else if (board[i][j].check == BLACK_CHECKER)
					glColor3f(0.9, 0.1, 0.1);
				glVertex2f(*mx, *my - 10 * uimanager::SIDE_COEF);
				glVertex2f(*mx + 10, *my - 10 * uimanager::SIDE_COEF);
				glVertex2f(*mx + 10, *my + 10 * uimanager::SIDE_COEF);
				glVertex2f(*mx + 5, *my - 5 * uimanager::SIDE_COEF);
				glVertex2f(*mx, *my + 10 * uimanager::SIDE_COEF);
				glVertex2f(*mx - 5, *my - 5 * uimanager::SIDE_COEF);
				glVertex2f(*mx - 10, *my + 10 * uimanager::SIDE_COEF);
				glVertex2f(*mx - 10, *my - 10 * uimanager::SIDE_COEF);
				glEnd();

				free(mx);
				free(my);
			}

		}
	}

	//desenam cifrele din stinga tablei, literele de jos
	//si doua linii de contur
	drawAround(uimanager::SIDE_COEF);

	//Desenam toate miscarile posibile daca exista
	if (POS_MOVES)
		draw_possible_moves();

	//desenam output-ul meniului "Ajutor"
	if (HELP) {
		showHelp(uimanager::SIDE_COEF);
		if (uimanager::PRESSED) {
			if (uimanager::MOUSEX * uimanager::SIDE_COEF > 190 && uimanager::MOUSEY * uimanager::SIDE_COEF > 140 && uimanager::MOUSEX * uimanager::SIDE_COEF < 210 && uimanager::MOUSEY * uimanager::SIDE_COEF < 160) {
				HELP = 0;
			}
		}
	}
	else {
		if (!check_list.empty()) check_list.clear();
		if (!jump_list.empty()) jump_list.clear();
		if (!move_list.empty()) move_list.clear();

		list_of_jumpes(jump_list, check_list);
		list_of_moves(move_list);

		//desenam intro Joc de Dame de Curcudel Eugen, la inceput de joc
		if (TYPE1 == TYPE2)
			showIntro(uimanager::SIDE_COEF);
		else {
			//afisam invingatorul sau detinatorul de miscare in timpul dat
			if (count_checkers(WHITE_CHECKER) == 0 || no_more_moves(WHITE_CHECKER)) {
				showWiner("Negrele au invins!", uimanager::SIDE_COEF);
				showTurn("Negrele au invins!", uimanager::SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
			}
			else if (count_checkers(BLACK_CHECKER) == 0 || no_more_moves(BLACK_CHECKER)) {
				showWiner("Albele au invins!", uimanager::SIDE_COEF);
				showTurn("Albele au invins!", uimanager::SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
			}
			else if (GO == WHITE_CHECKER)
				showTurn("Albele merg", uimanager::SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
			else if (GO == BLACK_CHECKER)
				showTurn("Negrele merg", uimanager::SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
		}
	}

	//curatam ecranul si schimbam buferele
	glFlush();
	glutSwapBuffers();
}
