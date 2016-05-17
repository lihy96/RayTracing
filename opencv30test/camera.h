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

	MyVec3 eye, front, up; // �۾�λ��,ǰ����λ�������Ϸ���λ����
	MyVec3 right;//�ҷ�����
	double viewAngle, viewAngleScale; // ��Ұ�ĽǶ�fov����Ұ�������ϵĿ��
};

/*
�ο�ͼƬ��http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html
camera����ʾ���ľ���Ϊ1



*/

#endif