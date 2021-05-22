#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<vector>

#include <windows.h>
#include <GL/glut.h>
#include <iostream>

#define pi (2*acos(0.0))

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};

point crossProduct(point a, point b){
	struct point res;
	res.x = a.y*b.z - a.z*b.y;
	res.y = -(a.x*b.z - a.z*b.x);
	res.z = a.x*b.y - a.y*b.x;
	return res;
}

double magnitude(point a){
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

point normalize(point a){
	struct point res;
	double mag = magnitude(a);
	res.x = a.x / mag;
	res.y = a.y / mag;
	res.z = a.z / mag;
	return res;
}

point scale(point vec, double val){
	struct point res;
	res.x = vec.x * val;
	res.y = vec.y * val;
	res.z = vec.z * val;
	return res;
}

void rotate(point* target, point normal, double deg){
	struct point right = normalize(crossProduct(*target, normal));
	right = scale(right, magnitude(*target));

	target->x = target->x * cos(deg) + right.x * sin(deg);
	target->y = target->y * cos(deg) + right.y * sin(deg);
	target->z = target->z * cos(deg) + right.z * sin(deg);
}

void print(point a, string name){
	cout << name << ": " << a.x  << " " << a.y << " " << a.z << " ";
}

class Object {


	public:
		double angleX=0;
		double angleY=0;
		double angleZ=0;
		double maxAngle = 50;

		double radius = 50;

		//cylinder
		double halfWidth = 100;
		void updateAngle(int dir, int axis){
			if (dir > 0){
				switch(axis){
					case 1:
						angleX += 1.5;					
						if (angleX > maxAngle)angleX = maxAngle;
						break;

					case 2:
						angleY += 1.5;
						if (angleY > maxAngle)angleY = maxAngle;
						break;

					case 3:
						angleZ += 1.5;
						if (angleZ > maxAngle)angleZ = maxAngle;
						break;
				}
			}
			else{
				switch(axis){
					case 1:
						angleX -= 1.5;					
						if (angleX < -maxAngle)angleX = -maxAngle;
						break;

					case 2:
						angleY -= 1.5;
						if (angleY < -maxAngle)angleY = -maxAngle;
						break;

					case 3:
						angleZ -= 1.5;
						if (angleZ < -maxAngle)angleZ = -maxAngle;
						break;
				}


				

			}
		}
};


class Camera {
	int mag = 2;
	public:
	 struct point position;
	 struct point lookPosition;
	 struct point upVector;
	 struct point lookVector;

	 void updateCamera(){
		 lookPosition.x = position.x + lookVector.x;
		 lookPosition.y = position.y + lookVector.y;
		 lookPosition.z = position.z + lookVector.z;
	 }

	 Camera(){
		 position.x = 200;
		 position.y = 0;
		 position.z = 0;

		 lookVector.x = -200;
		 lookVector.y = -0;
		 lookVector.z = -0;

		 upVector.x = 0;
		 upVector.y = 1;
		 upVector.z = 0;
		 updateCamera();
	 }

	 void moveForward(int dir){
		 struct point normalisedLook = normalize(lookVector);
		 dir*=mag;

		 position.x -= normalisedLook.x*dir;
		 position.y -= normalisedLook.y*dir;
		 position.z -= normalisedLook.z*dir;

		 updateCamera();
	 }

	 void moveLR(int dir){
		 struct point lr = normalize(crossProduct(lookVector, upVector));
		 dir*=mag;
		 position.x += lr.x * dir;
		 position.y += lr.y * dir;
		 position.z += lr.z * dir;

		 updateCamera();
	 }

	 void moveUD(int dir){
		 dir*=mag;
		 position.x += upVector.x * dir;
		 position.y += upVector.y * dir;
		 position.z += upVector.z * dir;

		 updateCamera();
	 }

	 void rotateLR(int dir){
		 double degree = -3 * dir * pi /180.0;
		 rotate(&lookVector, upVector, degree);

		 updateCamera();
	 }

	 void rotateUD(int dir){
		 double degree = -3 * dir * pi /180.0;
		 struct point right = normalize(crossProduct(lookVector, upVector));
		 rotate(&lookVector, right, degree);
		 rotate(&upVector, right, degree);

		 updateCamera();
	 }

	 void tiltCW(int dir){
		 double degree = -3 * dir * pi /180.0;
		 rotate(&upVector, lookVector, degree);

		 updateCamera();
	 }
};


Camera camera;
Object lBall;
Object rBall;
Object cylinder;
Object lCylinderBall;
Object rCylinderBall;
vector<point> bullets;

point bulletCollision(){
	point res;
	res.x = tan((-cylinder.angleY - rBall.angleY)*pi / 180.0) * (800 - rBall.radius);
	res.y = tan((cylinder.angleX + rBall.angleX)*pi / 180.0) * (800 - rBall.radius);
	res.z = 800 - 3;

	cout << "BULLET " << res.x << " " << res.y << endl;
	return res;
}


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCenterCylinder(){
	double radius = 30;
	double halfWidth = cylinder.halfWidth;
	int stacks = 30;
	struct point points[100];
	double h,r;

	for (int i=0; i<=stacks;i++){
		h=radius*sin(((double)i/(double)stacks)*(pi));
		r=radius*cos(((double)i/(double)stacks)*(pi));
		points[i].x=r;
		points[i].y=h;
		points[i].z=0;
	}

	for (int i=0; i<stacks;i++){

		glBegin(GL_QUADS);
		{
			if (i % 2 == 0)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0, 0, 0);

			glVertex3f(points[i].x, points[i].y, points[i].z - halfWidth);
			glVertex3f(points[i].x, points[i].y, points[i].z + halfWidth);
			glVertex3f(points[i + 1].x, points[i + 1].y, points[i + 1].z + halfWidth);
			glVertex3f(points[i + 1].x, points[i + 1].y, points[i + 1].z - halfWidth);

			if (i % 2 != 0)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0, 0, 0);

			glVertex3f(points[i].x, -points[i].y, points[i].z - halfWidth);
			glVertex3f(points[i].x, -points[i].y, points[i].z + halfWidth);
			glVertex3f(points[i + 1].x, -points[i + 1].y, points[i + 1].z + halfWidth);
			glVertex3f(points[i + 1].x, -points[i + 1].y, points[i + 1].z - halfWidth);
		}
		glEnd();
	}

}

void drawCenterSphere(int type, double radius){
	int slices = 60;
	int stacks = 20;
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			if (j%2 == 0)
				glColor3f(1.0,1.0,1.0);
			else
				glColor3f(0,0,0);

			glBegin(GL_QUADS);{
			    //upper hemisphere
				if (type == 1){
					glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
				}
                //lower hemisphere
				
				else {
					glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
					glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
					glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
					glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
				}
			}glEnd();
		}
	}


}

void drawCenterConvex(double radius){
	int slices = 60;
	int stacks = 20;
	struct point points[100][100];
	int i,j;
	double h,r,r2;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=(2*radius - r)*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=(2*radius - r)*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			if (j%2 == 0)
				glColor3f(1.0,1.0,1.0);
			else
				glColor3f(0,0,0);

			glBegin(GL_QUADS);{
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}


}



void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
	glColor3f(1, 0, 0);
	//left center ball
	glPushMatrix();
	{
		glRotatef(lBall.angleX, 1, 0, 0);
		glRotatef(lBall.angleY, 0, 1, 0);
		glRotatef(lBall.angleZ, 0, 0, 1);
		drawCenterSphere(1, lBall.radius);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotatef(rBall.angleX, 1, 0, 0);
		glRotatef(rBall.angleY, 0, 1, 0);
		glRotatef(rBall.angleZ, 0, 0, 1);

		//right center ball
		drawCenterSphere(2, rBall.radius);

		//CYLINDER START
		
		glTranslatef(0, 0, -rBall.radius);

		glRotatef(cylinder.angleX, 1, 0, 0);
		glRotatef(cylinder.angleY, 0, 1, 0);
		glRotatef(cylinder.angleZ, 0, 0, 1);

		//left ball cylinder
		glTranslatef(0, 0, - lCylinderBall.radius);
		drawCenterSphere(1, lCylinderBall.radius);

		glTranslatef(0, 0, -cylinder.halfWidth);
		
		//cylinder
		drawCenterCylinder();

		glTranslatef(0, 0, -cylinder.halfWidth);
		//right convex cylinder
		drawCenterConvex(rCylinderBall.radius);
	}
	glPopMatrix();

	glColor3f(0.4, 0.4, 0.45);
	glTranslatef(0,0, -800);
	drawSquare(400);

	glColor3f(0.9,0.2,0);
	glTranslatef(0, 0, 1);
	for (int i=0; i<bullets.size(); i++){
		glPushMatrix();{
			glTranslatef(bullets[i].x, bullets[i].y, 0);
			drawSquare(10);
		}
		glPopMatrix();
	}
}

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
	cout <<"key: " <<  key << endl;
	cout << "position: " << camera.position.x << " " << camera.position.y << " " << camera.position.z <<endl;
	cout << "look: " << camera.lookPosition.x << " " << camera.lookPosition.y << " " << camera.lookPosition.z <<endl;
	cout << "up: " << camera.upVector.x << " " << camera.upVector.y << " " << camera.upVector.z <<endl;
	cout << endl;
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
	cout << "key: " << key << endl;
	cout << "position: " << camera.position.x << " " << camera.position.y << " " << camera.position.z <<endl;
	cout << "look: " << camera.lookPosition.x << " " << camera.lookPosition.y << " " << camera.lookPosition.z <<endl;
	cout << "up: " << camera.upVector.x << " " << camera.upVector.y << " " << camera.upVector.z <<endl;
	cout << endl;
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				//shoot
				point bullet = bulletCollision();	
				if (bullet.x <= 400 && bullet.x >=-400 && bullet.y <= 400 && bullet.y >= -400){
					bullets.push_back(bullet);
					cout << "BULLET PUSHED" << endl;
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



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
//	gluLookAt(camera.position.x, camera.position.y, camera.position.z,	0,0,0,	0,1,0);
	gluLookAt(camera.position.x, camera.position.y, camera.position.z, 
		      camera.lookPosition.x, camera.lookPosition.y, camera.lookPosition.z,
			  camera.upVector.x, camera.upVector.y, camera.upVector.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	lCylinderBall.radius = 30;
	rCylinderBall.radius = 30;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
