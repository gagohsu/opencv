
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cvaux.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
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

	Mat kernel;
	Mat frame, frame1;
	string win_in("Input"), win_out("Output");
	Point anchor;
	double delta;
	int ddepth;
	namedWindow(win_in, WINDOW_AUTOSIZE);  
	namedWindow(win_out, WINDOW_AUTOSIZE); 
	bool bQuit = false;
	double m1[3][3] = { {1, 0, -1}, {2, 0, -2},{1,0,-1} };//1
	double m2[3][3] = { {1, 2, 1}, {0, 0, 0},{-1,-2,-1} }; //2
	double m3[3][3] = { {0, 1, 0}, {1, -4, 1},{0,1,0} }; //3
	double m4[3][3] = {{1,1,1},{1,1,1},{1,1,1}};//4
	double m5[3][3] = {{-1,-1,0},{-1,0,1},{0,1,1}};//5
	double m6[3][3] = {{-1,-1,-1},{-1,-1,10},{-1,-1,-1}};
	
	 
	anchor = Point( -1, -1 );
	delta = 0;
	ddepth = -1;

	if(bCapture)
	{
		while(!bQuit)
		{
			if(capture.grab())
			{
				capture.retrieve(frame); 
				capture >> frame;
				if(!frame.empty()){
					imshow(win_in, frame);
				}
				if(frame1.empty()){               
					frame1 = frame.clone();
					imshow(win_out,frame1);
				}

				switch (waitKey(1)) {
					case 32: //space
							frame1=frame;
							imshow(win_out,frame1);
							break;
					case 27: // ESC   //離開
							bQuit = true;
							break;

					case 49:
							kernel= Mat(3, 3, CV_64F, m1); 
							filter2D(frame, frame1, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
							imshow(win_out,frame1);
							break;
					case 50:
							kernel= Mat(3, 3, CV_64F, m2); 
							filter2D(frame, frame1, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
							imshow(win_out,frame1);
							break;
					case 51:
							kernel= Mat(3, 3, CV_64F, m3); 
							filter2D(frame, frame1, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
							imshow(win_out,frame1);
							break;
					case 52:
							kernel= Mat(3, 3, CV_64F, m4); 
							filter2D(frame, frame1, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
							imshow(win_out,frame1);
							break;
					case 53:
							kernel= Mat(3, 3, CV_64F, m5); 
							filter2D(frame, frame1, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
							imshow(win_out,frame1);
							break;
					case 54:
							kernel= Mat(3, 3, CV_64F, m6); 
							filter2D(frame, frame1, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
							imshow(win_out,frame1);
							break;
				}
			}
		}
	}
	return 0;
}

