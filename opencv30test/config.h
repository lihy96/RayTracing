#ifndef CONFIG_H
#define CONFIG_H
//-----------------------------------------------------------------------------------------------
#define LHY_KDTREE //确定是否使用包围盒或者kdtree,包围盒好像有问题

#define LHY_PHONG
#define LHY_WINDOWS //操作系统
#define DUO
//#define LHY_PINTU //将一张图分开跑
	const int loCol = 0;
	const int hiCol = 600; 
	const int PATH_SAMPLE = 5; 
	//需要再去改一下线程

#define SUPERSAM
	const int SUPER_SAMPLES = 1; //(n)^2
#define jingshen  //要开景深必须开超采样
	const int APERTURE_R = 10;
	const int DEEP_OF_FIELD_NUM = 10; //景深采样次数
	const double FOCUS_DIS = 100;

#define SAMPLEWAY 

//--------------------------------------------------------------------------------------------------
const double randMaxHalf = (double)RAND_MAX * 0.5;
class Config
{
public:
	static const int LIGHT = 777;
	static const int LIGHT_POINT_TYPE = 111;
	static const int LIGHT_BOX_LIGHT = 444;
	static const int SPHERE_TYPE = 222;
	static const int PLANE_TYPE = 333;
	static const int TRI_TYPE = 666;
	static const int POLY_TYPE = 888;
	
	//包围盒大小
#ifdef LHY_KDTREE
	static const int GRIDSIZE = 1;
#else
	static const int GRIDSIZE = 200;
#endif
	
	static double myRand01(){ return (double)rand() / RAND_MAX; } //产生0-1之间的随机数
	static double myRand_11(){ double maxHalf = RAND_MAX / 2; return ( maxHalf - rand() ) / maxHalf; } //产生-1 1之间的随机数
	static double myRandhalf(){ return myRand01() * 0.5; } //产生0 0.5之间的随机数
	static	double rand01(){	
		double a = std::rand(); 
		return a / RAND_MAX;
	}
	static double exrand01()
	{
		double a = std::rand() - randMaxHalf;
		return a / randMaxHalf;
	}
	static double exrand01half()
	{
		return exrand01() * 0.5;
	}
};
const double DEVIANCE = 1e-6;//偏移量
const double DEVIATION = 1e-6;
const int TRACEMAXRECUR = 5;	//最大递归深度


const int SOFT_SHA_GRID_BIAN = 4;	//软阴影的格子边长
const int SOFT_SHA_GRID_SIZE = SOFT_SHA_GRID_BIAN * SOFT_SHA_GRID_BIAN;	//软阴影的格子个数
const double SOFT_SHA_GRID_BIAN_DOWN = 1.0 / SOFT_SHA_GRID_BIAN;//软阴影的格子边长的倒数
const int SOFT_SHA_GRID_MAX = SOFT_SHA_GRID_SIZE - 1;	//软阴影的格子的最大编号0-15

//采样
const int SAMPLES = 128*4;
const double SAMPLES_DOWN = 1.0 / SAMPLES;

//求交
const int MISS = 0;
const int HIT = 1;

const MyVec3 ZERO = MyVec3(0,0,0);
const MyVec3 XPLUS = MyVec3(1, 0, 0);
const MyVec3 YPLUS = MyVec3(0, 1, 0);
const MyVec3 ZPLUS = MyVec3(0, 0, 1);
const MyVec3 XYPLUS = MyVec3(1, 1, 0);


//超采样

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

//景深

const double APERTURE_R_PIXEL = ((double)APERTURE_R) / SCREEN_HEIGHT;
const double DEEP_OF_FIELD_NUM_DOWN = 1.0 / DEEP_OF_FIELD_NUM;

//range
const double WORLD_RANGE = 10000.0;
const MyVec3 BIG = MyVec3(WORLD_RANGE,WORLD_RANGE,WORLD_RANGE);
const MyVec3 SMALL = MyVec3(-WORLD_RANGE,-WORLD_RANGE,-WORLD_RANGE);

//kdtree
const int KDTREE_MIN_NODE = 20;

//读取obj文件的时候需要确定有多少个点
const int MAX_VERTEX_NUM = 500000;


const int GLOSSYSAMPLETIME = 128*4;
const double GLOSSYSAMPLEDOWN = 1.0 / (double)(GLOSSYSAMPLETIME);













/*

1.在软阴影中，只利用了x,z,而没有用到y这是因为光源在天花板上，如果光源在侧面的话还应该加上y

*/
#endif