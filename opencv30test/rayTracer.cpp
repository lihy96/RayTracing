#include "rayTracer.h"

void RayTracer::run(MyMat& m, Camera& camera, MyObj& myobj){
	int rows = m.getRow(), cols = m.getCol();
	for(int c = 0;c < cols;c++){
		double u = 1 - ((double)c) / cols;
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //屏的左下角是（0,0），右上角是（1,1）
			Ray ray = camera.emit(u,v);// 向转化过来的点发出一条光线
			m.setPixel(r, c, rayTraceRecursive(ray, myobj, 5, 1.0).getImgColor());	
		}

	}
}

const Color rayTraceRecursive(const Ray& ray, const MyObj& scene, int recursiveDepth, double irefIndex){
	Color re = BLACK;
	IntersectResult	result;
	result.distance = INF;
	bool is_light = false;
	double in_out;

	//	find the nearest intersection
	for (auto& it : scene.sphere_vec){
		IntersectResult temp;
		int re;
		if (re = it->intersect(ray, temp))
			if (temp.distance < result.distance){
				result = temp;
				in_out = re;
			}
	}
	for (auto& it : scene.plane_vec){
		IntersectResult temp;
		int re;
		if (re = it->intersect(ray, temp))
			if (temp.distance < result.distance){
				result = temp;
				in_out = re;
			}
	}
	for (auto& it : scene.plight_vec){
		IntersectResult temp;
		int re;
		if (re = it->get().intersect(ray, temp))
			if (temp.distance < result.distance){
				is_light = true;
				result = temp;
				in_out = re;
			}
	}

	//	no intersection
	if (result.distance == INF)
		return BLACK;
	//	intersection is in light
	if (is_light)
		return WHITE;
	//	figure out the diffuseColor and specularColor
	for (auto& it : scene.plight_vec){
		MyVec3 L = it->get().getCenter() - result.intersectPoint;
		double Ldist = sqrt(dot(L, L));			
		normalize(L);
		//	whether it's screened or not
		Ray shadowRay(result.intersectPoint, L);
		int shadowFlag = 0;
		for (auto& it : scene.sphere_vec){
			IntersectResult temp;
			if (it->intersect(shadowRay, temp) && temp.distance < Ldist){
				shadowFlag = 1;
				break;
			}
		}
	
		if (shadowFlag)
			continue;
		for (auto& it : scene.plane_vec){
			IntersectResult temp;
			if (it->intersect(shadowRay, temp) && temp.distance < Ldist){
				shadowFlag = 1;
				break;
			}
		}
		if (shadowFlag)
			continue;
		re = re + result.ma->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor());	
	}
		
	//	calculate reflection
	double reflectiveness = result.ma->getReflectiveness();
	if (recursiveDepth > 0 && reflectiveness > 0){
		MyVec3 dire = ray.getDir() - 2 * dot(ray.getDir(), result.normalVec) * result.normalVec;
		Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
		Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex);
		re = re + reflectiveness * got * result.ma->getColor();
	}	

	//	calculate refraction
	double refraction = result.ma->getRefraction();
	if (recursiveDepth > 0 && refraction > 0)
	{
		double tIndex = result.ma->getRefraIndex();
		double n = irefIndex / tIndex;
		MyVec3 N = in_out * result.normalVec;
		double cosI = -dot(N, ray.getDir() );
		double cosT = 1.0 - n * n * (1.0 - cosI * cosI);
		if (cosT > 0.0){
			MyVec3 T = (n * ray.getDir()) + (n * cosI - sqrt(cosT)) * N;
			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIANCE * T, T), scene, recursiveDepth - 1, tIndex);
			// apply Beer's law
			Color absorbance = -result.distance * 0.15 * result.ma->getColor();				
			Color transparency = Color(exp(absorbance.r()), exp(absorbance.g()), exp(absorbance.b()));				
			re = re + got * transparency;
		}
	}
	return re;
}


