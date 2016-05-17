#include <iostream>
#include <ctime>
#include "rayTracer.h"
#include "debug.h"
#define DEBUG

int main(int argc, char** argv){
	MyMat m(600,600);
	RayTracer* rayTracer = new RayTracer();
	MyObj scene;

	//scene.sphere_vec.push_back(new Sphere(MyVec3(20, 0, 0), 10));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(-10, 10, -10), 10, Phong(RED, WHITE, 16, 0)));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(10, 10, -10), 10, Phong(BLUE, WHITE, 16, 0)));

	scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5, Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0, 0.8, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-5.5, -0.5, 7), 2, Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0, 1.3)));
	Camera camera(MyVec3(0, 5, 15), MyVec3(0, 0, -1), MyVec3(0, 1, 0), 90);
	rayTracer->run(m,camera,scene);
	m.show("windows");
	waitKey(0);
	return 0;
}
