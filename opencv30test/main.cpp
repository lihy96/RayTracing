#include <iostream>
#include <ctime>
#include "rayTracer.h"
#include "debug.h"
#define DEBUG

int main(int argc, char** argv){
	freopen("lhy1.txt","w",stdout);

	MyMat m(600,600);
	RayTracer* rayTracer = new RayTracer();
	MyObj scene;

	scene.plight_vec.push_back(new PointLight(MyVec3(0, 5, 5), 0.1, Color(0.4, 0.4, 0.4)));
	scene.plight_vec.push_back(new PointLight(MyVec3(-3, 5, 1), 0.1, Color(0.6, 0.6, 0.6)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 4.4, new Phong(Color(0.4, 0.3, 0.3), 1, 0, 0, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0.4, 0, -1), 12, new Phong(Color(0.5, 0.3, 0.5), 0.6, 0, 0, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 7.4, new Phong(Color(0.4, 0.7, 0.7), 0.5, 0, 0, 0, 0, 1.5)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5, Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0, 0.8, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-5.5, -0.5, 7), 2, Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-1.5, -3.8, 1), 1.5, Phong(Color(1, 0.4, 0.4), 0.1, 0.8, 20, 0, 0.8, 1.5)));
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 7; y++){
			scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3 + y * 1.5, 10), 0.3, Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));	
		}

	Camera camera(MyVec3(0, 0, -3), MyVec3(0, 0, 1), MyVec3(0, 1, 0), 90);
	rayTracer->run(m,camera,scene);
	m.show("windows");
	waitKey(0);
	return 0;
}
