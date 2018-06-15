#ifndef GLOBAL_H
#define GLOBAL_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv.hpp>
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/legacy/legacy.hpp>
#include <QTimer>
#include <Windows.h>
#include <QUdpSocket>
#include "visiondll.h"
#include "common.h"
extern Mat g_dstImage1;
extern Mat g_dstImage2;
extern Mat g_dstImage3;
extern Mat g_dstImage4;
extern int g_nElementShape;
extern int thresh;
extern int max_thresh;
extern int offsetSign;
extern int medianBlurVal;
extern bool DEBUG;
extern IplImage *armImg;
extern int mousePosX;
extern int mousePosY;
extern Point contourP1;
extern bool hsvFg;
#endif // GLOBAL_H
