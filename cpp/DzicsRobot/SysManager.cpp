#include "SysManager.h"
#include "ui_SysManager.h"
#include "SysUtils.h"
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>
#include <QTableWidget>
#include <QComboBox>
#include "Global.h"

SysManager *g_sysManager=NULL;
QCheckBox *servoNameCheck[20];
SysManager::SysManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysManager)
{
    qDebug()<<"sysManager";
    ui->setupUi(this);

    rType.append("int");
    rType.append("bool");
    cur_solutionId=SysUtils::getSysParam( "lastStationId" ).toString();

    initData();
    updateUi();
    g_sysManager = this;
    connect(ui->tableWidget_2, SIGNAL(cellChanged(int, int)),
            this, SLOT(configurationPropertyChanged(int, int)));

}

SysManager::~SysManager()
{
    delete ui;
}

void SysManager::mouseMoveEvent(QMouseEvent *)
{
    ui->showInfo->setText("");
}




void SysManager::initUi()
{

    //ui->tableWidget->setColumnHidden(0,true);

    for(int i=0;i<14;i++)
    {
        ui->tableWidget->setColumnWidth(i,80);

    }
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,150);
    ui->tableWidget->setColumnWidth(3,150);
    ui->tableWidget->setColumnWidth(6,100);


    ui->tableWidget_2->setColumnHidden(0, 1);
    //ui->tableWidget_2->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget_2->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget_2->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行

    ui->tableWidget_2->setColumnWidth(3, 350);
    ui->tableWidget_2->setColumnWidth(4, 350);
    QStringList servoNameList=g_servo->servoNameList();
    QSqlQuery query;
    QString sql=QString("select * from SysParam where code ='%1' ").arg("servoname");
    query.exec(sql);
    QString  servoData="";
    if(query.next())
    {
         servoData=query.value(1).toString();
    }

    QString servoiniData="";
    if(servoData=="")
    {

        for(int i=0;i<servoNameList.size();i++)
        {
            servoiniData+="1,";
        }
        servoiniData.remove(servoiniData.length()-1,1);
        qDebug()<<servoiniData+"zzzzzzzzzzzzzzzzzzzzzz";
        sql=QString("INSERT INTO SysParam(code,value) VALUES ('%1','%2')").arg("servoname").arg(servoiniData);
        query.exec(sql);
    }
    QStringList servoList;
     if(servoData==""){
    servoList=servoiniData.split(",");
     }
     else
     {
        servoList=servoData.split(",");
     }
    for(int i=0;i<servoNameList.size();i++)
    {
        QFont list_font1;
        list_font1.setPointSize(14);
        list_font1.setBold(false);
        //list_font1.setWeight(15);
//        ServoName[i]=new QLabel(servoNameList.at(i),ui->widget_gongneng);
//        ServoName[i]->setFont(list_font1);
//        ServoName[i]->setAlignment(Qt::AlignRight);
//        ServoName[i]->setGeometry(-25,150+100*i,200,200);
//        ServoName[i]->move(100,100+100*i);
        servoNameCheck[i]=new QCheckBox(ui->widget_gongneng);
        servoNameCheck[i]->setText(QString(servoNameList.at(i)));
        servoNameCheck[i]->setGeometry(80,50+100*i,200,200);
        servoNameCheck[i]->setFont(list_font1);
        servoNameCheck[i]->setObjectName(QString::number(i));
        if(servoList.at(i)=="1")
        {
            servoNameCheck[i]->setChecked(true);
        }
        else
        {
            servoNameCheck[i]->setChecked(false);
        }
        connect(servoNameCheck[i],SIGNAL(clicked()),this,SLOT(clickSlot()));



    }
    //用户名密码框数据
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
        ui->lineEdit_glpassword->setText("");
        ui->lineEdit_ygpassword->setText("");
    }
    else
    {

        QStringList password=passwordData.split("|");
        ui->lineEdit_glpassword->setText(password[0]);
        ui->lineEdit_ygpassword->setText(password[1]);

    }


}
void SysManager::sysManagerDataUpdate()
{
    //stationID 更新后，重新读取stationID相关数据并显示
    qDebug()<<"call sysManagerDataUpdate";
    readstationConfigurationInfo();
    initData();
    initUi();

}

void SysManager::readstationConfigurationInfo()
{
//    if(!GetDbConnect())
//        return;

    for(int i = stationConfigurationList.size()-1; i>=0; i--)
    {
        ui->tableWidget_2->removeRow(i);
    }
    stationConfigurationList.clear();

    if(!queryStationConfigurationData())
        return;

    for(int i = 0; i< stationConfigurationList.size(); i++)
    {

        ui->tableWidget_2->insertRow(i);
        ui->tableWidget_2->setRowHeight(i,60);

        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(stationConfigurationList.at(i).id));
        ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(stationConfigurationList.at(i).name));

        QComboBox *comBox1=new QComboBox;
        comBox1->addItem("bool");
        comBox1->addItem("num");
        comBox1->addItem("robtarget");
        comBox1->addItem("tooldata");
        comBox1->addItem("wobjdata");
        comBox1->setCurrentIndex(stationConfigurationList.at(i).classIndex);
        ui->tableWidget_2->setCellWidget(i,2,comBox1);
        connect(comBox1,SIGNAL(currentIndexChanged(int)),this,SLOT(dataTypechangedSlot(int)));

        ui->tableWidget_2->setItem(i,3,new QTableWidgetItem(stationConfigurationList.at(i).value1));
        ui->tableWidget_2->setItem(i,4,new QTableWidgetItem(stationConfigurationList.at(i).value2));
    }
}

bool SysManager::queryStationConfigurationData()
{
    StationConfiguration aStationConfiguration;

    //Knife2List.clear();


    QSqlQuery query;
    bool ret = false;
    ret = query.exec(QString("alter table StationModule add yaw double"));

    ret = false;
    ret = query.exec(QString("SELECT name FROM sqlite_master  where type='table' and name='StationConfiguration'"));

    if(!query.next())
    {
       qDebug()<<"createTable start";
       ret = false;
       ret = query.exec("create table StationConfiguration (id varchar(50) not null primary key, stationId varchar(50), name  varchar(50), classIndex int, value1  varchar(512), value2  varchar(512), value3  varchar(512), value4  varchar(512), value5  varchar(512))");
       if(ret == true)
       {
           qDebug()<<"createTable success";
       }else
       {
           return false;
       }
    }

    if(pageStation == NULL)
    {
        return false;
    }
    if(!query.exec(QString("select * from StationConfiguration where stationId = '%1'").arg(pageStation->currentStationId)))
    {
        return false;
    }


    while(query.next())
    {

        aStationConfiguration.id = query.value(0).toString();
        aStationConfiguration.stationId = query.value(1).toString();
        aStationConfiguration.name = query.value(2).toString();
        aStationConfiguration.classIndex = query.value(3).toInt();
        aStationConfiguration.value1 = query.value(4).toString();
        aStationConfiguration.value2 = query.value(5).toString();
        aStationConfiguration.value3 = query.value(6).toString();
        aStationConfiguration.value4 = query.value(7).toString();
        aStationConfiguration.value5 = query.value(8).toString();

        stationConfigurationList.append(aStationConfiguration);

    }

    return true;
}
//ronglk 更新到最新版本数据库
bool SysManager::initData_2() {
    QSqlQuery query;
    uint dbVersion = SysUtils::getSysParam("dbVersion").toUInt();
    if (dbVersion < 13) {
        QSqlDatabase db = QSqlDatabase::database();
        db.transaction();
        if (!SysUtils::setSysParam("dbVersion", "13")) {
            db.rollback();
            return false;
        }
        QSqlQuery query;
        bool ret = false;
        ret = query.exec(QString("alter table StationModule add yaw double"));
        ret = query.exec(QString("alter table StationModule add pitch double"));
        ret = query.exec(QString("alter table StationModule add roll double"));
        ret = query.exec("create table StationConfiguration (id varchar(50) not null primary key, stationId varchar(50),name  varchar(50), classIndex int, value1  varchar(512), value2  varchar(512), value3  varchar(512), value4  varchar(512), value5  varchar(512)");
        if(!ret)
        {
            db.rollback();
            return false;
        }
        db.commit();
    }
}

void SysManager::updateUi()
{
    cur_solutionId=SysUtils::getSysParam( "lastStationId" ).toString();
    //ui->tableWidget->setColumnHidden(0,true);

    for(int i = ui->tableWidget->rowCount()-1; i>=0; i--)
    {
        ui->tableWidget->removeRow(i);
    }



    QStringList list=data.split("|");
    QStringList row;
    for(int i = 0; i<list.size(); i++)
    {

        row=list[i].split(",");


        while(row.size()<12)
            row.append("");
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setRowHeight(i,60);
        QCheckBox *check=new QCheckBox;
        check->setChecked(row[0].toInt());


        QPushButton *btn1=new QPushButton("上移");
        QPushButton *btn2=new QPushButton("下移");


        btn1->setObjectName(QString::number(i));
        btn2->setObjectName(QString::number(1000+i));

        connect(btn1,SIGNAL(clicked()),this,SLOT(moveData()));
        connect(btn2,SIGNAL(clicked()),this,SLOT(moveData()));

        QComboBox *comBox1=new QComboBox;
        comBox1->addItem("左侧文字");
        comBox1->addItem("右侧文字");
        comBox1->setCurrentIndex(row[4].toInt());

        QCheckBox* robot1=new QCheckBox;
        robot1->setChecked(row[5].toInt());
        QCheckBox* robot2=new QCheckBox;
        robot2->setChecked(row[6].toInt());
        QCheckBox* robot3=new QCheckBox;
        robot3->setChecked(row[7].toInt());
        QCheckBox* robot4=new QCheckBox;
        robot4->setChecked(row[8].toInt());
        QCheckBox* robot5=new QCheckBox;
        robot5->setChecked(row[9].toInt());

        QComboBox *comBox2=new QComboBox;
        comBox2->addItem("int");
        comBox2->addItem("bool");
        comBox2->setCurrentIndex(rType.indexOf(row[10]));

        //ui->tableWidget->setItem(i,0,new QTableWidgetItem(SysUtils::uuid()));

        ui->tableWidget->setCellWidget(i,0,check);
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(row[1]));
        if(row[1]=="当前速度"||row[1]=="送料节拍")
        {
        Qt::ItemFlags flag=  ui->tableWidget->item(i,1)->flags();
        ui->tableWidget->item(i,1)->setFlags(flag|Qt::NoItemFlags);
        qDebug()<<"flag"<<flag;
        }
        if(row[1]=="当前速度"||row[1]=="送料节拍")
        {
            ui->tableWidget->item(i,1)->setFlags(Qt::ItemIsEditable);
           Qt::ItemFlags flag=  ui->tableWidget->item(i,1)->flags();
            qDebug()<<"flag after"<<flag;
        }
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(row[2]));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(row[3]));
        ui->tableWidget->setCellWidget(i,4,btn1);
        ui->tableWidget->setCellWidget(i,5,btn2);
        ui->tableWidget->setCellWidget(i,6,comBox1);
        ui->tableWidget->setCellWidget(i,7,robot1);
        ui->tableWidget->setCellWidget(i,8,robot2);
        ui->tableWidget->setCellWidget(i,9,robot3);
        ui->tableWidget->setCellWidget(i,10,robot4);
        ui->tableWidget->setCellWidget(i,11,robot5);

        ui->tableWidget->setCellWidget(i,12,comBox2);
        ui->tableWidget->setItem(i,13,new QTableWidgetItem(row[11]));
//        ui->tableWidget->item(i,3)->setData(Qt::DisplayRole,QVariant(CutInfoList.at(i).CutTotNum).toInt());
//        ui->tableWidget->item(i,4)->setData(Qt::DisplayRole | Qt::EditRole,CutInfoList.at(i).CutTotNum-CutInfoList.at(i).CutUseNum);
//        ui->tableWidget->item(i,4)->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable| Qt:: ItemIsEnabled);
//        for(int cout=0; cout<8; cout++)
//         ui->tableWidget->item(i,cout)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}
//end

void SysManager::initData()
{
   cur_solutionId=SysUtils::getSysParam( "lastStationId" ).toString();
   QSqlQuery query;
   QString sql=QString("select * from safeArea where solutionId ='%1' and robotIndex=105").arg(cur_solutionId);

   query.exec(sql);
   QVariant var=NULL;


   if(query.next())
   {
        var=query.value(3).toString();
   }


   if(var==NULL)
   {

       data="1,送料节拍,ms,gg,0,0,0,0,0,0,int,0|1,名称,ms,aaa,0,1,1,1,1,1,int,0|0,hello,ms,c,0,0,0,0,0,1,int,asf|1,当前速度,%,bb,0,1,0,0,1,1,int,0";
       sql=QString("INSERT INTO safeArea(id,solutionId,robotIndex,value1,value2) VALUES ('%1','%2',105,'%3','')").arg(SysUtils::uuid()).arg(cur_solutionId).arg(data);
       query.exec(sql);
   }
   else
   {
       data=var.toString();

   }


   QVariant f9Set=SysUtils::getSysParam("f9_Set");




   if(f9Set==NULL)
   {

       SysUtils::addSysParam("f9_Set","1");
       f9Index="1";

   }
   else
   {

       f9Index=f9Set.toString();

   }

   if(f9Index.toInt()<4)
   {
       ui->comboBox_f9->setCurrentIndex(f9Index.toInt()-1);
   }

   if(SysUtils::getSysParam("dbVersion").toInt()<14)
   {

       QSqlQuery query;
       query.exec(QString("alter table CameraProperty add picNum"));
       query.exec(QString("alter table CameraProperty add interval"));
       SysUtils::setSysParam("dbVersion","14");
   }



   initData_2();

}

void SysManager::update()
{
    QString data1;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        qDebug()<<"aaa"<<i;
        QCheckBox *check=(QCheckBox *)ui->tableWidget->cellWidget(i,0);
        if(check->isChecked())
            data1+="1";
        else
            data1+="0";
        qDebug()<<"bbb"<<i;
        data1+=",";
        data1+=ui->tableWidget->item(i,1)->text();
        data1+=",";
        data1+=ui->tableWidget->item(i,2)->text();
        data1+=",";
        data1+=ui->tableWidget->item(i,3)->text();
        data1+=",";
        QComboBox *combo=(QComboBox *)ui->tableWidget->cellWidget(i,6);
        QComboBox *combo2=(QComboBox *)ui->tableWidget->cellWidget(i,12);
        data1+=QString::number(combo->currentIndex());
        qDebug()<<"ccc"<<i;
        data1+=((QCheckBox *)ui->tableWidget->cellWidget(i,7))->isChecked()?",1":",0";
        data1+=((QCheckBox *)ui->tableWidget->cellWidget(i,8))->isChecked()?",1":",0";
        data1+=((QCheckBox *)ui->tableWidget->cellWidget(i,9))->isChecked()?",1":",0";
        data1+=((QCheckBox *)ui->tableWidget->cellWidget(i,10))->isChecked()?",1":",0";
        data1+=((QCheckBox *)ui->tableWidget->cellWidget(i,11))->isChecked()?",1":",0";
        data1+=",";
        data1+=rType[combo2->currentIndex()];
        data1+=",";
        data1+=ui->tableWidget->item(i,13)->text();
        if(i<ui->tableWidget->rowCount()-1)
            data1+="|";
    }

   // SysUtils::setSysParam("leftText",data1);
    QSqlQuery query;
    QString sql=QString("UPDATE safeArea SET value1='%1' WHERE solutionId='%2' and robotIndex=105").arg(data1).arg(cur_solutionId);
    data=data1;



    query.exec(sql);

    register_CMD_COMMON_TRIGGER();
}

void SysManager::insertNullData()
{

}





void SysManager::on_pushButton_clicked()
{
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setRowHeight(0,60);
    QComboBox *comBox1=new QComboBox;
    comBox1->addItem("左侧文字");
    comBox1->addItem("右侧文字");
    comBox1->setCurrentIndex(0);
    QPushButton *btn1=new QPushButton("上移");
    QPushButton *btn2=new QPushButton("下移");


    btn1->setObjectName(QString::number(0));
    btn2->setObjectName(QString::number(1000));

    connect(btn1,SIGNAL(clicked()),this,SLOT(moveData()));
    connect(btn2,SIGNAL(clicked()),this,SLOT(moveData()));

    QCheckBox* robot1=new QCheckBox;
    QCheckBox* robot2=new QCheckBox;
    QCheckBox* robot3=new QCheckBox;
    QCheckBox* robot4=new QCheckBox;
    QCheckBox* robot5=new QCheckBox;

    QComboBox *comBox2=new QComboBox;
    comBox2->addItem("int");
    comBox2->addItem("bool");


    ui->tableWidget->setCellWidget(0,0,new QCheckBox);
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("名称"));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem("ms"));
    ui->tableWidget->setItem(0,3,new QTableWidgetItem("aaa"));
    ui->tableWidget->setCellWidget(0,4,btn1);
    ui->tableWidget->setCellWidget(0,5,btn2);
    ui->tableWidget->setCellWidget(0,6,comBox1);
    ui->tableWidget->setCellWidget(0,7,robot1);
    ui->tableWidget->setCellWidget(0,8,robot2);
    ui->tableWidget->setCellWidget(0,9,robot3);
    ui->tableWidget->setCellWidget(0,10,robot4);
    ui->tableWidget->setCellWidget(0,11,robot5);
    ui->tableWidget->setCellWidget(0,12,comBox2);
    ui->tableWidget->setItem(0,13,new QTableWidgetItem(""));


    setBtn();



}

void SysManager::on_pushButton_2_clicked()
{
    int cur=ui->tableWidget->currentRow();
    if(cur==-1)
        return;
    ui->tableWidget->removeRow(cur);
    setBtn();
    update();
    mainWindow->showIndexText();

}

void SysManager::on_pushButton_3_clicked()
{
    update();

    ui->showInfo->setText("保存成功");
    mainWindow->showIndexText();
}

bool SysManager::validateData(const QString &str, int row, int column)
{
    int nCount = str.count();
    if(nCount<=0) return false;
    for(int i = 0 ; i < nCount ; i++)
    {
        QChar cha = str.at(i);
        ushort uni = cha.unicode();
        if(uni >= 0x4E00 && uni <= 0x9FA5)
        {
            ui->tableWidget_2->setItem(row, column,new QTableWidgetItem(""));
            QMessageBox::warning(this,tr("系统提示"),tr("禁止输入中文"), tr("确定"));
            return false;
        }
    }
    if(column == 3)
    {
        QComboBox *comBox=(QComboBox*)ui->tableWidget_2->cellWidget(row,2);

        int value = comBox->currentIndex();

        if(value==BOOL_TYPE)
        {
            if(str=="true"||str=="false"||str=="TRUE"||str=="FALSE")
            {

            }else
            {
                {
                    ui->tableWidget_2->setItem(row, column,new QTableWidgetItem("false"));
                    QMessageBox::warning(this,tr("系统提示"),tr("布尔量只能true或false"), tr("确定"));
                    return false;
                }

            }
        }else if(value==NUM_TYPE)
        {
            //只能数字
        }
    }
    return true;
}

void SysManager::configurationPropertyChanged(int row, int column)
{
    QSqlQuery query;


    QString data1;
    QString data2;

    data1 = ui->tableWidget_2->item(row,column)->text();
    data2 = ui->tableWidget_2->item(row,0)->text();

    qDebug()<<"configurationPropertyChanged"<<data1<<data2<<row<<column;
    switch(column){
        case 1  :
            if(!validateData(data1, row, column)) return;
            if(query.exec(QString("UPDATE StationConfiguration SET name='%1' WHERE id='%2'").arg(data1, data2)));
                qDebug()<<QString("UPDATE StationConfiguration SET name='%1' WHERE id='%2'").arg(data1, data2);
           break;
        case 2  :
            if(query.exec(QString("UPDATE StationConfiguration SET classIndex='%1' WHERE id='%2'").arg(data1, data2)));
                qDebug()<<QString("UPDATE StationConfiguration SET classIndex='%1' WHERE id='%2'").arg(data1, data2);
           break;
        case 3  :
            if(!validateData(data1, row, column)) return;
            if(query.exec(QString("UPDATE StationConfiguration SET value1 ='%1' WHERE id='%2'").arg(data1, data2)));
                qDebug()<<QString("UPDATE StationConfiguration SET value1 ='%1' WHERE id='%2'").arg(data1, data2);
           break;
        case 4  :
            if(query.exec(QString("UPDATE StationConfiguration SET value2 ='%1' WHERE id='%2'").arg(data1, data2)));
                qDebug()<<QString("UPDATE StationConfiguration SET value2 ='%1' WHERE id='%2'").arg(data1, data2);
           break;
    }
}



void SysManager::dataTypechangedSlot(int index)
{

   int row= ui->tableWidget_2->currentRow();
   if(row<0) return;

   QString priKey = ui->tableWidget_2->item(row,0)->text();
   QComboBox *comBox=(QComboBox*)ui->tableWidget_2->cellWidget(row,2);

   int value = comBox->currentIndex();

   QSqlQuery query;
   query.exec(QString("UPDATE StationConfiguration set classIndex = '%1' where id = '%2'").arg(value).arg(priKey));

}

void SysManager::register_CMD_COMMON_TRIGGER()
{
    QStringList list=data.split("|");
    QString str1="??";
    QString str2="??";
    QString str3="??";
    for(int i=0;i<list.size();i++)
    {
        QStringList list2=list[i].split(",");
        if(list2.size()<12)
            break;
        if(list2[0]=="0")
            continue;

        if(list2[5]=="1")
        {
            str1+=list2[3]+","+list2[10]+","+list2[11]+"|";
        }else if(list2[6]=="1"){
            str2+=list2[3]+","+list2[10]+","+list2[11]+"|";
        }else if(list2[7]=="1"){
            str3+=list2[3]+","+list2[10]+","+list2[11]+"|";
        }

    }
    str1.remove(str1.length()-1,1);
    str2.remove(str2.length()-1,1);
    str3.remove(str3.length()-1,1);
//    qDebug()<<"str1"<<str1;
//    qDebug()<<"str2"<<str2;
//    qDebug()<<"str3"<<str3;
//    qDebug()<<"data"<<data;

    QByteArray r1=str1.toLocal8Bit();
    QByteArray r2=str2.toLocal8Bit();
    QByteArray r3=str3.toLocal8Bit();

    r1[0]=CMD_COMMON_TRIGGER;
    r2[0]=CMD_COMMON_TRIGGER;
    r3[0]=CMD_COMMON_TRIGGER;

    r1[1]=1;
    r2[1]=2;
    r3[1]=3;

    mainWindow->mysendService(r1,1);
    mainWindow->mysendService(r2,2);
    mainWindow->mysendService(r3,3);
}
void SysManager::moveData()
{
    QObject *object = QObject::sender();
    QPushButton *push_button = qobject_cast<QPushButton *>(object);

    int index = push_button->objectName().toInt();
    QModelIndex index2 = ui->tableWidget->indexAt(push_button->pos());
    ui->tableWidget->selectRow(index2.row());

    bool toDown=false;
    if(index<1000)
    {
        if(index==0)
            return;
    }
    if(index>999)
    {
        toDown=true;
        index-=1000;

        if(index==ui->tableWidget->rowCount()-1)
            return;
    }

    int r1,r2;
    if(toDown)
    {
        r1=index;
        r2=index+1;
    }
    else
    {
        r1=index;
        r2=index-1;
    }
    swapRow(r1,r2);

}

void SysManager::swapRow(int row1, int row2)
{


    QString s1=ui->tableWidget->item(row1,1)->text();
    QString s2=ui->tableWidget->item(row2,1)->text();
    QString s3=ui->tableWidget->item(row1,2)->text();
    QString s4=ui->tableWidget->item(row2,2)->text();
    QString s5=ui->tableWidget->item(row1,3)->text();
    QString s6=ui->tableWidget->item(row2,3)->text();
    QString s7=ui->tableWidget->item(row1,13)->text();
    QString s8=ui->tableWidget->item(row2,13)->text();
    bool b1=((QCheckBox *) ui->tableWidget->cellWidget(row1,0))->isChecked();
    bool b2=((QCheckBox *) ui->tableWidget->cellWidget(row2,0))->isChecked();

    int n1=((QComboBox *)ui->tableWidget->cellWidget(row1,6))->currentIndex();
    int n2=((QComboBox *)ui->tableWidget->cellWidget(row2,6))->currentIndex();
    int n3=((QComboBox *)ui->tableWidget->cellWidget(row1,12))->currentIndex();
    int n4=((QComboBox *)ui->tableWidget->cellWidget(row2,12))->currentIndex();
    ((QCheckBox *)ui->tableWidget->cellWidget(row1,0))->setChecked(b2);
    ((QCheckBox *)ui->tableWidget->cellWidget(row2,0))->setChecked(b1);

    ui->tableWidget->item(row1,1)->setText(s2);
    ui->tableWidget->item(row2,1)->setText(s1);
    if(s1=="当前速度"||s1=="送料节拍")
    {
        ui->tableWidget->item(row1,1)->setFlags((Qt::ItemFlags)63);
        ui->tableWidget->item(row2,1)->setFlags(Qt::ItemIsEditable);
    }
    if(s2=="当前速度"||s2=="送料节拍")
    {
        ui->tableWidget->item(row1,1)->setFlags(Qt::ItemIsEditable);
        ui->tableWidget->item(row2,1)->setFlags((Qt::ItemFlags)63);
    }

    ui->tableWidget->item(row1,2)->setText(s4);
    ui->tableWidget->item(row2,2)->setText(s3);
    ui->tableWidget->item(row1,3)->setText(s6);
    ui->tableWidget->item(row2,3)->setText(s5);
    ((QComboBox *)ui->tableWidget->cellWidget(row1,6))->setCurrentIndex(n2);
    ((QComboBox *)ui->tableWidget->cellWidget(row2,6))->setCurrentIndex(n1);


    bool r1=((QCheckBox *) ui->tableWidget->cellWidget(row1,7))->isChecked();
    bool r2=((QCheckBox *) ui->tableWidget->cellWidget(row2,7))->isChecked();
    bool r3=((QCheckBox *) ui->tableWidget->cellWidget(row1,8))->isChecked();
    bool r4=((QCheckBox *) ui->tableWidget->cellWidget(row2,8))->isChecked();
    bool r5=((QCheckBox *) ui->tableWidget->cellWidget(row1,9))->isChecked();
    bool r6=((QCheckBox *) ui->tableWidget->cellWidget(row2,9))->isChecked();
    bool r7=((QCheckBox *) ui->tableWidget->cellWidget(row1,10))->isChecked();
    bool r8=((QCheckBox *) ui->tableWidget->cellWidget(row2,10))->isChecked();
    bool r9=((QCheckBox *) ui->tableWidget->cellWidget(row1,11))->isChecked();
    bool r10=((QCheckBox *) ui->tableWidget->cellWidget(row2,11))->isChecked();

    ((QCheckBox *)ui->tableWidget->cellWidget(row1,7))->setChecked(r2);
    ((QCheckBox *)ui->tableWidget->cellWidget(row2,7))->setChecked(r1);
    ((QCheckBox *)ui->tableWidget->cellWidget(row1,8))->setChecked(r4);
    ((QCheckBox *)ui->tableWidget->cellWidget(row2,8))->setChecked(r3);
    ((QCheckBox *)ui->tableWidget->cellWidget(row1,9))->setChecked(r6);
    ((QCheckBox *)ui->tableWidget->cellWidget(row2,9))->setChecked(r5);
    ((QCheckBox *)ui->tableWidget->cellWidget(row1,10))->setChecked(r8);
    ((QCheckBox *)ui->tableWidget->cellWidget(row2,10))->setChecked(r7);
    ((QCheckBox *)ui->tableWidget->cellWidget(row1,11))->setChecked(r10);
    ((QCheckBox *)ui->tableWidget->cellWidget(row2,11))->setChecked(r9);

    ((QComboBox *)ui->tableWidget->cellWidget(row1,12))->setCurrentIndex(n4);
    ((QComboBox *)ui->tableWidget->cellWidget(row2,12))->setCurrentIndex(n3);
    ui->tableWidget->item(row1,13)->setText(s8);
    ui->tableWidget->item(row2,13)->setText(s7);
}

void SysManager::setBtn()
{
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        ((QPushButton *) ui->tableWidget->cellWidget(i,4))->setObjectName(QString::number(i));
        ((QPushButton *) ui->tableWidget->cellWidget(i,5))->setObjectName(QString::number(i+1000));
    }
}
//增加一行配置信息
void SysManager::on_pushButton_4_clicked()
{
    disconnect(ui->tableWidget_2,SIGNAL(cellChanged(int,int)),this,SLOT(configurationPropertyChanged(int,int)));

    ui->tableWidget_2->insertRow(0);
    ui->tableWidget_2->setRowHeight(0,60);

    ui->tableWidget_2->setColumnWidth(3, 350);
    ui->tableWidget_2->setColumnWidth(4, 350);

    QString id = SysUtils::uuid();
    QComboBox *comBox1=new QComboBox;
    comBox1->addItem("bool");
    comBox1->addItem("num");
    comBox1->addItem("robtarget");
    comBox1->addItem("tooltarget");
    comBox1->addItem("wobjdata");

    comBox1->setCurrentIndex(0);

    ui->tableWidget_2->setItem(0,0,new QTableWidgetItem(id));

    ui->tableWidget_2->setItem(0,1,new QTableWidgetItem("name"));

    ui->tableWidget_2->setCellWidget(0,2,comBox1);
    connect(comBox1,SIGNAL(currentIndexChanged(int)),this,SLOT(dataTypechangedSlot(int)));

    ui->tableWidget_2->setItem(0,3,new QTableWidgetItem(""));
    ui->tableWidget_2->setItem(0,4,new QTableWidgetItem(""));
    ui->tableWidget_2->selectRow(0);
    //增加到数据库
    QString stationId = pageStation->currentStationId;
    QSqlQuery query;

    if(!query.exec(QString("INSERT INTO StationConfiguration (id, stationId, name, classIndex, value1, value2, value3, value4, value5) VALUES('%1','%2','name',0,'','','','','')").arg(id, stationId)));
        qDebug()<<QString("INSERT INTO StationConfiguration (id, stationId, name, classIndex, value1, value2, value3, value4, value5) VALUES('%1','%2','name',0,'','','','','')").arg(id, stationId);
    //ui->tableWidget->setItem(0,4,new QTableWidgetItem("1"));
    //ui->tableWidget->setCellWidget(0,5,comBox1);

    connect(ui->tableWidget_2,SIGNAL(cellChanged(int,int)),this,SLOT(configurationPropertyChanged(int,int)));
}

bool SysManager::deleteStationConfigurationData(QString &id)
{

    QSqlQuery quer;
    QString sql = QString("Delete From StationConfiguration where id='%1'").arg(id);

    if(!quer.exec(sql))
    {
        return false;
    }
    else
    {
        return true;
    }
}
//删除一行配置信息
void SysManager::on_pushButton_5_clicked()
{
    int row = ui->tableWidget_2->currentRow();
    if(row < 0)
        return;
    if(0==QMessageBox::question(this,tr("系统提示"),tr("确定要删除当前选中的配置项吗？"), tr("确定"), tr("取消"))) {
        QString id = ui->tableWidget_2->item(row,0)->text();
        ui->tableWidget_2->removeRow(row);
        if(row >0)
        {
            ui->tableWidget_2->selectRow(row-1);
        }
        else
        {
            ui->tableWidget_2->selectRow(0);
        }
        if(deleteStationConfigurationData(id))
        {
            qDebug()<<"删除一行";
        }
    }
    //QuerAllData();
}


void SysManager::on_comboBox_f9_currentIndexChanged(int index)
{

    if(f9Index==QString::number(index+1))
        return;
    f9Index=QString::number(index+1);
    SysUtils::setSysParam("f9_Set",index+1);
    ui->f9_label->setText(QString("设置成功 time:%1").arg(QTime::currentTime().toString("mm:ss")));
    mainWindow->setButtonF9(f9Index,false);

}

void SysManager::clickSlot()
{

    QObject *object = QObject::sender();
    QCheckBox *check_box = qobject_cast<QCheckBox *>(object);
    QString btnName = check_box->objectName();
    int current=btnName.toInt();
    qDebug()<<btnName<<"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
    //获取数据
    QSqlQuery query;
    QString sql=QString("select * from SysParam where code ='%1' ").arg("servoname");
    query.exec(sql);
    QString  servoData="";
    if(query.next())
    {
         servoData=query.value(1).toString();
    }
    QStringList servoList=servoData.split(",");
    QString aftervalue;
    QString beforevalue;
    QString value;
     QString servovalue;
    for(int i=0;i<btnName.toInt();i++)
    {
       beforevalue+=servoList.at(i);
       beforevalue+=",";
    }
    for(int i=btnName.toInt()+1;i<servoList.size();i++)
    {
       aftervalue+=servoList.at(i);
       aftervalue+=",";
    }
     aftervalue.remove(aftervalue.length()-1,1);
     if(servoNameCheck[current]->isChecked())
     {
      value="1";
      g_servo->tabwidget_show(current);
     }
     else{
         value="0";
         g_servo->tabwidget_hide(current);
     }
     if(servoList.size()==current+1)
     {

          servovalue=beforevalue+value;
     }
     else
     {

       servovalue=beforevalue+value+","+aftervalue;
     }

   QSqlQuery query1;
   QString sql1=QString("UPDATE SysParam SET value='%1' WHERE code='servoname'").arg(servovalue);
   qDebug()<<"sql:"<<sql1<<query1.exec(sql1);

}

void SysManager::on_pushButton_6_clicked()
{
    QSqlQuery query;
    QString sql=QString("select * from SysParam where code ='%1' ").arg("userpassword");
    query.exec(sql);
    QString  passwordData="";
    QString  glpassword=ui->lineEdit_glpassword->text();
    QString  ygpassword=ui->lineEdit_ygpassword->text();
    if(glpassword== "" || ygpassword=="")
    {
       QMessageBox::information(this, "提示", "密码不能为空！");
       return;
    }
    if(query.next())
    {
         passwordData=query.value(1).toString();
    }
    if(passwordData=="")
    {

        QString password=glpassword+"|"+ygpassword;
        QString  sql1=QString("INSERT INTO SysParam(code,value) VALUES ('%1','%2')").arg("userpassword").arg(password);
        query.exec(sql1);
    }
    else
    {
        QString password=glpassword+"|"+ygpassword;
        QString  sql1=QString("UPDATE SysParam SET value='%1' WHERE code='userpassword'").arg(password);
        query.exec(sql1);

    }

}
