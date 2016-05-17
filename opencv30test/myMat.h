#ifndef MYMAT_H
#define MYMAT_H

#include "myVec3.h"
#include "color.h"
#include <opencv2/opencv.hpp>

#define MYMATDEBUG

class MyMat
{
	cv::Mat mat;
public:
	MyMat(int _row,int _col) : mat(_row, _col, CV_64FC3, cv::Scalar::all(0)){};
	~MyMat(){};
	int getRow() {return mat.rows; }
	int getCol() {return mat.cols; }
	void show(const char* name){ cv::imshow(name,mat);}
	void setPixel(int _row, int _col,const cv::Vec3b& color){ mat.at<cv::Vec3b>(_row, _col) = color; }
	void setPixel(int _row, int _col,const Color& color ){ mat.at<cv::Vec3b>(_row, _col) = color.getColor(); }

#ifdef MYMATDEBUG
	friend ostream &operator<<(ostream &os, const MyMat &m) ;
#endif
};

#endif