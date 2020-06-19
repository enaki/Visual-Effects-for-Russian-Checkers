#pragma once
#include <GL/freeglut_std.h>

namespace uimanager{
	int WIN;
	
	void keyboard_listener(unsigned char key, int x, int y) {
		switch (key) {
		case 27:
			glutDestroyWindow(WIN);
			exit(0);
		}
	}
};