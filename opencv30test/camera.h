#ifndef CAMERA_H
#define CAMERA_H

#include "myVec3.h"
#include "ray.h"


class Camera
{
public:
	Camera(const MyVec3& _eye, const MyVec3& _front,const MyVec3& _realUp, double _viewAngle):
	eye(_eye), front(MyVec3(_front).unit()), realUp(MyVec3(_realUp).unit()), viewAngle(_viewAngle){
		right = MyVec3::cross(front,realUp);
		up = MyVec3::cross(right, front);
		viewAngleScale = std::tan(viewAngle * 0.5 * PI / 180) * 2;
	};
	Camera(const MyVec3& eye, const MyVec3& _front, const MyVec3& _right, double _spanr, double _spanu):
	eye(eye), 
	front(MyVec3(_front).unit()), 
	right(MyVec3(_right).unit()),
	spanr(_spanr),
	spanu(_spanu)
	//	add "this->" in front of the same variable !!!
	{
		this->up = MyVec3::cross(this->right, this->front);
		this->spanScaler = this->spanr * PI / 360.0;
		this->spanScaleu = this->spanu * PI / 360.0;
	}


	~Camera(){};

	Ray emit(double sx, double sy);
	void emit(double sx, double sy, Ray& ray); // sx,sy in [0,1]

	void emit(double u, double v, Ray& ray, int);

	Ray emit(MyVec3&);

	MyVec3 eye, front, up, realUp; // 眼睛位置,前方单位向量，上方单位向量
	MyVec3 right;//右方向量
	double viewAngle, viewAngleScale; // 视野的角度fov，视野对于屏上的宽度
	double spanScaler, spanScaleu, spanr, spanu;
};

/*
参考图片见http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html
camera离显示屏的距离为1



*/

#endif