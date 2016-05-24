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
	Light(const Color& color):
	color(color){}
	const Color& getColor() const {	return color;	}
	void setColor(const Color& color){	this->color = color;}
	Material* material() {	cout << "bug!"; exit(33);	}
	//int intersect(const Ray& ray, IntersectResult& result) const ;
	//int isIntersectWithBox(const AABB& a_Box) ;
	//AABB getAABB() ;
private:
	Color color;
};

class PointLight : public Light
{
public:
	PointLight(double x, double y, double z, double r, const Color& color):
	Light(color),
	position(MyVec3(x, y, z), r){}
	PointLight(const MyVec3& center, double r, const Color& color):
	Light(color),
	position(center, r){}
	PointLight(const Sphere& position, const Color& color):
	Light(color),
	position(position){}
	
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
	int getType() const override {
		return Config::LIGHT_POINT_TYPE;
	}
	AABB getAABB() override {
		return position.getAABB();
	}

	//Color getColorTexture(MyVec3& a_Pos) {
	//	return BLACK;
	//}

private:
	Sphere position;//��Դ�ı�����һ����
};



class BoxLight : public Light
{
public:
	BoxLight():
	Light(WHITE),
	box(MyVec3(0,0,0), MyVec3(0,0,0)){ initSampleGrid(); }

	BoxLight(const AABB& _box, const Color& color):
	Light(color),
	box(_box){ initSampleGrid(); }
	
	AABB& get() { return box; }

	int intersect(const Ray& ray, IntersectResult& result) override {
		id = ray.getID();//���Ȼ�ȡid
		int re = box.intersect(ray, result);
		if(re)
			result.primi = this;
		return re;
	}


	int isIntersectWithBox(const AABB& a_Box) override {
		return box.isIntersect(a_Box);
	}
	int getType() const override {
		return Config::LIGHT_BOX_LIGHT;
	}

	AABB getAABB() override {
		return box;
	}

	//Color getColorTexture(MyVec3& a_Pos) {
	//	return BLACK;
	//}


	void initSampleGrid(){//��ʼ������Ӱ��������
		int seq[SOFT_SHA_GRID_SIZE+1];
		for(int i = 0;i < SOFT_SHA_GRID_SIZE;i++)
			seq[i] = i;
		random_shuffle(seq, seq+SOFT_SHA_GRID_SIZE);
		for(int i = 0; i < SOFT_SHA_GRID_SIZE; i++){
			sampleGrid[i][0] = (seq[i] / SOFT_SHA_GRID_BIAN) * box.getSize().x / SOFT_SHA_GRID_BIAN + box.getPos().x;
			sampleGrid[i][1] = (seq[i] % SOFT_SHA_GRID_BIAN) * box.getSize().z / SOFT_SHA_GRID_BIAN + box.getPos().z;
		}
	}

	double getSampleGridX(int id){ return sampleGrid[id & SOFT_SHA_GRID_MAX][0]; }//���غ��ӹ�Դ�����������x
	double getSampleGridY(int id){ return sampleGrid[id & SOFT_SHA_GRID_MAX][1]; }//���غ��ӹ�Դ�����������z

private:
	AABB box;//��Դ�ı�����һ������
	double sampleGrid[SOFT_SHA_GRID_SIZE+1][2+1];//���ӹ�Դ����������x,y
};



#endif