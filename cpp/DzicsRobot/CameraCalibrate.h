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
    QString  name;       //ģ������
    int code;     //ģ����
    int type;            // ģʽ  0 �̶�����ͷ  1 �ƶ�����ͷ
    int leftUpX;         //ģ�����Ͻ�X����
    int leftUpY;         //ģ�����Ͻ�Y����
    int rightDownX;       //ģ�����½�X����
    int rightDownY;      //ģ�����½�Y����
    int tmpWidth;
    int tmpHeight;
    int bright;
    int robotX;
    int robotY;
    int robotZ;

};


enum SETTINGSTATE{
    AREASETTING=0,   //�����������
    TMPSETTING,      //�ƶ�����궨ģʽ
    CENTERSETTING,    //�̶�����궨ģʽ (���ĵ�궨)
    MOUSESETTING
};

//���������ʾ״̬
enum AREASTATE{
    ALLSHOWSTATE = 0,  //ȫ����ʾ
    BESTSHOWSTATE,     //�����ʾ
    AFFRAMSHOWSTATE     //ȷ����ʾ
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
    void switchSolution(const QString &soultionId);  //�ӿں��� �������������Ӧ��ͬ�ı궨����
     void getCameraImage();
     void largerImgMove();
    // void refreshCameraImage();    //ˢ�����������״̬
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


    QImage *cameraImg; //δ����ʵ������������ģ��
    QImage *displyImg;
    QImage *largerImg;
    QImage *mainImg;






//    QImage allShowImg;   //ȫ����ʾģ�徵��
   QImage realAreaImg;  //ʵ�ʼ��������
//    QImage realAreaShowImg; //ȷ����ʾ��ģ��궨ʱ������
//    QImage bestShowImg;  //�����ʾģ�徵��
//    QImage showImg;     //����Ļ�� ������ʾ
//    QImage enlargerImg;   //�Ŵ�ͼ����
//    QImage largerShowImg; //�Ŵ�ͼ��Ļ������������ʾ

    AREASTATE areaShowState;
    SETTINGSTATE  settingState;

    QPoint rightUpPos;   //ȫͼ��ʾyou�Ͻ���labelFullImage�����ꣻ
    QPoint areaStartPos;  //���ü��������ʼ����
    QPoint areaEndPos;    //���ü�������������   ��showImg������
    QPoint cameraStartPos;
    QPoint cameraEndPos;    //�������������ͷ����
    QPoint areatopLeft;
    QPoint areaBottomRight;
    QPoint tmpStartPos; //ģ��궨��ʼλ�úͽ���λ�� ��showImg������
    QPoint tmpEndPos;

    int tpLeftUpX;         //ģ�����Ͻ�X����
    int tpLeftUpY;         //ģ�����Ͻ�Y����
    int tpRightDownX;       //ģ�����½�X����
    int tpRightDownY;      //ģ�����½�Y����
    int tpWidth;
    int tpHeight;
    float scaleWidth;    //   allShowImg/realTemplateImg ����ϵ��
    float scaleHeight;


    bool isButtonPressd; //�ж����ü������ ��ģ������Ƿ���

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
