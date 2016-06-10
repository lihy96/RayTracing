#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "primitive.h"
#include "kdTree.h"

class TriangleTree;

class Polyhedron : public Primitive {
	MyVec3 O, size, angles; //ÖÐµã³ß´ç½Ç¶È
	MyVec3* vertexN;
	std::pair<double, double>* pixel;
	std::string meshFile;
	TriangleTree* tree;

public:
	Polyhedron();
	~Polyhedron();
	
	MyVec3 GetO() { return O; }
	MyVec3 GetSize() { return size; }
	MyVec3 GetAngles() { return angles; }
	void SetVertexN(MyVec3* _vertexN) { vertexN = _vertexN; }
	MyVec3& GetVertexN(int i) { return vertexN[i]; }
	TriangleTree* GetTree() { return tree; }
	void SetPixel(std::pair<double, double>* _pixel) { pixel = _pixel; }
	std::pair<double, double>& GetPixel(int i) { return pixel[i]; }
	void Input(std::string, std::stringstream&);
	void PreTreatment();
	int intersect(Ray ray,IntersectResult& result);
};

#endif