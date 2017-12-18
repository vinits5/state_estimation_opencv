#include "colorDetection.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat colorDetection::diffRGB(Mat img){
	vector<Mat> channel(3);
	split(img,channel);

	Mat diff_blue,diff_red,diff_green;
	diff_red=(channel[2]-channel[1]);
	diff_green=(channel[1]-channel[0]);
	diff_blue=(channel[0]-channel[2]);
	
	vector<Mat> channel1;
	channel1.push_back(diff_blue);
	channel1.push_back(diff_green);
	channel1.push_back(diff_red);

	merge(channel1,img);
	
	return img;
}

Mat colorDetection::blueThresh(Mat img){
	Mat gray,blue_extracted;
	vector<Mat> channel(3);
	cvtColor(img,gray,COLOR_BGR2GRAY);
	split(img,channel);
	blue_extracted=channel[0]-gray;

	GaussianBlur(blue_extracted,blue_extracted,Size(5,5),0);
	threshold(blue_extracted,blue_extracted,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
	
	erode(blue_extracted,blue_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	dilate(blue_extracted,blue_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	dilate(blue_extracted,blue_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	erode(blue_extracted,blue_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	return blue_extracted;
}

Mat colorDetection::redThresh(Mat img){
	Mat gray,red_extracted;
	vector<Mat> channel(3);
	cvtColor(img,gray,COLOR_BGR2GRAY);
	split(img,channel);
	red_extracted=channel[2]-gray;

	GaussianBlur(red_extracted,red_extracted,Size(5,5),0);
	threshold(red_extracted,red_extracted,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
	
	erode(red_extracted,red_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	dilate(red_extracted,red_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	dilate(red_extracted,red_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	erode(red_extracted,red_extracted,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	return red_extracted;
}


	

