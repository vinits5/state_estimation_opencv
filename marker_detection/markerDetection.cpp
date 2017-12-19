#include "markerDetection.h"
#include "colorDetection.h"
#include "shapeDetection.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

using namespace cv;
using namespace std;

void markerDetection::showFrame(string s,Mat img){
	namedWindow(s,CV_WINDOW_NORMAL);
	imshow(s,img);
}

vector<Point> markerDetection::cropPoints(vector<Point> p){
	double X[4],Y[4];
	for (int i=0; i<4; i++){
		X[i]=p[i].x;
		Y[i]=p[i].y;
	}
	double xmax=X[0],xmin=X[0],ymax=Y[0],ymin=Y[0];
	int ixmax,ixmin,iymax,iymin;
	for (int i=0; i<4; i++){
		if(X[i]>=xmax){
			xmax=X[i];
			ixmax=i;
		}
		if(X[i]<=xmin){
			xmin=X[i];
			ixmin=i;
		}
		if(Y[i]>=ymax){
			ymax=Y[i];
			iymax=i;
		}
		if(Y[i]<=ymin){
			ymin=Y[i];
			iymin=i;
		}
	}
	vector<Point> p1;
	p1.push_back(Point(xmax,ymin));
	p1.push_back(Point(xmin,ymax));
	p1.push_back(p[0]);p1.push_back(p[1]);p1.push_back(p[2]);p1.push_back(p[3]);
	return p1;
}

vector<Point> markerDetection::marker_by_shape(vector<Point> p,Mat img){
	Mat crop,img1;
	img(Rect(p[0],p[1])).copyTo(crop);
	colorDetection cdm;
	img1=cdm.diffRGB(crop);
	img1=cdm.redThresh(img1);
	vector<Point> m;
	shapeDetection sdm;
	vector< vector<Point> > hex=sdm.hexagonDetection(img1);
	if(hex.size()>0){
		m.push_back(p[2]);
		m.push_back(p[3]);
		m.push_back(p[4]);
		m.push_back(p[5]);
	}
	else{
		m.push_back(Point(-1,-1));
	}
	return m;
}

vector<Point> markerDetection::marker_by_area(vector<Point> s,vector<Point> h){
	double sA,hA,r;
	sA=contourArea(s);
	hA=contourArea(h);
	r=sA/hA;
	vector<Point> m;
	if (r>6 && r<8){
		for (int i=0; i<4; i++){
			m.push_back(s[i]);
		}
	}
	else{
		m.push_back(Point(-1,-1));
	}
	return m;
}


