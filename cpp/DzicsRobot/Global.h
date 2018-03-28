/****************************************************************************
**
**  Created By:  Daiyl  at  2015-7-3
**  Copyright (c) 2015 Dzics.com, All rights reserved.
**
****************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#include "MainWindow.h"
#include "PageStation.h"
#include "Viewer.h"

#include "ModuleObject.h"
#include "RobotObject.h"
#include "CameraCalibrate.h"
#include "KnifeWidget.h"
#include "SysManager.h"
#include <QMutex>
#include "Servo.h"
extern MainWindow *mainWindow;
extern PageStation *pageStation;
extern Viewer *pageViewer;
extern CameraCalibrate *g_camera;
extern Servo *g_servo;
extern SolutionSet * solutionSet2;
extern QMap<QString, AxisObject*> axisMap;
extern QMap<QString, ModuleObject*> moduleMap;
extern QMap<QString, RobotObject*> robotMap;
extern SysManager *g_sysManager;
extern IplImage *cameraIpImg;
extern QMap<int,IplImage*> IpImgCache;
extern QMap<int,IplImage *> IpImgArray[5];
extern QMutex mylock[5];
extern QList<Knife>KnifeList;
extern QList<Knife2>Knife2List;
extern int fNum[5];
extern bool showVisionRet[5];
extern cv::Point showRect[5];
extern QMap<QString,int> txtSequence;
extern QLabel *indexLabel[7];
extern QLabel *indexLabelName[7];
extern QStringList sysCheckList,sysIndexInfoList;
extern bool saveCache[5];
typedef unsigned char byte;

const byte CMD_POSITION = 0x10;
const byte CMD_UNIT_DATA = 0x11;
const byte CMD_PROGRAM_ROW = 0x12;
const byte CMD_DISPLAY_STATE = 0x13;
const byte CMD_PANEL = 0x14;
const byte CMD_LOG = 0x15;
const byte CMD_OPERATE_MODE = 0x16;
const byte CMD_EXECUTE_STATE = 0x17;
const byte CMD_CONNECT_STATE = 0x18;
const byte CMD_SPEED_RATIO = 0x19;
const byte CMD_PROCESS_TIME = 0x1A;
const byte CMD_SIGNAL_CHANGED = 0x1B;
const byte CMD_VAR_CHANGED = 0x1C;
const byte CMD_LOGON_RESULT = 0x1D;
const byte CMD_ERROR_NUMBER = 0x1E;
const byte CMD_GET_PROGRAM_RESULT = 0x1F;
const byte CMD_PROGRAM_GENERATE_RESULT = 0x20;
const byte CMD_POSITION_TRANSFORM_RESULT = 0x25;
const byte CMD_GT2_RESULT=0x30;
const byte CMD_WORKPIECE_COUNT=0x31;
const byte CMD_KILL_SERVICE=0x32;
const byte CMD_KNIFE_STATE_RESULT = 0x34;
const byte CMD_DETECT_STATE = 0x47;

//rong lk
//rapid指令
const byte CMD_RAPID_DATA = 0x2B;
const byte CMD_RAPID_END = 0x2A;
//end

//arm 视觉
const byte CMD_visual_001=0x50;
const byte CMD_visual_002=0x51;


//刀补
//r
const byte NC_CMD_LOG = 0x50;
const byte NC_CMD_LOGON_RESULT = 0x51;
const byte NC_CMD_CONNECT_STATE = 0x52;
const byte NC_CMD_TOOL_DATA=0x53;
//s
const byte NC_CMD_LOGON = 0xB1;
const byte NC_CMD_LOGOFF = 0xB2;
const byte NC_CMD_GET_CONNECT_STATE = 0xB3;
const byte NC_CMD_WRITE_VAR = 0xB4;
//end
const byte CMD_COMMON_DATA = 0x66;//首页文字接收

const byte CMD_LOGON = 0x81;
const byte CMD_LOGOFF = 0x82;
const byte CMD_START_VC = 0x83;
const byte CMD_STOP_VC = 0x84;
const byte CMD_START_PROGRAM = 0x85;
const byte CMD_STOP_PROGRAM = 0x86;
const byte CMD_RESET_PROGRAM = 0x87;
const byte CMD_GET_PROGRAM = 0x88;
const byte CMD_PUT_PROGRAM = 0x89;
const byte CMD_PAUSE_PROGRAM = 0x8A;
const byte CMD_RESUME_PROGRAM = 0x8B;
const byte CMD_SPEED_DOWN = 0x8C;
const byte CMD_SPEED_UP = 0x8D;
const byte CMD_REG_SIGNAL_TRIGGER = 0x8E;
const byte CMD_REG_VAR_TRIGGER = 0x8F;
const byte CMD_PROGRAM_GENERATE = 0x90;
const byte CMD_STEP_PROGRAM = 0x91;
const byte CMD_WRITE_SIGNAL = 0x92;
const byte CMD_WRITE_VAR = 0x93;
const byte CMD_RESET_WORKPIECE_COUNT = 0x94;
const byte CMD_POSITION_TRANSFORM=0x96;
const byte CMD_WRITE_VAR_QJK = 0x98;
const byte CMD_SAFEAREA_DATA = 0xE0;
const byte CMD_COMMON_TRIGGER = 0xEE;//首页文字注册

//伺服电机
const byte CMD_SERVO_DISTANCE=0x95;
const byte CMD_SERVO_STATE_RESULT = 0x33;
const byte CMD_SERVO_STATE = 0x97;
const byte CMD_KNIFE_STATE = 0x99;
//探针
const byte FCMD_TOOL_OFFSET = 0xA1;

//折弯机
const byte CMD_WRITE_VAR_PLAN = 0xA5;
const byte CMD_WRITE_VAR_MANUAL_ACTION = 0xA6;
const byte CMD_WRITE_VAR_SERVO_PARAMS = 0xA7;
const byte CMD_WRITE_VAR_PUNCH_PARAMS = 0xA9;




namespace GLOBAL {

}

#endif // GLOBAL_H
