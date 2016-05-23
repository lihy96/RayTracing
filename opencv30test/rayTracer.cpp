#include "rayTracer.h"

const int MAX_RECURSIVE_DEEPTH = 5;//最大递归深度

void RayTracer::run(MyMat& m, Camera& camera, MyObj& myobj){
	int rows = m.getRow(), cols = m.getCol();
	for(int c = 0;c < cols;c++){
		double u = 1 - ((double)c) / cols;
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //屏的左下角是（0,0），右上角是（1,1）
			Ray ray = camera.emit(u,v);// 向转化过来的点发出一条光
			Vec3d color = rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0).getImgColor();
			m.setPixel(r, c, color);
		}
	}
}

//找到最近的物体的交点
//没有交点返回0，有物体相交：如果是光源则返回-1；如果是非光源的物体则返回1；
int RayTracer::findNearest(const Ray& ray, const MyObj& scene, IntersectResult& result){
	bool is_light = false;
	int in_out = 0;
	//在场景中的所有物体中寻找最近的一个交点
	for (auto& it : scene.sphere_vec){
		IntersectResult temp;
		int re = it->intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				result = temp;
				in_out = re;
			}
		}
	}
	for (auto& it : scene.plane_vec){
		IntersectResult temp;
		int re = it->intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				result = temp;
				in_out = re;
			}
		}
		
	}
	for (auto& it : scene.plight_vec){
		IntersectResult temp;
		int re = it->get().intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				is_light = true;
				result = temp;
				in_out = re;
			}
		}
	}

	if(result.distance == INF)//没有相加的物体返回0
		return 0;
	else //相交了的话反映是内部还是外部
		return in_out;
}

const Color RayTracer::rayTraceRecursive(const Ray& ray, const MyObj& scene, int recursiveDepth, double irefIndex){
	
	Color re = BLACK;
	IntersectResult	result;
	result.distance = INF;
	bool is_light = false;
	double in_out = 0.0;
	
	int flag = findNearest(ray, scene, result);
	if(flag == 0)
		return BLACK;
	else{
		in_out = flag;
		//if(result.primi->getType() == Config::LIGHT_TYPE){
		//	//return ((PointLight*)(result.primi))->getColor();
		//}
		if (dynamic_cast<PointLight*>(const_cast<Primitive*>(result.primi)) != 0)
			return dynamic_cast<PointLight*>(const_cast<Primitive*>(result.primi))->getColor();
			
	}
	
	//cout << result << endl;
	//在场景中的所有物体中寻找最近的一个交点
	/*for (auto& it : scene.sphere_vec){
		IntersectResult temp;
		int re = it->intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				result = temp;
				in_out = re;
			}
		}
	}
	for (auto& it : scene.plane_vec){
		IntersectResult temp;
		int re = it->intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				result = temp;
				in_out = re;
			}
		}
		
	}
	for (auto& it : scene.plight_vec){
		IntersectResult temp;
		int re = it->get().intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				is_light = true;
				result = temp;
				in_out = re;
			}
		}
	}*/
	//cout << result << endl;
	//没有交点的情况下
	//if (result.distance == INF)
	//	return BLACK;
	////如果最近的交点是光源
	//if (is_light)
	//	return WHITE;

	//漫反射和镜面反射
	for (auto& it : scene.plight_vec){
		MyVec3 L = it->get().getCenter() - result.intersectPoint;	//表面交点到光源的向量	
		double Ldist = L.module();	//表面到光源的距离
		MyVec3::normalize(L);	
	
		//判断光源是否能照到表面交点
		Ray shadowRay(result.intersectPoint, L); //从表面向光源发射一条光线
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

		//光源可以照到表面交点处，利用phong模型计算
		//注意这里其实传入表面相交点并没有用
		re = re + result.ma->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor());	
	}
		
	// 计算反射的情况
	double reflectiveness = result.ma->getReflectiveness();
	if (recursiveDepth > 0 && reflectiveness > 0) {
		MyVec3 dire = ray.getDir() - 2 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
		Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
		Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex);
		re = re + reflectiveness * got * result.ma->getColor();
	}	

	// 计算折射的情况
	double refraction = result.ma->getRefraction();
	if (recursiveDepth > 0 && refraction > 0) {
		double tIndex = result.ma->getRefraIndex();
		double n = irefIndex / tIndex;
		MyVec3 N = in_out * result.normalVec;
		double cosI = -MyVec3::dot(N, ray.getDir());
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


