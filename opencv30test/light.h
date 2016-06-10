#ifndef LIGHT_H
#define LIGHT_H

#include "scene.h"
#include "sphere.h"
#include "myVec3.h"
#include "color.h"
#include "primitive.h"
#include "config.h"

class Light : public Primitive
{
public:
	Light():
	Primitive(Config::LIGHT){}
	Light(const Color& color):
	Primitive(Config::LIGHT),color(color){}

	const Color& getColor() const {	return color;	}
	void setColor(const Color& color){	this->color = color;}
	//Material* material() {	cout << "bug!"; exit(33);	}
	//int intersect(const Ray& ray, IntersectResult& result) const ;
	//int isIntersectWithBox(const AABB& a_Box) ;
	//AABB getAABB() ;
	//virtual Material* material();
private:
	Color color;
};

class PointLight : public Light
{
public:
	PointLight(double x, double y, double z, double r, const Color& color):
	Light(color),
	position(MyVec3(x, y, z), r){type = Config::LIGHT_POINT_TYPE;}

	PointLight(const MyVec3& center, double r, const Color& color):
	Light(color),
	position(center, r){type = Config::LIGHT_POINT_TYPE;}

	PointLight(const Sphere& position, const Color& color):
	Light(color),
	position(position){type = Config::LIGHT_POINT_TYPE;}

	PointLight(const Sphere& position):
	position(position){type = Config::LIGHT_POINT_TYPE;}

	Sphere& get() { return position; }

	int intersect(const Ray& ray, IntersectResult& result) override {
		int flag =  position.intersect(ray, result);
		if(flag){
			result.primi = this;
		}	
		return flag;
	}
	int isIntersectWithBox(const AABB& a_Box) override {
		return position.isIntersectWithBox(a_Box);
	}
	//int getType() const override {
	//	return Config::LIGHT_POINT_TYPE;
	//}
	AABB getAABB() override {
		return position.getAABB();
	}

	//Color getColorTexture(MyVec3& a_Pos) {
	//	return BLACK;
	//}

	Material* material() { return position.material();}

private:
	Sphere position;//光源的本质是一个球
};



class BoxLight : public Light
{
public:
	BoxLight():
	Light(WHITE),
	box(MyVec3(0,0,0), MyVec3(0,0,0)),
	sampleGrid(0){ type = Config::LIGHT_BOX_LIGHT; initSampleGrid(); }

	BoxLight(const AABB& _box, const Color& color):
	Light(color),
	box(_box),
	sampleGrid(0){ type = Config::LIGHT_BOX_LIGHT; initSampleGrid(); }
	
	AABB& get() { return box; }

	int intersect(const Ray& ray, IntersectResult& result) override {
		id = ray.getID();//首先获取id
		int re = box.intersect(ray, result);
		if(re)
			result.primi = this;
		return re;
	}


	int isIntersectWithBox(const AABB& a_Box) override {
		return box.isIntersect(a_Box);
	}
	//int getType() const override {
	//	return Config::LIGHT_BOX_LIGHT;
	//}

	AABB getAABB() override {
		return box;
	}

	//Color getColorTexture(MyVec3& a_Pos) {
	//	return BLACK;
	//}


	void initSampleGrid();
	double getSampleGridX(int a){ return sampleGrid[(a & SAMPLEGRIDSIZEMOD) << 1]; }//返回盒子光源的网格的坐标x
	double getSampleGridY(int a){ return sampleGrid[((a & SAMPLEGRIDSIZEMOD) << 1) ^ 1]; }//返回盒子光源的网格的坐标z
	Material* material() {	cout << "aa" << endl; exit(13);return 0;	}
	static const int SAMPLEGRIDSIZE;
	static const int SAMPLEGRIDSIZEMOD;
	static const int SAMPLEGRIDSIZESQRT;
	static const double SAMPLEGRIDSIZESQRTDOWN;
private:
	AABB box;//光源的本质是一个盒子
	double* sampleGrid;//盒子光源的网格坐标x,y
};



#endif