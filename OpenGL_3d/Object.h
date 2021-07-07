#ifndef OBJECT_H
#define OBJECT_H

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

#endif