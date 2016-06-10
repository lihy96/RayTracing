#ifndef CONFIG_H
#define CONFIG_H

//#define lhy
const double randMaxHalf = (double)RAND_MAX * 0.5;
class Config
{
public:
	//��������
	static const int LIGHT = 777;
	static const int LIGHT_POINT_TYPE = 111;
	static const int LIGHT_BOX_LIGHT = 444;
	static const int SPHERE_TYPE = 222;
	static const int PLANE_TYPE = 333;
	static const int TRI_TYPE = 666;
	static const int POLY_TYPE = 888;
	
	//��Χ�д�С
	static const int GRIDSIZE = 200;
	
	static double myRand01(){ return (double)rand() / RAND_MAX; } //����0-1֮��������
	static double myRand_11(){ double maxHalf = RAND_MAX / 2; return ( maxHalf - rand() ) / maxHalf; } //����-1 1֮��������
	static double myRandhalf(){ return myRand01() * 0.5; } //����0 0.5֮��������
	static	double rand01(){
#ifdef lhy
		return 0.5;
#endif		
		double a = std::rand(); 
		return a / RAND_MAX;
	}
	static double exrand01()
	{
#ifdef lhy
		return 0;
#endif
		double a = std::rand() - randMaxHalf;
		return a / randMaxHalf;
	}
	static double exrand01half()
	{
		return exrand01() * 0.5;
	}
};
const double DEVIANCE = 1e-6;//ƫ����
const double DEVIATION = 1e-6;
const int TRACEMAXRECUR = 5;	//���ݹ����


const int SOFT_SHA_GRID_BIAN = 4;	//����Ӱ�ĸ��ӱ߳�
const int SOFT_SHA_GRID_SIZE = SOFT_SHA_GRID_BIAN * SOFT_SHA_GRID_BIAN;	//����Ӱ�ĸ��Ӹ���
const double SOFT_SHA_GRID_BIAN_DOWN = 1.0 / SOFT_SHA_GRID_BIAN;//����Ӱ�ĸ��ӱ߳��ĵ���
const int SOFT_SHA_GRID_MAX = SOFT_SHA_GRID_SIZE - 1;	//����Ӱ�ĸ��ӵ������0-15

//����
const int SAMPLES = 128;
const double SAMPLES_DOWN = 1.0 / SAMPLES;

//��
const int MISS = 0;
const int HIT = 1;

const MyVec3 ZERO = MyVec3(0,0,0);
const MyVec3 XPLUS = MyVec3(1, 0, 0);
const MyVec3 YPLUS = MyVec3(0, 1, 0);
const MyVec3 ZPLUS = MyVec3(0, 0, 1);
const MyVec3 XYPLUS = MyVec3(1, 1, 0);


//������
const int SUPER_SAMPLES = 1; //(n)^2
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

//����
const int APERTURE_R = 55;
const double FOCUS_DIS = 5.8;
const double APERTURE_R_PIXEL = ((double)APERTURE_R) / SCREEN_HEIGHT;
const int DEEP_OF_FIELD_NUM = 1; //�����������
const double DEEP_OF_FIELD_NUM_DOWN = 1.0 / DEEP_OF_FIELD_NUM;

//range
const double WORLD_RANGE = 10000.0;
const MyVec3 BIG = MyVec3(WORLD_RANGE,WORLD_RANGE,WORLD_RANGE);
const MyVec3 SMALL = MyVec3(-WORLD_RANGE,-WORLD_RANGE,-WORLD_RANGE);

//kdtree
const int KDTREE_MIN_NODE = 20;

//��ȡobj�ļ���ʱ����Ҫȷ���ж��ٸ���
const int MAX_VERTEX_NUM = 500000;













/*

1.������Ӱ�У�ֻ������x,z,��û���õ�y������Ϊ��Դ���컨���ϣ������Դ�ڲ���Ļ���Ӧ�ü���y

*/
#endif