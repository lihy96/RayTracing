#include "rayTracer.h"

const int MAX_RECURSIVE_DEEPTH = 5;//最大递归深度


void RayTracer::initGrid(const MyObj& scene){
	MyVec3 loBound(-14.0, -5.0, -6.0), hiBound(14.0, 8.0, 30.0);
	wholeSpace = AABB(loBound, hiBound - loBound);
	
	int gridSize = Config::GRIDSIZE;
	
	double unitx = (hiBound.x - loBound.x) / gridSize;
	double unity = (hiBound.y - loBound.y) / gridSize;
	double unitz = (hiBound.z - loBound.z) / gridSize;
	
	unit = MyVec3(unitx, unity, unitz);

	for (auto& it : scene.sphere_vec)
		addPrimiToBox(it, loBound, hiBound);
		
	for (auto& it : scene.plane_vec)
		addPrimiToBox(it, loBound, hiBound);
		
	for (auto& it : scene.plight_vec)
		addPrimiToBox(it, loBound, hiBound);


}

void RayTracer::run(MyMat& m, Camera& camera, MyObj& myobj){
	
	initGrid(myobj);

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
int RayTracer::findNearestNoAABB(const Ray& ray, const MyObj& scene, IntersectResult& result){
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
		int re = (it->get()).intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				is_light = true;
				result = temp;
				result.primi = it;
				in_out = re;
			}
		}
	}

	if(result.distance == INF)//没有相加的物体返回0
		return 0;
	else //相交了的话反映是内部还是外部
		return in_out;
}







//#define DEBUG_
int RayTracer::findNearest(const Ray& ray, const MyObj& scene, IntersectResult& result){

#ifdef DEBUG_
	cout << "-------------" << endl;
	for(int i = 0;i < 8;i++){
		for(int j = 0;j < 8;j++){
			for(int k = 0;k < 8;k++){
				cout << allGrids[i][j][k].sphere_vec.size() << endl;
				cout << allGrids[i][j][k].plane_vec.size() << endl;
				cout << allGrids[i][j][k].plight_vec.size() << endl;
			}

		}

	}
	//exit(9);
#endif




	//	X/Y/Z: current coordinate of grid 
	
	int gridSize = Config::GRIDSIZE;

	int X = (ray.getOri().x - wholeSpace.getPos().x) / unit.x;
	int Y = (ray.getOri().y - wholeSpace.getPos().y) / unit.y;
	int Z = (ray.getOri().z - wholeSpace.getPos().z) / unit.z;	
	
	//	the source of ray is out of grid
	if ((X < 0) || (X >= gridSize)
		|| (Y < 0) || (Y >= gridSize)
		|| (Z < 0) || (Z >= gridSize))
		return Config::MISS;
		
	//	step: the increasing delta
	//	out: the stop boundary		
	//	nextCoordinate: the next coordiante along ray direction	
	int stepX, outX, stepY, outY, stepZ, outZ;	
	MyVec3 nextCoordinate;
	
	if (ray.getDir().x > 0.0){
		stepX = 1;
		outX = gridSize;
		nextCoordinate.x = wholeSpace.getPos().x + (X + 1.0) * unit.x;
	}
	else{
		stepX = -1;
		outX = -1;
		nextCoordinate.x = wholeSpace.getPos().x + X * unit.x;
	}

	if (ray.getDir().y > 0.0){
		stepY = 1;
		outY = gridSize;
		nextCoordinate.y = wholeSpace.getPos().y + (Y + 1.0) * unit.y;
	}
	else{
		stepY = -1;
		outY = -1;
		nextCoordinate.y = wholeSpace.getPos().y + Y * unit.y;
	}

	if (ray.getDir().z > 0.0){
		stepZ = 1;
		outZ = gridSize;
		nextCoordinate.z = wholeSpace.getPos().z + (Z + 1.0) * unit.z;
	}
	else{
		stepZ = -1;
		outZ = -1;
		nextCoordinate.z = wholeSpace.getPos().z + Z * unit.z;
	}

	//	tmax: getting a boundary at most needs t (o + t * dire)
	//	tmax.x->x boundary tmax.y->y boundary tmax.z->z boundary (select the min) 
	//	tdelta: crossing a cell at least needs t 
	//	x,y,z -> cross x,y,z boundary respectly
	
	MyVec3 tmax, tdelta;
	if (ray.getDir().x != 0.0){
		tmax.x = (nextCoordinate.x - ray.getOri().x) / ray.getDir().x;
		tdelta.x = unit.x * stepX / ray.getDir().x;
	}
	else 
		tmax.x = INF;

	if (ray.getDir().y != 0.0){
		tmax.y = (nextCoordinate.y - ray.getOri().y) / ray.getDir().y;
		tdelta.y = unit.y * stepY / ray.getDir().y;
	}
	else
		tmax.y = INF;

	if (ray.getDir().z != 0.0){
		tmax.z = (nextCoordinate.z - ray.getOri().z) / ray.getDir().z;
		tdelta.z = unit.z * stepZ / ray.getDir().z;
	}
	else
		tmax.z = INF;

	//cout << tmax << tdelta << endl;exit(9);


	//	find the first intersection
	int re = Config::MISS;
	while (true){
		//int i = 0;
		
		for (auto& it : allGrids[X][Y][Z].sphere_vec){	
			//cout << "1--" << endl;
			//cout << allGrids[X][Y][Z].sphere_vec.size() << endl;
			if (nearest(it, ray, scene, result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].plane_vec){
			//cout << "2--" << endl;
			//cout << allGrids[X][Y][Z].plane_vec.size() << endl;
			if (nearest(it, ray, scene, result, re)){
				goto closer;
			}
		}

		for (auto& it : allGrids[X][Y][Z].plight_vec){
			//cout << "3--" << endl;
			//cout << allGrids[X][Y][Z].plight_vec.size() << endl;
			if (nearest(it, ray, scene, result, re)){
				goto closer;
			}
		}
		//cout << "4--" << endl;
		if (tmax.x < tmax.y){
			if (tmax.x < tmax.z){
				X += stepX;
				if (X == outX)
					return Config::MISS;
				tmax.x += tdelta.x;
			}
			else{
				Z += stepZ;
				if (Z == outZ)
					return Config::MISS;
				tmax.z += tdelta.z;
			}
		}
		else {
			if (tmax.y < tmax.z){
				Y += stepY;
				if (Y == outY)
					return Config::MISS;
				tmax.y += tdelta.y;
			}
			else{
				Z += stepZ;
				if (Z == outZ)
					return Config::MISS;
				tmax.z += tdelta.z;
			}
		}
	}




closer:
	//cout << tmax << tdelta << endl;exit(9);
	//	find the correct intersection
	while (true){
		for (auto& it : allGrids[X][Y][Z].sphere_vec)				
			nearest(it, ray, scene, result, re);
		for (auto& it : allGrids[X][Y][Z].plane_vec)
			nearest(it, ray, scene, result, re);
		for (auto& it : allGrids[X][Y][Z].plight_vec)
			nearest(it, ray, scene, result, re);

		if (tmax.x < tmax.y){
			if (tmax.x < tmax.z){
				//	find the correct intersection
				if (result.distance < tmax.x)
					break;
				X += stepX;
				if (X == outX)
					break;
				tmax.x += tdelta.x;
			}
			else{
				if (result.distance < tmax.z)
					break;
				Z += stepZ;
				if (Z == outZ)
					break;
				tmax.z += tdelta.z;
			}
		}
		else {
			if (tmax.y < tmax.z){
				if (result.distance < tmax.y)
					break;
				Y += stepY;
				if (Y == outY)
					break;
				tmax.y += tdelta.y;
			}
			else{
				if (result.distance < tmax.z)
					break;
				Z += stepZ;
				if (Z == outZ)
					break;
				tmax.z += tdelta.z;
			}
		}
	}
	return re;
}

const Color RayTracer::rayTraceRecursive(const Ray& ray, const MyObj& scene, int recursiveDepth, double irefIndex){
	
	Color re = BLACK;
	IntersectResult	result;
	result.distance = INF;
	double in_out = findNearest(ray, scene, result);
	//cout << in_out << endl;
	if(in_out == 0)	//光线未与任何物体相交
		return BLACK;
	else{	//光线与某一个物体相交
		if(result.primi->getType() == Config::LIGHT_TYPE){
			const PointLight* m = static_cast<const PointLight*>(result.primi);
			return m->getColor();
			//return dynamic_cast<PointLight*>(const_cast<Primitive*>(result.primi))->getColor();
		}
	}


	

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


