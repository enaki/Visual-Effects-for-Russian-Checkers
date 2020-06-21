#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cstdlib>
#include "Application.h"
#include "utilities.h"
#include "grafix.hpp"
#include <queue>
#include <list>
#include "data.h"
#include "Game.h"
#include "UIManager/keyboard.h"

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint shader_programme, vao;
GLuint vbo = 1;
float board_squares[ROWS][COLUMNS][12];


#define PI glm::pi<float>()

float LIGHT = 0.5f;
float rotAngle = 0;
float rotAngleInc = M_PI / 64;


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	projectionMatrix = glm::perspective(PI / 6, (float)w / h, 0.1f, 100.0f);
	/*
	viewMatrix este matricea transformarii de observare. Parametrii functiei
	lookAt sunt trei vectori ce reprezinta, in ordine:
	- pozitia observatorului
	- punctul catre care priveste observatorul
	- directia dupa care este orientat observatorul
	*/
	viewMatrix = glm::lookAt(view_pos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
}

//functia main in care initializam rutina OpenGL si Glut
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(550, 550);
	glutInitWindowPosition(100, 100);
	WIN = glutCreateWindow("Russian Checkers");
	init();
	glutPassiveMotionFunc(passive_motion);
	glutDisplayFunc(display);
	glutMouseFunc(uimanager::mouse_listener);
	glutMotionFunc(uimanager::motion_listener);
	glutKeyboardFunc(uimanager::keyboard_listener);
	timer(0);
	glutMainLoop();
}

void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	//glDepthFunc(GL_ALWAYS);
	//glEnable(GL_DEPTH_TEST);
	glewInit();

	std::string vstext = textFileRead((char *)"shader/light_vertex.vert");
	std::string fstext = textFileRead((char *)"shader/light_fragment.vert");
	const char* vertex_shader = vstext.c_str();
	const char* fragment_shader = fstext.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	
	create_menu();
	uimanager::WIN = WIN;
	glClearColor(0.9, 0.9, 0.9, 0.9);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-275.0, 275.0, -275.0, 275.0, 0.0, 1.0);
	board_init();
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
		JUMPED = false;
		uimanager::PRESSED = 0;

		sel.first = to.first;
		sel.second = to.second;

		timer(0);//recursie indirecta catre timer() in caz ca userul are o serie de sarituri	
	}
	else {

		GO = (GO == WHITE_CHECKER) ? BLACK_CHECKER : WHITE_CHECKER;
		JUMPED = false;
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
	x = (x - 275) * uimanager::SIDE_COEF;
	y = (y - 275) * (-1) * uimanager::SIDE_COEF;

	for (auto i = 0; i < ROWS; i++)
		for (auto j = 0; j < COLUMNS; j++) {
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
			const auto x = static_cast<float>(s) + 120 * j;
			const auto y = static_cast<float>(-240) + 60 * i;

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
	//load board square values
	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLUMNS; j++) {
			board_squares[i][j][0] = board[i][j].x - 30;
			board_squares[i][j][1] = board[i][j].y - 30;
			board_squares[i][j][2] = 0.0;

			board_squares[i][j][3] = board[i][j].x + 30;
			board_squares[i][j][4] = board[i][j].y - 30;
			board_squares[i][j][5] = 0.0;

			board_squares[i][j][6] = board[i][j].x + 30;
			board_squares[i][j][7] = board[i][j].y + 30;
			board_squares[i][j][8] = 0.0;

			board_squares[i][j][9] = board[i][j].x - 30;
			board_squares[i][j][10] = board[i][j].y + 30;
			board_squares[i][j][11] = 0.0;
		}
	}

	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLUMNS; j++) {
			for (auto k = 0; k < 12; k++) {
				board_squares[i][j][k] /= 275;
			}
		}
	}
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
		ROTIRI = !ROTIRI;
		break;
	case 5:
		save_to_file();
		break;
	case 6:
		if (!are_identic(undo_board, board))
			undo();
		break;
	case 7:
		POS_MOVES = !POS_MOVES;
		break;
	case 8:
		HELP = true;
		break;
	default: break;
	}

	glutPostRedisplay();
}

//desenarea grafica a tuturor miscarilor posibile
void draw_possible_moves() {
	int ver = 1;
	int a = sel.first, b = sel.second, c = to.first, d = to.second, e = JUMPED;
	
	for (auto& it : jump_list)
	{
		for (auto i = 0; i < ROWS; i++)
			for (auto j = 0; j < COLUMNS; j++) {
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
			for (auto i = 0; i < ROWS; i++)
				for (auto j = 0; j < COLUMNS; j++) {
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


void update_uniform_fragment_shader()
{
	modelMatrix = glm::mat4();
	GLuint lightPosLoc = glGetUniformLocation(shader_programme, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light_pos));

	GLuint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(view_pos));

	//modelMatrix *= glm::rotate(rotAngle, glm::vec3(0, 1, 0));
	GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "mvpMatrix");
	auto mvp = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	// se determina matricea ce realizeaza corectia normalelor. Ea se trimite catre vertex shader la fel cum s-a procedat si cu mvpMatrix 
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

//functia de desenare grafica principala
void display() {
	//curatam ecranul
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(0);
	int s;
	float x, y;
	
	GLuint vbo = 1;
	glGenBuffers(1, &vbo);
	GLuint checkers_vbo = 3;
	glGenBuffers(1, &checkers_vbo);
	GLuint crown_vbo = 3;
	glGenBuffers(1, &crown_vbo);
	GLuint color_vbo = 2;
	glGenBuffers(1, &color_vbo);
	
	vao = 0;
	glGenVertexArrays(1, &vao);
	
	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLUMNS; j++) {
			glm::vec3 color;
			
			//one square drawing
			glUseProgram(shader_programme);
			glEnableVertexAttribArray(0);
			
			auto* const current_square = board_squares[i][j];
			
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), current_square, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			GLuint color_id = glGetUniformLocation(shader_programme, "diffuse");
			glUniform3fv(color_id, 1, glm::value_ptr(glm::vec3(1, 0, 1)));

			update_uniform_fragment_shader();
			
			glDrawArrays(GL_QUADS, 0, 4);
			glDisableVertexAttribArray(0);
			glUseProgram(0);
			
			//transformation time, become queen if needed
			if (i == 0 && board[i][j].check == TYPE2)//'B')
				board[i][j].type = KING;

			if (i == 7 && board[i][j].check == TYPE1)//'W')
				board[i][j].type = KING;
			
			//color the checkers according to type
			if (board[i][j].check == WHITE_CHECKER)
				color = glm::vec3(1, 1, 1); //Albe
			else if (board[i][j].check == BLACK_CHECKER)
				color = glm::vec3(0.2, 0.2, 0.8); //Albastre
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
							color = glm::vec3(0.2, 0.2, 0.6); //violeta
						if (board[i][j].check == WHITE_CHECKER)
							color = glm::vec3(0.7, 0.7, 0.7);	//surie
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
							color = glm::vec3(0.2, 0.2, 0.6);//violeta
						if (board[i][j].check == WHITE_CHECKER)
							color = glm::vec3(0.7, 0.7, 0.7);//surie
					}
				}
			}

			//desenam piesele de joc (octagoane)
			//conform formulelor parametrice a cercului
			const auto circle_precision = 16;
			float checker_positions[3 * circle_precision];
			for (auto k = 0; k < circle_precision; k++) {
				auto x = 2 * M_PI * k / static_cast<float>(circle_precision);
				checker_positions[3 * k] = 25 * cos(x) + (board[i][j].x - 30 + board[i][j].x + 30) / 2;
				checker_positions[3 * k + 1] = 25 * sin(x) + (board[i][j].y - 30 + board[i][j].y + 30) / 2;
				checker_positions[3 * k + 2] = 0;
			}

			for (auto& checker_position : checker_positions)
			{
				checker_position /= 275;
			}
			
			glUseProgram(shader_programme);
			glEnableVertexAttribArray(0);
			
			glUniform3fv(color_id, 1, glm::value_ptr(color));
			glBindBuffer(GL_ARRAY_BUFFER, checkers_vbo);
			glBufferData(GL_ARRAY_BUFFER, 3 * circle_precision * sizeof(float), checker_positions, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glDrawArrays(GL_POLYGON, 0, circle_precision);
			
			glDisableVertexAttribArray(0);
			glUseProgram(0);
			
			//desenam coronita la dame
			if (board[i][j].type == KING) {
				const auto mx = board[i][j].x;
				const auto my = board[i][j].y;

				float crown[3 * 8] = {
					mx,			my - 10 * uimanager::SIDE_COEF,	 0.0f,
					mx + 10,	my - 10 * uimanager::SIDE_COEF,	 0.0f,
					mx + 10,	my + 10 * uimanager::SIDE_COEF,	 0.0f,
					mx + 5,		my - 5 * uimanager::SIDE_COEF,	 0.0f,
					mx,			my + 10 * uimanager::SIDE_COEF,	 0.0f,
					mx - 5,		my - 5 * uimanager::SIDE_COEF,	 0.0f,
					mx - 10,	my + 10 * uimanager::SIDE_COEF,	 0.0f,
					mx - 10,	my - 10 * uimanager::SIDE_COEF,	 0.0f
				};
				for (auto& crown_angle : crown)
				{
					crown_angle /= 275;
				}

				if (board[i][j].check == WHITE_CHECKER)
					color = glm::vec3(0.0, 0.0, 1.0);
				else if (board[i][j].check == BLACK_CHECKER)
					color = glm::vec3(0.9, 0.1, 0.1);
				
				glUseProgram(shader_programme);
				glEnableVertexAttribArray(0);

				glUniform3fv(color_id, 1, glm::value_ptr(color));
				glBindBuffer(GL_ARRAY_BUFFER, crown_vbo);
				glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), crown, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				glDrawArrays(GL_POLYGON, 0, 8);

				glDisableVertexAttribArray(0);
				glUseProgram(0);
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
				HELP = false;
			}
		}
	}
	else {
		if (!check_list.empty()) check_list.clear();
		if (!jump_list.empty()) jump_list.clear();
		if (!move_list.empty()) move_list.clear();

		list_of_jumpes(jump_list, check_list);
		list_of_moves(move_list);

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
				showTurn("White's turn", uimanager::SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
			else if (GO == BLACK_CHECKER)
				showTurn("Black's turn", uimanager::SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
		}
	}
	glFlush();
}
