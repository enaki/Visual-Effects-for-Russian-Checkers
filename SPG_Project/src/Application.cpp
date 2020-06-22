#include <GL/glew.h>
#include <cstdlib>
#include <queue>
#include <list>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

#include "Graphics/grafix.hpp"

#include "Game/Game.h"
#include "UIManager/keyboard.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"

GLuint vbo = 1;
GLuint color_vbo = 2;
GLuint checkers_vbo = 3;
GLuint crown_vbo = 3;
GLuint board_texture_vbo = 4;
GLuint circle_vbo = 5;



//functia main in care initializam rutina OpenGL si Glut
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(550, 550);
	glutInitWindowPosition(100, 100);
	WIN = glutCreateWindow("Russian Checkers");
	init_data();
	init();
	glutPassiveMotionFunc(passive_motion);
	glutDisplayFunc(display);
	glutMouseFunc(uimanager::mouse_listener);
	glutMotionFunc(uimanager::motion_listener);
	glutKeyboardFunc(uimanager::keyboard_listener);
	timer(0);
	glutMainLoop();
}

void init_data()
{
	for (auto i = 0; i < 4; i++)
	{
		full_board[8 * i] /= 275;
		full_board[8 * i + 1] /= 275;
	}
}

void compile_shader(GLuint& shader)
{
	GLint is_compiled = 0;

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

	if (is_compiled == GL_FALSE)
	{
		GLint max_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(max_length);
		glGetShaderInfoLog(shader, max_length, &max_length, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(shader);
		glDeleteShader(shader);
		std::string strInfoLog(infoLog.begin(), infoLog.end());
		std::cout << strInfoLog << std::endl;
		// In this simple program, we'll just leave
		throw new std::exception(strInfoLog.c_str());
	}
}

void create_shader_program(char *vertex_shader_file, char *fragment_shader_path, GLuint &shader_programme)
{
	std::string vstext = textFileRead(vertex_shader_file);
	std::string fstext = textFileRead(fragment_shader_path);
	const char* vertex_shader = vstext.c_str();
	const char* fragment_shader = fstext.c_str();


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	compile_shader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	compile_shader(fs);

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
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
			if (MOUSEX < board[i][j].x + 30 && MOUSEX > board[i][j].x - 30 && MOUSEY < board[i][j].y + 30 && MOUSEY > board[i][j].y - 30)
				if (PRESSED) {
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
		PRESSED = 0;

		sel.first = to.first;
		sel.second = to.second;

		timer(0);//recursie indirecta catre timer() in caz ca userul are o serie de sarituri	
	}
	else {

		GO = (GO == WHITE_CHECKER) ? BLACK_CHECKER : WHITE_CHECKER;
		JUMPED = false;
		PRESSED = 0;

		display();

		if (ROTIRI) {
			sleep(1);
			glRotatef(180, 0, 0, 1);
			SIDE_COEF *= -1;
		}
		sel.first = sel.second = -1;
		to.first = to.second = -1;
		MOUSEX = MOUSEY = -240;
	}
}

//fixeaza coordonatele cursorului, chiar daca nu se apasa nici un buton
void passive_motion(int x, int y) {
	x = (x - 275) * SIDE_COEF;
	y = (y - 275) * (-1) * SIDE_COEF;

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
	SIDE_COEF = 1;
	MOUSEX = MOUSEY = -241;
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
		SIDE_COEF *= -1;
	}
	glutPostRedisplay();
}

//definirea si structurarea meniului
void create_menu() {
	int options = glutCreateMenu(action_menu);

	//submeniul "Optiuni"
	glutAddMenuEntry("Texture On/Off", 9);
	glutAddMenuEntry("Light On/Off", 10);
	glutAddMenuEntry("Normal On/Off", 11);
	glutAddMenuEntry("Change Light Type", 12);
	glutAddMenuEntry("Undo", 6);
	glutAddMenuEntry("Rotation 180", 3);
	glutAddMenuEntry("Rotations on/off", 4);
	glutAddMenuEntry("Possible Moves on/off", 7);


	glutCreateMenu(action_menu);

	glutAddMenuEntry("New Game", 1);
	glutAddMenuEntry("Continue", 2);
	glutAddMenuEntry("Save", 5);
	glutAddSubMenu("Settings", options);
	glutAddMenuEntry("Help", 8);

	glutAddMenuEntry("Exit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//actiunile la selectarea optiunilor din meniu
void action_menu(int option) {
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
		board_init();
		break;
	case 2:
		TYPE1 = WHITE_CHECKER;
		TYPE2 = BLACK_CHECKER;
		init_from_file();
		if (SIDE_COEF == -1)
			glRotatef(180, 0, 0, 1);
		break;
	case 3:
		SIDE_COEF *= -1;
		MOUSEX = MOUSEY = -241;
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
	case 9:
		enable_texture = !enable_texture;
		break;
	case 10:
		enable_lighting = !enable_lighting;
		break;
	case 11:
		enable_normal = !enable_normal;
		break;
	case 12:
		light_type = !light_type;
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

void draw_circle(float cx, float cy, float radius, const int num_segments, const glm::vec3 color, GLuint& circle_vbo, int c, bool scalation=false)
{
	glUseProgram(lighting_shader_programme);
	glEnableVertexAttribArray(0);
	
	GLuint color_id = glGetUniformLocation(lighting_shader_programme, "color");
	glUniform3fv(color_id, 1, glm::value_ptr(color));

	GLuint lightingEn_id = glGetUniformLocation(lighting_shader_programme, "enableLighting");
	glUniform1i(lightingEn_id, enable_lighting);

	GLuint lighting_type = glGetUniformLocation(lighting_shader_programme, "lightingType");
	glUniform1i(lighting_type, light_type);
	
	update_uniform_fragment_shader(lighting_shader_programme);
	
	if (scalation)
	{
		cx /= 275;
		cy /= 275;
		radius /= 275;
	}

	float* circle_buffer = new float[3 * num_segments];
	
	for (auto ii = 0; ii < num_segments; ii++)
	{
		auto theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

		auto x = radius * cosf(theta);
		auto y = radius * sinf(theta);

		circle_buffer[3 * ii] = c* (x + cx);
		circle_buffer[3 * ii + 1] = c* (y + cy);
		circle_buffer[3 * ii + 2] = 0;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, circle_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * num_segments * sizeof(float), circle_buffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_LINE_LOOP, 0, num_segments);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	delete[] circle_buffer;
}

void draw_checkers_piece(float board_x, float board_y, const int num_segments, const glm::vec3 color, GLuint& checkers_vbo, int c, bool scalation = false)
{
	glUseProgram(lighting_shader_programme);
	glEnableVertexAttribArray(0);

	GLuint color_id = glGetUniformLocation(lighting_shader_programme, "color");
	glUniform3fv(color_id, 1, glm::value_ptr(color));

	GLuint lightingEn_id = glGetUniformLocation(lighting_shader_programme, "enableLighting");
	glUniform1i(lightingEn_id, enable_lighting);

	GLuint lighting_type = glGetUniformLocation(lighting_shader_programme, "lightingType");
	glUniform1i(lighting_type, light_type);
	
	update_uniform_fragment_shader(lighting_shader_programme);

	float *checker_positions = new float[3 * num_segments];
	for (auto k = 0; k < num_segments; k++) {
		auto x = 2 * M_PI * k / static_cast<float>(num_segments);
		checker_positions[3 * k] = 25 * cos(x) + (board_x - 30 + board_x + 30) / 2;
		checker_positions[3 * k + 1] = 25 * sin(x) + (board_y - 30 + board_y + 30) / 2;
		checker_positions[3 * k + 2] = 0;
	}
	if (c < 0)
	{
		for (auto k = 0; k < 3* num_segments; k++) {
			checker_positions[k] *= c;
		}
	}
	

	if (scalation)
	{
		for (auto checker = 0; checker < 3 * num_segments; checker++)
		{
			checker_positions[checker] /= 275;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, checkers_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * num_segments * sizeof(float), checker_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_POLYGON, 0, num_segments);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	delete[] checker_positions;
}

void draw_board_square(int i, int j, const glm::vec3 color, GLuint& vbo)
{
	glUseProgram(lighting_shader_programme);
	glEnableVertexAttribArray(0);

	GLuint color_id = glGetUniformLocation(lighting_shader_programme, "color");
	glUniform3fv(color_id, 1, glm::value_ptr(color));

	GLuint lightingEn_id = glGetUniformLocation(lighting_shader_programme, "enableLighting");
	glUniform1i(lightingEn_id, enable_lighting);

	GLuint lighting_type = glGetUniformLocation(lighting_shader_programme, "lightingType");
	glUniform1i(lighting_type, light_type);
	
	update_uniform_fragment_shader(lighting_shader_programme);

	auto* const current_square = board_squares[i][j];

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), current_square, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

void update_uniform_fragment_shader(GLuint &shader_programme)
{
	model_matrix = glm::mat4();
	GLuint lightPosLoc = glGetUniformLocation(shader_programme, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light_pos));

	GLuint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(view_pos));

	//modelMatrix *= glm::rotate(rotAngle, glm::vec3(0, 1, 0));
	GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "mvpMatrix");
	auto mvp = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	// se determina matricea ce realizeaza corectia normalelor. Ea se trimite catre vertex shader la fel cum s-a procedat si cu mvpMatrix 
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model_matrix));
	GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

auto set_texture(char* filename, GLuint& shader_programme, GLuint& texture) -> void
{
	glUseProgram(shader_programme);

	//load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_DEPTH_TEST);
	glewInit();

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &color_vbo);
	glGenBuffers(1, &checkers_vbo);
	glGenBuffers(1, &crown_vbo);
	glGenBuffers(1, &board_texture_vbo);
	glGenBuffers(1, &circle_vbo);
	
	create_shader_program((char*)"shader/light_vertex.shader", (char*)"shader/light_fragment.shader", lighting_shader_programme);
	create_shader_program((char*)"shader/btext_vertex.shader", (char*)"shader/btext_fragment.shader", texture_shader_programme);

	set_texture((char*)"textures/board.jpg", texture_shader_programme, board_texture);
	set_texture((char*)"textures/board_normal.jpg", texture_shader_programme, board_texture_normal);
	
	create_menu();
	glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glOrtho(-275.0, 275.0, -275.0, 275.0, 0.0, 1.0);
	board_init();
}

void draw_board_with_texture(GLuint &board_texture_vbo)
{
	glUseProgram(texture_shader_programme);
	glEnableVertexAttribArray(0);
		
	glBindBuffer(GL_ARRAY_BUFFER, board_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), full_board, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	GLuint texture1ID = glGetUniformLocation(texture_shader_programme, "textureColor");
	glUniform1i(texture1ID, 0);
	GLuint texture2ID = glGetUniformLocation(texture_shader_programme, "textureNormal");
	glUniform1i(texture2ID, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, board_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, board_texture_normal);
		
	GLuint enable_normal_1 = glGetUniformLocation(texture_shader_programme, "enableNormal");
	glUniform1i(enable_normal_1, enable_normal);
	GLuint lighting_en_id = glGetUniformLocation(texture_shader_programme, "enableLighting");
	glUniform1i(lighting_en_id, enable_lighting);
		
	update_uniform_fragment_shader(texture_shader_programme);
		
	glDrawArrays(GL_QUADS, 0, 4);
		
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glUseProgram(0);
}

void draw_crown(GLuint crown_vbo, int i, int j, int c)
{
	glm::vec3 color;
	
	GLuint color_id = glGetUniformLocation(lighting_shader_programme, "color");
	//desenam coronita la dame
	const auto mx = board[i][j].x;
	const auto my = board[i][j].y;

	float crown[3 * 8] = {
		mx,			my - 10,	 0.0f,
		mx + 10,	my - 10,	 0.0f,
		mx + 10,	my + 10,	 0.0f,
		mx + 5,		my - 5,		0.0f,
		mx,			my + 10,	 0.0f,
		mx - 5,		my - 5,		0.0f,
		mx - 10,	my + 10,	 0.0f,
		mx - 10,	my - 10,	 0.0f
	};
	for (auto& crown_angle : crown)
	{
		crown_angle /= 275;
	}
	if (c < 0)
	{
		for (auto& crown_angle : crown)
		{
			crown_angle *= c;
		}
	}
	if (board[i][j].check == WHITE_CHECKER)
		color = glm::vec3(0.0, 0.0, 1.0);
	else if (board[i][j].check == BLACK_CHECKER)
		color = get_alternate_color_2(type2_color);
				
	glUseProgram(lighting_shader_programme);
	glEnableVertexAttribArray(0);

	glUniform3fv(color_id, 1, glm::value_ptr(color));
	glBindBuffer(GL_ARRAY_BUFFER, crown_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), crown, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_POLYGON, 0, 8);

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

//functia de desenare grafica principala
void display() {
	//curatam ecranul
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(0);
	draw_background();
	
	glm::vec3 color;

	if (enable_texture)
		draw_board_with_texture(board_texture_vbo);
	else
		for (auto i = 0; i < ROWS; i++)
			for (auto j = 0; j < COLUMNS; j++)
				draw_board_square(i, j, board_square_color, vbo);
	
	//draw checkers pieces
	for (auto i = 0; i < ROWS; i++)
	{
		for (auto j = 0; j < COLUMNS; j++)
		{
			//transformation time, become queen if needed
			if (i == 0 && board[i][j].check == TYPE2)//'B')
				board[i][j].type = KING;

			if (i == 7 && board[i][j].check == TYPE1)//'W')
				board[i][j].type = KING;

			//color the checkers according to type
			if (board[i][j].check == WHITE_CHECKER)
				color = type1_color;
			else if (board[i][j].check == BLACK_CHECKER)
				color = type2_color;
			else
				continue;

			//selectarea unei piese
			if (board[i][j].check == GO && MOUSEX < board[i][j].x + 30 && MOUSEX > board[i][j].x - 30 && MOUSEY < board[i][j].y + 30 && MOUSEY > board[i][j].y - 30)
			{
				if (!jump_list.empty())
				{
					if (list_contain_element(jump_list, i, j) && board[i][j].check == GO)
					{
						if (PRESSED)
						{
							sel.first = i;
							sel.second = j;
						}
						//culoarea pieselor selectate in dependenta de tip
						if (board[i][j].check == WHITE_CHECKER)
							color = type1_selected_color;
						else if (board[i][j].check == BLACK_CHECKER)
							color = type2_selected_color;
					}
				}

				else if (!move_list.empty())
					if (list_contain_element(move_list, i, j))
					{
						if (PRESSED)
						{
							sel.first = i;
							sel.second = j;
						}

						//culoarea pieselor selectate in dependenta de tip
						if (board[i][j].check == WHITE_CHECKER)
							color = type1_selected_color;
						else if (board[i][j].check == BLACK_CHECKER)
							color = type2_selected_color;

					}
			}

			//desenam piesele de joc (octagoane)
			//conform formulelor parametrice a cercului
			const auto circle_precision = 16;

			draw_checkers_piece(board[i][j].x, board[i][j].y, 16, color, checkers_vbo, SIDE_COEF, true);
			draw_circle(board[i][j].x, board[i][j].y, 30 - 10, 30, get_alternate_color(color), color_vbo, SIDE_COEF, true);
			if (board[i][j].type != KING)
			{
				draw_circle(board[i][j].x, board[i][j].y, 30 - 17, 30, get_alternate_color(color), color_vbo, SIDE_COEF, true);
				draw_circle(board[i][j].x, board[i][j].y, 30 - 24, 30, get_alternate_color(color), color_vbo, SIDE_COEF, true);
			} else
			{
				draw_crown(crown_vbo, i, j, SIDE_COEF);
			}
		}
	}

	//desenam cifrele din stinga tablei, literele de jos si doua linii de contur
	draw_around(SIDE_COEF);
	draw_light_position(light_pos.x, light_pos.y, SIDE_COEF);
	
	//Desenam toate miscarile posibile daca exista
	if (POS_MOVES) draw_possible_moves();

	//desenam output-ul meniului "Ajutor"
	if (HELP)
	{
		show_help(SIDE_COEF);
		if (PRESSED)
			if (MOUSEX * SIDE_COEF > 190 && MOUSEY * SIDE_COEF > 140 && MOUSEX * SIDE_COEF < 210 && MOUSEY * SIDE_COEF < 160)
			{
				HELP = false;
			}
	}
	else
	{
		if (!check_list.empty()) check_list.clear();
		if (!jump_list.empty()) jump_list.clear();
		if (!move_list.empty()) move_list.clear();

		list_of_jumpes(jump_list, check_list);
		list_of_moves(move_list);

		if (TYPE1 == TYPE2)
			show_intro(SIDE_COEF);
		else
		{
			//afisam invingatorul sau detinatorul de miscare in timpul dat
			if (count_checkers(WHITE_CHECKER) == 0 || no_more_moves(WHITE_CHECKER))
			{
				show_winner("Black wins!", SIDE_COEF);
				show_turn("Black wins!", SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
			}
			else if (count_checkers(BLACK_CHECKER) == 0 || no_more_moves(BLACK_CHECKER))
			{
				show_winner("White wins!", SIDE_COEF);
				show_turn("White wins!", SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
			}
			else if (GO == WHITE_CHECKER)
				show_turn("White's turn", SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
			else if (GO == BLACK_CHECKER)
				show_turn("Black's turn", SIDE_COEF, count_checkers(WHITE_CHECKER), count_checkers(BLACK_CHECKER));
		}
	}
	glFlush();
}

