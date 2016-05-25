#include <iostream>
#include <ctime>
#include "rayTracer.h"
#include "debug.h"
#define DEBUG

#define VER7

int Ray::number = 0;

int main(int argc, char** argv){
	//freopen("lhy1.txt","w",stdout);

	MyMat m(600,600);
	RayTracer* rayTracer = new RayTracer();
	MyObj scene;

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

	Sphere*  p10 = new Sphere(MyVec3(-2, 0.8, 10), 1.8, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p10->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p10->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p10);

	//Sphere*  p11 = new Sphere(MyVec3(-2, 0.3, 4), 0.7, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p11->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p11->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p11);

	Sphere*  p12 = new Sphere(MyVec3(-2, -0.3, 1), 0.6, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p12->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p12->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p12);

	Sphere*  p17 = new Sphere(MyVec3(-2, -0.9, -2), 0.5, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p17->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p17->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p17);

	//Sphere*  p13 = new Sphere(MyVec3(2, 1.5, 10), 0.9, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p13->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p13->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p13);

	Sphere*  p14 = new Sphere(MyVec3(2, 0.8, 10), 1.8, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p14->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p14->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p14);

	//Sphere*  p15 = new Sphere(MyVec3(2, 0.3, 4), 0.7, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	//p15->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	//p15->material()->SetUVScale( 0.8, 0.8 );
	//scene.sphere_vec.push_back(p15);

	Sphere*  p16 = new Sphere(MyVec3(2, -0.3, 1), 0.6, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p16->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p16->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p16);

	Sphere*  p18 = new Sphere(MyVec3(2, -0.9, -2), 0.5, Phong(Color(0.8, 0.8, 0.8), 0.5, 0.8, 20, 0.05, 0, 1.5));
	p18->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p18->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p18);

	Sphere*  p3 = new Sphere(MyVec3(-5, -0.8, 7), 1, Phong(Color(0.7, 0.7, 1.0), 0.7, 0.8, 20, 0.2, 0, 1.5));
	p3->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p3->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p3);

	Sphere*  p4 = new Sphere(MyVec3(5, -0.8, 7), 1, Phong(Color(0.8, 0.8, 1.0), 0.4, 0.8, 20, 0.6, 0, 1.3));
	p4->material()->SetTexture( new Texture( "textures/marble.tga" ) );
	p4->material()->SetUVScale( 0.8, 0.8 );
	scene.sphere_vec.push_back(p4);

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
	scene.sphere_vec.push_back(new Sphere(MyVec3(0, -0.8, 7), 2, Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0.2, 0.8, 1.3, 0.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-5, -0.8, 7), 2, Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0.0, 1.3, 0.6)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(5, -0.8, 7), 2, Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0.0, 1.3)));
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
	scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5, Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0, 0.8, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-5.5, -0.5, 7), 2, Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-1.5, -3.8, 1), 1.5, Phong(Color(1, 0.4, 0.4), 0.1, 0.8, 20, 0, 0.8, 1.5)));
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 7; y++){
			scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3 + y * 1.5, 10), 0.3, Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));	
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
	scene.sphere_vec.push_back(new Sphere(MyVec3(2, 0.8, 3), 2.5, Phong(Color(0.7, 0.7, 1), 0.2, 0.8, 20, 0, 0.8, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-5.5, -0.5, 7), 2, Phong(Color(0.7, 0.7, 1), 0.1, 0.8, 20, 0.5, 0, 1.3)));
	scene.sphere_vec.push_back(new Sphere(MyVec3(-1.5, -3.8, 1), 1.5, Phong(Color(1, 0.4, 0.4), 0.1, 0.8, 20, 0, 0.8, 1.5)));

	
	//Ò»¶ÑÐ¡ÂÌÇò
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 7; y++){
			scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3 + y * 1.5, 10), 0.3, Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));	
		}
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++){
			scene.sphere_vec.push_back(new Sphere(MyVec3(-4.5 + x * 1.5, -4.3, 10.0 - y * 1.5), 0.3, Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));
		}
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 8; y++){
			scene.sphere_vec.push_back(new Sphere(MyVec3(-8.5 + x * 1.5, 4.3, 10.0 - y), 0.3, Phong(Color(0.3, 1, 0.4), 0.6, 0.6, 20, 0, 0, 1.5)));			
		}
	
	
#endif	
		
	//	eye
	Camera camera(MyVec3(0, 0, -4.5), MyVec3(0, 0, 1), MyVec3(0, 1, 0), 90);
	//Camera camera(MyVec3(0, 5, -9.5), MyVec3(0, -1, 1), MyVec3(0, 1, 1), 90);
	rayTracer->run(m,camera,scene);
	m.show("windows");
	waitKey(0);
	return 0;
}
