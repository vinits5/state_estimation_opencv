#include "shapeDetection.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

vector< vector<Point> > shapeDetection::squareDetection(Mat img){
	int ratio=3,kernel_size=3,lowThreshold=100;
	Canny(img,img,lowThreshold,lowThreshold*ratio,kernel_size);
	vector<Point> Points;
	vector< vector<Point> >contours;
	vector< vector<Point> >contours0;
	vector<Vec4i> hierarchy;
	vector< vector<Point> > detPoints;
	
	findContours(img,contours0,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	
	
	contours.resize(contours0.size());
	for (size_t k=0; k<contours0.size(); k++){
		approxPolyDP(Mat(contours0[k]),contours[k],9,true);
		if(contours[k].size()==4){
			detPoints.push_back(contours[k]);
		}
	}
	return detPoints;
}

vector< vector<Point> > shapeDetection::hexagonDetection(Mat img){
	int ratio=3,kernel_size=3,lowThreshold=100;
	Canny(img,img,lowThreshold,lowThreshold*ratio,kernel_size);
	vector<Point> Points;
	vector< vector<Point> >contours;
	vector< vector<Point> >contours0;
	vector<Vec4i> hierarchy;
	vector< vector<Point> > detPoints;
	
	findContours(img,contours0,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	
	
	contours.resize(contours0.size());
	for (size_t k=0; k<contours0.size(); k++){
		approxPolyDP(Mat(contours0[k]),contours[k],9,true);
		if(contours[k].size()==6){
			detPoints.push_back(contours[k]);
		}
	}
	return detPoints;
}


