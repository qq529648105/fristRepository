#include "KnifeWidget.h"
#include "ui_KnifeWidget.h"
#include <QTableWidget>
#include <QScrollBar>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QUuid>
#include <QTimer>
#include<QDebug>
#include "SysUtils.h"
#include "Global.h"
#include <QFileDialog>

/*****************************
 *          数据库字段命明
 *dbTabName :  表名称
 *fPriKey   :  主键字段
 *fCutName  :  字段： 刀具名称
 *fCutNo    :  字段  刀具编号
 *fCutSpe   :  字段  刀具规格
 *fCutToNum :  字段  刀具使用总次数
 *fCutUsNum :  字段  刀具已使用次数
 ****************************/


const QString dbTabName = "KnifeLibrary";
const QString fPriKey  = "id";
const QString fCutName  = "name";
const QString fCutNo    = "code";
const QString fCutSpe  = "model";
const QString fCutToNum  = "count";
const QString fCutUsNum = "used";
//chenjy update
const QString dbAbrasionTable = "Knife";
const QString AbrasionId  = "id";
const QString AbrasionNameOne  = "name1";
const QString AbrasionNameTwo   = "name2";
const QString AbrasionNameThree  = "name3";
const QString AbrasionNameFour  = "name4";
const QString AbrasionX = "x";
const QString AbrasionZ = "z";
const QString AbrasionR = "r";
const QString AbrasionY = "y";
const QString AbrasionMax = "max";
const QString AbrasionStandard = "standard";
const QString AbrasionVOne = "v1";
const QString AbrasionVTwo = "v2";
const QString AbrasionSOne = "s1";
const QString AbrasionSTwo = "s2";

const QString dbSwitchTable = "Knife2";
const QString SwitchId  = "id";
const QString SwitchParId  = "parId";
const QString SwitchXi   = "xi";
const QString SwitchVar  = "var";
const QString SwitchNameOne  = "name1";
const QString SwitchNameTwo  = "name2";
const QString SwitchVOne = "v1";
const QString SwitchVTwo = "v2";
const QString SwitchVThree = "v3";
const QString SwitchVFour= "v4";
const QString SwitchVFive = "v5";
const QString SwitchVSix = "v6";
const QString SwitchBind = "bind";
//end
QList<CUTINFO>CutInfoList;
QList<Knife>KnifeList;
QList<Knife2>Knife2List;

KnifeWidget::KnifeWidget(QWidget *parent) :
    QWidget(parent), isDbCon(false),
    ui(new Ui::KnifeWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("刀具配置"));
    fieldName=QString("id,name1,name2,name3,name4,x,z,r,y,max,standard,v1").split(",");
    fieldName2=QString("id,parId,xi,var,name1,name2,v1,v2,v3,v4,v5,v6,bind").split(",");
  //QString dbPath = "/Dz_Project.db3";
   // this->OpenDb(dbPath);
    QTimer::singleShot(500,this,SLOT(InitKinMenu()));
    listModel=new QStringListModel(listLog);
    ui->listView->setModel(listModel);
    ui->tabWidget->setTabEnabled(0,false);
    ui->tabWidget->setTabEnabled(3,false);

}

KnifeWidget::~KnifeWidget()
{

    delete ui;
}

//查询所有数据
void mySql(QComboBox*com,QString &sql)
{

    com->clear();
    QSqlQuery query3;
    query3.exec(sql);
    while(query3.next())
    {
        com->addItem(query3.value(0).toString());
    }
}
void KnifeWidget::InitKinMenu()
{

     clearData();
     InitDataTable();
     ReadCutInfo();

     QString sql3="SELECT DISTINCT name1 from knife where name1<>''";
     QString sql4="SELECT DISTINCT name3 from knife where name3<>''";
     QString sql5="SELECT DISTINCT name4 from knife where name4<>''";

     mySql(ui->comboBox_2,sql3);
     mySql(ui->comboBox_3,sql4);
     mySql(ui->comboBox_4,sql5);
     for(int i=0;i<KnifeList.size();i++)
     {
        ui->comboBox->addItem(KnifeList[i].name1+" 刀具:"+KnifeList[i].name3+" 刀沿:"+KnifeList[i].name4,KnifeList[i].id);
     }

     connect(ui->KniInfoWidget,SIGNAL(itemSelectionChanged()),this,SLOT(ShowKniInfo()));
     connect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem(int,int)));
    // connect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(ShowKniInfo(int ,int)));
     connect(ui->KniInfoWidget,SIGNAL(cellClicked(int,int)),SLOT(EditDjItem(int,int)));

     //knife2
     connect(ui->KniInfoWidget2,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem2(int,int)));

}


void KnifeWidget::InitDataTable()
{
    QFont font;
    font.setPixelSize(16);
    ui->KniInfoWidget->setFont(font);
    ui->KniInfoWidget->horizontalHeader()->setFont(font);
    for(int i = 0; i<12; i++)
        ui->KniInfoWidget->setColumnWidth(i,ui->KniInfoWidget->width()/12);
    ui->KniInfoWidget->setColumnHidden(0,true);
    ui->KniInfoWidget->horizontalHeader()->setFixedHeight(40);
    ui->KniInfoWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->KniInfoWidget->verticalHeader()->setVisible(false);
    ui->KniInfoWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->KniInfoWidget->horizontalScrollBar()->setVisible(false);


    ui->KniInfoWidget2->setFont(font);
    ui->KniInfoWidget2->horizontalHeader()->setFont(font);
    for(int i = 0; i<13; i++)
        ui->KniInfoWidget2->setColumnWidth(i,ui->KniInfoWidget2->width()/13);
    ui->KniInfoWidget2->setColumnHidden(0,true);
    ui->KniInfoWidget2->setColumnHidden(1,true);
    ui->KniInfoWidget2->horizontalHeader()->setFixedHeight(40);
    ui->KniInfoWidget2->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->KniInfoWidget2->verticalHeader()->setVisible(false);
    ui->KniInfoWidget2->setSelectionBehavior(QTableWidget::SelectRows);
    ui->KniInfoWidget2->horizontalScrollBar()->setVisible(false);

//    QComboBox *comBox1=new QComboBox;
//    comBox1->addItem("开");
//    comBox1->addItem("关");
//    ui->KniInfoWidget2->setCellWidget(0,2,comBox1);




}





/**************************
函数: 初始化读取SQLITE数据显示

**************************/

void KnifeWidget:: ReadCutInfo()
{
//    if(!GetDbConnect())
//        return;
    if(!QuerAllData())
        return;
    for(int i = 0; i< KnifeList.size(); i++)
    {

        ui->KniInfoWidget->insertRow(i);
        ui->KniInfoWidget->setRowHeight(i,60);
        ui->KniInfoWidget->setItem(i,0,new QTableWidgetItem(KnifeList.at(i).id));
        ui->KniInfoWidget->setItem(i,1,new QTableWidgetItem(KnifeList.at(i).name1));
        ui->KniInfoWidget->setItem(i,2,new QTableWidgetItem(KnifeList.at(i).name2));
        ui->KniInfoWidget->setItem(i,3,new QTableWidgetItem(KnifeList.at(i).name3));
        ui->KniInfoWidget->setItem(i,4,new QTableWidgetItem(KnifeList.at(i).name4));
        ui->KniInfoWidget->setItem(i,5,new QTableWidgetItem(QString::number(KnifeList.at(i).x)));
        ui->KniInfoWidget->setItem(i,6,new QTableWidgetItem(QString::number(KnifeList.at(i).z)));
        ui->KniInfoWidget->setItem(i,7,new QTableWidgetItem(QString::number(KnifeList.at(i).r)));
        ui->KniInfoWidget->setItem(i,8,new QTableWidgetItem(QString::number(KnifeList.at(i).y)));
        ui->KniInfoWidget->setItem(i,9,new QTableWidgetItem(QString::number(KnifeList.at(i).max)));
        ui->KniInfoWidget->setItem(i,10,new QTableWidgetItem(QString::number(KnifeList.at(i).standard)));
        ui->KniInfoWidget->setItem(i,11,new QTableWidgetItem(QString::number(KnifeList.at(i).v1)));

//        ui->KniInfoWidget->item(i,3)->setData(Qt::DisplayRole,QVariant(CutInfoList.at(i).CutTotNum).toInt());
//        ui->KniInfoWidget->item(i,4)->setData(Qt::DisplayRole | Qt::EditRole,CutInfoList.at(i).CutTotNum-CutInfoList.at(i).CutUseNum);
//        ui->KniInfoWidget->item(i,4)->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable| Qt:: ItemIsEnabled);
//        for(int cout=0; cout<8; cout++)
//         ui->KniInfoWidget->item(i,cout)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    if(!QuerAllData2())
        return;

    for(int i = 0; i< Knife2List.size(); i++)
    {

        ui->KniInfoWidget2->insertRow(i);
        ui->KniInfoWidget2->setRowHeight(i,60);
        ui->KniInfoWidget2->setItem(i,0,new QTableWidgetItem(Knife2List.at(i).id));
        ui->KniInfoWidget2->setItem(i,1,new QTableWidgetItem(Knife2List.at(i).parId));
        //ui->KniInfoWidget2->setItem(i,2,new QTableWidgetItem(Knife2List.at(i).xi));

        QComboBox *comBox1=new QComboBox;
        comBox1->addItem("开");
        comBox1->addItem("关");
        if(Knife2List[i].xi=="关")
            comBox1->setCurrentIndex(1);
        ui->KniInfoWidget2->setCellWidget(i,2,comBox1);
        connect(comBox1,SIGNAL(currentIndexChanged(int)),this,SLOT(modifyON(int)));

        ui->KniInfoWidget2->setItem(i,3,new QTableWidgetItem(Knife2List.at(i).var));

        ui->KniInfoWidget2->setItem(i,4,new QTableWidgetItem(Knife2List.at(i).name1));
        ui->KniInfoWidget2->setItem(i,5,new QTableWidgetItem(Knife2List.at(i).name2));
        ui->KniInfoWidget2->setItem(i,6,new QTableWidgetItem(QString::number(Knife2List.at(i).v1)));
        ui->KniInfoWidget2->setItem(i,7,new QTableWidgetItem(QString::number(Knife2List.at(i).v2)));
        ui->KniInfoWidget2->setItem(i,8,new QTableWidgetItem(QString::number(Knife2List.at(i).v3)));
        ui->KniInfoWidget2->setItem(i,9,new QTableWidgetItem(QString::number(Knife2List.at(i).v4)));
        ui->KniInfoWidget2->setItem(i,10,new QTableWidgetItem(QString::number(Knife2List.at(i).v5)));
        ui->KniInfoWidget2->setItem(i,11,new QTableWidgetItem(QString::number(Knife2List.at(i).v6)));
        ui->KniInfoWidget2->setItem(i,12,new QTableWidgetItem(Knife2List.at(i).bind));


   }

}

/*****************
 *得到列表某个单元格值变化后
 *进行操作
 *
 *******************/
void KnifeWidget::GetItem(int x, int y)
{

    QString sql ="";
    QString priKey = ui->KniInfoWidget->item(x,0)->text();
    QString value = ui->KniInfoWidget->item(x,y)->text();

    sql = QString("UPDATE Knife set %1='%2' where id='%3'").arg(fieldName[y]).arg(value).arg(priKey);
    qDebug()<<sql;
    ExecSql(sql);

    QuerAllData();

}

void KnifeWidget::GetItem2(int x, int y)
{

    QString sql ="";
    QString priKey = ui->KniInfoWidget2->item(x,0)->text();
    QString value = ui->KniInfoWidget2->item(x,y)->text();

    if(y==6)
    mainWindow->standardVal(x+1,value);

    sql = QString("UPDATE Knife2 set %1='%2' where id='%3'").arg(fieldName2[y]).arg(value).arg(priKey);
    qDebug()<<sql;
    ExecSql(sql);
    ShowKniInfo();
    QuerAllData2();
    if(Knife2List.size()>0)
    {
        QString modifyParam=QString("修改记录:%1  %2  %3  %4  %5  %6")
                .arg(Knife2List[0].v1)
                .arg(Knife2List[0].v2)
                .arg(Knife2List[0].v3)
                .arg(Knife2List[0].v4)
                .arg(Knife2List[0].v5)
                .arg(Knife2List[0].v6);
        mainWindow->writeLog(modifyParam);
    }

}

void KnifeWidget::EditDjItem(int x, int y)
{


    if(y != 4)
    {

        ui->KniInfoWidget->editItem(ui->KniInfoWidget->item(x,y));
    }

}


void KnifeWidget::ShowKniInfo()
{
    int row = ui->KniInfoWidget->currentRow();
    if(row >= 0)
    {
//        ui->Edit_DjName->setText(ui->KniInfoWidget->item(row,0)->text());
//        ui->Edit_DjId->setText(ui->KniInfoWidget->item(row,1)->text());
//        ui->Edit_DjGg->setText(ui->KniInfoWidget->item(row,2)->text());
//        ui->Edit_DjToNum->setText(ui->KniInfoWidget->item(row,3)->text());
//        ui->Edit_LeftNum->setText(ui->KniInfoWidget->item(row,4)->text());
    }
    else
    {
//        ui->Edit_DjName->clear();
//        ui->Edit_DjId->clear();
//        ui->Edit_DjToNum->clear();
//        ui->Edit_DjGg->clear();
//        ui->Edit_LeftNum->clear();
    }
}


void KnifeWidget::ShowKniInfo(int x,int y)
{

//    switch(y)
//    {
//    case 0:
//        ui->Edit_DjName->setText(ui->KniInfoWidget->item(x,y)->text());
//        break;
//    case 1:
//        ui->Edit_DjId->setText(ui->KniInfoWidget->item(x,y)->text());
//        break;
//    case 2:
//        ui->Edit_DjGg->setText(ui->KniInfoWidget->item(x,y)->text());
//        break;
//    case 3:
//        ui->Edit_DjToNum->setText(ui->KniInfoWidget->item(x,y)->text());
//        break;
//    case 4:
//        ui->Edit_LeftNum->setText(ui->KniInfoWidget->item(x,y)->text());
//        break;
//    }
}

void KnifeWidget::paintEvent(QPaintEvent *e)
{


    for(int i =0; i<12; i++)
        ui->KniInfoWidget->setColumnWidth(i,ui->KniInfoWidget->width()/12);

    for(int i =0; i<13; i++)
        ui->KniInfoWidget2->setColumnWidth(i,ui->KniInfoWidget2->width()/13);
    ui->KniInfoWidget2->setColumnWidth(12,ui->KniInfoWidget2->width()/13*2);

    e->accept();



}




void KnifeWidget::addKnife1()
{

    QString keyid = SysUtils::uuid();//QUuid::createUuid().toString();//
    InsertNullData(keyid);

    disconnect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem(int,int)));
  //  disconnect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(ShowKniInfo(int ,int)));
//    ui->KniInfoWidget->setSortingEnabled(false);
   // int row = ui->KniInfoWidget->rowCount();
     ui->KniInfoWidget->insertRow(0);
     //int row = ui->KniInfoWidget->currentRow();
    ui->KniInfoWidget->setRowHeight(0,60);

    for(int i = 0; i<12; i++)
    {

        ui->KniInfoWidget->setItem(0,i,new QTableWidgetItem());
//        if(i<3)
//        ui->KniInfoWidget->item(0,i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    }

    ui->KniInfoWidget->item(0,5)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget->item(0,6)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget->item(0,7)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget->item(0,8)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget->item(0,9)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget->item(0,10)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget->item(0,11)->setData(Qt::DisplayRole,QVariant(1000.0));
    //ui->KniInfoWidget->item(0,6)->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->KniInfoWidget->selectRow(0);
    ui->KniInfoWidget->item(0,0)->setText(keyid);

//    ui->KniInfoWidget->setSortingEnabled(true);
    connect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem(int,int)));
   // connect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(ShowKniInfo(int ,int)));

    QuerAllData();
}


void KnifeWidget::addKnife2()
{
    if(ui->comboBox->count()==0)
    {
        return;
    }
    QString keyid = SysUtils::uuid();
    QString parId=ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();

    QSqlQuery quer;

    QString sql =QString("Insert into Knife2(id,parId,v1,v2,v3,v4,v5,v6,bind) VALUES('%1','%2',1000.0,1000.0,1000.0,1000.0,1000.0,1000.0,'%3')").arg(keyid).arg(parId).arg(ui->comboBox->currentText());
    if(!quer.exec(sql))
        qDebug()<<"error"<<sql;

    disconnect(ui->KniInfoWidget2,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem2(int,int)));

     ui->KniInfoWidget2->insertRow(0);

    ui->KniInfoWidget2->setRowHeight(0,60);



    for(int i = 0; i<13; i++)
    {

        ui->KniInfoWidget2->setItem(0,i,new QTableWidgetItem());
    }

    QComboBox *comBox1=new QComboBox;
    comBox1->addItem("开");
    comBox1->addItem("关");
    ui->KniInfoWidget2->setCellWidget(0,2,comBox1);
    connect(comBox1,SIGNAL(currentIndexChanged(int)),this,SLOT(modifyON(int)));

   // ui->KniInfoWidget2->item(0,6)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget2->item(0,7)->setData(Qt::DisplayRole,QVariant(1000.0));
   // ui->KniInfoWidget2->item(0,8)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget2->item(0,9)->setData(Qt::DisplayRole,QVariant(1));

    ui->KniInfoWidget2->item(0,10)->setData(Qt::DisplayRole,QVariant(1));

    //ui->KniInfoWidget2->item(0,11)->setData(Qt::DisplayRole,QVariant(1000.0));
    ui->KniInfoWidget2->item(0,12)->setData(Qt::DisplayRole,ui->comboBox->currentText());

    ui->KniInfoWidget2->selectRow(0);

    ui->KniInfoWidget2->item(0,0)->setText(keyid);
    //ui->KniInfoWidget2->item(0,1)->setText(parId);
   // ui->KniInfoWidget2->item(0,2)->setText(xi);


    connect(ui->KniInfoWidget2,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem2(int,int)));

    QuerAllData2();

}
void KnifeWidget::addKnife()
{

    if(ui->tabWidget->currentIndex()==1)
       {
        addKnife1();
    }
    else if(ui->tabWidget->currentIndex()==2)
    {
        addKnife2();
    }

}
void KnifeWidget::removeKnife()
{
    if(ui->tabWidget->currentIndex()==1)
       {
        removeKnife1();
    }
    else if(ui->tabWidget->currentIndex()==2)
    {
        removeKnife2();
    }
}

void KnifeWidget::removeKnife1()
{
    int row = ui->KniInfoWidget->currentRow();
    if(row < 0)
        return;
    if(0==QMessageBox::question(this,tr("系统提示"),tr("确定要删除当前选中的刀具吗？"), tr("确定"), tr("取消"))) {
        QString KeyId = ui->KniInfoWidget->item(row,0)->text();
        ui->KniInfoWidget->removeRow(row);
        if(row >0)
        {
            ui->KniInfoWidget->selectRow(row-1);
        }
        else
        {
            ui->KniInfoWidget->selectRow(0);
        }
        DelData(KeyId );
    }
    QuerAllData();
}

void KnifeWidget::removeKnife2()
{
    int row = ui->KniInfoWidget2->currentRow();
    if(row < 0)
        return;
    if(0==QMessageBox::question(this,tr("系统提示"),tr("确定要删除当前选中的刀具吗？"), tr("确定"), tr("取消"))) {
        QString KeyId = ui->KniInfoWidget2->item(row,0)->text();
        ui->KniInfoWidget2->removeRow(row);
        if(row >0)
        {
            ui->KniInfoWidget2->selectRow(row-1);
        }
        else
        {
            ui->KniInfoWidget2->selectRow(0);
        }
        QSqlQuery quer;
        QString sql = QString("Delete From Knife2 where id='%1'").arg(KeyId);

        if(!quer.exec(sql))
        {
            qDebug()<<"del fail";
        }
    }
    QuerAllData2();
}


/***********************
函数：     重置刀具
返回值:    重置后刀具剩余次数
返回类型 ： Int
返回无效值 ：-9999
************************/

int KnifeWidget::resetKnife()
{
    return 0;
//    int row = ui->KniInfoWidget->currentRow();
//    if(row >= 0)
//    {
//        QString keyId = ui->KniInfoWidget->item(row,5)->text();
//        int leftNum = QueTotNum(keyId);
//       // ui->KniInfoWidget->item(row,4)->setText(QString::number(leftNum));
//         ui->KniInfoWidget->item(row,4)->setData(Qt::DisplayRole,leftNum);
//        QString sql = "update " + dbTabName +" set " +fCutUsNum +"=0 where " + fPriKey + "=" +QString("'%1'").arg(keyId);
//        ExecSql(sql);
//        return leftNum;
//    }
//    else
//        return -9999;

}

void KnifeWidget::clearData()
{
    QString sql1="UPDATE Knife set x='1000.0',z='1000.0',r='1000.0',y='1000.0',max='1000.0',standard='1000.0',v1='1000.0',v2='1000.0'";
    //QString sql2="UPDATE Knife2 set v2='1000.0'";
    ExecSql(sql1);
    //ExecSql(sql2);
}
void KnifeWidget::updateTab1()
{
    ui->Edit1->setText("1000");
    ui->Edit2->setText("1000");
    ui->Edit3->setText("1000");
    ui->Edit4->setText("1000");
    ui->Edit5->setText("1000");
    ui->Edit6->setText("1000");
    ui->Edit7->setText("1000");
    ui->Edit8->setText("1000");
    ui->Edit9->setText("1000");
    ui->Edit10->setText("1000");
    ui->Edit11->setText("1000");
    ui->Edit12->setText("1000");


    QString name=ui->comboBox_2->currentText();
    QString name3=ui->comboBox_3->currentText();
    float r=1000,z=1000;

    for(int i=0;i<KnifeList.size();i++)
    {
        if(name==KnifeList[i].name1&&name3==KnifeList[i].name3)
        {
            r=KnifeList[i].r;
            z=KnifeList[i].z;
            if(name=="镗孔")
            {
                r=KnifeList[i].y;
                z=KnifeList[i].r;

            }
            if(KnifeList[i].name4=="1")
            {
                ui->Edit1->setText(QString::number(KnifeList[i].x,'f',3));
                ui->Edit2->setText(QString::number(z,'f',3));
                ui->Edit3->setText(QString::number(r,'f',3));
            }
            else if(KnifeList[i].name4=="2")
            {
                ui->Edit4->setText(QString::number(KnifeList[i].x,'f',3));
                ui->Edit5->setText(QString::number(z,'f',3));
                ui->Edit6->setText(QString::number(r,'f',3));
            }
            else if(KnifeList[i].name4=="3")
            {
                ui->Edit7->setText(QString::number(KnifeList[i].x,'f',3));
                ui->Edit8->setText(QString::number(z,'f',3));
                ui->Edit9->setText(QString::number(r,'f',3));
            }
            else if(KnifeList[i].name4=="4")
            {
                ui->Edit10->setText(QString::number(KnifeList[i].x,'f',3));
                ui->Edit11->setText(QString::number(z,'f',3));
                ui->Edit12->setText(QString::number(r,'f',3));
            }

        }
    }
}
void KnifeWidget::writeKnife1(QString data)
{

    QStringList list=data.split("|");
    int i=0,j=-1;
    if(list.size()!=5)
        return;


    QSqlQuery query;
    //QString sql=QString("UPDATE Knife set %1=%2 where name2='%3' and name3='%4' and name4='%5'").arg(list[3]).arg(list[4]).arg(list[0]).arg(list[1]).arg(list[2]);
     QString sql=QString("select name2,name3,name4 from knife");
    if(!query.exec(sql))
        qDebug()<<"error"<<sql;

    for(i=0;i<KnifeList.size();i++)
    {
        if(KnifeList[i].name2==list[0]&&KnifeList[i].name3==list[1]&&KnifeList[i].name4==list[2])
            break;
    }

    if(i==KnifeList.size())
        return;


    if(list[3]=="X")
    {
        j=5;
    }
    else if(list[3]=="Z")
    {j=6;}
    else if(list[3]=="R")
    {j=7;}
    else if(list[3]=="Y")
    {j=8;}

    if(j==-1)
        return;
//disconnect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem(int,int)));

     ui->KniInfoWidget->item(i,j)->setData(Qt::DisplayRole,QVariant(list[4].toFloat()));
//    QString sql2=QString("update knife set %1=%2 where name2='%3' and name3='%4' and name4='%5'").arg(list[3]).arg(list[4]).arg(list[0]).arg(list[1]).arg(list[2]);
//    ExecSql(sql2);
//    QuerAllData();

    updateTab1();
//connect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem(int,int)));
}

void KnifeWidget::writeKnife1Log(QString &data)
{
    listLog<<data;
    listModel->setStringList(listLog);
    ui->listView->scrollToBottom();
}



int KnifeWidget::autoKnife(QString type,double data,QString& out)
{
    QSqlQuery query,query2;
    QString sql=QString("select * from knife2 where name2='%1'").arg(type);

    if(!query.exec(sql))
        return 1;
    if(!query.next())
        return 2;
    QString direct=query.value(4).toString();
    double standard=query.value(6).toDouble();
    double max=query.value(8).toDouble();
    int negative=query.value(9).toDouble();
    double ratio=query.value(10).toDouble();
    double result=1000;

    if(data>qAbs(max)||data<qAbs(max)*-1)
        return 3;

    result=qAbs(standard-data);

    if(result>0.002)
        return 4;

    result*=ratio;
    result= (negative>0)?result:result*-1;

    sql=QString("select %1,name2,name3,name4 from knife where id='%2'").arg(direct).arg(query.value(1).toString());
    qDebug()<<"aaaa"<<sql<<result;
    if(!query2.exec(sql))
        return 5;
    if(!query2.next())
        return 6;
    double directValue=query2.value(0).toDouble();
    result+=directValue;
    out=QString("?%1|%2|%3|%4|%5").arg(query2.value(1).toString()).arg(query2.value(2).toString()).arg(query2.value(3).toString()).arg(direct).arg(result);


    double v1=directValue;
    double value=result;

    if(value>9.9||value<-9.9||v1==1000)
    {
        return 7;
    }
    if(qAbs(v1-value)>0.3)
    {
        return 8;
    }

    return 0;


}

/***********************
函数：     使用刀具
返回值:    刀具剩余次数
返回类型 ： Int
返回无效值 ：-9999
************************/
int KnifeWidget::useKnife()
{

    long totNum =0;
    long useNum = 0;
    QSqlQuery quer;
    int row = ui->KniInfoWidget->currentRow();
    if(row >=0 )
    {

        QString keyId = ui->KniInfoWidget->item(row,5)->text();
        QString querSql = "select " +fCutToNum +"," +fCutUsNum + " from "   \
                +dbTabName +" where " +fPriKey +"="+QString("'%1'").arg(keyId);

        if(!quer.exec(querSql))
        {

            return -9999;
        }
        while(quer.next())
        {

          totNum = quer.value(0).toInt();
          useNum = quer.value(1).toInt();

       }

        useNum++;
        QString upSql  = "update " + dbTabName +" set " +fCutUsNum +"="    \
                + QString("%1").arg(useNum)+" where " + fPriKey + "="  \
                +QString("'%1'").arg(keyId);
        ExecSql(upSql);
        // totNum
        //ui->KniInfoWidget->item(row,4)->setText(QString::number(totNum -useNum));
            ui->KniInfoWidget->item(row,4)->setData(Qt::DisplayRole,totNum -useNum);
        return (totNum -useNum);
    }
    else
        return -9999;
}


/****************数据库操作***********/



////打开数据库
//bool KnifeWidget::OpenDb(const QString& db)
//{
//    QSqlDatabase dbcon = QSqlDatabase::addDatabase("QSQLITE");
//    dbcon.setDatabaseName(db);
//    if ( !dbcon.open())
//    {
//        isDbCon = false;
//        return false;
//    }

//    isDbCon = true;
//    return true;
//}



bool KnifeWidget::QuerAllData()
{
    Knife knife;

    KnifeList.clear();



    QSqlQuery quer;


     QString sql1="SELECT name FROM sqlite_master  where type='table' and name='Knife'";
     quer.exec(sql1);
       if(!quer.next())
       {
           QString createTable="create table Knife(id varchar(50) not null,name1 varchar(32),name2 varchar(32),name3 varchar(32),name4 varchar(32),x float,z float,r float,y float,max float,standard float,v1 float,v2 float,s1 varchar(32),s2 varchar(32))";
           if(quer.exec(createTable))
               qDebug()<<"createTable success";
       }


    QString sql = "select * from knife";


    if(!quer.exec(sql))
    {
        return false;
    }

    while(quer.next())
    {

        knife.id=quer.value(0).toString();
        knife.name1=quer.value(1).toString();
        knife.name2=quer.value(2).toString();
        knife.name3=quer.value(3).toString();
        knife.name4=quer.value(4).toString();
        knife.x=quer.value(5).toFloat();
        knife.z=quer.value(6).toFloat();
        knife.r=quer.value(7).toFloat();
        knife.y=quer.value(8).toFloat();
        knife.max=quer.value(9).toFloat();
        knife.standard=quer.value(10).toFloat();
        knife.v1=quer.value(11).toFloat();
        knife.v2=quer.value(12).toFloat();
        knife.s1=quer.value(13).toString();
        knife.s2=quer.value(14).toString();
        KnifeList.append(knife);



    }



    return true;
}

bool KnifeWidget::QuerAllData2()
{

    Knife2 knife2;

    Knife2List.clear();


    QSqlQuery quer2;



     QString sql_2="SELECT name FROM sqlite_master  where type='table' and name='Knife2'";
     quer2.exec(sql_2);


       if(!quer2.next())
       {
           qDebug()<<"createTable start";
           QString createTable="create table Knife2(id varchar(50) not null,parId varchar(50),xi varchar(32),var varchar(32),name1 varchar(32),name2 varchar(32),v1 float,v2 float,v3 float,v4 float,v5 float,v6 float,bind var varchar(50))";
           if(quer2.exec(createTable))
               qDebug()<<"createTable success";
       }

    QString sql2 = "select * from knife2";


    if(!quer2.exec(sql2))
    {
        return false;
    }


    while(quer2.next())
    {

        knife2.id=quer2.value(0).toString();
        knife2.parId=quer2.value(1).toString();
        knife2.xi=quer2.value(2).toString();
        knife2.var=quer2.value(3).toString();
        knife2.name1=quer2.value(4).toString();
        knife2.name2=quer2.value(5).toString();
        knife2.v1=quer2.value(6).toFloat();
        knife2.v2=quer2.value(7).toFloat();
        knife2.v3=quer2.value(8).toFloat();
        knife2.v4=quer2.value(9).toFloat();
        knife2.v5=quer2.value(10).toFloat();
        knife2.v6=quer2.value(11).toFloat();
        knife2.bind=quer2.value(12).toString();

        Knife2List.append(knife2);

    }

    return true;
}

bool KnifeWidget::DelData(QString &keyId)
{

    QSqlQuery quer;
    QString sql = QString("Delete From Knife where id='%1'").arg(keyId);

    if(!quer.exec(sql))
    {

        return false;
    }
    else
        return true;
}

bool KnifeWidget::InsertNullData(QString& keyid )
{

     QSqlQuery quer;
     QString sql =QString("Insert into Knife(id,x,z,r,y,max,standard,v1,v2) VALUES('%1',1000.0,1000.0,1000.0,1000.0,1000.0,1000.0,1000.0,1000.0)").arg(keyid);

     if(!quer.exec(sql))
     {

         return false;
     }
     else
         return true;

     ///qDebug()<<sql;
}

//查询最新插入一行记录的主键
unsigned int KnifeWidget::QueLastKeyId()
{
    /* QSqlQuery quer;
     QString sql = "select " +fPriKey +" from ";
     sql.append(dbTabName + " order by ");
     sql.append(fPriKey);
     sql.append(" desc LIMIT 1;");

      if(!quer.exec(sql))
     {

         return 0;
     }
     while(quer.next())
     {

        return (quer.value(0).toInt());


     }*/
     return  0;
}

int KnifeWidget::QueTotNum(QString &keyId)
{
    QSqlQuery quer;
    QString sql =" SELECT " +fCutToNum + " FROM " + dbTabName + " WHERE " + fPriKey + "=";
    sql.append(QString("'%1'").arg(keyId));
    if(!quer.exec(sql))
    {

        return 0;
    }
    while(quer.next())
    {

       return quer.value(0).toInt();
   }
    return 0;

}


int KnifeWidget::QueUseNum(QString& keyId)
{
    QSqlQuery quer;
    QString sql =" SELECT " +fCutUsNum + " FROM " + dbTabName + " WHERE " + fPriKey + "=";
    sql.append(QString("'%1'").arg(keyId));
    if(!quer.exec(sql))
    {

        return 0;
    }
    while(quer.next())
    {

       return quer.value(0).toInt();
   }
    return 0;
}

void KnifeWidget::setCurTab(int index)
{

    ui->tabWidget->setCurrentIndex(index);
}


//执行SQL语句
bool KnifeWidget::ExecSql(QString &sql)
{
    QSqlQuery quer;
    if(!quer.exec(sql))
    {

        return false;
    }
    else
        return true;

}







void KnifeWidget::on_pushButton_3_clicked()
{




    QString kName="?";
    QByteArray k1=kName.toLocal8Bit();
    if(ui->pushButton_3->text()=="连接CNC")
    {
        k1[0]=0xB1;
        ui->pushButton_3->setText("断开CNC");
    }
    else
    {
        k1[0]=0xB2;
        ui->pushButton_3->setText("连接CNC");
    }
    mainWindow->sendKnife(k1);


}

void KnifeWidget::on_comboBox_2_currentIndexChanged(int index)
{

    if(ui->comboBox_2->currentText()=="镗孔")
    {
        ui->label_11->setText("工件偏移Y(mm)");
        ui->label_10->setText("半径(mm)");
    }
    else
    {
        ui->label_10->setText("长度Z(mm)");
        ui->label_11->setText("半径(mm)");
    }
    updateTab1();
}
void KnifeWidget::sendKnifeParam()
{



    float value=ui->value->text().toFloat();
    QMessageBox::StandardButton reply;
    QSqlQuery query;
    QString sql=QString("select %4,name2 from knife where name1='%1' and name3='%2' and name4='%3'").arg(ui->comboBox_2->currentText()).arg(ui->comboBox_3->currentText()).arg(ui->comboBox_4->currentText()).arg(ui->comboBox_5->currentText());
    bool b=query.exec(sql);
    if(!b)
    {
        listLog<<"数据库无记录";
        listModel->setStringList(listLog);
        ui->listView->scrollToBottom();
        return;
    }
    if(query.next())
    {
        float v1=query.value(0).toFloat();
        if(value>9.9||value<-9.9||v1==1000)
        {
            listLog<<"写入值范围超出"<<QString::number(v1);
            listModel->setStringList(listLog);
            ui->listView->scrollToBottom();
            return;
        }
        if(qAbs(v1-value)>0.3)
        {
            reply = QMessageBox::question(this, "磨损", "超出0.3是否继续写入数据?", QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::No)
            {
                return;
            }
        }
    }


    QString wData="?"+query.value(1).toString()+"|"+ui->comboBox_3->currentText()+"|"+ui->comboBox_4->currentText()+"|"+ui->comboBox_5->currentText()+"|"+ui->value->text();
    QByteArray array=wData.toLocal8Bit();
    array[0]=0xB4;

    listLog<<wData;
    listModel->setStringList(listLog);
    ui->listView->scrollToBottom();


    reply = QMessageBox::question(this, "磨损", "是否写入数据?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
    {
        return;
    }
    mainWindow->sendKnife(array);
    listLog<<"写入成功";
    listModel->setStringList(listLog);
    ui->listView->scrollToBottom();
}
void KnifeWidget::on_pushButton_4_clicked()
{

    QTimer::singleShot(0,this,SLOT(sendKnifeParam()));

}

void KnifeWidget::on_comboBox_3_currentIndexChanged(int index)
{
    updateTab1();
}

void KnifeWidget::on_comboBox_currentIndexChanged(int index)
{

}

void KnifeWidget::modifyON(int index)
{

   int row= ui->KniInfoWidget2->currentRow();
   qDebug()<<"hello"<<row;

   if(row<0)
       return;
   QString sql ="";
   QString priKey = ui->KniInfoWidget2->item(row,0)->text();

   QComboBox *comBox=(QComboBox*)ui->KniInfoWidget2->cellWidget(row,2);

   QString value=comBox->currentText();

   sql = QString("UPDATE Knife2 set %1='%2' where id='%3'").arg(fieldName2[2]).arg(value).arg(priKey);
   qDebug()<<sql;
   ExecSql(sql);
   QuerAllData2();
}



void KnifeWidget::on_comboBox_activated(int index)
{
    int row = ui->KniInfoWidget2->currentRow();
    if(row<0)
        return;
    QString id=ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
    ui->KniInfoWidget2->item(row,12)->setText(ui->comboBox->currentText());
    ui->KniInfoWidget2->item(row,1)->setText(id);
    //QString id=ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
    //GetItem2(row,1);
}

void KnifeWidget::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("批量导入磨损数据"), "./", tr("*.csv"));
       if(path == "") return;
       SolutionSet *solutionSet = new SolutionSet();
       if( solutionSet->importCsvToAbrasion(path) ) {
           ui->KniInfoWidget->clearContents();
           while(ui->KniInfoWidget->rowCount()){ui->KniInfoWidget->removeRow(0);}
           disconnect(ui->KniInfoWidget,SIGNAL(itemSelectionChanged()),this,SLOT(ShowKniInfo()));
           disconnect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem(int,int)));
           disconnect(ui->KniInfoWidget,SIGNAL(cellClicked(int,int)),this,SLOT(EditDjItem(int,int)));
           selectAllDataToAbrasionList();
           for(int i = 0; i< KnifeList.count(); ++i)
           {
               ui->KniInfoWidget->insertRow(i);
               ui->KniInfoWidget->setRowHeight(i,60);
               ui->KniInfoWidget->setItem(i,0,new QTableWidgetItem(KnifeList.at(i).id));
               //ui->KniInfoWidget->item(i,0)->setData(Qt::DisplayRole,QVariant(erronList.at(i).eCode.toInt()));
               ui->KniInfoWidget->setItem(i,1,new QTableWidgetItem(KnifeList.at(i).name1));
               ui->KniInfoWidget->setItem(i,2,new QTableWidgetItem(KnifeList.at(i).name2));
               ui->KniInfoWidget->setItem(i,3,new QTableWidgetItem(KnifeList.at(i).name3));
               ui->KniInfoWidget->setItem(i,4,new QTableWidgetItem(KnifeList.at(i).name4));
               ui->KniInfoWidget->setItem(i,5,new QTableWidgetItem(QString::number(KnifeList.at(i).x)));
               ui->KniInfoWidget->setItem(i,6,new QTableWidgetItem(QString::number(KnifeList.at(i).z)));
               ui->KniInfoWidget->setItem(i,7,new QTableWidgetItem(QString::number(KnifeList.at(i).r)));
               ui->KniInfoWidget->setItem(i,8,new QTableWidgetItem(QString::number(KnifeList.at(i).y)));
               ui->KniInfoWidget->setItem(i,9,new QTableWidgetItem(QString::number(KnifeList.at(i).max)));
               ui->KniInfoWidget->setItem(i,10,new QTableWidgetItem(QString::number(KnifeList.at(i).standard)));
               ui->KniInfoWidget->setItem(i,11,new QTableWidgetItem(QString::number(KnifeList.at(i).v1)));
               for(int cout=0; cout<12; cout++)
                ui->KniInfoWidget->item(i,cout)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
           }
           connect(ui->KniInfoWidget,SIGNAL(itemSelectionChanged()),this,SLOT(ShowKniInfo()));
           connect(ui->KniInfoWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem(int,int)));
           connect(ui->KniInfoWidget,SIGNAL(cellClicked(int,int)),SLOT(EditDjItem(int,int)));


           ui->KniInfoWidget2->clearContents();
           while(ui->KniInfoWidget2->rowCount()){ui->KniInfoWidget2->removeRow(0);}
           disconnect(ui->KniInfoWidget2,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem2(int,int)));
           selectAllDataToSwitchList();
           for(int i = 0; i< Knife2List.count(); ++i){
               ui->KniInfoWidget2->insertRow(i);
               ui->KniInfoWidget2->setRowHeight(i,60);
               ui->KniInfoWidget2->setItem(i,0,new QTableWidgetItem(Knife2List.at(i).id));
               ui->KniInfoWidget2->setItem(i,1,new QTableWidgetItem(Knife2List.at(i).parId));

               //ui->KniInfoWidget2->setItem(i,2,new QTableWidgetItem(Knife2List.at(i).xi));
               QComboBox *comBox1=new QComboBox;
               comBox1->addItem("开",Knife2List.at(i).id);
               comBox1->addItem("关",Knife2List.at(i).id);
               if(Knife2List[i].xi=="关")
                   comBox1->setCurrentIndex(1);
               ui->KniInfoWidget2->setCellWidget(i,2,comBox1);
               connect(comBox1,SIGNAL(currentIndexChanged(int)),this,SLOT(modifyON(int)));



               ui->KniInfoWidget2->setItem(i,3,new QTableWidgetItem(Knife2List.at(i).var));
               ui->KniInfoWidget2->setItem(i,4,new QTableWidgetItem(Knife2List.at(i).name1));
               ui->KniInfoWidget2->setItem(i,5,new QTableWidgetItem(Knife2List.at(i).name2));
               ui->KniInfoWidget2->setItem(i,6,new QTableWidgetItem(Knife2List.at(i).v1));
               ui->KniInfoWidget2->setItem(i,7,new QTableWidgetItem(Knife2List.at(i).v2));
               ui->KniInfoWidget2->setItem(i,8,new QTableWidgetItem(Knife2List.at(i).v3));
               ui->KniInfoWidget2->setItem(i,9,new QTableWidgetItem(Knife2List.at(i).v4));
               ui->KniInfoWidget2->setItem(i,10,new QTableWidgetItem(Knife2List.at(i).v5));
               ui->KniInfoWidget2->setItem(i,11,new QTableWidgetItem(Knife2List.at(i).v6));
               ui->KniInfoWidget2->setItem(i,12,new QTableWidgetItem(Knife2List.at(i).bind));
               for(int cout=0; cout<13; cout++){
                   if(cout==2) continue;
                ui->KniInfoWidget2->item(i,cout)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
               }
           }
           connect(ui->KniInfoWidget2,SIGNAL(cellChanged(int,int)),this,SLOT(GetItem2(int,int)));

            QMessageBox::information(this,"系统提示","导入磨损数据成功！",QMessageBox::tr("确认"));
       }else {
           QMessageBox::information(this,"系统警告","导入磨损数据失败！",QMessageBox::tr("确认"));
       }


}
//chenyj  update
void KnifeWidget::selectAllDataToAbrasionList()
{
    Knife knifeInfo;
    KnifeList.clear();
    QSqlQuery query;
    query.exec(QString("select %1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15 from %16 order by %17")
               .arg(AbrasionId,AbrasionNameOne,AbrasionNameTwo, AbrasionNameThree,AbrasionNameFour,AbrasionX,AbrasionZ,AbrasionR,AbrasionY)
               .arg(AbrasionMax,AbrasionStandard,AbrasionVOne,AbrasionVTwo,AbrasionSOne,AbrasionSTwo,dbAbrasionTable,AbrasionNameThree));

    while(query.next())
    {
        knifeInfo.id=query.value(0).toString();
        knifeInfo.name1=query.value(1).toString();
        knifeInfo.name2=query.value(2).toString();
        knifeInfo.name3=query.value(3).toString();
        knifeInfo.name4=query.value(4).toString();
        knifeInfo.x=query.value(5).toFloat();
        knifeInfo.z=query.value(6).toFloat();
        knifeInfo.r=query.value(7).toFloat();
        knifeInfo.y=query.value(8).toFloat();
        knifeInfo.max=query.value(9).toFloat();
        knifeInfo.standard=query.value(10).toFloat();
        knifeInfo.v1=query.value(11).toFloat();
        knifeInfo.v2=query.value(12).toFloat();
        knifeInfo.s2=query.value(13).toString();
        knifeInfo.s2=query.value(14).toString();
        KnifeList.append(knifeInfo);
    }
}


void KnifeWidget::selectAllDataToSwitchList(){
  Knife2 knife2Info;
  Knife2List.clear();
  QSqlQuery query;
  query.exec(QString("select %1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13 from %14 ")
             .arg(SwitchId,SwitchParId,SwitchXi,SwitchVar,SwitchNameOne,SwitchNameTwo,SwitchVOne,SwitchVTwo,SwitchVThree)
             .arg(SwitchVFour,SwitchVFive,SwitchVSix,SwitchBind,dbSwitchTable));
  while(query.next())
  {
      knife2Info.id=query.value(0).toString();
      knife2Info.parId=query.value(1).toString();
      knife2Info.xi=query.value(2).toString();
      knife2Info.var=query.value(3).toString();
      knife2Info.name1=query.value(4).toString();
      knife2Info.name2=query.value(5).toString();
      knife2Info.v1=query.value(6).toFloat();
      knife2Info.v2=query.value(7).toFloat();
      knife2Info.v3=query.value(8).toFloat();
      knife2Info.v4=query.value(9).toFloat();
      knife2Info.v5=query.value(10).toFloat();
      knife2Info.v6=query.value(11).toFloat();
      knife2Info.bind=query.value(12).toString();
      Knife2List.append(knife2Info);
  }


}

void KnifeWidget::on_pushButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("批量导出磨损数据"), "./", tr("*.csv"));
      if(path == ""){return; }
      SolutionSet *solutionSet = new SolutionSet();
      if( solutionSet->exportAbrasionToCsv(path)){
         QMessageBox::information(this,"系统提示","批量导出磨损数据成功！",QMessageBox::tr("确认"));
      }else{
         QMessageBox::information(this,"系统警告","批量导出磨损数据失败！",QMessageBox::tr("确认"));
      }
}
// chenyj  update end

void KnifeWidget::on_pushButton_5_clicked()
{

    QString data=QString("?%1,%2,%3,%4")
            .arg(KnifeList[0].name2)
            .arg(KnifeList[0].name3)
            .arg(Knife2List[0].name1)
            .arg(ui->lineEdit->text());
    qDebug()<<data;
    QByteArray b=data.toLocal8Bit();
    b[0]=FCMD_TOOL_OFFSET;

    mainWindow->mysendServiceA(b,9666);

    ui->label_3->setText("发送成功");


}
