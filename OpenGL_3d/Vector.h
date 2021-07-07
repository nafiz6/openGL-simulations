#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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

void print(point a, std::string name){
	std::cout << name << ": " << a.x  << " " << a.y << " " << a.z << " ";
}

#endif
