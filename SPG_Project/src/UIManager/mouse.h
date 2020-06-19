#pragma once
#include <GL/freeglut_std.h>

namespace uimanager {
	int MOUSEX = 0;
	int MOUSEY = 0;
	int PRESSED = 0;
	int SIDE_COEF;


	//fixeaza x si y daca butonul sting al mouse-ului este aparat
	void mouse_listener(int button, int state, int x, int y) {
		if (button == GLUT_LEFT_BUTTON) {
			MOUSEX = (x - 275) * SIDE_COEF;
			MOUSEY = (y - 275) * (-1) * SIDE_COEF;
			PRESSED = state == GLUT_DOWN;
		}
	}

	//fixeaza coordonatele cind se trage mouse-ul cu un buton apasat
	void motion_listener(int x, int y) {
		MOUSEX = (x - 275) * SIDE_COEF;
		MOUSEY = (y - 275) * (-1) * SIDE_COEF;
	}
}