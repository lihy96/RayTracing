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
	~Camera(){};

	Ray emit(double sx, double sy);
	void emit(double sx, double sy, Ray& ray); // sx,sy in [0,1]
	Ray emit(MyVec3&);

	MyVec3 eye, front, up, realUp; // �۾�λ��,ǰ����λ�������Ϸ���λ����
	MyVec3 right;//�ҷ�����
	double viewAngle, viewAngleScale; // ��Ұ�ĽǶ�fov����Ұ�������ϵĿ��
};

/*
�ο�ͼƬ��http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html
camera����ʾ���ľ���Ϊ1



*/

#endif