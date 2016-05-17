#ifndef CAMERA_H
#define CAMERA_H

#include "myVec3.h"
#include "ray.h"

class Camera
{
public:
	Camera(const MyVec3& _eye, const MyVec3& _front,const MyVec3& _up, double _viewAngle):
	eye(_eye), front(MyVec3(_front).unit()), up(MyVec3(_up).unit()), viewAngle(_viewAngle){
		right = MyVec3::cross(front,up);
		viewAngleScale = std::tan(viewAngle * 0.5 * PI / 180) * 2;
	};
	~Camera(){};

	Ray emit(double sx, double sy);
	void emit(double sx, double sy, Ray& ray); // sx,sy in [0,1]

	MyVec3 eye, front, up; // 眼睛位置,前方单位向量，上方单位向量
	MyVec3 right;//右方向量
	double viewAngle, viewAngleScale; // 视野的角度fov，视野对于屏上的宽度
};

/*
参考图片见http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html
camera离显示屏的距离为1



*/

#endif