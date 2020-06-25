#include <cstring>
#include <GL/glut.h>
#include "grafix.hpp"

#include <string>
#include <glm/detail/type_vec3.hpp>

glm::vec3 menu_color(0.1f, 0.1f, 0.1f);
glm::vec3 menu_color_2(0.3f, 0.3f, 0.3f);

glm::vec3 menu_text_color1(1.0f, 1.0f, 1.0f);
glm::vec3 menu_text_color2(0.4f, 0.2f, 0.2f);

glm::vec3 winner_color(0.2f, 1.0f, 0.2f);

//functia care arata la intrarea in joc numele autorului
void show_intro(int c) {
	//desenam un patrat suriu
	glBegin(GL_QUADS);
	glColor4f(menu_color.x, menu_color.y, menu_color.z, 0.5f);
	glVertex2f(-100.0f * c, 80.0f * c);
	glVertex2f(100.0f * c, 80.0f * c);
	glVertex2f(100.0f * c, -80.0f * c);
	glVertex2f(-100.0f * c, -80.0f * c);
	glEnd();


	//scrim in patratul desenat mai sus
	std::string str1 = "Russian Checkers";
	glColor3f(menu_text_color2.x, menu_text_color2.y, menu_text_color2.z);
	glRasterPos2f(-85.0f * c, 15.0f * c);
	for (auto i : str1)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);

	glColor3f(menu_text_color1.x, menu_text_color1.y, menu_text_color1.z);
	glRasterPos2f(-90.0f * c, 20.0f * c);
	for (auto i : str1)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);

	std::string str2 = "Have fun!";
	
	glColor3f(menu_text_color2.x, menu_text_color2.y, menu_text_color2.z);
	glRasterPos2f(-60.0f * c, -40.0f * c);
	for (auto i : str2)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);

	glColor3f(menu_text_color1.x, menu_text_color1.y, menu_text_color1.z);
	glRasterPos2f(-55.0f * c, -35.0f * c);
	for (auto i : str2)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);
}

//Afiseaza un mesaj cind este un cistigator
void show_winner(const char* string, int c) {
	//desenam un dreptunghi suriu
	glBegin(GL_QUADS);
	glColor4f(menu_color.x, menu_color.y, menu_color.z, 0.5f);
	glVertex2f(-110.0f * c, 50.0f * c);
	glVertex2f(110.0f * c, 50.0f * c);
	glVertex2f(110.0f * c, -50.0f * c);
	glVertex2f(-110.0f * c, -50.0f * c);
	glEnd();

	//scrim cine a cistigat
	glColor3f(winner_color.x, winner_color.y, winner_color.z);
	glRasterPos2f(-90.0f * c, -5.0f * c);
	for (size_t i = 0; i < strlen(string); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

//outputul meniului "Ajutor"
void show_help(int c) {
	//desenam un dreptunghi suriu
	glBegin(GL_QUADS);
	glColor4f(menu_color.x, menu_color.y, menu_color.z, 0.5f);
	glVertex2f(-200.0f * c, 150.0f * c);
	glVertex2f(200.0f * c, 150.0f * c);
	glVertex2f(200.0f * c, -190.0f * c);
	glVertex2f(-200.0f * c, -190.0f * c);

	//desenam un patrat mic cu x, pentru a iesi din meniu
	glColor4f(menu_color_2.x, menu_color_2.y, menu_color_2.z, 0.5f);
	glVertex2f(190.0f * c, 160.0f * c);
	glVertex2f(210.0f * c, 160.0f * c);
	glVertex2f(210.0f * c, 140.0f * c);
	glVertex2f(190.0f * c, 140.0f * c);
	glEnd();

	//desenam x-ul de iesire
	glColor3f(1, 0, 0);
	glRasterPos2f(193.0f * c, 145.0f * c);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');

	//afisam mesajele
	std::string str1 = "Russian Checkers";
	std::string str2 = "Fore more info:";
	std::string str3 = "https://en.wikipedia.org/wiki/Russian_draughts";
	std::string str4 = "Controls";
	std::string str[16] = {
		"w: Light Up",
		"s: Light Down",
		"a: Light Left",
		"d: Light Right",
		"q: Light Up-Left",
		"e: Light Up-Right",
		"z: Light Down-Left",
		"x: Light Down-Right",
		"l: Light On/Off",
		"t: Texture On/Off",
		"n: Normals On/Off",
		"o: Change Light Type",
		"_: Mul SpecPower by 2",
		"+: Div SpecPower by 2",
		"-: Dec SpecPower",
		"=: Inc SpecPower"
	};

	glColor3f(1, 0, 0);
	//string
	glRasterPos2f(-100.0f * c, 110.0f * c);
	for (auto i : str1)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);
	//string2
	glColor3f(menu_text_color1.x, menu_text_color1.y, menu_text_color1.z);
	glRasterPos2f(-80.0f * c, 75.0f * c);
	for (auto i : str2)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
	//string3
	glRasterPos2f(-190.0f * c, 50.0f * c);
	for (auto i : str3)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
	//string4
	glColor3f(1, 1, 0);
	glRasterPos2f(-60.0f * c, 00.0f * c);
	for (auto i : str4)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);

	//controls
	glColor3f(1, 0, 1);
	for (int i = 0; i < 8; i++)
	{
		glRasterPos2f(-180.0f * c, (-20.0f - i * 20) * c);
		for (auto letter : str[i])
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
		glRasterPos2f(10.0f * c, (-20.0f - i * 20) * c);
		for (auto letter : str[i+8])
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
	}
}

//deseneaza cifrele si literele in jurul tablei
//deasemenea doua linii de contur ale tablei
void draw_around(int c) {
	//desenam prima linie de contur
	glBegin(GL_LINE_STRIP);
	glVertex2f(-240.0f, -240.0f);
	glVertex2f(-240.0f, 240.0f);
	glVertex2f(240.0f, 240.0f);
	glVertex2f(240.0f, -240.0f);
	glVertex2f(-240.0f, -240.0f);
	glEnd();

	//selectam culoarea neagra
	glColor3f(1, 1, 1);
	char l = '1';
	char s = 'A';
	//afisam cifrele si literele
	for (float i = -210; i < 260; i += 60) {
		glRasterPos2f(-265.0f * c, i);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, l);
		l++;
		glRasterPos2f((i - 10.0f) * c, -265.0f * c);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s);
		s++;
	}

	//desenam prima linie de contur
	glBegin(GL_LINE_STRIP);
	glVertex2f(-240.0f, -240.0f);
	glVertex2f(-240.0f, 240.0f);
	glVertex2f(240.0f, 240.0f);
	glVertex2f(240.0f, -240.0f);
	glVertex2f(-240.0f, -240.0f);
	glEnd();

	//desenam a doua linie de contur
	glBegin(GL_LINE_STRIP);
	glVertex2f(-245, -245);
	glVertex2f(-245, 245);
	glVertex2f(245, 245);
	glVertex2f(245, -245);
	glVertex2f(-245, -245);
	glEnd();
}

void draw_background()
{
	//background-ul
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(-275.0f, -275.0f);
	glVertex2f(-275.0f, 275.0f);
	glVertex2f(275.0f, 275.0f);
	glVertex2f(275.0f, -275.0f);
	glEnd();

	//water-ul
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex2f(-240.0f, -240.0f);
	glVertex2f(-240.0f, 240.0f);
	glVertex2f(240.0f, 240.0f);
	glVertex2f(240.0f, -240.0f);
	glEnd();
}

//afiseaza cine detine miscarea si numarul de piese
void show_turn(const char* s, int c, int white, int black) {
	glColor3f(menu_text_color1.x, menu_text_color1.y, menu_text_color1.z);
	glRasterPos2f(-40.0f * c, 255.0f * c);
	//afisam cine detine miscarea
	for (size_t i = 0; i < strlen(s); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);

	std::string str1 = "White: ";
	std::string str2 = "Black: ";
	//afisam cite piese Albe sunt
	glRasterPos2f(-240.0f * c, 255.0f * c);
	for (auto i : str1)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);

	glRasterPos2f(-190.0f * c, 255.0f * c);
	if (white > 9)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, white / 10 + 48);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, white % 10 + 48);

	//afisam cite piese Negre sunt
	glRasterPos2f(160.0f * c, 255.0f * c);
	for (auto i : str2)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);

	glRasterPos2f(225.0f * c, 255.0f * c);
	if (black > 9)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, black / 10 + 48);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, black % 10 + 48);
}

void draw_light_position(float x, float y, int c)
{
	//selectam culoarea alba
	char buffer[40];
	glColor3f(1, 1, 0);
	sprintf_s(buffer, "Lx: %.2f\0", x);
	glRasterPos2f(225.0f, -258.0f);
	for (size_t i = 0; i < strlen(buffer); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, buffer[i]);
	sprintf_s(buffer, "Ly: %.2f\0", y);
	glRasterPos2f(225.0f, -268.0f);
	for (size_t i = 0; i < strlen(buffer); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, buffer[i]);
}

void draw_light_spec_power(float spec, int c)
{
	char buffer[40];
	glColor3f(0, 1, 1);
	sprintf_s(buffer, "Sp: %.0f\0", spec);
	glRasterPos2f(-270.0f, -262.0f);
	for (size_t i = 0; i < strlen(buffer); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, buffer[i]);
}