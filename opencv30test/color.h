#ifndef COLOR_H
#define COLOR_H
#include <opencv2/opencv.hpp>

class Color
{	
	cv::Vec3d color;//Vec3d is b, g, r but i transfer r,g,b
public:
	Color(double r = 0, double g = 0, double b = 0) : color(b , g , r){};
	~Color(){};
	const cv::Vec3d& getColor()const { return color; }
	const cv::Vec3d getImgColor()const {
		return cv::Vec3d(color[0] > 1 ? 1 : color[0],
					 color[1] > 1 ? 1 : color[1],
					 color[2] > 1 ? 1 : color[2]);
	}
	const double r() const { return color[2]; }
	const double g() const { return color[1]; }
	const double b() const { return color[0]; }

	friend Color operator + ( const Color& , const Color& );
	friend Color operator - ( const Color& , const Color& );
	friend Color operator * ( const Color& , const Color& );
	friend Color operator * ( const double& ,const Color& );
};

const Color BLACK = Color(0,0,0);
const Color WHITE = Color(1,1,1);
const Color RED   = Color(1,0,0);
const Color GREEN = Color(0,1,0);
const Color BLUE  = Color(0,0,1);

#endif