#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"

#define pi (2*acos(0.0))


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

#endif