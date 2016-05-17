#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "myVec3.h"
#include <cmath>

class Material
{
public:
	Material(const Color& _color, double _diffuse, double _specular, double _shininess = 0, double _reflectiveness = 0, double _refraction = 0, double _refraIndex = 1.0):
	color(_color), //颜色
	diffuse(_diffuse), specular(_specular),	//漫反射和镜面反射
	shininess(_shininess),	//发光系数
	reflectiveness(_reflectiveness), refraction(_refraction), //反射和折射
	refraIndex(_refraIndex){};//折射系数

	const double getReflectiveness() const{	return reflectiveness;	}
	virtual const Color sample(const MyVec3* ray, const MyVec3* L, const MyVec3* normal, const MyVec3* position, const Color* light)const = 0;	
	virtual const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const = 0;	
	
	const double getDiffuse() const { return diffuse;	}
	const double getSpecular() const {	return specular;	}	
	const double getRefraction() const {	return refraction;	}
	const double getRefraIndex() const {	return refraIndex;	}
	const Color& getColor() const {	return color;	}

	Color color;
	double diffuse, specular, shininess, reflectiveness, refraction, refraIndex;
};

class Phong:public Material
{
public:
	Phong(const Color& color, double diffuse, double specular, double shininess = 0, double reflectiveness = 0, double refraction = 0, double refraIndex = 1.0):
	Material(color, diffuse, specular, shininess, reflectiveness, refraction, refraIndex){}
	
	const Color sample(const MyVec3* ray, const MyVec3* L, const MyVec3* normal, const MyVec3* position, const Color* light)const
	{
		Color re;
		sample(*ray, *L, *normal, *light, re);
		return re;	
	}

	const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const
	{
		Color re;
		sample(ray, L, normal, light, re);
		return re;	
	}

	const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const Color& light)const
	{
		Color re;
		sample(ray, L, normal, light, re);
		return re;
	}

	void sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const Color& light, Color& re)const
	{
		//	l: the opposite vector of incident light
		//	r: reflected light
		//	v: the vector towards the viewer(the opposite vector of camera ray)
		//	h: the angle-bisector of l and v
		//	n: normal vector

		//	r = (-l) - 2*dot(-l, n)n
		//	f(l->v) = diffuseColor + specularColor * dot(r, v)^s / dot(n, l)
		//=>f(l->v) = diffuseColor + specularColor * dot(n, h)^s / dot(n, l)
		//=>f(l->v) = diffuseColor + specularColor * dot(n, h)^s    [used]
		double NdotL = std::max(dot(normal,L), 0.0);
		MyVec3 H = (L - ray).unit();
		double NdotH = std::max(dot(normal, H), 0.0);
		re = NdotL * diffuse * color * light + std::pow(NdotH, shininess) * specular * light;       
	}
};

class Chess:public Material
{
public:
	Chess(const Color& color, const Color& color1, double diffuse, double specular, double shininess = 0, double reflectiveness = 0, double refraction = 0, double refraIndex = 1.0, double scale = 0.1):
	Material(color, diffuse, specular, shininess, reflectiveness, refraction, refraIndex), color1(color1), scale(scale){}
	
	const Color sample(const MyVec3* ray, const MyVec3* L, const MyVec3* normal, const MyVec3* position, const Color* light)const
	{
		int oe = abs(floor(position->x * scale) + floor(position->z * scale));
		if (oe & 1)
			return color * (*light);
		else
			return color1 * (*light);
	}

	const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const
	{
		int oe = abs(floor(position.x * scale) + floor(position.z * scale));
		if (oe & 1)
			return color * light;
		else
			return color1 * light;
	}

	const Color sample(const MyVec3& position)const
	{
		int oe = abs(floor(position.x * scale) + floor(position.z * scale));
		if (oe & 1)
			return color;
		else
			return color1;	
	}

	void sample(const MyVec3& position, Color& re)const
	{
		int oe = abs(floor(position.x * scale) + floor(position.z * scale));
		if (oe & 1)
			re = color;
		else
			re = color1;
	}
private:
	Color color1;
	double scale;
};
#endif

