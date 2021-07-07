#ifndef BUBBLES_H
#define BUBBLES_H

#include <time.h>       /* time */
#include<math.h>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include"Vector.h"

#define pi (2*acos(0.0))
#define bubbleCount 5

extern long long currentTime;

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
	return  (magnitude(point(a.posX - b.posX, a.posY -b.posY, 0)) <= a.radius + b.radius) ;
}

extern std::vector<Bubbles >bubbles;

void Bubbles::checkForReflection(){
	point speed = point(speedX, speedY, 0);
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


#endif