#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "myVec3.h"
#include "texture.h"
#include <cmath>

class Material
{
public:
	Material(const Color& _color, double _diffuse, double _specular, double _shininess = 20, double _reflectiveness = 0, double _refraction = 0, double _refraIndex = 1.0, double _difflection = 0):
	color(_color), //颜色
	diffuse(_diffuse), specular(_specular),	//漫反射和镜面反射
	shininess(_shininess),	//发光系数
	reflectiveness(_reflectiveness), refraction(_refraction), //反射和折射
	refraIndex(_refraIndex),//折射系数
	difflection(_difflection),//软阴影
	
	m_Texture(0),//纹理贴图
	m_UScale( 1.0 ),
	m_VScale( 1.0 )
	{}

	const double getReflectiveness() const{	return reflectiveness;	}
/*	virtual const Color sample(const MyVec3* ray, const MyVec3* L, const MyVec3* normal, const MyVec3* position, const Color* light)const = 0;*/	
	virtual const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const = 0;

	//为了实现纹理贴图专门加的函数
	virtual const Color sample(const MyVec3& ray, const MyVec3& L, const  MyVec3& normal, const MyVec3& position, const Color& light, const Color& primiColor)const = 0;
	
	const double getDiffuse() const { return diffuse; }
	const double getSpecular() const {	return specular; }	
	const double getRefraction() const { return refraction; }
	const double getRefraIndex() const { return refraIndex; }
	const double getDifflection() const { return difflection; }

	const Color& getColor() const {	return color;	}

//纹理贴图的函数
	void SetTexture( Texture* a_Texture ) { m_Texture = a_Texture; }
	Texture* GetTexture() { return m_Texture; }
	void Material::SetUVScale( double a_UScale, double a_VScale ){ 
		m_UScale = a_UScale; 
		m_VScale = a_VScale; 
		m_RUScale = 1.0f / a_UScale;
		m_RVScale = 1.0f / a_VScale;
	}
	double GetUScale() { return m_UScale; }
	double GetVScale() { return m_VScale; }
	double GetUScaleReci() { return m_RUScale; }
	double GetVScaleReci() { return m_RVScale; }
//end


//----------------------------------------------------------------------------
	Color color;
	double diffuse, specular, shininess, reflectiveness, refraction, refraIndex;
	double difflection;  //漫反射反射，用于软阴影部分
	
	//纹理贴图的部分
	Texture* m_Texture;;
	double m_UScale, m_VScale, m_RUScale, m_RVScale;
};

class Phong:public Material
{
public:
	Phong(const Color& color, double diffuse, double specular, double shininess = 20, double reflectiveness = 0, double refraction = 0, double refraIndex = 1.0, double difflection = 0):
	Material(color, diffuse, specular, shininess, reflectiveness, refraction, refraIndex, difflection){}
	
	const Color sample(const MyVec3* ray, const MyVec3* L, const MyVec3* normal, const MyVec3* position, const Color* light)const
	{
		Color re;
		sample(*ray, *L, *normal, *light, re);
		return re;	
	}

	const Color sample(const MyVec3& ray, const MyVec3& L, const  MyVec3& normal, const MyVec3& position, const Color& light,const Color& primiColor)const{	
		double NdotL = std::max(MyVec3::dot(normal,L), 0.0);

		MyVec3 R = L - 2 * MyVec3::dot(L, normal) * normal;
		double VdotR = std::max(MyVec3::dot(ray, R), 0.0);

		return (NdotL * diffuse * primiColor * light + std::pow(VdotR, shininess) * specular * light); 
	}
	const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const
	{
		Color re;
		sample(ray, L, normal, light, re);
		return re;	
	}

	const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const Color& light) const 
	{
		Color re;
		sample(ray, L, normal, light, re);
		return re;
	}


	void sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const Color& light, Color& re)const
	{
		//	l: 入射光的反方向
		//	r: 反射后的光线方向
		//	v: 向着camera发出的方向，即camera发出ray光线的反方向
		//	h: 入射光的反方向和视角v的方向（ray的方向）的夹角
		//	n: 法向量

		//	r = (-l) - 2*dot(-l, n)n
		//	f(l->v) = diffuseColor + specularColor * dot(r, v)^s / dot(n, l)
		//=>f(l->v) = diffuseColor + specularColor * dot(n, h)^s / dot(n, l)
		//=>f(l->v) = diffuseColor + specularColor * dot(n, h)^s    [used]
		double NdotL = std::max(MyVec3::dot(normal,L), 0.0);
		//MyVec3 H = (L - ray).unit();
		//double NdotH = std::max(MyVec3::dot(normal, H), 0.0);
		//re = NdotL * diffuse * color * light + std::pow(NdotH, shininess) * specular * light;  

		MyVec3 R = L - 2 * MyVec3::dot(L, normal) * normal;
		double VdotR = std::max(MyVec3::dot(ray, R), 0.0);

		re = NdotL * diffuse * color * light + std::pow(VdotR, shininess) * specular * light; 

	}	
};

class Chess:public Material
{
public:
	Chess(const Color& color, const Color& color1, double diffuse, double specular, double shininess = 20, double reflectiveness = 0, double refraction = 0, double refraIndex = 1.0,double difflection = 0, double scale = 0.1):
	Material(color, diffuse, specular, shininess, reflectiveness, refraction, refraIndex,difflection), color1(color1), scale(scale){}
	
	const Color sample(const MyVec3& ray, const MyVec3& L, const  MyVec3& normal, const MyVec3& position, const Color& light,const Color& primiColor)const{	
		int oe = abs(floor(position.x * scale) + floor(position.z * scale));
		if (oe & 1)
			return color * light;
		else
			return color1 * light;
	}


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

