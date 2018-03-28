#ifndef CAMERACALIBRATE_H
#define CAMERACALIBRATE_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include "CaptureCamera.h"
#include "CameraProperty.h"
#include "highgui.h"
//#include "cv.h"


class CameraTemplate : public QObject
{
    Q_OBJECT
public:
    CameraTemplate(QObject* parent = 0);
    QString id;
    QString  name;       //模板名称
    int code;     //模板编号
    int type;            // 模式  0 固定摄像头  1 移动摄像头
    int leftUpX;         //模板左上角X坐标
    int leftUpY;         //模板左上角Y坐标
    int rightDownX;       //模板右下角X坐标
    int rightDownY;      //模板右下角Y坐标
    int tmpWidth;
    int tmpHeight;
    int bright;
    int robotX;
    int robotY;
    int robotZ;

};


enum SETTINGSTATE{
    AREASETTING=0,   //检测区域设置
    TMPSETTING,      //移动相机标定模式
    CENTERSETTING,    //固定相机标定模式 (中心点标定)
    MOUSESETTING
};

//检测区域显示状态
enum AREASTATE{
    ALLSHOWSTATE = 0,  //全部显示
    BESTSHOWSTATE,     //最佳显示
    AFFRAMSHOWSTATE     //确认显示
};

namespace Ui {
class CameraCalibrate;
}



class CameraCalibrate : public QWidget
{
    Q_OBJECT
    
public:
    explicit CameraCalibrate(QWidget *parent = 0);
    ~CameraCalibrate();
    //CaptureCamera *captureCamera[5];
    BaseCapture *captureCamera[5];
    QStringList ipAraay;
    QList<int> camModel;
    QList<CameraProperty *>cameraList;
    void initCamera();
    bool getCheckBox();
    float workParam[5];
    int getTemplateIndex();
    int getCamIndex(QString name);
    void setLabelText(QString &s);
    bool recordLog;
    bool isAjdust;


protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void mouseMoveEvent(QMouseEvent *);
    bool eventFilter ( QObject * obj, QEvent * event );

signals:
    void cameraResult();
    //void autoAdjustExposure();
    
private slots:
    void initSetting(const QString &solutionId);
    void switchCamera(int);
    void setStateChanged(int);
    void on_ButtonBestShow_clicked();
    void on_ButtonAllShow_clicked();
    void on_ButtonShow_clicked();
    void on_ButtonUp_clicked();
    void on_ButtonLeft_clicked();
    void on_ButtonRight_clicked();
    void on_ButtonDown_clicked();
    void on_ButtonSelectAll_clicked();
    void on_ButtonSave_clicked();

    void slotAreaSpinBoxUp(int);
    void slotAreaSpinBoxDown(int);
    void slotAreaSpinBoxLeft(int);
    void slotAreaSpinBoxRight(int);

    void on_ButtonAddTmp_clicked();
    void on_ButtonDelTmp_clicked();

    void selectTemplate();
    void refreshTemplate(int,int);
    void refreshTemplateLeftUpX(int);
    void refreshTemplateLeftUpY(int);
    void refreshTemplateRightDownX(int);
    void refreshTemplateRightDownY(int);
    void refreshCenterWidth(int);
    void refreshCenterHeight(int);
    void refreshExPosure(int);
    void refreshGain(int);
  //  void saveTemplate();
   // void cancelTemplate();
    void on_ButtonAddCenter_clicked();
    void on_ButtonDelCenter_clicked();


    void on_ButtonAffrmTmp_clicked();
//    void on_ButtonCancelTmp_clicked();
    void on_ButtonAffrmCenter_clicked();

    void on_buttonProperty_clicked();
    void on_pushButton_clicked();

    void editTmpItem(int row,int column);
    void editCenterItem(int,int);

   // void startCapCamera();
    void initIpImage();
    void getScaleValue();


    void on_pushButton_2_clicked();
    void on_pushButton_6_clicked();
    void on_checkBox_clicked();
    void on_pushButton_8_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked(bool checked);
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_12_clicked();



    void on_pushButton_13_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_14_clicked();
    void messageBox();

    void on_pushButton_11_clicked();



    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

public slots:
    void switchSolution(const QString &soultionId);  //接口函数 更换解决方案对应不同的标定方案
     void getCameraImage();
     void largerImgMove();
    // void refreshCameraImage();    //刷新摄像机拍摄状态
     void addCamera(QString &cameraId);
     void delCamera(QString &cameraId);
     void updateCamera(QString &cameraId);


     void autoAdjustExposureSlot();

private:
    Ui::CameraCalibrate *ui;
    QString currentSolutionId;
    QString currentCameraId;
    int  screenWidth;
    int  screenHeight;
    void initTemplateWidget();

    IplImage* displyIpImg;
    IplImage* largerIpImg;
    IplImage* largerIpImg2;
    IplImage* grayImage;


    QImage *cameraImg; //未缩放实际照相机拍摄的模板
    QImage *displyImg;
    QImage *largerImg;
    QImage *mainImg;






//    QImage allShowImg;   //全部显示模板镜像
   QImage realAreaImg;  //实际检测区域镜像
//    QImage realAreaShowImg; //确认显示（模板标定时）镜像
//    QImage bestShowImg;  //最佳显示模板镜像
//    QImage showImg;     //镜像幕布 用来显示
//    QImage enlargerImg;   //放大图像镜像
//    QImage largerShowImg; //放大图的幕布镜像用来显示

    AREASTATE areaShowState;
    SETTINGSTATE  settingState;

    QPoint rightUpPos;   //全图显示you上角在labelFullImage上坐标；
    QPoint areaStartPos;  //设置检测区域起始坐标
    QPoint areaEndPos;    //设置检测区域结束坐标   在showImg上坐标
    QPoint cameraStartPos;
    QPoint cameraEndPos;    //检测区域在摄像头坐标
    QPoint areatopLeft;
    QPoint areaBottomRight;
    QPoint tmpStartPos; //模版标定起始位置和结束位置 在showImg上坐标
    QPoint tmpEndPos;

    int tpLeftUpX;         //模板左上角X坐标
    int tpLeftUpY;         //模板左上角Y坐标
    int tpRightDownX;       //模板右下角X坐标
    int tpRightDownY;      //模板右下角Y坐标
    int tpWidth;
    int tpHeight;
    float scaleWidth;    //   allShowImg/realTemplateImg 比例系数
    float scaleHeight;


    bool isButtonPressd; //判断设置监测区域 和模版鼠标是否按下

    void updateAraePosValue();
    void updateCameraInfo();
    void setSysParam(QString title,QString value);
 //   void drawEnlargeImage(int x,int y);
    float areaScaleWidth;
    float areaScaleHeight;


    QString selectTmpId;
    QString selectCenterId;

    QSpinBox *spinLeftUpX;
    QSpinBox *spinLeftUpY;
    QSpinBox *spinRightDownX;
    QSpinBox *spinRightDownY;

    QSpinBox *spinWidth;
    QSpinBox *spinHeight;
    QSpinBox *spinBrightTmp;
    QSpinBox *spinBrightCenter;


    QPixmap pixMouse;

    bool isMouseIn;




//    QPoint largerAreaPos;
//     int largerAreaWidth;
//     int largerAreaHeight;






};

#endif // CAMERACALIBRATE_H
