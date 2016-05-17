#include "debug.h"
#include "mymat.h"
#include <iostream>
using namespace std;
ostream &operator<<(ostream &os, const MyVec3 &m) { 
		return os << "(" << m.x << " " << m.y << " " << m.z << ")";
	}
ostream &operator<<(ostream &os, Ray &m) { 
		return os << "origin: " << m.getOri().x << " " << m.getOri().y << " " << m.getOri().z
		<< " dire: " << m.getDir().x << " " << m.getDir().y << " " << m.getDir().z ;
	}
ostream &operator<<(ostream &os, const MyMat &m) {
	return os << "mymat:" << endl << m.mat;
}