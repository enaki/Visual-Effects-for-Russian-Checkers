#pragma once
#include <GL/freeglut_std.h>
#include "data.h"

namespace uimanager{
	int WIN;
	
	void keyboard_listener(unsigned char key, int x, int y) {
		switch (key) {
		case 27:
			glutDestroyWindow(WIN);
			exit(0);
		case 'w':
			light_pos[1] += 0.5;
			break;
		case 'a':
			light_pos[0] -= 0.5;
			break;
		case 's':
			light_pos[1] -= 0.5;
			break;
		case 'd':
			light_pos[0] += 0.5;
			break;
		}
		glutPostRedisplay();
	}
};