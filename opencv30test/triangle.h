#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "primitive.h"
#include "myVec3.h"
#include "material.h"
#include "intersectResult.h"
#include "config.h"

class Triangle : public Primitive
{
public:
	MyVec3 normalVec;
	MyVec3 vertex[3];
	Material* ma;
	Triangle (): Primitive(Config::TRI_TYPE){};
	Triangle (const MyVec3& v0, const MyVec3& v1, const MyVec3& v2, const MyVec3& _fxl, Material* _ma) :
	Primitive(Config::TRI_TYPE),normalVec(_fxl), ma(_ma){vertex[0]=v0; vertex[1]=v1; vertex[2]=v2;	};
	~Triangle () {};

	int intersect(const Ray& ray, IntersectResult& result) override{
		id = ray.getID();
		MyVec3 e1 = vertex[1] - vertex[0];
		MyVec3 e2 = vertex[2] - vertex[0];
		MyVec3 q = MyVec3::cross(ray.getDir(), e2);
		double a = MyVec3::dot(e1, q);
		if ((a > -1e-6) && (a < 1e-6))
			return 0;
		double f = 1.0 / a;
		MyVec3 s = ray.getOri() - vertex[0];
		double u = f * MyVec3::dot(s, q);
		if (u < 0.0)
			return 0;
		MyVec3 r = MyVec3::cross(s, e1);
		double v = f * MyVec3::dot(ray.getDir(), r);
		if ((v < 0.0) || (u + v > 1.0))
			return 0;
		double t = f * MyVec3::dot(e2, r);
		if (t < 0)
			return 0;
		if (t < result.distance - 1e-5){
			result.distance = t;
			result.intersectPoint = ray.getOri() + t * ray.getDir();
			result.primi = this;
			result.normalVec = normalVec;
			result.ma = ma;
		}
		else if ((t - result.distance < 1e-5) && (t - result.distance > -1e-5)){
			result.normalVec = result.normalVec + normalVec;
			MyVec3::normalize(result.normalVec);
		}
		if(MyVec3::dot(ray.direction, result.normalVec) < 0){
			result.isInObj = false;
			return 1;
		}
		else{
			result.isInObj = true;
			return -1;
		}
	}

	//-----------------------------------------
	#define axis_X 0
	#define axis_Y 1
	#define axis_Z 2

	#define CROSS(dest,v1,v2) \
		dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
		dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
		dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

	#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

	#define SUB(dest,v1,v2) \
		dest[0]=v1[0]-v2[0]; \
		dest[1]=v1[1]-v2[1]; \
		dest[2]=v1[2]-v2[2];

	#define FINDMINMAX(x0,x1,x2,min,max) \
		min = max = x0; \
		if(x1<min) min=x1;\
		if(x1>max) max=x1;\
		if(x2<min) min=x2;\
		if(x2>max) max=x2;

	int planeBoxOverlap(double normal[3],double d, double maxbox[3]) const{
		int q;
		double vmin[3],vmax[3];
		for(q=axis_X;q<=axis_Z;q++){
			if(normal[q]>0.0f){
				vmin[q]=-maxbox[q];
				vmax[q]=maxbox[q];
			}
			else{
				vmin[q]=maxbox[q];
				vmax[q]=-maxbox[q];
			}
		}
		if(DOT(normal,vmin)+d>0.0f) return 0;
		if(DOT(normal,vmax)+d>=0.0f) return 1;
		return 0;
	}


	/*======================== X-tests ========================*/
	#define AXISTEST_X01(a, b, fa, fb) \
		p0 = a*v0[axis_Y] - b*v0[axis_Z];\
		p2 = a*v2[axis_Y] - b*v2[axis_Z];\
		if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
		rad = fa * boxhalfsize[axis_Y] + fb * boxhalfsize[axis_Z]; \
		if(min>rad || max<-rad) return 0;

	#define AXISTEST_X2(a, b, fa, fb)\
		p0 = a*v0[axis_Y] - b*v0[axis_Z];\
		p1 = a*v1[axis_Y] - b*v1[axis_Z];\
		if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
		rad = fa * boxhalfsize[axis_Y] + fb * boxhalfsize[axis_Z]; \
		if(min>rad || max<-rad) return 0;

	/*======================== Y-tests ========================*/
	#define AXISTEST_Y02(a, b, fa, fb) \
		p0 = -a*v0[axis_X] + b*v0[axis_Z]; \
		p2 = -a*v2[axis_X] + b*v2[axis_Z]; \
		if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
		rad = fa * boxhalfsize[axis_X] + fb * boxhalfsize[axis_Z]; \
		if(min>rad || max<-rad) return 0;

	#define AXISTEST_Y1(a, b, fa, fb)\
		p0 = -a*v0[axis_X] + b*v0[axis_Z]; \
		p1 = -a*v1[axis_X] + b*v1[axis_Z]; \
		if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
		rad = fa * boxhalfsize[axis_X] + fb * boxhalfsize[axis_Z]; \
		if(min>rad || max<-rad) return 0;

	/*======================== Z-tests ========================*/

	#define AXISTEST_Z12(a, b, fa, fb) \
		p1 = a*v1[axis_X] - b*v1[axis_Y];\
		p2 = a*v2[axis_X] - b*v2[axis_Y];\
		if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
		rad = fa * boxhalfsize[axis_X] + fb * boxhalfsize[axis_Y]; \
		if(min>rad || max<-rad) return 0;

	
	#define AXISTEST_Z0(a, b, fa, fb)\
		p0 = a*v0[axis_X] - b*v0[axis_Y];\
		p1 = a*v1[axis_X] - b*v1[axis_Y];\
		if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
		rad = fa * boxhalfsize[axis_X] + fb * boxhalfsize[axis_Y]; \
		if(min>rad || max<-rad) return 0;

	
	int triBoxOverlap(double boxcenter[3],double boxhalfsize[3],double triverts[3][3]){
		/*use separating axis theorem to test overlap between triangle and box */
		/*need to test for overlap in these directions: */
		/*1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
		/* we do not even need to test these) */
		/*2) normal of the triangle */
		/*3) crossproduct(edge from tri, {x,y,z}-directin) */
		/* this gives 3x3=9 more tests */
		double v0[3],v1[3],v2[3];
		double min,max,d,p0,p1,p2,rad,fex,fey,fez;
		double normal[3],e0[3],e1[3],e2[3];

		 /* This is the fastest branch on Sun */
		 /* move everything so that the boxcenter is in (0,0,0) */
		 SUB(v0,triverts[0],boxcenter);
		 SUB(v1,triverts[1],boxcenter);
		 SUB(v2,triverts[2],boxcenter);

		 /* compute triangle edges */
		 SUB(e0,v1,v0);/* tri edge 0 */
		 SUB(e1,v2,v1);/* tri edge 1 */
		 SUB(e2,v0,v2);/* tri edge 2 */

		 /* Bullet 3:*/
		 /*test the 9 tests first (this was faster) */
		 fex = fabs(e0[axis_X]);
		 fey = fabs(e0[axis_Y]);
		 fez = fabs(e0[axis_Z]);
		 AXISTEST_X01(e0[axis_Z], e0[axis_Y], fez, fey);
		 AXISTEST_Y02(e0[axis_Z], e0[axis_X], fez, fex);
		 AXISTEST_Z12(e0[axis_Y], e0[axis_X], fey, fex);

		 fex = fabs(e1[axis_X]);
		 fey = fabs(e1[axis_Y]);
		 fez = fabs(e1[axis_Z]);
		 AXISTEST_X01(e1[axis_Z], e1[axis_Y], fez, fey);
		 AXISTEST_Y02(e1[axis_Z], e1[axis_X], fez, fex);
		 AXISTEST_Z0(e1[axis_Y], e1[axis_X], fey, fex);

		 fex = fabs(e2[axis_X]);
		 fey = fabs(e2[axis_Y]);
		 fez = fabs(e2[axis_Z]);
		 AXISTEST_X2(e2[axis_Z], e2[axis_Y], fez, fey);
		 AXISTEST_Y1(e2[axis_Z], e2[axis_X], fez, fex);
		 AXISTEST_Z12(e2[axis_Y], e2[axis_X], fey, fex);

		 /* Bullet 1: */
		 /*first test overlap in the {x,y,z}-directions */
		 /*find min, max of the triangle each direction, and test for overlap in */
		 /*that direction -- this is equivalent to testing a minimal AABB around */
		 /*the triangle against the AABB */

		 /* test in X-direction */
		 FINDMINMAX(v0[axis_X],v1[axis_X],v2[axis_X],min,max);
		 if(min>boxhalfsize[axis_X] || max<-boxhalfsize[axis_X]) return 0;

		 /* test in Y-direction */
		 FINDMINMAX(v0[axis_Y],v1[axis_Y],v2[axis_Y],min,max);
		 if(min>boxhalfsize[axis_Y] || max<-boxhalfsize[axis_Y]) return 0;

		 /* test in Z-direction */
		 FINDMINMAX(v0[axis_Z],v1[axis_Z],v2[axis_Z],min,max);
		 if(min>boxhalfsize[axis_Z] || max<-boxhalfsize[axis_Z]) return 0;

		 /* Bullet 2: */
		 /*test if the box intersects the plane of the triangle */
		 /*compute plane equation of triangle: normal*x+d=0 */
		 CROSS(normal,e0,e1);
		 d=-DOT(normal,v0);/* plane eq: normal.x+d=0 */
		 if(!planeBoxOverlap(normal,d,boxhalfsize)) 
			 return 0;
		 return 1; /* box and triangle overlaps */
	}


	int isIntersectWithBox(const AABB& box) override{
	
		double boxhalfsize[3] = {box.getSize().x * 0.5, 
								box.getSize().y * 0.5, 
								box.getSize().z * 0.5};

		double boxcenter[3] = {box.pos.x + boxhalfsize[0], 
								box.pos.y + boxhalfsize[1], 
								box.pos.z + boxhalfsize[2]};

		double triverts[3][3] = {{ vertex[0].x, vertex[0].y, vertex[0].z},
								{vertex[1].x, vertex[1].y, vertex[1].z},
								{vertex[2].x, vertex[2].y, vertex[2].z}};

		return triBoxOverlap(boxcenter, boxhalfsize, triverts);
	}



	AABB getAABB() override {
		double minx = INF, maxx = -INF;
		double miny = INF, maxy = -INF;
		double minz = INF, maxz = -INF;
		for (int i = 0; i < 3; i++){
			minx = min(minx, vertex[i].x); maxx = max(maxx, vertex[i].x);
			miny = min(miny, vertex[i].y); maxy = max(maxy, vertex[i].y);
			minz = min(minz, vertex[i].z); maxz = max(maxz, vertex[i].z);
		}
		return AABB(MyVec3(minx, miny, minz), MyVec3(maxx - minx, maxy - miny, maxz - minz));	
	}

	Material* material() {	return ma;	}

	Color getColorTexture(MyVec3& a_Pos){
		Color retval;
		if (ma->GetTexture())
		{
			//cout << "have plane texture " << endl;
			Texture* t = ma->GetTexture();

			MyVec3 m_UAxis = MyVec3( normalVec.y, normalVec.z, -normalVec.x );
			MyVec3 m_VAxis = MyVec3::cross(m_UAxis, normalVec );

			double u = MyVec3::dot( a_Pos, m_UAxis ) * ma->GetUScale();
			double v = MyVec3::dot( a_Pos, m_VAxis ) * ma->GetVScale();
			retval = t->GetTexel( u, v ) * ma->getColor();
		}
		else{
			//cout << "no plane texture" << endl;
			retval = ma->getColor();
		}
		return retval;
	}

	double GetMinCoord(int coord){
		double x0 = vertex[0][coord];
		double x1 = vertex[1][coord];
		double x2 = vertex[2][coord];
		if (x0 < x1)
			return (x0 < x2) ? x0 : x2;
		return (x1 < x2) ? x1 : x2;
	}

	double GetMaxCoord(int coord) {
		double x0 = vertex[0][coord];
		double x1 = vertex[1][coord];
		double x2 = vertex[2][coord];
		if (x0 > x1)
			return (x0 > x2) ? x0 : x2;
		return (x1 > x2) ? x1 : x2;
	}


	//int getType()  const override{
	//	//cout << "meme" << endl;
	//	return Config::TRI_TYPE;
	//}
	
	//---------------------------
	/*int isIntersectWithBox(const AABB& box) override{
		double hx = box.getSize().x * 0.5;
		double hy = box.getSize().y * 0.5;
		double hz = box.getSize().z * 0.5;
		MyVec3 o(box.getPos().x + hx, box.getPos().y + hy, box.getPos().z + hz);
		MyVec3 w, c, one, ano;
		for (int i = 0;i < 3;i++){
			one = vertex[i] - o;
			ano = vertex[(i == 2) ? 0 : (i + 1)] - o;
			w = 0.5 * (ano - one);
			c = one + w;
			if (!(fabs(c.x) > fabs(w.x) + hx))
				return true;
			if (!(fabs(c.y * w.z - c.z * w.y) > (hy * fabs(w.z) + hz * fabs(w.y))))
				return true;
			if (!(fabs(c.y) > fabs(w.y) + hy))
				return true;
			if (!(fabs(c.x * w.z - c.z * w.x) > (hx * fabs(w.z) + hz * fabs(w.x))))
				return true;
			if (!(fabs(c.z) > fabs(w.z) + hz))
				return true;
			if (!(fabs(c.x * w.y - c.y * w.x) > (hx * fabs(w.y) + hy * fabs(w.x))))
				return true;
		}

		return false;
	}*/
};



#endif