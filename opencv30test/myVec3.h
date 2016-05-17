#ifndef MYVEC3_H
#define MYVEC3_H

#include <cmath>
#include <iostream>
using namespace std;

const double EPS = 1e-8;
const double PI = 3.1415926535897932384626;

class MyVec3
{
	
public:
	MyVec3(double X = 0,double Y = 0,double Z = 0) : x(X) ,y(Y) ,z(Z){};
	~MyVec3(){};

	double x,y,z;
	
	friend MyVec3 operator - ( const MyVec3& );//取反向
	friend MyVec3 operator + ( const MyVec3& , const MyVec3& );//向量加
	friend MyVec3 operator - ( const MyVec3& , const MyVec3& );//向量减
	friend MyVec3 operator * ( const MyVec3& , const double& );//向量乘系数
	friend MyVec3 operator * ( const double& , const MyVec3& );//向量乘系数
	friend MyVec3 operator / ( const MyVec3& , const double& );//向量除系数
	friend MyVec3 operator / ( const double& , const MyVec3& );//向量除系数
	friend MyVec3 operator * ( const MyVec3& , const MyVec3& );//向量乘向量

	friend MyVec3& operator += ( MyVec3& , const MyVec3& );
	friend MyVec3& operator -= ( MyVec3& , const MyVec3& );
	friend MyVec3& operator *= ( MyVec3& , const double& );
	friend MyVec3& operator /= ( MyVec3& , const double& );
	friend MyVec3& operator *= ( MyVec3& , const MyVec3& );

	double module_2();//模的平方
	double module();//模
	double distance_2( MyVec3& );//两点距离的平方
	double distance( MyVec3& );//两点的距离
	MyVec3 unit();
	bool isZeroVector();

	
};

const double dot(const MyVec3& A, const MyVec3& B){
	return A.x * B.x + A.y * B.y + A.z * B.z;
}

const MyVec3 cross(const MyVec3& A, const MyVec3& B){
	return MyVec3(A.y * B.z - A.z * B.y , A.z * B.x - A.x * B.z , A.x * B.y - A.y * B.x);
}

const MyVec3& normalize(MyVec3& A){
	double length = A.module();
	if(fabs(length) > EPS)
		A /= length;
	else{
		exit(99);
		A = MyVec3(0,0,1);
	}
	return A;
}


#endif
