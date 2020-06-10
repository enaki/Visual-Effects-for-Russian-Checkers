#ifdef __WIN32___
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Application.hpp"
#include "list.hpp"
#include "grafix.hpp"
#define M_PI 3.1415926535897932384626433832795

//declararea listei de miscari, sarituri si verificare
coada* mListFirst, * mListLast;
coada* jListFirst, * jListLast;
coada* vListFirst, * vListLast;

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

	glClearColor(0.9, 0.9, 0.9, 0.9);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-275.0, 275.0, -275.0, 275.0, 0.0, 1.0);
	boardInit();

	glutPassiveMotionFunc(passiveMotion);
	glutDisplayFunc(display);
	timer(0);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

//functia de salvare in fisier
void saveToFile() {
	FILE* out;
	fopen_s(&out, "joc.check", "w");
	if (!out) {
		glutDestroyWindow(WIN);
		exit(1);
	}

	fprintf(out, "%d %d %d %d %d %d %d %d %d %d", SIDE_COEF, MOUSEX, MOUSEY, sel.i, sel.j, to.i, to.j, GO, POS_MOVES, HELP);

	int i, j;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			fprintf(out, "%f %f %d %d\n", _board[i][j].x, _board[i][j].y, _board[i][j].check, _board[i][j].type);

	fclose(out);
}

//este rechemata recursiv pe parcursul programului
//si permite lucrul in timp real
void timer(int s) {
	if (!isEmptymList(vListFirst, vListLast))
		delAllmList(vListFirst, vListLast);
	if (!isEmptymList(mListFirst, mListLast))
		delAllmList(mListFirst, mListLast);
	if (!isEmptymList(jListFirst, jListLast))
		delAllmList(jListFirst, jListLast);

	listOfJumpes();
	listOfMoves();


	display();


	int i, j;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			if (MOUSEX < _board[i][j].x + 30 && MOUSEX > _board[i][j].x - 30 && MOUSEY < _board[i][j].y + 30 && MOUSEY > _board[i][j].y - 30)
				if (PRESSED) {
					to.i = i;
					to.j = j;
				}


	if (sel.i != -1 && to.i != -1) {
		copyArray(_board, _undoBoard);
		if (moveIsLegal(1))
			putChecker(); //recursie indirecta catre putChecker() care va redirectiona spre 
						//timer() in caz ca jucatorul are o serie de sarituri
	}


	//rechemarea recursiva a functiei timer()
	glutTimerFunc(10, timer, 0);
}

//returneaza 1 daca miscarea efectuata de user este posibila, 0 in caz contrar
int moveIsLegal(int p) {
	int ret = 0;
	JUMPED = 0;
	if (_board[sel.i][sel.j].type == CHECKER) {
		if (_board[to.i][to.j].check == NO_CHECKER) {

			//saritura
			if (sel.i == to.i - 2) {
				if (sel.i % 2) {
					if (sel.j == to.j - 1 || sel.j == to.j + 1)
						if (_board[sel.i + 1][max(sel.j, to.j)].check != _board[sel.i][sel.j].check && _board[sel.i + 1][max(sel.j, to.j)].check != NO_CHECKER) {
							if (p) {
								_board[sel.i + 1][max(sel.j, to.j)].check = NO_CHECKER;
								_board[sel.i + 1][max(sel.j, to.j)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
				else {
					if (sel.j == to.j + 1 || sel.j == to.j - 1)
						if (_board[sel.i + 1][min(sel.j, to.j)].check != _board[sel.i][sel.j].check && _board[sel.i + 1][min(sel.j, to.j)].check != NO_CHECKER) {
							if (p) {
								_board[sel.i + 1][min(sel.j, to.j)].check = NO_CHECKER;
								_board[sel.i + 1][min(sel.j, to.j)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
			}
			if (sel.i == to.i + 2) {
				if (sel.i % 2) {
					if (sel.j == to.j - 1 || sel.j == to.j + 1)
						if (_board[sel.i - 1][max(sel.j, to.j)].check != _board[sel.i][sel.j].check && _board[sel.i - 1][max(sel.j, to.j)].check != NO_CHECKER) {
							if (p) {
								_board[sel.i - 1][max(sel.j, to.j)].check = NO_CHECKER;
								_board[sel.i - 1][max(sel.j, to.j)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
				else {
					if (sel.j == to.j + 1 || sel.j == to.j - 1)
						if (_board[sel.i - 1][min(sel.j, to.j)].check != _board[sel.i][sel.j].check && _board[sel.i - 1][min(sel.j, to.j)].check != NO_CHECKER) {
							if (p) {
								_board[sel.i - 1][min(sel.j, to.j)].check = NO_CHECKER;
								_board[sel.i - 1][min(sel.j, to.j)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
			}


			//miscare simpla
			if (sel.i == to.i - 1) {
				if (_board[sel.i][sel.j].check == WHITE_CHECKER) {
					if (sel.i % 2) {//impar
						if (sel.j == to.j || sel.j == to.j - 1) {
							ret = 1;
							goto end;
						}
					}
					else {
						if (sel.j == to.j || sel.j == to.j + 1) {
							ret = 1;
							goto end;
						}
					}
				}
			}

			if (sel.i == to.i + 1) {
				if (_board[sel.i][sel.j].check == BLACK_CHECKER) {
					if (sel.i % 2) {//impar
						if (sel.j == to.j || sel.j == to.j - 1) {
							ret = 1;
							goto end;
						}
					}
					else {
						if (sel.j == to.j || sel.j == to.j + 1) {
							ret = 1;
							goto end;
						}
					}
				}
			}

		}
	} //end of if piesa is checker


	//begin of if type is King
	if (_board[sel.i][sel.j].type == KING) {
		if (_board[to.i][to.j].check == NO_CHECKER) {
			struct location rem;
			rem.i = rem.j = -1;
			int count = 0, i, j;
			int ver = 0, is = 0;

			if (sel.i % 2) {
				i = sel.i + 1;
				j = sel.j + 1;
			}
			else {
				i = sel.i;
				j = sel.j;
			}

			if (to.i > sel.i && to.j >= j) { //daca ma duc in dreapta sus
				int need = 0;
				for (; j < C; i++) {
					++need;

					if (i == sel.i && j == sel.j)
						continue;


					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.i][sel.j].check) {
						if (!count) {
							rem.i = i;
							rem.j = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.i][sel.j].check)
						return 0;

					if (to.i == i && to.j == j) {
						++is;
						break;
					}

					if (need == 2) {
						++j;
						need = 0;
					}
				}

				if (count == 1 && is) {
					if (p) {
						_board[rem.i][rem.j].check = NO_CHECKER;
						_board[rem.i][rem.j].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}
			}


			if (sel.i % 2) {
				i = sel.i;
				j = sel.j;
			}
			else {
				i = sel.i + 1;
				j = sel.j - 1;
			}

			if (to.i > sel.i && to.j <= j) { //daca ma duc in stinga sus
				int need = 0;


				for (; j >= 0; i++) {
					++need;
					if (i == sel.i && j == sel.j)
						continue;

					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.i][sel.j].check) {
						if (!count) {
							rem.i = i;
							rem.j = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.i][sel.j].check)
						return 0;

					if (to.i == i && to.j == j) {
						++is;
						break;
					}

					if (need == 2) {
						--j;
						need = 0;
					}
				}

				if (count == 1 && is) {
					if (p) {
						_board[rem.i][rem.j].check = NO_CHECKER;
						_board[rem.i][rem.j].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}

			}



			if (sel.i % 2) {
				i = sel.i - 1;
				j = sel.j + 1;
			}
			else {
				i = sel.i;
				j = sel.j;
			}

			if (to.i < sel.i && to.j >= j) { //daca ma duc in dreapta jos 
				int need = 0;


				for (; j <= 3; i--) {
					++need;

					if (i == sel.i && j == sel.j) {
						continue;
					}

					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.i][sel.j].check) {
						if (!count) {
							rem.i = i;
							rem.j = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.i][sel.j].check)
						return 0;

					if (to.i == i && to.j == j) {
						++is;
						break;
					}

					if (need == 2) {
						++j;
						need = 0;
					}
				}

				if (count == 1 && is) {
					if (p) {
						_board[rem.i][rem.j].check = NO_CHECKER;
						_board[rem.i][rem.j].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}
			}


			if (sel.i % 2) {
				i = sel.i;
				j = sel.j;
			}
			else {
				i = sel.i - 1;
				j = sel.j - 1;
			}

			count = 0;
			if (to.i < sel.i && to.j <= j) { //daca ma duc in stinga jos 
				int need = 0;


				for (; j >= 0; i--) {
					++need;
					if (i == sel.i && j == sel.j)
						continue;

					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.i][sel.j].check) {
						if (!count) {
							rem.i = i;
							rem.j = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.i][sel.j].check)
						return 0;

					if (to.i == i && to.j == j) {
						++is;
						break;
					}

					if (need == 2) {
						--j;
						need = 0;
					}
				}

				if (count == 1 && is) {
					if (p) {
						_board[rem.i][rem.j].check = NO_CHECKER;
						_board[rem.i][rem.j].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}

			}


		} //end of NO_CHECKER
	} //end of if piesa is king 
	return 0;

end:
	if (isInList(jListFirst, sel.i, sel.j) && !JUMPED)
		return 0;
	else
		return ret;

}

//returneaza maximul dintre doua valori
int max(int a, int b) {
	return (a > b) ? a : b;
}

//returneaza minimul dintre doua valori
int min(int a, int b) {
	return (a < b) ? a : b;
}

//daca o miscare este posibila, atunci aceasta este efectuata
void putChecker() {
	_board[to.i][to.j].check = _board[sel.i][sel.j].check;
	_board[to.i][to.j].type = _board[sel.i][sel.j].type;
	_board[sel.i][sel.j].check = NO_CHECKER;
	_board[sel.i][sel.j].type = CHECKER;


	if (!isEmptymList(jListFirst, jListLast))
		delAllmList(jListFirst, jListLast);

	listOfJumpes();

	if (JUMPED && isInList(jListFirst, to.i, to.j)) {
		JUMPED = 0;
		PRESSED = 0;

		sel.i = to.i;
		sel.j = to.j;

		timer(0);//recursie indirecta catre timer() in caz ca userul are o serie de sarituri	
	}
	else {

		GO = (GO == WHITE_CHECKER) ? BLACK_CHECKER : WHITE_CHECKER;
		JUMPED = 0;
		PRESSED = 0;

		display();

		if (ROTIRI) {
			sleep(1);
			glRotatef(180, 0, 0, 1);
			SIDE_COEF *= -1;
		}
		sel.i = sel.j = -1;
		to.i = to.j = -1;
		MOUSEX = MOUSEY = -240;
	}
}

//completarea cozii mListFirst cu piese care pot efectua miscari
void listOfMoves() {
	int i, j;

	for (i = 0; i < R; i++) {
		for (j = 0; j < C; j++) {
			if (_board[i][j].check != NO_CHECKER) {
				int k;
				if (i % 2)
					k = j + 1;
				else
					k = j - 1;
				if (_board[i][j].type == KING) {
					if ((_board[i + 1][j].check == NO_CHECKER && i + 1 < R) ||
						(_board[i - 1][j].check == NO_CHECKER && i - 1 >= 0) ||
						(_board[i + 1][k].check == NO_CHECKER && i + 1 < R && k < C && k >= 0) ||
						(_board[i - 1][k].check == NO_CHECKER && i - 1 >= 0 && k < C && k >= 0)) {
						addTomList(mListFirst, mListLast, i, j);
					} //if lung
				} //if KING
				if (_board[i][j].type == CHECKER) {
					if ((_board[i + 1][j].check == NO_CHECKER && i + 1 < R && _board[i][j].check == WHITE_CHECKER) ||
						(_board[i - 1][j].check == NO_CHECKER && i - 1 >= 0 && _board[i][j].check == BLACK_CHECKER) ||
						(_board[i + 1][k].check == NO_CHECKER && i + 1 < R && k < C && k >= 0 && _board[i][j].check == WHITE_CHECKER) ||
						(_board[i - 1][k].check == NO_CHECKER && i - 1 >= 0 && k < C && k >= 0 && _board[i][j].check == BLACK_CHECKER)) {
						addTomList(mListFirst, mListLast, i, j);
					} //if lung
				} //if CHECKER
			} //if !NO_CHECHER
		} //for j
	} //for i	
} //end listOfMoves

//completarea cozii jListFirst cu piesele care trebuie sa sara
void listOfJumpes() {
	int i, j;

	for (i = 0; i < R; i++) {
		for (j = 0; j < C; j++) {
			if (_board[i][j].check != NO_CHECKER) {
				int k, s;
				if (i % 2) {
					k = j + 1;
					s = j - 1;
				}
				else {
					k = j - 1;
					s = j + 1;
				}

				if (_board[i][j].type == CHECKER) {
					if ((_board[i + 1][j].check != NO_CHECKER && _board[i + 1][j].check != _board[i][j].check && _board[i + 2][s].check == NO_CHECKER && i + 2 < R && s < C && s >= 0) ||
						(_board[i - 1][j].check != NO_CHECKER && _board[i - 1][j].check != _board[i][j].check && _board[i - 2][s].check == NO_CHECKER && i - 2 >= 0 && s < C && s >= 0) ||
						(_board[i + 1][k].check != NO_CHECKER && _board[i + 1][k].check != _board[i][j].check && _board[i + 2][k].check == NO_CHECKER && i + 2 < R && k < C && k >= 0) ||
						(_board[i - 1][k].check != NO_CHECKER && _board[i - 1][k].check != _board[i][j].check && _board[i - 2][k].check == NO_CHECKER && i - 2 >= 0 && k < C && k >= 0)) {
						if (_board[i][j].check == GO)
							addTomList(jListFirst, jListLast, i, j);
						addTomList(vListFirst, vListLast, i, j);
					} // if lung

				} // if CHECKER


				if (_board[i][j].type == KING) {

					int count = 0;
					//dreapta sus
					if (i % 2) {
						k = i + 1;
						s = j + 1;
					}
					else {
						k = i;
						s = j;
					}

					int need = 0;
					for (; s < C; k++) {
						++need;

						if (k == i && s == j)
							continue;


						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;


						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							++s;
							need = 0;
						}

						if (count == 1) {
							++k;
							if (_board[k][s].check == NO_CHECKER && k < R && s < C)
								if (_board[i][j].check == GO)
									addTomList(jListFirst, jListLast, i, j);
							addTomList(vListFirst, vListLast, i, j);
							break;
						}
					}
					// end of dreapta sus


				   //stinga sus
					if (i % 2) {
						k = i;
						s = j;
					}
					else {
						k = i + 1;
						s = j - 1;
					}

					count = 0;
					need = 0;
					for (; s >= 0; k++) {
						++need;
						if (k == i && s == j)
							continue;

						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;

						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							--s;
							need = 0;
						}

						if (count == 1) {
							++k;
							if (_board[k][s].check == NO_CHECKER && k < R && s >= 0)
								if (_board[i][j].check == GO)
									addTomList(jListFirst, jListLast, i, j);
							addTomList(vListFirst, vListLast, i, j);
							break;
						}
					}
					//end of stinga sus



					//dreapta jos
					if (i % 2) {
						k = i - 1;
						s = j + 1;
					}
					else {
						k = i;
						s = j;
					}

					count = 0;
					need = 0;


					for (; s <= 3; k--) {
						++need;

						if (k == i && s == j) {
							continue;
						}

						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;

						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							++s;
							need = 0;
						}

						if (count == 1) {
							--k;
							if (_board[k][s].check == NO_CHECKER && k >= 0 && s < C)
								if (_board[i][j].check == GO)
									addTomList(jListFirst, jListLast, i, j);
							addTomList(vListFirst, vListLast, i, j);
							break;
						}
					}

					//end of dreapta jos

					//stinga jos
					if (i % 2) {
						k = i;
						s = j;
					}
					else {
						k = i - 1;
						s = j - 1;
					}

					count = 0;
					need = 0;


					for (; s >= 0; k--) {
						++need;
						if (k == i && s == j)
							continue;

						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;

						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							--s;
							need = 0;
						}

						if (count == 1) {
							--k;
							if (_board[k][s].check == NO_CHECKER && k >= 0 && s >= 0)
								if (_board[i][j].check == GO)
									addTomList(jListFirst, jListLast, i, j);
							addTomList(vListFirst, vListLast, i, j);
							break;
						}
					}

					//end of stinga jos


				} //if KING		

			} // if !NO_CHECKER
		} // for j
	} // for i

} //end of listOfJumpes

//permite atirnarea programului pentru "Sec" secunde
void sleep(unsigned int Sec) {
	clock_t ticks1 = clock(), ticks2 = ticks1;
	while ((ticks2 / CLOCKS_PER_SEC - ticks1 / CLOCKS_PER_SEC) < Sec)
		ticks2 = clock();
}

//converteste coordonatele in locatie
struct location coordsToIndex(int x, int y) {
	struct location ret;
	for (ret.i = 0; ret.i < R; ret.i++)
		for (ret.j = 0; ret.j < C; ret.j++)
			if (x < _board[ret.i][ret.j].x + 30 && x > _board[ret.i][ret.j].x - 30 && y < _board[ret.i][ret.j].y + 30 && y > _board[ret.i][ret.j].y - 30)
				return ret;
}

//fixeaza x si y daca butonul sting al mouse-ului este aparat
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		MOUSEX = (x - 275) * SIDE_COEF;
		MOUSEY = (y - 275) * (-1) * SIDE_COEF;

		PRESSED = state == GLUT_DOWN; //echivalent cu seria de jos
		/*if(state == GLUT_DOWN)
			PRESSED = 1;
		else
			PRESSED = 0;
		*/
	}
}

//fixeaza coordonatele cind se trage mouse-ul cu un buton apasat
void motion(int x, int y) {
	MOUSEX = (x - 275) * SIDE_COEF;
	MOUSEY = (y - 275) * (-1) * SIDE_COEF;
}

//fixeaza coordonatele cursorului, chiar daca nu se apasa nici un buton
void passiveMotion(int x, int y) {
	int i, j;
	x = (x - 275) * SIDE_COEF;
	y = (y - 275) * (-1) * SIDE_COEF;

	int* cursMina = (int*)malloc(sizeof(int));
	*cursMina = 0; //daca cursMina = 1, atunci cursorul va fi mina

	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++) {
			//sageata cursorului devine "mina" sau "drag" deasupra pieselor
			if (_board[i][j].check != NO_CHECKER && x < _board[i][j].x + 30 && x > _board[i][j].x - 30 && y < _board[i][j].y + 30 && y > _board[i][j].y - 30) {
				*cursMina = 1;
				break;
			}
		}

	if (*cursMina)
		glutSetCursor(GLUT_CURSOR_INFO);
	else
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	free(cursMina);
}

//initializarea tablei de dame din fisierul "joc.check"
void initFromFile() {
	FILE* in;
	fopen_s(&in, "joc.check", "r");
	if (!in) {
		glutDestroyWindow(WIN);
		exit(1);
	}

	fscanf_s(in, "%d %d %d %d %d %d %d %d %d %d", &SIDE_COEF, &MOUSEX, &MOUSEY, &sel.i, &sel.j, &to.i, &to.j, &GO, &POS_MOVES, &HELP);

	int i, j, check, type;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++) {
			fscanf_s(in, "%f %f %d %d", &_board[i][j].x, &_board[i][j].y, &check, &type);
			_board[i][j].check = check;
			_board[i][j].type = type;
		}

	fclose(in);
}

//initializarea tablei de dame -=Joc Nou=-
void boardInit() {
	int i, j, s = -240;
	float x, y;
	SIDE_COEF = 1;
	MOUSEX = MOUSEY = -241;
	sel.i = sel.j = -1;
	to.i = to.j = -1;

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
		SIDE_COEF *= -1;
	}
	glutPostRedisplay();
}

//returneaza 1 daca piesele de tip "checker" nu mai au nici o miscare
int noMoreMoves(int checker) {
	int ver1 = 1, ver2 = 1;
	coada* t = mListFirst;
	while (t != NULL) {
		if (_board[t->i][t->j].check == checker) {
			ver1 = 0;
			break;
		}
		t = t->next;
	}

	t = vListFirst;
	while (t != NULL) {
		if (_board[t->i][t->j].check == checker) {
			ver2 = 0;
			break;
		}
		t = t->next;
	}

	return (ver1 && ver2);
}

//returneaza numarul de piese de tip "checker"
int countCheckers(int checker) {
	int i, j, count = 0;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			if (_board[i][j].check == checker)
				++count;
	return count;
}

//copie informatia tablei pentru a putea face undo()
void copyArray(struct square from[R][C], struct square to[R][C]) {
	int i, j;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			to[i][j] = from[i][j];
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
		if (SIDE_COEF == -1)
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
		if (SIDE_COEF == -1)
			glRotatef(180, 0, 0, 1);
		break;
	case 3:
		SIDE_COEF *= -1;
		MOUSEX = MOUSEY = -241;
		sel.i = sel.j = -1;
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

//returneaza 1 daca informatia de pe doua table este la fel, pentru undo()
int areIdentic(struct square a[R][C], struct square b[R][C]) {
	int i, j;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			if (a[i][j].check != b[i][j].check || a[i][j].type != b[i][j].type)
				return 0;
	return 1;
}

//citirea datelor de la tastatura
//in cazul meu, doar "Esc" pentru iesire
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		glutDestroyWindow(WIN);
		exit(0);
	}
}

//desenarea grafica a tuturor miscarilor posibile
void drawPossibleMoves() {
	int i, j, ver = 1;
	int a = sel.i, b = sel.j, c = to.i, d = to.j, e = JUMPED;
	coada* first = jListFirst;
	while (first != NULL) {
		for (i = 0; i < 8; i++)
			for (j = 0; j < 4; j++) {
				sel.i = first->i;
				sel.j = first->j;
				to.i = i;
				to.j = j;
				if (moveIsLegal(0)) {
					if (JUMPED) {
						(GO == WHITE_CHECKER) ? glColor3f(0, 1, 0) : glColor3f(1, 0, 0);
						glBegin(GL_LINES);
						glVertex2f(_board[sel.i][sel.j].x, _board[sel.i][sel.j].y);
						glVertex2f(_board[to.i][to.j].x, _board[to.i][to.j].y);
						glEnd();
						ver = 0;
					}
				}
			}
		first = first->next;
	}

	if (ver) {
		first = mListFirst;
		while (first != NULL) {
			for (i = 0; i < 8; i++)
				for (j = 0; j < 4; j++) {
					if (_board[first->i][first->j].check == GO) {
						sel.i = first->i;
						sel.j = first->j;
						to.i = i;
						to.j = j;
						if (moveIsLegal(0)) {
							if (!JUMPED) {
								(GO == WHITE_CHECKER) ? glColor3f(0, 1, 0) : glColor3f(1, 0, 0);
								glBegin(GL_LINES);
								glVertex2f(_board[sel.i][sel.j].x, _board[sel.i][sel.j].y);
								glVertex2f(_board[to.i][to.j].x, _board[to.i][to.j].y);
								glEnd();
							}
						}
					}
				}
			first = first->next;
		}
	}

	sel.i = a;
	sel.j = b;
	to.i = c;
	to.j = d;
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
			if (_board[i][j].check == GO && MOUSEX < _board[i][j].x + 30 && MOUSEX > _board[i][j].x - 30 && MOUSEY < _board[i][j].y + 30 && MOUSEY > _board[i][j].y - 30) {
				if (!isEmptymList(jListFirst, jListLast)) {
					if (isInList(jListFirst, i, j) && _board[i][j].check == GO) {
						if (PRESSED) {
							sel.i = i;
							sel.j = j;
						}

						//culoarea pieselor selectate in dependenta de tip
						if (_board[i][j].check == BLACK_CHECKER)
							glColor3f(0.2, 0.2, 0.6); //violeta
						if (_board[i][j].check == WHITE_CHECKER)
							glColor3f(0.7, 0.7, 0.7); //surie
					}

				}

				else if (!isEmptymList(mListFirst, mListLast)) {
					if (isInList(mListFirst, i, j)) {
						if (PRESSED) {
							sel.i = i;
							sel.j = j;
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
			//coordonatele virfurilor deduse de mine
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
				glVertex2f(*mx, *my - 10 * SIDE_COEF);
				glVertex2f(*mx + 10, *my - 10 * SIDE_COEF);
				glVertex2f(*mx + 10, *my + 10 * SIDE_COEF);
				glVertex2f(*mx + 5, *my - 5 * SIDE_COEF);
				glVertex2f(*mx, *my + 10 * SIDE_COEF);
				glVertex2f(*mx - 5, *my - 5 * SIDE_COEF);
				glVertex2f(*mx - 10, *my + 10 * SIDE_COEF);
				glVertex2f(*mx - 10, *my - 10 * SIDE_COEF);
				glEnd();

				free(mx);
				free(my);
			}

		}
	}

	//desenam cifrele din stinga tablei, literele de jos
	//si doua linii de contur
	drawAround(SIDE_COEF);

	//Desenam toate miscarile posibile daca exista
	if (POS_MOVES)
		drawPossibleMoves();

	//desenam output-ul meniului "Ajutor"
	if (HELP) {
		showHelp(SIDE_COEF);
		if (PRESSED) {
			if (MOUSEX * SIDE_COEF > 190 && MOUSEY * SIDE_COEF > 140 && MOUSEX * SIDE_COEF < 210 && MOUSEY * SIDE_COEF < 160) {
				HELP = 0;
			}
		}
	}
	else {
		if (!isEmptymList(vListFirst, vListLast))
		{
			delAllmList(vListFirst, vListLast);
			vListFirst = nullptr;
			vListLast = nullptr;
		}
		if (!isEmptymList(mListFirst, mListLast))
		{
			delAllmList(mListFirst, mListLast);
			mListFirst = nullptr;
			mListLast = nullptr;
		}
		if (!isEmptymList(jListFirst, jListLast))
		{
			delAllmList(jListFirst, jListLast);
			jListFirst = nullptr;
			jListLast = nullptr;
		}

		listOfJumpes();
		listOfMoves();

		//desenam intro Joc de Dame de Curcudel Eugen, la inceput de joc
		if (TYPE1 == TYPE2)
			showIntro(SIDE_COEF);
		else {
			//afisam invingatorul sau detinatorul de miscare in timpul dat
			if (countCheckers(WHITE_CHECKER) == 0 || noMoreMoves(WHITE_CHECKER)) {
				showWiner("Negrele au invins!", SIDE_COEF);
				showTurn("Negrele au invins!", SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
			}
			else if (countCheckers(BLACK_CHECKER) == 0 || noMoreMoves(BLACK_CHECKER)) {
				showWiner("Albele au invins!", SIDE_COEF);
				showTurn("Albele au invins!", SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
			}
			else if (GO == WHITE_CHECKER)
				showTurn("Albele merg", SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
			else if (GO == BLACK_CHECKER)
				showTurn("Negrele merg", SIDE_COEF, countCheckers(WHITE_CHECKER), countCheckers(BLACK_CHECKER));
		}
	}

	//curatam ecranul si schimbam buferele
	glFlush();
	glutSwapBuffers();
}
