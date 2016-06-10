#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "myVec3.h"
#include "texture.h"
#include <cmath>
#include <assert.h>
class Material
{
public:
	Material(const Color& _color, char* filename = 0):
	color(_color), //��ɫ	
	m_Texture(0),//������ͼ
	m_UScale( 1.0 ),
	m_VScale( 1.0 )
	{
		if (filename != 0)
			SetTexture(new Texture(filename));	
	}

	
	virtual const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const = 0;

	//Ϊ��ʵ��������ͼר�żӵĺ���
	virtual const Color sample(const MyVec3& ray, const MyVec3& L, const  MyVec3& normal, const MyVec3& position, const Color& light, const Color& primiColor)const = 0;
	
	virtual const double getDiffuse() const = 0;
	virtual const double getSpecular() const = 0;
	virtual const double getShininess() const = 0;
	virtual const double getReflectiveness() const = 0;
	virtual const double getRefraction() const = 0;
	virtual const double getRefraIndex() const = 0;
	virtual const double getGlossyReflect() const = 0;

	const Color& getColor() const {	return color;	}

		//	BRDF
	virtual const Color& getEmission() const = 0;
	virtual const int getBRDFType() const = 0;

	//������ͼ�ĺ���
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
	
	//������ͼ�Ĳ���
	Texture* m_Texture;;
	double m_UScale, m_VScale, m_RUScale, m_RVScale;
};

class Phong:public Material
{
public:
	Phong(const Color& color, double diffuse, double specular, double shininess = 20, double reflectiveness = 0, double refraction = 0, double refraIndex = 1.0, double glossyReflect = 0.0, char* filename = 0):
	Material(color, filename),diffuse(diffuse), specular(specular), shininess(shininess), 
	reflectiveness(reflectiveness), refraction(refraction), refraIndex(refraIndex), glossyReflect(glossyReflect){}

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
		//	l: �����ķ�����
		//	r: �����Ĺ��߷���
		//	v: ����camera�����ķ��򣬼�camera����ray���ߵķ�����
		//	h: �����ķ�������ӽ�v�ķ���ray�ķ��򣩵ļн�
		//	n: ������

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
	virtual const double getDiffuse() const {	return diffuse;	}
	virtual const double getSpecular() const {	return specular;	}
	virtual const double getShininess() const {	return shininess;	}
	virtual const double getReflectiveness() const {	return reflectiveness;	}
	virtual const double getRefraction() const {	return refraction;	}	
	virtual const double getRefraIndex() const {	return refraIndex;	}
	virtual const double getGlossyReflect() const {	return glossyReflect;	}

	virtual const Color& getEmission() const {	exit(124); return Color(0,0,0);}
	virtual const int getBRDFType() const {	exit(125);return 0;}
	double diffuse, specular, shininess, reflectiveness, refraction, refraIndex, glossyReflect;	
};

class Chess:public Material
{
public:
	Chess(const Color& color, const Color& color1, double diffuse, double specular, double shininess = 20, double reflectiveness = 0, double refraction = 0, double refraIndex = 1.0, double glossyReflect = 0.0, double scale = 0.1, char* filename = 0):
	Material(color, filename), color1(color1), scale(scale),
	diffuse(diffuse), specular(specular), shininess(shininess), 
	reflectiveness(reflectiveness), refraction(refraction), refraIndex(refraIndex), glossyReflect(glossyReflect){}
	
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

	virtual const double getDiffuse() const {	return diffuse;	}
	virtual const double getSpecular() const {	return specular;	}
	virtual const double getShininess() const {	return shininess;	}
	virtual const double getReflectiveness() const {	return reflectiveness;	}
	virtual const double getRefraction() const {	return refraction;	}	
	virtual const double getRefraIndex() const {	return refraIndex;	}
	virtual const double getGlossyReflect() const {	return glossyReflect;	}
	virtual const Color& getEmission() const {	assert(false);	return Color(0,0,0);}
	virtual const int getBRDFType() const {	assert(false);	return 0;}

private:
	Color color1;
	double scale;
	double diffuse, specular, shininess, reflectiveness, refraction, refraIndex, glossyReflect;		

};


class BRDF : public Material
{
public:
	BRDF(const Color& color, const Color& emission, int BRDFType, double refraIndex = 1.0, char* filename = 0):
	Material(color, filename), emission(emission), BRDFType(BRDFType), refraIndex(refraIndex){}

	virtual const Color& getEmission() const {	return emission;	}
	virtual const int getBRDFType() const {	return BRDFType;	}


	//virtual const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const  {assert(false);return Color(0,0,0);}

	//virtual const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const Primitive* pri, const MyVec3& position, const Color& light)const {	exit(1);	return Color(0,0,0);	}
	//virtual const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const Color& texc, const MyVec3& position, const Color& light)const {	exit(2);	return Color(0,0,0);}	

	virtual const Color sample(const MyVec3& ray, const MyVec3& L, const MyVec3& normal, const MyVec3& position, const Color& light)const {exit(1);return Color(0,0,0);};

	//Ϊ��ʵ��������ͼר�żӵĺ���
	virtual const Color sample(const MyVec3& ray, const MyVec3& L, const  MyVec3& normal, const MyVec3& position, const Color& light, const Color& primiColor)const{exit(123);return Color(0,0,0);}

	virtual const double getDiffuse() const {	assert(false);	return 0;}
	virtual const double getSpecular() const {	assert(false);return 0;	}
	virtual const double getShininess() const {	assert(false);	return 0;}
	virtual const double getReflectiveness() const {	assert(false);	return 0;}
	virtual const double getRefraction() const {	assert(false);return 0;	}
	virtual const double getRefraIndex() const {	return refraIndex;	return 0;}
	virtual const double getGlossyReflect() const {	assert(false);	return 0;}
private:
	Color emission;
	int BRDFType;
	double refraIndex;
};
#endif

