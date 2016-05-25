#ifndef CONFIG_H
#define CONFIG_H




class Config
{
public:
	//物体类型
	static const int LIGHT_POINT_TYPE = 111;
	static const int LIGHT_BOX_LIGHT = 444;
	static const int SPHERE_TYPE = 222;
	static const int PLANE_TYPE = 333;
	
	//包围盒大小
	static const int GRIDSIZE = 8;
	static double myRand01(){ return (double)rand() / RAND_MAX; } //产生0-1之间的随机数
	static double myRand_11(){ double maxHalf = RAND_MAX / 2; return ( maxHalf - rand() ) / maxHalf; } //产生-1 1之间的随机数
};
const double DEVIANCE = 1e-6;//偏移量
const int MAX_RECURSIVE_DEEPTH = 4;	//最大递归深度


const int SOFT_SHA_GRID_BIAN = 4;	//软阴影的格子边长
const int SOFT_SHA_GRID_SIZE = SOFT_SHA_GRID_BIAN * SOFT_SHA_GRID_BIAN;	//软阴影的格子个数
const double SOFT_SHA_GRID_BIAN_DOWN = 1.0 / SOFT_SHA_GRID_BIAN;//软阴影的格子边长的倒数
const int SOFT_SHA_GRID_MAX = SOFT_SHA_GRID_SIZE - 1;	//软阴影的格子的最大编号0-15

//采样
const int SAMPLES = 128;


const double SAMPLES_DOWN = 1.0 / SAMPLES;

//求交
const int MISS = 0;
const int HIT = 1;

const MyVec3 XPLUS = MyVec3(1, 0, 0);
const MyVec3 YPLUS = MyVec3(0, 1, 0);
const MyVec3 ZPLUS = MyVec3(0, 0, 1);
const MyVec3 XYPLUS = MyVec3(1, 1, 0);


//超采样
const int SUPER_SAMPLES = 1; //(n)^2
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

//景深
const int APERTURE_R = 55;
const double FOCUS_DIS = 5.8;
const double APERTURE_R_PIXEL = ((double)APERTURE_R) / SCREEN_HEIGHT;
const int DEEP_OF_FIELD_NUM = 28; //景深采样次数
const double DEEP_OF_FIELD_NUM_DOWN = 1.0 / DEEP_OF_FIELD_NUM;















/*

1.在软阴影中，只利用了x,z,而没有用到y这是因为光源在天花板上，如果光源在侧面的话还应该加上y

*/
#endif