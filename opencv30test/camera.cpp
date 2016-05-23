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




