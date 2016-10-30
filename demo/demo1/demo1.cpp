
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cvaux.h>
#include <iostream>
#include <vector>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

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


int ptx,pty;
void MouseTouchEvent(int event, int x, int y, int flags, void* userdata);
vector <Point2d> pt;
Mat frame, frame1;
string win_in("Input"), win_out("Output");
bool drawc=false;
int DiameterSlider =1;
int main(int argc, char* argv[])
{
	VideoCapture capture;
	int width = 0, height = 0;
	bool bCapture;
	

#ifdef _CAPTURE_FROM_AVI_FILE_
	if (argc != 2) {
		fprintf(stderr, "Usage: %s input-avi-filename\n", *argv);
		exit(1);
	}
	char *AviFileName = * ++argv;
	bCapture = capture.open(AviFileName);
#else
	bCapture = capture.open(0);
#endif

	namedWindow(win_in, WINDOW_AUTOSIZE);  
	namedWindow(win_out, WINDOW_AUTOSIZE); 
	 bool bQuit = false;
	if(bCapture)
	{
		while(!bQuit)
		{
			setMouseCallback(win_out, MouseTouchEvent, NULL);
			if(capture.grab())
			{
				capture.retrieve(frame);
				createTrackbar("BrushSize ", win_out, &DiameterSlider, 30);
				capture >> frame;
				if(!frame.empty()){
					imshow(win_in, frame);
				}
				if(frame1.empty()){
					frame1 = frame.clone();
					imshow(win_out,frame1);
				}
				if(drawc){
					pt.push_back(Point2d(ptx,pty));
					for(int i = 0 ; i<pt.size()-1;i++)
					{
							line( frame1,Point2d(pt[i].x,pt[i].y),Point2d(pt[i+1].x,pt[i+1].y),cv::Scalar( 0, 0, 255 ),  DiameterSlider);
					}
					imshow(win_out,frame1);
				 }
				else{
					pt.clear();
				}


			switch (cv::waitKey(1)) {
				case 32: //space
						frame1 = frame.clone();
						imshow(win_out,frame1);
							break;
					case 27: // ESC
							bQuit = true;
							break;
				}
			}
		}
	}
	return 0;
}

/* detect mouse movement and touch position*/
void MouseTouchEvent(int event, int x, int y, int flags, void* userdata)
{
if  ( event == cv::EVENT_LBUTTONDOWN )
     {	  
		drawc=true;
     }
     else if  ( event == cv::EVENT_LBUTTONUP )
     {
		 drawc=false;
     }
	ptx = x;
	pty = y;

}