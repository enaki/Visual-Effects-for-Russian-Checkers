#pragma once
#include <GL/freeglut_std.h>
#include "../data.h"

namespace uimanager{
	
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
		case 'q':
			light_pos[0] -= 0.5;
			light_pos[1] += 0.5;
			break;
		case 'e':
			light_pos[0] += 0.5;
			light_pos[1] += 0.5;
			break;
		case 'z':
			light_pos[0] -= 0.5;
			light_pos[1] -= 0.5;
			break;
		case 'x':
			light_pos[0] += 0.5;
			light_pos[1] -= 0.5;
			break;
		case 'l':
			enable_lighting = !enable_lighting;
			break;
		case 't':
			enable_texture = !enable_texture;
			break;
		case 'n':
			enable_normal = !enable_normal;
			break;
		case 'o':
			light_type = !light_type;
			break;
		}
		glutPostRedisplay();
	}
};