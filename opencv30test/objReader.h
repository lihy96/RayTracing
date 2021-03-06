#ifndef OBJREADER_H
#define OBJREADER_H

#include "triangle.h"
#include <string>
#include <map>

class Polyhedron;
class ObjReader {
	Polyhedron* polyhedron;
	int vSize, vtSize, vnSize, fSize, matSize;
	MyVec3* v;
	std::pair<double, double>* vt;
	MyVec3* vn;
	Triangle** tris;
	Material** mat;
	std::map<std::string, int> matMap;
	
	void ReadMtlSize(std::string file);
	void ReadObjSize(std::string file);
	void ReadMtl(std::string file);
	void CalnVn();

public:
	ObjReader();
	~ObjReader() {}
	void SetPolyhedron(Polyhedron* _polyhedron) { polyhedron = _polyhedron; }
	void ReadObj(std::string file);
};

#endif