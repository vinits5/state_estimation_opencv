#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "colorDetection.h"
#include "shapeDetection.h"
#include "markerDetection.h"

using namespace cv;
using namespace std;

int main(){
	VideoCapture cap(1);

	if(!cap.isOpened()){
		cout<<"Cannout open the video file"<<endl;
		return -1;
	}

	colorDetection cd;
	shapeDetection sd;
	markerDetection md;

	while(1){
		Mat inp,inpc;
		bool bSuccess=cap.read(inp);
		if(!bSuccess){
			cout<<"Cannot read the frame from the video file"<<endl;
			break;
		}
		//md.showFrame("inp",inp);
		inp.copyTo(inpc);

		
		Mat inp1=cd.diffRGB(inp);
		inp1=cd.blueThresh(inp1);
		vector< vector<Point> > square=sd.squareDetection(inp1);
		
		md.showFrame("inputFrame",inpc);
		if(square.size()>0){
			for (size_t k=0; k<square.size(); k++){
				vector<Point> p=md.cropPoints(square[k]);
				vector<Point> m=md.marker_by_shape(p,inp);
				if(m.size()==4){
					line(inpc,m[0],m[1],Scalar(0,255,0),CV_AA,0);
					line(inpc,m[1],m[2],Scalar(0,255,0),CV_AA,0);
					line(inpc,m[2],m[3],Scalar(0,255,0),CV_AA,0);
					line(inpc,m[3],m[0],Scalar(0,255,0),CV_AA,0);
				}
			}
		}
		md.showFrame("markerDetected",inpc);

/*		Mat inp2=cd.diffRGB(inp);
		inp2=cd.redThresh(inp2);
		vector< vector<Point> > hex=sd.hexagonDetection(inp2);
		if (square.size()>0 && hex.size()>0){
			for (size_t k=0; k<square.size(); k++){
				for (size_t j=0; j<hex.size(); j++){
					vector<Point> m;
					m=md.marker_by_area(square[k],hex[j]);
					if(m.size()==4){
						line(inpc,m[0],m[1],Scalar(0,255,0),CV_AA,0);
						line(inpc,m[1],m[2],Scalar(0,255,0),CV_AA,0);
						line(inpc,m[2],m[3],Scalar(0,255,0),CV_AA,0);
						line(inpc,m[3],m[0],Scalar(0,255,0),CV_AA,0);
					}
				}
			}
		}
		md.showFrame("markerDetected",inpc);
*/
		
		if (waitKey(3)==27){
			break;
		}
	}
	cap.release();
	return 0;
}

