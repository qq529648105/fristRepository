#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStringListModel>
#include <QUdpSocket>
#include "FindDialog.h"
#include "AxisObject.h"

#include "TMThread.h"
#include "WeightThread.h"
#include "workthread.h"
#include "visiondll.h"
#include <QTimer>
#include <QLabel>
//rong lk
#include <QToolBar>
//end


#define ROBOTS 3
namespace Ui {
    class MainWindow;
}

enum NumPadKeys {
    KeyStartProgram,
    KeyStopProgram,
    KeyPauseProgram,
    KeyResumeProgram,
    KeyOpenVM,
    KeyCloseVM,
    KeyOpenTP,
    KeyCloseTP,
    KeyResetProgram,
    KeyStepProgram,
    KeyKeyboard,
    KeyReConnect,
    KeyDisConnect,
    KeySpeedDown,
    KeySpeedUp,
    KeyGetProgram,
    KeyPutProgram,
    KeyFindProgram,
    KeyGenerateMoveJ,
    KeyGenerateWait,
    KeyNormalScreen,
    KeyFullScreen,
    KeyMeasureTool,
    KeySelectTool,
    KeyAddWarning,
    KeyRemoveWarning,
    KeyExportWarning,
    KeyImportWarning,
    KeyAddKnife,
    KeyResetKnife,
    KeyRemoveKnife,
    KeyOpenXi,
    KeyLock,
    KeyUnlock
};
enum Version{
    ZT0,//振挺第一套
    ZT1,//振挺第二套
    XL,//雪龙,新华昌
    HD,//华东
    HF,//汇丰
    TEST,//展会
    RXPZ//柔性配置

};

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

     void setCodeRow ( int row, QString programName );
     int curRobotIndex;
     int currentUdpIndex;
     int currentSocket;
     QMap<int,QStringList> regAxis;
     QMap<int,bool> vmState;
     int heartBeat,heartBeat2;
     void connectService(int connRobot=-1);
     void standardVal(int index, QString d);
     void savedDetectImg(IplImage * src,QString folderName,QString fileName);
     void savedDetectImg2(IplImage * src,QString folderName,QString fileName);
     void writeLog(QString str);
     void rebootSeviceFunc();
     double preKnifeVal;
     void setIndexInfoName(QStringList isShow,QStringList name);
     int getIndexByName(QString name);
     int getIndexByCommand(QString command);
     void setButtonF9(QString str,bool setWidget);
     void callVision(int camIndex, int num);
     QTime visionTime;
     //void manualVision();

     //rong lk
     QUdpSocket *pythonSocket;
     //end

protected:
    void changeEvent( QEvent *e );
    void timerEvent ( QTimerEvent * event );
    void keyPressEvent( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);


private:
    Ui::MainWindow *m_ui;
    QStringListModel *codeModel;
    int sysTimeTimerId;
    int panelCheckTimerId;
    int reconnectTimerId;
    int axisAutoMoveTimerId;

    QString currentStationId;

    QMap<QString, QList<QString> > signalTriggerMap, varTriggerMap, signalTriggerMap2, varTriggerMap2;
    QMap<QString, QString> signalTriggerCache, varTriggerCache, signalTriggerCache2, varTriggerCache2;
//update zhugz

    QMap<QString, QList<QString> > signalTriggerMaps[ROBOTS], varTriggerMaps[ROBOTS];
    QMap<QString, QString> signalTriggerCaches[ROBOTS], varTriggerCaches[ROBOTS];

    QMap<int,QString> operatorStates;
    QMap<int,QString> execStates;
    QMap<int,QString> connStates;
    IplImage* m_camImage;
    IplImage* m_camImage2;

    QList<IplImage *> templateImage;
    QList<cv::Mat> templateImage2;
    int currentRobotPort[ROBOTS], currentRobotPort2,knifePort;
    QUdpSocket *serviceSocket[ROBOTS], *service2Socket,*knifeSocket,*servoSocket;
    bool serviceConnected[ROBOTS], service2Connected;
    QMap<QString,bool> knifeConnected;
    QMap<int,bool> servoState;
    QList<double> cpkList[ROBOTS],knifeTmp[ROBOTS];


    QDateTime workPieceTime;
    int speedCache[ROBOTS];
    QThread *workTh;
    QString outputName1,outputName2;
    QTimer *t1;
    QString picSuff;
    QFile *file[3];
    bool fileState[3];
    bool DEBUG;
    QMap<AxisObject*, QString> autoMoveMap;
    QString programCode, programFile;
    double neddleValue[6],preNeddleValue[6],savedNeddleValue2[5],savedNeddleValue3_avg[5];
    double savedNeddleValueA[5],savedNeddleValueB[5];
    int userLevel;
    int curRow;

    workThread* work;
    QList<double> cpkArray;
    double armRatio1,armRatio2;
    QString armVarName1,armVarName2;
    QLabel *labelSetName[7];
    QLabel *labelSetValue[7];
    QMap<QString,int> textVar;
    QMap<QString,QString> varUnit;
//end


    QStringList logList;
    QStringListModel *logListModel;

    FindDialog *findDialog;

    QPoint mLastMousePosition;
    bool mMoving;

    TMThread *tmThread; // 基恩士测量线程
    WeightThread * weightThread;    // 称重线程
    QPushButton *btn_unlock;
    QLineEdit* pwd;
    QWidget* unlock;
    QStringList modelNameList;



    //ronglk
        QPushButton *btn_vertexPointSelect;
        QPushButton *btn_anyPointSelect;
        QPushButton *btn_endPointSelect;
        QPushButton *btn_middlePointSelect;
        QPushButton *btn_centrePointSelect;
        QPushButton *btn_surfaceSelect;
        QPushButton *btn_edgePointSelect;
        QPushButton *btn_edgeSelect;
        QPushButton *btn_componentSelect;

        QToolBar* compsBoxToolBar;
    //end



    void diableOtherSelectButton(int index);
    void enableOtherSelectButton(int index);

    void registerKeys();
	bool openPanel(int state);
    void triggerValueChanged(QStringList idList);
    bool startAxisAutoMove(AxisObject *axis, double value, double time);
    void loadKnifeValue();
    void initInterface();
    void writeTxt(QString fileName,QString context);
    void initLog();
    void visionSendRet(QStringList &varName,params &value,int type);

private slots:
    void on_button_F1_clicked();
    void on_button_F2_clicked();
    void on_button_F3_clicked();
    void on_button_F4_clicked();
    void on_button_F5_clicked();
    void on_button_F6_clicked();
    void on_button_F7_clicked();
    void on_button_F8_clicked();
    void on_button_F9_clicked();

    void on_pushButton_clicked();

//	void on_codeView_doubleClicked ( const QModelIndex & index );

//	void readServiceMessage();
	void processServiceMessage();
    void processServiceMessage_2();
    void processServiceMessage_3();
    void processKnife();
    void processServo();
    void processServiceMessage2();  // 第2机器人用

    void moduleAllLoaded( int count );

    void editCursorPositionChanged();

//    void processKey (Qt::Key key);
    void processKey(int id);

    void stationChanged(QString stationId);

    void unlocked();
    void VMState();

    void openService();
    void conn1();

    void runProc();

    //rong lk
    QString positionRand();
        void vertexPointSelectSlot();
        void anyPointSelectSlot();
        void endPointSelectSlot();
        void middlePointSelectSlot();
        void centrePointSelectSlot();
        void surfaceSelectSlot();
        void edgePointSelectSlot();
        void edgeSelectSlot();
        void componentSelectSlot();
        void cancelButtonDownSignal();
    //end



public slots:
    void updateUnitDataSlot( QByteArray data );
    void updateProgramRowSlot( QByteArray data );
    void addLogSlot(QString message, QString time);
    void updateSpeedRatioSlot( int speedRatio );
    void updateProcessTimeSlot( float sec );
    int mysendServiceA(const char *data, int port);
    int mysendService( const char *data, int indexRobot );
    int mysendService(const char *data,int len, int indexRobot);
    int sendService2( QByteArray data );
    int sendService2( const char *data, int len );

    int sendKnife( QByteArray data );
    void updateProgram ( QString programText );
    void updateTmMeasure( QImage *image, float values[], bool valids[] );
    void updateTmImage( QImage *image );
    void updateTmValue( QList<float> *valueList, QList<char> *valueStateList );
    void updateTmGapResult(bool result, QString params);
    void updateWeightValue( float value );


    void setLock_slot(bool x);


    void setOperatorState(int x);
    void setExecState(int x);
    void setConnState(int x);
    void setSpeedState(int x);

    void startRobot();
    void stopRobot();
    void pauseRobot();
    void resumeRobot();
    void clearWorkNum();
    void openWorkCpk();



    void servoMove(float val);
    void sendVisualRes(QString str, QString ip);
    void serviceTest(int index);
    void setTitle(QString str);
    QString getTitle();
    void showIndexText();
    //rong lk
        int sendToPythonService(const char *data,int len);
        void readRapidFromPythonService();//读取rapid信息
    //end
signals:
//	void syncPosition(double axis1, double axis2, double axis3, double axis4, double axis5, double axis6 );
    void syncPosition( QStringList posList, int index );
	void updateUnitData( QByteArray data );
	void updateProgramRow( QByteArray data );
	void syncDisplayState( int displayState, int pickCount );
	void showAll();
	void addLog(QString message, QString time);
    void updateSpeedRatio( int speedRatio );
    void updateProcessTime( float sec );
    void changeStationTab(int index);

    bool getTMMeasure(int savePage);  // savePage 0:只显示不保存，1:保存第1页，2:保存第2页，3:保存第3页
    bool getTMMeasureGap(int xStart,int yStart,int xEnd,int yEnd, int threshold, int threshod1, QString params);  //   threshold 灰度图阈值 设为40 40以上白  40以下黑    threshol1  间隙边缘阈值 ,目前设为3
//zhu
    void positionTransform();
    void click_camera();
    void autoAdjustExposure();
    void tmGap(QString,QString);


protected:
    void closeEvent(QCloseEvent *e);
    void updateCpk(QString filename,double number);
    void createCpk();
    void addCpk();
    void createCpk2();

    void addCpk2(int total, QString dt);
    void getXmlValue(QString name,QString key,QString &out);
    void writeXmlValue(QString name,QString key,QString value);
    void createTable();
    void visionAssemble(QStringList &tmpList,CvRect areaRect[],QStringList &areaRectId,int method,int camIndex);
    void visionAssemble2(QStringList &tmpList,CvRect areaRect[],QStringList &areaRectId,int method,int camIndex);
    void visionAssemble3(QStringList &tmpList,CvRect areaRect[],QStringList &areaRectId,int method,int camIndex);

    void calcTmValue(double val);
};

#endif // MAINWINDOW_H
