#include "PoseEstimation.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

using namespace cv;
using namespace std;

Mat PoseEstimation::rotateMarker(Mat img,vector<Point> marker){
	Point2f inp[4],sq[4];
	inp[0]=marker[0];
	inp[1]=marker[1];
	inp[2]=marker[2];
	inp[3]=marker[3];

	Rect boundRect=boundingRect(marker);
	sq[0]=Point(boundRect.x,boundRect.y);
        sq[1]=Point(boundRect.x,boundRect.y+boundRect.height);               
	sq[2]=Point(boundRect.x+boundRect.width,boundRect.y+boundRect.height);
	sq[3]=Point(boundRect.x+boundRect.width,boundRect.y);
	
	Mat transmtx=getPerspectiveTransform(inp,sq);
	Mat img1=Mat::zeros(img.rows,img.cols,CV_8UC1);
	warpPerspective(img,img1,transmtx,img.size());
	rectangle(img1,boundRect,Scalar(0,255,0),1,8,0);

	Mat arker=img1(boundRect);
	return arker;
}
