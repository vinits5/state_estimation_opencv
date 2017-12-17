#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace cv;
using namespace std;

//Area of L is 16.44
//Area of Square is 121
//Ratio of square to L is 7.36
//Segmentation error occurs when there is no square detected or L is detected.

//Function Header
Mat diffRGB(Mat);
Mat blueThresh(Mat);
Mat redThresh(Mat);
vector< vector<Point> > squareDetection(Mat);
vector< vector<Point> > LshapeDetection(Mat);
void showFrame(string,Mat);
vector< vector<Point> > findPoints(vector< vector<Point> >);
vector<Point> marker_detection(vector< vector<Point> >,vector< vector<Point> >); 
int markerDet(vector<Mat>);

int main(){
	// o or 1 for external camera.
	VideoCapture cap(1);
	//cap.set(CV_CAP_PROP_FPS, 5);

	if(!cap.isOpened()){
		cout<<"Cannout open the video file"<<endl;
		return -1;
	}
	
	while(1){
		//inp is input frame from camera.
		Mat inp,inp_copy,inp_copy1;
		bool bSuccess=cap.read(inp);
		if(!bSuccess){
			cout<<"Cannot read the frame from the video file"<<endl;
			break;
		}
		
		inp.copyTo(inp_copy);
		inp.copyTo(inp_copy1);
		
		//showFrame("VideoStream",inp);
		showFrame("VideoStream",inp);		

		//inp1 is the frame after differentialRGB thresholding.
		Mat inp1;
		inp1=diffRGB(inp);
//		showFrame("Differential",inp1);

		//inp2 is the frame after detection of blue color.
//		Mat inp2;
		inp1=blueThresh(inp1);
//		showFrame("BlueDetected",inp2);

		//inp3 is the frame after detection of red color.
		Mat inp3;
		inp3=diffRGB(inp);
		inp3=redThresh(inp3);
//		showFrame("RedDetected",inp3);

		//All contours with 4 corners are detected from the blue thresholded image.
		vector< vector<Point> > detPoints;
		detPoints=squareDetection(inp1);
/*		for (int k=0; k<detPoints.size(); k++){
			Point P1=detPoints[k][0];
			Point P2=detPoints[k][1];
			Point P3=detPoints[k][2];
			Point P4=detPoints[k][3];
			line(inp,P1,P2,Scalar(0,255,0),8,CV_AA,0);
			line(inp,P2,P3,Scalar(0,255,0),8,CV_AA,0);
			line(inp,P3,P4,Scalar(0,255,0),8,CV_AA,0);
			line(inp,P4,P1,Scalar(0,255,0),8,CV_AA,0);		
		}
		showFrame("SquareDetected",inp);
*/

		//All contours with 6 corners are detected from the red thresholded image.
		vector< vector<Point> > lPoints;
		lPoints=LshapeDetection(inp3);
/*		for (int k=0; k<lPoints.size(); k++){
			Point P1=lPoints[k][0];
			Point P2=lPoints[k][1];
			Point P3=lPoints[k][2];
			Point P4=lPoints[k][3];
			Point P5=lPoints[k][4];
			Point P6=lPoints[k][5];
			line(inp_copy,P1,P2,Scalar(0,0,255),8,CV_AA,0);
			line(inp_copy,P2,P3,Scalar(0,0,255),8,CV_AA,0);
			line(inp_copy,P3,P4,Scalar(0,0,255),8,CV_AA,0);
			line(inp_copy,P4,P5,Scalar(0,0,255),8,CV_AA,0);	
			line(inp_copy,P5,P6,Scalar(0,0,255),8,CV_AA,0);		
			line(inp_copy,P6,P1,Scalar(0,0,255),8,CV_AA,0);			
		}
		showFrame("LDetected",inp_copy);
*/
		
		//Points with cropped image
		vector< vector<Point> > cropPoints;
		cropPoints=findPoints(detPoints);
		//Show the frames of croppedImage
		vector<Mat> cropImgs;		
		for (size_t k=0; k<cropPoints.size(); k++){
			Mat cropIm;
			//Below line crops the input frame as per bounding rectangle (cropped Points)
			inp(Rect(cropPoints[k][0],cropPoints[k][1])).copyTo(cropIm);
			cropImgs.push_back(cropIm);
//			showFrame("croppedImage",cropIm);
		}
		
		//Index for the corners of marker is found from the markerDet function.
		int marker_index;
		//Below if condition is used to find if there is any blue quadrilateral in inp frame.
		//If we don't check this condition, it will rise segmentation fault when cropImgs is 			  empty vector.
		if(cropImgs.size()>0){
			marker_index=markerDet(cropImgs);
			//If marker_index is negative then there is no marker.
			if (marker_index>=0){
				Point P1=detPoints[marker_index][0];
				Point P2=detPoints[marker_index][1];
				Point P3=detPoints[marker_index][2];
				Point P4=detPoints[marker_index][3];
				line(inp_copy1,P1,P2,Scalar(0,255,0),8,CV_AA,0);
				line(inp_copy1,P2,P3,Scalar(0,255,0),8,CV_AA,0);
				line(inp_copy1,P3,P4,Scalar(0,255,0),8,CV_AA,0);
				line(inp_copy1,P4,P1,Scalar(0,255,0),8,CV_AA,0);
				showFrame("markerDetected",inp_copy1);
			}
		}else{
			showFrame("markerDetected",inp_copy1);
		}

		//waitKey is used to see the frames.
		if (waitKey(3)==27){
			break;
		}
	}
	cap.release();
	return 0;
}

//This function is used to show the frames with a given name and normal window size.
void showFrame(string name,Mat img){
	namedWindow(name,CV_WINDOW_NORMAL);
	imshow(name,img);
}

//This function is used for differntialRGB thresholding.
/*
1. All RGB channels are split from the frame. (split function)
   channel[0]=blue;
   channel[1]=green;
   channel[2]=red;
2. Red=Red-Green
   Green=Green-Blue
   Blue=Blue-Red
3. All three channels are merged into one frame. (merge function)
*/
Mat diffRGB(Mat img){
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

	Mat imgr;
	merge(channel1,imgr);
	
	return imgr;
}

//This function is used for blue color detection from the frame obtained after differentialRGB thresholding.
/*
1. Input image is differentialRGB thresholded image.
2. Input image is split into three channels and also converted into gray-scale image.
3. Blue color is extracted by subtracting the gray image from blue channel.
   blue_extracted=channel[0]-gray;
4. threshold is used to threshold every pixel with intensity greater than zero to 255.
5. erode and dilate is used to remove and fill the holes in the foreground of the image. 
*/
Mat blueThresh(Mat img){
	Mat gray,blue_extracted,thresholdedb;
	vector<Mat> channel(3);
	cvtColor(img,gray,COLOR_BGR2GRAY);
	split(img,channel);
	blue_extracted=channel[0]-gray;

	GaussianBlur(blue_extracted,blue_extracted,Size(5,5),0);
	threshold(blue_extracted,thresholdedb,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
	
	erode(thresholdedb,thresholdedb,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	dilate(thresholdedb,thresholdedb,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	dilate(thresholdedb,thresholdedb,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	erode(thresholdedb,thresholdedb,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	return thresholdedb;
}

//This function is used for detection of the contours with four corners from the frame obtained after blueThresh.
/*
1. Take the blue thresholded image as input.
2. Use canny edge detector algorithm to find edges in the input image.
3. Use a findContours function to find the contours in that image.
4. approxPolyDP is used to approximate the contours.
5. Store those points which have a contour with 4 corners in detPoints.
6. Return these detPoints.
*/
vector< vector<Point> > squareDetection(Mat img){
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

//This function is used to find the points which will crop those parts which are detected as a square in the inp frame (coming from camera).
/*
1. Take the four corners of each contour.
2. Find the max & min of x- & y- co-ordinates from all four points.
3. (xmax,ymin) & (xmin,ymax) will be bounding for the rectangle which is to be cropped from the image.
4. Store these points in a vector and return the vector as cropPoints.
*/
vector< vector<Point> > findPoints(vector< vector<Point> > inp){
	double X[4],Y[4];
	vector< vector<Point> > cropPoints;
	for (size_t k=0; k<inp.size(); k++){
		for (int i=0; i<4; i++){
			X[i]=inp[k][i].x;
			Y[i]=inp[k][i].y;
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
		vector<Point> p;
		p.push_back(Point(xmax,ymin));
		p.push_back(Point(xmin,ymax));
		cropPoints.push_back(p);
	}
	return cropPoints;
}

//This function is used for red color detection from the frame obtained after differentialRGB thresholding.
/*
1. Input image is differentialRGB thresholded image.
2. Input image is split into three channels and also converted into gray-scale image.
3. Red color is extracted by subtracting the gray image from red channel.
   red_extracted=channel[2]-gray;
4. threshold is used to threshold every pixel with intensity greater than zero to 255.
5. erode and dilate is used to remove and fill the holes in the foreground of the image. 
*/
Mat redThresh(Mat img){
	Mat gray,red_extracted,thresholdedr;
	vector<Mat> channel(3);
	cvtColor(img,gray,COLOR_BGR2GRAY);
	split(img,channel);
	red_extracted=channel[2]-gray;

	GaussianBlur(red_extracted,red_extracted,Size(5,5),0);
	threshold(red_extracted,thresholdedr,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
	
	erode(thresholdedr,thresholdedr,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	dilate(thresholdedr,thresholdedr,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	dilate(thresholdedr,thresholdedr,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	erode(thresholdedr,thresholdedr,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	
	return thresholdedr;
}
	
//This function is used for detection of the contours with six corners from the frame obtained after redThresh.
/*
1. Take the red thresholded image as input.
2. Use canny edge detector algorithm to find edges in the input image.
3. Use a findContours function to find the contours in that image.
4. approxPolyDP is used to approximate the contours.
5. Store those points which have a contour with 6 corners in detPoints.
6. Return these detPoints.
*/
vector< vector<Point> > LshapeDetection(Mat img){
	int ratio=3,kernel_size=3,lowThreshold=100;
	Canny(img,img,lowThreshold,lowThreshold*ratio,kernel_size);
	vector<Point> Points;
	vector< vector<Point> >contours;
	vector< vector<Point> >contours0;
	vector<Vec4i> hierarchy;
	vector< vector<Point> > detPoints;
	
	findContours(img,contours0,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	
	
	contours.resize(contours0.size());
	for (size_t k=0; k<contours0.size(); k++){
		approxPolyDP(Mat(contours0[k]),contours[k],9,true);
		if(contours[k].size()==6){
			detPoints.push_back(contours[k]);
		}
	}
	return detPoints;
}

//This function is used to find the corner of the marker.
/*
1. Take the cropped frames (which are cropped blue squares in inp image).
2. Use differentialRGB thresholding for these frames.
3. Red thresholding is implemented to obtain the red portion in the frame.
4. LshapeDetection is used to find contours with 6 corners in the thresholded frame.
5. If a contour(6 corners) is detected inside contour with 4 corners then it is our marker.
6. Store the index of the cropped frame.
7. Same index belongs to the corner points of the detected blue contour.
8. Use the index to find the corners of the marker from the vector obtained from squareDetection      	 function.
*/
int markerDet(vector<Mat> img){
	int marker_index=-1;
	for (int k=0; k<img.size(); k++){
		Mat im=img[k];
		im=diffRGB(im);
		im=redThresh(im);
		vector< vector<Point> > lPoints;
		lPoints=LshapeDetection(im);
		if(lPoints.size()>0){
			marker_index=k;
		}
	}
	return marker_index;
}
