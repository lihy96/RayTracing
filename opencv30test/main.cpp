#include <iostream>
#include <ctime>
#include "rayTracer.h"
#include "debug.h"
#include <ctime>
#define DEBUG
#define DIFF 0
#define SPEC 1
#define REFR 2
#define VER10

int Ray::number = 0;




int main(int argc, char** argv){
	std::srand(time(0));
	//freopen("lhy1.txt","w",stdout);
	double start_time = clock();
	MyMat m(600,600);
	RayTracer* rayTracer = new RayTracer();
	MyObj scene;

	//rayTracer->readObj("dargon03wh.obj",scene);
	//rayTracer->readObj("dragon.obj",scene,ZERO, MyVec3(1,1,1), MyVec3(0,0,0));
	//rayTracer->readObj("cube.obj",scene,ZERO, MyVec3(1,1,1), MyVec3(0,30,30));
	cout << "ok" << endl;
#ifdef VER10
	
	//readObj("c.obj", scene);
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
	//Camera camera(MyVec3(50,52,295.6), MyVec3(0,-0.042612,-1), MyVec3(1, 0, 0), 60.0, 60.0);
	rayTracer->addScene(scene);
	rayTracer->render(m,camera);
	double end_time10 = clock();
	printf("Escaped time: %.5lf\n" , (end_time10 - start_time)/CLOCKS_PER_SEC);
	m.show("windows");
	waitKey(0);
	m.savePicture("pic.png");
	return 0;
#endif

#ifdef VER9
	//Sphere*  p10 = new Sphere(MyVec3(-0.5, -4.1, 6), 0.38,new Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p10->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p10->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p10);

	scene.plight_vec.push_back(new PointLight(MyVec3(0, 6, 0), 0.1, Color(1, 1, 1)));
	//scene.blight_vec.push_back(new BoxLight(AABB(MyVec3(-0.5, 5, 2), MyVec3(1, 0.1, 1)), Color(1, 1, 1)));
	//scene.plight_vec.push_back(new PointLight(MyVec3(0, 6, 4), 0.1, Color(1, 1, 1)));
	
	//	plane
	//	ceiling
	scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 7, new Phong(WHITE, 1, 0.8, 20, 0, 0, 1.5, 0)));
	//	floor
	scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 4.6, new Phong(WHITE, 1, 0.8, 20, 0.5, 0, 1.5, 0)));
	//	back
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 7.2, new Phong(RED + GREEN, 1, 0.8, 20, 0, 0, 1.5, 0)));
	//	left
	scene.plane_vec.push_back(new Plane(MyVec3(1, 0, 0), 5.8, new Phong(RED, 1, 0.8, 20, 0, 0, 1.5, 0)));
	//	right
	scene.plane_vec.push_back(new Plane(MyVec3(-1, 0, 0), 5.8, new Phong(GREEN, 1, 0.8, 20, 0, 0, 1.5, 0)));
#endif

#ifdef VER8
	//r8-----------------------------------------------

	//scene.plane_vec.push_back(new Plane(MyVec3(0, 1, 0), 4.4, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0, 0, 1.5)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -1), 7, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0.2, 0, 1.5)));
	//scene.plane_vec.push_back(new Plane(MyVec3(0, -1, 0), 5.2, new Phong(Color(0.4, 0.3, 0.3), 1, 0.8, 20, 0, 0, 1.5)));

	
	rayTracer->readScene("scene/balls.txt",scene);
	scene.plight_vec.push_back( new PointLight(MyVec3(0,3,-4), 1, Color(0.9, 0.9, 0.9)));
	scene.plight_vec.push_back( new PointLight(MyVec3(4, 4, -4), 1, Color(0.6, 0.6, 0.7)));
	scene.plight_vec.push_back( new PointLight(MyVec3(2, 5, -2), 1, Color(0.5, 0.8, 0.8)));
	scene.plane_vec.push_back(new Plane(MyVec3(0, 0, -10), 10, new Phong(Color(0.7, 0.3, 0.4), 0.1, 0, 0, 0, 0, 1.5)));
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
	//scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5, new Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0, 0.8, 1.3)));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(-5.5, -0.5, 7), 2,new Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0, 1.3)));
	//scene.sphere_vec.push_back(new Sphere(MyVec3(-1.5, -3.8, 1), 1.5,new Phong(Color(1, 0.4, 0.4), 0.1, 0.8, 20, 0, 0.8, 1.5)));

	
	//Ò»¶ÑÐ¡ÂÌÇò
	//for (int x = 0; x < 8; x++)
	//	for (int y = 0; y < 7; y++){
	//		scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3 + y * 1.5, 10), 0.3,new Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));	
	//	}
	//for (int x = 0; x < 8; x++)
	//	for (int y = 0; y < 8; y++){
	//		scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3, 10.0 - y * 1.5), 0.3,new Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));
	//	}
	//for (int x = 0; x < 16; x++)
	//	for (int y = 0; y < 8; y++){
	//		scene.sphere_vec.push_back(new Sphere(MyVec3(-8.5 + x * 1.5, 4.3, 10.0 - y), 0.3,new Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));			
	//	}
	
	
#endif	
	
#ifndef VER10
	//	eye
	Camera camera(MyVec3(0, 1.2, -2), MyVec3(0, 0, 1), MyVec3(0, 1, 0), 90);
	//Camera camera(MyVec3(0, 1.2, -2.5), MyVec3(0, 0, 1), MyVec3(0, 1, 0), 90);
	rayTracer->addScene(scene);
	rayTracer->run(m,camera);
		double end_time = clock();
	printf("Escaped time: %.5lf\n" , (end_time - start_time)/CLOCKS_PER_SEC);
	m.show("windows");
	waitKey(0);
	m.savePicture("pic.png");
	return 0;
#endif
}
