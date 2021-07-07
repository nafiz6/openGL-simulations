#ifndef LISTENER_H
#define LISTENER_H

#include "Camera.h"
#include <vector>
#include <GL/glut.h>
#include "Object.h"

extern int drawaxes;

extern Camera camera;
extern Object lBall;
extern Object rBall;
extern Object cylinder;
extern Object lCylinderBall;
extern Object rCylinderBall;
extern std::vector<point> bullets;


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			camera.rotateLR(1);
			break;

		case '2':
			camera.rotateLR(-1);
			break;

		case '3':
			camera.rotateUD(1);
			break;

		case '4':
			camera.rotateUD(-1);
			break;

		case '5':
			camera.tiltCW(1);
			break;

		case '6':
			camera.tiltCW(-1);
			break;

		case 'q':
			lBall.updateAngle(1, 2);
			rBall.updateAngle(1, 2);
			break;

		case 'w':
			lBall.updateAngle(-1, 2);
			rBall.updateAngle(-1, 2);
			break;
			
		case 'e':
			rBall.updateAngle(1, 1);
			break;

		case 'r':
			rBall.updateAngle(-1, 1);
			break;

		case 'a':
			cylinder.updateAngle(1, 1);
			break;

		case 's':
			cylinder.updateAngle(-1, 1);
			break;
			
		case 'd':
			cylinder.updateAngle(-1, 3);
			break;

		case 'f':
			cylinder.updateAngle(1, 3);
			break;

		default:
			break;
	}
	
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			camera.moveForward(1);
			break;
		case GLUT_KEY_UP:		// up arrow key
			camera.moveForward(-1);
			break;

		case GLUT_KEY_RIGHT:
			camera.moveLR(1);
			break;
		case GLUT_KEY_LEFT:
			camera.moveLR(-1);
			break;

		case GLUT_KEY_PAGE_UP:
			camera.moveUD(1);
			break;
		case GLUT_KEY_PAGE_DOWN:
			camera.moveUD(-1);
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

point bulletCollision(){
	point res;
	res.x = tan((-cylinder.angleY - rBall.angleY)*pi / 180.0) * (800 - rBall.radius);
	res.y = tan((cylinder.angleX + rBall.angleX)*pi / 180.0) * (800 - rBall.radius);
	res.z = 800 - 3;

	return res;
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				//shoot
				point bullet = bulletCollision();	
				if (bullet.x <= 400 && bullet.x >=-400 && bullet.y <= 400 && bullet.y >= -400){
					bullets.push_back(bullet);
				}
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
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