#ifndef MYVEC3_H
#define MYVEC3_H

#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;

const double EPS = 1e-8;
const double PI = 3.1415926535897932384626;
const double PI_DOWN = 1 / PI;

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


	double& operator[](int i){
		if(i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
		else 
			exit(98);
	}

	static const double dot(const MyVec3& A, const MyVec3& B){
		return A.x * B.x + A.y * B.y + A.z * B.z;
	}

	static const MyVec3 cross(const MyVec3& A, const MyVec3& B){
		return MyVec3(A.y * B.z - A.z * B.y , A.z * B.x - A.x * B.z , A.x * B.y - A.y * B.x);
	}

	static const MyVec3& normalize(MyVec3& A){
		double length = A.module();
		if(fabs(length) > EPS )
			A /= length;
		else{
			cout << "hehe" << endl;
			cout << A.x << " " << A.y << " " << A.z << endl;
			exit(99);
			A = MyVec3(0,0,1);
		}
		return A;
	}

	void Input( std::stringstream& fin ) {
		fin >> x >> y >> z;
	}

	MyVec3 Rotate( MyVec3 axis , double theta ){
		MyVec3 ret;
		double cost = cos( theta );
		double sint = sin( theta );
		ret.x += x * ( axis.x * axis.x + ( 1 - axis.x * axis.x ) * cost );
		ret.x += y * ( axis.x * axis.y * ( 1 - cost ) - axis.z * sint );
		ret.x += z * ( axis.x * axis.z * ( 1 - cost ) + axis.y * sint );
		ret.y += x * ( axis.y * axis.x * ( 1 - cost ) + axis.z * sint );
		ret.y += y * ( axis.y * axis.y + ( 1 - axis.y * axis.y ) * cost );
		ret.y += z * ( axis.y * axis.z * ( 1 - cost ) - axis.x * sint );
		ret.z += x * ( axis.z * axis.x * ( 1 - cost ) - axis.y * sint );
		ret.z += y * ( axis.z * axis.y * ( 1 - cost ) + axis.x * sint );
		ret.z += z * ( axis.z * axis.z + ( 1 - axis.z * axis.z ) * cost );
		return ret;
	}

};
#endif
