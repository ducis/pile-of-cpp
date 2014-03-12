// lk.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>
#include <opencv/cv.h>
#include <queue>

#include "Basic.hpp"
#include "LucasKanade.hpp"
#include "OpenCVWrapping.hpp"


struct MouseEvent{
	int evt,x,y,flags;
	MouseEvent(int _evt,int _x, int _y, int _flags):evt(_evt),x(_x),y(_y),flags(_flags){}
};
std::queue<MouseEvent> g_mouseEvents;

void MouseProc(int evt,int x,int y,int flags,void *){
	g_mouseEvents.push(MouseEvent(evt,x,y,flags));
}


int _tmain(int argc, _TCHAR* argv[])
{
	using namespace cv;
	using namespace std;
	using namespace Basic;
	using namespace LucasKanade;
	bool go(true);
	wcout<<"����flow��������ͼƬ��Ĺ�����\n����tracking����Ƶ�ļ����и���\n����exit�˳�:\n";
	while(go){
		wstring cmd;
		wcin>>cmd;
		if(L"tracking"==cmd)
		{
			wcout<<"��������Ƶ�ļ���,��ball.avi:\n";
			VideoCapture capture(*istream_iterator<string>(cin));
			wcout<<"Esc-�˳�\nSpace-����/��ͣ\nr-�ص���ʼλ��\n��ͣʱ������������,�Ҽ�ȡ������������\no-����д��LK���ٺ�OpenCV�ṩ��LK���ټ��л�,��ʼ״̬������д��LK.\nRelease����OpenCV��LK���ܻ����,��ʹ��Debug��\n";
			Mat frame;
			int key(0);
			bool paused(true);
			Swap2< Mat_<float> > grayscales( (Mat_<float>()), (Mat_<float>()) );
			vector< Point_<float> > trackedPoints;
			Swap2< Mat > g( (Mat()), (Mat()) );

			capture>>frame;
			cvtColor(frame,g.Current(),CV_BGR2GRAY);
			grayscales.Current() = g.Current();
			//grayscales.Current()*=1.0f/255.0f;
			imshow("LK Video", frame);
			cvSetMouseCallback("LK Video",&MouseProc);
			while(!g_mouseEvents.empty()) g_mouseEvents.pop();
			bool useOpenCVImpl(false);

			while( 27!=(key=waitKey(5)) ){
				if(paused){
					while(!g_mouseEvents.empty()){
						const MouseEvent me(g_mouseEvents.front());
						if(CV_EVENT_LBUTTONUP==me.evt){
							trackedPoints.push_back( Point_<float>(me.x,me.y) );
						}
						if(CV_EVENT_RBUTTONUP==me.evt){
							trackedPoints.clear();
						}
						g_mouseEvents.pop();
					}
				}else{
					capture >> frame;
					if (!frame.empty()){
						grayscales.Swap();
						g.Swap();
						
						cvtColor(frame,g.Current(),CV_BGR2GRAY);
						grayscales.Current() = g.Current();

						Rect_< float > r(Point_<float>(0,0),grayscales.Current().size());
						r.height-=1.0f;
						r.width-=1.0f;
						if(useOpenCVImpl){
							vector< Point_<float> > t;
							vector<uchar>  status;
							vector<float>  err;
							calcOpticalFlowPyrLK(g.Previous(), g.Current(), trackedPoints, t, status, err);
							trackedPoints.swap(t);
						}else{
							for(std::vector< Point_<float> >::iterator itr(trackedPoints.begin()); itr!=trackedPoints.end(); ++itr){
								if(itr->inside(r)){
									std::pair<float,float> r(
										IterativeOpticalFlowAtPoint
										(
										grayscales.Previous(),
										grayscales.Current(),
										Constant2D<size_t,size_t>(Unity()),
										ConstantSize<5>(),
										ConstantSize<20>(),
										itr->y,
										itr->x
										)
										);
									*itr += Point_<float>(r.second,r.first);
								}
							}
						}
					}
				}
				if(' '==key){
					paused = !paused;
					cvSetMouseCallback("LK Video",paused?&MouseProc:NULL);
					while(!g_mouseEvents.empty()) g_mouseEvents.pop();
				}
				if('o'==key){
					useOpenCVImpl = !useOpenCVImpl;
					if(useOpenCVImpl){
						wcout<<"�л���OpenCV�ṩ��LK����\n";
					}else{
						wcout<<"�л�����д��LK����\n";
					}
				}
				if('r'==key){
					paused = true;
					capture.set(CV_CAP_PROP_POS_FRAMES, 0);
					capture>>frame;
					cvtColor(frame,g.Current(),CV_BGR2GRAY);
					grayscales.Current() = g.Current();

					//grayscales.Current()*=1.0f/255.0f;
					cvSetMouseCallback("LK Video",&MouseProc);
					while(!g_mouseEvents.empty()) g_mouseEvents.pop();
					trackedPoints.clear();
				}
				if (!frame.empty()){
					Mat painted(frame.clone());
					for(std::vector< Point_<float> >::const_iterator itr(trackedPoints.begin()); itr!=trackedPoints.end(); ++itr){
						circle(painted,*itr,3,Scalar(0,0,255),-1);
					}
					imshow("LK Video", painted);
				}else{
					namedWindow("LK Video");
				}
			}
			cvDestroyWindow("LK Video");
		}else if(L"flow" == cmd){
			wcout<<"�������һ��ͼƬ���ļ���,��1.png\n";
			Mat_<unsigned char> i1uint8(imread( (*istream_iterator<string>(cin)), 0 ));
			wcout<<"������ڶ���ͼƬ���ļ���,��2.png\n";
			Mat_<unsigned char> i2uint8(imread( (*istream_iterator<string>(cin)), 0 ));
			wcout<<"ע��Debug��Ĺ����ٶȺ���\n";

			Mat_<float> i1f( i1uint8 );
			Mat_<float> i2f( i2uint8 );
			i1f*=1.0f/255.0f;
			i2f*=1.0f/255.0f;
			int sx( min( i1f.size().height, i2f.size().height ) );
			int sy( min( i1f.size().width, i2f.size().width ) );
			
			Mat_< float > vmx( sx,sy ), vmy( sx,sy );
			OpticalFlow(vmx,vmy,i1f,i2f,Constant2D<size_t,size_t>(Unity()),ConstantSize<2>());
			vmx*=0.5f;
			vmy*=0.5f;
			vmx+=0.5f;
			vmy+=0.5f;
			//OpticalFlow(
			//	vmx,
			//	vmy,
			//	ResultCast2D<int>(i1uint8),
			//	ResultCast2D<int>(i2uint8),
			//	Constant2D<size_t,size_t>(Unity()),
			//	ConstantSize<2>()
			//	);
			//imshow(	"OpticalFlow", vm );

			Mat_<float> vxs( sx, sy ), vys( sx, sy );
			//CvMat* pvxs = cvCreateMat(sx,sy,CV_32FC1);
			//CvMat* pvys = cvCreateMat(sx,sy,CV_32FC1);
			CvSize sz;
			sz.height = 5;
			sz.width = 5;
			cvCalcOpticalFlowLK(&CvMat(i1uint8),&CvMat(i2uint8),sz,&CvMat(vxs),&CvMat(vys));
			vxs*=0.25f;
			vys*=0.25f;
			vxs+=0.5f;
			vys+=0.5f;
			wcout<<"����Esc�˳�\n";
			while(27!=waitKey(200)){
				imshow( "First", i1uint8 );
				imshow( "Second", i2uint8 );
				imshow( "My LK Vertical",vmx );
				imshow( "My LK Horizontal",vmy );
				imshow("OpenCV LK Horizontal",vxs);
				imshow("OpenCV LK Vertical",vys);
			}
			cvDestroyWindow( "My LK Vertical" );
			cvDestroyWindow( "My LK Horizontal" );
			cvDestroyWindow("OpenCV LK Horizontal");
			cvDestroyWindow("OpenCV LK Vertical");
			cvDestroyWindow( "First");
			cvDestroyWindow( "Second");
			//CvMat c1(i1uint8);
			//CvMat c2(i2uint8);
			//cvCalcOpticalFlowLK(&c1,&c2,sz,pvxs,pvys);
			//cvShowImage("OpenCVLKX",pvxs);
			//cvShowImage("OpenCVLKY",pvys);
		}else if(L"exit"==cmd){
			go = false;
		}
	}
	return 0;
}
