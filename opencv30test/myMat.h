#ifndef MYMAT_H
#define MYMAT_H

#include "myVec3.h"
#include "color.h"
#include <opencv2/opencv.hpp>

#define MYMATDEBUG

class MyMat
{
public:
	cv::Mat mat;

	MyMat(int _row=600,int _col=600) : mat(_row, _col, CV_64FC3, cv::Scalar::all(0)){};
	~MyMat(){};
	int getRow() {return mat.rows; }
	int getCol() {return mat.cols; }
	void show(const char* name){ cv::imshow(name,mat);}
	void setPixel(int _row, int _col,const cv::Vec3d& color){ mat.at<cv::Vec3d>(_row, _col) = color; }
	void setPixel(int _row, int _col,const Color& color ){ mat.at<cv::Vec3d>(_row, _col) = color.getColor(); }
	void savePicture(const char* filename){
		cv::Mat a;
		mat.convertTo(a, CV_8UC3, 255);
		cout << imwrite(filename, a);	
		
	}

	cv::Vec3d& pixel(int r, int c){	return mat.at<cv::Vec3d>(r, c);	}


#ifdef MYMATDEBUG
	friend ostream &operator<<(ostream &os, const MyMat &m) ;
#endif

};

#endif