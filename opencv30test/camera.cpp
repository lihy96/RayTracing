#include "camera.h"
#include "debug.h"
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
	//MyVec3 pPing = eye + distance * dir;	//����һ�������
	//Ray ray;
	//emit((pPing.x)  , (pPing.y) );
	Ray ray;
	ray.setDir((p - eye).unit());
	ray.setOri(eye);
	return ray;

}





