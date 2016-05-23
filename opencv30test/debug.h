#ifndef DEBUG_H
#define DEBUG_H
#include "all.h"
#include "intersectResult.h"
#include "myVec3.h"
#include "color.h"
#include <assert.h>
#include <iostream>
using namespace std;
class Debug
{
public:
	Debug(void);
	~Debug(void);
	friend ostream &operator<<(ostream &os, const MyVec3 &m);
	friend ostream &operator<<(ostream &os, Ray &m);
	friend ostream &operator<<(ostream &os, const MyMat &m);
	friend ostream &operator<<(ostream &os, Color &m);
	friend ostream &operator<<(ostream &os, const IntersectResult &m);

};
#endif

