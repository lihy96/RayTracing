#include "rayTracer.h"



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

	for(auto& it : scene.blight_vec)
		addPrimiToBox(it, loBound, hiBound);

}


#define SUPERSAM
void RayTracer::run(MyMat& m, Camera& camera, MyObj& myobj){
	
	initGrid(myobj);

	int rows = m.getRow(), cols = m.getCol();

#ifndef SUPERSAM
	for(int c = 0;c < cols;c++){
		double u = 1 - ((double)c) / cols;
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //屏的左下角是（0,0），右上角是（1,1）
			Ray ray = camera.emit(u,v);// 向转化过来的点发出一条光
			Vec3d color = rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0).getImgColor();
			m.setPixel(r, c, color);
		}
		
	}
#endif

#ifdef SUPERSAM

	double pixel_width = 1.0 / cols;
	double pixel_height = 1.0 / rows;
	double pixel_width_half = 0.5 * pixel_width;
	double pixel_height_half = 0.5 * pixel_height;

	double sample_width = pixel_width / SUPER_SAMPLES;
	double sample_height = pixel_height / SUPER_SAMPLES;

	double SUPER_SAMPLES_2_DOWN = (1.0) / ( (SUPER_SAMPLES  + 1) * (SUPER_SAMPLES + 1) );

	for(int c = 0;c < cols;c++){
		double u = 1 - ((double)c) / cols;
		//printf("%.2lf \%\n",(1-u)*100);
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //屏的左下角是（0,0），右上角是（1,1）

			double sampleStartU = u - pixel_width_half;
			double sampleStartV = v - pixel_height_half;


			Vec3d color(0,0,0);
			for(int x = 0; x <= SUPER_SAMPLES; x++){
				for(int y = 0;y <= SUPER_SAMPLES; y++){
					Ray ray = camera.emit(sampleStartU + x * sample_width, sampleStartV + y * sample_height);
					color += SUPER_SAMPLES_2_DOWN * rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0).getImgColor();
				}
			}
			m.setPixel(r, c, color);
		}

		//m.show("f");
		//waitKey(5);
	}
#endif



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



int RayTracer::findNearest(const Ray& ray, const MyObj& scene, IntersectResult& result){
	//	X/Y/Z: current coordinate of grid 
	
	int gridSize = Config::GRIDSIZE;

	int X = (ray.getOri().x - wholeSpace.getPos().x) / unit.x;
	int Y = (ray.getOri().y - wholeSpace.getPos().y) / unit.y;
	int Z = (ray.getOri().z - wholeSpace.getPos().z) / unit.z;	
	
	//	the source of ray is out of grid
	if ((X < 0) || (X >= gridSize)
		|| (Y < 0) || (Y >= gridSize)
		|| (Z < 0) || (Z >= gridSize))
		return MISS;
		
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

	//	find the first intersection
	int re = MISS;
	while (true){
		for (auto& it : allGrids[X][Y][Z].sphere_vec){	
			if (nearest(it, ray, scene, result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].plane_vec){
			if (nearest(it, ray, scene, result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].plight_vec){

			if (nearest(it, ray, scene, result, re)){
				goto closer;
			}
		}
		for (auto& it : allGrids[X][Y][Z].blight_vec){
			if (nearest(it, ray, scene, result, re)){
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
			nearest(it, ray, scene, result, re);
		for (auto& it : allGrids[X][Y][Z].plane_vec)
			nearest(it, ray, scene, result, re);
		for (auto& it : allGrids[X][Y][Z].plight_vec)
			nearest(it, ray, scene, result, re);
		for (auto& it : allGrids[X][Y][Z].blight_vec)
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


	}

	

	//漫反射和镜面反射
	for (auto& it : scene.plight_vec){
		MyVec3 L = it->get().getCenter() - result.intersectPoint;	//表面交点到光源的向量	
		double Ldist = L.module();	//表面到光源的距离
		MyVec3::normalize(L);	
	
		//判断光源是否能照到表面交点
		Ray shadowRay(result.intersectPoint + DEVIANCE * L, L); //从表面向光源发射一条光线
		IntersectResult temp;
		findNearest(shadowRay, scene, temp);
		if(temp.primi != it)
			continue;
		/*int shadowFlag = 0;
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
			continue;*/

		//光源可以照到表面交点处，利用phong模型计算
		//注意这里其实传入表面相交点并没有用
		re = re + result.ma->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor(),ColorTexture);	


	}


	for (auto& it : scene.blight_vec){
		MyVec3 L;
		double shadow = calSoftShade(it, result.intersectPoint, L, scene);
		if (shadow > 0)
			re = re + shadow * result.primi->material()->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor());			
	}
		
	// 计算反射的情况
	double reflectiveness = result.ma->getReflectiveness();
	if (recursiveDepth > 0 && reflectiveness > 0){
		double diffReflectOffset = result.primi->material()->difflection;
		//	only perform diffuse reflection for primary rays; otherwise it's just too slow.
		if ((diffReflectOffset > 0) && (recursiveDepth == MAX_RECURSIVE_DEEPTH)){
			MyVec3 dire = ray.getDir() - 2.0 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
			//	direXZ and dire are vertical crossing in xz plane
			//MyVec3 direXZ(dire.z, dire.y, -dire.x);
			MyVec3 direXZ(dire.z, 0, -dire.x);
			MyVec3 direY = MyVec3::cross(dire, direXZ);
			reflectiveness *= SAMPLES_DOWN;
			for (int i = 0; i < SAMPLES; i++){
				double xzoffs, yoffs;
				do
				{
					xzoffs = Config::myRand_11();
					yoffs = Config::myRand_11();
				}
				while ((xzoffs * xzoffs + yoffs * yoffs) > 1);
				MyVec3 realDire = dire + xzoffs * diffReflectOffset * direXZ + yoffs * diffReflectOffset * direY;
				MyVec3::normalize(realDire);
				Ray reflect(result.intersectPoint + DEVIANCE * realDire, realDire);
				Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex);
				//re = re + reflectiveness * got * result.primi->material()->getColor();
				re = re + reflectiveness * got * ColorTexture;
			}
	}
	else{
		MyVec3 dire = ray.getDir() - 2 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
		//	move origin towards direction a little distance
		//	in order to figure out right intersection
		Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
		Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex);
		//re = re + reflectiveness * got * result.primi->material()->getColor();
		re = re + reflectiveness * got * ColorTexture;
	}
}


	/*if (recursiveDepth > 0 && reflectiveness > 0) {
		MyVec3 dire = ray.getDir() - 2 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
		Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
		Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex);
		re = re + reflectiveness * got * result.ma->getColor();
	}	*/

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


int RayTracer::nearest(Primitive* it,const Ray& ray, const MyObj& scene, IntersectResult& result, int& re){
	int in_out = 0;
	if (it->getLastRay() != ray.getID()){
		IntersectResult temp;
		if (in_out = it->intersect(ray, temp))
			if (temp.distance < result.distance){
				result = temp;
				re = in_out;
			}													
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
				//cout << l << endl;
				if (primi->isIntersectWithBox(cell)){						
					allGrids[x][y][z].push_back(primi);
				}

			}	
}



double RayTracer::calSoftShade(BoxLight* light, MyVec3 ip, MyVec3& L, const MyObj& scene){
	double re = 0;
	L = light->get().getPos() + 0.5 * light->get().getSize() - ip;
	MyVec3::normalize(L);
	double deltaX = (light->get()).getSize().x * SOFT_SHA_GRID_BIAN_DOWN	;
	double deltaY = (light->get()).getSize().y * SOFT_SHA_GRID_BIAN_DOWN;
	for (int i = 0; i < SAMPLES; i++){
		MyVec3 lightPoint(light->getSampleGridX(i) + Config::myRand01() * deltaX,
			light->get().getPos().y, light->getSampleGridY(i) + Config::myRand01() * deltaY);
		MyVec3 dire = lightPoint - ip;
		MyVec3::normalize(dire);
		IntersectResult temp;
		if (findNearest(Ray(ip + DEVIANCE * dire, dire), scene, temp) && (temp.primi == light) )
			re += SAMPLES_DOWN;
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
