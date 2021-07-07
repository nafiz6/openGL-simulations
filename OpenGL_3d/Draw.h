#ifndef DRAW_H
#define DRAW_H

#include <GL/glut.h>
#include "Vector.h"
#include "Object.h"
#include<stdio.h>
#include<stdlib.h>
#include<vector>

#define pi (2*acos(0.0))

extern double cameraHeight;
extern double cameraAngle;
extern int drawgrid;
extern int drawaxes;
extern double angle;

extern Object lBall;
extern Object rBall;
extern Object cylinder;
extern Object lCylinderBall;
extern Object rCylinderBall;
extern std::vector<point> bullets;

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

#endif