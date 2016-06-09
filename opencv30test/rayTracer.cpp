#include "rayTracer.h"

void RayTracer::initGrid(const MyObj& scene){
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


#define SUPERSAM
//#define jingshen
//#define KDTREE
#define SAMPLEWAY
void RayTracer::run(MyMat& m, Camera& camera, MyObj& myobj){

#ifdef KDTREE
	std::vector<Primitive*> objects;
	objects.clear();
	for (auto& it :myobj.sphere_vec){	
		objects.push_back(it);
	}
	for (auto& it : myobj.plane_vec){
		objects.push_back(it);
	}
	for (auto& it :myobj.plight_vec){
		objects.push_back(it);
	}
	for (auto& it : myobj.blight_vec){
		objects.push_back(it);
	}
	for (auto& it : myobj.tri_vec){
		objects.push_back(it);
	}
	root = new KdTreeNode(0, 0, objects);
#endif


	initGrid(myobj);

	int rows = m.getRow(), cols = m.getCol();

#ifndef SUPERSAM
	for(int c = 0;c < cols;c++){
		double u = 1 - ((double)c) / cols;
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //�������½��ǣ�0,0�������Ͻ��ǣ�1,1��

			Vec3d color(0,0,0);

			//for(int i = 0;i < DEEP_OF_FIELD_NUM;i++){	
			//	double disturbX = rand() * DISTURB / RAND_MAX;
			//	double disturbY = rand() * DISTURB / RAND_MAX;
			//	Ray ray = camera.emit(u + disturbX, v + disturbY);
			//	color += DEEP_OF_FIELD_NUM_DOWN *  rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0).getImgColor();
			//}
			//Ray ray = camera.emit(u,v);// ��ת�������ĵ㷢��һ����
			MyVec3 vec(1- 2 * u + camera.eye.x,  2 * v - 1 + camera.eye.y ,1 + camera.eye.z);
			Ray ray = camera.emit(vec);
			//cout << ray << endl << camera.emit(u,v)<< endl << endl;
			color = rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0, SAMPLES, SAMPLES_DOWN).getImgColor();
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
		double u = 1 - ((double)c) / cols;
		//printf("%.2lf \%\n",(1-u)*100);
		for(int r = 0;r < rows; r++){
			double v = 1 - ((double)r) / rows; //�������½��ǣ�0,0�������Ͻ��ǣ�1,1��

			double sampleStartU = u - pixel_width_half + sample_width_half;
			double sampleStartV = v - pixel_height_half + sample_height_half;


			Vec3d color(0,0,0);
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
						color += DEEP_OF_FIELD_NUM_DOWN * SUPER_SAMPLES_2_DOWN * rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0, SAMPLES, SAMPLES_DOWN).getImgColor();
					}
#else
					Ray ray = camera.emit(sampleStartU + x * sample_width, sampleStartV + y * sample_height);
					color += SUPER_SAMPLES_2_DOWN * rayTraceRecursive(ray, myobj, MAX_RECURSIVE_DEEPTH, 1.0, SAMPLES, SAMPLES_DOWN).getImgColor();
#endif
				}
			}
			m.setPixel(r, c, color);
			//m.setPixel(r ,c,  Color(0.7, 0.3, 0.4));
		}

		m.show("super");
		waitKey(5);
	}
#endif



}

//�ҵ����������Ľ���
//û�н��㷵��0���������ཻ������ǹ�Դ�򷵻�-1������Ƿǹ�Դ�������򷵻�1��
int RayTracer::findNearestNoAABB(const Ray& ray, const MyObj& scene, IntersectResult& result){
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

	if(result.distance == INF)//û����ӵ����巵��0
		return 0;
	else //�ཻ�˵Ļ���ӳ���ڲ������ⲿ
		return in_out;
}



int RayTracer::findNearest(const Ray& ray, const MyObj& scene, IntersectResult& result){
	//	X/Y/Z: current coordinate of grid 
	
	int gridSize = Config::GRIDSIZE;

	int X = (ray.getOri().x - wholeSpace.getPos().x) / unit.x;
	int Y = (ray.getOri().y - wholeSpace.getPos().y) / unit.y;
	int Z = (ray.getOri().z - wholeSpace.getPos().z) / unit.z;	
	
	//	the source of ray is out of grid
	// �������������һ����������ģ����Է���㲻�ں�����Ĺ��߲��迼��
	if ((X < 0) || (X >= gridSize)
		|| (Y < 0) || (Y >= gridSize)
		|| (Z < 0) || (Z >= gridSize))
		return MISS;
		
	//	step: the increasing delta
	//	out: the stop boundary		
	//	nextCoordinate: the next coordiante along ray direction	
	int stepX, outX, stepY, outY, stepZ, outZ;	
	MyVec3 nextCoordinate;
	
	//���ں����Ǵ�[0,gridsize)������out�ֱ�Ϊgridsize and -1;
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
		for (auto& it : allGrids[X][Y][Z].tri_vec){
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
		for (auto& it : allGrids[X][Y][Z].tri_vec)
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

const Color RayTracer::rayTraceRecursive(const Ray& ray, const MyObj& scene, int recursiveDepth, double irefIndex,double sampleTime,double sampleDown){
	
	Color re = BLACK;
	IntersectResult	result;
	result.distance = INF;
	double in_out = findNearest(ray, scene, result);
	//cout << result.distance << endl;
	
	//cout << "haha" << endl;
	//int aa[10];
	//for(int i = 0;i < 10;i++) aa[i] = i;

	Color ColorTexture;

	if(in_out == 0)	//����δ���κ������ཻ
		return BLACK;
	else{	//������ĳһ�������ཻ
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
	
	//if(result.primi->getType()==Config::TRI_TYPE) {
	//cout << result;

	//}
	
	//cout << result << endl;
		
	
	//������;��淴��
	for (auto& it : scene.plight_vec){
		

		MyVec3 L = it->get().getCenter() - result.intersectPoint;	//���潻�㵽��Դ������	
		double Ldist = L.module();	//���浽��Դ�ľ���
		MyVec3::normalize(L);	
	
		//�жϹ�Դ�Ƿ����յ����潻��
		Ray shadowRay(result.intersectPoint + DEVIANCE * L, L); //�ӱ������Դ����һ������
		IntersectResult temp;
		findNearest(shadowRay, scene, temp);
		if(temp.primi != it)
			continue;

		//��Դ�����յ����潻�㴦������phongģ�ͼ���
		//ע��������ʵ��������ཻ�㲢û����
		
		re = re + result.ma->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor(),ColorTexture);	

		

	}

	
	
	for (auto& it : scene.blight_vec){
		MyVec3 L;
		double shadow = calSoftShade(it, result.intersectPoint, L, scene,sampleTime, sampleDown);
		if (shadow > 0)
			re = re + shadow * result.primi->material()->sample(ray.getDir(), L, result.normalVec, result.intersectPoint, it->getColor());			
	}
		
	// ���㷴������
	double reflectiveness = result.ma->getReflectiveness();
	if (recursiveDepth > 0 && reflectiveness > 0){
		double diffReflectOffset = result.primi->material()->difflection;
		//	only perform diffuse reflection for primary rays; otherwise it's just too slow.
#ifdef SAMPLEWAY
		if ((diffReflectOffset > 0) && (recursiveDepth == MAX_RECURSIVE_DEEPTH)){
			MyVec3 dire = ray.getDir() - 2.0 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
			//	direXZ and dire are vertical crossing in xz plane
			//MyVec3 direXZ(dire.z, dire.y, -dire.x);
			MyVec3 direXZ(dire.z, 0, -dire.x);
			MyVec3 direY = MyVec3::cross(dire, direXZ);
			reflectiveness *= SAMPLES_DOWN;
			for (int i = 0; i < SAMPLES; i++){
				double xzoffs = Config::myRandhalf();
				double yoffs = Config::myRand01() * (0.5 - xzoffs * xzoffs);
				MyVec3 realDire = dire + xzoffs * diffReflectOffset * direXZ + yoffs * diffReflectOffset * direY;
				MyVec3::normalize(realDire);
				Ray reflect(result.intersectPoint + DEVIANCE * realDire, realDire);
				Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex,sampleTime, sampleDown);
				//re = re + reflectiveness * got * result.primi->material()->getColor();
				re = re + reflectiveness * got * ColorTexture;
			}
		}
#else
		if ((diffReflectOffset > 0) && (recursiveDepth > MAX_RECURSIVE_DEEPTH - 2)){
			MyVec3 dire = ray.getDir() - 2.0 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
			//	direXZ and dire are vertical crossing in xz plane
			//MyVec3 direXZ(dire.z, dire.y, -dire.x);
			MyVec3 direXZ(dire.z, 0, -dire.x);
			MyVec3 direY = MyVec3::cross(dire, direXZ);
			reflectiveness *= sampleDown;
			for (int i = 0; i < sampleTime; i++){
				double xzoffs = Config::myRand01();
				double yoffs = Config::myRand01() * (1 - xzoffs * xzoffs);
				MyVec3 realDire = dire + xzoffs * diffReflectOffset * direXZ + yoffs * diffReflectOffset * direY;
				MyVec3::normalize(realDire);
				Ray reflect(result.intersectPoint + DEVIANCE * realDire, realDire);
				Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex,sampleTime * 0.25, sampleDown * 4.0);
				//re = re + reflectiveness * got * result.primi->material()->getColor();
				re = re + reflectiveness * got * ColorTexture;
			}
		}
#endif
		else{
			MyVec3 dire = ray.getDir() - 2 * MyVec3::dot(ray.getDir(), result.normalVec) * result.normalVec;
			//	move origin towards direction a little distance
			//	in order to figure out right intersection
			Ray reflect(result.intersectPoint + DEVIANCE * dire, dire);
#ifdef SAMPLEWAY
			Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex, sampleTime, sampleDown);
#else
			Color got = rayTraceRecursive(reflect, scene, recursiveDepth - 1, irefIndex, sampleTime * 0.25, sampleDown*4);
#endif
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
#ifdef SAMPLEWAY
			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIANCE * T, T), scene, recursiveDepth - 1, tIndex, sampleTime, sampleDown);
#else
			Color got = rayTraceRecursive(Ray(result.intersectPoint + DEVIANCE * T, T), scene, recursiveDepth - 1, tIndex, sampleTime * 0.25, sampleDown * 4.0);
#endif
			// apply Beer's law
			Color absorbance = -result.distance * 0.15 * result.ma->getColor();				
			Color transparency = Color(exp(absorbance.r()), exp(absorbance.g()), exp(absorbance.b()));				
			re = re + got * transparency;
		}
	}
	return re;
}


int RayTracer::nearest(Primitive* it,const Ray& ray, const MyObj& scene, IntersectResult& result, int& re){
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
		if (in_out = it->intersect(ray, result)) //������ߺ������ཻ
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
		if (findNearest(Ray(ip + DEVIANCE * dire, dire), scene, temp) && (temp.primi == light) )
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
		if(rand() * 1.0 / RAND_MAX > 0.4)
			scene.sphere_vec.push_back(new Sphere(center, radius,new Phong(Color(r,g,b), 0.1, 0.8, shiness, refla, 0.1, 1.5)));
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
void RayTracer::readObj(char* file, MyObj& scene){
	
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
				double beishu = 2;
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
				 double beishu = 3.3;
				 objPoint[now].x = objPoint[now].x * beishu;
				 objPoint[now].y = objPoint[now].y * beishu-2; ;
				 objPoint[now].z = objPoint[now].z * beishu + 7;

#endif
				// cout << objPoint[now] << endl;
			}
			else if (ch == 'f'){	//	face			
				int a, b, c;
				fin >> a >> b >> c;
			//	Vec3 fxl = cross(obj[b] - obj[a], obj[a] - obj[c]);
				//MyVec3 fxl = MyVec3::cross(objPoint[b] - objPoint[a], objPoint[a] - objPoint[c]);
				MyVec3 fxl = MyVec3::cross(objPoint[c] - objPoint[a], objPoint[a] - objPoint[b]);
				
				if(fxl.module_2() < EPS * EPS)
					continue;

				MyVec3::normalize(fxl);	
				Triangle* p = new Triangle(objPoint[a], objPoint[b], objPoint[c], fxl, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 0));
				//p->material()->SetTexture( new Texture( "textures/wood.tga" ) );
				//p->material()->SetUVScale( 0.05, 0.05 );
				scene.tri_vec.push_back(p);	
			
			}
		}
	}
	cout << "----" << endl;
	fin.close();
	//cout << now <<  endl;
}



