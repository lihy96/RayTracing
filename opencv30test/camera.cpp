#include "camera.h"
#include "debug.h"
#include <iostream>
using namespace std;

void Camera::emit(double sx, double sy, Ray& ray){
	// [0,1] -> [-1,-1] : (2 * sxy - 1) but 2 is in viewScale
	MyVec3 r = ((sx - 0.5) * viewAngleScale) * right;
	MyVec3 u = ((sy - 0.5) * viewAngleScale) * up;
	ray.setOri(eye);
	//cout << ray << " " << eye << endl;
	ray.setDir(front + r + u);

}

Ray Camera::emit(double sx, double sy){
	Ray ray;
	emit(sx,sy,ray);
	return ray;
}

Ray Camera::emit(MyVec3& p){
	//MyVec3 dir = (p - eye).unit();
	//double cosA = MyVec3::dot(dir, MyVec3(0, 0, 1));
	//double distance = 1.0 / cosA;
	//MyVec3 pPing = eye + distance * dir;	//屏上一点的坐标
	//Ray ray;
	//emit((pPing.x)  , (pPing.y) );
	Ray ray;
	ray.setDir((p - eye).unit());
	ray.setOri(eye);
	return ray;

}

void Camera::emit(double u, double v, Ray& ray, int)
{
	MyVec3 rightOffset = (u - 0.5) * spanScaler * right;
	MyVec3 upOffset = (v - 0.5) * spanScaleu * up;
	MyVec3 tmp = front + rightOffset + upOffset;
	ray.direction = MyVec3::normalize(tmp);
	ray.origin = eye;
}





