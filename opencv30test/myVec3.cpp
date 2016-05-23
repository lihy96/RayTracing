#include "myVec3.h"



MyVec3 operator - ( const MyVec3& A ) {
	return MyVec3( -A.x , -A.y , -A.z );
}

MyVec3 operator + ( const MyVec3& A , const MyVec3& B ) {
	return MyVec3( A.x + B.x , A.y + B.y , A.z + B.z );
}

MyVec3 operator - ( const MyVec3& A , const MyVec3& B ) {
	return MyVec3( A.x - B.x , A.y - B.y , A.z - B.z );
}

MyVec3 operator * ( const MyVec3& A , const double& k ) {
	return MyVec3( A.x * k , A.y * k , A.z * k );
}

MyVec3 operator * ( const double& k ,const MyVec3& A ) {
	return MyVec3( A.x * k , A.y * k , A.z * k );
}

MyVec3 operator / ( const MyVec3& A , const double& k ) {
	return MyVec3( A.x / k , A.y / k , A.z / k );
}

MyVec3 operator / (const double& k , const MyVec3& A ) {
	return MyVec3( A.x / k , A.y / k , A.z / k );
}


MyVec3 operator * ( const MyVec3& A , const MyVec3& B ) {
	return MyVec3( A.x * B.x , A.y * B.y , A.z * B.z );
}

MyVec3& operator += ( MyVec3& A , const MyVec3& B ) {
	A = A + B;
	return A;
}

MyVec3& operator -= ( MyVec3& A , const MyVec3& B ) {
	A = A - B;
	return A;
}

MyVec3& operator *= ( MyVec3& A , const double& k ) {
	A = A * k;
	return A;
}

MyVec3& operator /= ( MyVec3& A , const double& k ) {
	A = A / k;
	return A;
}

MyVec3& operator *= ( MyVec3& A , const MyVec3& B ) {
	A = A * B;
	return A;
}

//double MyVec3::dot( MyVec3& term ) {
//	return x * term.x + y * term.y + z * term.z;
//}
//
//MyVec3 MyVec3::cross(MyVec3& term) {
//	return MyVec3(y * term.z - z * term.y , z * term.x - x * term.z , x * term.y - y * term.x );
//}

double MyVec3::module_2() {
	return (x * x + y * y + z * z);
}

double MyVec3::module() {
	return sqrt( x * x + y * y + z * z );
}

double MyVec3::distance_2( MyVec3& term ) {
	return ( term - *this ).module_2();
}

double MyVec3::distance( MyVec3& term ) {
	return ( term - *this ).module();
}

MyVec3 MyVec3::unit(){
	double mod = module();
	if (mod < EPS) 
		return MyVec3(0, 0, 1);
	return *this / mod;
}

bool MyVec3::isZeroVector(){
	return (fabs( x ) < EPS && fabs( y ) < EPS && fabs( z ) < EPS);
}



