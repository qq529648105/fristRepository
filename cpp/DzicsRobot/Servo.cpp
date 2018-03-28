#include "Servo.h"
#include "ui_Servo.h"
#include "Global.h"
#include <QTimer>
#include <QTime>
#include <QScrollArea>
#include <QCheckBox>
#include <QSqlQuery>
#include <QMouseEvent>
#include <QMessageBox>
#include "SysUtils.h"
#include <QListView>
Servo *g_servo=NULL;
QTimer *t1;
QTime t3,t4;
//钣金下拉框第几个
int CURRRENT=0;
//动作下拉框第几个
int ACTION=0;
//第几个工件型号
int CURRENT_MODEL=-1;
QCheckBox *servoActionCheck[10],*servoCheckVal[50],*banjinActionCheck[10];
QLabel *servoName1[10],*servoName2[10],*deviationName[10];
QPushButton *servoManual[50],*servoActionManual[10];
QLineEdit *servoEditHeight[50],*servoEditWidth[50],*servoEditPos[50],*deviationX[10],*deviationY[10],*deviationZ[10];
Servo::Servo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Servo)
{
    qDebug()<<"servo";
    t1=new QTimer(this);
    qsrand(QTime::currentTime().msec());
    connect(t1,SIGNAL(timeout()),this,SLOT(pressActionSlot()));
    ui->setupUi(this);
    //t1->start(1000);
    //ui->tabWidget->setCurrentIndex(1);
   // QTableWidget *tableWidget =ui->tableWidget;
    ui->tableWidget_gjmodel->verticalHeader()->hide();
    ui->tableWidget_gjmodel->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_gjmodel->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

   //滚动区域
    QScrollArea *s = new QScrollArea(ui->tab_3);
    s->setGeometry(0, 0, 700, 1000);
    s->setWidget(ui->widget);
    ui->widget->setGeometry(0, 0, 700, 3500);

    QScrollArea *s2 = new QScrollArea(ui->tab_3);
    s2->setGeometry(700, 0, 260, 1000);
    s2->setWidget(ui->widget_2);
    ui->widget_2->setGeometry(700,0,260,900);

    //钣金滚动区域
    QScrollArea *s_banjin = new QScrollArea(ui->tab_4);
    s_banjin->setGeometry(0, 100, 650, 1000);
    s_banjin->setWidget(ui->banjin_widget);
    ui->banjin_widget->setGeometry(0, 100, 650, 1300);

    ui->tableWidget_gjmodel->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //下拉框样式

//    ui->comboBox_BanjinModel->setStyleSheet("QComboBox{border:1px solid gray;}"
//        "QComboBox QAbstractItemView::item{height:80px;}"
//        "QComboBox::drop-down{border:0px;}");
//     //ui->comboBox_BanjinModel->setFont();
     //ui->comboBox_BanjinModel->setStyleSheet("font-size:35px;");
     ui->comboBox_BanjinModel->setStyleSheet("QComboBox QAbstractItemView::item{height:70px;}");
     QFont list_font;
     list_font.setPointSize(22);
     //list_font.setBold(true);
     list_font.setWeight(22);
     QFont list_font1;
     list_font1.setPointSize(15);
     list_font1.setBold(false);
     list_font1.setWeight(15);
     QListView *listView1=new QListView();
     QListView *listView2=new QListView();
     listView1->setFont(list_font);
     listView2->setFont(list_font1);
     ui->comboBox_BanjinModel->setView(listView1);
     //ui->comboBox_pianyiX->setStyleSheet("font: 15pt '微软雅黑';");

     ui->comboBox_pianyiX->setStyleSheet("QComboBox QAbstractItemView::item{height:35px;}");
     ui->comboBox_pianyiX->setView(listView2);


        //ui->comboBoxCamera->setView(new QListView());
    //设置滚动区域的窗体

    initUi();
    manualFlag=false;
    pressFlag=false;
    g_servo=this;




    connect(ui->btnPick1,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPick1,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnPick2,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPick2,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnPick3,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPick3,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnPick4,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPick4,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnPos1,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPos1,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnPos2,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPos2,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnPos3,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPos3,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnPos4,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnPos4,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnW1,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnW1,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnW2,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnW2,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnW3,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnW3,SIGNAL(released()),this,SLOT(releaseSlot()));
    connect(ui->btnW4,SIGNAL(pressed()),this,SLOT(pressSlot()));
    connect(ui->btnW4,SIGNAL(released()),this,SLOT(releaseSlot()));


    connect(ui->btnPick1,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnPick2,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnPick3,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnPick4,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnPos1,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnPos2,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnPos3,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnPos4,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnW1,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnW2,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnW3,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(ui->btnW4,SIGNAL(clicked()),this,SLOT(clickSlot()));

}



void Servo::initUi()
{
    //获取要显示的界面
    QSqlQuery servoquery;
    QString servosql=QString("select * from SysParam where code ='%1' ").arg("servoname");
    servoquery.exec(servosql);
    QString  servoData="";
    if(servoquery.next())
    {
         servoData=servoquery.value(1).toString();
         QStringList servoList=servoData.split(",");
         for(int i=0;i<servoList.size();i++){
             if(servoList.at(i)=="0")
             {
                 ui->tabWidget->setTabEnabled(i, false);
             }
             else
             {
                 ui->tabWidget->setTabEnabled(i, true);
             }
         }

         ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
    }



    cur_solutionId=SysUtils::getSysParam( "lastStationId" ).toString();
    QSqlQuery query;
    QString sql=QString("select * from safeArea where solutionId ='%1' and robotIndex=100").arg(cur_solutionId);

    QString sql2=QString("select * from safeArea where solutionId ='%1' and robotIndex=101").arg(cur_solutionId);

    QString sql3=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);

    query.exec(sql2);
    if(query.next())
    {
        cfg3=query.value(3).toString();
    }
    else
    {
        qDebug()<<"servo::create safeArea 101"<< query.exec(QString("INSERT INTO safeArea(id,solutionId,robotIndex,value1,value2) VALUES ('%1','%2',101,'','')").arg(SysUtils::uuid()).arg(cur_solutionId));
    }
    query.exec(sql);

    if(query.next())
    {
        cfg1=query.value(3).toString();
        cfg2=query.value(4).toString();
    }
    else
    {

        qDebug()<<"servo::create safeArea 100"<< query.exec(QString("INSERT INTO safeArea(id,solutionId,robotIndex,value1,value2) VALUES ('%1','%2',100,'','')").arg(SysUtils::uuid()).arg(cur_solutionId));
    }

    query.exec(sql3);

    if(query.next())
    {
        cfg4=query.value(3).toString();
        cfg5=query.value(4).toString();
    }
    else
    {

        qDebug()<<"servo::create safeArea 102"<< query.exec(QString("INSERT INTO safeArea(id,solutionId,robotIndex,value1,value2) VALUES ('%1','%2',102,'','')").arg(SysUtils::uuid()).arg(cur_solutionId));
    }

    int height1=40,height2=330;
    QStringList name1,name2;
    int count=0;
    int banjincount=0;
    name1<<"准备值"<<"作业前"<<"作业中"<<"作业后"<<"退出值";
    name2<<"前靠山"<<"前伸出"<<"后伸出"<<"托举"<<"再伸出";
    QRegExp regExp("^(\\+|\\-)?(\\d+|\\d+\\.[0-9]+)$");//浮点数
    QRegExp regExp2("^(\\d)?(\\d+|\\d+\\.[0-9]+)$");//正数
    QRegExpValidator *pRevalidotor = new QRegExpValidator(regExp, this);
    QRegExpValidator *pRevalidotor2 = new QRegExpValidator(regExp2, this);



    for(int i=0;i<10;i++)
    {
        servoActionCheck[i]=new QCheckBox(ui->widget);
        servoActionCheck[i]->setText(QString("动作%1").arg(i+1));
        servoActionCheck[i]->move(20,height2*i);
        //钣金动作
        banjinActionCheck[i]=new QCheckBox(ui->banjin_widget);
        banjinActionCheck[i]->setText(QString("动作%1").arg(i+1));
        banjinActionCheck[i]->move(20,35+90*i);

        servoActionManual[i]=new QPushButton("动作测试"+QString::number(i+1),ui->widget);
        servoActionManual[i]->setGeometry(530,40+height2*i,120,33);
        servoActionManual[i]->setObjectName(QString::number(i));

        connect(servoActionManual[i],SIGNAL(clicked()),this,SLOT(actionExec()));
        //钣金label和文本框创建
        for(int m=0;m<1;m++)
        {

                    deviationName[m]=new QLabel("偏移X",ui->banjin_widget);
                    deviationName[m]->move(20,25+40+90*i);
                    //deviationName[m]->setStyleSheet("font: bold; font-size:25px; color: rgb(241, 70, 62);");
                    deviationX[banjincount]=new QLineEdit(ui->banjin_widget);
                    deviationX[banjincount]->setGeometry(100,25+35+90*i,80,25);
                    //deviationX[banjincount]->setFont(QFont("微软雅黑" , 15 ));
                    deviationX[banjincount]->setObjectName(QString("banjin"+banjincount));
                    deviationX[banjincount]->setValidator(pRevalidotor);
                    deviationName[m]=new QLabel("偏移Y",ui->banjin_widget);

                    //deviationName[m]->setStyleSheet("font: bold; font-size:25px; color: rgb(241, 70, 62);");
                    deviationName[m]->move(240,25+40+90*i);

                    deviationY[banjincount]=new QLineEdit(ui->banjin_widget);

                    //deviationY[banjincount]->setFont(QFont("微软雅黑" , 15 ));
                    deviationY[banjincount]->setGeometry(320,25+35+90*i,80,25);
                    deviationY[banjincount]->setObjectName(QString("banjin"+banjincount));
                    deviationY[banjincount]->setValidator(pRevalidotor);


                    deviationName[m]=new QLabel("偏移Z",ui->banjin_widget);
                    deviationName[m]->move(440,25+40+90*i);
                    deviationZ[banjincount]=new QLineEdit(ui->banjin_widget);
                    deviationZ[banjincount]->setGeometry(520,25+35+90*i,80,25);
                    deviationZ[banjincount]->setObjectName(QString("banjin"+banjincount));
                    deviationZ[banjincount]->setValidator(pRevalidotor);

                    banjincount++;
        }




        for(int j=0;j<5;j++)
        {

        //准备值
        servoName1[i]=new QLabel(name1[j],ui->widget);
        servoName1[i]->move(20,80+height2*i+j*height1);

        //靠山

        servoCheckVal[count]=new QCheckBox(name2[j],ui->widget);
        servoCheckVal[count]->move(20+100*j,height2*i+38);

        //抓取
        servoEditHeight[count]=new QLineEdit(ui->widget);
        if(j>0&&j<5)
        {
            servoName1[i]=new QLabel("动作延时",ui->widget);
            servoEditHeight[count]->setValidator(pRevalidotor2);
        }
        else
        {
            servoName1[i]=new QLabel("抓取偏移",ui->widget);
            servoEditHeight[count]->setValidator(pRevalidotor);
        }
        servoName1[i]->move(105,80+height2*i+j*height1);


        servoEditHeight[count]->setGeometry(190,78+height2*i+j*height1,65,25);


        //位置
        servoName1[i]=new QLabel("位置",ui->widget);
        servoName1[i]->move(270,80+height2*i+j*height1);

        servoEditPos[count]=new QLineEdit(ui->widget);
        servoEditPos[count]->setGeometry(315,78+height2*i+j*height1,65,25);
        servoEditPos[count]->setObjectName(QString::number(count));
       // servoEditPos[count]->setValidator(pRevalidotor);

        connect(servoEditPos[count],SIGNAL(editingFinished()),this,SLOT(posChangeSlot()));

        //宽度
        servoName1[i]=new QLabel("宽度",ui->widget);
        servoName1[i]->move(400,80+height2*i+j*height1);

        servoEditWidth[count]=new QLineEdit(ui->widget);
        servoEditWidth[count]->setGeometry(450,78+height2*i+j*height1,65,25);
        servoEditWidth[count]->setObjectName(QString::number(count));
        //servoEditWidth[count]->setValidator(pRevalidotor2);
        connect(servoEditWidth[count],SIGNAL(editingFinished()),this,SLOT(widthChangeSlot()));


        servoManual[count]=new QPushButton("手动测试",ui->widget);
        servoManual[count]->setGeometry(530,75+height2*i+j*height1,120,33);
        servoManual[count]->setObjectName(QString::number(count));




        connect(servoManual[count],SIGNAL(clicked()),this,SLOT(singleStep()));
        ++count;
        }

    }

    ui->a1->setValidator(pRevalidotor);
    ui->a2->setValidator(pRevalidotor);
    ui->a3->setValidator(pRevalidotor);
    ui->a4->setValidator(pRevalidotor);
    ui->b1->setValidator(pRevalidotor);
    ui->b2->setValidator(pRevalidotor);
    ui->b3->setValidator(pRevalidotor);
    ui->b4->setValidator(pRevalidotor);


    updateUi();

}
//获取偏移数据
void Servo::getdeviation()
{
//    QString  workpiece_cfg="";
//    QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
//    QSqlQuery  workpiecequery;
//    workpiecequery.exec(workpiecesql);
//    if(workpiecequery.next())
//     {
//      workpiece_cfg=workpiecequery.value(3).toString();
//     }
//    QStringList cfg3List=workpiece_cfg.split("|");
//    cfgList1.clear();
//    cfgList2.clear();
//    cfgList1=cfg1.mid(10).split("|");
//    cfgList2=cfg2.split("|");

//    for(int i=0;i<10;i++)
//    {
//        if(cfg1.at(i)=='1')
//        {
//            banjinActionCheck[i]->setChecked(true);
//        }
//        else
//        {
//            banjinActionCheck[i]->setChecked(false);
//        }
//    }

}

void Servo::lockServo()
{
    ui->widget->setEnabled(false);
    ui->widget_2->setEnabled(false);
    ui->tab_2->setEnabled(false);
    ui->tab_3->setEnabled(false);
    ui->tab_4->setEnabled(false);
    ui->tab_5->setEnabled(false);
    ui->tab->setEnabled(false);



}

void Servo::unlockServo()
{
    ui->widget->setEnabled(true);
    ui->widget_2->setEnabled(true);
    ui->tab_2->setEnabled(true);
    ui->tab_3->setEnabled(true);
    ui->tab_4->setEnabled(true);
    ui->tab_5->setEnabled(true);
}
QStringList cfgList1,cfgList2;
void Servo::updateUi()
{
    ui->comboBox_BanjinModel->clear();
    ui->lineEdit_gjid->clear();
    ui->lineEdit_gjmodel->clear();
    cur_solutionId=SysUtils::getSysParam( "lastStationId" ).toString();
    QSqlQuery query;
    QString sql=QString("select * from safeArea where solutionId ='%1' and robotIndex=100").arg(cur_solutionId);




    query.exec(sql);

    if(query.next())
    {
        cfg1=query.value(3).toString();
        cfg2=query.value(4).toString();
    }

    sql=QString("select * from safeArea where solutionId ='%1' and robotIndex=101").arg(cur_solutionId);
    qDebug()<<"aaa"<<sql;
    query.exec(sql);
    if(query.next())
    {
        cfg3=query.value(3).toString();

    }
    QStringList cfg3List=cfg3.split("|");
    if(cfg3List.size()>1)
    {

        ui->workAset->setText(cfg3List[0]);
        ui->workBset->setText(cfg3List[1]);
    }

    QStringList tempList;
    cfgList1.clear();
    cfgList2.clear();
    cfgList1=cfg1.mid(10).split("|");
    cfgList2=cfg2.split("|");

    for(int i=0;i<10&&cfg1.size()>10;i++)
    {
        if(cfg1.at(i)=='1')
        {
            servoActionCheck[i]->setChecked(true);
        }
        else
        {
            servoActionCheck[i]->setChecked(false);
        }
    }



    int count=0;
    //动作界面初始化
    for(int i=0;i<cfgList1.size()&&cfgList1.size()>=10;i++)
    {
        tempList=cfgList1[i].split(",");

        if(tempList.size()<25)
            break;

        for(int j=0;j<tempList.size();j+=5)
        {
            //qDebug()<<"aaa"<<count+3<<j*5+3;
            servoCheckVal[count]->setChecked(tempList[j]=="100");
            servoEditHeight[count]->setText(tempList[j+1]);
            servoEditPos[count]->setText(tempList[j+2]);
            servoEditWidth[count]->setText(tempList[j+3]);

            count++;

        }




    }
    //int rowCount=-1;
    //工件参数界面初始化 zhangc
    QTableWidget *tableWidget =ui->tableWidget_gjmodel;
    for(int i = tableWidget->rowCount()-1; i>=0; i--)
      {
          tableWidget->removeRow(i);
      }

    QString  workpiece_cfg;
    QStringList GongjianList;
    QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
    QSqlQuery  workpiecequery;
    workpiecequery.exec(workpiecesql);
    if(workpiecequery.next())
      {
     workpiece_cfg=workpiecequery.value(4).toString();
      }
    if(workpiece_cfg!=""){
   GongjianList=workpiece_cfg.split("|");
   //QStringList workpieceidList,workpiecemodelList;
   for(int i=0;i<GongjianList.size();i++)
   {
       //rowCount++;
       tableWidget->setRowCount(i + 1);
       tableWidget->setRowHeight(i, 35);
       tableWidget->setItem(i,0,new QTableWidgetItem(qPrintable(GongjianList[i].split(",").at(0))));
       tableWidget->setItem(i,1,new QTableWidgetItem(qPrintable(GongjianList[i].split(",").at(1))));
       //workpieceidList.append(cfg3List[i].split(",").at(0));
       //workpiecemodelList.append(cfg3List[i].split(",").at(1));
   }

   //工件型号下拉框初始化
   ui->comboBox_BanjinModel->clear();
    QStringList workpieceidList,workpiecemodelList;
   for(int i=0;i<GongjianList.size();i++)
   {
       workpieceidList.append(GongjianList[i].split(",").at(0));
       workpiecemodelList.append(GongjianList[i].split(",").at(1));
   }
   for(int i=0;i<workpiecemodelList.size();i++)
       {
       ui->comboBox_BanjinModel->addItem(workpiecemodelList.at(i));
        }
}
    //参数配置初始化

    QList<QLineEdit*> ui1;
    ui1.push_back(ui->a1);
    ui1.push_back(ui->a2);
    ui1.push_back(ui->a3);
    ui1.push_back(ui->a4);
    ui1.push_back(ui->a5);
    ui1.push_back(ui->b1);
    ui1.push_back(ui->b2);
    ui1.push_back(ui->b3);
    ui1.push_back(ui->b4);
    ui1.push_back(ui->b5);



    for(int i=0;i<cfgList2.size();i++)
    {

        ui1[i]->setText(cfgList2[i]);

    }

    //选择最后选取的工件
    QSqlQuery lastworkquery;
    QString lastworksql=QString("select * from safeArea where solutionId ='%1' and robotIndex=120").arg(cur_solutionId);
    lastworkquery.exec(lastworksql);
    QString  lastWorkpiece="";
    if(lastworkquery.next())
    {
       lastWorkpiece=lastworkquery.value(3).toString();
       ui->comboBox_BanjinModel->setCurrentIndex(lastWorkpiece.toInt());
       ui->comboBox_pianyiX->setCurrentIndex(lastworkquery.value(4).toInt());
    }
    else
    {

        qDebug()<<"servo::create safeArea 120"<< query.exec(QString("INSERT INTO safeArea(id,solutionId,robotIndex,value1,value2) VALUES ('%1','%2',120,'0','0')").arg(SysUtils::uuid()).arg(cur_solutionId));
    }

}




void Servo::on_pushButton_9_clicked()
{


    QBrush myBrush;
    QPalette palette;
    myBrush = QBrush(Qt::red,Qt::DiagCrossPattern);
    palette.setBrush( QPalette::Text,  myBrush);
    //ui->pushButton_9->setStyleSheet("background-color: rgb(170, 0, 255);");

    QString data2="",data3="??",tempData="";
    int selectNum=0,curIndex=-1;
    QString selectIndex,selectIndexDB;
    for(int i=0;i<10;i++)
    {
        if(servoActionCheck[i]->isChecked())
        {
            ++selectNum;
            selectIndex+=QString("%1,").arg(i+1);
            selectIndexDB+="1";
        }
        else
        {
            selectIndexDB+="0";
        }

    }
    data2+=selectIndexDB;
    selectIndex.remove(selectIndex.length()-1,1);


    data3+=QString::number(selectNum);
    //data3+=selectIndex;
    for(int i=0;i<50;i++)
    {

        if(i%5==0)
        {
            data3+="|[";

        }


        tempData=QString("%1,%2,%3,%4,%5,")
                .arg(servoCheckVal[i]->isChecked()?"100":"200")
                .arg(servoEditHeight[i]->text()==""?"0":servoEditHeight[i]->text())
                .arg(servoEditPos[i]->text()==""?"0":servoEditPos[i]->text())
                .arg(servoEditWidth[i]->text()==""?"0":servoEditWidth[i]->text())
                .arg(0);



        if(!servoActionCheck[i/5]->isChecked())
        {
            if(curIndex!=i/5)
                data3+=QString("8200000,8200000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
            curIndex=i/5;
        }
        else
        {
            //qDebug()<<"ccc"<<tempData;
            data3+=tempData;
        }
        data2+=tempData;

        if((i+1)%5==0&&i>0)
        {
            data2+="|";
            data3+="]";
        }


    }


    data3.replace(",]","]");
    data2.replace(",|","|");
    data2.remove(data2.length()-1,1);
    QSqlQuery query;
    QString sql=QString("UPDATE safeArea SET value1='%1' WHERE solutionId='%2' and robotIndex=100").arg(data2).arg(cur_solutionId);

    qDebug()<<"sql:"<<sql<<query.exec(sql);

    qDebug()<<"data3"<<data3;

    qDebug()<<"data2"<<data2;

    QByteArray b1=data3.toLocal8Bit();
    b1[0]=CMD_WRITE_VAR_PLAN;
    b1[1]=1;

    mainWindow->mysendService(b1,1);
    ui->label_info->setText("发送成功");





}


Servo::~Servo()
{

    delete ui;
}

void Servo::setTabIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}




void Servo::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="开始")
    {
        t1->start(ui->lineEdit->text().toInt());
        ui->pushButton->setText("停止");
    }
    else
    {
        t1->stop();
        ui->pushButton->setText("开始");
    }
}

void Servo::clickSlot()
{

    if(pressFlag)
       return;
    if(!manualFlag)
        return;

    QObject *object = QObject::sender();
    QPushButton *push_button = qobject_cast<QPushButton *>(object);

    QString btnName = push_button->objectName();

    setParam(btnName);

    setCurEdit(editType,manualIndex,changeVal);

   QTimer::singleShot(300,this,SLOT(singleSend()));

   t3=QTime::currentTime();



}

void Servo::setParam(QString btnName)
{

    if(btnName=="btnPick1")
    {
        editType=1;
        changeVal=1;
    }
    else if(btnName=="btnPick2")
    {
        editType=1;
        changeVal=0.1;
    }
    else if(btnName=="btnPick3")
    {
        editType=1;
        changeVal=-1;
    }else if(btnName=="btnPick4")
    {
        editType=1;
        changeVal=-0.1;
    }else if(btnName=="btnPos1")
    {
        editType=2;
        changeVal=1;
    }
    else if(btnName=="btnPos2")
        {
            editType=2;
            changeVal=0.1;
        }
    else if(btnName=="btnPos3")
        {
            editType=2;
            changeVal=-1;
        }
    else if(btnName=="btnPos4")
        {
            editType=2;
            changeVal=-0.1;
        }
    else if(btnName=="btnW1")
        {
            editType=3;
            changeVal=1;
        }
    else if(btnName=="btnW2")
        {
            editType=3;
            changeVal=0.1;
        }
    else if(btnName=="btnW3")
        {
            editType=3;
            changeVal=-1;
        }
    else if(btnName=="btnW4")
        {
            editType=3;
            changeVal=-0.1;
        }
}
void Servo::pressSlot()
{
    QObject *object = QObject::sender();
    QPushButton *push_button = qobject_cast<QPushButton *>(object);

    QString btnName = push_button->objectName();

    setParam(btnName);

    //qDebug()<<btnName<<editType<<changeVal;

    int t=ui->lineEdit_2->text().toInt();

    t4=QTime::currentTime();
    if(t<50)
        t=50;
    t1->start(t);

}
void Servo::setPressFlag()
{
     pressFlag=false;
}
void Servo::releaseSlot()
{

    QTimer::singleShot(300,this,SLOT(setPressFlag()));
    t1->stop();
}

void Servo::singleSend()
{

    if(!manualFlag)
        return;

    if(t3.msecsTo(QTime::currentTime())<300)
        return;
    int num1=1000+manualIndex/5+1;
    QString data=QString("??%1%2").arg(num1).arg(manualIndex%5+1);
    qDebug()<<"index"<<manualIndex<<"data:"<<data;

    QByteArray b1=data.toLocal8Bit();
    b1[0]=CMD_WRITE_VAR_MANUAL_ACTION;
    b1[1]=1;
    mainWindow->mysendService(b1,1);
    ui->label_time->setText(QString::number(++allCount));
}

void Servo::actionSend()
{

    int num1=1000+actionIndex+1;
    QString data=QString("??%1").arg(num1);
    qDebug()<<"index"<<actionIndex<<"data:"<<data;

    QByteArray b1=data.toLocal8Bit();
    b1[0]=CMD_WRITE_VAR_MANUAL_ACTION;
    b1[1]=1;
    mainWindow->mysendService(b1,1);

}

void Servo::pressActionSlot()
{
    if(t4.msecsTo(QTime::currentTime())<300)
        return;
    setCurEdit(editType,manualIndex,changeVal);



    bool flag=true;

     pressFlag=true;
    qDebug()<<"pressActionSlot"<<changeVal<<manualIndex<<editType;
    if(flag)
        singleSend();
}

void Servo::singleStep()
{

    allCount=0;
    manualFlag=!manualFlag;

    QObject *object = QObject::sender();
    QPushButton *push_button = qobject_cast<QPushButton *>(object);

    int index = push_button->objectName().toInt();
    if(manualFlag)
    {
        manualIndex=index;
        curRow[0] = servoEditHeight[index]->text().toDouble();

        curRow[1] = servoEditPos[index]->text().toDouble();
        curRow[2] = servoEditWidth[index]->text().toDouble();
        //qDebug()<<"record val"<<curRow[0]<<curRow[1]<<curRow[2];
        if((index+1)%5==1)
        {
            ui->label_20->setText("抓取");
            ui->label_19->setText("  1mm");
            ui->label_21->setText("  0.1mm");
        }
        else
        {
            ui->label_20->setText("动作");
            ui->label_19->setText("  10ms");
            ui->label_21->setText("  1ms");
        }


        for(int i=0;i<50;i++)
        {


            if(servoManual[i]->objectName()!=push_button->objectName())
                servoManual[i]->setEnabled(false);
        }
        for(int i=0;i<10;i++)
        {
            servoActionManual[i]->setEnabled(false);
        }

        push_button->setStyleSheet("background-color: rgb(188, 188, 188);border: 2px solid rgb(50 , 50 , 50);");

        singleSend();
    }
    else
    {
        for(int i=0;i<50;i++)
        {
            servoManual[i]->setEnabled(true);
        }

        for(int i=0;i<10;i++)
        {
            servoActionManual[i]->setEnabled(true);
        }
        push_button->setStyleSheet("background-color:none");
    }







}

void Servo::actionExec()
{


    manualFlag=!manualFlag;


    QObject *object = QObject::sender();
    QPushButton *push_button = qobject_cast<QPushButton *>(object);

    int index = push_button->objectName().toInt();
    if(manualFlag)
    {
        actionIndex=index;

        for(int i=0;i<50;i++)
        {

                servoManual[i]->setEnabled(false);


        }
        for(int i=0;i<10;i++)
        {
             if(servoActionManual[i]->objectName()!=push_button->objectName())
                servoActionManual[i]->setEnabled(false);
        }

        push_button->setStyleSheet("background-color: rgb(188, 188, 188);border: 2px solid rgb(50 , 50 , 50);");


        actionSend();
    }
    else
    {
        for(int i=0;i<50;i++)
        {
            servoManual[i]->setEnabled(true);
        }

        for(int i=0;i<10;i++)
        {
            servoActionManual[i]->setEnabled(true);
        }
        push_button->setStyleSheet("background-color:none");
    }







}

void Servo::on_pushButton_2_clicked()
{

    if(ui->a5->text().toInt()<100||ui->a5->text().toInt()>4000)
        ui->a5->setText("1000");

    if(ui->b5->text().toInt()<100||ui->b5->text().toInt()>4000)
        ui->b5->setText("1000");

    QString data=QString("??%1|%2|%3|%4|%5|%6|%7|%8|%9|%10")
            .arg(ui->a1->text())
            .arg(ui->a2->text())
            .arg(ui->a3->text())
            .arg(ui->a4->text())
            .arg(ui->a5->text())
            .arg(ui->b1->text())
            .arg(ui->b2->text())
            .arg(ui->b3->text())
            .arg(ui->b4->text())
            .arg(ui->b5->text());
    cfgList2.clear();
    cfgList2=data.mid(2).split("|");
    QByteArray b1=data.toLocal8Bit();
    b1[0]=CMD_WRITE_VAR_SERVO_PARAMS;
    b1[1]=1;
    mainWindow->mysendService(b1,1);

    qDebug()<<data;
    QSqlQuery query;
    QString sql=QString("UPDATE safeArea SET value2='%1' WHERE solutionId='%2' and robotIndex=100").arg(data.mid(2)).arg(cur_solutionId);

    qDebug()<<"exec ret:"<<query.exec(sql);
    qDebug()<<"s1"<<data;
}

void Servo::setPosEdit(int index,double val)
{
    if(manualFlag)
    {
    double d=servoEditPos[index]->text().toDouble();
    servoEditPos[index]->setText(QString::number(d+val));
    }
}

void Servo::setWidthEdit(int index, double val)
{
    if(manualFlag)
    {
    double d=servoEditWidth[index]->text().toDouble();
    servoEditWidth[index]->setText(QString::number(d+val));
    }
}

void Servo::setHeightEdit(int index, double val)
{
    if(manualFlag)
    {
    double d=servoEditHeight[index]->text().toDouble();
    servoEditHeight[index]->setText(QString::number(d+val));
    }
}

void Servo::setCurEdit(int type, int index, double val)
{

    if(manualFlag)
    {



        if(type==1)
        {
            double d=servoEditHeight[index]->text().toDouble();
            servoEditHeight[index]->setText(QString::number(d+val));
            ui->label_24->setText(QString::number(d+val-curRow[0]));

        }
        else if(type==2)
        {

             double d=servoEditPos[index]->text().toDouble();
             servoEditPos[index]->setText(QString::number(d+val));
             ui->label_25->setText(QString::number(d+val-curRow[1]));
             qDebug()<<"cccc"<<d+val<<curRow[1];
             checkPos(index);

        }
        else if(type==3)
        {

            double d=servoEditWidth[index]->text().toDouble();
            if(d+val>=0)
            {
            servoEditWidth[index]->setText(QString::number(d+val));
            ui->label_27->setText(QString::number(d+val-curRow[2]));

            checkWidth(index);

            }

        }

    }
}



void Servo::posChangeSlot()
{


    QObject *object = QObject::sender();
    QLineEdit *edit = qobject_cast<QLineEdit *>(object);

    int index = edit->objectName().toInt();

    qDebug()<<"index"<<index<<servoEditPos[index]->text();

    checkPos(index);


}

void Servo::widthChangeSlot()
{


    QObject *object = QObject::sender();
    QLineEdit *edit = qobject_cast<QLineEdit *>(object);

    int index = edit->objectName().toInt();

    checkWidth(index);



}

void Servo::checkPos(int index)
{
    if(servoEditPos[index]->text().toDouble()<0)
        servoEditPos[index]->setText("0");




    ui->label_info->setText("");
    int cfg_a1= ui->a1->text().toInt();
    int cfg_a2= ui->a2->text().toInt();
    int cfg_b1= ui->b1->text().toInt();
    int cfg_b2= ui->b2->text().toInt();

    double pos1=servoEditPos[index]->text().toDouble();
    double width1=servoEditWidth[index]->text().toDouble();

        if(servoCheckVal[index/5*5]->isChecked())
        {
            //A
            if(cfg_a1-pos1<0)
            {

                ui->label_info->setText(QString("超出最小值"));
                servoEditPos[index]->setText(QString::number(cfg_a1));

            }

            if(cfg_a1-pos1>cfg_a2)
            {

                ui->label_info->setText(QString("超出最大值"));
                servoEditPos[index]->setText(QString::number(cfg_a1-cfg_a2));

            }

            //B
            if(cfg_b1-width1+pos1<0)
            {
                ui->label_info->setText(QString("超出最小值"));
                servoEditPos[index]->setText(QString::number(width1-cfg_b1));
            }


            if(cfg_b1-width1+pos1>cfg_b2)//B
            {

                ui->label_info->setText(QString("超出最大值"));
                servoEditPos[index]->setText(QString::number(cfg_b2-cfg_b1+width1));

            }
        }
        else
        {//后靠山
            if(cfg_a1-width1+pos1<0)//A
            {

                ui->label_info->setText(QString("超出最小值"));
                servoEditPos[index]->setText(QString::number(width1-cfg_a1));


            }
            if(cfg_a1-width1+pos1>cfg_a2)
            {

                ui->label_info->setText(QString("超出最大值"));
                servoEditPos[index]->setText(QString::number(cfg_a2-cfg_a1+width1));

            }

            if(cfg_b1-pos1<0)//B
            {

                ui->label_info->setText(QString("超出最小值"));
                servoEditPos[index]->setText(QString::number(cfg_b1));

            }

            if(cfg_b1-pos1>cfg_b2)
            {

                ui->label_info->setText(QString("超出最大值"));
                servoEditPos[index]->setText(QString::number(cfg_b1-cfg_b2));

            }




        }


}
void Servo::checkWidth(int index)
{
    if(servoEditWidth[index]->text().toDouble()<0)
        servoEditWidth[index]->setText("0");



    ui->label_info->setText("");
    int cfg_a1= ui->a1->text().toInt();
    int cfg_a2= ui->a2->text().toInt();
    int cfg_b1= ui->b1->text().toInt();
    int cfg_b2= ui->b2->text().toInt();
    double pos1=servoEditPos[index]->text().toDouble();
    double width1=servoEditWidth[index]->text().toDouble();


    if(servoCheckVal[index/5*5]->isChecked())
    {

        qDebug()<<"bbb"<<cfg_b1<<width1<<pos1;
        if(cfg_b1-width1+pos1<0)
        {
            ui->label_info->setText(QString("超出最小值"));
            servoEditWidth[index]->setText(QString::number(pos1+cfg_b1));
        }


        if(cfg_b1-width1+pos1>cfg_b2)//B
        {

            ui->label_info->setText(QString("超出最大值"));
            servoEditWidth[index]->setText(QString::number(cfg_b1+pos1-cfg_b2));

        }
    }
    else
    {//后靠山
        if(cfg_a1-width1+pos1<0)//A
        {

            ui->label_info->setText(QString("超出最小值"));
            servoEditWidth[index]->setText(QString::number(pos1+cfg_a1));


        }
        if(cfg_a1-width1+pos1>cfg_a2)
        {

            ui->label_info->setText(QString("超出最大值"));
            servoEditWidth[index]->setText(QString::number(cfg_a1+pos1-cfg_a2));

        }



    }


}

//void Servo::on_pushButton_3_clicked()
//{

//     QString path = QFileDialog::getSaveFileName(this, tr("导出数据"), "./", tr("*.txt"));
//     if(path == ""){return; }
//     QFile file(path);
//     if(!file.open(QIODevice::WriteOnly))
//         return ;
//     QTextStream in(&file);

//     QSqlQuery query;
//     QString sql=QString("select * from safeArea where solutionId ='%1' and robotIndex=100").arg(cur_solutionId);
//     query.exec(sql);
//     if(query.next())
//     {
//         in<<query.value(3).toString();
//         in<<"\r\n";
//         in<<query.value(4).toString();
//     }
//     file.close();
//     ui->label_19->setText("导出成功");
//}
//int Servo::servoImport()
//{

//     QString path = QFileDialog::getOpenFileName(this, tr("导入数据"), "./", tr("*.txt"));
//     if(path=="")
//         return 1;
//    QFile file(path);
//    if(!file.open(QIODevice::ReadOnly))
//        return 2 ;
//    QString str= file.readAll();
//    QStringList list=str.split("\r\n");

//    if(list.size()<1)
//       return 3;

//    QSqlQuery query;
//    QString sql=QString("UPDATE safeArea SET value1='%1',value2='%2' WHERE solutionId='%3' and robotIndex=100").arg(list[0]).arg(list[1]).arg(cur_solutionId);
//    qDebug()<<"sql:"<<sql;
//    if(!query.exec(sql))
//        return 4;

//    file.close();
//    cfg1=list[0];
//    cfg2=list[1];
//    return 0;
//}



//void Servo::on_pushButton_4_clicked()
//{
//    int ret=servoImport();
//    if(ret==0)
//    {
//        ui->label_19->setText("导入成功");
//        updateUi();
//    }
//    else
//        ui->label_19->setText("导入失败"+QString::number(ret));
//}



void Servo::on_pushButton_3_clicked()
{
    QString str=QString("%1|%2").arg(ui->workAset->text().toInt()).arg(ui->workBset->text().toInt());
    QSqlQuery query;
    QString sql=QString("UPDATE safeArea SET value1='%1' WHERE solutionId='%2' and robotIndex=101").arg(str).arg(cur_solutionId);
    query.exec(sql);
}

void Servo::on_pushButton_4_clicked()
{
        QTableWidget *tableWidget =ui->tableWidget_gjmodel;
        int rowCount=tableWidget->rowCount();
        QString intdata="0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
        //工件偏移量数据
        QString  workpiece_cfg="";
        QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
        QSqlQuery  workpiecequery;
        workpiecequery.exec(workpiecesql);
        if(workpiecequery.next())
         {
          workpiece_cfg=workpiecequery.value(3).toString();
         }
        //工件型号数据
        QString  workpiece_cfg1="";
        QString workpiecesql1=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
        QSqlQuery  workpiecequery1;
        workpiecequery1.exec(workpiecesql1);
        if(workpiecequery1.next())
         {
          workpiece_cfg1=workpiecequery1.value(4).toString();
         }
        //获取数据库工件型号Id
        QStringList workpieceidList,workpiecemodelList;
        workpieceidList.clear();
        workpiecemodelList.clear();
        for(int i=0;i<rowCount;i++)
        {
        workpieceidList.append(tableWidget->item(i,0)->text());
        workpiecemodelList.append(tableWidget->item(i,1)->text());

        }
        QString id=ui->lineEdit_gjid->text();
        QString model=ui->lineEdit_gjmodel->text();
        if(workpieceidList.contains(id))
        {
           QMessageBox::information(this, "提示", id+"工件编号已存在！");
           return;
        }
        if(workpiecemodelList.contains(model))
        {
           QMessageBox::information(this, "提示", model+"工件型号已存在！");
           return;
        }
        QString pattern("^[0-9]*[1-9][0-9]*$");
        QRegExp rx(pattern);
        if (rx.exactMatch(id)==false)
        {
            QMessageBox::information(this, "提示", id+"工件编号要为正整数！");
            return;
               }
        if (model=="" || model=="NULL" || model=="null")
        {
            QMessageBox::information(this, "提示", model+"工件型号不能为空！");
            return;
               }
        if(workpiece_cfg1!="")
            {
            workpiece_cfg1+="|";
        }
        workpiece_cfg1+=id;
        workpiece_cfg1+=",";
        workpiece_cfg1+=model;
        if(workpiece_cfg!="")
            {
            workpiece_cfg+="|";
        }
        workpiece_cfg+=model;
        workpiece_cfg+=",";
        workpiece_cfg+=intdata;
        workpiecemodelList.append(model);
        QSqlQuery query1;
        QString sql1=QString("UPDATE safeArea SET value1='%1' , value2='%2' WHERE solutionId='%3' and robotIndex=102").arg(workpiece_cfg).arg(workpiece_cfg1).arg(cur_solutionId);

        qDebug()<<"sql:"<<sql1<<query1.exec(sql1);
        //刷新数据
        ui->comboBox_BanjinModel->clear();
        for(int i=0;i<workpiecemodelList.size();i++)
        {
        ui->comboBox_BanjinModel->addItem(workpiecemodelList.at(i));
         }
        ui->comboBox_BanjinModel->setCurrentIndex(workpiecemodelList.size()-1);

        QString  workpiece_cfg2;
        QStringList GongjianList2;
        QString workpiecesql2=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
        QSqlQuery  workpiecequery2;
        workpiecequery2.exec(workpiecesql2);
        if(workpiecequery2.next())
          {
         workpiece_cfg2=workpiecequery2.value(4).toString();
          }
        if(workpiece_cfg2!=""){
       GongjianList2=workpiece_cfg2.split("|");
       //QStringList workpieceidList,workpiecemodelList;
       for(int i=0;i<GongjianList2.size();i++)
       {
           //rowCount++;
           tableWidget->setRowCount(i + 1);
           tableWidget->setRowHeight(i, 35);
           tableWidget->setItem(i,0,new QTableWidgetItem(qPrintable(GongjianList2[i].split(",").at(0))));
           tableWidget->setItem(i,1,new QTableWidgetItem(qPrintable(GongjianList2[i].split(",").at(1))));
           //workpieceidList.append(cfg3List[i].split(",").at(0));
           //workpiecemodelList.append(cfg3List[i].split(",").at(1));
       }


}

}

void Servo::on_pushButton_6_clicked()
{
        QString  workpiece_cfg;
        QStringList GongjianList,workIdList;
        QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
        QSqlQuery  workpiecequery;
        workpiecequery.exec(workpiecesql);
        if(workpiecequery.next())
         {
          workpiece_cfg=workpiecequery.value(3).toString();
          }
        GongjianList=workpiece_cfg.split("|");
        for(int i=0;i<GongjianList.size();i++)
         {
         workIdList.append(GongjianList[i].split(",").at(0));

        }

        QTableWidget *tableWidget =ui->tableWidget_gjmodel;
        int rowCount=tableWidget->rowCount();
        QString workpiecedata="";
        QString deviationdata="";
        QStringList workpieceidList,workpiecemodelList;
        workpieceidList.clear();
        workpiecemodelList.clear();
        for(int i=0;i<rowCount && i!=CURRENT_MODEL;i++)
        {
        workpieceidList.append(tableWidget->item(i,0)->text());
        workpiecemodelList.append(tableWidget->item(i,1)->text());

        }

            QString id=ui->lineEdit_gjid->text();
            QString model=ui->lineEdit_gjmodel->text();
            if(workpieceidList.contains(id))
            {
               QMessageBox::information(this, "提示", id+"工件编号已存在！");
               return;
            }
            if(workpiecemodelList.contains(model))
            {
               QMessageBox::information(this, "提示", model+"工件型号已存在！");
               return;
            }
            QString pattern("^[0-9]*[1-9][0-9]*$");
            QRegExp rx(pattern);
            if (rx.exactMatch(id)==false)
            {
                QMessageBox::information(this, "提示", id+"工件编号要为正整数！");
                return;
                   }
            if (model=="" || model=="NULL" || model=="null")
            {
                QMessageBox::information(this, "提示", model+"工件型号不能为空！");
                return;
                   }


        for(int i=0;i<rowCount;i++)
        {
            QString gongjianid;
            QString gongjianmodel;
            if(i==CURRENT_MODEL)
            {
             gongjianid=ui->lineEdit_gjid->text();
             gongjianmodel=ui->lineEdit_gjmodel->text();
            }
            else
            {
                gongjianid=tableWidget->item(i,0)->text();
                gongjianmodel=tableWidget->item(i,1)->text();

            }


            QString pattern("^[0-9]*[1-9][0-9]*$");
            QRegExp rx(pattern);
            if (gongjianid == "NULL" || gongjianid=="")
            {
                QMessageBox::information(this, "提示", "工件编号不能为NULL！");
                return;
            }


            if (rx.exactMatch(gongjianid)==false)
            {
                QMessageBox::information(this, "提示", gongjianid+"工件编号要为正整数！");
                return;
                   }
            if(gongjianmodel== "NULL" || gongjianmodel=="")
            {
               QMessageBox::information(this, "提示", "工件型号不能为NULL！");
               return;
            }
        workpiecedata+=gongjianid;
        workpiecedata+=",";
        workpiecedata+=gongjianmodel;
        workpiecedata+="|";
        QString qs_pianyi=GongjianList[i].mid(GongjianList[i].indexOf(",")+1,GongjianList[i].size());
        deviationdata+=gongjianid;
        deviationdata+=",";
        deviationdata+=qs_pianyi;
        deviationdata+="|";
        }
        workpiecedata.remove(workpiecedata.length()-1,1);
        deviationdata.remove(deviationdata.length()-1,1);


        QSqlQuery query;
        QString sql=QString("UPDATE safeArea SET value1='%1' , value2='%2' WHERE solutionId='%3' and robotIndex=102").arg(deviationdata).arg(workpiecedata).arg(cur_solutionId);

        qDebug()<<"sql:"<<sql<<query.exec(sql);




        //刷新数据


        QString  workpiece_cfg2;
        QStringList GongjianList2;
        QString workpiecesql2=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
        QSqlQuery  workpiecequery2;
        workpiecequery2.exec(workpiecesql2);
        if(workpiecequery2.next())
          {
         workpiece_cfg2=workpiecequery2.value(4).toString();
          }
        if(workpiece_cfg2!=""){
       GongjianList2=workpiece_cfg2.split("|");
       //QStringList workpieceidList,workpiecemodelList;

       for(int i=0;i<GongjianList2.size();i++)
       {
           //rowCount++;
           tableWidget->setRowCount(i + 1);
           tableWidget->setRowHeight(i, 35);
           tableWidget->setItem(i,0,new QTableWidgetItem(qPrintable(GongjianList2[i].split(",").at(0))));
           tableWidget->setItem(i,1,new QTableWidgetItem(qPrintable(GongjianList2[i].split(",").at(1))));
           //workpieceidList.append(cfg3List[i].split(",").at(0));
           //workpiecemodelList1.append(cfg3List[i].split(",").at(1));
       }


}
        QStringList workpiecemodelList1;
        ui->comboBox_BanjinModel->clear();

        for(int i=0;i<rowCount ;i++)
        {
        workpiecemodelList1.append(tableWidget->item(i,1)->text());

        }

        for(int i=0;i<workpiecemodelList1.size();i++)
        {
        ui->comboBox_BanjinModel->addItem(workpiecemodelList1.at(i));
         }


}

void Servo::on_pushButton_5_clicked()
{
    QMessageBox messageBox;

    messageBox.setText(tr("确定删除该工件吗？"));

    messageBox.setWindowTitle(tr("温馨提示"));

    //确定，取消按钮

    QPushButton *okButton = messageBox.addButton(tr("确定"),QMessageBox::YesRole);

    QPushButton *cancelButton = messageBox.addButton(tr("取消"),QMessageBox::NoRole);

    messageBox.exec(); //循环

    if(messageBox.clickedButton() == okButton)

    {
        int currentRow=ui->tableWidget_gjmodel->currentRow();
        if(currentRow!=-1)
        {
            //工件偏移量数据
            QString  workpiece_cfg="";
            QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
            QSqlQuery  workpiecequery;
            workpiecequery.exec(workpiecesql);
            if(workpiecequery.next())
             {
              workpiece_cfg=workpiecequery.value(3).toString();
             }
            QStringList pianyiList=workpiece_cfg.split("|");
            //工件型号数据
            QString  workpiece_cfg1="";
            QString workpiecesql1=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
            QSqlQuery  workpiecequery1;
            workpiecequery1.exec(workpiecesql1);
            if(workpiecequery1.next())
             {
              workpiece_cfg1=workpiecequery1.value(4).toString();
             }
             QStringList xinghaoList=workpiece_cfg1.split("|");
             QString value1,value2;
             for(int i=0;i<pianyiList.size();i++)
             {
                 if(i!=currentRow)
                 {
                  value1+=pianyiList.at(i);
                  value1+="|";
                  value2+=xinghaoList.at(i);
                  value2+="|";

                 }
             }
            value1.remove(value1.length()-1,1);
            value2.remove(value2.length()-1,1);
            QSqlQuery query1;
            QString sql1=QString("UPDATE safeArea SET value1='%1' , value2='%2' WHERE solutionId='%3' and robotIndex=102").arg(value1).arg(value2).arg(cur_solutionId);
            qDebug()<<"sql:"<<sql1<<query1.exec(sql1);
            ui->tableWidget_gjmodel->removeRow(currentRow);
            //获取数据库工件型号Id
            QStringList workpiecemodelList;
            workpiecemodelList.clear();
            int rowCount=ui->tableWidget_gjmodel->rowCount();
            for(int i=0;i<rowCount;i++)
            {

            workpiecemodelList.append(ui->tableWidget_gjmodel->item(i,1)->text());

            }
            ui->comboBox_BanjinModel->clear();
            for(int i=0;i<workpiecemodelList.size();i++)
            {
            ui->comboBox_BanjinModel->addItem(workpiecemodelList.at(i));
             }
        }


    }





//        QList<QTableWidgetSelectionRange>ranges=ui->tableWidget_gjmodel->selectedRanges();
//        int count=ranges.count();
//        for(int i=0;i<count;i++)
//        {
//           int topRow=ranges.at(i).topRow();
//           int bottomRow=ranges.at(i).bottomRow();
//           for(int j=topRow;j<=bottomRow;j++)
//           {
//             ui->tableWidget_gjmodel->removeRow(j);

//            }
//}
}

void Servo::on_comboBox_BanjinModel_currentIndexChanged(int index)
{
    CURRRENT=index;
    int countX=0;
    int countY=0;
    int countZ=0;
    int countX1=0;
    int countY1=0;
    int countZ1=0;
    QString  workpiece_cfg="";
    QStringList pianyilist;

    QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
    QSqlQuery  workpiecequery;
    workpiecequery.exec(workpiecesql);
    if(workpiecequery.next())
     {
      workpiece_cfg=workpiecequery.value(3).toString();
     }
    QStringList cfg3List=workpiece_cfg.split("|");
    if(index!=-1){
        if(index>cfg3List.size())
        {
            for(int i=0;i<10;i++)
            {


                    banjinActionCheck[i]->setChecked(false);
                    deviationX[countX1]->setText("0");
                    deviationY[countY1]->setText("0");
                    deviationZ[countZ1]->setText("0");

            }
//            for(int i=10;i<20;i++)
//            {
//                deviationX[countX1]->setText("0");
//                countX1++;
//            }
//            for(int i=20;i<30;i++)
//            {


//                countY1++;
//            }

        }
     else{
    pianyilist=cfg3List.at(index).split(",");
    for(int i=0;i<10;i++)
    {
        if(pianyilist.at(i+1)=="1")
        {
            banjinActionCheck[i]->setChecked(true);
        }
        else
        {
            banjinActionCheck[i]->setChecked(false);
        }
    }
    for(int i=11;i<21;i++)
    {
        deviationX[countX]->setText(pianyilist.at(i));
        countX++;
    }
    for(int i=21;i<31;i++)
    {
        deviationY[countY]->setText(pianyilist.at(i));

        countY++;
    }
    if(pianyilist.size()==31)
    {
        for(int i=0;i<10;i++)
        {
         deviationZ[i]->setText("0");
        }
    }
    else{
    for(int i=31;i<41;i++)
    {
        deviationZ[countZ]->setText(pianyilist.at(i));

        countZ++;
    }
    }
        }


}
    else{

        for(int i=0;i<10;i++)
        {


                banjinActionCheck[i]->setChecked(false);
                deviationX[countX1]->setText("0");
                deviationY[countY1]->setText("0");
                deviationZ[countZ1]->setText("0");

        }


    }
}
//钣金数据保存发送
void Servo::on_pushButton_10_clicked()
{
    int rowCount=ui->tableWidget_gjmodel->rowCount();
    if(rowCount==0)
    {
        return;
    }
    //工件偏移量数据
    QString  workpiece_cfg="";
    QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
    QSqlQuery  workpiecequery;
    QString aftervalue;
    QString beforevalue;
    QString value;
    QString pianyiData;
    QStringList workpieceidList;
    workpieceidList.clear();
    //int rowCount=ui->tableWidget_gjmodel->rowCount();
    for(int i=0;i<rowCount;i++)
    {
    workpieceidList.append(ui->tableWidget_gjmodel->item(i,0)->text());
    }
    workpiecequery.exec(workpiecesql);
    if(workpiecequery.next())
     {
      workpiece_cfg=workpiecequery.value(3).toString();
     }
    QStringList pianyiList=workpiece_cfg.split("|");
    for(int i=0;i<CURRRENT;i++)
    {
       beforevalue+=pianyiList.at(i);
       beforevalue+="|";
    }
    for(int i=CURRRENT+1;i<pianyiList.size();i++)
    {
       aftervalue+=pianyiList.at(i);
       aftervalue+="|";
    }
    aftervalue.remove(aftervalue.length()-1,1);
    QString selectIndexDB,pianyiX,pianyiY,pianyiZ;
    int selectNum=0;
   QString data3="??";
    for(int i=0;i<10;i++)
    {
        if(banjinActionCheck[i]->isChecked())
        {

            selectIndexDB+="1,";
            selectNum++;
        }
        else
        {
            selectIndexDB+="0,";
        }

    }
   data3+=QString::number(selectNum);
   data3+=",";
   data3+=workpieceidList.at(CURRRENT);
    for(int i=0;i<10;i++)
    {
        pianyiX+=deviationX[i]->text();
        pianyiX+=",";
        pianyiY+=deviationY[i]->text();
        pianyiY+=",";
        pianyiZ+=deviationZ[i]->text();
        pianyiZ+=",";
        data3+="|[";
        if(!banjinActionCheck[i]->isChecked()){
        data3+=QString("8200000,8200000,8200000");
        }
        else
        {
          data3+=deviationX[i]->text();
          data3+=",";
          data3+=deviationY[i]->text();
          data3+=",";
          data3+=deviationZ[i]->text();
        }
        data3+="]";
    }
     data3.replace(",]","]");
     qDebug()<<data3;
     //pianyiY.remove(pianyiY.length()-1,1);
     pianyiZ.remove(pianyiZ.length()-1,1);
     value=workpieceidList.at(CURRRENT)+","+selectIndexDB+pianyiX+pianyiY+pianyiZ;
     if(pianyiList.size()==CURRRENT+1)
     {

          pianyiData=beforevalue+value;
     }
     else
     {

       pianyiData=beforevalue+value+"|"+aftervalue;
     }

   QSqlQuery query1;
   QString sql1=QString("UPDATE safeArea SET value1='%1' WHERE solutionId='%2' and robotIndex=102").arg(pianyiData).arg(cur_solutionId);
   qDebug()<<"sql:"<<sql1<<query1.exec(sql1);




   QByteArray b1=data3.toLocal8Bit();
   b1[0]=CMD_WRITE_VAR_PUNCH_PARAMS;
   b1[1]=1;

   mainWindow->mysendService(b1,1);
   //ui->label_info->setText("发送成功");

   //保存最后选取的工件型号
   QSqlQuery query;
   QString sql=QString("select * from safeArea where solutionId ='%1' and robotIndex=120").arg(cur_solutionId);
   query.exec(sql);
   if(query.next())
   {
       QString lastwork=QString::number(CURRRENT);
       QString lastaction=QString::number(ui->comboBox_pianyiX->currentIndex());
       QString sql1=QString("UPDATE safeArea SET value1='%1', value2='%2' WHERE solutionId='%3' and robotIndex=120").arg(lastwork).arg(lastaction).arg(cur_solutionId);
       query.exec(sql1);
   }
//   else
//   {

//       qDebug()<<"servo::create safeArea 120"<< query.exec(QString("INSERT INTO safeArea(id,solutionId,robotIndex,value1,value2) VALUES ('%1','%2',102,'','')").arg(SysUtils::uuid()).arg(cur_solutionId));
//   }


}
//确认添加
//void Servo::on_pushButton_7_clicked()
//{

//    QTableWidget *tableWidget =ui->tableWidget_gjmodel;
//    int rowCount=tableWidget->rowCount();
//    QString intdata="0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
//    //工件偏移量数据
//    QString  workpiece_cfg="";
//    QString workpiecesql=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
//    QSqlQuery  workpiecequery;
//    workpiecequery.exec(workpiecesql);
//    if(workpiecequery.next())
//     {
//      workpiece_cfg=workpiecequery.value(3).toString();
//     }
//    //工件型号数据
//    QString  workpiece_cfg1="";
//    QString workpiecesql1=QString("select * from safeArea where solutionId ='%1' and robotIndex=102").arg(cur_solutionId);
//    QSqlQuery  workpiecequery1;
//    workpiecequery1.exec(workpiecesql1);
//    if(workpiecequery1.next())
//     {
//      workpiece_cfg1=workpiecequery1.value(4).toString();
//     }
//    //获取数据库工件型号Id
//    QStringList workpieceidList,workpiecemodelList;
//    workpieceidList.clear();
//    workpiecemodelList.clear();
//    for(int i=0;i<rowCount-1;i++)
//    {
//    workpieceidList.append(tableWidget->item(i,0)->text());
//    workpiecemodelList.append(tableWidget->item(i,1)->text());

//    }

//    if(workpiece_cfg1!="")
//        {
//        workpiece_cfg1+="|";
//    }
//    workpiece_cfg1+="NULL";
//    workpiece_cfg1+=",";
//    workpiece_cfg1+="NULL";
//    if(workpiece_cfg!="")
//        {
//        workpiece_cfg+="|";
//    }
//    workpiece_cfg+="NULL";
//    workpiece_cfg+=",";
//    workpiece_cfg+=intdata;
//    workpiecemodelList.append("NULL");
//    QSqlQuery query1;
//    QString sql1=QString("UPDATE safeArea SET value1='%1' , value2='%2' WHERE solutionId='%3' and robotIndex=102").arg(workpiece_cfg).arg(workpiece_cfg1).arg(cur_solutionId);

//    qDebug()<<"sql:"<<sql1<<query1.exec(sql1);
//    ui->comboBox_BanjinModel->clear();
//    for(int i=0;i<workpiecemodelList.size();i++)
//    {
//    ui->comboBox_BanjinModel->addItem(workpiecemodelList.at(i));
//     }



//}


void Servo::on_btnpyx_add_big_clicked()
{

    double d=deviationX[ACTION]->text().toDouble();
    deviationX[ACTION]->setText(QString::number(d+1));
}

void Servo::on_btnpyx_add_small_clicked()
{
    double d=deviationX[ACTION]->text().toDouble();
    deviationX[ACTION]->setText(QString::number(d+0.1));
}

void Servo::on_btnpyx_reduce_big_clicked()
{
    double d=deviationX[ACTION]->text().toDouble();
    deviationX[ACTION]->setText(QString::number(d-1));
}

void Servo::on_btnpyx_reduce_small_clicked()
{
    double d=deviationX[ACTION]->text().toDouble();
    deviationX[ACTION]->setText(QString::number(d-0.1));
}

void Servo::on_btnpyy_add_big_clicked()
{
    double d=deviationY[ACTION]->text().toDouble();
    deviationY[ACTION]->setText(QString::number(d+1));
}

void Servo::on_btnpyy_add_small_clicked()
{
    double d=deviationY[ACTION]->text().toDouble();
    deviationY[ACTION]->setText(QString::number(d+0.1));
}

void Servo::on_btnpyy_reduce_big_clicked()
{
    double d=deviationY[ACTION]->text().toDouble();
    deviationY[ACTION]->setText(QString::number(d-1));
}

void Servo::on_btnpyy_reduce_small_clicked()
{
    double d=deviationY[ACTION]->text().toDouble();
    deviationY[ACTION]->setText(QString::number(d-0.1));
}

void Servo::on_btnpyz_add_big_clicked()
{
    double d=deviationZ[ACTION]->text().toDouble();
    deviationZ[ACTION]->setText(QString::number(d+1));
}

void Servo::on_btnpyz_add_small_clicked()
{
    double d=deviationZ[ACTION]->text().toDouble();
    deviationZ[ACTION]->setText(QString::number(d+0.1));
}

void Servo::on_btnpyz_reduce_big_clicked()
{
    double d=deviationZ[ACTION]->text().toDouble();
    deviationZ[ACTION]->setText(QString::number(d-1));
}

void Servo::on_btnpyz_reduce_small_clicked()
{
    double d=deviationZ[ACTION]->text().toDouble();
    deviationZ[ACTION]->setText(QString::number(d-0.1));
}

void Servo::on_comboBox_pianyiX_currentIndexChanged(int index)
{
    ACTION=index;
}




void Servo::on_tableWidget_gjmodel_itemDoubleClicked(QTableWidgetItem *item)
{
    CURRENT_MODEL=ui->tableWidget_gjmodel->currentRow();
    QList<QTableWidgetItem*>items=ui->tableWidget_gjmodel->selectedItems();

    int count=items.count();

    for(int i=0;i<count;i++)

        {


           QString id=items.at(0)->text();
           QString model=items.at(1)->text();
           ui->lineEdit_gjid->setText(id);
           ui->lineEdit_gjmodel->setText(model);

        }
    qDebug()<<CURRENT_MODEL;
}
QStringList Servo::servoNameList()
{
    QStringList tabnameList;
    for(int i=0;i<ui->tabWidget->count();i++)
    {
        tabnameList.append(ui->tabWidget->tabText(i));
    }
 return tabnameList;
}
void Servo::tabwidget_show(int index)
{
    ui->tabWidget->setTabEnabled(index, true);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
}
void Servo::tabwidget_hide(int index)
{
    ui->tabWidget->setTabEnabled(index, false);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
}
int Servo::firsttabshow()
{
    QSqlQuery servoquery;
    QString servosql=QString("select * from SysParam where code ='%1' ").arg("servoname");
    servoquery.exec(servosql);
    QString  servoData="";
    if(servoquery.next())
    {
         servoData=servoquery.value(1).toString();
         QStringList servoList=servoData.split(",");
         for(int i=0;i<servoList.size();i++){
             if(servoList.at(i)=="0")
             {
               continue;
             }
             else
             {
                 return i;
                 break;
             }
         }


    }
    else
    {
        return 0;
    }
}



