
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

void creat_H(string inputfilename2d,string inputfilename3d,string ouputfilename)
{
	fstream fs;
	vector <Point2d> n2;
	vector <Point3d> n3;
	float a,b,c;
	Mat src,dist,eigenvectors;
	
	vector < vector<double> > one;
	vector<double> two ; 
	double tmp,min;
	double m1[12][12] ,h[3][4];
	int ind=0;

	fs.open(inputfilename2d,fstream::in);
	while(fs>>a>>b)
	{
		if(a==0&&b==0)break;
		n2.push_back(Point2d(a,b));
		
	}
	fs.close();
	fs.open(inputfilename3d,fstream::in);
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
	//cout<<min<<endl;
	for(int i = 0; i<12;i++)
	{
		h[i/4][i%4]=eigenvectors.at<double>(ind,i);
	}
	fs.open(ouputfilename,ios::out);
	for(int i = 0;i<3;i++)
	{
		for(int j = 0 ; j<4 ;j++)
		{
		//	cout<<setw(15)<<h[i][j];
			fs<<setw(15)<<h[i][j];
		}
		//cout<<endl;
		fs<<endl;
	}
	fs.close();
}
void printMat(Mat M)
{
	for(int i = 0; i < M.rows; i++)
	{
		const double* Mi = M.ptr<double>(i);
		for(int j = 0; j < M.cols; j++)
			cout<<Mi[j]<<'\t';
		cout<<endl;
	}
}

void creat_f(string h1file,string h2file,string ouputfilename)
{
	Mat h1,h2,c1,c2,MP1,Mp1,MP2,Mp2,C1,e,e2x,f;
	fstream fs;
	double num;
	double dh1[3][4],dh2[3][4],P1[3][3],p1[3][1],P2[3][3],p2[3][1];
	fs.open(h1file,fstream::in);
	int i=0;
	while(fs>>num)
	{
		dh1[i/4][i%4]=num;
		if(i%4==3)p1[i/4][0]=num;
		else
			P1[i/4][i%4]=num;
		i++;
	}
	fs.close();
	fs.open(h2file,fstream::in);
	i=0;
	while(fs>>num)
	{
		dh2[i/4][i%4]=num;
		if(i%4==3)p2[i/4][0]=num;
		else
			P2[i/4][i%4]=num;
		i++;
	}
	fs.close();
	h1 = Mat(3,4,CV_64F,dh1);
	h2 = Mat(3,4,CV_64F,dh2);
	MP1 = Mat(3, 3, CV_64F, P1);
	Mp1 = Mat(3, 1, CV_64F, p1);
	MP2 = Mat(3, 3, CV_64F, P2);
	Mp2 = Mat(3, 1, CV_64F, p2);
	C1 = -MP1.inv()*Mp1;
	//cout<<endl;
	Mat tmp = Mat(1, 1, CV_64F,1);
	C1.push_back(tmp);
	e=h2*C1;
	double e2xtmp[3][3]={{0,-e.at<double>(2,0),e.at<double>(1,0)},{e.at<double>(1,0),0,-e.at<double>(0,0)},{-e.at<double>(1,0),e.at<double>(0,0),0}};
	e2x=Mat(3, 3, CV_64F, e2xtmp);
	f=e2x*MP2*MP1.inv();
	fs.open(ouputfilename,ios::out);
	for(int i = 0; i < f.rows; i++)
	{
		const double* Mi = f.ptr<double>(i);
		for(int j = 0; j < f.cols; j++)
			fs<<setw(15)<<Mi[j];
		fs<<endl;
	}
	fs.close();
}
void demo(string Ffilename)
{


}
int main(int argc, char* argv[])
{
	//write h into file  
	/*
	creat_H("lcalib.p2d","lcalib.p3d","l_H.txt");
	creat_H("rcalib.p2d","rcalib.p3d","r_H.txt");
	*/

	
	//read h from file
	//creat_f("l_H.txt","r_H.txt","f.txt");
	



	int q;
	cin>>q;
	//cvWaitKey(0); 
	return 0;
}







