#ifndef SHAPEDETECTION_H
#define SHAPEDETECTION_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

class shapeDetection{
	public:
		vector< vector<Point> > squareDetection(Mat);
		vector< vector<Point> > hexagonDetection(Mat);
};

#endif
