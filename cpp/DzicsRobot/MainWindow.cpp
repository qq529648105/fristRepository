#include "MainWindow_all.h"



//SimulationPlanner planner;
//Interpreter interp(&planner);

//struct KnifeBlade
//{
//    QString id;
//    QString name;
//    std::vector<gp_Pnt2d> bladePolygon;
//    std::map<QString, double> originOffsetMap;    // 每轴的零坐标偏移值
//};
//std::map<std::string, KnifeBlade*> knifeBladeMap;
//std::string currentKnifeBlade;
//
//struct Product
//{
//    QString id;
//    QString name;
//    QString codeList;
//    std::vector<std::string> knifeBladeList;
//    std::vector<std::string> cuttingKnifeBladeList;
//    gp_Pnt programOrigin;   // 编程原点
//    QString roughId;    // 毛坯
////    double originOffset[5];    // 零坐标偏移值
//};
//std::map<QString, Product*> productMap;
//std::string currentProduct;

//#include <QSettings>
//QString robotId, robotPrg, robotPath;

//struct AutoMoveStruct
//{
//    AxisObject *axis;
//    double target;
//    double time;
//};
//QList<AutoMoveStruct> autoMoveList;

//long autoMoveLastTime = 0;



QProcess *tpPanel = NULL;

Viewer *pageViewer = NULL;
PageStation *pageStation = NULL;
SolutionSet * solutionSet2;
QLabel *mainImage = NULL;
int version;

extern bool isOpenXLContoller;
void MainWindow::initInterface()
{
    userLevel=0;
    if(DEBUG)
    {
        userLevel=2;


    }
    QSqlQuery query;
    QString sql="select code,value from SysParam";
    query.exec(sql);
    if(query.last())
    {
        if(query.value(0).toString()=="title")
        {
            m_ui->label_title->setText(query.value(1).toString());
        }
    }

    m_ui->weightLabel_->setText("");
    m_ui->weightLabel->setText("");
    m_ui->directLabel_->setText("");
    m_ui->directLabel->setText("");
    m_ui->labelInput1->setText("");
    m_ui->labelInput2->setText("");
    m_ui->labelInput3->setText("");
    m_ui->labelInput4->setText("");
    m_ui->labelOutput1->setText("");
    m_ui->labelOutput2->setText("");
    m_ui->labelOutput3->setText("");
    m_ui->labelOutput4->setText("");
    m_ui->label_30->setText("");
    m_ui->label_31->setText("");
    m_ui->label_32->setText("");
    m_ui->label_33->setText("");
    m_ui->label_16->setText("");
    m_ui->label_9->setText("");
    m_ui->label_15->setText("");
    m_ui->label_14->setText("");

    m_ui->label->setText("测量值1:");
    m_ui->label_4->setText("测量值2:");
    m_ui->label_6->setText("测量值3:");
    m_ui->label_10->setText("测量值4:");
    m_ui->label_17->setText("测量值5:");
    m_ui->label_19->setText("测量值6:");
    m_ui->label_25->setText("标准值1:");
    m_ui->label_27->setText("标准值2:");
    m_ui->label_29->setText("标准值3:");
    m_ui->label_35->setText("标准值4:");
    m_ui->label_37->setText("标准值5:");
    m_ui->label_39->setText("标准值6:");

    m_ui->label_21->setText("");
    m_ui->label_23->setText("");
    m_ui->label_41->setText("");
    m_ui->label_43->setText("");

    m_ui->labelMeasure7->setText("");
    m_ui->labelMeasure8->setText("");
    m_ui->labelMeasure15->setText("");
    m_ui->labelMeasure16->setText("");

    m_ui->infoLayout->setContentsMargins(0,29,0,0);
    m_ui->outputLayout->setContentsMargins(0,29,0,0);
    m_ui->formLayout->setContentsMargins(0,29,0,0);

    /*

    m_ui->maodingLabel_->setText("工件个数");
    m_ui->maodingLabel->setText("0");




    if(version<=ZT1)
    {

        m_ui->weightLabel_->setText("");
        m_ui->weightLabel->setText("");
        m_ui->directLabel_->setText("");
        m_ui->directLabel->setText("");
        m_ui->labelInput1->setText("");
        m_ui->labelInput2->setText("");
        m_ui->labelInput3->setText("");
        m_ui->labelInput4->setText("");
        m_ui->labelOutput1->setText("");
        m_ui->labelOutput2->setText("");
        m_ui->labelOutput3->setText("");
        m_ui->labelOutput4->setText("");
        m_ui->label_30->setText("");
        m_ui->label_31->setText("");
        m_ui->label_32->setText("");
        m_ui->label_33->setText("");
        m_ui->label_16->setText("");
        m_ui->label_9->setText("");
        m_ui->label_15->setText("");
        m_ui->label_14->setText("");

        m_ui->label->setText("测量值1:");
        m_ui->label_4->setText("测量值2:");
        m_ui->label_6->setText("测量值3:");
        m_ui->label_10->setText("测量值4:");
        m_ui->label_17->setText("测量值5:");
        m_ui->label_19->setText("测量值6:");
        m_ui->label_25->setText("标准值1:");
        m_ui->label_27->setText("标准值2:");
        m_ui->label_29->setText("标准值3:");
        m_ui->label_35->setText("标准值4:");
        m_ui->label_37->setText("标准值5:");
        m_ui->label_39->setText("标准值6:");

        m_ui->label_21->setText("");
        m_ui->label_23->setText("");
        m_ui->label_41->setText("");
        m_ui->label_43->setText("");

        m_ui->labelMeasure7->setText("");
        m_ui->labelMeasure8->setText("");
        m_ui->labelMeasure15->setText("");
        m_ui->labelMeasure16->setText("");

        m_ui->infoLayout->setContentsMargins(0,29,0,0);
        m_ui->outputLayout->setContentsMargins(0,29,0,0);
        m_ui->formLayout->setContentsMargins(0,29,0,0);


    }

    if(version<=ZT1){

        m_ui->yuanxinjuLabel_->setText("");
        m_ui->yuanxinjuLabel->setText("");

        m_ui->weightLabel_->setText("");
        //m_ui->weightLabel->setText("0");

        m_ui->directLabel_->setText("");
        //m_ui->directLabel->setText("0");
        m_ui->button_F9->setText("刀补设置");
        m_ui->label_25->setText("标准值1:");
        m_ui->label_27->setText("标准值2:");
        m_ui->label_29->setText("标准值3:");
        m_ui->label_35->setText("标准值4:");
        m_ui->label_37->setText("标准值5:");
        m_ui->label_39->setText("标准值6:");
        //       m_ui->label->setText("探针值1:");
        //        m_ui->label_4->setText("探针值2:");
        //        m_ui->label_6->setText("探针值3:");
        //        m_ui->label_10->setText("气密值1:");
        //        m_ui->label_17->setText("气密值2:");
        //        m_ui->label_19->setText("气密值3:");

    }
    else if(version==XL)
    {

        m_ui->button_F9->setText("伺服电机");
        //m_ui->maodingLabel_->setText("");
        //m_ui->maodingLabel->setText("");
        m_ui->maodingLabel_->setGeometry(5,0,100,30);
        m_ui->checkDataLabel_->setText("");
        m_ui->checkDataLabel->setText("");
        m_ui->weightLabel_->setText("");
        m_ui->weightLabel->setText("");
        m_ui->directLabel_->setText("");
        m_ui->directLabel->setText("");
        m_ui->yuanxinjuLabel->setText("");
        m_ui->yuanxinjuLabel_->setText("");
        if(userLevel<2)
            processKey(KeyLock);

    }else if(version==TEST)
    {
        m_ui->button_F9->setText("刀补设置");
        m_ui->maodingLabel_->setText("上次数据");
        m_ui->maodingLabel->setText("000.000mm");
        m_ui->yuanxinjuLabel_->setText("标准值");
        m_ui->yuanxinjuLabel->setText("000.000mm");
        m_ui->weightLabel_->setText("差值");
        m_ui->weightLabel->setText("000.000mm");
        //        m_ui->maodingLabel_->setText("");
        //        m_ui->maodingLabel->setText("");

        //        m_ui->yuanxinjuLabel_->setText("");
        //        m_ui->yuanxinjuLabel->setText("");
        //m_ui->weightLabel_->setText("");
        //m_ui->weightLabel->setText("");
        m_ui->directLabel_->setText("");
        m_ui->directLabel->setText("");
    }
    else if(version==HD)
    {

        m_ui->yuanxinjuLabel_->setText("");
        m_ui->yuanxinjuLabel->setText("");
        m_ui->weightLabel_->setText("");
        m_ui->weightLabel->setText("");
        m_ui->directLabel_->setText("");
        m_ui->directLabel->setText("");
        m_ui->button_F9->setText("远程协助");
        m_ui->button_F9->setEnabled(false);
        m_ui->maodingLabel_->setText("最大直径");
        m_ui->maodingLabel->setText("000.000mm");
        m_ui->checkDataLabel_->setText("最小直径");
        m_ui->checkDataLabel->setText("000.000mm");

    }else if(version==HF)
    {
        m_ui->button_F9->setText("远程协助");
        m_ui->button_F9->setEnabled(false);
    }else if(version==RXPZ)
    {
        m_ui->button_F9->setText("柔性配置");
    }
    */

    labelSetName[0]=m_ui->label_8;
    labelSetName[1]=m_ui->label_3;
    labelSetName[2]=m_ui->checkDataLabel_;
    labelSetName[3]=m_ui->maodingLabel_;
    labelSetName[4]=m_ui->yuanxinjuLabel_;
    labelSetName[5]=m_ui->weightLabel_;
    labelSetName[6]=m_ui->directLabel_;

    labelSetValue[0]=m_ui->speedLabel;
    labelSetValue[1]=m_ui->processTimeLabel;
    labelSetValue[2]=m_ui->checkDataLabel;
    labelSetValue[3]=m_ui->maodingLabel;
    labelSetValue[4]=m_ui->yuanxinjuLabel;
    labelSetValue[5]=m_ui->weightLabel;
    labelSetValue[6]=m_ui->directLabel;

    //showIndexText();


    if(unlock==NULL)
    {
        unlock=new QWidget();

        QLabel* label=new QLabel("密码：");
        label->setParent(unlock);
        label->setGeometry(20,20,150,30);
        pwd=new QLineEdit(unlock);

        pwd->setGeometry(200,20,150,30);

        pwd->setEchoMode(QLineEdit::Password);

        btn_unlock=new QPushButton("解锁");

        btn_unlock->setParent(unlock);
        btn_unlock->setGeometry(20,70,100,30);
        connect(btn_unlock,SIGNAL(clicked()),this,SLOT(unlocked()));
    }





    QString f9Str=SysUtils::getSysParam("f9_Set").toString();

    setButtonF9(f9Str,false);
    if(f9Str=="3"||f9Str=="4")
    {
        m_ui->numPad->lockKey();
        m_ui->button_F2->setEnabled(false);
        m_ui->button_F3->setEnabled(false);
        m_ui->button_F4->setEnabled(false);
        m_ui->button_F5->setEnabled(false);
        m_ui->button_F6->setEnabled(false);
        m_ui->button_F7->setEnabled(false);
        m_ui->button_F8->setEnabled(false);

        g_servo->lockServo();

    }



m_ui->dockWidget->hide();
}

void MainWindow::writeTxt(QString fileName, QString context)
{
    QFile f(fileName);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
    {
        emit addLog("txt打开失败","");
        return;
    }

    QTextStream txtOutput(&f);
    txtOutput <<context.toLocal8Bit();

    f.close();
}

void MainWindow::initLog()
{
    //log
    QDir dir("d:/dzLog");
    dir.mkdir("d:/dzLog");

    dir.setFilter(QDir::Files);
    QFileInfoList logList= dir.entryInfoList();

    for(int i=0;i<logList.size()-30;i++)
    {
        QFile::remove("d:/dzLog/"+logList[i].fileName());
    }


    QString fileName="d:/dzLog/"+QDateTime::currentDateTime().toString("yy-MM-dd_log0")+".txt";

    file[0]=new QFile(fileName);
    if(QFile::exists(fileName))
    {
        fileState[0]=file[0]->open(QIODevice::WriteOnly | QIODevice::Append);
    }
    else
    {
        fileState[0]=file[0]->open(QIODevice::WriteOnly | QIODevice::Truncate);
    }

    file[1]=new QFile("d:/dzLog/data.txt");
    if(file[1]->size()>1024*32)
    {
        fileState[1]=file[1]->open(QIODevice::WriteOnly | QIODevice::Truncate);
    }
    else
    {
        fileState[1]=file[1]->open(QIODevice::WriteOnly | QIODevice::Append);
    }

    //end log
}



void MainWindow::registerKeys()
{
    m_ui->numPad->registerKey(KeyStartProgram, Qt::Key_0, "运行指令");
    m_ui->numPad->registerKey(KeyStopProgram, Qt::Key_Period, "停止");
    m_ui->numPad->registerKey(KeyPauseProgram, Qt::Key_0, "暂停运行", Qt::blue, false);
    m_ui->numPad->registerKey(KeyResumeProgram, Qt::Key_0, "恢复运行", Qt::red, false);
    m_ui->numPad->registerKey(KeyOpenVM, Qt::Key_1, "虚拟机开");
    m_ui->numPad->registerKey(KeyCloseVM, Qt::Key_1, "虚拟机关", Qt::blue, false);
    m_ui->numPad->registerKey(KeyOpenTP, Qt::Key_2, "示教器开");
    m_ui->numPad->registerKey(KeyCloseTP, Qt::Key_2, "示教器关", Qt::blue, false);
    m_ui->numPad->registerKey(KeyResetProgram, Qt::Key_3, "复位指令");
    m_ui->numPad->registerKey(KeyStepProgram, Qt::Key_4, "单步调试");

    m_ui->numPad->registerKey(KeyGetProgram, Qt::Key_4, "获取指令", Qt::black, false);
    m_ui->numPad->registerKey(KeyPutProgram, Qt::Key_5, "应用指令", Qt::black, false);
    //m_ui->numPad->registerKey(KeyFindProgram, Qt::Key_5, "查找指令", Qt::black, false);
    //m_ui->numPad->registerKey(KeyOpenXi, Qt::Key_5, "西门子");
    m_ui->numPad->registerKey(KeyGenerateMoveJ, Qt::Key_7, "插入\n移动指令", Qt::black, false);
    m_ui->numPad->registerKey(KeyGenerateWait, Qt::Key_8, "插入\n等待指令", Qt::black, false);

    m_ui->numPad->registerKey(KeyNormalScreen, Qt::Key_9, "窗口显示");
    m_ui->numPad->registerKey(KeyFullScreen, Qt::Key_9, "全屏显示", Qt::blue, false);
    m_ui->numPad->registerKey(KeyMeasureTool, Qt::Key_8, "距离测量");
    m_ui->numPad->registerKey(KeySelectTool, Qt::Key_7, "捕捉曲线\n生成路径");
    m_ui->numPad->registerKey(KeyKeyboard, Qt::Key_NumLock, "屏幕键盘");
    m_ui->numPad->registerKey(KeyReConnect, Qt::Key_Slash, "重新连接");
    m_ui->numPad->registerKey(KeyDisConnect, Qt::Key_Asterisk, "断开连接");
    m_ui->numPad->registerKey(KeySpeedDown, Qt::Key_Minus, "减速");
    m_ui->numPad->registerKey(KeySpeedUp, Qt::Key_Plus, "加速");

    m_ui->numPad->registerKey(KeyAddWarning, Qt::Key_4, "增加告警", Qt::black, false);
    m_ui->numPad->registerKey(KeyRemoveWarning, Qt::Key_5, "删除告警", Qt::black, false);
    m_ui->numPad->registerKey(KeyImportWarning, Qt::Key_7, "导入告警", Qt::black, false);
    m_ui->numPad->registerKey(KeyExportWarning, Qt::Key_8, "导出告警", Qt::black, false);

    m_ui->numPad->registerKey(KeyAddKnife, Qt::Key_4, "增加刀具", Qt::black, false);
    m_ui->numPad->registerKey(KeyResetKnife, Qt::Key_5, "重置刀具", Qt::black, false);
    m_ui->numPad->registerKey(KeyRemoveKnife, Qt::Key_6, "删除刀具", Qt::black, false);

    m_ui->numPad->registerKey(KeyLock, Qt::Key_5, "锁定屏幕", Qt::black);
    m_ui->numPad->registerKey(KeyUnlock, Qt::Key_6, "解锁屏幕", Qt::black);
}


MainWindow::MainWindow(QWidget *parent) :
    //    QWidget(parent),
    QWidget(parent, Qt::FramelessWindowHint),
    //    QWidget(parent, Qt::X11BypassWindowManagerHint),
    m_ui(new Ui::MainWindow)
{
    //version
    QString strVer=SysUtils::getSysParam("projectVersion").toString();
    if(strVer=="ZT1")
        version=ZT1;
    else if(strVer=="XL")
        version=XL;
    else if(strVer=="HD")
        version=HD;
    else if(strVer=="HF")
        version=HF;
    else if(strVer=="TEST")
        version=TEST;
    else if(strVer=="RXPZ")
        version=RXPZ;
    else
        version=ZT1;



    work=new workThread;
    workTh=new QThread(this);
    work->moveToThread(workTh);
    t1=new QTimer(this);
    connect(t1,SIGNAL(timeout()),work,SLOT(serviceTimer()));
    connect(work,SIGNAL(addLog(QString,QString)), this, SLOT(addLogSlot(QString,QString)), Qt::QueuedConnection);
    connect(work,SIGNAL(reConn()),this,SLOT(conn1()));

    workTh->start();


    curRobotIndex=-1;
    //r_num=0;

    service2Socket = NULL;

    //rong lk
        pythonSocket = NULL;
    //end
    initLog();

    for(int i=0;i<ROBOTS;i++)
    {
        serviceConnected[i] = false;
        serviceSocket[i] = NULL;
        speedCache[i]=100;




    }
    for(int i=0;i<KnifeList.size();i++)
    {
        knifeConnected.insert(KnifeList.at(i).name1,false);
    }
    service2Connected = false;

    this->move(0,0);

    m_ui->setupUi(this);

    mMoving = false;
    setMouseTracking( true );

    mainImage = m_ui->mainImage;
    mainImage->setVisible(false);

    pageStation = m_ui->page_6;
    pageViewer = m_ui->viewer;

    solutionSet2=new SolutionSet();
    registerKeys();

    logListModel = new QStringListModel(logList);
    m_ui->listMessage->setModel(logListModel);



    //    this->resize(1024, 768);
    //      this->resize(1680, 1050);

    //	QSettings *configIniRead = new QSettings("D:\\DzicsRobot\\ROBOT.INI", QSettings::IniFormat);
    //	robotId = configIniRead->value("/Robot/SYSID").toString();
    //	robotPrg = configIniRead->value("/Robot/PRG").toString();
    //	robotPath = configIniRead->value("/Robot/PATH").toString();
    //	delete configIniRead;

    m_ui->tSysTime->setText(QDateTime::currentDateTime().toString(tr("MM-dd hh:mm")));

    sysTimeTimerId = startTimer(60000);

    currentStationId = QString.null;

    //    QString lastStationId = SysUtils::getSysParam( "lastStationId" ).toString();
    //    if (!lastStationId.isEmpty()) {
    //        currentStationId = lastStationId;
    //        this->stationChanged(lastStationId);
    //    }


    //    QStringList code = QString("N0010 G00 X80 Z20 M03;"
    //                        "N0020 G00 X0 Z0 S2000;"
    //                        "N0030 G01 Z-0.3 F100;"
    //                        "N0040 X-6;"
    //                        "N0050 Z-0.3;"
    //                        "N0060 X-1.0 F1500;"
    //                        "N0070 Z-5.1 F100;"
    //                        "N0080 G00 X80 Z20;"
    //                        "N0090 G04 P1;"
    //                        "N0100 M30").split(";");
    //    QStringList code = QString( SysUtils::readFile("MainModule.mod") ).replace("\t", " ").split('\n');

    //	programCode = QString( SysUtils::readFile(robotPrg) );

    //	programCode = QString( SysUtils::readFile("D:\\DzicsRobot\\Program\\"+robotId.mid(1, 36)+"\\MainModule.mod") );
    //	codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
    //    m_ui->codeView->setModel(codeModel);
    //    m_ui->codeView->setCurrentIndex(codeModel->index(-1));

    //    KnifeBlade *knifeBlade = new KnifeBlade;
    //    currentKnifeBlade = "1001";
    //
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -23.17));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(25.0, -22.17));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(11.5, -2.47));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
    ////                    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -50.0));
    ////                    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
    ////                    knifeBlade->bladePolygon.push_back(gp_Pnt2d(1.1, 0.0));
    ////                    knifeBlade->bladePolygon.push_back(gp_Pnt2d(10.0, -50.0));
    ////                    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -50.0));
    //
    //    knifeBlade->originOffsetMap["X"] = 20.0;
    //    knifeBlade->originOffsetMap["Z"] = -204.0;
    //
    //    knifeBladeMap[currentKnifeBlade] = knifeBlade;
    //
    //
    //
    //
    //
    //
    //    //＊圆柱
    //    code = QString( SysUtils::readFile("testCircle.nc") ).split('\n');
    //    codeModel->setStringList( code );
    //    m_ui->codeView->setCurrentIndex(codeModel->index(-1));
    //    m_ui->codeView->scrollToTop();
    //
    //    knifeBlade = new KnifeBlade;
    //    currentKnifeBlade = "1002";
    //
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -23.17));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(25.0, -22.17));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(11.5, -2.47));
    //    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));

    ////    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -50.0));
    ////    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
    ////    knifeBlade->bladePolygon.push_back(gp_Pnt2d(1.1, 0.0));
    ////    knifeBlade->bladePolygon.push_back(gp_Pnt2d(10.0, -50.0));
    ////    knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -50.0));

    //    knifeBlade->originOffsetMap["X"] = 10.0;
    //    knifeBlade->originOffsetMap["Z"] = -94.1;
    //
    //    knifeBladeMap[currentKnifeBlade] = knifeBlade;
    //
    ////                programOrigin = gp_Pnt(109.9, 0.0, 0.0  );
    //
    //    planner.updateBlade(knifeBlade->bladePolygon);

    ////	connect(this, SIGNAL(syncPosition(double,double,double,double,double,double)), ((PageWork*)m_ui->stackedWidget->widget(0)), SIGNAL(syncPosition(double,double,double,double,double,double)), Qt::QueuedConnection);
    //	connect(this, SIGNAL(syncPosition(QStringList)), ((PageWork*)m_ui->stackedWidget->widget(0)), SIGNAL(syncPosition(QStringList)), Qt::QueuedConnection);//, Qt::QueuedConnection);
    ////	connect(this, SIGNAL(syncPosition(double,double,double,double,double,double)), ((PageWork*)m_ui->stackedWidget->widget(0))->viewer, SLOT(syncPosition(double,double,double,double,double,double)), Qt::QueuedConnection);
    //	connect(this, SIGNAL(syncDisplayState( int, int )), ((PageWork*)m_ui->stackedWidget->widget(0)), SIGNAL(syncDisplayState( int, int )), Qt::QueuedConnection);

    connect(this, SIGNAL(updateUnitData(QByteArray)), this, SLOT(updateUnitDataSlot(QByteArray)), Qt::QueuedConnection);
    connect(this, SIGNAL(addLog(QString,QString)), this, SLOT(addLogSlot(QString,QString)), Qt::QueuedConnection);
    for(int i=0;i<m_ui->page_8->ipAraay.length();i++)
    {
        connect(m_ui->page_8->captureCamera[i],SIGNAL(addLog(QString,QString)), this, SLOT(addLogSlot(QString,QString)), Qt::QueuedConnection);

    }

    connect(this, SIGNAL(updateSpeedRatio(int)), this, SLOT(updateSpeedRatioSlot(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(updateProcessTime(float)), this, SLOT(updateProcessTimeSlot(float)), Qt::QueuedConnection);


    ////	connect(m_ui->codeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)), Qt::QueuedConnection);

    connect(pageViewer, SIGNAL(moduleAllLoaded(int)), this, SLOT(moduleAllLoaded(int)), Qt::AutoConnection);
    connect(this, SIGNAL(showAll()), pageViewer, SLOT(showAll()), Qt::AutoConnection);
    connect(pageViewer, SIGNAL(updateProgram(QString)), this, SLOT(updateProgram(QString)), Qt::AutoConnection);

    connect(((PageStation*)m_ui->page_6), SIGNAL(stationChanged(QString)), this, SLOT(stationChanged(QString)), Qt::QueuedConnection);
    connect(((PageStation*)m_ui->page_6), SIGNAL(stationChanged(QString)), pageViewer, SLOT(stationChanged(QString)), Qt::QueuedConnection);
    connect(((PageStation*)m_ui->page_6), SIGNAL(importModuleFile(QString,QString,QString)), pageViewer, SLOT(importModuleFile(QString,QString,QString)), Qt::QueuedConnection);
    connect(((PageStation*)m_ui->page_6), SIGNAL(moduleChanged(QString,bool)), pageViewer, SLOT(moduleChanged(QString,bool)), Qt::QueuedConnection);
    connect(((PageStation*)m_ui->page_6), SIGNAL(addLog(QString,QString)), this, SLOT(addLogSlot(QString,QString)), Qt::QueuedConnection);

    //connect(pageViewer, SIGNAL(shapeSelected(QString)), ((PageStation*)m_ui->page_6), SLOT(shapeSelected(QString)), Qt::QueuedConnection);
    //rong lk
    connect(pageViewer, SIGNAL(shapeSelected(QStringList, bool)), ((PageStation*)m_ui->page_6), SLOT(shapeSelected(QStringList, bool)), Qt::QueuedConnection);
    //end
    connect(this, SIGNAL(changeStationTab(int)), ((PageStation*)m_ui->page_6), SLOT(changeStationTab(int)), Qt::QueuedConnection);
    connect(pageViewer, SIGNAL(moduleAllLoaded(int)), ((PageStation*)m_ui->page_6), SLOT(moduleAllLoaded(int)), Qt::QueuedConnection);

    connect(m_ui->numPad, SIGNAL(processKey(int)), this, SLOT(processKey(int)), Qt::DirectConnection);

    connect(this, SIGNAL(syncPosition(QStringList,int)), pageViewer, SLOT(syncPosition(QStringList,int)), Qt::DirectConnection);
    //    connect(this, SIGNAL(syncDisplayState( int, int )), pageViewer, SLOT(syncDisplayState( int, int )), Qt::DirectConnection);
    connect(this, SIGNAL(showAll()), pageViewer, SLOT(showAll()), Qt::QueuedConnection);


    //    m_ui->page_3->set
    //    ((PageWork*)m_ui->stackedWidget->widget(0))->updateAxisValue( axisSign, value );

    this->setWindowState(Qt::WindowFullScreen);
    this->showFullScreen();

    findDialog = NULL;
    unlock=NULL;
    //    emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  正在加载方案，请稍候...", QString());




    currentRobotPort[0] = 8566;
    currentUdpIndex=0;
    currentRobotPort2 = 9566;
    //刀补
    knifePort=8899;
    knifeSocket=new QUdpSocket(this);
    //knifeSocket->bind(18899, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    knifeSocket->bind(18899,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    //    servoSocket=new QUdpSocket(this);
    //    servoSocket->bind(18905, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    //    for(int i=0;i<ROBOTS;i++)
    //    {
    //        servoState[i]=false;
    //    }
    //m_ui->button_F8->setEnabled(false);
    //m_ui->button_F9->setEnabled(false);
    for(int i=0;i<ROBOTS;i++)
    {
        serviceSocket[i] = new QUdpSocket(this);
        if(version==ZT0){
            serviceSocket[i]->bind(QHostAddress("192.168.128.221"),18566+i);
        }else
            serviceSocket[i]->bind(18566+i, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    }
    connect(serviceSocket[0], SIGNAL(readyRead()), this, SLOT(processServiceMessage()));
    connect(serviceSocket[1], SIGNAL(readyRead()), this, SLOT(processServiceMessage_2()));
    connect(serviceSocket[2], SIGNAL(readyRead()), this, SLOT(processServiceMessage_3()));

    connect(knifeSocket, SIGNAL(readyRead()), this, SLOT(processKnife()));
    //connect(servoSocket, SIGNAL(readyRead()), this, SLOT(processServo()));

    service2Socket = new QUdpSocket(this);
    service2Socket->bind(19566, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(service2Socket, SIGNAL(readyRead()), this, SLOT(processServiceMessage2()));

    //	serviceSocket->connectToHost(QHostAddress::LocalHost, 8566);


    //                processKey(Qt::Key_Slash);

    tmThread = new TMThread(mainImage->width(), mainImage->height());
    connect(tmThread,SIGNAL(addLog(QString,QString)), this, SLOT(addLogSlot(QString,QString)), Qt::QueuedConnection);
    connect(this,SIGNAL(tmGap(QString,QString)),tmThread,SLOT(tmGapDetect(QString,QString)),Qt::QueuedConnection);
    connect(tmThread, SIGNAL(updateImage(QImage*)), this, SLOT(updateTmImage(QImage*)));
    connect(tmThread, SIGNAL(updateValue(QList<float> *,QList<char> *)), this, SLOT(updateTmValue(QList<float> *, QList<char> *)));
    connect(tmThread, SIGNAL(updateMeasure(QImage*,float[],bool[])), this, SLOT(updateTmMeasure(QImage*,float[],bool[])));
    connect(tmThread, SIGNAL(updateGapResult(bool,QString)), this, SLOT(updateTmGapResult(bool,QString)));
    connect(this, SIGNAL(getTMMeasure(int)), tmThread, SLOT(getMeasure(int)));
    connect(this, SIGNAL(getTMMeasureGap(int,int,int,int,int,int,QString)), tmThread, SLOT(getMeasureGap(int,int,int,int,int,int,QString)));
    tmThread->start();
    //*******************update zhu
    //    weightThread = new WeightThread();
    //    connect(weightThread,SIGNAL(sendValue(float)),this,SLOT(updateWeightValue(float)));

    //    if(!weightThread->openSerial(QString("COM1"),9600)){
    //        qDebug()<<"串口打开失败！";
    //    } else {
    //        weightThread->startTask(1);
    //    }
    //************************end

    connect(this,SIGNAL(autoAdjustExposure()),g_camera,SLOT(autoAdjustExposureSlot()));
    connect(pageStation,SIGNAL(signalVMState()),this,SLOT(VMState()));
    if(cameraIpImg)
    {
        m_camImage = cvCreateImage(cvGetSize(cameraIpImg),8,1);
        m_camImage2=cvCreateImage(cvGetSize(cameraIpImg),8,1);
    }

    if(QFile::exists("d:/debug.txt"))
        DEBUG=true;
    else
        DEBUG=false;
    //QTimer::singleShot(14000,this,SLOT(openService()));
    createTable();
    initInterface();

    picSuff=".png";
    curRow=-1;

    //rong lk
        //python 与 QT 通过socket通信
        pythonSocket = new QUdpSocket(this);
        pythonSocket->bind(18812, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

        connect(pythonSocket, SIGNAL(readyRead()), this, SLOT(readRapidFromPythonService()));

        m_ui->dockWidget->setMaximumWidth(600);
        m_ui->dockWidget->setMaximumHeight(90);

          //QPushButton *button1= new QPushButton();

          btn_vertexPointSelect = new QPushButton("顶点",this);
          btn_anyPointSelect = new QPushButton("任意点",this);
          btn_endPointSelect = new QPushButton("端点",this);
          btn_middlePointSelect = new QPushButton("中点",this);
          btn_centrePointSelect = new QPushButton("圆心点",this);
          btn_edgePointSelect = new QPushButton("边界点",this);
          btn_surfaceSelect = new QPushButton("面",this);
          btn_edgeSelect = new QPushButton("边",this);
          btn_componentSelect = new QPushButton("组件",this);

          btn_vertexPointSelect->setCheckable(true);
          btn_anyPointSelect->setCheckable(true);
          btn_endPointSelect->setCheckable(true);
          btn_middlePointSelect->setCheckable(true);
          btn_centrePointSelect->setCheckable(true);
          btn_edgePointSelect->setCheckable(true);
          btn_surfaceSelect->setCheckable(true);
          btn_edgeSelect->setCheckable(true);
          btn_componentSelect->setCheckable(true);
          //btn_vertexPointSelect->setDown(false);

          //button1->setText(tr("xx"));
          //QPushButton * button2 = ;
          QHBoxLayout * layout = new QHBoxLayout;
          layout->addWidget(btn_vertexPointSelect);
          layout->addWidget(btn_anyPointSelect);
          layout->addWidget(btn_endPointSelect);
          layout->addWidget(btn_middlePointSelect);
          layout->addWidget(btn_centrePointSelect);
          layout->addWidget(btn_edgePointSelect);
          layout->addWidget(btn_surfaceSelect);
          layout->addWidget(btn_edgeSelect);
          layout->addWidget(btn_componentSelect);


          m_ui->dockWidgetContents->setLayout(layout);
          m_ui->dockWidget->setStyleSheet("color: rgb(0,0,0);background:rgb(222,240,251)");
          m_ui->dockWidget->setAutoFillBackground(true);


          connect(pageViewer,SIGNAL(cancelButtonDownSignal()),this,SLOT(vertexPointSelectSlot()));
          connect(btn_vertexPointSelect,SIGNAL(clicked()),this,SLOT(vertexPointSelectSlot()));
          connect(btn_anyPointSelect,SIGNAL(clicked()),this,SLOT(anyPointSelectSlot()));
          connect(btn_endPointSelect,SIGNAL(clicked()),this,SLOT(endPointSelectSlot()));
          connect(btn_middlePointSelect,SIGNAL(clicked()),this,SLOT(middlePointSelectSlot()));
          connect(btn_centrePointSelect,SIGNAL(clicked()),this,SLOT(centrePointSelectSlot()));
          connect(btn_edgePointSelect,SIGNAL(clicked()),this,SLOT(edgePointSelectSlot()));
          connect(btn_surfaceSelect,SIGNAL(clicked()),this,SLOT(surfaceSelectSlot()));
          connect(btn_edgeSelect,SIGNAL(clicked()),this,SLOT(edgeSelectSlot()));
          connect(btn_componentSelect,SIGNAL(clicked()),this,SLOT(componentSelectSlot()));
          pageViewer->showToolWidget(4);

          //end

}

MainWindow::~MainWindow()
{
    //rong lk
        system("pause");
    //end
    delete m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//始终保存１万张检测图片
void MainWindow::savedDetectImg(IplImage * src,QString folderName,QString fileName)
{


    QDir dir("D:/"+folderName);
    dir.mkdir("D:/"+folderName);
    dir.setFilter(QDir::Files);
    if(dir.entryInfoList().size()>10000)
    {

        if(dir.exists("D:/"+folderName+"_backup"))
        {
            system(QString("rd/s/q d:\\%1_backup").arg(folderName).toLocal8Bit().data());
        }
        dir.rename("D:/"+folderName,"D:/"+folderName+"_backup");
        dir.mkdir("D:/"+folderName);
    }

    int ret=cvSaveImage(QString("D:/%1/%2%3").arg(folderName).arg(fileName).arg(picSuff).toLocal8Bit().data(),src);

}

void MainWindow::savedDetectImg2(IplImage * src,QString folderName,QString fileName)
{


    QDir dir("D:/"+folderName);
    dir.mkdir("D:/"+folderName);
    dir.setFilter(QDir::Files);
    if(dir.entryInfoList().size()>10000)
    {

        if(dir.exists("D:/"+folderName+"_backup"))
        {
            system(QString("rd/s/q d:\\%1_backup").arg(folderName).toLocal8Bit().data());
        }
        dir.rename("D:/"+folderName,"D:/"+folderName+"_backup");
        dir.mkdir("D:/"+folderName);
    }

    int ret=cvSaveImage(QString("D:/%1/%2%3").arg(folderName).arg(fileName).arg(".bmp").toLocal8Bit().data(),src);

}


void MainWindow::writeLog(QString str)
{
    if(fileState[1])
    {
        QTextStream txtOutput(file[1]);
        txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<"  "<<str<<"\r\n";
        file[1]->flush();
    }

}

void MainWindow::rebootSeviceFunc()
{
    QTimer::singleShot(0,work,SLOT(rebootServiceSlot()));

}

//void MainWindow::manualVision()
//{
//    if (signalTriggerMaps[0].contains("Vision_1")){
//        signalTriggerCache[data[0]] = "1";
//        triggerValueChanged((signalTriggerMaps[0])["Vision_1"]);
//    }

//}
void MainWindow::timerEvent ( QTimerEvent * event )
{

    if (event->timerId() == sysTimeTimerId) {
        m_ui->tSysTime->setText(QDateTime::currentDateTime().toString(tr("MM-dd hh:mm")));
    } else if (event->timerId() == panelCheckTimerId) {
        if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
            m_ui->numPad->showKey(KeyOpenTP);
            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  示教器已关闭", QString());
            killTimer(panelCheckTimerId);   // 关闭示教器定时检查
            //        } else {
            //            long h = FindWindowW(QString::fromUtf8("VMPlayerFrame").toStdWString().c_str(), NULL);
            //            if (h) {
            //            ShowWindow(h, 0);
            //            }
        }
    } else if (event->timerId() == reconnectTimerId) {

        processKey(KeyReConnect);
    } else if (event->timerId() == axisAutoMoveTimerId) {

        killTimer(axisAutoMoveTimerId);
        Handle_AIS_Shape aisShape = NULL;
        long currentTime = clock();
        foreach (AxisObject *axis, autoMoveMap.keys()) {

            QStringList valueList = autoMoveMap.value(axis).split(",");
            if (valueList.size() != 3)
                continue;
            double target = valueList[0].toDouble();
            long time = valueList[1].toDouble() * 1000;
            long startTime = valueList[2].toDouble();
            if (currentTime - startTime >= time) {
                axis->value = target;
                autoMoveMap.remove(axis);
            } else if (axis->value == target) {
                axis->value = target;
                autoMoveMap.remove(axis);
            } else {
                //              double stepValue = (target - axis->value) / (time - (currentTime - startTime));
                axis->value += (target - axis->value) * (currentTime - startTime) / time;
            }

            // 需要时更新外部轴位置
            while (!axis->parentId.isEmpty()) {
                axis = axisMap[axis->parentId];
            }
            TopLoc_Location locBase;
            while (true) {
                gp_Trsf trsfBase;
                if (axis->type == 1) {
                    trsfBase.SetTranslation((axis->value - axis->originOffset) * axis->symbol * gp_Vec(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z]));
                    locBase = locBase * TopLoc_Location(trsfBase);
                } else if(axis->type == 2) {
                    trsfBase.SetRotation(gp_Ax1(gp_Pnt(axis->start[X] + moduleMap[axis->moduleList.last()]->pos[X], axis->start[Y] + moduleMap[axis->moduleList.last()]->pos[Y], axis->start[Z] + moduleMap[axis->moduleList.last()]->pos[Z]), gp_Dir( axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z])), (axis->value - axis->originOffset) * axis->symbol * M_PI / 180);
                    locBase = locBase * TopLoc_Location(trsfBase);
                }
                for(int j = 0; j < axis->moduleList.size(); ++j) {
                    aisShape = moduleMap[axis->moduleList[j]]->aisShape;
                    aisShape->GetContext()->SetLocation(aisShape, locBase);
                }
                if (axis->subAxisList.isEmpty()) {
                    break;
                }
                axis = axisMap[axis->subAxisList.at(0)];
            }
        }
        if (aisShape) {
            aisShape->GetContext()->UpdateCurrentViewer();
        }
        if (autoMoveMap.count()>0) {
            //        autoMoveLastTime = clock();
            axisAutoMoveTimerId = startTimer(40);
        }

        //        for ( int i=0;i<autoMoveList.size();i++ ) {
        //            AutoMoveStruct autoMove = autoMoveList.at(i);
        //        }
    }
}

void MainWindow::keyPressEvent ( QKeyEvent * e )
{
    if (e->modifiers() == Qt::ControlModifier) {
        if (e->key() == Qt::Key_R) {
            static bool isRecording = false;
            static QProcess p;
            if (!isRecording) {
                isRecording = true;
                QString fileName = QFileDialog::getSaveFileName(this, tr(" 请选择方案视频保存文件"),
                                                                "",
                                                                tr("方案视频 (*.avi)"));
                if (fileName.isEmpty()) return;
                p.setProcessChannelMode(QProcess::MergedChannels);
                p.start("VideoRecord.exe", QStringList()<<QString::number(int(this->winId()))<<"40"<<fileName);
            } else {
                isRecording = false;
                //                if (p.state() != QProcess::NotRunning) {
                p.terminate();
                //                }
            }
        }
        else if (e->key() == Qt::Key_F) {
            if (m_ui->stackedWidget->currentIndex() != 1)
                return;
            if ( !findDialog ) {
                findDialog = new FindDialog(m_ui->textEdit,this);
            }
            findDialog->show();
        }
    } else {
        switch (e->key()) {
        case Qt::Key_F1:
            if (!m_ui->button_F1->isEnabled()) break;
            m_ui->button_F1->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F2:
            if (!m_ui->button_F2->isEnabled()) break;
            m_ui->button_F2->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F3:
            if (!m_ui->button_F3->isEnabled()) break;
            m_ui->button_F3->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F4:
            if (!m_ui->button_F4->isEnabled()) break;
            m_ui->button_F4->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F5:
            if (!m_ui->button_F5->isEnabled()) break;
            m_ui->button_F5->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F6:
            if (!m_ui->button_F6->isEnabled()) break;
            m_ui->button_F6->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F7:
            if (!m_ui->button_F7->isEnabled()) break;
            m_ui->button_F7->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F8:
            if (!m_ui->button_F8->isEnabled()) break;
            m_ui->button_F8->setDown(true);
            this->grabKeyboard();
            break;
        case Qt::Key_F9:
            if (!m_ui->button_F9->isEnabled()) break;
            m_ui->button_F9->setDown(true);
            this->grabKeyboard();
            break;

        default:
            QApplication::sendEvent(m_ui->numPad, e);
        }
    }
}

void MainWindow::keyReleaseEvent ( QKeyEvent * e )
{
    switch (e->key()) {
    case Qt::Key_F1:
        if (!m_ui->button_F1->isEnabled()) break;
        m_ui->button_F1->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F1->click();
        break;
    case Qt::Key_F2:
        if (!m_ui->button_F2->isEnabled()) break;
        m_ui->button_F2->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F2->click();
        break;
    case Qt::Key_F3:
        if (!m_ui->button_F3->isEnabled()) break;
        m_ui->button_F3->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F3->click();
        break;
    case Qt::Key_F4:
        if (!m_ui->button_F4->isEnabled()) break;
        m_ui->button_F4->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F4->click();
        break;
    case Qt::Key_F5:
        if (!m_ui->button_F5->isEnabled()) break;
        m_ui->button_F5->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F5->click();
        break;
    case Qt::Key_F6:
        if (!m_ui->button_F6->isEnabled()) break;
        m_ui->button_F6->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F6->click();
        break;
    case Qt::Key_F7:
        if (!m_ui->button_F7->isEnabled()) break;
        m_ui->button_F7->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F7->click();
        break;
    case Qt::Key_F8:
        if (!m_ui->button_F8->isEnabled()) break;
        m_ui->button_F8->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F8->click();
        break;
    case Qt::Key_F9:
        if (!m_ui->button_F9->isEnabled()) break;
        m_ui->button_F9->setDown(false);
        this->releaseKeyboard();
        m_ui->button_F9->click();
        break;

    default:
        QApplication::sendEvent(m_ui->numPad, e);
    }
    //    ((PageWork*)m_ui->stackedWidget->widget(0))->keyPressed( e->key() );
}

void MainWindow::on_button_F4_clicked()
{
    //    QMessageBox::information(this, "系统提示", "本授权无此功能！", "确定");
    m_ui->frameNumPad->setVisible(false);
    emit changeStationTab(1);
    m_ui->stackedWidget_2->setCurrentIndex(1);
    m_ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_button_F5_clicked()
{
    //    QMessageBox::information(this, "系统提示", "本授权无此功能！", "确定");
    m_ui->frameNumPad->setVisible(false);
    emit changeStationTab(3);
    m_ui->stackedWidget_2->setCurrentIndex(1);
    m_ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_button_F6_clicked()
{
    m_ui->stackedWidget->setCurrentIndex(4);
    m_ui->stackedWidget_2->setCurrentIndex(0);

    m_ui->numPad->showKey(KeyAddWarning);
    m_ui->numPad->showKey(KeyRemoveWarning);
    m_ui->numPad->showKey(KeyExportWarning);
    m_ui->numPad->showKey(KeyImportWarning);
    m_ui->numPad->hideKey(KeyRemoveKnife);
    m_ui->numPad->hideKey(KeyMeasureTool);
    m_ui->numPad->hideKey(KeyFindProgram);

    m_ui->frameNumPad->setVisible(true);

    //    QMessageBox::information(this, "系统提示", "本授权无此功能！", "确定");
}

void MainWindow::on_button_F7_clicked()
{

    m_ui->numPad->hideKey(KeyGetProgram);
    m_ui->numPad->hideKey(KeyPutProgram);
    m_ui->numPad->hideKey(KeyFindProgram);
    m_ui->numPad->hideKey(KeyGenerateMoveJ);
    m_ui->numPad->hideKey(KeyGenerateWait);
    m_ui->numPad->hideKey(KeyAddWarning);
    m_ui->numPad->hideKey(KeyRemoveWarning);
    m_ui->numPad->hideKey(KeyExportWarning);
    m_ui->numPad->hideKey(KeyImportWarning);
    m_ui->numPad->hideKey(KeyMeasureTool);

    //    // 刀具管理
    //    m_ui->stackedWidget->setCurrentIndex(3);
    //    m_ui->stackedWidget_2->setCurrentIndex(0);
    //    m_ui->numPad->showKey(KeyAddKnife);
    //    m_ui->numPad->showKey(KeyResetKnife);
    //    m_ui->numPad->showKey(KeyRemoveKnife);

    // 视觉设置
    m_ui->stackedWidget->setCurrentIndex(5);
    m_ui->stackedWidget_2->setCurrentIndex(0);
    //zhu???
//     m_ui->page_8->switchSolution(currentStationId);

//        QString fileName="";
//        fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));

//        if(fileName=="")
//        {
//            QMessageBox::information(NULL,"信息","请选择文件",QMessageBox::tr("确认"));
//            return;
//        }

    //    char*  ch;
    //    QByteArray ba = fileName.toLatin1();
    //    ch=ba.data();
    //    IplImage *m_camImage =cvLoadImage(ch,0);
    //    CvRect cvRect1;
    //    cvRect1.x = 898;
    //    cvRect1.y = 789;
    //    cvRect1.width = 813;
    //    cvRect1.height = 722;
    ////    cvRect1.x = 0;
    ////    cvRect1.y = 0;
    ////    cvRect1.width = 2592;
    ////    cvRect1.height = 1944;
    //    bool isRight =detecitonWorkPiece(m_camImage,cvRect1,6.0,6.5,0.2,100,0.0253);
    //    m_ui->maodingLabel->setText(QString().sprintf("%07.3fmm", rivetDia));  //工件内部铆钉直径
    //    m_ui->yuanpanLabel->setText(QString().sprintf("%07.3fmm", diskDia));  //圆盘直径
    //    m_ui->yuanxinjuLabel->setText(QString().sprintf("%07.3fmm", rivetDceDiff)); //铆钉圆心位置和圆盘圆心位置距离差值


    // 系统管理
       // m_ui->stackedWidget->setCurrentIndex(4);
       // m_ui->stackedWidget_2->setCurrentIndex(0);


    m_ui->frameNumPad->setVisible(true);

}

void MainWindow::updateCpk(QString filename,double number)
{

    if(!QFile::exists(filename))
    {
        cpkArray.clear();
    }

        QString data="";
        for(int i=0;i<cpkArray.size();i++)
        {
            data+=QString(" R%1=\"%2\"").arg(i).arg(cpkArray[i]);
        }
        int row=cpkArray.size();

        file[2]=new QFile(filename);
        fileState[2]=file[2]->open(QIODevice::WriteOnly | QIODevice::Truncate);

        QTextStream txtOutput(file[2]);
        txtOutput <<QString("<?xml version=\"1.0\" encoding=\"utf-8\"?><TyCPKToolsFileData><Config USL_E=\"0\" USL_V=\"0\" Target_E=\"0\" Target_V=\"0\" LSL_E=\"0\" LSL_V=\"0\" SubGroupSize=\"5\" DataGroupSize=\"0\" SampleSize=\"5\" DecimalSize=\"2\" CalculateSTDKind=\"0\"/><Cells><ColCount>1</ColCount><RowCount>%1</RowCount><Item %2>C0</Item></Cells></TyCPKToolsFileData>").arg(row).arg(data);
        file[2]->flush();
        file[2]->close();


}

void MainWindow::createCpk()
{
    QFile file("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk");
    if(!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(instruction);

    QDomElement root=doc.createElement("TyCPKToolsFileData");

    QDomElement Config=doc.createElement("Config");
    Config.setAttribute("USL_E","-1");
    Config.setAttribute("USL_V","0.5");
    Config.setAttribute("Target_E","-1");
    Config.setAttribute("Target_V","0");
    Config.setAttribute("LSL_E","-1");
    Config.setAttribute("LSL_V","-0.5");
    Config.setAttribute("SubGroupSize","16");
    Config.setAttribute("DataGroupSize","0");
    Config.setAttribute("SampleSize","8");
    Config.setAttribute("DecimalSize","2");
    Config.setAttribute("CalculateSTDKind","0");


    QDomElement Cells=doc.createElement("Cells");

    QDomElement  ColCount=doc.createElement("ColCount");
    text=doc.createTextNode("1");
    ColCount.appendChild(text);

    Cells.appendChild(ColCount);

    QDomElement  RowCount=doc.createElement("RowCount");
    text=doc.createTextNode("0");
    RowCount.appendChild(text);
    Cells.appendChild(RowCount);

    QDomElement  Item=doc.createElement("Item");
    text=doc.createTextNode("C0");
    Item.appendChild(text);
    Cells.appendChild(Item);


    root.appendChild(Config);
    root.appendChild(Cells);
    doc.appendChild(root);

    QTextStream out(&file);
    doc.save(out,4);
    file.close();

}
void MainWindow::addCpk()
{//discard
    QFile file("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk");
    if(!file.exists())
    {
        createCpk();
    }
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    if (!doc.setContent(&file))
    {\
        qDebug()<<"set error";
        return;
    }
    file.close();
    QDomNode RowCount= doc.elementsByTagName("RowCount").at(0);
    QDomElement Item= doc.elementsByTagName("Item").at(0).toElement();
    int oldNum=RowCount.toElement().text().toInt();
    int num=oldNum+cpkList[0].size();
    RowCount.firstChild().setNodeValue(QString::number(num));
    qDebug()<<oldNum<<num;
    for(int i=0;i<cpkList[0].size();i++)
    {
        Item.setAttribute(QString("R%1").arg(i+oldNum),QString::number(cpkList[0].at(i)));

    }


    if(!file.open(QIODevice::WriteOnly))
        qDebug() << "open for add error!";


    QTextStream out(&file);
    doc.save(out,4);
    file.close();
    cpkList[0].clear();
}


void MainWindow::openService()
{


}



//VideoRecord *record = NULL;
void MainWindow::on_button_F8_clicked()
{


//    if(version<2)
//        addCpk();

    // CPK分析
    //    if (QFile::exists("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk")) {

    CopyFileA("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk", "D:\\DzicsRobot\\CPK\\Demo.cpk", false);
    //    }
    WinExec("D:\\DzicsRobot\\CPK\\CPKTools.exe", SW_NORMAL);

    m_ui->numPad->hideKey(KeyGetProgram);
    m_ui->numPad->hideKey(KeyPutProgram);
    m_ui->numPad->hideKey(KeyFindProgram);
    m_ui->numPad->hideKey(KeyGenerateMoveJ);
    m_ui->numPad->hideKey(KeyGenerateWait);
    m_ui->numPad->hideKey(KeyAddWarning);
    m_ui->numPad->hideKey(KeyRemoveWarning);
    m_ui->numPad->hideKey(KeyExportWarning);
    m_ui->numPad->hideKey(KeyImportWarning);
    m_ui->numPad->hideKey(KeyAddKnife);
    m_ui->numPad->hideKey(KeyResetKnife);
    m_ui->numPad->hideKey(KeyRemoveKnife);
    m_ui->frameNumPad->setVisible(true);

    return;

    //    m_ui->stackedWidget->setCurrentIndex(5);
    m_ui->stackedWidget_2->setCurrentIndex(0);

    //    m_ui->numPad->showKey(KeyAddWarning);
    //    m_ui->numPad->showKey(KeyRemoveWarning);
    //    m_ui->numPad->showKey(KeyExportWarning);
    //    m_ui->numPad->showKey(KeyImportWarning);
    //    m_ui->numPad->hideKey(KeyRemoveKnife);
    //    m_ui->numPad->hideKey(KeyMeasureTool);

    m_ui->numPad->hideKey(KeyGetProgram);
    m_ui->numPad->hideKey(KeyPutProgram);
    m_ui->numPad->hideKey(KeyFindProgram);
    m_ui->numPad->hideKey(KeyGenerateMoveJ);
    m_ui->numPad->hideKey(KeyGenerateWait);
    m_ui->numPad->hideKey(KeyAddWarning);
    m_ui->numPad->hideKey(KeyRemoveWarning);
    m_ui->numPad->hideKey(KeyExportWarning);
    m_ui->numPad->hideKey(KeyImportWarning);
    m_ui->numPad->hideKey(KeyAddKnife);
    m_ui->numPad->hideKey(KeyResetKnife);
    m_ui->numPad->hideKey(KeyRemoveKnife);
    m_ui->frameNumPad->setVisible(true);

    //    static QProcess p;
    //    if (p.state() != QProcess::NotRunning) {
    //        p.close();
    //    }
    //    p.start("D:\\ThorX6\\X6.exe", QStringList("D:\\ThorX6\\Robot.Tx6"));

    //    SysUtils::externalPrint(true);

    //    static bool isRecording = false;
    //    if (!isRecording) {
    //        isRecording = true;
    //        record = new VideoRecord(this);
    //        record->StartVedioRecord(this, 15);
    ////        record->StartVedioRecord(((PageWork*)m_ui->stackedWidget->widget(0))->viewer, 40);
    //    } else {
    //        record->StopVedioRecord();
    ////        delete record;
    //        isRecording = false;
    //    }

    //    QMessageBox::information(this, "系统提示", "本授权无此功能！！", "确定");
    //    m_ui->label_unit->setText(tr("毫米"));
}


void MainWindow::on_button_F9_clicked()
{


//    if(version<2)
//    {
//        // 刀具管理
//        m_ui->stackedWidget->setCurrentIndex(3);
//        m_ui->stackedWidget_2->setCurrentIndex(0);

//        m_ui->numPad->showKey(KeyAddKnife);
//        m_ui->numPad->showKey(KeyResetKnife);
//        m_ui->numPad->showKey(KeyRemoveKnife);
//    }
//    if(version==XL)
//    {

//        //伺服电机
//        m_ui->stackedWidget->setCurrentIndex(6);

//        m_ui->page_9->setTabIndex(0);



//    }else if(version==TEST)
//    {


//        m_ui->stackedWidget->setCurrentIndex(3);
//        m_ui->stackedWidget_2->setCurrentIndex(0);
//        m_ui->page_4->setCurTab(3);

//        m_ui->numPad->showKey(KeyAddKnife);
//        m_ui->numPad->showKey(KeyResetKnife);
//        m_ui->numPad->showKey(KeyRemoveKnife);



//    }
//    else if(version==ZT1){
//        m_ui->page_4->setCurTab(2);
//    }else if(version==RXPZ){
//       m_ui->stackedWidget->setCurrentIndex(8);
//    }

    setButtonF9(g_sysManager->f9Index,true);
    m_ui->stackedWidget_2->setCurrentIndex(0);

    m_ui->frameNumPad->setVisible(true);
    return;
    //end


    //    static QProcess p;
    //    if (p.state() != QProcess::NotRunning) {
    //        p.close();
    //    }
    //    p.start("D:\\DzicsRobot\\DzicsHelp.exe", QStringList());

    //    QProcess::startDetached("D:\\DzicsRobot\\DzicsHelp.exe", QStringList());

    //  SysUtils::remoteHelp(true);

    //    m_ui->label_unit->setText(tr("英寸"));

    //    m_ui->stackedWidget->setCurrentIndex(2);
    //QMessageBox::information(this, "系统提示", "演示版本，无此功能！", "确定");

    //    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择方案配置文件"), "D:\\DzicsRobot",  tr("方案配置文件(*.INI)"));
    //    if (fileName.endsWith(".INI")) {
    //        if (serviceSocket) {
    //            char data[] = {CMD_STOP_VC};
    //            sendService(data, 1);
    //            serviceSocket->waitForBytesWritten(1000);
    //            Sleep(1000);
    //            serviceSocket->disconnect();
    //        }
    //        QFile::remove("D:\\DzicsRobot\\ROBOT.INI");
    //        if (QFile::copy(fileName, "D:\\DzicsRobot\\ROBOT.INI")) {
    //            qApp->quit();
    //            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    //        }
    //    }
}


//void reportError(int error_code,int print_stack)
//{
//    //char interp_error_text_buf[LINELEN];
//    //int k;

//    //interp.errorText(error_code, interp_error_text_buf, 5); /* for coverage of code */
//    //interp.errorText(error_code, interp_error_text_buf, LINELEN);
//    //fprintf(stderr, "%s\n",((interp_error_text_buf[0] == 0) ? "Unknown error, bad error code" : interp_error_text_buf));
//    //interp.lineText(interp_error_text_buf, LINELEN);
//    //fprintf(stderr, "%s\n", interp_error_text_buf);
//    //if (print_stack == ON)
//    //{
//    //    for (k = 0; ; k++)
//    //    {
//    //        interp.stackName(k, interp_error_text_buf, LINELEN);
//    //        if (interp_error_text_buf[0] != 0)
//    //            fprintf(stderr, "%s\n", interp_error_text_buf);
//    //        else
//    //            break;
//    //    }
//    //}
//}


//int interpretFromString(int do_next,int block_delete,int print_stack,char* commandString)
//{
//        //int status=0;
//        //char line[LINELEN];

//        //char* commandText[2];
//        //commandText[0] = commandText[1] = commandString;

//        //for(; ;)
//        //{
//        //       status = interp.readString(NULL,commandText);
//        //       if ((status == INTERP_EXECUTE_FINISH) && (block_delete == ON))
//        //                continue;
//        //        else if (status == INTERP_ENDFILE)
//        //                break;
//        //        if ((status != INTERP_OK) &&    // should not be EXIT
//        //                (status != INTERP_EXECUTE_FINISH))
//        //        {
//        //                reportError(status, print_stack);
//        //                if ((status == NCE_FILE_ENDED_WITH_NO_PERCENT_SIGN) ||
//        //                        (do_next == 2)) /* 2 means stop */
//        //                {
//        //                        status = 1;
//        //                        break;
//        //                }
//        //                else if (do_next == 1) /* 1 means MDI */
//        //                {
//        //                        fprintf(stderr, "starting MDI\n");
//        //                        ////interpret_from_keyboard(block_delete, print_stack);
//        //                        ////fprintf(stderr, "continue program? y/n =>");
//        //                        fgets(line, LINELEN, stdin);
//        //                        if (line[0] != 'y')
//        //                        {
//        //                                status = 1;
//        //                                break;
//        //                        }
//        //                        else
//        //                                continue;
//        //                }
//        //                else /* if do_next == 0 -- 0 means continue */
//        //                        continue;
//        //        }
//        //        status = interp.executeString(0,commandText);

//        //        if ((status != INTERP_OK) &&
//        //                (status != INTERP_EXIT) &&
//        //                (status != INTERP_EXECUTE_FINISH))
//        //        {
//        //                reportError(status, print_stack);
//        //                status = 1;
//        //                if (do_next == 1) /* 1 means MDI */
//        //                {
//        //                        fprintf(stderr, "starting MDI\n");
//        //                        ////interpret_from_keyboard(block_delete, print_stack);
//        //                        ////fprintf(stderr, "continue program? y/n =>");
//        //                        ////fgets(line, LINELEN, stdin);
//        //                        if (line[0] != 'y')
//        //                                break;
//        //                }
//        //                else if (do_next == 2) /* 2 means stop */
//        //                        break;
//        //        }
//        //        else if (status == INTERP_EXIT)
//        //                return 0;
//        //}
//        //return ((status == 1) ? 1 : 0);
//	return 0;
//}

//void report_error(int error_code,int print_stack)
//{
//        //char interp_error_text_buf[LINELEN];
//        //int k;

//        //interp.errorText(error_code, interp_error_text_buf, 5); /* for coverage of code */
//        //interp.errorText(error_code, interp_error_text_buf, LINELEN);
//        //fprintf(stderr, "%s\n",((interp_error_text_buf[0] == 0) ? "Unknown error, bad error code" : interp_error_text_buf));
//        //interp.lineText(interp_error_text_buf, LINELEN);
//        //fprintf(stderr, "%s\n", interp_error_text_buf);
//        //if (print_stack == ON)
//        //{
//        //        for (k = 0; ; k++)
//        //        {
//        //                interp.stackName(k, interp_error_text_buf, LINELEN);
//        //                if (interp_error_text_buf[0] != 0)
//        //                        fprintf(stderr, "%s\n", interp_error_text_buf);
//        //                else
//        //                        break;
//        //        }
//        //}
//}

////extern std::vector <CommandStruct> commandList;
//void printVector(QWidget *widget)
//{
//    //QString result;

//    ////commandList长度
//    //for(uint i=0; i< commandList.size(); i++) {
//    //    result.append( QString::number(commandList[i].sequence) );
//    //    result.append (" ");
//    //    result.append (QString::number(commandList[i].line));
//    //    result.append (" N");
//    //    result.append (QString::number(commandList[i].ncLine));
//    //    result.append (" ");
//    //    result.append (QString::number(commandList[i].command));
//    //    result.append ("(");

//    //    //params长度
//    //    int n = commandList[i].params.size();

//    //    for (int m = 0;m < n;m++)
//    //    {
//    //        result.append ( QString::number(commandList[i].params[m]) );
//    //        result.append (", ");
//    //    }

//    //    result.append ( QString::number(commandList[i].typeParam) );
//    //    if (commandList[i].strParam) result.append (", ");
//    //    result.append ( commandList[i].strParam );
//    //    result.append (")\n");
//    //}
//    //QMessageBox::information(widget, "解析", QString(result));
//}

extern QTranslator languageTranslator;

#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>



void MainWindow::setOperatorState(int x)
{
    if(curRobotIndex!=x)
        return;
    QPalette p;
    if (operatorStates[x] == "自动"){
        m_ui->tMode2->setText("自动");
        p.setColor(QPalette::WindowText, Qt::black);
    }else if(operatorStates[x] == "手动"){
        m_ui->tMode2->setText("手动");
        p.setColor(QPalette::WindowText, Qt::darkMagenta);
    }else if(operatorStates[x] == "手动全速"){
        m_ui->tMode2->setText("手动全速");
        p.setColor(QPalette::WindowText, Qt::darkMagenta);
    }else if(operatorStates[x] == "手动"){
        m_ui->tMode2->setText("手动");
        p.setColor(QPalette::WindowText, Qt::darkMagenta);
    }else if(operatorStates[x] == "示教"){
        m_ui->tMode2->setText("示教");
        p.setColor(QPalette::WindowText, Qt::darkMagenta);
    }else if(operatorStates[x] == "力控"){
        m_ui->tMode2->setText("力控");
        p.setColor(QPalette::WindowText, Qt::darkMagenta);
    }else if(operatorStates[x] == "扭矩"){
        m_ui->tMode2->setText("扭矩");
        p.setColor(QPalette::WindowText, Qt::darkMagenta);
    }else if(operatorStates[x] == "未知"){
        m_ui->tMode2->setText("未知");
        p.setColor(QPalette::WindowText, Qt::darkYellow);
    }else if(operatorStates[x] == ""){
        m_ui->tMode2->setText("自动");
        p.setColor(QPalette::WindowText, Qt::black);
    }

    m_ui->tMode2->setPalette(p);
}

void MainWindow::setConnState(int x)
{
    if(curRobotIndex!=x)
        return;
    QPalette p;
    if(connStates[x]=="联机")
    {
        killTimer(reconnectTimerId);
        m_ui->tMode1->setText("联机");
        m_ui->tMode1->setEnabled(true);
        m_ui->tMode2->setEnabled(true);
        m_ui->tMode3->setEnabled(true);
        p.setColor(QPalette::WindowText, Qt::black);
        m_ui->numPad->enableKey(KeyOpenVM, false);
    }
    if(connStates[x]=="虚拟机")
    {
        killTimer(reconnectTimerId);
        m_ui->tMode1->setText("虚拟机");
        m_ui->tMode1->setEnabled(true);
        m_ui->tMode2->setEnabled(true);
        m_ui->tMode3->setEnabled(true);
        p.setColor(QPalette::WindowText, Qt::darkGreen);
        m_ui->numPad->showKey(KeyCloseVM);
    }
    if(connStates[x]=="脱机")
    {
        m_ui->tMode1->setText("脱机");
        m_ui->tMode1->setEnabled(false);
        m_ui->tMode2->setEnabled(false);
        m_ui->tMode3->setEnabled(false);
        p.setColor(QPalette::WindowText, Qt::gray);
        m_ui->numPad->showKey(KeyOpenVM);
        m_ui->numPad->enableKey(KeyOpenVM, true);
    }
    if(connStates[x]=="")
    {
        m_ui->tMode1->setText("脱机");
        m_ui->tMode1->setEnabled(false);
        m_ui->tMode2->setEnabled(false);
        m_ui->tMode3->setEnabled(false);
        p.setColor(QPalette::WindowText, Qt::gray);
        m_ui->numPad->showKey(KeyOpenVM);
        m_ui->numPad->enableKey(KeyOpenVM, true);
    }
    m_ui->tMode1->setPalette(p);

}

void MainWindow::setSpeedState(int x)
{

    m_ui->speedLabel->setText(QString::number(speedCache[x])+ "%");
}

void MainWindow::startRobot()
{

    QString str;
    QByteArray dataArray;
    for(int i=1;i<=pageStation->robotNum();i++)
    {
        //        if (serviceConnected[i-1]==false)
        //            continue;
        str= QString("??startRobot_%1|1").arg(i);
        dataArray = str.toLocal8Bit();
        dataArray[0] = CMD_WRITE_VAR;
        dataArray[1] = i;
        mysendService(dataArray.data(),i);
        emit addLog(QString("打开机器人%1").arg(i),"");
    }



}

void MainWindow::stopRobot()
{
    QString str;
    QByteArray dataArray;
    for(int i=1;i<=pageStation->robotNum();i++)
    {
        //        if (serviceConnected[i-1]==false)
        //            continue;
        str= QString("??stopRobot_%1|1").arg(i);
        dataArray = str.toLocal8Bit();
        dataArray[0] = CMD_WRITE_VAR;
        dataArray[1] = i;
        mysendService(dataArray.data(),i);
        emit addLog(QString("关闭机器人%1").arg(i),"");
    }
}

void MainWindow::pauseRobot()
{
    for(int i=1;i<=pageStation->robotNum();i++)
    {
        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_PAUSE_PROGRAM,i};
            mysendService(data,i);
            emit addLog( QTime::currentTime().toString("hh:mm:ss %1").arg(i)+ "  暂停运行", QString());
        }
    }
}

void MainWindow::resumeRobot()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "全部恢复", "是否全部恢复?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
    {
        return;
    }
    for(int i=1;i<=pageStation->robotNum();i++)
    {
        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_RESUME_PROGRAM,i};
            mysendService(data,i);
            emit addLog( QTime::currentTime().toString("hh:mm:ss %1").arg(i) + "  恢复运行", QString());
        }
    }
}
void MainWindow::createCpk2()
{//discard
    QFile file("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyy").toLocal8Bit()+"work.cpk");
    if(!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(instruction);

    QDomElement root=doc.createElement("TyCPKToolsFileData");

    QDomElement Config=doc.createElement("Config");
    Config.setAttribute("USL_E","-1");
    Config.setAttribute("USL_V","0.5");
    Config.setAttribute("Target_E","-1");
    Config.setAttribute("Target_V","0");
    Config.setAttribute("LSL_E","-1");
    Config.setAttribute("LSL_V","-0.5");
    Config.setAttribute("SubGroupSize","16");
    Config.setAttribute("DataGroupSize","0");
    Config.setAttribute("SampleSize","8");
    Config.setAttribute("DecimalSize","2");
    Config.setAttribute("CalculateSTDKind","0");


    QDomElement Cells=doc.createElement("Cells");

    QDomElement  ColCount=doc.createElement("ColCount");
    text=doc.createTextNode("3");
    ColCount.appendChild(text);
    Cells.appendChild(ColCount);

    QDomElement  RowCount=doc.createElement("RowCount");
    text=doc.createTextNode("0");
    RowCount.appendChild(text);
    Cells.appendChild(RowCount);


    QDomElement  date=doc.createElement("date");
    text=doc.createTextNode("2000-01-01 23:11:20");
    date.appendChild(text);
    Cells.appendChild(date);


    QDomElement  Item=doc.createElement("Item");
    QDomElement  Item1=doc.createElement("Item");
    QDomElement  Item2=doc.createElement("Item");
    Cells.appendChild(Item);
    Cells.appendChild(Item1);
    Cells.appendChild(Item2);


    root.appendChild(Config);
    root.appendChild(Cells);
    doc.appendChild(root);

    QTextStream out(&file);
    doc.save(out,4);
    file.close();


}

void MainWindow::getXmlValue(QString name,QString key,QString &out)
{
    QFile file(name);
    if(!file.exists())
    {
        return;
    }
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return ;
    }
    QDomDocument doc;
    if (!doc.setContent(&file))
    {\
        qDebug()<<"set error";
        return;
    }
    QDomNode date= doc.elementsByTagName("date").at(0);
    QString strDate=date.toElement().text();
    out=strDate;
    file.close();
}

void MainWindow::writeXmlValue(QString name,QString key,QString value)
{
    QFile file("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyy").toLocal8Bit()+"work.cpk");
    if(!file.exists())
    {
        return;
    }
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    if (!doc.setContent(&file))
    {\
        qDebug()<<"set error";
        return;
    }
    file.close();

    QDomNode date= doc.elementsByTagName("date").at(0);
    date.firstChild().setNodeValue(value);

    if(!file.open(QIODevice::WriteOnly))
        qDebug() << "open for add error!";


    QTextStream out(&file);
    doc.save(out,4);
    file.close();
}

void MainWindow::createTable()
{

    QSqlQuery query;

    QString sql1="SELECT * FROM sqlite_master  where type='table' and name='RobotSetting'";
    QString createTable="CREATE TABLE RobotSetting(id   VARCHAR (32)  NOT NULL,knifeType int, knifeData1  double , knifeData2  double ,  knifeData3  double , pos1 int, pos2 double, pos3 double, pos4 double, r double, deep double, angle double, density double,  precision1 double, precision2 double, flange1 double, flange2 double, flange3 double,  attitude1 int, attitude2 double, attitude3 double, attitude4 double,in1 int, out1 int, other1 double, other2 double, other3 double, solutionId VERCHAR(32) NOT NULL )";

    query.exec(sql1);

    if(!query.next())
    {

        if(query.exec(createTable))
            qDebug()<<"create RobotSetting Table success";
    }
    else
    {

        sql1="PRAGMA table_info(RobotSetting)";
        query.exec(sql1);
        bool b=false;
        while(query.next())
        {
            if(query.value(1).toString()=="solutionId")
            {
                b=true;
                break;
            }
        }
        if(!b)
        {
            sql1="drop table RobotSetting";
            query.exec(sql1);
            if(query.exec(createTable))
                qDebug()<<"create RobotSetting Table success";
        }

    }

    //safeArea
    createTable="create table safeArea(id varchar(50) not null,solutionId varchar(50) not null,robotIndex int not null,value1 varchar(500),value2 varchar(100))";
    sql1="SELECT name FROM sqlite_master  where type='table' and name='safeArea'";
    query.exec(sql1);
    if(!query.next())
    {

        if(query.exec(createTable))
            qDebug()<<"createTable safeArea success";
    }
    else
    {

        sql1="PRAGMA table_info(safeArea)";
        query.exec(sql1);
        bool b=false;
        int cols=0;
        while(query.next())
        {
           ++cols;
        }
        if(cols<5)
        {
            sql1="drop table safeArea";
            qDebug()<<"drop table"<<query.exec(sql1)<<"cols"<<cols;

            if(!query.exec(createTable))
                qDebug()<<"create safeArea Table fail";
        }

    }

    //end

    QString sql2="select * from SysParam";
    query.exec(sql2);
    bool title_b=false,version_b=false;
    while(query.next())
    {
        if(query.value(0).toString()=="title")
            title_b=true;

        if(query.value(0).toString()=="projectVersion")
            version_b=true;
    }

    if(!title_b)
    {
        QString sql3="insert into SysParam values('title','大正工业机器人三维控制系统')";
        query.exec(sql3);
    }

    if(!version_b)
    {
        QString sql3="insert into SysParam values('projectVersion','ZT1')";
        query.exec(sql3);
    }


    //rong lk
          //增加的新表，最好通过dbversion控制
          sql1="SELECT * FROM sqlite_master  where type='table' and name='RobotParameter'";
          createTable="create table RobotParameter(id varchar(50) not null primary key, name varchar(50), type int, RobotParamsX1 double, RobotParamsX2 double, RobotParamsX3 double, RobotParamsX4 double, RobotParamsX5 double, RobotParamsX6 double, RobotParamsZ1 double, RobotParamsZ2 double, RobotParamsZ3 double, RobotParamsZ4 double, RobotParamsZ5 double, RobotParamsZ6 double, RobotParamsAS1 double, RobotParamsAS2 double, RobotParamsAS3 double, RobotParamsAS4 double, RobotParamsAS5 double, RobotParamsAS6 double, RobotParamsAE1 double, RobotParamsAE2 double, RobotParamsAE3 double, RobotParamsAE4 double, RobotParamsAE5 double, RobotParamsAE6 double)";

          query.exec(sql1);

            if(!query.next())
            {

                if(query.exec(createTable))
                    qDebug()<<"create RobotParameter Table success";
            }
            else
            {

            }

            sql1="SELECT * FROM sqlite_master  where type='table' and name='CalibrationCategory'";
            createTable="create table CalibrationCategory (id varchar(50) not null primary key, stationId varchar(50), name  varchar(50), calibrationNum int)";

            query.exec(sql1);

              if(!query.next())
              {

                  if(query.exec(createTable))
                      qDebug()<<"create CalibrationCategory Table success";
              }
              else
              {

              }

              sql1="SELECT * FROM sqlite_master  where type='table' and name='CalibrationDetail'";
              createTable="create table CalibrationDetail (id varchar(50) not null primary key, calibrationId varchar(50), name varchar(50), point0 varchar(50), point1 varchar(50), point2 varchar(50), point3 varchar(50), point4 varchar(50), point5 varchar(50), point6 varchar(50), point7 varchar(50), point8 varchar(50), falaX double, falaY double, falaZ double, falaQ1 double, falaQ2 double, falaQ3 double, falaQ4 double, toolPosX double, toolPosY double, toolPosZ double, toolQ1 double, toolQ2 double, toolQ3 double, toolQ4 double, toolKg double, toolMassX double, toolMassY double, toolMassZ double, sensorPosX double, sensorPosY double, sensorPosZ double, sensorDirectionX double, sensorDirectionY double, sensorDirectionZ double, sensorReads double, sensorDistance double)";

              query.exec(sql1);

                if(!query.next())
                {

                    if(query.exec(createTable))
                        qDebug()<<"create CalibrationDetail Table success";
                }
                else
                {

                }


}


void MainWindow::addCpk2(int total,QString dt)
{//discard
    QFile file("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyy").toLocal8Bit()+"work.cpk");
    if(!file.exists())
    {
        createCpk2();
    }



    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    if (!doc.setContent(&file))
    {\
        qDebug()<<"set error";
        return;
    }
    file.close();
    QDomNode RowCount= doc.elementsByTagName("RowCount").at(0);

    QDomElement Item= doc.elementsByTagName("Item").at(0).toElement();
    QDomElement Item2= doc.elementsByTagName("Item").at(1).toElement();
    QDomElement Item3= doc.elementsByTagName("Item").at(2).toElement();
    int oldNum=RowCount.toElement().text().toInt();
    int num=oldNum;
    RowCount.firstChild().setNodeValue(QString::number(++num));


    Item.setAttribute(QString("R%1").arg(oldNum),QString::number(total));
    Item2.setAttribute(QString("R%1").arg(oldNum),"'"+dt);
    Item3.setAttribute(QString("R%1").arg(oldNum),"'"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));





    if(!file.open(QIODevice::WriteOnly))
        qDebug() << "open for add error!";


    QTextStream out(&file);
    doc.save(out,4);
    file.close();

}

void MainWindow::clearWorkNum()
{

    QString time;
    getXmlValue("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyy").toLocal8Bit()+"work.cpk","date",time);

    QFile f("d:/dzicsrobot/num.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
    {
        qDebug() << "Open failed.";
        return;
    }

    QTextStream txtOutput(&f);
    static QDateTime dt1=QDateTime::currentDateTime();
    QString s1=QString("工件个数：%1  开始时间：%2   结束时间：%3\n").arg(m_ui->maodingLabel->text()).arg(time).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    txtOutput <<s1.toLocal8Bit();
    dt1=QDateTime::currentDateTime();
    f.close();

    if (serviceSocket[0]) {
        char data[] = {CMD_RESET_WORKPIECE_COUNT,3};
        mysendService(data,1);
        emit addLog( QTime::currentTime().toString("hh:mm:ss %1").arg(3) + "  数据清除", QString());
    }



    //cpk
    addCpk2( m_ui->maodingLabel->text().toInt(),time);

    m_ui->maodingLabel->setText("000");

}

void MainWindow::openWorkCpk()
{
    CopyFileA("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyy").toLocal8Bit()+"work.cpk", "D:\\DzicsRobot\\CPK\\Demo.cpk", false);
    WinExec("D:\\DzicsRobot\\CPK\\CPKTools.exe", SW_NORMAL);
}



void MainWindow::servoMove(float val)
{
    //    QString str;
    //    QByteArray dataArray;

    //    str= QString("?DISTANCE|%1").arg(val);
    //    dataArray = str.toLocal8Bit();
    //    dataArray[0] = CMD_SERVO_DISTANCE;
    //    servoSocket->writeDatagram(dataArray,  QHostAddress::LocalHost,8905);
    //    //mysendService(dataArray.data(),1);
    //    emit addLog(QString("移动距离%1").arg(val),"");


}

void MainWindow::sendVisualRes(QString str,QString ip)
{
    QByteArray dataArray=str.toLocal8Bit();
    mainWindow->serviceSocket[0]->writeDatagram(dataArray,dataArray.length(),  QHostAddress(ip), 8888);

}

void MainWindow::serviceTest(int index)
{
    QString str;
    QByteArray dataArray;


    if(index==1)
    {
        str = "??camvar1_1|" + QString::number((qrand()%100000)/1000.0,'f',0);
        str+=";camvar2_1|" + QString::number((qrand()%100000)/1000.0,'f',0);
        str+=";camvar3_1|" + QString::number((qrand()%100000)/1000.0,'f',0);
        dataArray = str.toLocal8Bit();
        dataArray[0] = CMD_WRITE_VAR;
        dataArray[1] = 1;
        mysendService(dataArray.data(),dataArray.length(),1);
        emit addLog("send 1","");
    }

    if(index ==2)
    {
        str = "??camvar1_2|" + QString::number((qrand()%100000)/1000.0,'f',0);
        str+=";camvar2_2|" + QString::number((qrand()%100000)/1000.0,'f',0);
        str+=";camvar3_2|" + QString::number((qrand()%100000)/1000.0,'f',0);
        dataArray = str.toLocal8Bit();
        dataArray[0] = CMD_WRITE_VAR;
        dataArray[1] = 2;
        mysendService(dataArray.data(),dataArray.length(),1);
        emit addLog("send 2","");
    }

    if(index==3)
    {
        str = "??camvar1_3|" + QString::number((qrand()%100000)/1000.0,'f',0);
        str+=";camvar2_3|" + QString::number((qrand()%100000)/1000.0,'f',0);
        str+=";camvar3_3|" + QString::number((qrand()%100000)/1000.0,'f',0);
        dataArray = str.toLocal8Bit();
        dataArray[0] = CMD_WRITE_VAR;
        dataArray[1] = 3;
        mysendService(dataArray.data(),dataArray.length(),1);
        emit addLog("send 3","");
    }

}

void MainWindow::setTitle(QString str)
{
    m_ui->label_title->setText(str);

}
void MainWindow::setButtonF9(QString str,bool setWidget)
{


    if(str=="2")
    {

        m_ui->button_F9->setText("刀补设置");
        if(setWidget)
        {
        m_ui->stackedWidget->setCurrentIndex(3);
        m_ui->stackedWidget_2->setCurrentIndex(0);

        m_ui->numPad->showKey(KeyAddKnife);
        m_ui->numPad->showKey(KeyResetKnife);
        m_ui->numPad->showKey(KeyRemoveKnife);
        }
    }
    else if(str=="3")
    {

        m_ui->button_F9->setText("伺服电机");
        if(setWidget)
        {
            m_ui->stackedWidget->setCurrentIndex(6);

            m_ui->page_9->setTabIndex(g_servo->firsttabshow());

        }
    }
    else if(str=="4")
    {

        m_ui->button_F9->setText("钣金冲压");
        if(setWidget)
        {
            m_ui->stackedWidget->setCurrentIndex(6);

            m_ui->page_9->setTabIndex(g_servo->firsttabshow());

        }
    }


    if(str=="1")
    {
        m_ui->button_F9->setEnabled(false);
        m_ui->button_F9->setText("远程协助");
    }
    else
        m_ui->button_F9->setEnabled(true);


}


QString MainWindow::getTitle()
{
    return m_ui->label_title->text();
}
void MainWindow::showIndexText()
{
    QString str=g_sysManager->data;
    QStringList list=str.split("|");
    textVar.clear();
    varUnit.clear();

    QStringList list2;

    for(int i=0;i<7;i++)
    {
        labelSetName[i]->setText("     ");
        labelSetValue[i]->setText("          ");
    }
    int n=0;
    for(int i=0;i<list.size();i++)
    {
        if(n>=7)
            break;
        list2=list[i].split(",");

        if(list2.size()==12)
        {

            if(list2[0]=="1")
            {

                labelSetName[n]->setText(list2[1].left(7));
                labelSetValue[n]->setText("0"+list2[2]+"      ");
                textVar[list2[3]]=n++;
                varUnit[list2[3]]=list2[2];



            }
        }

    }

//    QMapIterator<QString, int> i(textVar);
//     while (i.hasNext()) {
//         i.next();
//         qDebug()<<"jjjj" << i.key() << ": " << i.value();
//     }

}



void MainWindow::closeEvent(QCloseEvent *e)
{

     WinExec("taskkill /f /t /im workObjCal.exe", SW_HIDE);

    delete  m_ui->page_8;
    delete unlock;
    workTh->quit();
    workTh->wait();



}

void MainWindow::setExecState(int x)
{
    if(curRobotIndex!=x)
        return;
    QPalette p;

    if (execStates[x] == "运行") {
        m_ui->tMode3->setText("运行");
        p.setColor(QPalette::WindowText, Qt::blue);
        m_ui->numPad->showKey(KeyPauseProgram);
    } else if(execStates[x] == "停止") {
        m_ui->tMode3->setText("停止");
        p.setColor(QPalette::WindowText, Qt::red);
        m_ui->numPad->showKey(KeyStartProgram);
    } else if(execStates[x] == "暂停") {
        m_ui->tMode3->setText("暂停");
        p.setColor(QPalette::WindowText, Qt::magenta);
        m_ui->numPad->showKey(KeyResumeProgram);
    } else if(execStates[x] == "告警") {
        m_ui->tMode3->setText("告警");
        p.setColor(QPalette::WindowText, Qt::red);
        m_ui->numPad->showKey(KeyStartProgram);
    } else if(execStates[x] == "急停") {
        m_ui->tMode3->setText("急停");
        p.setColor(QPalette::WindowText, Qt::red);
        m_ui->numPad->showKey(KeyStartProgram);
    } else if(execStates[x] == "空闲") {
        m_ui->tMode3->setText("空闲");
        p.setColor(QPalette::WindowText, Qt::red);
        m_ui->numPad->showKey(KeyStartProgram);
    } else if(execStates[x] == "断电") {
        m_ui->tMode3->setText("断电");
        p.setColor(QPalette::WindowText, Qt::red);
        m_ui->numPad->showKey(KeyStartProgram);
    } else if(execStates[x]=="未知"){
        m_ui->tMode3->setText(execStates[x]);
        p.setColor(QPalette::WindowText, Qt::black);
    } else if(execStates[x]==""){
        m_ui->tMode3->setText("停止");
        p.setColor(QPalette::WindowText, Qt::red);
        m_ui->numPad->showKey(KeyStartProgram);
    }


    m_ui->tMode3->setPalette(p);



}





void MainWindow::processKnife()
{

    while (knifeSocket->hasPendingDatagrams()) {
        try {
            QByteArray result;
            result.resize(knifeSocket->pendingDatagramSize());
            knifeSocket->readDatagram(result.data(), result.size());
            uchar len = result[0];
            uchar command = result[1];

            if(command==NC_CMD_LOGON_RESULT)
            {
                knifeConnected[result.mid(1)]=true;
                QString state="??"+result.mid(1);
                QByteArray b=state.toLocal8Bit();
                b[0]=NC_CMD_CONNECT_STATE;
                b[1]=10;
                sendKnife(b);
            }else if(command==NC_CMD_GET_CONNECT_STATE)
            {
                emit addLog("状态"+result.mid(2),"");
            }
            else if(command ==NC_CMD_TOOL_DATA){

                m_ui->page_4->writeKnife1(QString(result.mid(2)));
            } else if(command ==NC_CMD_LOG)
            {
                m_ui->page_4->writeKnife1Log(QString(result.mid(2)));
            }

        }catch(...){

        }
    }

}

void MainWindow::processServo()
{
    while (servoSocket->hasPendingDatagrams()) {
        try {
            QByteArray result;
            result.resize(servoSocket->pendingDatagramSize());
            servoSocket->readDatagram(result.data(), result.size());
            uchar len = result[0];
            uchar command = result[1];
            uchar robot = result[2];
            if(command==CMD_SERVO_STATE_RESULT)
            {

                QString state=result.mid(3);
                QString ret="??";
                if(state=="OK")
                {
                    servoState[robot]=true;
                    ret+="OK";
                }
                else
                {
                    servoState[robot]=false;
                    ret+="NG";
                }

                QByteArray b1=ret.toLocal8Bit();
                b1[0]=CMD_SERVO_STATE_RESULT;
                b1[1]=1;
                mysendService(b1,1);
                addLog("send to service"+ret.mid(2),"");

            }else if(command==CMD_KNIFE_STATE)
            {
                //            QByteArray b;
                //            b.append(CMD_KNIFE_STATE);
                //            servoSocket->writeDatagram(b,  QHostAddress::LocalHost, 18905);
                //            emit addLog("发送伺服请求数据","");
                QByteArray b=result.remove(0,1);
                mysendService(result,1);
                emit addLog("收到伺服请求","");
            }

        }catch(...){

        }
    }

}





bool MainWindow::startAxisAutoMove(AxisObject *axis, double target, double time)
{
    //    AutoMoveStruct autoMove;
    //    autoMove.axis = axis;
    //    autoMove.target = target;
    //    autoMove.time = time;
    //    autoMoveList.append(autoMove);
    //    axisAutoMoveTimerId = startTimer(40);
    return true;
}

bool MainWindow::openPanel(int state)
{
    //if (state < 1000) {
    if (state >= 0 ) {
        if (tpPanel == NULL) {
            tpPanel = new QProcess(this);
        }
        if (tpPanel->state() == QProcess::NotRunning) {
            QStringList params;

            /* notebook
            params.append("{C6C09017-A485-46BB-97B5-1FF10BE57CCA}");
            params.append("/panel");
            params.append("/notop");
            QProcess::startDetached("C:\\Program Files\\ABB Industrial IT\\Robotics IT\\RobotStudio 5.14\\Bin\\Virtual FlexPendant.exe",params);
            */

            int robotType;
            QString robotVmId, robotRealId;
            QSqlQuery query;
            query.exec(QString("SELECT RobotLibrary.type,vmid,guid from RobotLibrary,StationMotion where stationId='%1' and displayOrder=%2 and StationMotion.libraryId=RobotLibrary.id order by displayOrder ASC").arg(currentStationId).arg(curRobotIndex));
            while (query.next())
            {
                robotType = query.value(0).toInt();
                robotVmId = query.value(1).toString();
                robotRealId = query.value(2).toString();

                switch(robotType) {
                case 11:    // ABB
                case 12:    // ABB
                    if (QFile::exists( "D:\\DzicsRobot\\Virtual FlexPendant.exe" )) {
                        params.append("{"+robotVmId+"}");//"{E8A09A48-60DF-4FB1-8771-250C32462B13}");
                        params.append("/panel");    //	params.append("/notop");
                        tpPanel->start("D:\\DzicsRobot\\Virtual FlexPendant.exe", params);
                    }
                    break;
                case 21:    // FANUC
                case 22:    // FANUC
                    if (QFile::exists( "D:\\DzicsRobot\\FANUCTeachPendant.exe" )) {
                        params.append(robotVmId);//"{E8A09A48-60DF-4FB1-8771-250C32462B1``3}");
                        params.append("9100");
                        tpPanel->start("D:\\DzicsRobot\\FANUCTeachPendant.exe", params);
                    }
                    break;
                case 31:    // UR
                case 32:    // UR
                    if (QFile::exists( "D:\\DzicsRobot\\DzicsURTeach.exe" )) {
                        if (m_ui->tMode1->text() == "联机") {
                            params.append(robotRealId);
                        } else {
                            params.append(robotVmId);
                        }
                        tpPanel->start("D:\\DzicsRobot\\DzicsURTeach.exe", params);
                    }
                    break;
                case 41:    // DENSO
                case 42:    // DENSO 2
                    if (QFile::exists( "D:\\DzicsRobot\\DensoTeachPendant.exe" )) {
                        params.append( "Open" );
                        params.append( "D:\\DzicsRobot\\" + robotVmId );
                        tpPanel->start("D:\\DzicsRobot\\DensoTeachPendant.exe", params);
                    }
                    break;
                case 51:    // Kawasaki // 暂时用电装的
                case 52:    // Kawasaki 2
                    if (QFile::exists( "D:\\DzicsRobot\\KawasakiTeachPendant.exe" )) {
                        params.append( "Open" );
                        params.append( "D:\\DzicsRobot\\" + robotVmId );
                        tpPanel->start("D:\\DzicsRobot\\KawasakiTeachPendant.exe", params);
                    }
                    break;
                default:
                    return false;
                }


                m_ui->numPad->showKey(KeyCloseTP);
                emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  打开示教器...", QString());

                panelCheckTimerId = startTimer(2000);

            }


        } else {
            m_ui->numPad->showKey(KeyOpenTP);
            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  关闭示教器", QString());
            killTimer(panelCheckTimerId);   // 关闭示教器定时检查
            tpPanel->close();

            int robotType;
            QSqlQuery query;
            query.exec(QString("SELECT RobotLibrary.type from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id order by displayOrder ASC").arg(currentStationId));
            if (query.next())
            {
                robotType = query.value(0).toInt();
                if (robotType == 41) {  // DENSO
                    if (QFile::exists( "D:\\DzicsRobot\\DensoTeachPendant.exe" )) {
                        QProcess::startDetached("D:\\DzicsRobot\\DensoTeachPendant.e xe Close");
                    }
                }
            }
        }
    } else {
        //		Sleep(1000);
        static QTcpSocket *client = new QTcpSocket(this);
        client->connectToHost(QHostAddress::LocalHost, state);
        //connect(client, SIGNAL(readyRead()), this, SLOT(readServiceMessage()));

        char tt1[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x19,0x00,0x00
                    ,0x04,0x00,0x0C,0x00,0x07,0x00,0x40,0x1F,0x00,0x00,0x00,0x7D,0x00,0x00,0xDE};	// 25

        char tt2[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x1A,0x00,0x01
                    ,0x06,0x00,0x0C,0x00,0x04,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x9A,0xDE};	//26

        char tt3[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x42,0x00,0x02
                    ,0x01,0x00,0x0C,0x00,0x04,0x01,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x29
                    ,0x23,0x02,0x47,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
                    ,0x2F,0x43,0x4F,0x4E,0x54,0x52,0x4F,0x4C,0x4C,0x45,0x52,0x00,0x52,0x6F,0x62,0x41
                    ,0x50,0x49,0x50,0x77,0x64,0x00,0x00,0xDE};	//66

        char tt4[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x35,0x00,0x00
                    ,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C
                    ,0x20,0x02,0x55,0x4E,0x53,0x55,0x42,0x53,0x43,0x52,0x49,0x42,0x45,0x00,0x2F,0x78
                    ,0x78,0x78,0x2F,0x00,0x00,0x2D,0x31,0x35,0x00,0x00,0xDE};	//53

        char tt5[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0xA1,0x00,0x03
                    ,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88
                    ,0x20,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
                    ,0x2F,0x00,0x4C,0x6F,0x67,0x6F,0x6E,0x55,0x73,0x65,0x72,0x00,0x2D,0x6E,0x61,0x6D
                    ,0x65,0x20,0x22,0x44,0x65,0x66,0x61,0x75,0x6C,0x74,0x20,0x55,0x73,0x65,0x72,0x22
                    ,0x20,0x2D,0x70,0x77,0x64,0x20,0x22,0x30,0x38,0x3B,0x73,0x71,0x65,0x70,0x76,0x6C
                    ,0x64,0x75,0x22,0x20,0x2D,0x6C,0x6F,0x63,0x61,0x6C,0x65,0x20,0x22,0x4C,0x6F,0x63
                    ,0x61,0x6C,0x22,0x20,0x2D,0x61,0x70,0x70,0x20,0x22,0x52,0x6F,0x62,0x6F,0x74,0x53
                    ,0x74,0x75,0x64,0x69,0x6F,0x22,0x20,0x2D,0x6C,0x6F,0x63,0x20,0x22,0x44,0x41,0x49
                    ,0x59,0x4C,0x2D,0x31,0x22,0x20,0x2D,0x61,0x6C,0x69,0x61,0x73,0x20,0x22,0x44,0x61
                    ,0x69,0x79,0x6C,0x22,0x00,0x00,0xDE};	//161

        //(73)
        char tt6[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x49,0x00,0x8D,
                    0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x30,
                    0x20,0x02,0x47,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31,
                    0x2F,0x00,0x49,0x73,0x4F,0x70,0x74,0x69,0x6F,0x6E,0x50,0x72,0x65,0x73,0x65,0x6E,
                    0x74,0x00,0x2D,0x4F,0x70,0x74,0x69,0x6F,0x6E,0x20,0x49,0x56,0x00,0x00,0xDE};

        //(120)
        char tt7[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x78,0x00,0x92,
                    0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x5F,
                    0x21,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31,
                    0x2F,0x49,0x4F,0x53,0x59,0x53,0x54,0x45,0x4D,0x2F,0x49,0x4F,0x42,0x55,0x53,0x45,
                    0x53,0x2F,0x4C,0x6F,0x63,0x61,0x6C,0x2F,0x50,0x41,0x4E,0x45,0x4C,0x00,0x49,0x6E,
                    0x70,0x75,0x74,0x44,0x61,0x74,0x61,0x00,0x2D,0x4D,0x61,0x72,0x6B,0x20,0x30,0x20,
                    0x2D,0x44,0x61,0x74,0x61,0x20,0x34,0x32,0x39,0x34,0x39,0x36,0x35,0x33,0x37,0x35,
                    0x20,0x2D,0x4D,0x61,0x73,0x6B,0x20,0x32,0x30,0x31,0x36,0x00,0x00,0xDE};

        //(81)
        char tt8[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x51,0x03,0x45,
                    0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x1B,0x00,0x00,0x00,0x00,0x00,0x38,
                    0x20,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31,
                    0x2F,0x00,0x52,0x65,0x71,0x75,0x65,0x73,0x74,0x4D,0x61,0x73,0x74,0x65,0x72,0x32,
                    0x00,0x2D,0x44,0x6F,0x6D,0x61,0x69,0x6E,0x20,0x43,0x46,0x47,0x20,0x2D,0x55,0x69,
                    0x64,0x20,0x32,0x37,0x00,0x00,0xDE};


        char tt11[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x33,0x01,0x52
                     ,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x1A
                     ,0x22,0x02,0x47,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
                     ,0x2F,0x00,0x4D,0x6F,0x64,0x65,0x00,0x00,0xDE};//51

        char tt12[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x37,0x01,0x53
                     ,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x1E
                     ,0x22,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
                     ,0x2F,0x00,0x41,0x63,0x6B,0x00,0x41,0x55,0x54,0x4F,0x00,0x00,0xDE};//55

        client->write(tt1, 25);
        client->waitForBytesWritten(2000);
        client->write(tt2, 26);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);

        client->write(tt3, 66);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);
        client->write(tt4, 53);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);

        client->readAll();
        client->write(tt5, 161);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);

        QByteArray tta;
        while (client->bytesAvailable() < 40) {
            continue;
        }
        tta = client->readAll();

        client->write(tt6, 73);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);
        client->write(tt7, 120);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);

        tt8[19]=(tta[37] - 0x30) * 10 + (tta[38] - 0x30);
        tt8[76]=tta[37];
        tt8[77]=tta[38];
        //tt8[78]=tta.at(39);
        client->write(tt8, 81);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);

        client->write(tt11, 51);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);

        tt12[19]=(tta[37] - 0x30) * 10 + (tta[38] - 0x30);
        client->write(tt12, 55);
        client->waitForBytesWritten(2000);
        client->waitForReadyRead(2000);

        // QApplication::aboutQt();

    }
    return true;
}


void MainWindow::processKey(int id)
{
    if (!m_ui->numPad->isVisible()) return;
    switch (id) {
    case KeyPauseProgram: // 暂停运行
    {
        //serviceClient = new QTcpSocket(this);
        //serviceClient->abort(); //
        ////QHostAddress s(QString("192.168.1.3"));
        ////serviceClient->connectToHost(s, 8566);	//QHostAddress::LocalHost
        //serviceClient->connectToHost(QHostAddress::LocalHost, 8566);	//
        //connect(serviceClient, SIGNAL(readyRead()), this, SLOT(readServiceMessage()));
        //serviceClient->write("a", 1);

        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_PAUSE_PROGRAM,curRobotIndex};
            mysendService(data,curRobotIndex);

            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  暂停运行", QString());
        }

    }
        break;
    case KeyResumeProgram: // 恢复运行
    {
        //serviceClient->write("d", 1);
        //serviceClient->waitForBytesWritten();
        //serviceClient->abort(); //取消已有的连接

        if (serviceSocket[currentUdpIndex]) {
            //					char data[] = {CMD_LOGOFF};
            char data[] = {CMD_RESUME_PROGRAM,curRobotIndex};
            mysendService(data,curRobotIndex);
            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  恢复运行", QString());
        }
    }
        break;
    case KeyResetProgram:	// 复位指针
    {
        //serviceClient->write("e", 1);

        if (serviceSocket[currentUdpIndex]) {
            if(0==QMessageBox::warning(this,tr("系统警告"),tr("复位指令将导致机器人重新回到指令开始处运行！\n本操作存在风险，确定要继续吗？"), tr("确定"), tr("取消"))) {
                char data[] = {CMD_RESET_PROGRAM,curRobotIndex};
                mysendService(data,curRobotIndex);
            }
        }


        if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
            m_ui->numPad->showKey(KeyOpenTP);
        }

        //double currentPos[] = {-0.0694262,-0.03789099,13.00715,0,0.0006179682,1.296571E-07};


        //		t	"[[9.656411,-7.258155,14.28174,7.983665,12.43874,0.5149484],[0,0,0,0,0,0]]"	string
        //				emit syncPosition( -0.0694262,-0.03789099,13.00715,0,0.0006179682,1.296571E-07 );

        //				QByteArray result = serviceClient->readAll();
        //				if (result[0] == '[' && result[result.size() - 1] == ']') {
        //					QStringList resultPos = QString::fromLocal8Bit(result.data()+1, result.size()-2).split(',');
        //					emit syncPosition( resultPos[0].toDouble(), resultPos[1].toDouble(), resultPos[2].toDouble(), resultPos[3].toDouble(), resultPos[4].toDouble(), resultPos[5].toDouble() );
        ////					emit syncPosition( 16.52611,-12.34823,15.17737,13.61496,21.21335,0.884317 );
        //				}

    }
        break;
    case KeyStepProgram:    // 单步调试
    {
        if (serviceSocket[currentUdpIndex]) {
            //                    m_ui->codeView->currentIndex();
            int row = m_ui->codeView->currentIndex().row();
            if (row > 0) {
                QString currentCode = "??????" + m_ui->codeView->currentIndex().data().toString();
                //                    if ( MainWindow.isVisible() ) {
                QByteArray data = currentCode.toLocal8Bit();
                data[0] = CMD_STEP_PROGRAM;
                data[1] = curRobotIndex;
                data[2] = (row >> 24) & 0xFF;
                data[3] = (row >> 16) & 0xFF;
                data[4] = (row >> 8) & 0xFF;
                data[5] = (row) & 0xFF;
                mysendService(data,curRobotIndex);
                emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  单步调试", QString());
                //                    }
            }
        }
    }
        break;
    case KeyOpenVM:	// 虚拟机开
    {
        //serviceClient->write("v", 1);
        //serviceClient->flush();

        //				if (serviceSocket) {
        //					char data[] = {CMD_START_VC};
        //					sendService(data, 1);
        vmState[curRobotIndex]=true;
        QSqlQuery query;
        query.exec(QString("SELECT RobotLibrary.type,vmid,vmPath,displayOrder from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id and displayOrder=%2 order by displayOrder ASC").arg(currentStationId).arg(curRobotIndex));
        while (query.next())
        {
            qDebug()<<"type:"<<query.value(0).toInt();
            if (query.value(0).toInt() == 31) { // UR
                QProcess::startDetached("D:\\DzicsRobot\\DzicsURVM.exe Start D:\\DzicsRobot\\" + query.value(2).toString());
                WinExec("net start \"VMware NAT Service\"", 0);  //SW_HIDE);
            }

            if (serviceSocket[currentUdpIndex]) {
                QString path = "??D:\\DzicsRobot\\" + query.value(2).toString()+"|"+ query.value(1).toString();
                QByteArray data = path.toLocal8Bit();
                data[0] = CMD_START_VC;
                data[1] = query.value(3).toInt();
                //addLog(QString("%1虚拟机开:").arg(query.value(3).toInt())+data,"");
                mysendService(data,curRobotIndex);
            }
        }


        //serviceSocket->flush();
        m_ui->numPad->showKey(KeyCloseVM);
        //				}

        //Sleep(1500);

        //QStringList params;

        ///* notebook
        //params.append("{C6C09017-A485-46BB-97B5-1FF10BE57CCA}");
        //params.append("/panel");
        //params.append("/notop");
        //QProcess::startDetached("C:\\Program Files\\ABB Industrial IT\\Robotics IT\\RobotStudio 5.14\\Bin\\Virtual FlexPendant.exe",params);
        //*/

        //params.append(robotId);//"{E8A09A48-60DF-4FB1-8771-250C32462B13}");
        //params.append("/panel");
        //params.append("/notop");
        //QProcess::startDetached("Virtual FlexPendant.exe",params);

        // 连接
        //serviceSocket->writeDatagram("a", 1, QHostAddress::LocalHost, 8566);
    }
        break;
    case KeyCloseVM:	// 虚拟机关case 11:    // ABB
    {
        // serviceClient->write("t", 1);
        vmState[curRobotIndex]=false;
        QSqlQuery query;
        query.exec(QString("SELECT RobotLibrary.type,vmid,vmPath,displayOrder from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id and displayOrder=%2 order by displayOrder ASC").arg(currentStationId).arg(curRobotIndex));
        if (query.next())
        {
            if (query.value(0).toInt() == 31) { // UR
                QProcess::startDetached("D:\\DzicsRobot\\DzicsURVM.exe Stop");
            }

            if (serviceSocket[currentUdpIndex]) {
                char data[] = {CMD_STOP_VC,-1};
                data[1]= query.value(3).toInt();
                //addLog(QString("%1虚拟机关:").arg(query.value(3).toInt())+data,"");
                mysendService(data,curRobotIndex);

            }
        }
        m_ui->numPad->showKey(KeyOpenVM);//add by zhugz

        if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
            m_ui->numPad->showKey(KeyOpenTP);
        }

    }
        break;
    case KeyOpenTP:	// 示教器开
    case KeyCloseTP:	// 示教器开
    {
        //if (serviceSocket) {
        //	serviceSocket->writeDatagram("n", 1, QHostAddress::LocalHost, 8566);
        //}

        openPanel(0);

        /*
                QStringList params;

                // notebook
                //params.append("{C6C09017-A485-46BB-97B5-1FF10BE57CCA}");
                //params.append("/panel");
                //params.append("/notop");
                //QProcess::startDetached("C:\\Program Files\\ABB Industrial IT\\Robotics IT\\RobotStudio 5.14\\Bin\\Virtual FlexPendant.exe",params);


                params.append(robotId);//"{E8A09A48-60DF-4FB1-8771-250C32462B13}");
                params.append("/panel");
//				params.append("/notop");
                QProcess::startDetached("Virtual FlexPendant.exe",params);
*/
    }
        break;
    case KeyStartProgram:
    {
        //				serviceClient->write("b", 1);


        if(version!=XL)
        {
            if (serviceSocket[currentUdpIndex]) {
                char data[] = {CMD_START_PROGRAM,curRobotIndex};

                mysendService(data,curRobotIndex);

            }
        }
        else
        {
//新华昌
            if(curRow==-1)
            {
                curRow=0;
                connectService();
                QTimer::singleShot(1000,this,SLOT(runProc()));
                return;
            }


            runProc();
        }


    }
        break;
    case KeyStopProgram:
    {
        //				serviceClient->write("c", 1);

        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_STOP_PROGRAM,curRobotIndex};
            mysendService(data,curRobotIndex);
        }

        if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
            m_ui->numPad->showKey(KeyOpenTP);
        }


    }
        break;
    case KeySpeedDown:
    {
        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_SPEED_DOWN,-1};
            data[1]=curRobotIndex;
            mysendService(data,curRobotIndex);
        }

        //                QStringList code = QString( SysUtils::readFile("testCircle.nc") ).split('\n');
        //                codeModel->setStringList( code );
        //                m_ui->codeView->setCurrentIndex(codeModel->index(-1));
        //                m_ui->codeView->scrollToTop();
        //
        //                KnifeBlade *knifeBlade = new KnifeBlade;
        //                currentKnifeBlade = "1002";
        //
        //                //＊圆柱
        //                knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -50.0));
        //                knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
        //                knifeBlade->bladePolygon.push_back(gp_Pnt2d(1.1, 0.0));
        //                knifeBlade->bladePolygon.push_back(gp_Pnt2d(10.0, -50.0));
        //                knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -50.0));
        //
        ////                knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -18.0));
        ////                knifeBlade->bladePolygon.push_back(gp_Pnt2d(-7.5,-30.0));
        ////                knifeBlade->bladePolygon.push_back(gp_Pnt2d(9.5, -30.0));
        ////                knifeBlade->bladePolygon.push_back(gp_Pnt2d(1.0, -18.0));
        ////                knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -18.0));
        //
        //                knifeBlade->originOffsetMap["X"] = 10.0;
        //                knifeBlade->originOffsetMap["Z"] = -94.1;
        //
        //                knifeBladeMap[currentKnifeBlade] = knifeBlade;
        //
        ////                programOrigin = gp_Pnt(109.9, 0.0, 0.0  );
        //
        //                planner.updateBlade(knifeBlade->bladePolygon);

    }
        break;
    case KeySpeedUp:
    {
        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_SPEED_UP,-1};
            data[1]=curRobotIndex;
            mysendService(data,curRobotIndex);
        }
    }
        break;
    case KeyReConnect: // 重新连接
    {
        //            languageTranslator.load("DzicsRobot_cn");

        connectService(curRobotIndex);

        break;
    }
    case KeyDisConnect:  // 断开连接
    {
        //            languageTranslator.load("DzicsRobot_en");
        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_LOGOFF,curRobotIndex};


            mysendService(data,curRobotIndex);

        }

        if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
            m_ui->numPad->showKey(KeyOpenTP);
        }

        break;
        //            QStringList code = QString( SysUtils::readFile("testLine.nc") ).split('\n');
        //            codeModel->setStringList( code );
        //            m_ui->codeView->setCurrentIndex(codeModel->index(-1));
        //            m_ui->codeView->scrollToTop();
        //
        //            KnifeBlade *knifeBlade = new KnifeBlade;
        //            currentKnifeBlade = "1001";
        //
        //            knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
        //            knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, -23.17));
        //            knifeBlade->bladePolygon.push_back(gp_Pnt2d(25.0, -22.17));
        //            knifeBlade->bladePolygon.push_back(gp_Pnt2d(11.5, -2.47));
        //            knifeBlade->bladePolygon.push_back(gp_Pnt2d(0.0, 0.0));
        //
        //            knifeBlade->originOffsetMap["X"] = 20.0;
        //            knifeBlade->originOffsetMap["Z"] = -204.0;
        //
        //            knifeBladeMap[currentKnifeBlade] = knifeBlade;
        //
        ////            machineCutting programOrigin = gp_Pnt(0.0, 0.0, -10.0);
        //
        //            planner.updateBlade(knifeBlade->bladePolygon);

    }
        break;
    case KeyGetProgram:
        if (serviceSocket[currentUdpIndex]) {
            if(0==QMessageBox::question(this,tr("系统提示"),tr("确认要获取机器人系统的最新指令替换当前方案中的指令吗？"), tr("确定"), tr("取消"))) {
                qApp->restoreOverrideCursor();
                qApp->setOverrideCursor(Qt::WaitCursor);
                //				char data[] = {CMD_GET_PROGRAM};
                QString path = "??D:\\DzicsRobot\\Program\\" + currentStationId;
                QByteArray data = path.toLocal8Bit();
                data[0] = CMD_GET_PROGRAM;
                data[1]=curRobotIndex;
                mysendService(data,curRobotIndex);


                //                    Sleep(1500);

                //                //	programCode = QString( SysUtils::readFile(robotPrg) );
                //                    programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod";
                //                    if (! QFile::exists( programFile )) {
                //                        programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\Module1.mod";
                //                        if (! QFile::exists( programFile )) {
                //                            QDir programPath = QDir("D:\\DzicsRobot\\Program\\"+currentStationId);
                //                            QStringList files = programPath.entryList(QStringList("*.mod"), QDir::Files | QDir::NoSymLinks);
                //                            if (files.count()>0) {
                //                                programFile = files.at(0);
                //                            }
                //                        }
                //                    }
                //                    programCode = QString( SysUtils::readFile( programFile ) );
                //                    codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
                //                    m_ui->codeView->setModel(codeModel);
                //                    m_ui->codeView->setCurrentIndex(codeModel->index(-1));

                //                    m_ui->textEdit->setPlainText(programCode);

                //                    qApp->restoreOverrideCursor();
            }
        }
        break;
    case KeyPutProgram:
        if (serviceSocket[currentUdpIndex]) {
            if(0==QMessageBox::warning(this,tr("系统警告"),tr("应用当前指令将会替换机器人系统中的原有指令，并使机器人复位指令！\n本操作存在风险，确定要继续吗？"), tr("确定"), tr("取消"))) {
                qApp->restoreOverrideCursor();
                qApp->setOverrideCursor(Qt::WaitCursor);

                programCode = m_ui->textEdit->toPlainText();
                //                    SysUtils::writeFile("D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod", programCode.toLocal8Bit());
                SysUtils::writeFile(programFile, programCode.toLocal8Bit());

                //				char data[] = {CMD_PUT_PROGRAM};
                QString path = "??D:\\DzicsRobot\\Program\\" + currentStationId;
                QByteArray data = path.toLocal8Bit();
                data[0] = CMD_PUT_PROGRAM;
                data[1]=curRobotIndex;
                mysendService(data,curRobotIndex);


                codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
                m_ui->codeView->setModel(codeModel);
                //				m_ui->codeView->setCurrentIndex(codeModel->index(-1));

                qApp->restoreOverrideCursor();
            }
        }
        break;
    case KeyOpenXi:
    {
        emit addLog("打开西门子","");
    }
        break;
    case KeyFindProgram:
    {
        if ( !findDialog ) {
            findDialog = new FindDialog(m_ui->textEdit,this);
        }
        findDialog->show();
    }
        break;
    case KeyGenerateMoveJ:
    {
        if (serviceSocket[currentUdpIndex]) {
            QByteArray data = QString("??MOVEJ").toLocal8Bit();
            data[0] = CMD_PROGRAM_GENERATE;
            data[1] = curRobotIndex;
            mysendService(data,curRobotIndex);
        }
    }
        break;
    case KeyGenerateWait:
    {
        if (serviceSocket[currentUdpIndex]) {
            QByteArray data = QString("??WAIT").toLocal8Bit();
            data[0] = CMD_PROGRAM_GENERATE;
            data[1] = curRobotIndex;
            mysendService(data,curRobotIndex);
        }
    }
        break;
    case KeySelectTool:
    {
        pageViewer->showToolWidget(3);
    }
        break;
    case KeyMeasureTool:
    {
        pageViewer->showToolWidget(2);
//rong lk
        //pageViewer->showToolWidget(4);
        m_ui->dockWidget->show();
    }
        break;
    case KeyNormalScreen:
    case KeyFullScreen:
    {
        if(this->windowState() & Qt::WindowFullScreen) {
            this->showNormal();
            m_ui->numPad->showKey(KeyFullScreen);
            //        this->setWindowState(Qt::WindowNoState);
        } else {
            this->showFullScreen();
            m_ui->numPad->showKey(KeyNormalScreen);
            //			        this->setWindowState(Qt::WindowFullScreen);
        }

        if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
            m_ui->numPad->showKey(KeyOpenTP);
        }


        /*
                static QTcpSocket *client = new QTcpSocket(this);
                client->connectToHost(QHostAddress::LocalHost, 4011);
                //connect(client, SIGNAL(readyRead()), this, SLOT(readServiceMessage()));

                char tt1[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x19,0x00,0x00
                    ,0x04,0x00,0x0C,0x00,0x07,0x00,0x40,0x1F,0x00,0x00,0x00,0x7D,0x00,0x00,0xDE};	// 25

                char tt2[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x1A,0x00,0x01
,0x06,0x00,0x0C,0x00,0x04,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x9A,0xDE};	//26

char tt3[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x42,0x00,0x02
,0x01,0x00,0x0C,0x00,0x04,0x01,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x29
,0x23,0x02,0x47,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
,0x2F,0x43,0x4F,0x4E,0x54,0x52,0x4F,0x4C,0x4C,0x45,0x52,0x00,0x52,0x6F,0x62,0x41
,0x50,0x49,0x50,0x77,0x64,0x00,0x00,0xDE};	//66

char tt4[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x35,0x00,0x00
,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C
,0x20,0x02,0x55,0x4E,0x53,0x55,0x42,0x53,0x43,0x52,0x49,0x42,0x45,0x00,0x2F,0x78
,0x78,0x78,0x2F,0x00,0x00,0x2D,0x31,0x35,0x00,0x00,0xDE};	//53

char tt5[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0xA1,0x00,0x03
,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88
,0x20,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
,0x2F,0x00,0x4C,0x6F,0x67,0x6F,0x6E,0x55,0x73,0x65,0x72,0x00,0x2D,0x6E,0x61,0x6D
,0x65,0x20,0x22,0x44,0x65,0x66,0x61,0x75,0x6C,0x74,0x20,0x55,0x73,0x65,0x72,0x22
,0x20,0x2D,0x70,0x77,0x64,0x20,0x22,0x30,0x38,0x3B,0x73,0x71,0x65,0x70,0x76,0x6C
,0x64,0x75,0x22,0x20,0x2D,0x6C,0x6F,0x63,0x61,0x6C,0x65,0x20,0x22,0x4C,0x6F,0x63
,0x61,0x6C,0x22,0x20,0x2D,0x61,0x70,0x70,0x20,0x22,0x52,0x6F,0x62,0x6F,0x74,0x53
,0x74,0x75,0x64,0x69,0x6F,0x22,0x20,0x2D,0x6C,0x6F,0x63,0x20,0x22,0x44,0x41,0x49
,0x59,0x4C,0x2D,0x31,0x22,0x20,0x2D,0x61,0x6C,0x69,0x61,0x73,0x20,0x22,0x44,0x61
,0x69,0x79,0x6C,0x22,0x00,0x00,0xDE};	//161

//(73)
char tt6[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x49,0x00,0x8D,
0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x30,
0x20,0x02,0x47,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31,
0x2F,0x00,0x49,0x73,0x4F,0x70,0x74,0x69,0x6F,0x6E,0x50,0x72,0x65,0x73,0x65,0x6E,
0x74,0x00,0x2D,0x4F,0x70,0x74,0x69,0x6F,0x6E,0x20,0x49,0x56,0x00,0x00,0xDE};

//(120)
char tt7[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x78,0x00,0x92,
0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x5F,
0x21,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31,
0x2F,0x49,0x4F,0x53,0x59,0x53,0x54,0x45,0x4D,0x2F,0x49,0x4F,0x42,0x55,0x53,0x45,
0x53,0x2F,0x4C,0x6F,0x63,0x61,0x6C,0x2F,0x50,0x41,0x4E,0x45,0x4C,0x00,0x49,0x6E,
0x70,0x75,0x74,0x44,0x61,0x74,0x61,0x00,0x2D,0x4D,0x61,0x72,0x6B,0x20,0x30,0x20,
0x2D,0x44,0x61,0x74,0x61,0x20,0x34,0x32,0x39,0x34,0x39,0x36,0x35,0x33,0x37,0x35,
0x20,0x2D,0x4D,0x61,0x73,0x6B,0x20,0x32,0x30,0x31,0x36,0x00,0x00,0xDE};

//(81)
char tt8[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x51,0x03,0x45,
0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x1B,0x00,0x00,0x00,0x00,0x00,0x38,
0x20,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31,
0x2F,0x00,0x52,0x65,0x71,0x75,0x65,0x73,0x74,0x4D,0x61,0x73,0x74,0x65,0x72,0x32,
0x00,0x2D,0x44,0x6F,0x6D,0x61,0x69,0x6E,0x20,0x43,0x46,0x47,0x20,0x2D,0x55,0x69,
0x64,0x20,0x32,0x37,0x00,0x00,0xDE};


char tt11[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x33,0x01,0x52
,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x1A
,0x22,0x02,0x47,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
,0x2F,0x00,0x4D,0x6F,0x64,0x65,0x00,0x00,0xDE};//51

char tt12[]={0xA1,0xA2,0x01,0x0C,0x00,0x00,0x00,0x37,0x01,0x53
,0x01,0x00,0x0C,0x00,0x04,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x1E
,0x22,0x02,0x53,0x45,0x54,0x00,0x2F,0x31,0x32,0x37,0x2E,0x30,0x2E,0x30,0x2E,0x31
,0x2F,0x00,0x41,0x63,0x6B,0x00,0x41,0x55,0x54,0x4F,0x00,0x00,0xDE};//55

                client->write(tt1, 25);
client->waitForBytesWritten(2000);
                client->write(tt2, 26);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);

                client->write(tt3, 66);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);
                client->write(tt4, 53);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);

client->readAll();
                client->write(tt5, 161);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);

QByteArray tta;
while (client->bytesAvailable() < 40) {
    continue;
}
    tta = client->readAll();

                client->write(tt6, 73);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);
                client->write(tt7, 120);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);

tt8[19]=(tta[37] - 0x30) * 10 + (tta[38] - 0x30);
tt8[76]=tta[37];
tt8[77]=tta[38];
//tt8[78]=tta.at(39);
                client->write(tt8, 81);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);

                client->write(tt11, 51);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);

tt12[19]=(tta[37] - 0x30) * 10 + (tta[38] - 0x30);
                client->write(tt12, 55);
client->waitForBytesWritten(2000);
client->waitForReadyRead(2000);

                // QApplication::aboutQt();
*/			}
        break;
    case KeyKeyboard:
    {
        //QProcess::startDetached("C:\\Program Files\\Common Files\\Microsoft Shared\\INK\\TabTip.exe", QStringList());
        QProcess::startDetached("C:\\Windows\\System32\\osk.exe", QStringList());
    }
        break;
    case KeyAddWarning:
    {
        m_ui->page_7->AddErrorCode();
    }
        break;
    case KeyRemoveWarning:
    {
        m_ui->page_7->RemoveErrorCode();
    }
        break;
    case KeyExportWarning:
    {
        QString path = QFileDialog::getSaveFileName(this, tr("导出告警列表"), "./", tr("*.csv"));
        if(path == "")
            return;
        SolutionSet *solutionSet = new SolutionSet();
        if( solutionSet->exportErrorCodeToCsv(path) )
            QMessageBox::information(this,"系统提示","导出告警列表成功！",QMessageBox::tr("确认"));
        else
            QMessageBox::information(this,"系统警告","导出告警列表失败！",QMessageBox::tr("确认"));
    }
        break;
    case KeyImportWarning:
    {
        QString path = QFileDialog::getOpenFileName(this, tr("导入告警列表"), "./", tr("*.csv"));
        if(path == "")
            return;
        SolutionSet *solutionSet = new SolutionSet();
        if( solutionSet->importCsvToErrorCode(path) ) {
            m_ui->page_7->refreshErrorCode();
            QMessageBox::information(this,"系统提示","导入告警列表成功！",QMessageBox::tr("确认"));
        } else {
            QMessageBox::information(this,"系统警告","导入告警列表失败！",QMessageBox::tr("确认"));
        }
    }
        break;
    case KeyAddKnife:
    {

        m_ui->page_4->addKnife();
    }
        break;
    case KeyResetKnife:
    {
        m_ui->page_4->resetKnife();
    }
        break;
    case KeyRemoveKnife:
    {
        m_ui->page_4->removeKnife();
    }
        break;
    case KeyLock:
    {

        userLevel=0;
        m_ui->numPad->lockKey();
        m_ui->button_F2->setEnabled(false);
        m_ui->button_F3->setEnabled(false);
        m_ui->button_F4->setEnabled(false);
        m_ui->button_F5->setEnabled(false);
        m_ui->button_F6->setEnabled(false);
        m_ui->button_F7->setEnabled(false);
        m_ui->button_F8->setEnabled(false);

        g_servo->lockServo();


    }
        break;
    case KeyUnlock:
    {
        if(DEBUG)
            pwd->setText("dzicsdzics");
        else
            pwd->setText("");
        unlock->show();

    }
        break;
    default:
        break;
    }
}

void MainWindow::setCodeRow ( int row, QString programName )
{
    if (row <= codeModel->rowCount()) {
        m_ui->codeView->setCurrentIndex(codeModel->index( row -1 ));
        m_ui->codeView->scrollTo(codeModel->index( row -1 ));
    }
    m_ui->tCodeRow->setText(programName.split('.').at(0) + QString().sprintf(":%05d", row));
    ////    QMessageBox::information(0, QString(""),QString::number(row));
    //    QApplication::processEvents();
}

//void MainWindow::updateAxisValue ( QString axisSign, double value )
//{
//    ((PageWork*)m_ui->stackedWidget->widget(0))->updateAxisValue( axisSign, value );
//}

//void QAbstractItemView::mousePressEvent ( QMouseEvent * event ) {}
//void QAbstractItemView::mouseMoveEvent ( QMouseEvent * event ) {}

//void MainWindow::connectService(int connNum)
//{
//    t1->stop();
//    int sqlParam=-1;
//    for(int i=0;i<connNum;i++)
//    {
//        if(connNum>1)
//            sqlParam=i;
//        else
//            sqlParam=curRobotIndex-1;


//        if (serviceSocket[sqlParam-1]) {
//            QString robotHosts;
//            int robotType;
//            int port = 8566;
//            QSqlQuery query;
//            query.exec(QString("SELECT RobotLibrary.type,vmid,guid,displayOrder,vmPath from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id and displayOrder=1 order by displayOrder ASC").arg(currentStationId));
//            // query.exec(QString("SELECT RobotLibrary.type,vmid,guid,displayOrder,RobotLibrary.name from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id order by displayOrder ASC").arg(currentStationId));
//            //qDebug()<<"aaa"<<QString("SELECT RobotLibrary.type,vmid,guid,displayOrder,vmPath from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id and displayOrder=1 order by displayOrder ASC").arg(currentStationId);
//            while (query.next())
//            {

//                robotType = query.value(0).toInt();
//                switch(robotType) {
//                case 11:    // ABB
//                    port = 8566;
//                    break;
//                case 14:    // ABB 2
//                    port = 8882;
//                    break;
//                case 15:    // ABB 3
//                    port = 8883;
//                    break;
//                case 21:    // FANUC
//                    port = 8567;
//                    break;
//                case 22:    // FANUC 2
//                    port = 8567;
//                    break;
//                case 31:    // UR
//                    port = 8568;
//                    break;
//                case 32:    // UR 2
//                    port = 8568;
//                    break;
//                case 41:    // DENSO
//                    port = 8569;
//                    break;
//                case 42:    // DENSO 2
//                    port = 8569;
//                    break;
//                case 51:    // Kawasaki
//                    port = 8570;
//                    break;
//                case 52:    // Kawasaki 2
//                    port = 8570;
//                    break;
//                }
//                if (robotType<100) {
//                    //                        robotHosts = "|192.168.16.97";
//                    robotHosts = "|?"+query.value(1).toString()+"|"+query.value(4).toString();

//                    QByteArray data = robotHosts.toLocal8Bit();

//                    data[0] = CMD_LOGON;
//                    data[1] = query.value(3).toInt();//发给第n个机器人
//                    // r_num=query.value(3).toInt();

//                    //注释原先双服务
//                    //                if (query.value(3).toInt() > 1) {
//                    //                    currentRobotPort2 = port + 1000;
//                    //                    sendService2(data);
//                    //                    service2Connected = true;
//                    //                } else {
//                    currentRobotPort[data[1]-1] = port;

//                    mysendService(data,data[1]);
//                    addLog(QString("%1机器连接:").arg(query.value(3).toInt())+robotHosts.mid(2),"");
//                    serviceConnected[data[1]-1] = true;




//                    // addLog(QString("port:%1  data:%2").arg(currentRobotPort).arg(data.data()),"");

//                    // }

//                }

//            }

//            ////                QString robotHosts = "?192.168.128.130|192.168.16.118";   //"?192.168.128.130";
//            //        QByteArray data = robotHosts.toLocal8Bit();
//            //        qDebug(data.data());
//            //        data[0] = CMD_LOGON;
//            //        sendService(data);
//            //        serviceConnected = true;
//        }
//    }

//    if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
//        m_ui->numPad->showKey(KeyOpenTP);
//    }


//    //        QString kName="?";
//    //        QByteArray k1=kName.toLocal8Bit();
//    //        k1[0]=NC_CMD_LOGON;
//    //        sendKnife(k1);



//}


void MainWindow::connectService(int connRobot)
{
    t1->stop();
    int sqlParam=connRobot;


    if (serviceSocket[0]) {
        QString robotHosts;
        int robotType;
        int port = 8566;
        QSqlQuery query;
        QString sqlStr=QString("SELECT RobotLibrary.type,vmid,guid,displayOrder,vmPath from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id  order by displayOrder ASC").arg(currentStationId);
        if(sqlParam>0)
            sqlStr=QString("SELECT RobotLibrary.type,vmid,guid,displayOrder,vmPath from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id and displayOrder='%2' order by displayOrder ASC").arg(currentStationId).arg(sqlParam);
        query.exec(sqlStr);
        // query.exec(QString("SELECT RobotLibrary.type,vmid,guid,displayOrder,RobotLibrary.name from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id order by displayOrder ASC").arg(currentStationId));
        //qDebug()<<"aaa"<<QString("SELECT RobotLibrary.type,vmid,guid,displayOrder,vmPath from RobotLibrary,StationMotion where stationId='%1' and StationMotion.libraryId=RobotLibrary.id and displayOrder=1 order by displayOrder ASC").arg(currentStationId);
        while (query.next())
        {

            robotType = query.value(0).toInt();
            switch(robotType) {
            case 11:    // ABB
                port = 8566;
                break;
            case 14:    // ABB 2
                port = 8882;
                break;
            case 15:    // ABB 3
                port = 8883;
                break;
            case 21:    // FANUC
                port = 8567;
                break;
            case 22:    // FANUC 2
                port = 8567;
                break;
            case 31:    // UR
                port = 8568;
                break;
            case 32:    // UR 2
                port = 8568;
                break;
            case 41:    // DENSO
                port = 8569;
                break;
            case 42:    // DENSO 2
                port = 8569;
                break;
            case 51:    // Kawasaki
                port = 8570;
                break;
            case 52:    // Kawasaki 2
                port = 8570;
                break;
            }
            if (robotType<100) {
                //                        robotHosts = "|192.168.16.97";
                robotHosts = "|?"+query.value(1).toString()+"|"+query.value(4).toString();

                QByteArray data = robotHosts.toLocal8Bit();

                data[0] = CMD_LOGON;
                data[1] = query.value(3).toInt();//发给第n个机器人
                // r_num=query.value(3).toInt();

                //注释原先双服务
                //                if (query.value(3).toInt() > 1) {
                //                    currentRobotPort2 = port + 1000;
                //                    sendService2(data);
                //                    service2Connected = true;
                //                } else {
                currentRobotPort[data[1]-1] = port;

                mysendService(data,data[1]);
                addLog(QString("%1机器连接:").arg(query.value(3).toInt())+robotHosts.mid(2),"");
                serviceConnected[data[1]-1] = true;




                // addLog(QString("port:%1  data:%2").arg(currentRobotPort).arg(data.data()),"");

                // }

            }

        }

        ////                QString robotHosts = "?192.168.128.130|192.168.16.118";   //"?192.168.128.130";
        //        QByteArray data = robotHosts.toLocal8Bit();
        //        qDebug(data.data());
        //        data[0] = CMD_LOGON;
        //        sendService(data);
        //        serviceConnected = true;
    }


    if (tpPanel && tpPanel->state() == QProcess::NotRunning) {
        m_ui->numPad->showKey(KeyOpenTP);
    }


    //        QString kName="?";
    //        QByteArray k1=kName.toLocal8Bit();
    //        k1[0]=NC_CMD_LOGON;
    //        sendKnife(k1);



}

void MainWindow::standardVal(int index, QString d)
{
    if(index==1)
        m_ui->labelMeasure9->setText(d);
    else if(index==2)
        m_ui->labelMeasure10->setText(d);
    else if(index==3)
        m_ui->labelMeasure11->setText(d);
    else if(index==4)
        m_ui->labelMeasure14->setText(d);

}



//自定义F1按钮事件
void MainWindow::on_button_F1_clicked()
{
//    static double aaa=0;
//    cpkArray.append(++aaa);
//    QString fileName=QString("d:/dzicsrobot/cpk/%1.cpk").arg(QDate::currentDate().toString("yyyyMMdd"));
//updateCpk(fileName,aaa);



    if( m_ui->button_F1->text()=="解锁")
    {
        if(unlock==NULL)
        {
            unlock=new QWidget();
            QLabel* label=new QLabel("密码：");
            label->setParent(unlock);
            label->setGeometry(20,20,150,30);
            pwd=new QLineEdit();
            pwd->setParent(unlock);
            pwd->setGeometry(200,20,150,30);
            btn_unlock=new QPushButton("解锁");

            btn_unlock->setParent(unlock);
            btn_unlock->setGeometry(20,70,100,30);
            connect(btn_unlock,SIGNAL(clicked()),this,SLOT(unlocked()));
        }
        pwd->setText("");
        unlock->show();
    }


    if (m_ui->stackedWidget->currentIndex() > 0 || m_ui->stackedWidget_2->currentIndex() > 0) {
        m_ui->stackedWidget->setCurrentIndex(0);
        m_ui->stackedWidget_2->setCurrentIndex(0);
        m_ui->frameNumPad->setVisible(true);

        m_ui->numPad->hideKey(KeyGetProgram);
        m_ui->numPad->hideKey(KeyPutProgram);
        m_ui->numPad->hideKey(KeyFindProgram);
        m_ui->numPad->hideKey(KeyGenerateMoveJ);
        m_ui->numPad->hideKey(KeyGenerateWait);
        m_ui->numPad->hideKey(KeyAddWarning);
        m_ui->numPad->hideKey(KeyRemoveWarning);
        m_ui->numPad->hideKey(KeyExportWarning);
        m_ui->numPad->hideKey(KeyImportWarning);
        m_ui->numPad->hideKey(KeyAddKnife);
        m_ui->numPad->hideKey(KeyResetKnife);
        m_ui->numPad->hideKey(KeyRemoveKnife);
        m_ui->numPad->showKey(KeyStepProgram);
        m_ui->numPad->showKey(KeyOpenXi);
        m_ui->numPad->showKey(KeyMeasureTool);
        m_ui->numPad->showKey(KeySelectTool);
        m_ui->numPad->showKey(KeyLock);
        m_ui->numPad->showKey(KeyUnlock);

    } else if (m_ui->stackedWidget_2->currentIndex() == 0) {
        //        ((Viewer*)m_ui->stackedWidget->widget(0))->moduleAllLoaded(0);
        emit showAll();
    }
    if(version==XL)
    {
        if(userLevel<2)
            m_ui->numPad->lockKey();
    }
}

void MainWindow::editCursorPositionChanged()
{
    if (!m_ui->textEdit->isReadOnly()) {
        QList<QTextEdit::ExtraSelection> extraSelections;
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(QColor(245, 245, 240));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = m_ui->textEdit->textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
        m_ui->textEdit->setExtraSelections(extraSelections);
    }
}

#include "Highlighter.h"
Highlighter *highlighter = NULL;
void MainWindow::on_button_F2_clicked()
{
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

    //	m_ui->textEdit->clear();

    if (!highlighter) {
        highlighter = new Highlighter(m_ui->textEdit->document());

        connect(m_ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(editCursorPositionChanged()));
        //		m_ui->textEdit->setViewportMargins(0,0,0,0);
    }

    //	m_ui->textEdit->setPlainText(programCode);
    highlighter->rehighlight();

    //	for(int i=0;i<programCode.count();i++)
    //		m_ui->textEdit->append(programCode.at(i));
    m_ui->stackedWidget->setCurrentIndex(1);

    //QTextDocument* doc = m_ui->textEdit->document();
    //for(QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    //{
    //		QTextBlockFormat fmt = it.blockFormat();
    //		fmt.setLineHeight(80, QTextBlockFormat::ProportionalHeight);
    //		QTextCursor cur = m_ui->textEdit->textCursor();
    //		cur.setBlockFormat(fmt);
    //		m_ui->textEdit->setTextCursor(cur);
    //}

    m_ui->stackedWidget_2->setCurrentIndex(0);

    m_ui->numPad->showKey(KeyGetProgram);
    m_ui->numPad->showKey(KeyPutProgram);
    m_ui->numPad->showKey(KeyFindProgram);
    m_ui->numPad->showKey(KeyGenerateMoveJ);
    m_ui->numPad->showKey(KeyGenerateWait);
    m_ui->numPad->hideKey(KeyRemoveKnife);

    m_ui->frameNumPad->setVisible(true);

    qApp->restoreOverrideCursor();
}
void MainWindow::on_pushButton_clicked()
{
    m_ui->lineEdit->clear();
    m_ui->lineEdit->paste();
    QUuid id = QUuid( m_ui->lineEdit->text() );
    m_ui->lineEdit_2->setText( SysUtils::uuid(id) );
    m_ui->lineEdit_2->selectAll();
    m_ui->lineEdit_2->copy();
}
//#include <QPropertyAnimation>

void MainWindow::on_button_F3_clicked()
{
    m_ui->frameNumPad->setVisible(false);

    emit changeStationTab(0);
    m_ui->stackedWidget_2->setCurrentIndex(1);
    m_ui->stackedWidget->setCurrentIndex(0);

    //    QWidget *w1 = m_ui->stackedWidget_2->widget(1);
    //    m_ui->stackedWidget_2->setCurrentIndex(0);
    //    QRect rect = w1->rect();
    //    QPropertyAnimation *animation = new QPropertyAnimation(w1, "windowOpacity");
    //     animation->setDuration(500);
    ////     animation->setStartValue(rect);
    ////     //rect.setX(- rect.right());
    ////     rect.setRight(0);
    ////     animation->setEndValue(rect);
    //     animation->setStartValue(0);
    //     animation->setEndValue(1);
    ////     animation->setEasingCurve(QEasingCurve::OutInQuart);
    //     animation->start();
    ////m_ui->stackedWidget_2->setCurrentIndex(1);
    //     QWidget *w2 = m_ui->stackedWidget_2->widget(0);
    //     QRect rect2 = w2->rect();
    //     QPropertyAnimation *animation2 = new QPropertyAnimation(w2, "windowOpacity");
    //      animation2->setDuration(500);
    ////      rect2.setRight(0);
    ////      animation2->setStartValue(rect2);
    ////      rect2.setRight(w2->width());
    ////      animation2->setEndValue(rect2);
    //      animation->setStartValue(1);
    //      animation->setEndValue(0);
    ////      animation->setEasingCurve(QEasingCurve::OutBounce);
    //      animation2->start();

    //	QString fileName = QFileDialog::getOpenFileName(this, tr("请选择方案配置文件"), "D:\\DzicsRobot",  tr("方案配置文件(*.INI)"));
    //	if (fileName.endsWith(".INI")) {
    //		if (serviceSocket) {
    //			char data[] = {CMD_STOP_VC};
    //			sendService(data, 1);
    //			serviceSocket->waitForBytesWritten(1000);
    //			Sleep(1000);
    //			serviceSocket->disconnect();
    //		}
    //		QFile::remove("D:\\DzicsRobot\\ROBOT.INI");
    //		if (QFile::copy(fileName, "D:\\DzicsRobot\\ROBOT.INI")) {
    //			qApp->quit();
    //			QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    //		}
    //	}
}

// 文件名 \/:*?"<>|=

void MainWindow::stationChanged(QString stationId)
{


    g_servo->updateUi();
    g_sysManager->sysManagerDataUpdate();
    showIndexText();
    bool needDownloadProgram = false;
    //    if (currentStationId != stationId) {
    //        needDownloadProgram = true;
    //    }
    currentStationId = stationId;

    emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  正在加载方案，请稍候...", QString("CLEARLOG"));
    processKey(KeyDisConnect);


    QSqlQuery query;
    query.exec(QString("SELECT name FROM Station WHERE id='%1'").arg(stationId));
    if(query.next()) {
        QSqlRecord record = query.record();
        m_ui->tProductName->setText(record.value("name").toString());
    }


    programFile = "D:\\DzicsRobot\\Program\\"+stationId+"\\MainModule.mod";
    if (! QFile::exists( programFile )) {
        programFile = "D:\\DzicsRobot\\Program\\"+stationId+"\\Module1.mod";
        if (! QFile::exists( programFile )) {
            programFile = "D:\\DzicsRobot\\Program\\"+stationId+"\\MainModule.mod"; // 不存在module1就还原为mainmodule
            QDir programPath = QDir("D:\\DzicsRobot\\Program\\"+stationId);
            if(programPath.exists()) {
                QStringList files = programPath.entryList(QStringList("*.mod"), QDir::Files | QDir::NoSymLinks);
                if (files.count()>0) {
                    programFile = files.at(0);
                }
            } else {
                programPath.mkpath(programPath.path());
            }
        }
    }
    programCode = QString( SysUtils::readFile( programFile ) );
    //    programCode = QString( SysUtils::readFile("D:\\DzicsRobot\\Program\\"+stationId+"\\MainModule.mod") );
    codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
    m_ui->codeView->setModel(codeModel);
    m_ui->codeView->setCurrentIndex(codeModel->index(-1));

    m_ui->textEdit->setPlainText(programCode);

    if (needDownloadProgram) {
        if (serviceSocket[currentUdpIndex]) {
            if(0==QMessageBox::information(this,tr("系统提示"),tr("需要将新方案中的指令下载到机器人系统中，并复位指令吗？"), tr("是"), tr("否"))) {
                //				char data[] = {CMD_PUT_PROGRAM};
                QString path = "?D:\\DzicsRobot\\Program\\" + currentStationId;
                char *data = path.toLocal8Bit().data();
                data[0] = CMD_PUT_PROGRAM;

                mysendService(data,strlen(data),1);
                qApp->restoreOverrideCursor();
            }
        }
    }

    // 视觉
    //if (m_ui->button_F7->isEnabled()) {
        m_ui->stackedWidget->setCurrentIndex(5);
        m_ui->page_8->switchSolution(currentStationId);
        m_ui->stackedWidget->setCurrentIndex(0);
   // }

}
void MainWindow::loadKnifeValue()
{
    QLabel *label[3];
    label[0]=m_ui->labelMeasure9;
    label[1]=m_ui->labelMeasure10;
    label[2]=m_ui->labelMeasure11;

    QLabel *label2[3];
    label2[0]=m_ui->labelMeasure12;
    label2[1]=m_ui->labelMeasure13;
    label2[2]=m_ui->labelMeasure14;

    int k1=0,k2=0;

    for(int i=0;i<Knife2List.size();i++)
    {
        if(Knife2List[i].name2[0]=='A'&&k1<3)
            label[k1++]->setText(QString::number(Knife2List[i].v1));
        if(Knife2List[i].name2[0]=='B'&&k2<3)
            label2[k2++]->setText(QString::number(Knife2List[i].v1));
    }
    //if(Knife2List.size())
}

void MainWindow::moduleAllLoaded( int count )
{


    emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  方案载入完成", QString("CLEARLOG"));
    //    processKey(KeyReConnect);

    //连接服务
    connectService();
    //打开张益芳程序
    if(version==XL&&!isOpenXLContoller)
    {

        if(QFile::exists("D:/DzicsXLContollerCom/DzicsXLContollerCom.exe"))
        {

            WinExec("D:/DzicsXLContollerCom/DzicsXLContollerCom.exe",SW_SHOWMINNOACTIVE);
        }
    }
    pageStation->querySafeArea();
    if(version<2)
    {
        loadKnifeValue();

    }

    //    if (serviceSocket) {
    //        char data[] = {CMD_LOGON};
    //        sendService(data, 1);
    //    }

    qApp->restoreOverrideCursor();
}


#define GET_BIT(a, b)	((a)>>(b)&1)

void MainWindow::updateUnitDataSlot( QByteArray data )
{
    //    addLog(QString::fromAscii(data.toHex()),"");
    //    return;

    if (data[1] == (char)0x01) { // unit input/output signal 1
        //		emit syncUnitData(data);
        //		char t = data[2];

        //        m_ui->labelInput1->setText(QString().sprintf("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", (uchar)data.at(2), (uchar)data[3], (uchar)data[4], (uchar)data[5], (uchar)data[6], (uchar)data[7], (uchar)data.at(8), (uchar)data.at(9), (uchar)data.at(10), (uchar)data.at(11), (uchar)data[12], (uchar)data[2 + 11], (uchar)data[2 + 12], (uchar)data[2 + 13], (uchar)data[2 + 14], (uchar)data[2 + 15]));
        //	//	t = data[2 + 8];
        //        m_ui->labelOutput1->setText(QString().sprintf("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", (uchar)data[2 + 16], (uchar)data[2 + 17], (uchar)data[2 + 18], (uchar)data[2 + 19], (uchar)data[2 + 20], (uchar)data[2 + 21], (uchar)data[2 + 22], (uchar)data[2 + 23], 0, 0, 0, 0, 0, 0, 0, 0));

        //        m_ui->checkDataLabel->setText( QString().sprintf("%07.3fmm", (((uchar)data.at(8) << 16 | (uchar)data.at(7) << 8 | (uchar)data.at(6)) * 0.001 )));

        //		m_ui->labelDis3->setText( QString().sprintf("%07.3f", (((uchar)data.at(10) & 0x03) << 8 | (uchar)data.at(9))  * 0.01 ));
        //		m_ui->labelDis1->setText( QString().sprintf("%07.3f", (((uchar)data.at(11) & 0x0F) << 6 | ((uchar)data.at(10) & 0xFC) >> 2) * 0.01 ));
        //		m_ui->labelDis4->setText( QString().sprintf("%07.3f", (((uchar)data.at(12) & 0x3F) << 4 | ((uchar)data.at(11) & 0xF0) >> 4) * 0.01 ));
        //		m_ui->labelDis2->setText( QString().sprintf("%07.3f", (((uchar)data.at(13) & 0xFF) << 2 | ((uchar)data.at(12) & 0xC0) >> 6) * 0.01 ));

        //		m_ui->checkDataLabel->setText( QString().sprintf("%07.3f", (((uchar)data.at(15)) << 8 | ((uchar)data.at(14))) * 0.01 ));
        m_ui->checkDataLabel->setText( QString().sprintf("%07.3fmm", (((uchar)data.at(8) << 16 | (uchar)data.at(7) << 8 | (uchar)data.at(6)) * 0.001 )));

        m_ui->labelInput1->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput1->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));

    } else if (data[1] == (char)0x02) { // unit input/output signal 2
        m_ui->labelInput2->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput2->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));

        //        m_ui->checkDataLabel->setText( QString().sprintf("%07.3fmm", (((uchar)data.at(8) << 16 | (uchar)data.at(7) << 8 | (uchar)data.at(6)) * 0.001 )));

    } else if (data[1] == (char)0x03) { // unit input/output signal 3
        m_ui->labelInput3->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput3->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));
    } else if (data[1] == (char)0x04) { // unit input/output signal 4
        m_ui->labelInput4->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput4->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));
    }
}

void MainWindow::updateProgramRowSlot( QByteArray data )
{
    int row = 0;
    row = (data[1]<<24) & (data[2]<<16) & (data[3]<<8) & data[4];
}

void MainWindow::addLogSlot(QString message, QString time)
{
    //    qDebug(message.toLocal8Bit());
    if(logList.count()>500)
    {
        logList.removeAt(0);
    }
    if (time == "CLEARLOG1") {
        logList.clear();
        logListModel->setStringList(logList);
    }

    logList += message.split("\r\n").first();
    logListModel->setStringList(logList);
    m_ui->listMessage->scrollToBottom();
    if(fileState[0])
    {

        QTextStream txtOutput(file[0]);
        txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<" addLog:"<<message<<"\r\n";
        file[0]->flush();
    }

}

void MainWindow::updateSpeedRatioSlot( int speedRatio )
{

    m_ui->speedLabel->setText(QString::number(speedRatio) + "%");
}
int getUserLevel(QString pwd)
{
    //获取用户等级
       QSqlQuery passwordquery;
       QString passwordsql=QString("select * from SysParam where code ='%1' ").arg("userpassword");
       passwordquery.exec(passwordsql);
       QString  passwordData="";
       if(passwordquery.next())
       {
            passwordData=passwordquery.value(1).toString();
       }
       if(passwordData=="")
       {
         return 0;
       }
       else
       {
           QStringList password=passwordData.split("|");
           QString aa=pwd;
           if(aa==password[0])
           {
               return 2;
           }
           else if(aa==password[1])
           {
               return 1;
           }
           else
           {
              return 0;
           }
       }

}

void MainWindow::unlocked()
{

    int level=getUserLevel(pwd->text());

    if(pwd->text()=="dzicsdzics")
    {
        m_ui->stackedWidget->setCurrentIndex(8);
        level=3;
        //m_ui->button_F9->setEnabled(true);
    }
    if(level>1)
    {

        g_servo->unlockServo();
        m_ui->numPad->unLockKey();
        m_ui->button_F2->setEnabled(true);
        m_ui->button_F3->setEnabled(true);
        m_ui->button_F4->setEnabled(true);
        m_ui->button_F5->setEnabled(true);
        m_ui->button_F6->setEnabled(true);
        m_ui->button_F7->setEnabled(true);
        m_ui->button_F8->setEnabled(true);

        userLevel=2;

    }
    else if(level==1)
    {

        userLevel=1;
        g_servo->unlockServo();
        m_ui->button_F2->setEnabled(true);
        m_ui->button_F3->setEnabled(true);

    }

    unlock->hide();
}

void MainWindow::VMState()
{


    if(vmState[curRobotIndex])
    {
        m_ui->numPad->showKey(KeyCloseVM);

    }
    else
    {
        m_ui->numPad->showKey(KeyOpenVM);

    }


}



//接收select.cpp信号
void MainWindow::setLock_slot(bool x)
{

    m_ui->numPad->setEnabled(x);
    m_ui->button_F2->setEnabled(x);
    m_ui->button_F3->setEnabled(x);
    m_ui->button_F4->setEnabled(x);
    m_ui->button_F5->setEnabled(x);
    m_ui->button_F6->setEnabled(x);
    m_ui->button_F7->setEnabled(x);
    m_ui->button_F8->setEnabled(x);
    m_ui->button_F9->setEnabled(x);
    if(!x)
        m_ui->button_F1->setText("解锁");
    else
        m_ui->button_F1->setText("三维视图");
}




void MainWindow::updateProcessTimeSlot( float sec )
{
    m_ui->processTimeLabel->setText( QString().sprintf("%07.3fs", sec) );
}

int MainWindow::mysendServiceA(const char *data, int port)
{
    return  serviceSocket[0]->writeDatagram(data, QHostAddress::LocalHost,port);
}


int MainWindow::mysendService(const char *data, int indexRobot)
{

    if(fileState[0])
    {

        QTextStream txtOutput(file[0]);
        txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<" send cmd:"<<QString::number((uchar)data[0],16)<<" data:"<<data<<"\r\n";
        file[0]->flush();
    }


    int r=-1;
    if(version==ZT0)
    {
        if(indexRobot==1)
            r=serviceSocket[0]->writeDatagram(data,  QHostAddress("192.168.128.221"), currentRobotPort[0]);
        //r=serviceSocket[0]->writeDatagram(data,  QHostAddress::LocalHost, currentRobotPort[0]);
        else if(indexRobot==2)
        {
            r=serviceSocket[1]->writeDatagram(data, QHostAddress("192.168.128.222"), currentRobotPort[1]);

        }
        else if(indexRobot==3)
            r=serviceSocket[2]->writeDatagram(data,QHostAddress("192.168.128.223"), currentRobotPort[2]);
    }
    else
    {
        r=serviceSocket[0]->writeDatagram(data,  QHostAddress::LocalHost, currentRobotPort[0]);

    }

    return r;
}


int MainWindow::mysendService(const char *data,int len, int indexRobot)
{
    if(fileState[0])
    {
        QTextStream txtOutput(file[0]);
        txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<" send cmd:"<<QString::number((uchar)data[0],16)<<" data:"<<data<<"\r\n";
        file[0]->flush();
    }

    int r=-1;
    if(version==ZT0)
    {
        if(indexRobot==1)
            r=serviceSocket[0]->writeDatagram(data,len,  QHostAddress("192.168.128.221"), currentRobotPort[0]);
        //r=serviceSocket[0]->writeDatagram(data,len,  QHostAddress::LocalHost, currentRobotPort[0]);
        else if(indexRobot==2)
        {
            r=serviceSocket[1]->writeDatagram(data, len,QHostAddress("192.168.128.222"), currentRobotPort[1]);

        }
        else if(indexRobot==3)
            r=serviceSocket[2]->writeDatagram(data,len,QHostAddress("192.168.128.223"), currentRobotPort[2]);
    }
    else
    {
        r=serviceSocket[0]->writeDatagram(data,len,  QHostAddress::LocalHost, currentRobotPort[0]);
        // emit addLog(QString("port : %1").arg(currentRobotPort[0]),"");
    }
    return r;
}

int MainWindow::sendService2( QByteArray data )
{
    //qDebug("sendService2 %d", currentRobotPort2);
    return service2Socket->writeDatagram(data, QHostAddress::LocalHost, 18566);    //QHostAddress::LocalHost  QHostAddress("192.168.16.216")
}

int MainWindow::sendService2( const char *data, int len )
{
    qDebug("sendServiceLen2 %d", currentRobotPort2);
    return service2Socket->writeDatagram(data, len, QHostAddress::LocalHost, currentRobotPort2);
}

int MainWindow::sendKnife(QByteArray data)
{

    return knifeSocket->writeDatagram(data, QHostAddress::LocalHost, knifePort);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (this->isFullScreen()) return;
    if (QApplication::widgetAt(event->globalPos())->objectName() != "label_title") return;
    if( event->button() == Qt::LeftButton)
    {
        mMoving = true;
        mLastMousePosition = event->globalPos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(mMoving &&  event->buttons().testFlag(Qt::LeftButton))
    {
        this->move(this->pos() + (event->globalPos() - mLastMousePosition));
        mLastMousePosition = event->globalPos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if(mMoving && event->button() == Qt::LeftButton)
    {
        mMoving = false;
    }
}


void MainWindow::updateProgram ( QString programText )
{
    programCode = programText;
    codeModel = new QStringListModel(programText.replace("\t", " ").split('\n'));
    m_ui->codeView->setModel(codeModel);
    m_ui->codeView->setCurrentIndex(codeModel->index(-1));

    qDebug(programText.toLocal8Bit());

    m_ui->textEdit->setPlainText(programText);

}

void MainWindow::updateTmMeasure( QImage *image, float values[], bool valids[] )
{
    try {
        if (mainImage) {
            mainImage->setPixmap(QPixmap::fromImage(*image));
            if (!mainImage->isVisible())
                mainImage->setVisible(true);
        }
        static QLabel* labelMeasures[16] = {m_ui->labelMeasure1,m_ui->labelMeasure2,m_ui->labelMeasure3,m_ui->labelMeasure4,m_ui->labelMeasure5,m_ui->labelMeasure6,m_ui->labelMeasure7,m_ui->labelMeasure8,m_ui->labelMeasure9,m_ui->labelMeasure10,m_ui->labelMeasure11,m_ui->labelMeasure12,m_ui->labelMeasure13,m_ui->labelMeasure14,m_ui->labelMeasure15,m_ui->labelMeasure16};

        QPalette pValid, pInvalid;
        pValid.setColor(QPalette::WindowText, Qt::black);
        pInvalid.setColor(QPalette::WindowText, Qt::red);
        for (int i=0;i<16;i++){
            labelMeasures[i]->setText( QString().sprintf("%0.3f", values[i]) );
            labelMeasures[i]->setPalette(valids[i] ? pValid : pInvalid);
        }

    } catch(...) {

    }
}

void MainWindow::updateTmImage( QImage *image )
{
    try {
        if (mainImage) {
            mainImage->setPixmap(QPixmap::fromImage(*image));
            if (!mainImage->isVisible())
                mainImage->setVisible(true);
        }
    } catch(...) {

    }
}

void MainWindow::updateTmValue( QList<float> *valueList, QList<char> *valueStateList)
{
    try {
        static QLabel* labelMeasures[16] = {m_ui->labelMeasure1,m_ui->labelMeasure2,m_ui->labelMeasure3,m_ui->labelMeasure4,m_ui->labelMeasure5,m_ui->labelMeasure6,m_ui->labelMeasure7,m_ui->labelMeasure8,m_ui->labelMeasure9,m_ui->labelMeasure10,m_ui->labelMeasure11,m_ui->labelMeasure12,m_ui->labelMeasure13,m_ui->labelMeasure14,m_ui->labelMeasure15,m_ui->labelMeasure16};

        QPalette pValid, pInvalid;
        pValid.setColor(QPalette::WindowText, Qt::black);
        pInvalid.setColor(QPalette::WindowText, Qt::red);
        if(version==TEST&&valueList->size()>0)
        {
            calcTmValue(valueList->at(0));
        }
        for (int i=0;i<valueList->count()&&i<16;i++){
            labelMeasures[i]->setText( valueStateList->at(i)==3 ? "" : QString().sprintf("%0.3f", valueList->at(i)) );     //QString::number(valueList->at(i)) );//
            labelMeasures[i]->setPalette(valueStateList->at(i)==1 ? pValid : pInvalid);
        }
    } catch(...) {

    }
}

void MainWindow::updateTmGapResult(bool result, QString params)
{
    try {
        QStringList outputVarList = params.split(",");
        while (outputVarList.length()<1) outputVarList.append("");
        if (!outputVarList[0].isEmpty()) {
            if (serviceSocket[0]) {
                QString str = "??" + outputVarList[0] + "|" + (result?"1":"0");
                QByteArray dataArray = str.toLocal8Bit();
                dataArray[0] = CMD_WRITE_VAR;
                dataArray[1] = 1;
                mysendService(dataArray.data(),dataArray.length(),1);
            }
        }
        emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  完成间隙检测：" + (result?"合格":"不合格"), QString());
    } catch(...) {

    }
}

void MainWindow::updateWeightValue( float value )
{
    //    try {
    //        m_ui->weightLabel->setText( QString().sprintf("%0.3fkg", value) );
    //    } catch(...) {

    //    }
}
void MainWindow::conn1()
{
    connectService();
}

void MainWindow::runProc()
{


    QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "请确认推车数量", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
    {
        return;
    }

    QStringList codeList= m_ui->textEdit->toPlainText().replace("\t", " ").split('\n');
    //qDebug()<<"aaa"<<codeList[curRow-1];

    if(curRow>0)
        emit addLog("测试值："+codeList[curRow-1],"");
    if(curRow>0&&curRow<codeList.size()&&codeList[curRow-1]=="Main_Start:")
    {
        if (serviceSocket[currentUdpIndex]) {
            char data[] = {CMD_START_PROGRAM,curRobotIndex};

            mysendService(data,curRobotIndex);

        }
    }
    else
    {
        emit addLog("不在main()行首","");
    }


}


