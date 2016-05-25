#ifndef CONFIG_H
#define CONFIG_H




class Config
{
public:
	//��������
	static const int LIGHT_POINT_TYPE = 111;
	static const int LIGHT_BOX_LIGHT = 444;
	static const int SPHERE_TYPE = 222;
	static const int PLANE_TYPE = 333;
	
	//��Χ�д�С
	static const int GRIDSIZE = 8;
	static double myRand01(){ return (double)rand() / RAND_MAX; } //����0-1֮��������
	static double myRand_11(){ double maxHalf = RAND_MAX / 2; return ( maxHalf - rand() ) / maxHalf; } //����-1 1֮��������
};
const double DEVIANCE = 1e-6;//ƫ����
const int MAX_RECURSIVE_DEEPTH = 4;	//���ݹ����


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
const int DEEP_OF_FIELD_NUM = 28; //�����������
const double DEEP_OF_FIELD_NUM_DOWN = 1.0 / DEEP_OF_FIELD_NUM;















/*

1.������Ӱ�У�ֻ������x,z,��û���õ�y������Ϊ��Դ���컨���ϣ������Դ�ڲ���Ļ���Ӧ�ü���y

*/
#endif