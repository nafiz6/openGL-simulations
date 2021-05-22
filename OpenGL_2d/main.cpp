#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>

#include <windows.h>
#include <time.h>       /* time */

#include <glut.h>

#define pi (2*acos(0.0))

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
bool paused = false;
long long currentTime;


#define bubbleCount 5

struct point
{
	double x,y,z;
	point(double i, double j, double k){
		x=i;
		y=j;
		z=k;
	}

	point(){

	}
};


double dotProduct(point a, point b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
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

point getReflectVector(point dirVector, point normal){
	normal = normalize(normal);
	point scaledNormal = scale(normal, 2*dotProduct(dirVector,normal));
	point res;
	res.x = dirVector.x - scaledNormal.x;
	res.y = dirVector.y - scaledNormal.y;
	res.z = 0;
	return res;
}


class Bubbles{
	double minSpeed = 0.012;
	double maxSpeed = 0.1;
	double initSpeed = 0.02;
	public:
		bool inCircle;
		int id;
		double radius = 15;
		double speedX;
		double speedY;
		double posX;
		double posY;
		bool active = false;
		//double updateRate = 0.01;

		bool canReflect[bubbleCount];

		Bubbles(){
			double randAngle = pi * (rand()%90)/180.0;
			speedX = initSpeed * cos(randAngle);
			speedY = initSpeed * sin(randAngle);
			posX = -104;
			posY = -104;
			inCircle = false;
			for (int i=0; i<bubbleCount; i++){
				canReflect[i] = false;
			}
		}

		void increaseSpeed(){
			point speed = point(speedX, speedY,0);
			double currSpeed = magnitude(speed);
			if (currSpeed > maxSpeed)return;
			currSpeed+=0.01;
			speed = scale(normalize(speed), currSpeed);
			speedX = speed.x;
			speedY = speed.y;
		}

		void decreaseSpeed(){
			point speed = point(speedX, speedY, 0);
			double currSpeed = magnitude(speed);
			if (currSpeed < minSpeed)return;
			currSpeed-=0.01;
			speed = scale(normalize(speed), currSpeed);
			speedX = speed.x;
			speedY = speed.y;
		}

		void move(){
			if (!active)return;
			/*
			point normalize = point(speedX, speedY, 0);
			posX += normalize.x * updateRate;
			posY += normalize.y * updateRate;
			*/
			posX += speedX;
			posY += speedY;
		}

		bool isInCircle(){
			double distFromCenter = magnitude(point(posX, posY, 0));
			if (distFromCenter < 75-radius)
				return true;
			return false;
		}
		bool isInCircleAfter(){
			double distFromCenter = magnitude(point(posX+speedX, posY + speedY, 0));
			if (distFromCenter < 75-radius)
				return true;
			return false;
		}

		void update(){
			if (currentTime > 1000 + id*1200)active=true;
//			if (currentTime > 1000 )active=true;
			if (!active)return;
			if (!inCircle){
				inCircle = isInCircle();
			}
			/*
			int times = magnitude(point(speedX,speedY,0))/updateRate;
			for (int i=0; i<times; i++){
				*/
			checkForReflection();
			//move();
			//}

		}

		void checkForReflection();



};

bool doCollide(Bubbles a, Bubbles b){
	return  (magnitude(point(a.posX - b.posX,a.posY -b.posY,0)) <= a.radius + b.radius) ;
//		&& (magnitude(point(a.posX - b.posX,a.posY -b.posY,0)) >= 0.95*a.radius);
}

Bubbles bubbles[bubbleCount];

void Bubbles::checkForReflection(){
	point speed = point(speedX, speedY, 0);
	//cout << speed.x << " " << speed.y << " " << id<< endl;
	//box
	if (posX - radius <= -120)
	{
		point normal = point(1, 0, 0);
		speed = getReflectVector(speed, normal);
		posX = -120 + radius + 0.001;
	}
	else if (posX + radius >= 120)
	{
		point normal = point(-1, 0, 0);
		speed = getReflectVector(speed, normal);
			posX = 120 - radius - 0.001;
	}
	if (posY - radius <= -120)
	{
		point normal = point(0, 1, 0);
		speed = getReflectVector(speed, normal);
		posY = -120 + radius + 0.001;
	}
	else if (posY + radius >= 120)
	{
		point normal = point(0, -1, 0);
		speed = getReflectVector(speed, normal);
		posY = 120 - radius - 0.001;
	}

	speedX = speed.x;
	speedY = speed.y;

	if (inCircle)
	{
		//big circle
		if (!isInCircleAfter())
		{
			point normal = point(-posX, -posY, 0);
			speed = getReflectVector(speed, normal);
			normal = scale(normalize(normal), 75-radius-0.01);
		}

		//small circle
		for (Bubbles bubble : bubbles)
		{
			if (id == bubble.id || !bubble.inCircle)
				continue; // ignore self and outside circles
			bool collides = doCollide(*this, bubble);
			if (collides && canReflect[bubble.id])
			{
				canReflect[bubble.id] = false;
				point normal = point(posX - bubble.posX, posY - bubble.posY, 0);
				speed = getReflectVector(speed, normal);
				speedX = speed.x;
				speedY = speed.y;
			}
			else if (!collides){
				canReflect[bubble.id] = true;
			}
		}
	}

	speedX = speed.x;
	speedY = speed.y;
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
	glBegin(GL_LINES);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);

		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);

		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);

		glVertex3f(-a, a,2);
		glVertex3f( a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
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
    glColor3f(1,0,0);
	drawCircle(75, 30);

    glColor3f(0,1,0);
	drawSquare(120);

	glColor3f(0.9,0.5,0.2);
	for(int i=0; i< bubbleCount; i++){
		if (!bubbles[i].active)continue;
		glPushMatrix();
		{
			glTranslatef(bubbles[i].posX, bubbles[i].posY, 0);
			drawCircle(bubbles[i].radius, 15);
		}glPopMatrix();
	}



}

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
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


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

	if (paused)return;
	if (currentTime < 50000)
		currentTime++;

	for (int i=0; i<bubbleCount; i++){
		bubbles[i].update();
	}
	for (int i=0; i<bubbleCount; i++){
		bubbles[i].move();
	}



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
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	for (int i=0; i<bubbleCount;i++){
		bubbles[i].id = i;
	}



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
	currentTime = 0;
	srand (time(NULL));
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
