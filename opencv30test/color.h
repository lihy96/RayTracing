#ifndef COLOR_H
#define COLOR_H
#include <opencv2/opencv.hpp>

class Color
{	
	cv::Vec3d color;//Vec3d is b, g, r but i transfer r,g,b
public:
	double r,g,b;
	Color(double _r = 0, double _g = 0, double _b = 0) : color(_b , _g , _r),r(_r), g(_g), b(_b){};
	~Color(){};
	const cv::Vec3d& getColor() const { return color; }
	const cv::Vec3d getImgColor() const {
		return cv::Vec3d(color[0] > 1 ? 1 : color[0],
					 color[1] > 1 ? 1 : color[1],
					 color[2] > 1 ? 1 : color[2]);
	}
	//const double r() const { return color[2]; }
	//const double g() const { return color[1]; }
	//const double b() const { return color[0]; }

	friend Color operator + ( const Color& , const Color& );
	friend Color operator * ( const Color& , const Color& );
	friend Color operator * ( const double& ,const Color& );
	friend Color operator * ( const Color& ,const double&);

	const Color clampColor() const
	{
		double R = color[2] > 1 ? 1 : color[2];
		R = R < 0 ? 0 : R;		
		double G = color[1] > 1 ? 1 : color[1];
		G = G < 0 ? 0 : G;
		double B = color[0] > 1 ? 1 : color[0];
		B = B < 0 ? 0 : B;
		return Color(R, G, B);
	}

	const cv::Vec3d imgcolor() const
	{
		double R = color[2] > 1 ? 1 : color[2];
		R = R < 0 ? 0 : R;		
		double G = color[1] > 1 ? 1 : color[1];
		G = G < 0 ? 0 : G;
		double B = color[0] > 1 ? 1 : color[0];
		B = B < 0 ? 0 : B;
		return cv::Vec3d(B, G, R);
	}

	
};

const Color BLACK = Color(0,0,0);
const Color WHITE = Color(1,1,1);
const Color RED   = Color(1,0,0);
const Color GREEN = Color(0,1,0);
const Color BLUE  = Color(0,0,1);

#endif