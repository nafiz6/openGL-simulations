#ifndef VECTOR_H
#define VECTOR_H

#include<math.h>

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



#endif