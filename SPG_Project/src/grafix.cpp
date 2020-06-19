#include <string.h>
#include <GL/glut.h>
#include "grafix.hpp"

#include <string>

//functia care arata la intrarea in joc numele autorului
void showIntro(int c) {
	//desenam un patrat suriu
	glBegin(GL_QUADS);
	glColor4f(0.7, 0.7, 0.7, 0.5);
	glVertex2f(-100 * c, 80 * c);
	glVertex2f(100 * c, 80 * c);
	glVertex2f(100 * c, -80 * c);
	glVertex2f(-100 * c, -80 * c);
	glEnd();


	//scrim in patratul desenat mai sus
	int i;
	std::string str1 = "Russian Checkers";
	glColor3f(0.6,  0.6, 0.6);
	glRasterPos2f(-55 * c, 25 * c);
	for (i = 0; i < str1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str1[i]);

	glColor3f(0.1, 0.2, 0.2);
	glRasterPos2f(-60 * c, 30 * c);
	for (i = 0; i < str1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str1[i]);

	std::string str2 = "Have fun!";
	glColor3f(0.6, 0.6, 0.6);
	glRasterPos2f(-65 * c, -35 * c);
	for (i = 0; i < str2.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[i]);

	glColor3f(0.1, 0.2, 0.2);
	glRasterPos2f(-70 * c, -40 * c);
	for (i = 0; i < str2.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[i]);
}

//Afiseaza un mesaj cind este un cistigator
void showWiner(const char* string, int c) {
	//desenam un dreptunghi suriu
	glBegin(GL_QUADS);
	glColor4f(0.7, 0.7, 0.7, 0.5);
	glVertex2f(-110 * c, 50 * c);
	glVertex2f(110 * c, 50 * c);
	glVertex2f(110 * c, -50 * c);
	glVertex2f(-110 * c, -50 * c);
	glEnd();

	//scrim cine a cistigat
	int i;
	glColor3f(0.5, 0.5, 0.5);
	glRasterPos2f(-90 * c, -5 * c);
	for (i = 0; i < strlen(string); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);

	glColor3f(0, 0, 0.8);
	glRasterPos2f(-95 * c, 0);
	for (i = 0; i < strlen(string); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

//outputul meniului "Ajutor"
void showHelp(int c) {
	//desenam un dreptunghi suriu
	glBegin(GL_QUADS);
	glColor4f(0.7, 0.7, 0.7, 0.5);
	glVertex2f(-200 * c, 150 * c);
	glVertex2f(200 * c, 150 * c);
	glVertex2f(200 * c, -150 * c);
	glVertex2f(-200 * c, -150 * c);

	//desenam un patrat mic cu x, pentru a iesi din meniu
	glColor4f(0.7, 0.7, 0.7, 0.5);
	glVertex2f(190 * c, 160 * c);
	glVertex2f(210 * c, 160 * c);
	glVertex2f(210 * c, 140 * c);
	glVertex2f(190 * c, 140 * c);
	glEnd();

	//desenam x-ul de iesire
	glColor3f(0, 0, 0);
	glRasterPos2f(193 * c, 145 * c);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');

	//afisam mesajele
	std::string str1 = "Russian Checkers";
	std::string str2 = "Fore more info:";
	std::string str3 = "https://en.wikipedia.org/wiki/Russian_draughts";
	int i;

	glColor3f(0.3, 0.4, 0.3);
	//string
	glRasterPos2f(-180 * c, 110 * c);
	for (i = 0; i < str1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str1[i]);
	//string2
	glRasterPos2f(-180 * c, 90 * c);
	for (i = 0; i < str2.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str2[i]);
	//string3
	glRasterPos2f(-180 * c, 50 * c);
	for (i = 0; i < str3.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str3[i]);
}

//deseneaza cifrele si literele in jurul tablei
//deasemenea doua linii de contur ale tablei
void drawAround(int c) {
	//selectam culoarea neagra
	glColor3f(0, 0, 0);
	int i;
	char l = '1';
	char s = 'A';
	//afisam cifrele si literele
	for (i = -210; i < 260; i += 60) {
		glRasterPos2f(-265 * c, i);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, l);
		l++;
		glRasterPos2f((i - 10) * c, -265 * c);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s);
		s++;
	}

	//desenam prima linie de contur
	glBegin(GL_LINE_STRIP);
	glVertex2f(-240, -240);
	glVertex2f(-240, 240);
	glVertex2f(240, 240);
	glVertex2f(240, -240);
	glVertex2f(-240, -240);
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

//afiseaza cine detine miscarea si numarul de piese
void showTurn(const char* s, int c, int white, int black) {
	//selectam culoarea neagra
	glColor3f(0, 0, 0);
	glRasterPos2f(-40 * c, 255 * c);
	int i;
	//afisam cine detine miscarea
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);

	std::string str1 = "White: ";
	std::string str2 = "Black: ";
	//afisam cite piese Albe sunt
	glRasterPos2f(-240 * c, 255 * c);
	for (i = 0; i < str1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str1[i]);

	glRasterPos2f(-190 * c, 255 * c);
	if (white > 9)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, white / 10 + 48);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, white % 10 + 48);

	//afisam cite piese Negre sunt
	glRasterPos2f(160 * c, 255 * c);
	for (i = 0; i < str2.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str2[i]);

	glRasterPos2f(225 * c, 255 * c);
	if (black > 9)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, black / 10 + 48);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, black % 10 + 48);
}
