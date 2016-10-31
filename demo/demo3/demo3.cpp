
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cvaux.h>
#include <iostream>
#include <fstream>  
#include <iomanip>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <vector>
using namespace std;
using namespace cv;
//using namespace fstream;
//#define _CAPTURE_FROM_AVI_FILE_

#define OPENCV2413Debug
#ifdef _MSC_VER
#	ifdef OPENCV210
#		pragma comment(lib, "cv210.lib")
#		pragma comment(lib, "cxcore210.lib")
#		pragma comment(lib, "cvaux210.lib")
#		pragma comment(lib, "highgui210.lib")
#	endif
#	ifdef OPENCV220
#		pragma comment(lib, "opencv_core220.lib")
#		pragma comment(lib, "opencv_highgui220.lib")
#	endif
#	ifdef OPENCV231
#		pragma comment(lib, "opencv_core231.lib")
#		pragma comment(lib, "opencv_highgui231.lib")
#		pragma comment(lib, "opencv_imgproc231.lib")
#		pragma comment(lib, "opencv_objdetect231d.lib")
#	endif
#	ifdef OPENCV2410
#		pragma comment(lib, "opencv_core2410.lib")
#		pragma comment(lib, "opencv_highgui2410.lib")
#		pragma comment(lib, "opencv_imgproc2410.lib")
#	endif
#	ifdef OPENCV2413
#		pragma comment(lib, "opencv_core2413.lib")
#		pragma comment(lib, "opencv_highgui2413.lib")
#		pragma comment(lib, "opencv_imgproc2413.lib")
#	endif
#	ifdef OPENCV2413Debug
#		pragma comment(lib, "opencv_core2413d.lib")
#		pragma comment(lib, "opencv_highgui2413d.lib")
#		pragma comment(lib, "opencv_imgproc2413d.lib")
#	endif
#endif



int main(int argc, char* argv[])
{
	fstream fs;
	vector <Point2d> n2;
	vector <Point3d> n3;
	float a,b,c;
	Mat src,dist,eigenvectors;
	
	vector < vector<double> > one;
	vector<double> two ;
	namedWindow("L000.bmp", WINDOW_AUTOSIZE);
	IplImage *image; 
	double tmp,min;
	double m1[12][12] ,h[3][4];
	int ind=0;

	fs.open("lcalib.p2d",fstream::in);
	while(fs>>a>>b)
	{
		if(a==0&&b==0)break;
		n2.push_back(Point2d(a,b));
		
	}
	fs.close();
	fs.open("lcalib.p3d",fstream::in);
	while(fs>>a>>b>>c)
	{
		if(a==0&&b==0&&c==0)break;
		n3.push_back(Point3d(a,b,c));
		
	}
	fs.close();

	
	for(int i =0; i<n2.size();i++)
	{
		two.push_back(n3[i].x);
		two.push_back(n3[i].y);
		two.push_back(n3[i].z);
		two.push_back(1);
		two.push_back(0);
		two.push_back(0);
		two.push_back(0);
		two.push_back(0);
		two.push_back(-n2[i].x*n3[i].x);
		two.push_back(-n2[i].x*n3[i].y);
		two.push_back(-n2[i].x*n3[i].z);
		two.push_back(-n2[i].x);
		one.push_back(two);
		two.clear();
		two.push_back(0);
		two.push_back(0);
		two.push_back(0);
		two.push_back(0);
		two.push_back(n3[i].x);
		two.push_back(n3[i].y);
		two.push_back(n3[i].z);
		two.push_back(1);
		two.push_back(-n2[i].y*n3[i].x);
		two.push_back(-n2[i].y*n3[i].y);
		two.push_back(-n2[i].y*n3[i].z);
		two.push_back(-n2[i].y);
		one.push_back(two);
		two.clear();
	}
	

	for(int i = 0 ; i<12; i++)
	{
		for(int j = 0 ; j<12; j++)
		{
			tmp = 0;
			for(int k = 0 ; k<one.size();k++)
			{
				tmp += (one[k][j]*one[k][i]);
			
			}
			m1[i][j]=tmp;
		}
	}

	src = Mat(12, 12, CV_64F, m1);
	eigen(src,dist,eigenvectors);
	min=dist.at<double>(0,0);

	for(int i = 1; i < 12; i++)
	{
		if(min>dist.at<double>(i,0))
		{
			min=dist.at<double>(i,0);
			ind=i;
		}
	}
	cout<<min<<endl;
	for(int i = 0; i<12;i++)
	{
		h[i/4][i%4]=eigenvectors.at<double>(ind,i);
	}
	for(int i = 0;i<3;i++)
	{
		for(int j = 0 ; j<4 ;j++)
			cout<<setw(12)<<h[i][j];
		cout<<endl;
	}
	image = cvLoadImage("L000.bmp",CV_LOAD_IMAGE_UNCHANGED);
	
	Mat aimgmat(image, 0),imgmat;
	cvtColor(aimgmat,imgmat,CV_GRAY2BGR);
	for(int i = 0 ; i<n2.size();i++)
	{
		circle(imgmat,Point2d(n2[i].x,n2[i].y),3,Scalar( 0, 0, 255 ),-1);
	}
	double n3x,n3y;
	for(int i =0 ; i<n3.size();i++)
	{
		n3x=(n3[i].x*h[0][0]+n3[i].y*h[0][1]+n3[i].z*h[0][2]+h[0][3])/(n3[i].x*h[2][0]+n3[i].y*h[2][1]+n3[i].z*h[2][2]+h[2][3]);
		n3y=(n3[i].x*h[1][0]+n3[i].y*h[1][1]+n3[i].z*h[1][2]+h[1][3])/(n3[i].x*h[2][0]+n3[i].y*h[2][1]+n3[i].z*h[2][2]+h[2][3]);
		circle(imgmat,Point2d(n3x,n3y),3,Scalar( 0, 255, 255 ),-1);
	
	
	}
	imshow("L000.bmp", imgmat);
	//cvShowImage("L000.bmp", image);
	cvWaitKey(0); 

	cin>>a;
	return 0;
}
