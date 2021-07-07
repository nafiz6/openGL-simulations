#ifndef LISTENER_H
#define LISTENER_H

#include <glut.h>
#include <vector>
#include "Bubbles.h"
#define bubbleCount 5

extern bool paused;
extern double cameraAngle;
extern int drawaxes;

extern std::vector<Bubbles >bubbles;

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'p':
			paused =!paused;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			for (int i=0; i<bubbleCount; i++){
				bubbles[i].decreaseSpeed();
			}
			break;
		case GLUT_KEY_UP:		// up arrow key
			for (int i=0; i<bubbleCount; i++){
				bubbles[i].increaseSpeed();
			}
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

#endif
