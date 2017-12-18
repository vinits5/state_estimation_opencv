#ifndef MARKERDETECTION_H
#define MARKERDETECTION_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "string.h"
using namespace cv;
using namespace std;

class markerDetection{
	public:
		void showFrame(string,Mat);
		vector<Point> cropPoints(vector<Point>);
		vector<Point> marker_by_shape(vector<Point>,Mat);
		vector<Point> marker_by_area(vector<Point>,vector<Point>);
};

#endif
