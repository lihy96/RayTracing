#include "rayTracer.h"

const int MAX_RECURSIVE_DEEPTH = 5;//���ݹ����

void RayTracer::run(MyMat& m, Camera& camera, MyObj& myobj){
	int rows = m.getRow(), cols = m.getCol();
	for(int c = 0;c < cols;c++){
		double u = 1 - ((double)c) / cols;
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //�������½��ǣ�0,0�������Ͻ��ǣ�1,1��
			Ray ray = camera.emit(u,v);// ��ת�������ĵ㷢��һ����
			Vec3d color = rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0).getImgColor();
			m.setPixel(r, c, color);
		}
	}
}

//�ҵ����������Ľ���
//û�н��㷵��0���������ཻ������ǹ�Դ�򷵻�-1������Ƿǹ�Դ�������򷵻�1��
int RayTracer::findNearest(const Ray& ray, const MyObj& scene, IntersectResult& result){
	bool is_light = false;
	int in_out = 0;
	//�ڳ����е�����������Ѱ�������һ������
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

	if(result.distance == INF)//û����ӵ����巵��0
		return 0;
	else //�ཻ�˵Ļ���ӳ���ڲ������ⲿ
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
	//�ڳ����е�����������Ѱ�������һ������
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
	//û�н���������
	//if (result.distance == INF)
	//	return BLACK;
	////�������Ľ����ǹ�Դ
	//if (is_light)
	//	return WHITE;

	//������;��淴��
	for (auto& it : scene.plight_vec){
		MyVec3 L = it->get().getCenter() - result.intersectPoint;	//���潻�㵽��Դ������	
		double Ldist = L.module();	//���浽��Դ�ľ���
		MyVec3::normalize(L);	
	
		//�жϹ�Դ�Ƿ����յ����潻��
		Ray shadowRay(result.intersectPoint, L); //�ӱ������Դ����һ������
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

		//��Դ�����յ����潻�㴦������phongģ�ͼ���
		//ע��������ʵ��������ཻ�㲢û����
		re = re + result.ma->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor());	
	}
		
	// ���㷴������
	double reflectiveness = result.ma->getReflectiveness();
	if (recursiveDepth > 0 && reflectiveness > 0) {
		MyVec3 dire = ray.getDir() - 2 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
		Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
		Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex);
		re = re + reflectiveness * got * result.ma->getColor();
	}	

	// ������������
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


