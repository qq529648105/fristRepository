#include "CameraCalibrate.h"
#include "ui_CameraCalibrate.h"
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QSqlQuery>
#include <QFileDialog>
#include "SysUtils.h"
#include  <QCursor>
#include <QMap>
#include <QMessageBox>
#include "CameraManager.h"
#include "CameraProperty.h"
#include <QTimer>
#include <QBuffer>
#include <QSqlError>
#include <QDebug>
#include "CaptureCamera.h"
#include "CaptureCamera2.h"
#include "CaptureCamera3.h"
#include "CaptureCameraHik.h"
#include "cv.h"
#include "highgui.h"
#include <QMutex>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include "Global.h"
#include <QListView>
#include <QDesktopServices>
extern int version;


static int mouseX =0;
static  int mouseY = 0;
static int lastMouseX = 0;
static int lastMouseY = 0;
static int bigMouseX = 0;
static int bigMouseY = 0;
static int cMouseX=0;
static int cMouseY=0;

static int bestWidth;
static int bestHeight;



QMap<QString,CameraProperty*>cameraMap;
QMap<QString, CameraTemplate*>templateMap;
QMap<int,QString>tpCodeMap;


QMutex lock;
QMutex mylock[5];
IplImage *cameraIpImg;
QMap<int,IplImage *> IpImgCache;
QMap<int,IplImage *> IpImgArray[5];
int cur_cameraIndex;
QString   cameraImage;
QList<CameraTemplate*> autoExposure;
bool showVisionRet[5];
Point showRect[5];
bool saveCache[5];
extern QLabel * mainImage;

//<<<
int calibrateResult;        //0 检测失败 1 检测成功
int calibrateOffsetX;       //偏移量X
int calibrateOffsetY;        //偏移量Y
int calibrateOffseZ;
int calibrateAngle;           //旋转角度
double pixelRatio;          //像素比例

using namespace cv;
CameraTemplate::CameraTemplate(QObject *parent):
    QObject(parent)
{

}
CameraCalibrate* g_camera=NULL;

CameraCalibrate::CameraCalibrate(QWidget *parent) :
    QWidget(parent),isButtonPressd(false),
    ui(new Ui::CameraCalibrate)
{
    ui->setupUi(this);
    ui->fullWidget->installEventFilter(this);
    ui->fullWidget->setMouseTracking(true);
    initTemplateWidget();
    currentSolutionId = "";
    currentCameraId = "";
    rightUpPos  = QPoint(0,0);
    areaStartPos = QPoint(0,0);
    areaEndPos  = QPoint(0,0);
    cameraStartPos = QPoint(0,0);
    cameraEndPos = QPoint(0,0);
    areatopLeft = QPoint(0,0);
    areaBottomRight = QPoint(0,0);
    selectTmpId = "";
    selectCenterId = "";
    spinLeftUpX = NULL;
    spinLeftUpY = NULL;
    spinRightDownX = NULL;
    spinRightDownY = NULL;
    spinWidth = NULL;
    spinHeight = NULL;
    spinBrightTmp = NULL;
    spinBrightCenter = NULL;
    displyIpImg=NULL;
    displyImg=NULL;
    largerIpImg=NULL;
    largerImg=NULL;

    ui->tabWidget->setCurrentIndex(3);
    areaShowState = AFFRAMSHOWSTATE;
    settingState = MOUSESETTING;
    ui->SliderExposure->setTickPosition(QSlider::TicksBelow);
    ui->SliderGain->setTickPosition(QSlider::TicksBelow);
    pixMouse.load(":/icon/arrow_l.ico");
    cur_cameraIndex=0;
    recordLog=false;

    initCamera();




    ui->areaSpinBoxDown->setReadOnly(true);
    ui->areaSpinBoxLeft->setReadOnly(true);
    ui->areaSpinBoxRight->setReadOnly(true);
    ui->areaSpinBoxUp->setReadOnly(true);

    //zhu
    g_camera=this;
    isAjdust=true;
    ui->comboBoxCamera->setStyleSheet("QComboBox QAbstractItemView::item{height:40px;}");
    ui->comboBoxCamera->setView(new QListView());

    ui->comboBoxTemplate->setStyleSheet("QComboBox QAbstractItemView::item{height:40px;}");
    ui->comboBoxTemplate->setView(new QListView());
    ui->comboBoxTemplate->addItem("选择工件模板",0);
    ui->comboBoxTemplate->addItem("76mm",76);
    ui->comboBoxTemplate->addItem("86mm",86);
    ui->comboBoxTemplate->addItem("127mm",127);
    ui->comboBoxTemplate->addItem("164mm",164);
    ui->comboBoxTemplate->addItem("180mm",180);



}
    //grayImage = cvCreateImage(cvGetSize(cameraIpImg),8,1);

    //readFile();

//    screenWidth = ui->fullWidget->width();
//    screenHeight = ui->fullWidget->height();
//    initIpImage();
//    captureCamera->startCaptureCamera();
//    if(ui->comboBoxCamera->count() != 0)
//        QTimer::singleShot(200,this,SLOT(startCapCamera()));



void CameraCalibrate::initCamera()
{
    for(int i=0;i<5;i++)
    {
        captureCamera[i]=NULL;
        showVisionRet[i]=false;
    }
    QSqlQuery sql;
    QString solutionId1=SysUtils::getSysParam( "lastStationId" ).toString();
    sql.exec(QString("select name ,code ,picNum,interval from CameraProperty where solutionId='%1'").arg(solutionId1));
    ipAraay.clear();
    camModel.clear();
    cameraList.clear();
    while(sql.next())
    {

        ipAraay.append(sql.value(0).toString());
        camModel.append(sql.value(1).toInt());
        CameraProperty *camera=new CameraProperty();
        camera->name=sql.value(0).toString();
        camera->model=sql.value(1).toString();
        camera->picNum=sql.value(2).toInt();
        camera->interval=sql.value(3).toInt();
        cameraList.append(camera);
    }

//    if(ipAraay.length()==0)
//        ipAraay<<"0.0.0.0";
    for(int i=0;i<ipAraay.length();i++)
    {
        //qDebug()<<"ipArray"<<ipAraay[i]<<camModel[i];
        if(camModel[i]==0)
        {

             captureCamera[i] = new CaptureCamera2(i,1280,960,this);
        }
        else if(camModel[i]==1)
        {
            captureCamera[i] = new CaptureCamera2(i,2592,1944,this);

        }else if(camModel[i]==2){
            captureCamera[i] = new CaptureCamera(ipAraay[i].toLocal8Bit(),i,this);
        }else if(camModel[i]==3){
            captureCamera[i] =new CaptureCamera3(0,640,480);
        }else if(camModel[i]==4){
            captureCamera[i] =new CaptureCameraHik(ipAraay[i].toLocal8Bit(),i,this);
        }
        saveCache[i]=false;
        captureCamera[i]->picNum=cameraList[i]->picNum;
        captureCamera[i]->interval=cameraList[i]->interval;
        qDebug()<<"cameraCalibrate"<<cameraList[i]->interval<<cameraList[i]->picNum;
        connect(captureCamera[i],SIGNAL(signalCamera()),this,SLOT(getCameraImage()));

    }


}

bool CameraCalibrate::getCheckBox()
{
    return ui->checkBox_2->isChecked();
}

int CameraCalibrate::getTemplateIndex()
{
    return ui->comboBoxTemplate->itemData(ui->comboBoxTemplate->currentIndex()).toInt();
}

int CameraCalibrate::getCamIndex(QString name)
{
    int index=-1;
    for(int i=0;i<ui->comboBoxCamera->count();i++)
    {
        if(ui->comboBoxCamera->itemText(i)==name)
        {

            index=i;
            break;
        }
    }
    return index;
}



void CameraCalibrate::setLabelText(QString &s)
{
    ui->label_15->setText(s);
}

CameraCalibrate::~CameraCalibrate()
{
    delete ui;
    for(int i=0;i< ipAraay.length();i++)
    {
      // captureCamera[i]->stopCaptureCamera();
        //if(captureCamera[i]->runFlag)
        //delete captureCamera[i];

        if(captureCamera[i])
        {
        captureCamera[i]->runFlag=false;
        qDebug()<< i<<captureCamera[i]->runFlag;
        captureCamera[i]->quit();
        captureCamera[i]->wait();

        delete captureCamera[i];




        }

    }



}





void CameraCalibrate::initIpImage()
{
       if(displyIpImg)
       {
           cvReleaseImage(&displyIpImg);
           displyIpImg=NULL;
       }
       if(displyImg)
       {
           delete displyImg;
           displyImg=NULL;
       }
       if(largerIpImg)
       {
           cvReleaseImage(&largerIpImg);
           largerIpImg=NULL;
       }
       if(largerImg)
       {
           delete largerImg;
           largerImg=NULL;
       }
    displyIpImg = cvCreateImageHeader(cvSize(ui->fullWidget->width(), ui->fullWidget->height()),IPL_DEPTH_8U, 3);
    displyImg = new QImage(QSize(ui->fullWidget->width(), ui->fullWidget->height()), QImage::Format_RGB888);
    displyImg->fill(Qt::black);
    displyIpImg->imageData = (char*)displyImg->bits();

int w1 = ui->largerWidget->width();
int h1 = ui->largerWidget->height();
    ui->largerWidget->resize(ui->largerWidget->width()+(ui->largerWidget->width() % 4), ui->largerWidget->height()+(ui->largerWidget->height() % 4));
    w1 = ui->largerWidget->width();
    h1 = ui->largerWidget->height();

    largerIpImg = cvCreateImageHeader(cvSize(w1,h1),IPL_DEPTH_8U,3);
    largerImg = new QImage(QSize(ui->largerWidget->width(),ui->largerWidget->height()),QImage::Format_RGB888);
    largerImg->fill(Qt::black);
    largerIpImg->imageData = (char*)largerImg->bits();

    //update zhugz
    largerIpImg2 = cvCreateImageHeader(cvSize(168,168),IPL_DEPTH_8U,3);
    mainImg=new QImage(QSize(168,168),QImage::Format_RGB888);
    mainImg->fill(Qt::black);
    largerIpImg2->imageData = (char*)mainImg->bits();
    //end

    getScaleValue();

    if (mainImage && !mainImage->isVisible()) {
        mainImage->setVisible(true);
    }

}


void CameraCalibrate::getScaleValue()
{

    if(currentCameraId != ""){
        int cameraWidth=1280;
        int cameraHeight=960;

        if(areaShowState==ALLSHOWSTATE && settingState == AREASETTING){
            if(cameraMap[currentCameraId])
            {
            cameraWidth = cameraMap[currentCameraId]->cameraWidth;
            cameraHeight = cameraMap[currentCameraId]->cameraHeight;
            }
        }else if(areaShowState==AFFRAMSHOWSTATE && settingState ==AREASETTING){
            cameraWidth = abs(cameraEndPos.x() - cameraStartPos.x());
            cameraHeight = abs(cameraEndPos.y() - cameraStartPos.y());
       //<<< }else if(settingState==CENTERSETTING || settingState==TMPSETTING){
              }else if(settingState==CENTERSETTING || settingState==TMPSETTING||settingState==MOUSESETTING){
            if(cameraMap[currentCameraId])
            {
            cameraWidth = cameraMap[currentCameraId]->arearightDownX -cameraMap[currentCameraId]->arealeftUpX;
            cameraHeight =cameraMap[currentCameraId]->arearightDownY - cameraMap[currentCameraId]->arealeftUpY;
            }
        }


        if(cameraWidth<=0 || cameraHeight<=0){
            return;
        }

        scaleWidth = (float)ui->fullWidget->width()/cameraWidth;
        scaleHeight = (float)ui->fullWidget->height()/cameraHeight;
        scaleWidth>scaleHeight ? scaleWidth=scaleHeight : scaleHeight=scaleWidth;
        cvResetImageROI(displyIpImg);
        cvSetImageROI(displyIpImg,cvRect(0,(ui->fullWidget->height()-cameraHeight*scaleHeight)/2,cameraWidth*scaleWidth,cameraHeight*scaleHeight));
        rightUpPos.setY((ui->fullWidget->height()-cameraHeight*scaleHeight)/2);
        rightUpPos.setX(cameraWidth*scaleWidth);
        mouseX = rightUpPos.x()/2;
        mouseY = (ui->fullWidget->height()-2*rightUpPos.y())/2;
        bigMouseX = rightUpPos.x()/2;   //-1;
        bigMouseY = (ui->fullWidget->height()-2*rightUpPos.y())/2;
//        lastMouseX = mouseX;
//        lastMouseY = mouseY;

    }

}


void CameraCalibrate::showEvent(QShowEvent *e)
{

    qDebug("showEvent");
    static bool isShow = false;
    if(isShow)
        return;

    screenWidth = ui->fullWidget->width();
    screenHeight = ui->fullWidget->height();
//    initIpImage();


//    if(ui->comboBoxCamera->count() != 0)
//        QTimer::singleShot(200,this,SLOT(startCapCamera()));
    isShow = true;


}


//void CameraCalibrate::startCapCamera(){
//    if(!captureCamera->isRunning()){
//       captureCamera->startCaptureCamera();
//    }
//}

/*****************************
 接口函数: 切换解决方案对应不同的标定方案
 参数:  solutionId 解决方案ID
 ****************************/
void CameraCalibrate::switchSolution(const QString &solutionId)
{
    //captureCamera[0]->stopCaptureCamera();
    //captureCamera[1]->stopCaptureCamera();
//    for(int i=0;i<ipAraay.length();i++)
//    {
//         captureCamera[i]->stopCaptureCamera();
//    }

    static bool isTpCode = false;
    if(!isTpCode){
        tpCodeMap.clear();
        QSqlQuery query;
        query.exec(QString("SELECT id ,code FROM CameraTemplate  ORDER BY code "));
        while(query.next())
        {
            QString id = query.value(0).toString();
            int code = query.value(1).toInt();
            tpCodeMap.insert(code,id);
        }
        isTpCode = true;
    }
    currentSolutionId = solutionId;
    initSetting(solutionId);

}


void CameraCalibrate::initSetting(const QString &solutionId)
{
    disconnect(ui->comboBoxCamera,SIGNAL(currentIndexChanged(int)),this,SLOT(switchCamera(int)));
    disconnect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(setStateChanged(int)));
    disconnect(ui->areaSpinBoxUp,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxUp(int)));
    disconnect(ui->areaSpinBoxLeft,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxLeft(int)));
    disconnect(ui->areaSpinBoxRight,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxRight(int)));
    disconnect(ui->areaSpinBoxDown,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxDown(int)));
    int count = ui->comboBoxCamera->count();
    for(int i=0; i<count; i++)
        ui->comboBoxCamera->removeItem(0);
    cameraMap.clear();
    autoExposure.clear();
    QSqlQuery query,query22;
    int code1=0;

    query.exec(QString("SELECT id,sn,name,exposure,gain,cameraWidth,cameraHeight, areaLeftUpX, areaLeftUpY,areaRightDownX,areaRightDownY,model,code,maxGain,minGain,maxExposure,minExposure,testImage,picNum,interval FROM CameraProperty WHERE solutionId ='%1'").arg(solutionId));
    while(query.next()){
        CameraProperty *cameraProperty = new CameraProperty(this);
        cameraProperty->id = query.value(0).toString();
        cameraProperty->sn = query.value(1).toInt();
        cameraProperty->name = query.value(2).toString();
        cameraProperty->exposure = query.value(3).toInt();
        cameraProperty->gain = query.value(4).toInt();
//        cameraProperty->cameraWidth = query.value(5).toInt();
//        cameraProperty->cameraHeight = query.value(6).toInt();
        code1=query.value(12).toInt();
        if(code1==0){
            cameraProperty->cameraWidth = 1280;
            cameraProperty->cameraHeight = 960;
        }else if(code1==1){
            cameraProperty->cameraWidth = 2592;
            cameraProperty->cameraHeight = 1944;
        }else if(code1==2){
            cameraProperty->cameraWidth = 1624;
            cameraProperty->cameraHeight = 1234;
        }else if(code1==3){
            cameraProperty->cameraWidth = 640;
            cameraProperty->cameraHeight = 480;
        }else if(code1==4){
            cameraProperty->cameraWidth = 2448;
            cameraProperty->cameraHeight = 2048;
        }
        cameraProperty->arealeftUpX = query.value(7).toInt();
        cameraProperty->arealeftUpY = query.value(8).toInt();
        cameraProperty->arearightDownX = query.value(9).toInt();
        cameraProperty->arearightDownY = query.value(10).toInt();
        if (cameraProperty->arealeftUpX == cameraProperty->arearightDownX || cameraProperty->arealeftUpY == cameraProperty->arearightDownY) {
            cameraProperty->arealeftUpX = 0;
            cameraProperty->arealeftUpY = 0;
            cameraProperty->arearightDownX = cameraProperty->cameraWidth;
            cameraProperty->arearightDownY = cameraProperty->cameraHeight;
        }
        cameraProperty->model = query.value(11).toString();
        cameraProperty->code = query.value(12).toInt();


        cameraProperty->maxGain = query.value(13).toInt();
        cameraProperty->minGain = query.value(14).toInt();
        cameraProperty->maxExposure = query.value(15).toInt();
        cameraProperty->minExposure = query.value(16).toInt();
        cameraProperty->testImage = query.value(17).toString();
        cameraProperty->picNum = query.value(18).toInt();
        cameraProperty->interval=query.value(19).toInt();

        cameraMap.insert(cameraProperty->id,cameraProperty);
        ui->comboBoxCamera->addItem(cameraProperty->name,cameraProperty->id);

        //update by zhugz (exposure template add)
        QStringList list=cameraMap.keys();

        if(list.size()>0)
        {
            if(cameraMap[list[0]]->model=="checked")
                ui->checkBox->setChecked(true);
        }
        query22.exec(QString("SELECT id,code,leftUpX,leftUpY,rightDownX,rightDownY,bright FROM CameraTemplate WHERE cameraId ='%1'").arg(cameraProperty->id));
        while(query22.next()){
            if(query22.value(1).toInt()<10)
            {
                CameraTemplate* cTemp=new CameraTemplate();
                cTemp->id=query22.value(0).toInt();
                cTemp->code=query22.value(1).toInt();
                cTemp->leftUpX=query22.value(2).toInt();
                cTemp->leftUpY=query22.value(3).toInt();
                cTemp->rightDownX=query22.value(4).toInt();
                cTemp->rightDownY=query22.value(5).toInt();
                cTemp->bright=query22.value(6).toInt();

                autoExposure.append(cTemp);

                break;
            }

        }
        //end
    }

    if(cameraMap.isEmpty()) {

        initIpImage();
        return;
    }
    switchCamera(0);
    connect(ui->comboBoxCamera,SIGNAL(currentIndexChanged(int)),this,SLOT(switchCamera(int)));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(setStateChanged(int)));
    connect(ui->areaSpinBoxUp,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxUp(int)));
    connect(ui->areaSpinBoxLeft,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxLeft(int)));
    connect(ui->areaSpinBoxRight,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxRight(int)));
    connect(ui->areaSpinBoxDown,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxDown(int)));
}


void CameraCalibrate::switchCamera(int index)
{


    cur_cameraIndex=ui->comboBoxCamera->currentIndex();
    areaShowState = AFFRAMSHOWSTATE;
    currentCameraId =ui->comboBoxCamera->itemData(index).toString();
    templateMap.clear();
    //   tpCodeMap.clear();
    disconnect(ui->tableWidgetTmp,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    disconnect(ui->tableWidgetCenter,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    selectTmpId ="";
    selectCenterId ="";
    ui->tableWidgetTmp->clearContents();
    ui->tableWidgetCenter->clearContents();
    int rowCount = ui->tableWidgetTmp->rowCount();
    for(int row= 0; row<rowCount;++row)
        ui->tableWidgetTmp->removeRow(0);
    rowCount = ui->tableWidgetCenter->rowCount();
    for(int row=0; row<rowCount;++row)
        ui->tableWidgetCenter->removeRow(0);
    spinLeftUpX = NULL;
    spinLeftUpY = NULL;
    spinRightDownX = NULL;
    spinRightDownY = NULL;
    spinHeight = NULL;
    spinWidth = NULL;
    spinBrightTmp = NULL;
    spinBrightCenter = NULL;
    QSqlQuery query;
    query.exec(QString("SELECT id,name,type,code,leftUpX,leftUpY,rightDownX,rightDownY,tmpWidth,tmpHeight,bright FROM CameraTemplate WHERE cameraId='%1' ORDER BY code ").arg(currentCameraId));
    while(query.next()){
        CameraTemplate *cameraTemplate = new CameraTemplate(this);
        cameraTemplate->id =  query.value(0).toString();
        cameraTemplate->name = query.value(1).toString();
        cameraTemplate->type = query.value(2).toInt();
        cameraTemplate->code = query.value(3).toInt();
        cameraTemplate->leftUpX = query.value(4).toInt();
        cameraTemplate->leftUpY = query.value(5).toInt();
        cameraTemplate->rightDownX = query.value(6).toInt();
        cameraTemplate->rightDownY = query.value(7).toInt();
        cameraTemplate->tmpWidth = query.value(8).toInt();
        cameraTemplate->tmpHeight = query.value(9).toInt();
        cameraTemplate->bright = query.value(10).toInt();
        templateMap.insert(cameraTemplate->id,cameraTemplate);
        //   tpCodeMap.insert(cameraTemplate->code, cameraTemplate->id);
        QSpinBox *spinBright = new QSpinBox(this);
        spinBright->setMaximum(100);
        spinBright->setValue(cameraTemplate->bright);

        if(cameraTemplate->type  ==0){

            ui->tableWidgetTmp->insertRow(ui->tableWidgetTmp->rowCount());
            QSpinBox *spinLeftUpX = new QSpinBox(this);
            QSpinBox *spinLeftUpY = new QSpinBox(this);
            QSpinBox *spinRightDownX = new QSpinBox(this);
            QSpinBox *spinRightDownY = new QSpinBox(this);

            spinLeftUpX ->setReadOnly(true);
            spinLeftUpY->setReadOnly(true);
            spinRightDownX->setReadOnly(true);
            spinRightDownY->setReadOnly(true);
            spinBright->setReadOnly(true);

            spinLeftUpX->setMaximum(cameraMap[currentCameraId]->arearightDownX - cameraMap[currentCameraId]->arealeftUpX);
            spinLeftUpY->setMaximum(cameraMap[currentCameraId]->arearightDownY - cameraMap[currentCameraId]->arealeftUpY);
            spinRightDownX->setMaximum(cameraMap[currentCameraId]->arearightDownX - cameraMap[currentCameraId]->arealeftUpX);
            spinRightDownY->setMaximum(cameraMap[currentCameraId]->arearightDownY - cameraMap[currentCameraId]->arealeftUpY);
            spinLeftUpX->setValue( cameraTemplate->leftUpX);
            spinLeftUpY->setValue( cameraTemplate->leftUpY);
            spinRightDownX->setValue( cameraTemplate->rightDownX);
            spinRightDownY->setValue( cameraTemplate->rightDownY);
            ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,0,new QTableWidgetItem(QString("%1").arg(QString::number(cameraTemplate->code))));
            ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,1,new QTableWidgetItem( cameraTemplate->name));
            ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,2,spinBright);
            ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,3,spinLeftUpX);
            ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,4,spinLeftUpY);
            ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,5,spinRightDownX);
            ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,6,spinRightDownY);

            ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,7, new QTableWidgetItem(""));
            ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,8, new QTableWidgetItem(""));
            ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,9, new QTableWidgetItem(""));
            ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,10, new QTableWidgetItem( cameraTemplate->id));
        }
        else if( cameraTemplate->type ==1){
            ui->tableWidgetCenter->insertRow(ui->tableWidgetCenter->rowCount());
            QSpinBox *spinWidth = new QSpinBox(this);
            QSpinBox *spinHeight = new QSpinBox(this);
            spinWidth->setReadOnly(true);
            spinHeight->setReadOnly(true);
            spinWidth->setMaximum(cameraMap[currentCameraId]->arearightDownX - cameraMap[currentCameraId]->arealeftUpX);
            spinHeight->setMaximum(cameraMap[currentCameraId]->arearightDownY - cameraMap[currentCameraId]->arealeftUpY);
            ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,0,new QTableWidgetItem(QString("%1").arg(QString::number( cameraTemplate->code))));
            // ui->tableWidgetCenter->item(ui->tableWidgetCenter->rowCount()-1,0)->setFlags(  ui->tableWidgetCenter->item(ui->tableWidgetCenter->rowCount()-1,0)->flags() & (~Qt::ItemIsEditable));
            ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,1,new QTableWidgetItem( cameraTemplate->name));
            ui->tableWidgetCenter->setCellWidget(ui->tableWidgetCenter->rowCount()-1,2,spinWidth);
            ui->tableWidgetCenter->setCellWidget(ui->tableWidgetCenter->rowCount()-1,3,spinHeight);
            spinWidth->setValue( cameraTemplate->tmpWidth);
            spinHeight->setValue( cameraTemplate->tmpHeight);
            ui->tableWidgetCenter->setCellWidget(ui->tableWidgetCenter->rowCount()-1,4,spinBright);
            ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,5, new QTableWidgetItem(""));
            ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,6, new QTableWidgetItem(""));
            ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,7, new QTableWidgetItem(""));
            ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,8, new QTableWidgetItem( cameraTemplate->id));
        }
    }

    connect(ui->tableWidgetTmp,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    connect(ui->tableWidgetCenter,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    if(cameraMap[currentCameraId]==NULL)
        return;
    cameraStartPos = QPoint(cameraMap[currentCameraId]->arealeftUpX,cameraMap[currentCameraId]->arealeftUpY);
    cameraEndPos = QPoint(cameraMap[currentCameraId]->arearightDownX,cameraMap[currentCameraId]->arearightDownY);
    tpLeftUpX =0;
    tpLeftUpY =0;
    tpRightDownX =0;
    tpRightDownY =0;
    tpWidth =0;
    tpHeight =0;
    ui->areaSpinBoxLeft->setMaximum(cameraMap[currentCameraId]->cameraWidth);
    ui->areaSpinBoxRight->setMaximum(cameraMap[currentCameraId]->cameraWidth);
    ui->areaSpinBoxUp->setMaximum(cameraMap[currentCameraId]->cameraHeight);
    ui->areaSpinBoxDown->setMaximum(cameraMap[currentCameraId]->cameraHeight);

    updateAraePosValue();
    updateCameraInfo();

    initIpImage(); // daiyl
   // QTimer::singleShot(200,this,SLOT(startCapCamera()));

    if(index >= 0)
    {
        for(int i=0;i<ipAraay.length();i++)
        {
            if(captureCamera[i])
            captureCamera[i]->startCaptureCamera();

        }

       //captureCamera[0]->startCaptureCamera();
       //captureCamera[1]->startCaptureCamera();
    }

    cameraImage = cameraMap[currentCameraId]->testImage;

    //startCapCamera();
}



void CameraCalibrate::initTemplateWidget()
{
    ui->tableWidgetTmp->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidgetCenter->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidgetTmp->hideColumn(10);
    ui->tableWidgetCenter->hideColumn(8);
    ui->tableWidgetTmp->hideColumn(1);
    ui->tableWidgetCenter->hideColumn(1);
    ui->tableWidgetTmp->verticalHeader()->setVisible(false);
    ui->tableWidgetCenter->verticalHeader()->setVisible(false);
    connect(ui->tableWidgetTmp,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    connect(ui->tableWidgetCenter,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    connect(ui->tableWidgetTmp,SIGNAL(cellClicked(int,int)),this,SLOT(editTmpItem(int,int)));
    connect(ui->tableWidgetCenter,SIGNAL(cellClicked(int,int)),this,SLOT(editCenterItem(int,int)));

}


void CameraCalibrate::editTmpItem(int row,int column)
{
    ui->tableWidgetTmp->editItem(ui->tableWidgetTmp->item(row, column));


}

void CameraCalibrate::editCenterItem(int row ,int column)
{

    ui->tableWidgetCenter->editItem(ui->tableWidgetCenter->item(row,column));
}


void CameraCalibrate::updateCameraInfo()
{

    if(cameraMap[currentCameraId]==NULL)
        return;
    ui->SliderExposure->setMinimum(cameraMap[currentCameraId]->minExposure);
    ui->SliderExposure->setMaximum(cameraMap[currentCameraId]->maxExposure);
    ui->spinBoxExposure->setMinimum(cameraMap[currentCameraId]->minExposure);
    ui->spinBoxExposure->setMaximum(cameraMap[currentCameraId]->maxExposure);
    ui->SliderGain->setMinimum(cameraMap[currentCameraId]->minGain);
    ui->SliderGain->setMaximum(cameraMap[currentCameraId]->maxGain);
    ui->spinBoxGain->setMinimum(cameraMap[currentCameraId]->minGain);
    ui->spinBoxGain->setMaximum(cameraMap[currentCameraId]->maxGain);
    disconnect(ui->SliderExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    disconnect(ui->spinBoxExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    disconnect(ui->SliderGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));
    disconnect(ui->spinBoxGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));
    ui->SliderExposure->setValue(cameraMap[currentCameraId]->exposure);
    ui->spinBoxExposure->setValue(cameraMap[currentCameraId]->exposure);
    ui->SliderGain->setValue(cameraMap[currentCameraId]->gain);
    ui->spinBoxGain->setValue(cameraMap[currentCameraId]->gain);
    connect(ui->SliderExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    connect(ui->spinBoxExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    connect(ui->SliderGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));
    connect(ui->spinBoxGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));
}



void CameraCalibrate::refreshExPosure(int value)
{
    disconnect(ui->SliderExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    disconnect(ui->spinBoxExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    ui->SliderExposure->setValue(value);
    ui->spinBoxExposure->setValue(value);
    connect(ui->SliderExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    connect(ui->spinBoxExposure,SIGNAL(valueChanged(int)),this,SLOT(refreshExPosure(int)));
    //update zhugz
    captureCamera[cur_cameraIndex]->setExpourseValue(value*1000);

    //end
}


void CameraCalibrate::refreshGain(int value)
{
    disconnect(ui->SliderGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));
    disconnect(ui->spinBoxGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));
    ui->SliderGain->setValue(value);
    ui->spinBoxGain->setValue(value);
    connect(ui->SliderGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));
    connect(ui->spinBoxGain,SIGNAL(valueChanged(int)),this,SLOT(refreshGain(int)));

}

void CameraCalibrate::autoAdjustExposureSlot()
{
    //相机2检测图片后调整4次曝光振挺第一条线使用
    static int adjust4Ex=1;
    if(adjust4Ex>4)
    {
        adjust4Ex=1;
        isAjdust=false;
    }

//update zhugz auto exposure
    for(int i=0;i<autoExposure.size();i++)
    {

        if(ui->checkBox->isChecked()&&captureCamera[i]->runFlag)
        {

            //相机2检测图片后调整4次曝光振挺第一条线使用
            if(version==ZT0&&i==1&&isAjdust==false)
            {

                continue;
            }


            //取亮度值
            CameraTemplate *cameraTemp=autoExposure[i];

            mylock[i].lock();
            cvResetImageROI(IpImgCache[i]);
            cvSetImageROI(IpImgCache[i],cvRect(cameraTemp->leftUpX,cameraTemp->leftUpY,cameraTemp->rightDownX-cameraTemp->leftUpX,cameraTemp->rightDownY-cameraTemp->leftUpY));

            CvScalar scalar = cvAvg(IpImgCache[i]);
            cvResetImageROI(IpImgCache[i]);

            mylock[i].unlock();
            try{

                int bright=cameraTemp->bright;
                double exposure=20000;
                if(captureCamera[i]->runFlag)
                    exposure= captureCamera[i]->getExpourseValue();

                double d= scalar.val[0]/255*100;
                double per;
                if(d>bright)
                    per=1/(d/bright);
                else
                    per=bright/d;

                if(i==cur_cameraIndex)
                {
                qDebug()<<d<<d-bright<<exposure;

                }
                if(qAbs(d-bright)>2)
                {


                    if(d-bright>2)
                    {
                        if(per>1.3||per<0.80)
                            exposure*=per;
                        else
                            exposure*=0.95;

                        if(exposure<10000)
                            exposure=10000;
                        if(i==cur_cameraIndex)
                        {
                            ui->SliderExposure->setValue(exposure/1000);
                            ui->spinBoxExposure->setValue(exposure/1000);
                        }
                        else
                        {
                        if(captureCamera[i]->runFlag)
                            captureCamera[i]->setExpourseValue(exposure);
                        }


                    }else if(d-bright<-2)
                    {
                        if(per>1.3||per<0.80)
                            exposure*=per;
                        else
                            exposure*=1.05;
                        if(exposure>100000)
                            exposure=100000;

                        if(i==cur_cameraIndex)
                        {
                            ui->SliderExposure->setValue(exposure/1000);
                            ui->spinBoxExposure->setValue(exposure/1000);
                        }
                        else
                        {
                          if(captureCamera[i]->runFlag)
                            captureCamera[i]->setExpourseValue(exposure);

                        }

                    }

                    qDebug()<<"exposure"<<exposure;

                }



            }catch(...){}

            //end
        }

    }
    ++adjust4Ex;
}


//接口函数 定时获取相机传过来界面显示
void CameraCalibrate::getCameraImage()
{

    autoAdjustExposureSlot();


    lock.lock();
    cvResetImageROI(cameraIpImg);
    cvCvtColor(IpImgCache[cur_cameraIndex], cameraIpImg, CV_GRAY2RGB );
    if(showVisionRet[cur_cameraIndex])
    {
        if(mainWindow->visionTime.secsTo(QTime::currentTime())<10)
        cvRectangle( cameraIpImg, cvPoint(showRect[cur_cameraIndex].x-100,showRect[cur_cameraIndex].y-100), cvPoint( showRect[cur_cameraIndex].x+100 , showRect[cur_cameraIndex].y+100 ), Scalar(0,0,255),2 );

    }
    lock.unlock();


    lock.lock();
try{






         //update zhugz
        //显示当前亮度
        cvResetImageROI(cameraIpImg);
         CvScalar scalar = cvAvg(cameraIpImg);
         double d= scalar.val[0]/255*100;
         ui->bright->setText(QString::number(d));

         cvResetImageROI(cameraIpImg);
         CvRect r12 = cvRect(qAbs(cameraMap[currentCameraId]->cameraWidth-cameraMap[currentCameraId]->cameraHeight)/2,0,cameraMap[currentCameraId]->cameraHeight,cameraMap[currentCameraId]->cameraHeight);
         cvSetImageROI(cameraIpImg,r12);
         cvResize(cameraIpImg,largerIpImg2);

         if (mainImage) {
             mainImage->setPixmap(QPixmap::fromImage(*mainImg));
         }


          //end
     if(settingState == AREASETTING && areaShowState == ALLSHOWSTATE){

        cvResetImageROI(cameraIpImg);
        cvResize(cameraIpImg,displyIpImg);
        cvSetImageROI(cameraIpImg,cvRect(mouseX/scaleWidth -ui->largerWidget->width()/(scaleWidth*2)/2,mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2,ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
        cvResize(cameraIpImg,largerIpImg);


    }
    else if(settingState == AREASETTING && areaShowState ==AFFRAMSHOWSTATE){
         cvResetImageROI(cameraIpImg);

         CvRect r1 = cvRect(cameraStartPos.x(),cameraStartPos.y(),cameraEndPos.x()-cameraStartPos.x(),cameraEndPos.y()-cameraStartPos.y());
         cvSetImageROI(cameraIpImg,r1);

         cvResize(cameraIpImg,displyIpImg);

//         cvResize(captureCamera->getZoomImage(cvRect(mouseX/scaleWidth-ui->largerWidget->width()/(scaleWidth*4) +areatopLeft.x(),mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+areatopLeft.y(),ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2))),largerIpImg);
//this->update();
//         cvResetImageROI(largerIpImg);
//         cvResetImageROI(cameraIpImg);
//         CvRect r = cvRect(0,0,100, 100);
//         CvRect r = cvRect(cameraStartPos.x(),cameraStartPos.y(),cameraEndPos.x()-cameraStartPos.x(),cameraEndPos.y()-cameraStartPos.y());
         CvRect r = cvRect(mouseX/scaleWidth-ui->largerWidget->width()/(scaleWidth*4) +areatopLeft.x(),mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+areatopLeft.y(),ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2));
         cvSetImageROI(cameraIpImg,r);//cvRect(mouseX/scaleWidth-ui->largerWidget->width()/(scaleWidth*4) +areatopLeft.x(),mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+areatopLeft.y(),ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
         cvResize(cameraIpImg,largerIpImg);
//         cvCopy(cameraIpImg, largerIpImg);



     }else if(settingState == AREASETTING && areaShowState ==BESTSHOWSTATE){
         cvResetImageROI(cameraIpImg);
         cvSetImageROI(cameraIpImg,cvRect(0,0,bestWidth,bestHeight));
         cvResize(cameraIpImg,displyIpImg);
         cvSetImageROI(cameraIpImg,cvRect(0,0,ui->largerWidget->width(),ui->largerWidget->height()));
         cvResize(cameraIpImg,largerIpImg);

     }

     else if(settingState == TMPSETTING){
         cvSetImageROI(cameraIpImg,cvRect(cameraMap[currentCameraId]->arealeftUpX,cameraMap[currentCameraId]->arealeftUpY,cameraMap[currentCameraId]->arearightDownX-cameraMap[currentCameraId]->arealeftUpX,cameraMap[currentCameraId]->arearightDownY - cameraMap[currentCameraId]->arealeftUpY));
         cvResize(cameraIpImg,displyIpImg);
         cvSetImageROI(cameraIpImg,cvRect(mouseX/scaleWidth -ui->largerWidget->width()/(scaleWidth*4) + cameraMap[currentCameraId]->arealeftUpX,mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+cameraMap[currentCameraId]->arealeftUpY,ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
         cvResize(cameraIpImg,largerIpImg);
    }
     //<<<
     else if(settingState == MOUSESETTING){
         cvSetImageROI(cameraIpImg,cvRect(cameraMap[currentCameraId]->arealeftUpX,cameraMap[currentCameraId]->arealeftUpY,cameraMap[currentCameraId]->arearightDownX-cameraMap[currentCameraId]->arealeftUpX,cameraMap[currentCameraId]->arearightDownY - cameraMap[currentCameraId]->arealeftUpY));
         cvResize(cameraIpImg,displyIpImg);
         cvSetImageROI(cameraIpImg,cvRect(mouseX/scaleWidth -ui->largerWidget->width()/(scaleWidth*4) + cameraMap[currentCameraId]->arealeftUpX,mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+cameraMap[currentCameraId]->arealeftUpY,ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
         cvResize(cameraIpImg,largerIpImg);

    }
     else if(settingState == CENTERSETTING){
          cvSetImageROI(cameraIpImg,cvRect(cameraMap[currentCameraId]->arealeftUpX,cameraMap[currentCameraId]->arealeftUpY,cameraMap[currentCameraId]->arearightDownX-cameraMap[currentCameraId]->arealeftUpX,cameraMap[currentCameraId]->arearightDownY - cameraMap[currentCameraId]->arealeftUpY));
          cvResize(cameraIpImg,displyIpImg);
          cvSetImageROI(cameraIpImg,cvRect(rightUpPos.x()/2/scaleWidth -ui->largerWidget->width()/(scaleWidth*2)/2+cameraMap[currentCameraId]->arealeftUpX,(ui->fullWidget->height()/2- rightUpPos.y())/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+cameraMap[currentCameraId]->arealeftUpY,ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
          cvResize(cameraIpImg,largerIpImg);
     }
} catch(...) {}

     lock.unlock();
     this->update();



}

void CameraCalibrate::largerImgMove()
{
    if(currentCameraId == "")
        return;


    lock.lock();
    try{
        if(settingState == AREASETTING && areaShowState == ALLSHOWSTATE){

           cvSetImageROI(cameraIpImg,cvRect(mouseX/scaleWidth -ui->largerWidget->width()/(scaleWidth*2)/2,mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2,ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
           cvResize(cameraIpImg,largerIpImg);

       }
       else if(settingState == AREASETTING && areaShowState ==AFFRAMSHOWSTATE){

            cvSetImageROI(cameraIpImg,cvRect(mouseX/scaleWidth-ui->largerWidget->width()/(scaleWidth*4) +areatopLeft.x(),mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+areatopLeft.y(),ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
            cvResize(cameraIpImg,largerIpImg);

        }
        else if(settingState == TMPSETTING||settingState==CENTERSETTING||settingState==MOUSESETTING){

            cvSetImageROI(cameraIpImg,cvRect(mouseX/scaleWidth -ui->largerWidget->width()/(scaleWidth*4) + cameraMap[currentCameraId]->arealeftUpX,mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+cameraMap[currentCameraId]->arealeftUpY,ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
            cvResize(cameraIpImg,largerIpImg);
        }
        //<<<
        //    else if(settingState == MOUSESETTING){

        //        cvSetImageROI(cameraIpImg,cvRect(mouseX/scaleWidth -ui->largerWidget->width()/(scaleWidth*4) + cameraMap[currentCameraId]->arealeftUpX,mouseY/scaleHeight -ui->largerWidget->height()/(scaleHeight*2)/2+cameraMap[currentCameraId]->arealeftUpY,ui->largerWidget->width()/(scaleWidth*2),ui->largerWidget->height()/(scaleHeight*2)));
        //        cvResize(cameraIpImg,largerIpImg);
        //        IplImgToQImg(displyIpImg,displyImg);
        //         IplImgToQImg(largerIpImg,largerImg);
        //    }
    }catch(...) {
    }

    lock.unlock();
    this->update();
}


void CameraCalibrate::setStateChanged(int index)
{
    if(captureCamera[ui->comboBoxCamera->currentIndex()]->runFlag==false)
        return;
    if(0==index){
        settingState = AREASETTING;

    }
    else if(1 == index){
        settingState = TMPSETTING;


    }else if(2 == index){
        settingState = CENTERSETTING;

    }else if(3== index){
            settingState=MOUSESETTING;
        }
    if(areaShowState == BESTSHOWSTATE)
       on_ButtonBestShow_clicked();
    else
        getScaleValue();
    displyImg->fill(Qt::black);

    getCameraImage();

 }







void CameraCalibrate::paintEvent(QPaintEvent *)
{


    if(currentCameraId == "")
        return;
//    if(captureCamera[ui->comboBoxCamera->currentIndex()]->runFlag==false)
//        return;

    if(ui->fullWidget->width()!= screenWidth || ui->fullWidget->height() != screenHeight){
        screenWidth = ui->fullWidget->width();
        screenHeight = ui->fullWidget->height();

        initIpImage();
    }

    QPainter painter(this);
    QPen pen(Qt::green,2,Qt::DotLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawImage(QRect(ui->fullWidget->pos(),ui->fullWidget->size()),*displyImg);

    painter.drawImage(QRect(ui->largerWidget->pos(),ui->largerWidget->size()), *largerImg);

    QPoint tempPos = ui->fullWidget->mapFromGlobal( this->cursor().pos() );
    if (tempPos.x()<0) { bigMouseX=-1; }
    if (tempPos.y()<0) { bigMouseY=-1; }
    if (tempPos.x()>ui->fullWidget->width() - 9) { bigMouseX=-1; }
    if (tempPos.y()>ui->fullWidget->height() - 20) { bigMouseY=-1; }

    if((bigMouseX>0 && bigMouseX<ui->largerWidget->width() && bigMouseY>0 &&bigMouseY<ui->largerWidget->height()) &&  settingState!=CENTERSETTING && areaShowState != BESTSHOWSTATE)
        painter.drawPixmap(bigMouseX+ui->largerWidget->x(),bigMouseY+ui->largerWidget->y(),pixMouse);
//qDebug("bigMouseY: %d, mouseY: %d", mouseY);
   if(settingState == AREASETTING && areaShowState == ALLSHOWSTATE){

       QPoint topLeft = QPoint(ui->fullWidget->x()+cameraStartPos.x()*scaleWidth,ui->fullWidget->y()+cameraStartPos.y()*scaleHeight+ rightUpPos.y());
        QPoint bottomRight = QPoint(ui->fullWidget->x()+cameraEndPos.x()*scaleWidth,ui->fullWidget->y()+cameraEndPos.y()*scaleHeight+ rightUpPos.y());
        painter.drawRect(QRect(topLeft,bottomRight));
        QPoint bigTopLeft = QPoint(areatopLeft.x()*scaleWidth*2,areatopLeft.y()*scaleHeight*2);
        QPoint bigBottomRight = QPoint(areaBottomRight.x()*scaleWidth*2,areaBottomRight.y()*scaleHeight*2);
        bigTopLeft.setX(ui->largerWidget->width()/2-(mouseX*2-bigTopLeft.x()) +ui->largerWidget->x());
        bigTopLeft.setY(ui->largerWidget->height()/2-((mouseY)*2-bigTopLeft.y()) +ui->largerWidget->y());
        bigBottomRight.setX(ui->largerWidget->width()/2-(mouseX*2-bigBottomRight.x())+ui->largerWidget->x());
        bigBottomRight.setY(ui->largerWidget->height()/2-(mouseY*2-bigBottomRight.y()) +ui->largerWidget->y());
        if(bigTopLeft.x()<ui->largerWidget->x())
            bigTopLeft.setX(ui->largerWidget->x());
        else if(bigTopLeft.x()>ui->largerWidget->width())
            bigTopLeft.setX(ui->largerWidget->width() + ui->largerWidget->x());
        if(bigBottomRight.x()<ui->largerWidget->x())
            bigBottomRight.setX(ui->largerWidget->x());
         else if(bigBottomRight.x()>ui->largerWidget->width())
            bigBottomRight.setX(ui->largerWidget->width() + ui->largerWidget->x());
        if(bigTopLeft.y()<ui->largerWidget->y())
            bigTopLeft.setY(ui->largerWidget->y());
        else if(bigTopLeft.y()>ui->largerWidget->height())
            bigTopLeft.setY(ui->largerWidget->height() +ui->largerWidget->y());
        if(bigBottomRight.y()<ui->largerWidget->y())
            bigBottomRight.setY(ui->largerWidget->y());
          else if(bigBottomRight.y()>ui->largerWidget->height())
            bigBottomRight.setY(ui->largerWidget->height()+ui->largerWidget->y());
           painter.drawRect(QRect(bigTopLeft,bigBottomRight));



    }
    else if(settingState == AREASETTING && areaShowState == AFFRAMSHOWSTATE)
    {

    }
    else if(settingState == TMPSETTING){

        painter.drawRect(ui->fullWidget->x() + tpLeftUpX*scaleWidth,ui->fullWidget->y()+ rightUpPos.y()+ tpLeftUpY*scaleHeight, (tpRightDownX-tpLeftUpX)*scaleWidth, (tpRightDownY-tpLeftUpY)*scaleHeight);
        QPoint bigTopLeft = QPoint(tpLeftUpX*scaleWidth*2,tpLeftUpY*scaleHeight*2);
        QPoint bigBottomRight = QPoint(tpRightDownX*scaleWidth*2,tpRightDownY*scaleHeight*2);
        bigTopLeft.setX(ui->largerWidget->width()/2-(mouseX*2-bigTopLeft.x()) +ui->largerWidget->x());
        bigTopLeft.setY(ui->largerWidget->height()/2-(mouseY*2-bigTopLeft.y()) +ui->largerWidget->y());
        bigBottomRight.setX(ui->largerWidget->width()/2-(mouseX*2-bigBottomRight.x())+ui->largerWidget->x());
        bigBottomRight.setY(ui->largerWidget->height()/2-(mouseY*2-bigBottomRight.y()) +ui->largerWidget->y());
        if(bigTopLeft.x()<ui->largerWidget->x())
            bigTopLeft.setX(ui->largerWidget->x());
        else if(bigTopLeft.x()>ui->largerWidget->width())
            bigTopLeft.setX(ui->largerWidget->width() + ui->largerWidget->x());
        if(bigBottomRight.x()<ui->largerWidget->x())
            bigBottomRight.setX(ui->largerWidget->x());
        else if(bigBottomRight.x()>ui->largerWidget->width())
            bigBottomRight.setX(ui->largerWidget->width() + ui->largerWidget->x());
        if(bigTopLeft.y()<ui->largerWidget->y())
            bigTopLeft.setY(ui->largerWidget->y());
        else if(bigTopLeft.y()>ui->largerWidget->height())
            bigTopLeft.setY(ui->largerWidget->height() +ui->largerWidget->y());
        if(bigBottomRight.y()<ui->largerWidget->y())
            bigBottomRight.setY(ui->largerWidget->y());
        else if(bigBottomRight.y()>ui->largerWidget->height())
            bigBottomRight.setY(ui->largerWidget->height()+ui->largerWidget->y());
        painter.drawRect(QRect(bigTopLeft,bigBottomRight));


    }
    else if(settingState == CENTERSETTING){
        painter.drawLine(QPointF(ui->fullWidget->x()+rightUpPos.x()/2-20 ,ui->fullWidget->height()/2),QPointF(ui->fullWidget->x()+rightUpPos.x()/2+20,ui->fullWidget->height()/2));
        painter.drawLine(QPointF(ui->fullWidget->x()+rightUpPos.x()/2 ,ui->fullWidget->height()/2-20),QPointF(ui->fullWidget->x()+rightUpPos.x()/2,ui->fullWidget->height()/2+20));
        painter.drawRect((rightUpPos.x()-tpWidth)/2+ui->fullWidget->x(),(ui->fullWidget->height()-tpHeight)/2,tpWidth,tpHeight);
        int bigWidth = tpWidth*2;
        int bigHeight = tpHeight*2;
        QPoint bigTopLeft =QPoint((ui->largerWidget->width()-bigWidth)/2+ui->largerWidget->x() ,(ui->largerWidget->height()-bigHeight)/2+ui->largerWidget->y());
        QPoint bigBottomRight = QPoint((ui->largerWidget->width()-bigWidth)/2+bigWidth +ui->largerWidget->x(),(ui->largerWidget->height()-bigHeight)/2+bigHeight+ui->largerWidget->y());
        if(bigTopLeft.x()<ui->largerWidget->x())
            bigTopLeft.setX(ui->largerWidget->x());
        if(bigTopLeft.y()<ui->largerWidget->y())
            bigTopLeft.setY(ui->largerWidget->y());
        if(bigBottomRight.x()>ui->largerWidget->x()+ui->largerWidget->width())
            bigBottomRight.setX(ui->largerWidget->x()+ui->largerWidget->width());
        if(bigBottomRight.y()>ui->largerWidget->y()+ui->largerWidget->height())
            bigBottomRight.setY(ui->largerWidget->y()+ui->largerWidget->height());
        painter.drawLine(QPoint(ui->largerWidget->x()+ ui->largerWidget->width()/2-40 ,ui->fullWidget->height()/2 +ui->largerWidget->y()),QPointF(ui->largerWidget->x()+ ui->largerWidget->width()/2+40,ui->fullWidget->height()/2 +ui->largerWidget->y()));
        painter.drawLine(QPoint(ui->largerWidget->x()+ ui->largerWidget->width()/2 ,ui->fullWidget->height()/2 +ui->largerWidget->y()-40),QPointF(ui->largerWidget->x()+ ui->largerWidget->width()/2,ui->fullWidget->height()/2 +ui->largerWidget->y()+40));
        painter.drawRect(QRect(bigTopLeft,bigBottomRight));

   }
   //<<<
   /*
cameraEndPos.y() //图片
rightUpPos.y() //pic offsetY
ui->fullWidget->height()  fullScreen height
scaleWidth // 比例
*/
   else if(settingState==MOUSESETTING)
   {
       int offsetX=ui->largerWidget->width()+12,offsetY=7;

       painter.drawLine(QPoint(cMouseX-20+offsetX,cMouseY+offsetY),QPoint(cMouseX+20+offsetX,cMouseY+offsetY));
       painter.drawLine(QPoint(cMouseX+offsetX,cMouseY-20+offsetY),QPoint(cMouseX+offsetX,cMouseY+20+offsetY));


   }

}






bool  CameraCalibrate::eventFilter(QObject *obj, QEvent *event)
{
    if(ui->comboBoxCamera->currentIndex()==-1||captureCamera[ui->comboBoxCamera->currentIndex()]==NULL)
    {
        return true;
    }

    if(captureCamera[ui->comboBoxCamera->currentIndex()]->runFlag==false)
        return true;


    static QPoint lastAreaStratPos, lastAreaEndPos, lastCameraStartPos,lastCameraEndPos;
    if(obj != ui->fullWidget){
       return false;
    }
    QMouseEvent *keyEvent = static_cast<QMouseEvent*>(event);
    if((event->type() == QEvent::MouseMove && !isButtonPressd ) ) {
        mouseX = keyEvent->x();
        mouseY = keyEvent->y()-rightUpPos.y();
        int areaWidth,areaHeight;
        if(settingState==MOUSESETTING)
        {
        cMouseX=keyEvent->x();
        cMouseY=keyEvent->y();
        }
        if(settingState == AREASETTING && areaShowState == ALLSHOWSTATE){
            areaWidth =cameraMap[currentCameraId]->cameraWidth;
            areaHeight = cameraMap[currentCameraId]->cameraHeight;

        }
        else{

            areaWidth = abs(cameraStartPos.x()-cameraEndPos.x());
            areaHeight = abs(cameraStartPos.y()-cameraEndPos.y());


        }
        if(mouseX/scaleWidth <ui->largerWidget->width()/(scaleWidth*4)){
            bigMouseX = 2*mouseX;
            mouseX =ui->largerWidget->width()/4;
        }else if(mouseX/scaleWidth + ui->largerWidget->width()/(scaleWidth*4) >areaWidth){
            bigMouseX = ui->largerWidget->width() - 2*(rightUpPos.x()-mouseX);
            mouseX = areaWidth*scaleWidth - ui->largerWidget->width()/4;
        }
        else {
             bigMouseX = ui->largerWidget->width()/2;
        }

        if(mouseY/scaleHeight <ui->largerWidget->height()/(scaleHeight*4) ){
            bigMouseY = 2*mouseY;
            mouseY = ui->largerWidget->height()/4;
        }
        else if(( mouseY)/scaleHeight + ui->largerWidget->height()/(scaleHeight*4) > areaHeight){
            bigMouseY = ui->largerWidget->height() -2*(ui->fullWidget->height()-2*rightUpPos.y()-mouseY);
            mouseY = areaHeight*scaleWidth -ui->largerWidget->height()/4;
        }
        else{
            bigMouseY = ui->largerWidget->height()/2;
        }

        lastMouseX = mouseX;
        lastMouseY = mouseY;

//        qDebug("%d %d %d %d", bigMouseX, mouseX, bigMouseY, mouseY);
        largerImgMove();

        return true;
     }
    if(event->type() == QEvent::MouseButtonPress  )
    {
        if(keyEvent->x()>rightUpPos.x() || keyEvent->y()<rightUpPos.y() || keyEvent->y()>ui->fullWidget->height()-rightUpPos.y())
            return false;
        if(settingState == AREASETTING && areaShowState== ALLSHOWSTATE){
               isButtonPressd = true;
            lastAreaStratPos.setX(areaStartPos.x());
            lastAreaStratPos.setY(areaStartPos.y());
            lastAreaEndPos.setX(areaEndPos.x());
            lastAreaEndPos.setY(areaEndPos.y());
            lastCameraStartPos.setX(cameraStartPos.x());
            lastCameraStartPos.setY(cameraStartPos.y());
            lastCameraEndPos.setX(cameraEndPos.x());
            lastCameraEndPos.setY(cameraEndPos.y());
            areaStartPos.setX(keyEvent->x());
            areaStartPos.setY(keyEvent->y()-rightUpPos.y());
            areaEndPos.setX(keyEvent->x());
            areaEndPos.setY(keyEvent->y()-rightUpPos.y());
            cameraStartPos.setX(areaStartPos.x()/scaleWidth);
            cameraStartPos.setY(areaStartPos.y()/scaleHeight);
            cameraEndPos.setX(areaEndPos.x()/scaleWidth);
            cameraEndPos.setY(areaEndPos.y()/scaleHeight);
            return true;
        }
        else if(settingState == TMPSETTING){
            if(selectTmpId == "")
                return false;
             isButtonPressd = true;
            tmpStartPos.setX(keyEvent->x());
            tmpStartPos.setY(keyEvent->y()-rightUpPos.y());

            disconnect(ui->tableWidgetTmp,SIGNAL(cellChanged(int,int)),this,SLOT(refreshTemplate(int,int)));
            disconnect(spinLeftUpX,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateLeftUpX(int)));
            disconnect(spinLeftUpY,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateLeftUpY(int)));
            disconnect(spinRightDownX,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateRightDownX(int)));
            disconnect(spinRightDownY,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateRightDownY(int)));
            return true;
        }
        else if(settingState==MOUSESETTING)
        {
            ui->label_15->setText(QString("x:%1  y:%2").arg(int(cMouseX/scaleWidth)).arg(int((cMouseY-rightUpPos.y())/scaleHeight)));
            return true;
        }
      }
    if((event->type() == QEvent::MouseMove ) && isButtonPressd)
    {

        if(keyEvent->x()>rightUpPos.x() || keyEvent->y()<rightUpPos.y() || keyEvent->y()>ui->fullWidget->height()-rightUpPos.y() ||keyEvent->x()<0)
            return false;
        mouseX = keyEvent->x();
        mouseY = keyEvent->y()-rightUpPos.y();
        if(settingState == AREASETTING){
            areaEndPos.setX(mouseX);
            areaEndPos.setY(mouseY);
            cameraEndPos.setX(areaEndPos.x()/scaleWidth);
            cameraEndPos.setY(areaEndPos.y()/scaleHeight);
            updateAraePosValue();

           // return true;
        }else if(settingState == TMPSETTING){
            if(selectTmpId == "")
                return false;

            tmpEndPos.setX(mouseX);
            tmpEndPos.setY(mouseY);
            tpLeftUpX = (tmpEndPos.x()>tmpStartPos.x())? (tmpStartPos.x()/scaleWidth):(tmpEndPos.x()/scaleWidth);
            tpRightDownX = (tmpEndPos.x()<tmpStartPos.x())? (tmpStartPos.x()/scaleWidth):(tmpEndPos.x()/scaleWidth);
            tpLeftUpY =(tmpEndPos.y()>tmpStartPos.y())? (tmpStartPos.y()/scaleHeight):(tmpEndPos.y()/scaleHeight);
            tpRightDownY =(tmpEndPos.y()<tmpStartPos.y())? (tmpStartPos.y()/scaleHeight):(tmpEndPos.y()/scaleHeight);
            spinLeftUpX->setValue(tpLeftUpX);
            spinLeftUpY->setValue(tpLeftUpY);
            spinRightDownX->setValue(tpRightDownX);
            spinRightDownY->setValue(tpRightDownY);
            // return true;
        }
        int areaWidth,areaHeight;
        if(settingState == AREASETTING && areaShowState == ALLSHOWSTATE){
            areaWidth =cameraMap[currentCameraId]->cameraWidth;
            areaHeight = cameraMap[currentCameraId]->cameraHeight;
        }
        else{
            areaWidth = abs(cameraStartPos.x()-cameraEndPos.x());
            areaHeight = abs(cameraStartPos.y()-cameraEndPos.y());
        }

        if(mouseX/scaleWidth <ui->largerWidget->width()/(scaleWidth*4)){
            bigMouseX = 2*mouseX;
            //mouseX = lastMouseX;
            mouseX =ui->largerWidget->width()/4;
        }else if(mouseX/scaleWidth + ui->largerWidget->width()/(scaleWidth*4) >areaWidth){
            bigMouseX = ui->largerWidget->width() - 2*(rightUpPos.x()-mouseX);
            mouseX = areaWidth*scaleWidth - ui->largerWidget->width()/4;
            //mouseX =lastMouseX;
        }
        else {
             bigMouseX = ui->largerWidget->width()/2;
        }
        if(mouseY/scaleHeight <ui->largerWidget->height()/(scaleHeight*4) ){
            bigMouseY = 2*mouseY;
            mouseY = ui->largerWidget->height()/4;

        }
        else if(( mouseY)/scaleHeight + ui->largerWidget->height()/(scaleHeight*4) > areaHeight){
            bigMouseY = ui->largerWidget->height() -2*(ui->fullWidget->height()-2*rightUpPos.y()-mouseY);
            mouseY = areaHeight*scaleWidth -ui->largerWidget->height()/4;
        }
        else{
            bigMouseY = ui->largerWidget->height()/2;
        }
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        largerImgMove();
    }
    else if(event->type() == QEvent::MouseButtonRelease && settingState == AREASETTING && areaShowState==ALLSHOWSTATE) {
        isButtonPressd = false;
        if(areaEndPos.x() == areaStartPos.x() || areaEndPos.y() == areaStartPos.y())
        {
            areaStartPos.setX(lastAreaStratPos.x());
            areaStartPos.setY(lastAreaStratPos.y());
            areaEndPos.setX(lastAreaEndPos.x());
            areaEndPos.setY(lastAreaEndPos.y());
            cameraStartPos.setX(lastCameraStartPos.x());
            cameraStartPos.setY(lastCameraStartPos.y());
            cameraEndPos.setX(lastCameraEndPos.x());
            cameraEndPos.setY(lastCameraEndPos.y());
            return false;
        }else{

        }
        int areaWidth = abs(areaEndPos.x()-areaStartPos.x());
        int areaHeight  = abs(areaEndPos.y() - areaStartPos.y());
        areaScaleWidth = (float)areaWidth/ui->fullWidget->width();
        areaScaleHeight = (float)areaHeight/ui->fullWidget->height();
        if(areaScaleHeight > areaScaleWidth){
            int realWidth = ui->fullWidget->width()*areaScaleHeight;
            int diff = (realWidth - abs(areaEndPos.x()-areaStartPos.x()))/2;
            if(areaEndPos.x()-areaStartPos.x()>0){
                int diffRight = rightUpPos.x()- areaEndPos.x()-diff;
                int diffLeft  = areaStartPos.x() - diff;
                if(diffRight>=0 && diffLeft>=0){
                    areaEndPos.setX(areaEndPos.x()+diff);
                    areaStartPos.setX(areaStartPos.x()-diff);
                }else if(diffRight<0){
                    areaEndPos.setX(rightUpPos.x());
                    areaStartPos.setX(areaStartPos.x()-diff+diffRight);
                }
                else if(diffLeft<0){
                    areaStartPos.setX(0);
                    areaEndPos.setX(areaEndPos.x()+diff-diffLeft);
                }

            }else{
                int diffLeft = areaEndPos.x()-diff;
                int diffRight = areaStartPos.x()+diff;
                if(diffRight>=0 && diffLeft>=0){
                    areaEndPos.setX(areaEndPos.x()-diff);
                    areaStartPos.setX(areaStartPos.x()+diff);
                }else if(diffRight<0){
                    areaStartPos.setX(rightUpPos.x());
                    areaEndPos.setX(areaEndPos.x()-diff+diffRight);
                }
                else if(diffLeft<0){
                    areaEndPos.setX(0);
                    areaStartPos.setX(areaStartPos.x()+diff-diffLeft);
                }
            }

        }
        else{
            int realHeight = ui->fullWidget->height()*areaScaleWidth;
            int diff = (realHeight - abs(areaEndPos.y()-areaStartPos.y()))/2;
            if(areaEndPos.y()-areaStartPos.y()>0){
                int diffUp = areaStartPos.y()- diff;
                int diffDown = (ui->fullWidget->height()-2*rightUpPos.y())- areaEndPos.y()-diff;

                if(diffUp>=0 && diffDown>=0){
                    areaEndPos.setY(areaEndPos.y()+diff);
                    areaStartPos.setY(areaStartPos.y()-diff);
                }
                else if(diffUp + diffDown <0){
                    areaStartPos.setX(0);
                    areaStartPos.setY(0);
                    areaEndPos.setX(rightUpPos.x());
                    areaEndPos.setY(ui->fullWidget->height()-2*rightUpPos.y());
                }
                else if(diffUp<0){
                    areaStartPos.setY(0);
                    areaEndPos.setY(areaEndPos.y()+diff - diffUp);
                }else if(diffDown < 0){

                    areaEndPos.setY(ui->fullWidget->height()-2*rightUpPos.y());
                    areaStartPos.setY(areaStartPos.y()-diff +diffDown);
                }

            }else{

                int diffUp = areaEndPos.y()- diff;
                int diffDown = ui->fullWidget->height()-2*rightUpPos.y()-areaStartPos.y()-diff;
                if(diffUp>=0 && diffDown>=0){
                    areaEndPos.setY(areaEndPos.y()-diff);
                    areaStartPos.setY(areaStartPos.y()+diff);
                }
                else if(diffUp + diffDown <0){
                    areaStartPos.setX(0);
                    areaStartPos.setY(0);
                    areaEndPos.setX(rightUpPos.x());
                    areaEndPos.setY(ui->fullWidget->height()-2*rightUpPos.y());
                }
                else if(diffUp<0){
                    areaEndPos.setY(0);
                    areaStartPos.setY(areaStartPos.y()+diff - diffUp);
                }else if(diffDown < 0){

                    areaEndPos.setY(areaEndPos.y()-diff + diffDown);
                    areaStartPos.setY(ui->fullWidget->height()-2*rightUpPos.y());
                }



            }

        }
        cameraStartPos.setX(areaStartPos.x()/scaleWidth);
        cameraStartPos.setY(areaStartPos.y()/scaleHeight);
        cameraEndPos.setX(areaEndPos.x()/scaleWidth);
        cameraEndPos.setY(areaEndPos.y()/scaleHeight);

        updateAraePosValue();
        getCameraImage();
        lastAreaStratPos.setX(areaStartPos.x());
        lastAreaStratPos.setY(areaStartPos.y());
        lastAreaEndPos.setX(areaEndPos.x());
        lastAreaEndPos.setY(areaEndPos.y());
        lastCameraStartPos.setX(cameraStartPos.x());
        lastCameraStartPos.setY(cameraStartPos.y());
        lastCameraEndPos.setX(cameraEndPos.x());
        lastCameraEndPos.setY(cameraEndPos.y());
        return true;
    }
    else if(event->type() == QEvent::MouseButtonRelease && settingState == TMPSETTING){

        isButtonPressd = false;
        if(selectTmpId == "")
            return true;
        connect(spinLeftUpX,SIGNAL(valueChanged(int)),SLOT(refreshTemplateLeftUpX(int)));
        connect(spinLeftUpY,SIGNAL(valueChanged(int)),SLOT(refreshTemplateLeftUpY(int)));
        connect(spinRightDownX,SIGNAL(valueChanged(int)),SLOT(refreshTemplateRightDownX(int)));
        connect(spinRightDownY,SIGNAL(valueChanged(int)),SLOT(refreshTemplateRightDownY(int)));
        connect(ui->tableWidgetTmp,SIGNAL(cellChanged(int,int)),this,SLOT(refreshTemplate(int,int)));
        return true;

    }

    else
    {
        return false;
    }


     return  QObject::eventFilter(obj, event);
}


void CameraCalibrate::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseIn)
    {
        isMouseIn =false;
    }

}

void CameraCalibrate::updateAraePosValue()
{

    disconnect(ui->areaSpinBoxUp,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxUp(int)));
    disconnect(ui->areaSpinBoxLeft,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxLeft(int)));
    disconnect(ui->areaSpinBoxRight,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxRight(int)));
    disconnect(ui->areaSpinBoxDown,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxDown(int)));
    int upPos = cameraEndPos.y()> cameraStartPos.y()? cameraStartPos.y():cameraEndPos.y();
    int downPos = cameraEndPos.y()<cameraStartPos.y()? cameraStartPos.y(): cameraEndPos.y();
    int leftPos = cameraEndPos.x()> cameraStartPos.x()? cameraStartPos.x():cameraEndPos.x();
    int rightPos = cameraEndPos.x()< cameraStartPos.x()? cameraStartPos.x(): cameraEndPos.x();
    areatopLeft.setX(leftPos);
    areatopLeft.setY(upPos);
    areaBottomRight.setX(rightPos);
    areaBottomRight.setY(downPos);
    ui->areaSpinBoxUp->setValue(areatopLeft.y());
    ui->areaSpinBoxDown->setValue(areaBottomRight.y());
    ui->areaSpinBoxLeft->setValue(areatopLeft.x());
    ui->areaSpinBoxRight->setValue(areaBottomRight.x());
    connect(ui->areaSpinBoxUp,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxUp(int)));
    connect(ui->areaSpinBoxLeft,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxLeft(int)));
    connect(ui->areaSpinBoxRight,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxRight(int)));
    connect(ui->areaSpinBoxDown,SIGNAL(valueChanged(int)),this,SLOT(slotAreaSpinBoxDown(int)));

}

void CameraCalibrate::on_ButtonSelectAll_clicked()
{
    if(currentCameraId == "")
        return;
    displyImg->fill(Qt::black);
    areaShowState = ALLSHOWSTATE;
    getScaleValue();
    areaStartPos = QPoint(0,0);
    areaEndPos = QPoint(rightUpPos.x(),ui->fullWidget->height()-2*rightUpPos.y());
    cameraStartPos = QPoint(0,0);
    cameraEndPos = QPoint(cameraMap[currentCameraId]->cameraWidth,cameraMap[ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString()]->cameraHeight);
    updateAraePosValue();


}

void CameraCalibrate::on_ButtonSave_clicked()
{
    if(currentCameraId == "")
        return;
    if(0 != QMessageBox::question(NULL,"系统信息","是否确认保存",QMessageBox::tr("确认"),QMessageBox::tr("取消")))
        return;
    QString id = ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString();
    cameraMap[id]->arealeftUpX = ui->areaSpinBoxLeft->value();
    cameraMap[id]->arealeftUpY = ui->areaSpinBoxUp->value();
    cameraMap[id]->arearightDownX = ui->areaSpinBoxRight->value();
    cameraMap[id]->arearightDownY = ui->areaSpinBoxDown->value();
    QSqlQuery query;
    if(!query.exec(QString("UPDATE CameraProperty SET areaLeftUpX=%1, areaLeftUpY=%2, areaRightDownX=%3,areaRightDownY=%4 WHERE id='%5'").arg(QString::number( cameraMap[id]->arealeftUpX),QString::number(cameraMap[id]->arealeftUpY),QString::number(cameraMap[id]->arearightDownX),QString::number(cameraMap[id]->arearightDownY),cameraMap[id]->id)))
        qDebug()<<"falied";
}


void CameraCalibrate::on_ButtonBestShow_clicked()
{
    if(currentCameraId == "")
        return;

    int width,height;
    int cameraWidth = cameraMap[currentCameraId]->cameraWidth/2;
    int cameraHeight = cameraMap[currentCameraId]->cameraHeight/2;
    if(cameraWidth>=ui->fullWidget->width()){
         bestWidth =ui->fullWidget->width()*2;
         width = ui->fullWidget->width();
    }
    else{
        bestWidth =cameraWidth*2;
        width = cameraWidth;
    }
    if(cameraHeight>=ui->fullWidget->height()){
         bestHeight = ui->fullWidget->height()*2;
         height = ui->fullWidget->height();
    }else{
        bestHeight = cameraHeight*2;
        height = cameraHeight;

    }
    //rightUpPos.setY((ui->fullWidget->height()-cameraHeight*scaleHeight)/2);
     cvSetImageROI(displyIpImg,cvRect(0,( ui->fullWidget->height()-height)/2,width,height));
     displyImg->fill(Qt::black);
     areaShowState = BESTSHOWSTATE;

}

void CameraCalibrate::on_ButtonAllShow_clicked()
{
    areaShowState = ALLSHOWSTATE;
    displyImg->fill(Qt::black);
    getScaleValue();
    getCameraImage();

}

void CameraCalibrate::on_ButtonShow_clicked()
{
    areaShowState = AFFRAMSHOWSTATE;
    displyImg->fill(Qt::black);
    getScaleValue();
    getCameraImage();

}


void CameraCalibrate::on_ButtonUp_clicked()
{
    if(currentCameraId == "")
        return;
    if(cameraStartPos.y()-10 < 0 || cameraEndPos.y()-10 <0)
        return;
    cameraStartPos.setY(cameraStartPos.y()-10);
    cameraEndPos.setY(cameraEndPos.y()-10);
    areaStartPos.setY(cameraStartPos.y()*scaleHeight);
    areaEndPos.setY(cameraEndPos.y()*scaleHeight);
    updateAraePosValue();
    if(areaShowState == ALLSHOWSTATE){
        getCameraImage();

    }
    else if(areaShowState == BESTSHOWSTATE){
        on_ButtonBestShow_clicked();
    }
}


void CameraCalibrate::on_ButtonLeft_clicked()
{
    if(currentCameraId=="")
        return;
    if(cameraStartPos.x()-10 <0 || cameraEndPos.x() -10 <0)
        return;
    cameraStartPos.setX(cameraStartPos.x()-10 );
    cameraEndPos.setX(cameraEndPos.x()-10);
    areaStartPos.setX(cameraStartPos.x()*scaleWidth);
    areaEndPos.setX(cameraEndPos.x()*scaleWidth);
    updateAraePosValue();
    if(areaShowState == ALLSHOWSTATE){
        getCameraImage();

    }else if(areaShowState == BESTSHOWSTATE){

        on_ButtonBestShow_clicked();
    }
}

void CameraCalibrate::on_ButtonRight_clicked()
{
    if(currentCameraId == "")
        return;
    if(cameraStartPos.x()+10 > cameraMap[currentCameraId]->cameraWidth || cameraEndPos.x()+10 >cameraMap[currentCameraId]->cameraWidth)
        return;
    cameraStartPos.setX(cameraStartPos.x()+10 );
    cameraEndPos.setX(cameraEndPos.x()+10);
    areaStartPos.setX(cameraStartPos.x()*scaleWidth);
    areaEndPos.setX(cameraEndPos.x()*scaleWidth);
    updateAraePosValue();
    if(areaShowState == ALLSHOWSTATE){
        getCameraImage();

    }else if(areaShowState == BESTSHOWSTATE){

        on_ButtonBestShow_clicked();
    }
}

void CameraCalibrate::on_ButtonDown_clicked()
{
    if(currentCameraId =="")
        return;
    if(cameraStartPos.y()+10 >cameraMap[currentCameraId]->cameraHeight || cameraEndPos.y()+10 > cameraMap[currentCameraId]->cameraHeight)
        return;
    cameraStartPos.setY(cameraStartPos.y()+10);
    cameraEndPos.setY(cameraEndPos.y()+10);
    areaStartPos.setY(cameraStartPos.y()*scaleHeight);
    areaEndPos.setY(cameraEndPos.y()*scaleHeight);
    updateAraePosValue();
    if(areaShowState == ALLSHOWSTATE){
        getCameraImage();
    }
    else if(areaShowState == BESTSHOWSTATE){

        on_ButtonBestShow_clicked();
    }
}


void CameraCalibrate::slotAreaSpinBoxUp(int pos)
{

    int value =0;
    if(pos > ui->areaSpinBoxDown->value()){
        ui->areaSpinBoxUp->setValue(ui->areaSpinBoxDown->value() -1);
        value = ui->areaSpinBoxDown->value()-1;
    }else{
        value = pos;
    }
    if(cameraStartPos.y() <= cameraEndPos.y()){
        cameraStartPos.setY(value);
        areaStartPos.setY(value*scaleHeight);
    }
    else{
        cameraEndPos.setY(value);
        areaEndPos.setY(value*scaleHeight);
    }



}

void CameraCalibrate::slotAreaSpinBoxDown(int pos)
{
    int value =0;
    if(pos <ui->areaSpinBoxDown->value()){
        ui->areaSpinBoxDown->setValue(ui->areaSpinBoxUp->value() +1);
        value = ui->areaSpinBoxUp->value()+1;
    }else{
        value = pos;
    }
    if(cameraStartPos.y() > cameraEndPos.y()){
        cameraStartPos.setY(value);
        areaStartPos.setY(value*scaleHeight);
    }
    else{
        cameraEndPos.setY(value);
        areaEndPos.setY(value*scaleHeight);
    }



}


void CameraCalibrate::slotAreaSpinBoxLeft(int pos)
{
    int value = 0;
    if(pos > ui->areaSpinBoxRight->value()){
        ui->areaSpinBoxLeft->setValue(ui->areaSpinBoxRight->value()-1);
        value = ui->areaSpinBoxRight->value()-1;
    }else{
        value = pos;
    }
    if(cameraStartPos.x() < cameraEndPos.x()){
        cameraStartPos.setX(value);
        areaStartPos.setX(value*scaleWidth);
    }
    else{
        cameraEndPos.setX(value);
        areaEndPos.setX(value*scaleWidth);
    }


}

void CameraCalibrate::slotAreaSpinBoxRight(int pos)
{

    int value = 0;
    if(pos < ui->areaSpinBoxLeft->value()){
        ui->areaSpinBoxRight->setValue(ui->areaSpinBoxLeft->value()+1);
        value = ui->areaSpinBoxLeft->value()+1;
    }else{
        value = pos;
    }
    if(cameraStartPos.x() > cameraEndPos.x()){
        cameraStartPos.setX(value);
        areaStartPos.setX(value*scaleWidth);
    }
    else{
        cameraEndPos.setX(value);
        areaEndPos.setX(value*scaleWidth);
    }

}






void CameraCalibrate::on_ButtonAddTmp_clicked()
{
    if(ui->comboBoxCamera->currentIndex()==-1)
        return;
    ui->tableWidgetTmp->insertRow(ui->tableWidgetTmp->rowCount());
    QString id = SysUtils::uuid();
    QSpinBox *spinLeftUpX = new QSpinBox(this);
    QSpinBox *spinLeftUpY = new QSpinBox(this);
    QSpinBox *spinRightDownX = new QSpinBox(this);
    QSpinBox *spinRightDownY = new QSpinBox(this);
    QSpinBox *spinBoxBright =  new QSpinBox(this);
    spinBoxBright->setReadOnly(true);
    spinLeftUpX ->setReadOnly(true);
    spinLeftUpY->setReadOnly(true);
    spinRightDownX->setReadOnly(true);
    spinRightDownY->setReadOnly(true);
    spinLeftUpX->setMaximum(realAreaImg.width());
    spinLeftUpY->setMaximum(realAreaImg.height());
    spinRightDownX->setMaximum(realAreaImg.width());
    spinRightDownY->setMaximum(realAreaImg.height());
    spinBoxBright->setMaximum(100);
    spinBoxBright->setValue(100);
    ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,0,new QTableWidgetItem());
    ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,1,new QTableWidgetItem(QString("tp%1").arg(QString::number(ui->tableWidgetTmp->rowCount()))));
    ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,2,spinBoxBright);
    ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,3,spinLeftUpX);
    ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,4,spinLeftUpY);
    ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,5,spinRightDownX);
    ui->tableWidgetTmp->setCellWidget(ui->tableWidgetTmp->rowCount()-1,6,spinRightDownY);
    ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,7, new QTableWidgetItem(""));
    ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,8, new QTableWidgetItem(""));
    ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,9, new QTableWidgetItem(""));
    ui->tableWidgetTmp->setItem(ui->tableWidgetTmp->rowCount()-1,10, new QTableWidgetItem(id));
    CameraTemplate *cameraTemplate = new CameraTemplate(this);
    cameraTemplate->type = 0;
    cameraTemplate->id = id;
    cameraTemplate->name = QString("tp%1").arg(QString::number(ui->tableWidgetTmp->rowCount()));
    QString cameraId = ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString();
    int code,realCode;
    for(code =1; code <100; code++){
        realCode = cameraMap[cameraId]->sn*1000+code;
        if(!tpCodeMap.contains(realCode))
            break;
    }
    cameraTemplate->code = realCode;
    ui->tableWidgetTmp->item(ui->tableWidgetTmp->rowCount()-1,0)->setText(QString::number(realCode));
    cameraTemplate->leftUpX = 0;
    cameraTemplate->leftUpY = 0;
    cameraTemplate->rightDownX = 0;
    cameraTemplate->rightDownY = 0;
    cameraTemplate->tmpWidth = 0;
    cameraTemplate->tmpHeight = 0;
    cameraTemplate->bright = 100;
    templateMap.insert(id,cameraTemplate);
    tpCodeMap.insert( cameraTemplate->code,id);
    QSqlQuery query;
    if(!query.exec(QString("INSERT INTO CameraTemplate (id,cameraId,name,code,type,leftUpX,leftUpY,rightDownX,rightDownY,bright) VALUES('%1','%2','%3',%4,0,%5,%6,%7,%8,100)").arg(id,ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString(), cameraTemplate->name,QString::number(cameraTemplate->code),QString::number(cameraTemplate->leftUpX),QString::number(cameraTemplate->leftUpY),QString::number(cameraTemplate->rightDownX),QString::number(cameraTemplate->rightDownY))))
        qDebug()<<QString("INSERT INTO CameraTemplate (id,cameraId,name,code,type,leftUpX,leftUpY,rightDownX,rightDownY,bright) VALUES('%1','%2','%3',%4,0,%5,%6,%7,%8,100)").arg(id,ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString(), cameraTemplate->name,QString::number(cameraTemplate->code),QString::number(cameraTemplate->leftUpX),QString::number(cameraTemplate->leftUpY),QString::number(cameraTemplate->rightDownX),QString::number(cameraTemplate->rightDownY));
}

void CameraCalibrate::on_ButtonDelTmp_clicked()
{
    int row = ui->tableWidgetTmp->currentRow();
    if(row<0)
        return;
    if(0 != QMessageBox::question(NULL,"系统信息","是否确认删除",QMessageBox::tr("是"),QMessageBox::tr("否")))
        return;
    disconnect(ui->tableWidgetTmp,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    QString delId = ui->tableWidgetTmp->item(row,10)->text();
    tpCodeMap.remove(templateMap[delId]->code);
    templateMap.remove(delId);
    ui->tableWidgetTmp->removeRow(row);
    connect(ui->tableWidgetTmp,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    if(ui->tableWidgetTmp->currentRow() == -1)
        selectTmpId ="";
    else
        ui->tableWidgetTmp->selectRow(ui->tableWidgetTmp->currentRow());
    QSqlQuery query;
    query.exec(QString("DELETE FROM CameraTemplate WHERE id='%1'").arg(delId));
    qDebug()<<QString("DELETE FROM CameraTemplate WHERE id='%1'").arg(delId);
}



void CameraCalibrate::selectTemplate()
{

    if(settingState == TMPSETTING){
        if(spinBrightTmp !=NULL){
            spinBrightTmp->setReadOnly(true);
        }

        if(spinLeftUpX != NULL){
            disconnect(spinLeftUpX,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateLeftUpX(int)));
            spinLeftUpX->setReadOnly(true);
        }
        if(spinLeftUpY != NULL){
            disconnect(spinLeftUpY,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateLeftUpY(int)));
            spinLeftUpY->setReadOnly(true);}
        if(spinRightDownX != NULL){
            disconnect(spinRightDownX,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateRightDownX(int)));
            spinRightDownX->setReadOnly(true);
        }
        if(spinRightDownY != NULL){
            disconnect(spinRightDownY,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateRightDownY(int)));
            spinRightDownY->setReadOnly(true);
        }

        selectTmpId = ui->tableWidgetTmp->item(ui->tableWidgetTmp->currentRow(),10)->text();

        ui->tableWidgetTmp->item(ui->tableWidgetTmp->currentRow(),0)->setText(QString::number(templateMap[selectTmpId]->code));
        tpLeftUpX = templateMap[selectTmpId]->leftUpX;
        tpLeftUpY = templateMap[selectTmpId]->leftUpY;
        tpRightDownX = templateMap[selectTmpId]->rightDownX;
        tpRightDownY = templateMap[selectTmpId]->rightDownY;
        spinBrightTmp =(QSpinBox*)ui->tableWidgetTmp->cellWidget(ui->tableWidgetTmp->currentRow(),2);
        spinLeftUpX = (QSpinBox*)ui->tableWidgetTmp->cellWidget(ui->tableWidgetTmp->currentRow(),3);
        spinLeftUpY = (QSpinBox*)ui->tableWidgetTmp->cellWidget(ui->tableWidgetTmp->currentRow(),4);
        spinRightDownX = (QSpinBox*)ui->tableWidgetTmp->cellWidget(ui->tableWidgetTmp->currentRow(),5);
        spinRightDownY = (QSpinBox*)ui->tableWidgetTmp->cellWidget(ui->tableWidgetTmp->currentRow(),6);

        spinLeftUpX->setValue(tpLeftUpX);
        spinLeftUpY->setValue(tpLeftUpY);
        spinRightDownX->setValue(tpRightDownX);
        spinRightDownY->setValue(tpRightDownY);
        spinBrightTmp->setValue(templateMap[selectTmpId]->bright);
        connect(spinLeftUpX,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateLeftUpX(int)));
        connect(spinLeftUpY,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateLeftUpY(int)));
        connect(spinRightDownX,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateRightDownX(int)));
        connect(spinRightDownY,SIGNAL(valueChanged(int)),this,SLOT(refreshTemplateRightDownY(int)));
        spinLeftUpX->setReadOnly(false);
        spinLeftUpY->setReadOnly(false);
        spinRightDownX->setReadOnly(false);
        spinRightDownY->setReadOnly(false);
        spinBrightTmp->setReadOnly(false);
    }
    else if(settingState == CENTERSETTING){
        if(spinBrightCenter !=NULL){
            spinBrightCenter->setReadOnly(true);
        }

        if(spinWidth!= NULL){
            disconnect(spinWidth,SIGNAL(valueChanged(int)),this,SLOT(refreshCenterWidth(int)));
            spinWidth->setReadOnly(true);
        }
        if(spinHeight != NULL){
            disconnect(spinHeight,SIGNAL(valueChanged(int)),this,SLOT(refreshCenterHeight(int)));
            spinHeight->setReadOnly(true);
        }


        selectCenterId = ui->tableWidgetCenter->item(ui->tableWidgetCenter->currentRow(),8)->text();
        ui->tableWidgetCenter->item(ui->tableWidgetCenter->currentRow(),0)->setText(QString::number(templateMap[selectCenterId]->code));
        tpWidth = templateMap[selectCenterId]->tmpWidth;
        tpHeight = templateMap[selectCenterId]->tmpHeight;
        spinWidth = (QSpinBox*)ui->tableWidgetCenter->cellWidget(ui->tableWidgetCenter->currentRow(),2);
        spinHeight = (QSpinBox*)ui->tableWidgetCenter->cellWidget(ui->tableWidgetCenter->currentRow(),3);
        spinBrightCenter = (QSpinBox*)ui->tableWidgetCenter->cellWidget(ui->tableWidgetCenter->currentRow(),4);
        spinWidth->setValue(tpWidth);
        spinHeight->setValue(tpHeight);
        spinBrightCenter->setValue(templateMap[selectCenterId]->bright);
        spinWidth->setReadOnly(false);
        spinHeight->setReadOnly(false);
        spinBrightCenter->setReadOnly(false);
        connect(spinHeight,SIGNAL(valueChanged(int)),this,SLOT(refreshCenterHeight(int)));
        connect(spinWidth,SIGNAL(valueChanged(int)),this,SLOT(refreshCenterWidth(int)));
        spinHeight->setMaximum(ui->fullWidget->height()-2*rightUpPos.y());
        spinWidth->setMaximum(rightUpPos.x());

    }

    this->update();

}


void CameraCalibrate::refreshTemplate(int a, int b)
{
}

void CameraCalibrate::refreshTemplateLeftUpX(int pos)
{

    int value;
    if(pos > templateMap[selectTmpId]->rightDownX)
        value = templateMap[selectTmpId]->rightDownX - 1;
    else
        value = pos;
    tpLeftUpX = value;
    getCameraImage();

}


void CameraCalibrate::refreshTemplateLeftUpY(int pos)
{
    int value;
    if(pos > templateMap[selectTmpId]->rightDownY)
        value = templateMap[selectTmpId]->rightDownY - 1;
    else
        value = pos;
    tpLeftUpY = value;
    getCameraImage();


}



void CameraCalibrate::refreshTemplateRightDownX(int pos)
{
    int value;
    if(pos < templateMap[selectTmpId]->leftUpX)
        value = templateMap[selectTmpId]->leftUpX + 1;
    else
        value = pos;
    tpRightDownX = value;
    getCameraImage();

}

void CameraCalibrate::refreshTemplateRightDownY(int pos){
    int value;
    if(pos < templateMap[selectTmpId]->leftUpY)
        value = templateMap[selectTmpId]->leftUpX + 1;
    else
        value = pos;
    tpRightDownY = value;
    getCameraImage();

}

void CameraCalibrate::refreshCenterWidth(int width){
    tpWidth = width;
    getCameraImage();
}



void CameraCalibrate::refreshCenterHeight(int height){
    tpHeight = height;
    getCameraImage();
}


void CameraCalibrate::on_ButtonAddCenter_clicked()
{
    if(ui->comboBoxCamera->currentIndex()==-1)
        return;
    ui->tableWidgetCenter->insertRow(ui->tableWidgetCenter->rowCount());
    QString id = SysUtils::uuid();
    QSpinBox *spinWidth = new QSpinBox(this);
    QSpinBox *spinHeight = new QSpinBox(this);
    QSpinBox *spinBright = new QSpinBox(this);
    spinBright->setReadOnly(true);
    spinWidth->setReadOnly(true);
    spinHeight->setReadOnly(true);
    spinWidth->setMaximum(realAreaImg.width());
    spinHeight->setMaximum(realAreaImg.height());
    spinBright->setMaximum(100);
    spinBright->setValue(100);
    ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,0,new QTableWidgetItem(QString("%1").arg(QString::number(ui->tableWidgetCenter->rowCount()))));
    ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,1,new QTableWidgetItem(QString("tp%1").arg(QString::number(ui->tableWidgetCenter->rowCount()))));
    ui->tableWidgetCenter->setCellWidget(ui->tableWidgetCenter->rowCount()-1,2,spinWidth);
    ui->tableWidgetCenter->setCellWidget(ui->tableWidgetCenter->rowCount()-1,3,spinHeight);
    ui->tableWidgetCenter->setCellWidget(ui->tableWidgetCenter->rowCount()-1,4,spinBright);
    ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,5, new QTableWidgetItem(""));
    ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,6, new QTableWidgetItem(""));
    ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,7, new QTableWidgetItem(""));
    ui->tableWidgetCenter->setItem(ui->tableWidgetCenter->rowCount()-1,8, new QTableWidgetItem(id));
    CameraTemplate *cameraTemplate = new CameraTemplate(this);
    cameraTemplate->type = 1;
    cameraTemplate->id = id;
    cameraTemplate->name = QString("tp%1").arg(QString::number(ui->tableWidgetCenter->rowCount()));
    QString cameraId = ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString();
    int code,realCode;
    for(code = 101; code<200;code++)
    {
        realCode = cameraMap[cameraId]->sn*1000 +code;
        if(!tpCodeMap.contains(realCode))
            break;
    }
    cameraTemplate->code = realCode;
    ui->tableWidgetCenter->item(ui->tableWidgetCenter->rowCount()-1,0)->setText(QString::number(realCode));
    // ui->tableWidgetCenter->item(ui->tableWidgetCenter->rowCount()-1,0)->setFlags(  ui->tableWidgetCenter->item(ui->tableWidgetCenter->rowCount()-1,0)->flags() & (~Qt::ItemIsEditable));
    cameraTemplate->leftUpX = 0;
    cameraTemplate->leftUpY = 0;
    cameraTemplate->rightDownX = 0;
    cameraTemplate->rightDownY = 0;
    cameraTemplate->tmpWidth = 0;
    cameraTemplate->tmpHeight = 0;
    cameraTemplate->bright = 100;
    templateMap.insert(id,cameraTemplate);
    tpCodeMap.insert( cameraTemplate->code,id);
    QSqlQuery query;
    query.exec(QString("INSERT INTO CameraTemplate (id,cameraId,name,type,code,tmpWidth,tmpHeight,bright) VALUES('%1','%2','%3',1,%4,%5,%6,100)").arg(id,ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString(),cameraTemplate->name,QString::number(cameraTemplate->code),QString::number(cameraTemplate->tmpWidth),QString::number(cameraTemplate->tmpHeight)));
}

void CameraCalibrate::on_ButtonDelCenter_clicked()
{
    int row = ui->tableWidgetCenter->currentRow();
    if(row<0)
        return;
    if(0 != QMessageBox::question(NULL,"系统信息","是否确认删除",QMessageBox::tr("是"),QMessageBox::tr("否")))
        return;
    QString delId = ui->tableWidgetCenter->item(row,8)->text();
    disconnect(ui->tableWidgetCenter,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    tpCodeMap.remove(templateMap[delId]->code);
    templateMap.remove(delId);
    ui->tableWidgetCenter->removeRow(row);
    connect(ui->tableWidgetCenter,SIGNAL(itemSelectionChanged()),this,SLOT(selectTemplate()));
    if(ui->tableWidgetCenter->currentRow() == -1)
        selectCenterId ="";
    else
        ui->tableWidgetCenter->selectRow(ui->tableWidgetCenter->currentRow());
    QSqlQuery query;
    query.exec(QString("DELETE FROM CameraTemplate WHERE id='%1'").arg(delId));
}


void CameraCalibrate::on_ButtonAffrmTmp_clicked()
{



    qDebug()<<"selectTmpId.isEmpty()"<<selectTmpId.isEmpty();
    if(selectTmpId.isEmpty())
        return;

    int code = ui->tableWidgetTmp->item(ui->tableWidgetTmp->currentRow(),0)->text().toInt();
    if(tpCodeMap.contains(code) && tpCodeMap.value(code) != selectTmpId){
        QMessageBox::warning(NULL,"修改失败","模板编号出现重复,请重新修改模板编号",QMessageBox::tr("确认"));
        return;
    }
    tpCodeMap.remove(templateMap[selectTmpId]->code);

    tpCodeMap.insert(code,selectTmpId);

    QSpinBox *spinBox =(QSpinBox*)ui->tableWidgetTmp->cellWidget(ui->tableWidgetTmp->currentRow(),2);


    //zhu截取图片
    //qDebug()<<"ccc"<<captureCamera[cur_cameraIndex]->runFlag<<autoExposure.size();
    if(captureCamera[cur_cameraIndex]->runFlag==true)
    {

        //    IplImage *pGrayImg=cvCreateImage(cvGetSize(cameraIpImg), IPL_DEPTH_8U, 1);
        //    cvCvtColor(cameraIpImg, pGrayImg, CV_RGB2GRAY );

        //  QString imgSrc=QString("d:/DzicsRobot/image/%1.png").arg(code).toLocal8Bit().data();

        //    CvMat *pMat = cvGetSubRect(pGrayImg, cvCreateMatHeader(100, 100, CV_8UC1), cvRect(tpLeftUpX, tpLeftUpY, tpRightDownX-tpLeftUpX, tpRightDownY-tpLeftUpY));

        //    IplImage *pSubImg = cvGetImage(pMat, cvCreateImageHeader(cvSize(tpRightDownX-tpLeftUpX, tpRightDownY-tpLeftUpY), 8, 1));

        //    cvSaveImage(imgSrc.toLocal8Bit().data(),pSubImg);



        //    cvSetImageROI(cameraIpImg,cvRect(tpLeftUpX, tpLeftUpY, tpRightDownX-tpLeftUpX, tpRightDownY-tpLeftUpY));
        //    cvSaveImage(imgSrc.toLocal8Bit().data(),cameraIpImg);


        //得到亮度
        CvScalar scalar = cvAvg(cameraIpImg);
        cvResetImageROI(cameraIpImg);


        if(templateMap[selectTmpId]->code<10)
        {
            double d= scalar.val[0]/255*100;

            spinBox->setValue(d);

            autoExposure[cur_cameraIndex]->bright=d;

        }
        //end
    }


    int bright = spinBox->value();
    templateMap[selectTmpId]->code = code;
    templateMap[selectTmpId]->leftUpX = tpLeftUpX;
    templateMap[selectTmpId]->leftUpY = tpLeftUpY;
    templateMap[selectTmpId]->rightDownX = tpRightDownX;
    templateMap[selectTmpId]->rightDownY = tpRightDownY;
    templateMap[selectTmpId]->bright = bright;



    QImage image = realAreaImg.copy(templateMap[selectTmpId]->leftUpX,templateMap[selectTmpId]->leftUpY,templateMap[selectTmpId]->rightDownX-templateMap[selectTmpId]->leftUpX,templateMap[selectTmpId]->rightDownY-templateMap[selectTmpId]->leftUpY);
    QByteArray ba;

    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"png"); // writes image into ba in PNG format
    QSqlQuery query;
    query.prepare(QString("UPDATE CameraTemplate SET name='%1',code='%2',leftUpX=%3,leftUpY=%4,rightDownX=%5,rightDownY=%6,tpImage=?,bright=%7 WHERE id='%8'").arg(templateMap[selectTmpId]->name,QString::number(templateMap[selectTmpId]->code),QString::number(templateMap[selectTmpId]->leftUpX),QString::number(templateMap[selectTmpId]->leftUpY),QString::number(templateMap[selectTmpId]->rightDownX),QString::number(templateMap[selectTmpId]->rightDownY),QString::number(bright),selectTmpId));
    query.addBindValue(ba);

    if(!query.exec())
        qDebug()<<query.lastError().text();
//        query.exec(QString("SELECT tpImage FROM CameraTemplate WHERE id='%1'").arg(selectTmpId));
//        if(!query.next())
//            qDebug()<<"NO SQL";
//        QByteArray byte = query.value(0).toByteArray();
//        QImage myImage;
//        myImage.loadFromData(byte);
//        qDebug()<<myImage.save("ggggg.png");
   // qDebug()<<image.save("e:\\ggggg.png");

}



//void CameraCalibrate::on_ButtonCancelTmp_clicked()
//{
//    if(selectTmpId.isEmpty())
//        return;

//}

void CameraCalibrate::on_ButtonAffrmCenter_clicked()
{
    if(selectCenterId.isEmpty())
        return;
    int code = ui->tableWidgetCenter->item(ui->tableWidgetCenter->currentRow(),0)->text().toInt();
    if(tpCodeMap.contains(code) && tpCodeMap.value(code) != selectCenterId){
        QMessageBox::warning(NULL,"修改失败","模板编号出现重复,请重新修改模板编号",QMessageBox::tr("确认"));
        return;
    }
    tpCodeMap.remove(templateMap[selectCenterId]->code);
    tpCodeMap.insert(code,selectCenterId);
    QSpinBox *spinBox =(QSpinBox*)ui->tableWidgetCenter->cellWidget(ui->tableWidgetCenter->currentRow(),4);
    int bright = spinBox->value();
    templateMap[selectCenterId]->tmpWidth = tpWidth;
    templateMap[selectCenterId]->tmpHeight = tpHeight;
    templateMap[selectCenterId]->bright = bright;
    templateMap[selectCenterId]->code = code;
    QImage image = realAreaImg.copy((realAreaImg.width()-templateMap[selectCenterId]->tmpWidth)/2,(realAreaImg.height()-templateMap[selectCenterId]->tmpHeight)/2,templateMap[selectCenterId]->tmpWidth,templateMap[selectCenterId]->tmpHeight);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"png"); // writes image into ba in PNG format
    QSqlQuery query;
    query.prepare(QString("UPDATE CameraTemplate SET name='%1',code=%2,tmpWidth=%3,tmpHeight=%4,tpImage=?,bright=%5 WHERE id='%6'").arg(templateMap[selectCenterId]->name,QString::number(templateMap[selectCenterId]->code),QString::number(templateMap[selectCenterId]->tmpWidth),QString::number(templateMap[selectCenterId]->tmpHeight),QString::number(bright),selectCenterId));
    query.addBindValue(ba);
    query.exec();
    qDebug()<<QString("UPDATE CameraTemplate SET name='%1',code=%2,tmpWidth=%3,tmpHeight=%4,tpImage=?,bright=%5 WHERE id='%6'").arg(templateMap[selectCenterId]->name,QString::number(templateMap[selectCenterId]->code),QString::number(templateMap[selectCenterId]->tmpWidth),QString::number(templateMap[selectCenterId]->tmpHeight),QString::number(bright),selectCenterId);

}

void CameraCalibrate::on_buttonProperty_clicked()
{
    CameraManager *cameraManager = new CameraManager(currentSolutionId,this);
    cameraManager->setAttribute(Qt::WA_DeleteOnClose);
    connect(cameraManager,SIGNAL(signalAddCamera(QString&)),this,SLOT(addCamera(QString&)));
    connect(cameraManager,SIGNAL(signalDelCamera(QString&)),this,SLOT(delCamera(QString&)));
    connect(cameraManager,SIGNAL(signalUpdateCamera(QString&)),this,SLOT(updateCamera(QString&)));

    cameraManager->show();
}


void CameraCalibrate::addCamera(QString &cameraId)
{
    ui->comboBoxCamera->addItem(cameraMap[cameraId]->name,cameraId);

}


void CameraCalibrate::delCamera(QString &cameraId)
{
    for(int i=0; i<ui->comboBoxCamera->count(); i++)
    {
        if(ui->comboBoxCamera->itemData(i).toString() == cameraId){
            ui->comboBoxCamera->removeItem(i);
            break;
        }

    }

}


void CameraCalibrate::updateCamera(QString &cameraId)
{
    if(ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString() == cameraId){
        updateCameraInfo();
        ui->comboBoxCamera->setItemText(ui->comboBoxCamera->currentIndex(),cameraMap[cameraId]->name);
    }else{
        for(int i=0; i<ui->comboBoxCamera->count(); i++){
            if(ui->comboBoxCamera->itemData(i).toString() == cameraId){
                ui->comboBoxCamera->setItemText(i,cameraMap[cameraId]->name);
                break;
            }

        }
    }
}



//保存相机调节参数
void CameraCalibrate::on_pushButton_clicked()
{   if(currentCameraId == "")
        return;
    cameraMap[currentCameraId]->exposure = ui->spinBoxExposure->value();
    cameraMap[currentCameraId]->gain = ui->spinBoxGain->value();
    QSqlQuery query;
    query.exec(QString("UPDATE CameraProperty SET exposure=%1, gain=%2  WHERE id='%3'").arg(QString::number(cameraMap[currentCameraId]->exposure),QString::number(cameraMap[currentCameraId]->gain),currentCameraId));
}



//void CameraCalibrate::readFile()
//{
//    QFile f("d:/dzicsrobot/camera_cfg.txt");
//    if(!f.exists())
//        on_pushButton_3_clicked();
//    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        qDebug() << "Open failed.";
//        return;
//    }

//    QTextStream txtInput(&f);
//    QString Str=txtInput.readAll();
//    f.close();
//    QStringList s=Str.split(",");


//}


//void CameraCalibrate::on_pushButton_3_clicked()
//{

//    QFile f("d:/dzicsrobot/camera_cfg.txt");
//    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
//    {
//        qDebug() << "Open failed.";
//        return;
//    }

//    QTextStream txtOutput(&f);
//    QStringList s;
//    s<<"abc";
//    txtOutput << s.join(",");

//    f.close();
//    ui->label_15->setText("保存成功");

//}
//从摄像头获取左右领点
//void CameraCalibrate::on_pushButton_4_clicked()
//{
//    int index=ui->comboBox->currentIndex();
//    if(index==0)
//    {
//        if(ui->img_path->text()=="")
//            return;
//        QFile f(ui->img_path->text());
//        if(!f.exists())
//        {
//            ui->label_15->setText("文件不存在");
//            return;
//        }
//        IplImage* gray=cvLoadImage(ui->img_path->text().toLocal8Bit().data(),0);
//        QPoint point= getTemplateCenter(gray,ui->lineEdit_5->text().toInt());
//        ui->label_15->setText(QString("%1,%2").arg(point.x()).arg(point.y()));
//    }


//}

//void CameraCalibrate::on_pushButton_5_clicked()
//{
//   QString tempFile = QFileDialog::getOpenFileName(this,
//                                           tr("选择模板文件"),
//                                            "d:/dzicsrobot/image/",
//                                            "Images (*.png *.xpm *.jpg * .bmp)");

//    ui->img_path->setText(tempFile);


////    cvNamedWindow( "cvHoughCircles", 0 );
////          cvResizeWindow("cvHoughCircles",500,300);
////          cvShowImage( "cvHoughCircles", m_camImage2 );
////          cvWaitKey(0);
//}

void CameraCalibrate::on_pushButton_2_clicked()
{
    lock.lock();
    cvResetImageROI(cameraIpImg);

    QString path=QString("d:/%1.png").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    cvSaveImage(path.toLocal8Bit().data(),cameraIpImg);
    lock.unlock();
    ui->label_15->setText("保存成功"+QDateTime::currentDateTime().toString("mm:ss"));



}
cv::Mat g_srcImage,g_destImage;
int g_threshold=96;
void on_threshold(int,void*)
{


    threshold(g_srcImage, g_destImage, g_threshold, 255, THRESH_BINARY);
    imshow("21", g_destImage);
}

void CameraCalibrate::on_pushButton_6_clicked()
{

    qDebug()<<"selectTmpId.isEmpty()"<<selectTmpId.isEmpty();
    if(selectTmpId.isEmpty())
        return;

    qDebug()<<"autoExposure.size()"<<tpLeftUpX<<tpLeftUpY<< tpRightDownX-tpLeftUpX<<tpRightDownY-tpLeftUpY;
    if(captureCamera[cur_cameraIndex]->runFlag==true)
    {
        lock.lock();
        QString imgSrc=QString("d:/DzicsRobot/temp/%1.png").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")).toLocal8Bit().data();
        cvResetImageROI(cameraIpImg);

        cvSetImageROI(cameraIpImg,cvRect(tpLeftUpX, tpLeftUpY, tpRightDownX-tpLeftUpX, tpRightDownY-tpLeftUpY));
        cvSaveImage(imgSrc.toLocal8Bit().data(),cameraIpImg);



        cvResetImageROI(cameraIpImg);
        lock.unlock();
        QTimer::singleShot(0,this,SLOT(messageBox()));
        //end
    }


//    if(selectTmpId.isEmpty())
//    {
//        QMessageBox::about(this,"警告","请选择模板");
//        return;
//    }

//    int code = ui->tableWidgetTmp->item(ui->tableWidgetTmp->currentRow(),0)->text().toInt();
//    QString pic=QString("D:/DzicsRobot/image/%1.png").arg(code);

//   bool b= QFile::exists(pic);
//   if(!b)
//   {
//       QMessageBox::about(this,"警告","模板图不存在");
//       return;
//   }
//   string s1=pic.toLocal8Bit();
//    Mat m=imread(s1,0);
//    Mat dst;
//    dst.create(m.size(),m.type());

//    cv::resize(m, dst, Size(), 0.448, 0.448);

//    imshow("模板图"+s1,dst);




}

void CameraCalibrate::on_checkBox_clicked()
{
    if(cameraMap.size()==0)
        return;
    QSqlQuery sql;
    QString v="unChecked";
    QStringList list=cameraMap.keys();
    if(ui->checkBox->checkState()==Qt::Checked)
    {
        v="checked";
    }


    QString str=QString("update CameraProperty set model='%1' where id='%2'").arg(v,list[0]);

    sql.exec(str);
}

//void on_pushButton_7_clicked()
//{

//   bool b= QFile::exists("D:/2016.bmp");
//   if(b)
//   {
//      QString str=QString("_%1_%2_%3.bmp").arg(workParam[0]).arg(workParam[1]).arg(workParam[2]);
//      bool b1= QFile::rename("D:/2016.bmp","d:/"+QDateTime::currentDateTime().toString("yyyyMMddhhmmss")+str);
//      if(b1)
//          ui->label_15->setText(QDateTime::currentDateTime().toString("yyyyMMddhhmmss.bmp"));
//   }
//   else
//   {
//       ui->label_15->setText("无");
//   }
//}

void CameraCalibrate::on_pushButton_8_clicked()
{


    lock.lock();
    cvResetImageROI(cameraIpImg);
    g_srcImage=Mat(cameraIpImg).clone();

    lock.unlock();

    medianBlur(g_srcImage, g_srcImage, 3);
    namedWindow("21", 0 );
    resizeWindow("21",800,600);

    imshow("21",g_srcImage);
    createTrackbar("0", "21",&g_threshold, 255,on_threshold );
    on_threshold(0,0);


}

void CameraCalibrate::on_checkBox_2_clicked()
{

}

void CameraCalibrate::on_checkBox_3_clicked(bool checked)
{
    recordLog=checked;
}

void CameraCalibrate::on_pushButton_9_clicked()
{
    //上
    QString str="visual_001";
    mainWindow->sendVisualRes(str,"192.168.125.100");
}

void CameraCalibrate::on_pushButton_10_clicked()
{
    //下
    QString str="visual_002";
    mainWindow->sendVisualRes(str,"192.168.125.101");
}

void CameraCalibrate::on_pushButton_13_clicked()
{
    //QString s=QString("???%1|1").arg(ui->lineEdit->text());

    static bool sendReg=false;
    if(!sendReg)
    {
        sendReg=true;
        QByteArray s=QString("???").toLocal8Bit();

        s[0]=s.length();
        s[1]=0x81;
        s[2]=1;

        mainWindow->sendService2(s);



        s=QString("???").toLocal8Bit();
        s[0]=s.length();
        s[1]=0x1D;
        s[2]=1;
        mainWindow->sendService2(s);


    }

    QString s1=QString("???Vision_1|1");
    s1[0]=s1.length();
    s1[1]=0x1C;
    s1[2]=1;
    mainWindow->sendService2(s1.toLocal8Bit());
}


void CameraCalibrate::on_pushButton_12_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("导入数据"), "./", tr("*.txt"));
    int ret=solutionSet2->safeAreaExport(path);
    if(ret==0)
        ui->label_15->setText("导出成功");
    else
        ui->label_15->setText("导出error"+QString::number(ret));


}

//void CameraCalibrate::on_pushButton_11_clicked()
//{
//    ui->label_15->setText("");
//    QString tempFile = QFileDialog::getOpenFileName(this,
//                                            tr("选择模板文件"),
//                                             "d:/",
//                                             "*.txt");

//     ui->img_path->setText(tempFile);
//     QFile file(tempFile);
//     if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
//     {
//         QMessageBox::about(this,"sql.txt","can't open");
//         return;
//     }

//     QTextStream in(&file);
//     QStringList sql=in.readAll().split("\n");

//     file.close();
//     QSqlQuery query;
//     query.exec("delete from knife");
//     query.exec("delete from knife2");


//     for(int i=0;i<sql.size();i++)
//     {
//        query.exec(sql[i]);

//     }
//     ui->label_15->setText(QString("write OK"));

//}

void CameraCalibrate::setSysParam(QString title,QString value)
{
    QSqlQuery query;
    QString sql=QString("UPDATE SysParam SET value = '%1' WHERE code = '%2'").arg(value).arg(title);
    query.exec(sql);
}

void CameraCalibrate::on_pushButton_3_clicked()
{

    if(mainWindow->getTitle()=="大正工业机器人三维控制系统")
    {
        setSysParam("title","大正三维可视化控制系统");
        mainWindow->setTitle("大正三维可视化控制系统");
    }
    else
    {
        setSysParam("title","大正工业机器人三维控制系统");

        mainWindow->setTitle("大正工业机器人三维控制系统");
    }

}
double mathAngle(double a,double b)
{
    double c=sqrt(a*a*1.0+b*b*1.0);
    return acos(b*1.0/c)*180/3.1415;
}
double calcAngle(Point p1,Point p2)
{
    if(p1.x==p2.x)
        return 90;

    if(p1.y==p2.y)
        return 0;

    if(p1.x>p2.x)
    {
        Point p3=p1;
        p1=p2;
        p2=p3;
    }
    if(p2.y<p1.y)
        return mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x))*-1;
    else
        return mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x));

}
int getLineLen(Point p1,Point p2)
{
    return sqrt((p1.x-p2.x*1.0)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
void CameraCalibrate::on_pushButton_4_clicked()
{
    QString str=ui->lineEdit->text();
    QStringList list=str.split(",");
    if(list.count()<4)
        return;
    ui->label_15->setText(QString::number(-1*calcAngle(Point(list.at(0).toInt(),list.at(1).toInt()),Point(list.at(2).toInt(),list.at(3).toInt()))));
}

void CameraCalibrate::on_pushButton_5_clicked()
{
    QString str=ui->lineEdit->text();
    QStringList list=str.split(",");
    if(list.count()<4)
        return;
    ui->label_15->setText(QString::number(getLineLen(Point(list.at(0).toInt(),list.at(1).toInt()),Point(list.at(2).toInt(),list.at(3).toInt()))));

}

void CameraCalibrate::on_pushButton_7_clicked()
{
    QSqlQuery query;
   // query.exec(QString("select *"));
    qDebug()<<"curId"<<currentCameraId;
    QList<QString> list=templateMap.keys();
    for(int i=0;i<list.size();i++)
    {
       if(templateMap[list[i]]->code<10)
       {
           if(templateMap[list[i]]->bright+1<=100)
           {
                query.exec(QString("UPDATE CameraTemplate SET bright=%1 where id='%2'").arg(++templateMap[list[i]]->bright).arg(templateMap[list[i]]->id));
                autoExposure[cur_cameraIndex]->bright=templateMap[list[i]]->bright;
                ui->label_15->setText(QString::number(templateMap[list[i]]->bright));
           }
       }

    }
}

void CameraCalibrate::on_pushButton_14_clicked()
{
    QSqlQuery query;
   // query.exec(QString("select *"));
    qDebug()<<"curId"<<currentCameraId;
    QList<QString> list=templateMap.keys();
    for(int i=0;i<list.size();i++)
    {
       if(templateMap[list[i]]->code<10)
       {
           if(templateMap[list[i]]->bright-1>=0)
           {
                query.exec(QString("UPDATE CameraTemplate SET bright=%1 where id='%2'").arg(--templateMap[list[i]]->bright).arg(templateMap[list[i]]->id));
                autoExposure[cur_cameraIndex]->bright=templateMap[list[i]]->bright;
                ui->label_15->setText(QString::number(templateMap[list[i]]->bright));
           }
       }

    }
}

void CameraCalibrate::messageBox()
{
    QMessageBox::about(this,"save","保存成功");
}

void CameraCalibrate::on_pushButton_11_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("导入数据"), "./", tr("*.txt"));
    int ret=solutionSet2->safeAreaImport(path);
    if(ret==0)
        ui->label_15->setText("导入成功");
    else
        ui->label_15->setText("导入error"+QString::number(ret));
}

void CameraCalibrate::on_pushButton_15_clicked()
{
//    static int proVer=1;
//    if(proVer>6)
//        proVer=1;

//    switch (proVer)
//    {
//    case 1:
//        ui->pushButton_15->setText("振挺");
//        setSysParam("projectVersion","ZT1");
//        break;
//    case 2:
//         ui->pushButton_15->setText("雪龙");
//         setSysParam("projectVersion","XL");
//        break;
//    case 3:
//        ui->pushButton_15->setText("华验");
//        setSysParam("projectVersion","HD");
//        break;
//    case 4:
//        ui->pushButton_15->setText("汇丰");
//        setSysParam("projectVersion","HF");
//        break;
//    case 5:
//        ui->pushButton_15->setText("展会");
//        setSysParam("projectVersion","TEST");
//        break;
//    case 6:
//        ui->pushButton_15->setText("柔性配置");
//        setSysParam("projectVersion","RXPZ");
//        break;
//    }




    //proVer++;
}


//拷贝文件：
bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}


void CameraCalibrate::on_pushButton_16_clicked()
{


    QString desktop_path = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    QDir dir("D://DzicsRobot");


    QStringList nameFilters;
    nameFilters << "*.h" << "*.cpp"<<"*.ui";
    QStringList  files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Time);
    int m;
    if(ui->lineEdit->text()=="")
    {
       m=files.size();
    }
    else{
      m=ui->lineEdit->text().toInt();
    }
    int count=0;
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("MMdd_hhmm");
    QString aa=desktop_path+"\\"+current_date;
    qDebug()<<aa;
    QDir dir1(desktop_path+"\\"+current_date);
    dir1.mkdir(desktop_path+"\\"+current_date);
   for(int i=0;i<files.size();i++)
   {
       if(count==m)
       {
           return;
       }
       else{
       if(!files.at(i).startsWith("ui_")){
       QString sourceDir="D://DzicsRobot//"+files.at(i);
       QString toDir=desktop_path+"\\"+current_date+"\\"+files.at(i);
      copyFileToPath(sourceDir,toDir,false);
      count++;
       }

       }
     }
}

