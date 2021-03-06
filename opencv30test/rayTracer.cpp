#include "rayTracer.h"
#include "config.h"

#define PATHTRACE
#define DEBUG
#ifdef PATHTRACE
#define PUSU
#define DIFF 0
#define SPEC 1
#define REFR 2
#endif

string name[10] = {"aa.png","bb.png","cc.png","dd.png","ee.png","ff.png","gg.png","hh.png"};
string tmpname[10] = {"aat.png","bbt.png","cct.png","ddt.png","eet.png","fft.png","ggt.png"};

void RayTracer::initGrid(const MyObj& scene){
#ifdef LHY_KDTREE
	exit(54);
#endif
	MyVec3 loBound(-15.0, -10.0, -5.0), hiBound(15.0, 10.0, 30.0);
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

	for(auto& it : scene.blight_vec)
		addPrimiToBox(it, loBound, hiBound);

	for(auto& it : scene.tri_vec)
		addPrimiToBox(it, loBound, hiBound);

}



void RayTracer::run(MyMat& m, Camera& camera,int lo, int hi){

#ifndef LHY_KDTREE
	initGrid(scene);
	cout <<" en ";
#endif

	int rows = m.getRow(), cols = m.getCol();

#ifndef SUPERSAM
	for(int c = 0;c < cols;c++){
		double u = 1 - ((double)c) / cols;
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //屏的左下角是（0,0），右上角是（1,1）
			Ray ray = camera.emit(u,v);
			Vec3d color = rayTraceRecursive(ray, TRACEMAXRECUR, 1.0, SAMPLES, SAMPLES_DOWN).getImgColor();
			m.setPixel(r, c, color);
		}
		m.show("f");
		waitKey(5);
	}
#endif

#ifdef SUPERSAM

	double pixel_width = 1.0 / cols;
	double pixel_height = 1.0 / rows;
	double pixel_width_half = 0.5 * pixel_width;
	double pixel_height_half = 0.5 * pixel_height;

	double sample_width = pixel_width / SUPER_SAMPLES;
	double sample_height = pixel_height / SUPER_SAMPLES;
	double sample_width_half = 0.5 * sample_width;
	double sample_height_half = 0.5 * sample_height;

	double SUPER_SAMPLES_2_DOWN = (1.0) / ( SUPER_SAMPLES * SUPER_SAMPLES );

	for(int c = 0;c < cols;c++){

				if(c < lo || c > hi) continue;

		double u = 1 - ((double)c) / cols;
		fprintf(stderr,"\rRendering (%d spp) %5.2f%%",1*4,100.*c/(cols-1));  
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //屏的左下角是（0,0），右上角是（1,1）
			double sampleStartU = u - pixel_width_half + sample_width_half;
			double sampleStartV = v - pixel_height_half + sample_height_half;


			Color color(0,0,0);
			for(int x = 0; x < SUPER_SAMPLES; x++){
				for(int y = 0;y < SUPER_SAMPLES; y++){

#ifdef jingshen
					double u_bia = sampleStartU + x * sample_width;
					double v_bia = sampleStartV + y * sample_height;
					Ray rayToFocus = camera.emit(u_bia , v_bia);
					
					MyVec3 dir = rayToFocus.getDir();
					
					MyVec3 focus_p = rayToFocus.getOri() + (FOCUS_DIS / MyVec3::dot(camera.front, dir) ) * dir;
					
					for(int i = 0;i < DEEP_OF_FIELD_NUM;i++){
						
						double disturbX = rand() * APERTURE_R_PIXEL / RAND_MAX;
						double disturbY = rand() * APERTURE_R_PIXEL / RAND_MAX;
						camera.eye.x += disturbX;
						camera.eye.y += disturbY;
						
						Ray ray = camera.emit(focus_p);

		
						camera.eye.x -= disturbX;
						camera.eye.y -= disturbY;
						IntersectResult result;
						color  = color + DEEP_OF_FIELD_NUM_DOWN * SUPER_SAMPLES_2_DOWN * rayTraceRecursive(ray,result, 0, 1.0, SAMPLES, SAMPLES_DOWN);
					}
#else
					Ray ray = camera.emit(sampleStartU + x * sample_width, sampleStartV + y * sample_height);
					IntersectResult result;
					color = color + SUPER_SAMPLES_2_DOWN * rayTraceRecursive(ray, result, 0, 1.0, GLOSSYSAMPLETIME, GLOSSYSAMPLEDOWN);
#endif
				}
			}
			m.setPixel(r, c, color);
		}

		m.show("super");
		cv::waitKey(5);
	}
#endif



}

//找到最近的物体的交点
//没有交点返回0，有物体相交：如果是光源则返回-1；如果是非光源的物体则返回1；
int RayTracer::findNearestNoAABB(const Ray& ray, IntersectResult& result){
		//int re = 0;
		//for (auto& it : scene.sphere_vec){
		//	int in_out = 0;
		//	if (it->id != ray.getID()){
		//		if (in_out = it->intersect(ray, result))
		//			re = in_out;													
		//	}
		//}
		//for (auto& it : scene.plight_vec){
		//	int in_out = 0;
		//	if (it->id != ray.getID()){
		//		if (in_out = it->intersect(ray, result))
		//			re = in_out;													
		//	}
		//}
		//for (auto& it : scene.plane_vec){
		//	int in_out = 0;
		//	if (it->id != ray.getID()){
		//		if (in_out = it->intersect(ray, result))
		//			re = in_out;													
		//	}
		//}
		//for (auto& it : scene.blight_vec){
		//	int in_out = 0;
		//
		//		if (in_out = it->intersect(ray, result))
		//			re = in_out;	
		//}
		//	
		//for (auto& it : scene.poly_vec){
		//	int in_out = 0;
		//
		//		if (in_out = it->intersect(ray, result))
		//			re = in_out;													
		//	
		//}
		//return re;



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

	for (auto& it : scene.blight_vec){
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

	for (auto& it : scene.poly_vec){
		IntersectResult temp;
		int re = it->intersect(ray, temp);
		if (re){
			if (temp.distance < result.distance){
				result = temp;
				in_out = re;
			}
		}
	}

	if(result.distance > INF - 1)//没有相加的物体返回0
		return 0;
	else //相交了的话反映是内部还是外部
		return in_out;
}



int RayTracer::findNearest(const Ray& ray, IntersectResult& result){
#ifdef LHY_KDTREE	
	return findNearestNoAABB(ray,result);
#endif	
	
	//	X/Y/Z: current coordinate of grid 
	
	int gridSize = Config::GRIDSIZE;

	int X = (ray.getOri().x - wholeSpace.getPos().x) / unit.x;
	int Y = (ray.getOri().y - wholeSpace.getPos().y) / unit.y;
	int Z = (ray.getOri().z - wholeSpace.getPos().z) / unit.z;	
	
	//	the source of ray is out of grid
	// 由于照相机是在一个盒子里面的，所以发光点不在盒子里的光线不予考虑
	if ((X < 0) || (X >= gridSize)
		|| (Y < 0) || (Y >= gridSize)
		|| (Z < 0) || (Z >= gridSize))
		return MISS;
		
	//	step: the increasing delta
	//	out: the stop boundary		
	//	nextCoordinate: the next coordiante along ray direction	
	int stepX, outX, stepY, outY, stepZ, outZ;	
	MyVec3 nextCoordinate;
	
	//由于盒子是从[0,gridsize)，所以out分别为gridsize and -1;
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

	//	find the first intersection
	int re = MISS;
	while (true){
		for (auto& it : allGrids[X][Y][Z].sphere_vec){	
			if (nearest(it, ray,result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].plane_vec){
			if (nearest(it, ray, result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].plight_vec){

			if (nearest(it, ray,result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].blight_vec){
			if (nearest(it, ray, result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].tri_vec){
			if (nearest(it, ray, result, re)){
				goto closer;
			}
		}

		if (tmax.x < tmax.y){
			if (tmax.x < tmax.z){
				X += stepX;
				if (X == outX)
					return MISS;
				tmax.x += tdelta.x;
			}
			else{
				Z += stepZ;
				if (Z == outZ)
					return MISS;
				tmax.z += tdelta.z;
			}
		}
		else {
			if (tmax.y < tmax.z){
				Y += stepY;
				if (Y == outY)
					return MISS;
				tmax.y += tdelta.y;
			}
			else{
				Z += stepZ;
				if (Z == outZ)
					return MISS;
				tmax.z += tdelta.z;
			}
		}
	}
closer:
	//	find the correct intersection
	while (true){
		for (auto& it : allGrids[X][Y][Z].sphere_vec)				
			nearest(it, ray, result, re);
		for (auto& it : allGrids[X][Y][Z].plane_vec)
			nearest(it, ray,  result, re);
		for (auto& it : allGrids[X][Y][Z].plight_vec)
			nearest(it, ray, result, re);
		for (auto& it : allGrids[X][Y][Z].blight_vec)
			nearest(it, ray,result, re);
		for (auto& it : allGrids[X][Y][Z].tri_vec)
			nearest(it, ray,result, re);

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


//const Color RayTracer::rayTraceRecursive(const Ray& ray, IntersectResult& result, int recursiveDepth, double irefIndex,double sampleTime,double sampleDown){
//	
//	Color re = BLACK;
//	//IntersectResult	result;
//	//result.distance = INF;
//	double in_out = findNearest(ray,result);
//
//	Color ColorTexture;
//
//	if(in_out == 0)	//光线未与任何物体相交
//		return BLACK;
//	else{	//光线与某一个物体相交
//		int type = result.primi->getType();
//		if(type == Config::LIGHT_POINT_TYPE){
//			const PointLight* m = static_cast<const PointLight*>(result.primi);
//			return m->getColor();
//			//return dynamic_cast<PointLight*>(const_cast<Primitive*>(result.primi))->getColor();
//		}else if(type == Config::LIGHT_BOX_LIGHT){
//			const BoxLight* m = static_cast<const BoxLight*>(result.primi);
//			return m->getColor();
//		}else if(type == Config::SPHERE_TYPE){
//			Sphere* m = static_cast<Sphere*>(result.primi);
//			ColorTexture = m->getColorTexture( result.intersectPoint );
//		}else if(type == Config::PLANE_TYPE){
//			Plane* m = static_cast<Plane*>(result.primi);
//			ColorTexture = m->getColorTexture( result.intersectPoint );
//		}
//		else if(type == Config::TRI_TYPE){
//			Triangle* m = static_cast<Triangle*>(result.primi);
//			ColorTexture = m->getColorTexture(result.intersectPoint);
//		}else
//			exit(66);
//	}
//	
//	
//	//漫反射和镜面反射
//	for (auto& it : scene.plight_vec){
//		MyVec3 L = it->get().getCenter() - result.intersectPoint;	//表面交点到光源的向量	
//		double Ldist = L.module();	//表面到光源的距离
//		MyVec3::normalize(L);	
//	
//		//判断光源是否能照到表面交点
//		Ray shadowRay(result.intersectPoint + DEVIANCE * L, L); //从表面向光源发射一条光线
//		IntersectResult temp;
//		findNearest(shadowRay,temp);
//		if(temp.primi != it)
//			continue;
//
//		//光源可以照到表面交点处，利用phong模型计算
//		//注意这里其实传入表面相交点并没有用
//		
//		re = re + result.primi->material()->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor(),ColorTexture);	
//	}
//
//	for (auto& it : scene.blight_vec){
//		MyVec3 L;
//		double shadow = calSoftShade(it, result.intersectPoint, L, scene,sampleTime, sampleDown);
//		if (shadow > 0)
//			re = re + shadow * result.primi->material()->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor());			
//	}
//		
//	// 计算反射的情况
//	double reflectiveness = result.ma->getReflectiveness();
//	if (recursiveDepth > 0 && reflectiveness > 0){
//		double diffReflectOffset = result.primi->material()->getGlossyReflect();
//		//	only perform diffuse reflection for primary rays; otherwise it's just too slow.
//#ifdef SAMPLEWAY
//		if ((diffReflectOffset > 0) && (recursiveDepth == 0)){
//			MyVec3 dire = ray.getDir() - 2.0 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
//			//	direXZ and dire are vertical crossing in xz plane
//			//MyVec3 direXZ(dire.z, dire.y, -dire.x);
//			MyVec3 direXZ(dire.z, 0, -dire.x);
//			MyVec3 direY = MyVec3::cross(dire, direXZ);
//			reflectiveness *= SAMPLES_DOWN;
//			for (int i = 0; i < SAMPLES; i++){
//				double xzoffs = Config::myRandhalf();
//				double yoffs = Config::myRand01() * (0.5 - xzoffs * xzoffs);
//				MyVec3 realDire = dire + xzoffs * diffReflectOffset * direXZ + yoffs * diffReflectOffset * direY;
//				MyVec3::normalize(realDire);
//				Ray reflect(result.intersectPoint + DEVIANCE * realDire, realDire);
//				IntersectResult resultNext;
//				Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime, sampleDown);
//				//re = re + reflectiveness * got * result.primi->material()->getColor();
//				re = re + reflectiveness * got * ColorTexture;
//			}
//		}
//#else
//		if ((diffReflectOffset > 0) && (recursiveDepth > TRACEMAXRECUR - 2)){
//			MyVec3 dire = ray.getDir() - 2.0 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
//			//	direXZ and dire are vertical crossing in xz plane
//			//MyVec3 direXZ(dire.z, dire.y, -dire.x);
//			MyVec3 direXZ(dire.z, 0, -dire.x);
//			MyVec3 direY = MyVec3::cross(dire, direXZ);
//			reflectiveness *= sampleDown;
//			for (int i = 0; i < sampleTime; i++){
//				double xzoffs = Config::myRand01();
//				double yoffs = Config::myRand01() * (1 - xzoffs * xzoffs);
//				MyVec3 realDire = dire + xzoffs * diffReflectOffset * direXZ + yoffs * diffReflectOffset * direY;
//				MyVec3::normalize(realDire);
//				Ray reflect(result.intersectPoint + DEVIANCE * realDire, realDire);
//				IntersectResult resultNext;
//				Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime * 0.25, sampleDown * 4.0);
//				//re = re + reflectiveness * got * result.primi->material()->getColor();
//				re = re + reflectiveness * got * ColorTexture;
//			}
//		}
//#endif
//		else{
//			MyVec3 dire = ray.getDir() - 2 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
//			//	move origin towards direction a little distance
//			//	in order to figure out right intersection
//			Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
//			IntersectResult resultNext;
//#ifdef SAMPLEWAY
//			Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime, sampleDown);
//#else
//			Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime * 0.25, sampleDown * 4.0);
//#endif
//			//re = re + reflectiveness * got * result.primi->material()->getColor();
//			re = re + reflectiveness * got * ColorTexture;
//		}
//	}
//
//
//	/*if (recursiveDepth > 0 && reflectiveness > 0) {
//		MyVec3 dire = ray.getDir() - 2 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
//		Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
//		Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex);
//		re = re + reflectiveness * got * result.ma->getColor();
//	}	*/
//
//	// 计算折射的情况
//	double refraction = result.primi->material()->getRefraction();
//	if (recursiveDepth > 0 && refraction > 0) {
//		double tIndex = result.ma->getRefraIndex();
//		if(in_out == -1) tIndex = 1;
//		double n = irefIndex / tIndex;
//		//double n = irefIndex / tIndex;
//		//double n = (in_out == 1) ? 1.0 / tIndex : irefIndex / tIndex; 
//		//double n = (in_out == 1) ? tIndex : irefIndex / tIndex; 
//		//double n = (in_out == 1) ? irefIndex : irefIndex / tIndex;
//		MyVec3 N = in_out * result.normalVec;
//		double cosI = -MyVec3::dot(N, ray.getDir());
//		double cosT = 1.0 - n * n * (1.0 - cosI * cosI);
//		if (cosT > 0.0){
//			MyVec3 T = (n * ray.getDir()) + (n * cosI - sqrt(cosT)) * N;
//			IntersectResult resultNext;
//#ifdef SAMPLEWAY
//			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * T, T), resultNext, recursiveDepth + 1, tIndex, sampleTime, sampleDown);
//#else
//			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * T, T), resultNext, recursiveDepth + 1, tIndex, sampleTime * 0.25, sampleDown * 4.0);
//#endif
//			// apply Beer's law
//			Color absorbance = -resultNext.distance * 0.15 * result.primi->material()->color;				
//			Color transparency = Color(exp(absorbance.r), exp(absorbance.g), exp(absorbance.b));				
//			re = re + got * transparency;
//		}
//	}
//	return re;
//}

const Color RayTracer::rayTraceRecursive(const Ray& ray, IntersectResult& result, int recursiveDepth, double irefIndex, double sampleTime, double sampleDown){
	//	find the nearest intersection
	double in_out = findNearest(ray, result);
	//	no intersection
	
	//	intersection is in light
	Color ColorTexture;

	if(in_out == 0)	//光线未与任何物体相交
		return BLACK;
	else{	//光线与某一个物体相交
		int type = result.primi->getType();
		if(type == Config::LIGHT_POINT_TYPE){
			const PointLight* m = static_cast<const PointLight*>(result.primi);
			return m->getColor();
			//return dynamic_cast<PointLight*>(const_cast<Primitive*>(result.primi))->getColor();
		}else if(type == Config::LIGHT_BOX_LIGHT){
			const BoxLight* m = static_cast<const BoxLight*>(result.primi);
			return m->getColor();
		}else if(type == Config::SPHERE_TYPE){
			Sphere* m = static_cast<Sphere*>(result.primi);
			ColorTexture = m->getColorTexture( result.intersectPoint );
		}else if(type == Config::PLANE_TYPE){
			Plane* m = static_cast<Plane*>(result.primi);
			ColorTexture = m->getColorTexture( result.intersectPoint );
		}
		else if(type == Config::TRI_TYPE){
			Triangle* m = static_cast<Triangle*>(result.primi);
			ColorTexture = m->getColorTexture(result.intersectPoint);
		}else
			exit(66);
	}

		Color re = BLACK;	
		//	figure out the diffuseColor and specularColor
		for (auto& it : scene.plight_vec){
			MyVec3 L = it->get().getCenter() - result.intersectPoint;
			double Ldist = sqrt(MyVec3::dot(L, L));			
			MyVec3::normalize(L);
			//	whether it's screened or not
			Ray shadowRay(result.intersectPoint + DEVIATION * L, L);
			IntersectResult temp;
			findNearest(shadowRay, temp);					
			if (temp.primi != it)
				continue;
			re = re + result.primi->material()->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor(),ColorTexture);
		}
		for (auto& it : scene.blight_vec){
			MyVec3 L;
			double shadow = calSoftShade(it, result.intersectPoint, L,scene, sampleTime, sampleDown);
			if (shadow > 0)
				re = re + shadow * result.primi->material()->sample(ray.direction, L, result.normalVec, result.intersectPoint, it->getColor());			
		}
	//	calculate reflection
		double reflectiveness = result.primi->material()->getReflectiveness();
		if (recursiveDepth < TRACEMAXRECUR && reflectiveness > 0){
			double glossyReflectOffset = result.primi->material()->getGlossyReflect();
			//	only perform diffuse reflection for primary rays; otherwise it's just too slow.
#ifndef SAMPLEWAY
			if ((glossyReflectOffset > 0) && (recursiveDepth == 0)){
				MyVec3 dire = ray.d - 2.0 * MyVec3::dot(ray.d, result.normalVec) * result.normalVec;
				//	direXZ and dire are vertical crossing in xz plane
				MyVec3 direXZ(dire.z, 0, -dire.x);
				MyVec3 direY = MyVec3::cross(dire, direXZ);
				reflectiveness *= GLOSSYSAMPLEDOWN;
				for (int i = 0; i < GLOSSYSAMPLETIME; i++){
					double xzoffs = Config::.exrand01();
					double yoffs = Config::.exrand01() * (1.0 - xzoffs * xzoffs);
					MyVec3 realDire = dire + xzoffs * glossyReflectOffset * direXZ + yoffs * glossyReflectOffset * direY;
					MyVec3::normalize(realDire);
					Ray reflect(result.intersectPoint + DEVIATION * realDire, realDire);
					IntersectResult resultNext;
					Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime, sampleDown);
					#ifndef TEXTURE
					re = re + reflectiveness * got * result.primi->material()->color;
					#else
					if (result.primi->material()->texExist)
						re = re + reflectiveness * got * result.primi->material()->getTexc(result.primi, result.intersectPoint);
					else
						re = re + reflectiveness * got * result.primi->material()->color;
					#endif
				}
			}
#else
		if ((glossyReflectOffset > 0) && (recursiveDepth < 2)){
			MyVec3 dire = ray.direction - 2.0 * MyVec3::dot(ray.direction, result.normalVec) * result.normalVec;
			//	direXZ and dire are vertical crossing in xz plane
			MyVec3 direXZ(dire.z, 0, -dire.x);
			MyVec3 direY = MyVec3::cross(dire, direXZ);
			reflectiveness *= sampleDown;
			for (int i = 0; i < sampleTime; i++){
				double xzoffs = Config::exrand01();
				double yoffs = Config::exrand01() * (1.0 - xzoffs * xzoffs);
				MyVec3 realDire = dire + xzoffs * glossyReflectOffset * direXZ + yoffs * glossyReflectOffset * direY;
				MyVec3::normalize(realDire);
				Ray reflect(result.intersectPoint + DEVIATION * realDire, realDire);
				IntersectResult resultNext;
				Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime * 0.25, sampleDown * 4.0);
				re = re + reflectiveness * got * ColorTexture;
			}
		}
#endif
		else{
			MyVec3 dire = ray.direction - 2 * MyVec3::dot(ray.direction, result.normalVec) * result.normalVec;
			//	move origin towards direction a little distance
			//	in order to figure out right intersection
			Ray reflect(result.intersectPoint + DEVIATION * dire, dire);
			IntersectResult resultNext;
			#ifdef SAMPLEWAY
			Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime, sampleDown);
			#else
			Color got = rayTraceRecursive(reflect, resultNext, recursiveDepth + 1, irefIndex, sampleTime * 0.25, sampleDown * 4.0);
			#endif
			re = re + reflectiveness * got * ColorTexture;
		}
	}	

	//	calculate refraction
	double refraction = result.primi->material()->getRefraction();
	if (recursiveDepth < TRACEMAXRECUR && refraction > EPS){
		double tIndex = result.ma->getRefraIndex();
		if(in_out == -1) tIndex = 1;
		double n = irefIndex / tIndex;
		MyVec3 N = in_out * result.normalVec;
		double cosI = -MyVec3::dot(N, ray.direction );
		double cosT = 1.0 - n * n * (1.0 - cosI * cosI);		
		if (cosT > 0.0 && cosT < 1){
			MyVec3 T = (n * ray.direction) + (n * cosI - sqrt(cosT)) * N;
			IntersectResult resultNext;
			#ifdef SAMPLEWAY
			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * T, T), resultNext, recursiveDepth + 1, tIndex, sampleTime, sampleDown);
			#else
			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * T, T), resultNext, recursiveDepth + 1, tIndex, sampleTime * 0.25, sampleDown * 4.0);
			#endif
			// apply Beer's law
			//#ifndef TEXTURE
			//Color absorbance = -resultNext.distance * 0.15 * result.primi->material()->color;	
			////cout << absorbance << endl;
			//#else
			//Color absorbance;
			//if (result.primi->material()->texExist)
			//	absorbance = -resultNext.distance * 0.15 * result.primi->material()->getTexc(result.primi, result.intersectPoint);					
			//else
			//	absorbance = -resultNext.distance * 0.15 * result.primi->material()->color;					
			//#endif
			//Color transparency = Color(exp(absorbance.r), exp(absorbance.g), exp(absorbance.b));				
			//re = re + got * transparency;		
			re = re + got;
		}
	}

	return re;
}
	




	const Color RayTracer::rayTraceRecursive(const Ray& ray, IntersectResult& result, int recursiveDepth, double irefIndex, int E){
		//cout << "haha"<< endl;
		double in_out = findNearest(ray, result);
		if (in_out == 0)
			return BLACK;
		MyVec3 nl = in_out * result.normalVec;
		Color color = result.primi->material()->color;
		
		const Color& e = result.primi->material()->getEmission();
		//cout << e.r << " "<<e.g << " "<<e.b << endl;

		double p = (color.r > color.g && color.r > color.b) ? color.r : (color.g > color.b ? color.g : color.b);
		if (recursiveDepth + 1 > TRACEMAXRECUR || p == 0){
			double ans = Config::rand01();
			if (ans < p){
				color = (1.0 / p) * color;
				//cout << ans << " " << p << endl;
			}	
			else
				return E * e;
		}
		IntersectResult resultNext;
		//cout << result.primi->type << endl;
		switch(result.primi->material()->getBRDFType()){
		case DIFF:	{
			double r1 = 2 * PI * Config::rand01();
			double r2 = Config::rand01();
			double r2s = sqrt(r2);
			MyVec3 w = nl;
			MyVec3 u = MyVec3::cross(fabs(w.x) > 0.1 ? MyVec3(0, 1, 0) : MyVec3(1, 0, 0), w);
			MyVec3::normalize(u);
			MyVec3 v = MyVec3::cross(w, u);
			MyVec3 d = (cos(r1) * r2s * u + sin(r1) * r2s * v + sqrt(1 - r2) * w);
			MyVec3::normalize(d);
			Color el;
			for (auto& it : scene.plight_vec){
				Sphere& po = it->get();
				const MyVec3& sp = po.getCenter();  
				MyVec3 sw = sp - result.intersectPoint;
				MyVec3 su = MyVec3::cross((fabs(sw.x) > .1 ? MyVec3(0, 1, 0) : MyVec3(1, 0, 0)), sw);
				MyVec3::normalize(su);
				MyVec3 sv = MyVec3::cross(sw, su);
				double cos_a_max = sqrt(1 - po.R_2 / MyVec3::dot(result.intersectPoint - sp, result.intersectPoint - sp));
				double eps1 = Config::rand01(), eps2 = Config::rand01();
				double cos_a = 1 - eps1 + eps1 * cos_a_max;
				double sin_a = sqrt(1 - cos_a * cos_a);
				double phi = 2 * PI * eps2;
				MyVec3 L = cos(phi) * sin_a * su + sin(phi) * sin_a * sv + cos_a * sw;
				MyVec3::normalize(L);
				Ray shadowRay(result.intersectPoint + DEVIATION * L, L);
				IntersectResult temp;
				findNearest(shadowRay, temp);	

				if (temp.primi != it)
					continue;
				double omega = 2 * PI * (1 - cos_a_max);
				el = el + (PI_DOWN * omega * MyVec3::dot(L, nl)) * (color * (po.material()->getEmission()));
			}
			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * d, d), resultNext, recursiveDepth + 1, irefIndex, 0);
			return E * e + el + color * got;
			}
		case SPEC:{
			MyVec3 nd = ray.direction - 2.0 * MyVec3::dot(ray.direction, result.normalVec) * result.normalVec;
			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * nd, nd), resultNext, recursiveDepth + 1, irefIndex);			
			return e + color * got;
			}
		}

		MyVec3 nd = ray.direction - 2.0 * MyVec3::dot(ray.direction, result.normalVec) * result.normalVec;
		Ray reflRay(result.intersectPoint + DEVIATION * nd, nd);
		//double tIndex = result.primi->material()->getRefraIndex();
		//double nnt = ((in_out == 1) ? 1.0 / tIndex : irefIndex / 1.0);
		double tIndex = result.ma->getRefraIndex();
		if(in_out == -1) tIndex = 1;
		double nnt = irefIndex / tIndex;
		double ddn = MyVec3::dot(ray.direction, nl), cos2t;
		if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection 
			return e + color * rayTraceRecursive(reflRay, resultNext, recursiveDepth + 1, irefIndex); 
		
		MyVec3 tdir = (nnt * ray.direction - (in_out * (ddn * nnt + sqrt(cos2t))) * result.normalVec);
		MyVec3::normalize(tdir); 

		double a = tIndex - 1.0;
		double b = tIndex + 1.0;
		double c = 1 - (in_out == 1 ? -ddn : MyVec3::dot(tdir, result.normalVec));   
		double R0 = a * a / (b * b);
		double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re;
		double P = 0.25 + 0.5 * Re, RP = Re / P, TP = Tr / (1.0 - P); 
		IntersectResult resultNext1;
		return e + color * (recursiveDepth > 1 
			? 
			(Config::rand01() < P ? 
				RP * rayTraceRecursive(reflRay, resultNext, recursiveDepth + 1, irefIndex) : 
				TP * rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * tdir, tdir), resultNext, recursiveDepth + 1, tIndex)) 
			: 
			Re * rayTraceRecursive(reflRay, resultNext, recursiveDepth + 1, irefIndex) + 
			Tr * rayTraceRecursive(Ray(result.intersectPoint + DEVIATION * tdir, tdir), resultNext1, recursiveDepth + 1, tIndex)); 
	}

	void RayTracer::render(MyMat& img, Camera& camera,int lo,int hi)
	{
		int n = 600 / (hi - lo);
		double rows = img.getRow();
		double cols = img.getCol();	
		const int samps = PATH_SAMPLE;
		for (double r = rows - 1; r >= 0; r--){
		//for (double r = 0; r < rows; r++){
			
			int rr = r;img.savePicture(tmpname[lo*n/600].c_str());
			fprintf(stderr,"\rRendering %d (%d spp) %5.2f%%",rr,int(samps/2),100.*r/(rows-1));  
			
			for (double c = 0; c < cols; c++){
#ifdef LHY_PINTU
				if(c < loCol || c > hiCol) continue;
#endif

#ifdef DUO
				
				if(c < lo || c >= hi) continue;
			//	cout << "haha" << endl;
#endif
				Color color;
				for (int sr = 0; sr < 2; sr++)
					for (int sc = 0; sc < 2; sc++){
					//	cout << "asdf"<<endl;
						Color temp;
						for (int i = 0; i < samps; i++){
							double r1 = 2 * Config::rand01(), dc = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1); 
							double r2 = 2 * Config::rand01(), dr = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2); 				
							double v = 1 - (r + (sr + .5 + dr) / 2) / rows;
							double u = 1 - (c + (sc + .5 + dc) / 2) / cols;							
							Ray ray;						
							camera.emit(u
								, v
								, ray, 0);	
							IntersectResult result;
							temp = temp + (1. / samps) * rayTraceRecursive(ray, result, 0, 1.0); 
							
						}
						color = color + temp.clampColor();
					}
				color = 0.25 * color;
				//img.pixel(r, c) = color.imgcolor();
				img.setPixel(r,c,color.imgcolor());
				//cout << color << endl;
				for (int i = 0; i < 3; i++)
					img.pixel(r, c)[i] = pow(img.pixel(r, c)[i], 1.0 / 2.2);
			}
			//img.show("fifth");
			//waitKey(10);
		}
		img.savePicture(name[lo*n/600].c_str());
		
	}





int RayTracer::nearest(Primitive* it,const Ray& ray, IntersectResult& result, int& re){
	//int in_out = 0;
	//if (it->getLastRay() != ray.getID()){
	//	IntersectResult temp;
	//	if (in_out = it->intersect(ray, temp))
	//		if (temp.distance < result.distance){
	//			result = temp;
	//			re = in_out;
	//		}													
	//}
	//return in_out;

	int in_out = 0;
	if (it->getLastRay() != ray.getID()){
		if (in_out = it->intersect(ray, result)) //如果光线和物体相交
			re = in_out;													
	}
	return in_out;
}


void RayTracer::addPrimiToBox(Primitive* primi, const MyVec3& loBound, const MyVec3& hiBound){

	AABB bigBOX = primi->getAABB();
	int x1 = (bigBOX.getPos().x - loBound.x) / unit.x;
	int x2 = (bigBOX.getPos_2().x - loBound.x) / unit.x + 1;
	x1 = x1 < 0 ? 0 : x1;
	x2 = x2 > Config::GRIDSIZE ? Config::GRIDSIZE : x2;
	int y1 = (bigBOX.getPos().y - loBound.y) / unit.y;
	int y2 = (bigBOX.getPos_2().y - loBound.y) / unit.y + 1;
	y1 = y1 < 0 ? 0 : y1;
	y2 = y2 > Config::GRIDSIZE ? Config::GRIDSIZE : y2;
	int z1 = (bigBOX.getPos().z - loBound.z) / unit.z;
	int z2 = (bigBOX.getPos_2().z - loBound.z) / unit.z + 1;

	z1 = z1 < 0 ? 0 : z1;
	z2 = z2 > Config::GRIDSIZE ? Config::GRIDSIZE : z2;
	
	//cout << x1 << x2 << y1 << y2 << z1 << z2 << endl;
	for (int x = x1; x < x2; x++)
		for (int y = y1; y < y2; y++)
			for (int z = z1; z < z2; z++){
				MyVec3 l(loBound.x + x * unit.x, loBound.y + y * unit.y, loBound.z + z * unit.z);
				AABB cell(l, unit);
				//cout << primi->getType() << endl;
				if (primi->isIntersectWithBox(cell)){						
					allGrids[x][y][z].push_back(primi);
				}

			}	
}



double RayTracer::calSoftShade(BoxLight* light, MyVec3 ip, MyVec3& L, const MyObj& scene, double sampleTime, double sampleDown){
	double re = 0;
	L = light->get().getPos() + 0.5 * light->get().getSize() - ip;
	MyVec3::normalize(L);
	double deltaX = (light->get()).getSize().x * SOFT_SHA_GRID_BIAN_DOWN	;
	double deltaY = (light->get()).getSize().y * SOFT_SHA_GRID_BIAN_DOWN;
	for (int i = 0; i < sampleTime; i++){
		MyVec3 lightPoint(light->getSampleGridX(i) + Config::myRand01() * deltaX,
			light->get().getPos().y, light->getSampleGridY(i) + Config::myRand01() * deltaY);
		MyVec3 dire = lightPoint - ip;
		MyVec3::normalize(dire);
		IntersectResult temp;
		if (findNearest(Ray(ip + DEVIANCE * dire, dire),temp) && (temp.primi == light) )
			re += sampleDown;
	}
	return re;
}

void RayTracer::addTexture(MyObj& scene){
	for (auto& it : scene.sphere_vec){
		it->material()->SetTexture( new Texture( "textures/marble.tga" ) );
		it->material()->SetUVScale( 0.8, 0.8 );
	}

	for (auto& it : scene.plane_vec){
		it->material()->SetTexture( new Texture( "textures/wood.tga" ) );
		it->material()->SetUVScale(0.05, 0.05 );
	}
		
}


void RayTracer::readScene(char* file, MyObj& scene){
	 ifstream in;
	 in.open(file, ifstream::in);
	
	
	string type;

   in >> type;
  cout << type << endl;

   while (in.good()) {
	  if (type[0] == '#' ) {
		 getline(in, type);
		
	  } 
	  else if (type.compare("sphere") == 0) {
		 MyVec3 center;
		 double radius;
		 

		 in >> center.x >> center.y >> center.z;
		 
		 in >> radius;
		 center.z = 7 - radius;
		 center.x /= 10;
		 center.y /= 10;
		 radius /= 10;

		 string type;
		 in >> type;
		 double r,g,b,shiness, refla;
		 in >> r >> g >> b >> shiness >> refla;
		// cout << refla << endl;
		// cout << center.x <<" ";
		//if(rand() * 1.0 / RAND_MAX > 0.4)
			scene.sphere_vec.push_back(new Sphere(center, radius,new Phong(Color(r,g,b), 0, 0, 20, refla, 3.5, 1.5)));
	  } 
	  else if (type.compare("light") == 0) {
		 MyVec3 position;
		 double intensity;

		 in >> position.x >> position.y >> position.z;
		 in >> intensity;

		 scene.plight_vec.push_back(new PointLight(position, 0.2, Color(intensity, intensity, intensity)));

	  }  else {
		 cerr << "Type not found: " << type << endl;
		 exit(EXIT_FAILURE);
	  }
	  in >> type;
	  in >> type;
   }

}

MyVec3 objPoint[MAX_VERTEX_NUM];
//#define HORSE
#define DRAGON
//#define CUBE
void RayTracer::readObj(const char* file, MyObj& scene, MyVec3 O, MyVec3 size, MyVec3 angles,Material* material){
	angles *= (PI / 180);

	ifstream fin(file);
	int now = 0;
	cout << "***" << endl;
	while (! fin.eof()){
		char ch;
		fin >> ch;
		if (fin.eof())
			break;		
		if (ch == '#'){	//	annotation
			fin >> noskipws;
			while (ch != '\n') fin >> ch;
			fin >> skipws;
		}
		else {	
			//cout << now << endl;
			if (ch == 'v'){	//	vertex
				now++;
				fin >> objPoint[now].x >> objPoint[now].y >> objPoint[now].z;
#ifdef CUBE
				double beishu = 10;
				 objPoint[now].x = objPoint[now].x / beishu + 0.8;
				 objPoint[now].y = objPoint[now].y / beishu ;
				 objPoint[now].z = objPoint[now].z / beishu;
#endif
#ifdef HORSE
				double beishu = 5;
				 objPoint[now].x = objPoint[now].x * beishu;
				 objPoint[now].y = objPoint[now].y * beishu; ;
				 objPoint[now].z = objPoint[now].z * beishu + 5;;
#endif
#ifdef DRAGON
				 //double beishu = 1;
				 //objPoint[now].x = objPoint[now].x * beishu;
				 //objPoint[now].y = objPoint[now].y * beishu - 2; ;
				 //objPoint[now].z = objPoint[now].z * beishu + 7;

				objPoint[now] = objPoint[now].Rotate(MyVec3(1, 0, 0), angles[0]);
				objPoint[now] = objPoint[now].Rotate(MyVec3(0, 1, 0), angles[1]);
				objPoint[now] = objPoint[now].Rotate(MyVec3(0, 0, 1), angles[2]);
				objPoint[now] = O + objPoint[now] * size;

#endif
				// cout << objPoint[now] << endl;
			}
			else if (ch == 'f'){	//	face			
				int a, b, c;
				fin >> a >> b >> c;

				//MyVec3 fxl = MyVec3::cross(objPoint[b] - objPoint[a], objPoint[a] - objPoint[c]);
				MyVec3 fxl = MyVec3::cross(objPoint[c] - objPoint[a], objPoint[a] - objPoint[b]);
				
				if(fxl.module_2() < EPS * EPS)
					continue;

				MyVec3::normalize(fxl);

				//cout << "ok" << endl;

				//Triangle* p = new Triangle(objPoint[a], objPoint[b], objPoint[c], fxl, new BRDF(Color(.75, .75, .75), BLACK, DIFF));
				Triangle* p = new Triangle(objPoint[a], objPoint[b], objPoint[c], fxl, material);
				scene.tri_vec.push_back(p);	
			}
		}
	}
	
	TriangleTree* tree = new TriangleTree();
	TriangleNode* root = tree->GetRoot();
	root->size = scene.tri_vec.size();
	root->tris = new Triangle*[root->size];
	for(int i = 0;i < root->size;i++){
		root->tris[i] = scene.tri_vec[i];
		root->box.Update(scene.tri_vec[i]);
	}
	tree->BuildTree();
	scene.poly_vec.push_back(tree);
	scene.tri_vec.clear();
	
	fin.close();
}



