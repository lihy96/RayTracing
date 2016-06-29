#include <iostream>
#include <ctime>
#include "rayTracer.h"
#include "debug.h"
#include "config.h"
#include "myMat.h"
#include <ctime>
#include <thread> 

#define DEBUG
#define DIFF 0
#define SPEC 1
#define REFR 2

RayTracer* rayTracer;
MyObj scene;
Camera* cameraP;
double start_time;
int Ray::number = 0;

void readInfo(RayTracer* rayTracer, MyObj& scene, const char* file){
	ifstream fin(file);
	double a[30];
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
			if (ch == 'o'){	//	vertex
				string objName;
				fin >> objName;
				for(int i = 0;i < 12;i++){
					fin >> a[i]; 
				}
				//位置，倍数，角度，颜色
				rayTracer->readObj(objName.c_str(),scene,MyVec3(a[0],a[1],a[2]),MyVec3(a[3],a[4],a[5]),MyVec3(a[6],a[7],a[8]),new BRDF(Color(a[9], a[10], a[11]), BLACK, REFR, 1.5));
			}
			else if (ch == 'f'){	//	face			

			}
		}
	}
	fin.close();
}

void thread_task(MyMat& img, Camera& camera,int lo,int hi) {
	std::cout << "hello thread "
		<< std::this_thread::get_id()
		<< lo << " " << hi << std::endl;
	rayTracer->run(img, camera, lo ,hi);
}
void tmpduostart(){
	const int n = 3;
	std::thread threads[n];
	MyMat img[n];
	//std::cout << "Spawning 5 threads...\n";
	for (int i = 0; i < n; i++) {
		threads[i] = std::thread(thread_task,std::ref(img[i]) ,std::ref(*cameraP), i*600/n,(i+1)*600/n);
	}
	//std::cout << "Done spawning threads! Now wait for them to join\n";
	for (auto& t: threads) {
		t.join();
	}
	//std::cout << "All threads joined.\n";

	double end_time10 = clock();
	printf("Escaped time: %.5lf\n" , (end_time10 - start_time)/CLOCKS_PER_SEC);
	MyMat m;
	for(int r = 0; r < 600; r++){
		for(int c = 0; c < 600; c++){
			m.mat.at<cv::Vec3d>(r, c) = img[c * n / 600].mat.at<cv::Vec3d>(r,c);
		}
	}
	m.savePicture("hecheng.png");
	m.show("hecheng");
	m.savePicture("hecheng.png");
	cout << "!!!!!!!" << endl;
	waitKey(0);
	return;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void duostart(){
	tmpduostart();
	return;

	const int n = 3;
	std::thread threads[n];
	MyMat img[n];
	std::cout << "Spawning 5 threads...\n";
	for (int i = 0; i < n; i++) {
		threads[i] = std::thread(thread_task,std::ref(img[i]) ,std::ref(*cameraP), i * 600 / n,(i+1)*600 / n);
	}
	std::cout << "Done spawning threads! Now wait for them to join\n";
	for (auto& t: threads) {
		t.join();
	}
	std::cout << "All threads joined.\n";

	double end_time10 = clock();
	printf("Escaped time: %.5lf\n" , (end_time10 - start_time)/CLOCKS_PER_SEC);
	MyMat m;
	for(int r = 0; r < 600; r++){
		for(int c = 0; c < 600; c++){
			m.mat.at<cv::Vec3d>(r, c) = img[c * n / 600].mat.at<cv::Vec3d>(r,c);
		}
	}
	m.savePicture("hecheng.png");
	m.show("hecheng");
	m.savePicture("hecheng.png");
	cout << "!!!!!!!" << endl;
	waitKey(0);
	return;
}




#define VER16
int main(int argc, char** argv){
	std::srand(time(0));

	//freopen("lhy1.txt","w",stdout);
	
	MyMat m(600,600);
	rayTracer = new RayTracer();
	
	//rayTracer->readObj("dargon03wh.obj",scene,MyVec3(50,16.5,40),MyVec3(25,25,25),ZERO,new BRDF(Color(0.999, 0.999, 0.999), BLACK, REFR, 1.5));
	//rayTracer->readObj("dragon.obj",scene,ZERO, MyVec3(1,1,1), MyVec3(0,0,0));
	//rayTracer->readObj("cube.obj",scene,ZERO, MyVec3(1,1,1), MyVec3(0,30,30));
#ifdef VER16  //ºìÂÌ¿µÄÎ¶ûºÐ
	//rayTracer->readObj("horse.obj",scene,MyVec3(50,20,65),MyVec3(25,25,25),MyVec3(-90,100,0),new Phong(WHITE, 1, 0.8, 20, 0, 0, 1.5, 1));
	//scene.plight_vec.push_back(new PointLight(MyVec3(50,81.6-16.5,84.6), 1.5, WHITE));
	scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(40, 72, 77), MyVec3(20, 0.1, 20)), 0.9*Color(1, 1, 1)));


	rayTracer->readObj("cube.obj",scene,MyVec3(76,0,35),MyVec3(27,40,6),MyVec3(-90,60,0),new Phong(Color(0.25, 0.25, 0.75), 1, 0.8, 20, 0, 0, 1.5, 1));

	scene.sphere_vec.push_back(new Sphere(MyVec3(55, 10.5, 55), 10.5 ,new Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0.0, 1.3, 0.6)));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(55, 10.5, 55), 10.5 ,new Phong(Color(0.7, 0.7, 1),0.04, 0.8, 20, 0, 0.8, 1.3)));
	//
	scene.sphere_vec.push_back(new Sphere(MyVec3(20,10.5, 65), 10.5 ,new Phong(Color(0.7, 0.7, 1), 0.04, 0.8, 20, 0, 0.8, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(76, 48, 20.5), 10.5,  new Phong(WHITE, 0.01, 0.8, 20, 0.8, 0, 1.5)));//Mirr 

	Sphere*  p10 = new Sphere(MyVec3(80,8.5, 80), 8.5,new Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p10->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p10->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p10);

	//	plane
	//	ceiling
	Plane* p4 = new Plane(MyVec3(0, -1, 0), 81.6, new Phong(WHITE, 1, 0.1, 20, 0, 0, 0, 1));
	scene.plane_vec.push_back(p4);
	//	floor
	Plane* p1 = new Plane(MyVec3(0, 1, 0), 0, new Phong(WHITE, 0.75, 0.1, 20, 0, 0, 0, 1));
	p1->material()->SetTexture( new Texture( "textures/rocks_2.tga" ) );
	p1->material()->SetUVScale( 0.05, 0.05 );
	scene.plane_vec.push_back(p1);
	//	back
	Plane* p5 = new Plane(MyVec3(0, 0, 1), 5, new Phong(WHITE, 1, 0.1, 20, 0, 0, 0, 1));
	p5->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p5->material()->SetUVScale( 0.05, 0.05 );
	scene.plane_vec.push_back(p5);
	//	FRONT
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new Phong(WHITE, 1, 0.1, 20, 0, 0, 0, 1)));
	//	left
	Plane* p2 = new Plane(MyVec3(1, 0, 0),1 ,new Phong(GREEN, 1, 0.1, 20, 0, 0, 1.5, 0));
	scene.plane_vec.push_back(p2);
	p2->material()->SetTexture( new Texture( "textures/brick22.tga" ) );
	p2->material()->SetUVScale( 0.01, 0.01 );
	//	right
	Plane* p3 = new Plane(MyVec3(-1, 0, 0), 99, new Phong(RED, 1, 0.1, 20, 0, 0, 1.5, 0));
	scene.plane_vec.push_back(p3);
	p3->material()->SetTexture( new Texture( "textures/brick22.tga" ) );
	p3->material()->SetUVScale( 0.01, 0.01 );
	cameraP = new Camera(MyVec3(50,45,167), MyVec3(0,-0.052612,-1), MyVec3(0, 1, 0), 60.0);

#endif
#ifdef VER15
	//rayTracer->readObj("horse.obj",scene,MyVec3(65,16,65),MyVec3(18,18,18),MyVec3(-90,110,0),new Phong(Color(0.5, 1, 0.5), 0.1, 0.3, 20, 0.5, 0, 0, 0));
	rayTracer->readObj("cube.obj",scene,MyVec3(76,0,35),MyVec3(27,55,6),MyVec3(-90,60,0),new Phong(Color(0.25, 0.25, 0.75), 1, 0.8, 20, 0, 0, 1.5, 1));
	rayTracer->readObj("bunny.obj",scene,MyVec3(37,-4,70),MyVec3(120,120,120),ZERO,new Phong(Color(1, 1, 1), 0.5, 0.9, 20, 0.3, 0.5, 0, 0));
	scene.plight_vec.push_back(new PointLight(MyVec3(50,81.6-16.5,84.6), 1.5, WHITE*0.8));
	//scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(40, 72, 77), MyVec3(20, 0.1, 20)), 0.9*Color(1, 1, 1)));

	

	//scene.sphere_vec.push_back(new Sphere(MyVec3(23, 6.5, 80), 6.5 ,new Phong(Color(0.7, 0.7, 1), 0.03, 0.8, 20, 0.5, 0.0, 1.3, 0)));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(50,32, 100), 16.5 ,new Phong(Color(0.7, 0.7, 1), 0.02, 0.8, 20, 0, 0.8, 1.3)));

	//	plane
	//	ceiling
	Plane* p4 = new Plane(MyVec3(0, -1, 0), 81.6, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1));
	scene.plane_vec.push_back(p4);
	//	floor
	Plane* p1 = new Plane(MyVec3(0, 1, 0), 0, new Phong(WHITE, 0.75, 0.8, 20, 0, 0, 0, 1));
	p1->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p1->material()->SetUVScale( 0.08, 0.08 );
	scene.plane_vec.push_back(p1);
	//	back
	Plane* p5 = new Plane(MyVec3(0, 0, 1), 5, new Phong(WHITE, 1, 0, 20, 0, 0, 0, 1));
	scene.plane_vec.push_back(p5);
	//	FRONT
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1)));
	//	left
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0),1 ,new Phong(GREEN, 1, 0.8, 20, 0, 0, 1.5, 0)));
	//	right
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 99, new Phong(RED, 1, 0.8, 20, 0, 0, 1.5, 0)));
	
	//scene.sphere_vec.push_back(new Sphere(MyVec3(15, 12.5, 22.5), 12.5,  new Phong(Color(0.7, 0.7, 0.7), 0.07, 0.8, 20, 0.1, 0.4, 1.5)));//Mirr 
	scene.sphere_vec.push_back(new Sphere(MyVec3(MyVec3(73,9,100)), 9,new Phong(0.8*WHITE, 0.2, 0.8, 20, 0.8, 0, 1.5)));
	cameraP = new Camera(MyVec3(50,32,167), MyVec3(0,-0.052612,-1), MyVec3(0, 1, 0), 60.0);	

#endif
#ifdef VER14  //红绿康奈尔盒

	scene.plight_vec.push_back(new PointLight(MyVec3(50,81.6-16.5,84.6), 1.5, WHITE*0.8));
	rayTracer->readObj("horse.obj",scene,MyVec3(52,25,65),MyVec3(30,30,30),MyVec3(-90,90,0),new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 0));
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 81.6, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 0, new Phong(WHITE, 0.75, 0.8, 20, 0, 0, 0, 1)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, 1), 5, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1)));
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0),1 ,new Phong(GREEN, 1, 0.8, 20, 0, 0, 1.5, 0)));
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 99, new Phong(RED, 1, 0.8, 20, 0, 0, 1.5, 0)));
	

	scene.sphere_vec.push_back(new Sphere(MyVec3(24, 10.5, 41), 10.5,new Phong(Color(0.7, 0.7, 0.7), 0.07, 0.8, 20, 0.1, 0.4, 1.5)));
	camera(MyVec3(50,52,167), MyVec3(0,-0.042612,-1), MyVec3(0, 1, 0), 60.0);	
#endif

#ifdef VER13  //红绿康奈尔盒
	rayTracer->readObj("horse.obj",scene,MyVec3(50,20,75),MyVec3(25,25,25),MyVec3(-90,100,0),new Phong(WHITE, 1, 0.8, 20, 0, 0, 1.5, 1));
	//scene.plight_vec.push_back(new PointLight(MyVec3(50,81.6-16.5,84.6), 1.5, WHITE));
	scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(40, 72, 77), MyVec3(20, 0.1, 20)), 0.9*Color(1, 1, 1)));

	

	//scene.sphere_vec.push_back(new Sphere(MyVec3(23, 6.5, 80), 6.5 ,new Phong(Color(0.7, 0.7, 1), 0.03, 0.8, 20, 0.5, 0.0, 1.3, 0)));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(50,32, 100), 16.5 ,new Phong(Color(0.7, 0.7, 1), 0.02, 0.8, 20, 0, 0.8, 1.3)));

	//	plane
	//	ceiling
	Plane* p4 = new Plane(MyVec3(0, -1, 0), 81.6, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1));
	scene.plane_vec.push_back(p4);
	//	floor
	Plane* p1 = new Plane(MyVec3(0, 1, 0), 0, new Phong(WHITE, 0.75, 0.8, 20, 0, 0, 0, 1));
	//p1->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p1->material()->SetUVScale( 0.05, 0.05 );
	scene.plane_vec.push_back(p1);
	//	back
	Plane* p5 = new Plane(MyVec3(0, 0, 1), 5, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1));
	scene.plane_vec.push_back(p5);
	//	FRONT
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 1)));
	//	left
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0),1 ,new Phong(GREEN, 1, 0.8, 20, 0, 0, 1.5, 0)));
	//	right
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 99, new Phong(RED, 1, 0.8, 20, 0, 0, 1.5, 0)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(14.5, 7.5, 20.5), 7.5,  new Phong(0.8*WHITE, 0.2, 0.8, 20, 0.8, 0, 1.5)));//Mirr 
	
	camera(MyVec3(50,45,167), MyVec3(0,-0.052612,-1), MyVec3(0, 1, 0), 60.0);

#endif
#ifdef VER12
	//rayTracer->readObj("dargon03wh.obj",scene,MyVec3(50,20,45),MyVec3(27,27,27),ZERO,new BRDF(Color(0.999, 0.999, 0.999), BLACK, REFR, 1.5));
	
	//scene.sphv.push_back(new Sphere(Vec3(1e5+1, 40.8, 81.6), 1e5, new BRDF(Color(.75, .25, .25), Vec3(), DIFF)));//Left 
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0), 1, new BRDF(Color(.75, .25, .25),BLACK, DIFF)));
	
	//scene.sphv.push_back(new Sphere(Vec3(-1e5+99, 40.8, 81.6), 1e5, new BRDF(Color(.25, .25, .75), Vec3(), DIFF)));//Rght 
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 99, new BRDF(Color(.25, .25, .75), BLACK, DIFF)));

	//scene.sphv.push_back(new Sphere(Vec3(50, 40.8, 1e5), 1e5, new BRDF(Color(.75, .75, .75), Vec3(), DIFF)));//Back 
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, 1), 5, new BRDF(Color(.75, .75, .75), BLACK, DIFF)));

	//scene.sphv.push_back(new Sphere(Vec3(50, 40.8, -1e5+170), 1e5, new BRDF(Color(), Vec3(), DIFF)));//Frnt 
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new BRDF(Color(.75, .75, .75), BLACK, DIFF)));

	//scene.sphv.push_back(new Sphere(Vec3(50, 1e5, 81.6), 1e5, new BRDF(Color(.75, .75, .75), Vec3(), DIFF)));//Botm 
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 0, new BRDF(Color(.75, .75, .75), BLACK, DIFF)));

	//scene.sphv.push_back(new Sphere(Vec3(50, -1e5+81.6, 81.6), 1e5, new BRDF(Color(.75, .75, .75), Vec3(), DIFF)));//Top 
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 81.6, new BRDF(Color(.75, .75, .75), BLACK, DIFF)));

	scene.sphere_vec.push_back(new Sphere(MyVec3(27, 16.5, 47), 16.5, new BRDF(Color(0.999, 0.999, 0.999), BLACK, SPEC)));//Mirr 
	scene.sphere_vec.push_back(new Sphere(MyVec3(73, 16.5, 78), 16.5, new BRDF(Color(0.999, 0.999, 0.999), BLACK, REFR, 1.5)));//Glas 
	
	scene.plight_vec.push_back(new PointLight(Sphere(MyVec3(50,81.6-16.5,81.6), 1.5, new BRDF(Color(), Color(400,400,400), DIFF))));//Lite 
	//scene.pltv.push_back(new PointLight(Sphere(Vec3(0, 5, 7.5), .5, new BRDF(Color(), Vec3(400,400,400), DIFF))));
	
	Camera camera(MyVec3(50,52,295.6), MyVec3(0,-0.042612,-1), MyVec3(1, 0, 0), 60.0, 60.0);	
#endif
#ifdef VER11
	//readInfo(rayTracer,scene, "objInfo.txt");
	//Sphere*  p10 = new Sphere(MyVec3(-0.5, -4.1, 6), 0.38,new Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p10->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p10->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p10);
	//rayTracer->readObj("dragon.obj",scene,MyVec3(50,17,30),MyVec3(18,18,18),ZERO,new Phong(Color(0.5, 1, 0.5), 0.1, 0.3, 20, 0.5, 0, 0, 0));
	rayTracer->readObj("bunny.obj",scene,MyVec3(50,-4,60),MyVec3(120,120,120),ZERO,new Phong(Color(1, 1, 1), 0.5, 0.9, 20, 0.3, 0.5, 0, 0));
	//rayTracer->readObj("horse.obj",scene,MyVec3(73,9,100),MyVec3(10,10,10),MyVec3(-90,120,0),new Phong(WHITE, 0.5, 0.6, 20, 0.3, 0, 0, 0));
	scene.plight_vec.push_back(new PointLight(MyVec3(50,81.6-16.5,84.6), 1.5, Color(1, 1, 1)));
	//scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(40, 78.6, 45), MyVec3(20, 1, 20)), Color(1, 1, 1)));

	

	scene.sphere_vec.push_back(new Sphere(MyVec3(23, 6.5, 80), 3.5 ,new Phong(Color(0.7, 0.7, 1), 0.03, 0.8, 20, 0.5, 0.0, 1.3, 0)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(50,32, 100), 6.5 ,new Phong(Color(0.7, 0.7, 1), 0.02, 0.8, 20, 0, 0.8, 1.3)));

	//	plane
	//	ceiling
	Plane* p4 = new Plane(MyVec3(0, -1, 0), 81.6, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 0));
	scene.plane_vec.push_back(p4);
	//	floor
	Plane* p1 = new Plane(MyVec3(0, 1, 0), 0, new Phong(WHITE, 0.5, 0.8, 20, 0.3, 0, 0, 0));
	p1->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p1->material()->SetUVScale( 0.05, 0.05 );
	scene.plane_vec.push_back(p1);
	//	back
	Plane* p5 = new Plane(MyVec3(0, 0, 1), 5, new Phong(Color(0.8, 0.8, 0.8), 1, 0.8, 20, 0, 0, 0, 0));
	p5->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p5->material()->SetUVScale( 0.1, 0.1 );
	scene.plane_vec.push_back(p5);
	//	back
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 0)));
	//	left
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0),1 ,new Phong(Color(.75, .25, .25), 1, 0.8, 20, 0, 0, 1.5, 0)));
	//	right
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 99, new Phong(Color(.25, .25, .75), 1, 0.8, 20, 0, 0, 1.5, 0)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(27, 16.5, 47), 16.5,  new Phong(WHITE, 0.02, 0.8, 20, 0, 10, 1.5)));//Mirr 
	//scene.sphere_vec.push_back(new Sphere(MyVec3(73, 16.5, 78), 16.5, new Phong(Color(0.999, 0.999, 0.999), BLACK, REFR, 1.5)));//Glas 
	cameraP =new Camera (MyVec3(50,32,167), MyVec3(0,-0.052612,-1), MyVec3(0, 1, 0), 60.0,60);	
#endif
#ifdef VER10
	//rayTracer->readObj("dargon03wh.obj",scene,MyVec3(50,20,45),MyVec3(27,27,27),ZERO,new BRDF(Color(0.999, 0.999, 0.999), BLACK, REFR, 1.5));
	//rayTracer->readObj("dragon.obj",scene,MyVec3(50,20,45),MyVec3(27,27,27),MyVec3(0,90,0),new BRDF(Color(0.999, 0.999, 0.999), BLACK, REFR, 1.5));
	//rayTracer->readObj("horse.obj",scene,MyVec3(52,25,74),MyVec3(30,30,30),MyVec3(-90,90,0),new BRDF(0.6*Color(0.999, 0.999, 0.999), BLACK, DIFF));
	//rayTracer->readObj("horse.obj",scene,MyVec3(63,22,32),MyVec3(25,25,25),MyVec3(-90,90,0),new BRDF(Color(0.69803, 0.13333, 0.13333), BLACK,  REFR, 1.5));
	//rayTracer->readObj("dragon.obj",scene,MyVec3(27,13,86),MyVec3(18,18,18),MyVec3(0,180,0),new BRDF(Color(0.74117, 0.71764, 0.419607), BLACK, DIFF));
	rayTracer->readObj("horse.obj",scene,MyVec3(52,25,89),MyVec3(30,30,30),MyVec3(-90,90,0),new BRDF(Color(0.69803, 0.13333, 0.13333), BLACK,  REFR, 1.5));
	start_time = clock();
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0), 1, new BRDF(Color(.75, .25, .25),BLACK, DIFF)));
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 99, new BRDF(Color(.25, .25, .75), BLACK, DIFF)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, 1), 5, new BRDF(Color(.75, .75, .75), BLACK, DIFF)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new BRDF(Color(.75, .75, .75), BLACK, DIFF)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 0, new BRDF(Color(.75, .75, .75), BLACK, DIFF))); 
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 81.6, new BRDF(Color(.75, .75, .75), BLACK, DIFF)));

//	scene.sphere_vec.push_back(new Sphere(MyVec3(57, 6.5, 96), 6.5, new BRDF(Color(0.999, 0.999, 0.999), BLACK, SPEC)));//Mirr 
	//scene.sphere_vec.push_back(new Sphere(MyVec3(80, 8.5, 92), 8.5, new BRDF(Color(0.999, 0.999, 0.999), BLACK, REFR,1.5)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(50, 681.6-0.27, 87), 600, new BRDF(Color(0.999, 0.999, 0.999),12* WHITE, REFR, 1.5)));
	//scene.plight_vec.push_back(new PointLight(Sphere(MyVec3(50, 681.6-0.27, 87), 600, new BRDF(BLACK, 12*0.9999*WHITE, DIFF))));//Lite 
	cameraP = new Camera(MyVec3(50,52,295.6), MyVec3(0,-0.042612,-1), MyVec3(1, 0, 0), 60.0, 60.0);	

#endif

#ifdef VER9
	//Sphere*  p10 = new Sphere(MyVec3(-0.5, -4.1, 6), 0.38,new Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p10->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p10->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p10);
	rayTracer->readObj("dargon03wh.obj",scene,MyVec3(50,20,45),MyVec3(27,27,27),ZERO,new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 0));
	scene.plight_vec.push_back(new PointLight(MyVec3(50,81.6-16.5,81.6), 1.5, Color(1, 1, 1)));
	//scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(-0.5, 5, 2), MyVec3(1, 0.1, 1)), Color(1, 1, 1)));
	//scene.plight_vec.push_back(new PointLight(MyVec3(0, 6, 4), 0.1, Color(1, 1, 1)));
	

	//	plane
	//	ceiling
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 81.6, new Phong(WHITE, 1, 0.8, 20, 0, 0, 0, 0)));
	//	floor
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 0, new Phong(WHITE, 1, 0.8, 20, 0.5, 0, 0, 0)));
	//	back
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, 1), 5, new Phong(RED + GREEN, 1, 0.8, 20, 0, 0, 0, 0)));
	//	back
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 170, new Phong(RED + GREEN, 1, 0.8, 20, 0, 0, 0, 0)));
	//	left
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0),1 ,new Phong(Color(.75, .25, .25), 1, 0.8, 20, 0, 0, 1.5, 0)));
	//	right
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 99, new Phong(Color(.25, .25, .75), 1, 0.8, 20, 0, 0, 1.5, 0)));

	camera(MyVec3(50,52,167), MyVec3(0,-0.042612,-1), MyVec3(0, 1, 0), 60.0);	

#endif

#ifdef VER8
	//r8-----------------------------------------------

	//scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 4.4, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0, 0, 1.5)));
	//scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 7, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0.2, 0, 1.5)));
	//scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 5.2, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0, 0, 1.5)));

	
	rayTracer->readScene("scene/balls.txt",scene);
//	scene.plight_vec.push_back( new PointLight(MyVec3(0,3,4), 1, Color(0.9, 0.9, 0.9)));
//	scene.plight_vec.push_back( new PointLight(MyVec3(4, 4, 4), 1, Color(0.6, 0.6, 0.7)));
	scene.plight_vec.push_back( new PointLight(MyVec3(0,0,60), 20, Color(1,1,1)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, 1), 0, new Phong(Color(0.7, 0.3, 0.4), 1, 0.8, 80, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 61, new Phong(Color(0.7, 0.3, 0.4), 1, 0.8, 80, 0, 0, 1.5)));
//	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0), 40, new Phong(Color(0.7, 0.3, 0.4), 1, 0.8, 80, 0, 0, 1.5)));
//	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 40, new Phong(Color(0.7, 0.3, 0.4), 1, 0.8, 80, 0, 0, 1.5)));
	Camera camera(MyVec3(0,0,39), MyVec3(0,0,-1), MyVec3(0, 1, 0), 30.0);
#endif
	//r7-------------------------------------------------------------------------

#ifdef VER7
	
	Plane* p1 = new Plane(MyVec3(0, 1, 0), 3, new Phong(Color(0.4, 0.3, 0.3), 0.7, 0.5, 20, 0, 0, 1.5));
	p1->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p1->material()->SetUVScale( 0.05, 0.05 );
	scene.plane_vec.push_back(p1);


	Plane* p5 = new Plane(MyVec3(0, 0, -1), 22.4, new Phong(Color(0.6, 0.5, 0.5), 1, 0.8, 20, 0, 0, 1.5));
	p5->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p5->material()->SetUVScale( 0.1, 0.1 );
	scene.plane_vec.push_back(p5);

	Plane* p6 = new Plane(MyVec3(1, 0, 0), 10, new Phong(Color(0.5, 0.5, 0.6), 1, 0.8, 20, 0, 0, 1.5));
	p6->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p6->material()->SetUVScale( 0.1, 0.1 );
	scene.plane_vec.push_back(p6);

	Plane* p7= new Plane(MyVec3(-1, 0, 0), 10, new Phong(Color(0.5, 0.5, 0.6), 1, 0.8, 20, 0, 0, 1.5));
	p7->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p7->material()->SetUVScale( 0.1, 0.1 );
	scene.plane_vec.push_back(p7);

	Plane* p8= new Plane(MyVec3(0, -2, 0), 8.2, new Phong(Color(0.7, 0.7, 0.7), 1, 0.8, 20, 0, 0, 1.5));
	//p8->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p8->material()->SetUVScale( 0.4, 0.4 );
	scene.plane_vec.push_back(p8);
//-------------------------------------------------------------------------------------------------------------------------
	//Sphere*  p9 = new Sphere(MyVec3(-2, 1.5, 10), 0.9, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p9->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p9->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p9);

	Sphere*  p10 = new Sphere(MyVec3(-2, 0.8, 10), 1.8,new Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p10->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p10->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p10);

	//Sphere*  p11 = new Sphere(MyVec3(-2, 0.3, 4), 0.7, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p11->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p11->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p11);

	Sphere*  p12 = new Sphere(MyVec3(-2, -0.3, 1), 0.6, new Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p12->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p12->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p12);

	Sphere*  p17 = new Sphere(MyVec3(-2, -0.9, -2), 0.5,new  Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p17->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p17->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p17);

	Sphere*  p13 = new Sphere(MyVec3(2, 1.5, 4), 0.7,new  Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0.11, 1.5));
	p13->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p13->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p13);

	Sphere*  p14 = new Sphere(MyVec3(2, 0.8, 10), 1,new  Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0.11, 1.5));
	p14->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p14->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p14);

	Sphere*  p15 = new Sphere(MyVec3(0, 0.3, 4), 0.7,new  Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0.05, 1.5));
	p15->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p15->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p15);

	Sphere*  p16 = new Sphere(MyVec3(2, -0.3, 1), 0.6,new  Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0.05, 1.5));
	p16->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p16->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p16);

	Sphere*  p18 = new Sphere(MyVec3(2, -0.9, -2), 0.5,new  Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0.2, 1.5));
	p18->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p18->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p18);

	Sphere*  p3 = new Sphere(MyVec3(-5, -0.8, 7), 1,new  Phong(Color(0.7, 0.7, 1.0), 0.7, 0.8, 20, 0.2, 0.5, 1.5));
	p3->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p3->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p3);

	Sphere*  p4 = new Sphere(MyVec3(5, -0.8, 7), 1,new  Phong(Color(0.8, 0.8, 1.0), 0.4, 0.8, 20, 0.6, 0, 1.3));
	p4->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p4->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p4);

	//scene.plight_vec.push_back( new PointLight(MyVec3(0, 1.2, 0.2), 0.5, Color(0.6, 0.6, 0.7)));
	scene.plight_vec.push_back( new PointLight(MyVec3(4, 4, -4), 0.1, Color(0.6, 0.6, 0.7)));
	scene.plight_vec.push_back( new PointLight(MyVec3(2, 5, -2), 0.1, Color(0.5, 0.8, 0.8)));

	//scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(-1, 5, 4), MyVec3(2, 0.1, 2)), Color(1, 1, 1)));
#endif

	//r6-------------------------------------------------------------------------

#ifdef VER6
	Plane* p1 = new Plane(MyVec3(0, 1, 0), 3, new Phong(Color(0.4, 0.3, 0.3), 0.7, 0.5, 20, 0, 0, 1.5));
	p1->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p1->material()->SetUVScale( 0.05, 0.05 );
	scene.plane_vec.push_back(p1);


	Plane* p5 = new Plane(MyVec3(0, 0, -1), 22.4, new Phong(Color(0.6, 0.5, 0.5), 1, 0.8, 20, 0, 0, 1.5));
	p5->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p5->material()->SetUVScale( 0.1, 0.1 );
	scene.plane_vec.push_back(p5);

	Plane* p6 = new Plane(MyVec3(1, 0, 0), 10, new Phong(Color(0.5, 0.5, 0.6), 1, 0.8, 20, 0, 0, 1.5));
	p6->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p6->material()->SetUVScale( 0.1, 0.1 );
	scene.plane_vec.push_back(p6);

	Plane* p7= new Plane(MyVec3(-1, 0, 0), 10, new Phong(Color(0.5, 0.5, 0.6), 1, 0.8, 20, 0, 0, 1.5));
	p7->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p7->material()->SetUVScale( 0.1, 0.1 );
	scene.plane_vec.push_back(p7);

	Plane* p8= new Plane(MyVec3(0, -1, 0), 5.2, new Phong(Color(0.7, 0.7, 0.7), 1, 0.8, 20, 0, 0, 1.5));
	//p8->material()->SetTexture( new Texture( "textures/wood.tga" ) );
	p8->material()->SetUVScale( 0.4, 0.4 );
	scene.plane_vec.push_back(p8);

	Sphere*  p2 = new Sphere(MyVec3(0, 0.5, 4), 2, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p2->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p2->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p2);


	Sphere*  p3 = new Sphere(MyVec3(-5, -0.8, 7), 2, Phong(Color(0.7, 0.7, 1.0), 0.7, 0.8, 20, 0.2, 0, 1.5));
	p3->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p3->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p3);

	Sphere*  p4 = new Sphere(MyVec3(5, -0.8, 7), 2, Phong(Color(0.8, 0.8, 1.0), 0.4, 0.8, 20, 0.6, 0, 1.3));
	p4->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p4->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p4);

	scene.plight_vec.push_back( new PointLight(MyVec3(4, 4, -4), 0.1, Color(0.6, 0.6, 0.7)));
	scene.plight_vec.push_back( new PointLight(MyVec3(2, 5, -2), 0.1, Color(0.5, 0.8, 0.8)));

	//scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(-1, 5, 4), MyVec3(2, 0.1, 2)), Color(1, 1, 1)));
#endif

	//r5--------------------------------------------------------------------------------------------------
	//Phong color, diffuse,  specular,  shininess = 0,  reflectiveness = 0,  refraction = 0,  refraIndex = 1.0,  difflection = 0
	//	light
#ifdef VER5
		//plane
	
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 4.4, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 12.4, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 5.2, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0, 0, 1.5)));
	//	Sphere
	scene.sphere_vec.push_back(new Sphere(MyVec3(0, -0.8, 7), 2,new Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0.2, 0.8, 1.3, 0.3)));
	/*scene.sphere_vec.push_back(new Sphere(MyVec3(-5, -0.8, 7), 2,new Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0.0, 1.3, 0.6)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(5, -0.8, 7), 2, new Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0.0, 1.3)));*/
	//arealight
	scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(-1, 5, 4), MyVec3(2, 0.1, 2)), Color(1, 1, 1)));
	//scene.plight_vec.push_back( new PointLight(MyVec3(4, 4, -4), 0.1, Color(0.6, 0.6, 0.7)));
#endif

	//------------------------------------------------------------------------------r3
#ifdef VER3
	scene.plight_vec.push_back(new PointLight(MyVec3(0, 5, 5), 0.1, Color(0.4, 0.4, 0.4)));
	scene.plight_vec.push_back(new PointLight(MyVec3(-3, 5, 1), 0.1, Color(0.6, 0.6, 0.6)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 4.4, new Phong(Color(0.4, 0.3, 0.3), 1, 0, 0, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0.4, 0, -1), 12, new Phong(Color(0.5, 0.3, 0.5), 0.6, 0, 0, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 7.4, new Phong(Color(0.4, 0.7, 0.7), 0.5, 0, 0, 0, 0, 1.5)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5,new Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0, 0.8, 1.3)));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5, Phong(Color(0.647, 0.546, 0.09), 0, 0.8, 174, 0.7, 0.1, 0)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-5.5, -0.5, 7), 2,new Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-1.5, -3.8, 1), 1.5,new Phong(Color(1, 0.4, 0.4), 0.1, 0.8, 20, 0, 0.8, 1.5)));
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 7; y++){
			scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3 + y * 1.5, 10), 0.3,new Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));	
		}

#endif
	


	//-----------------------------------------------------------------------------------------------------------r4
#ifdef VER4
	//		light
	scene.plight_vec.push_back(new PointLight(MyVec3(0, 5, 5), 0.1, Color(0.4, 0.4, 0.4)));
	scene.plight_vec.push_back(new PointLight(MyVec3(-3, 5, 1), 0.1, Color(0.6, 0.6, 0.6)));
	//	plane
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 4.4, new Phong(Color(0.4, 0.3, 0.3), 1, 0, 0, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0.4, 0, -1), 12, new Phong(Color(0.5, 0.3, 0.5), 0.6, 0, 0, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 7.4, new Phong(Color(0.4, 0.7, 0.7), 0.5, 0, 0, 0, 0, 1.5)));
	//	Sphere
	scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5, new Phong(Color(0.7, 0.7, 1), 0.02, 0.8, 20, 0, 0.8, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-5.5, -0.5, 7), 2,new Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-1.5, -3.8, 1), 1.5,new Phong(Color(1, 0.4, 0.4), 0.1, 0.8, 20, 0, 0.8, 1.5)));

	
	//一堆小绿球
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 7; y++){
			scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3 + y * 1.5, 10), 0.3,new Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));	
		}
	//for (int x = 0; x < 8; x++)
	//	for (int y = 0; y < 8; y++){
	//		scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3, 10.0 - y * 1.5), 0.3,new Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));
	//	}
	//for (int x = 0; x < 16; x++)
	//	for (int y = 0; y < 8; y++){
	//		scene.sphere_vec.push_back(new Sphere(MyVec3(-8.5 + x * 1.5, 4.3, 10.0 - y), 0.3,new Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));			
	//	}
	
	Camera camera(MyVec3(0, 0, -5), MyVec3(0, 0, 1), MyVec3(0, 1, 0), 60);
#endif	
	

	rayTracer->addScene(scene);
	duostart();
	return 0;
#ifdef LHY_PHONG
	rayTracer->run(m,*cameraP);
#else
	rayTracer->render(m,*cameraP);
#endif
	double end_time10 = clock();
	printf("Escaped time: %.5lf\n" , (end_time10 - start_time)/CLOCKS_PER_SEC);
	m.show("windows");
	waitKey(0);
	m.savePicture("pic.png");
	return 0;
}
