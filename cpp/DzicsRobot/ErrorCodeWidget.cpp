#include "ErrorCodeWidget.h"
#include "ui_ErrorCodeWidget.h"
#include <QSqlQuery>
#include "SysUtils.h"
#include <QMessageBox>
#include <QDebug>

const QString errorTable = "AlarmLibrary";
const QString fKeyId = "id";
const QString fCode = "code";
const QString fTitle ="title";
const QString fDesc ="desc";

ErrorCodeWidget::ErrorCodeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ErrorCodeWidget)
{
    ui->setupUi(this);
    InitTableWidget();
    ReadInfo();
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),SLOT(EditErrorItem(int,int)));

}

ErrorCodeWidget::~ErrorCodeWidget()
{
    delete ui;
}


void ErrorCodeWidget::InitTableWidget()
{
    QFont font;
    font.setPixelSize(16);
    ui->tableWidget->setFont(font);
    ui->tableWidget->horizontalHeader()->setFont(font);
    for(int i = 0; i<3; i++)
        ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()/3);
    ui->tableWidget->setColumnHidden(3,true);
    for(int i =0; i<3; i++)
        ui->tableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->horizontalHeader()->setFixedHeight(40);
    ui->tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->horizontalScrollBar()->setVisible(false);
//    ui->tableWidget->setSortingEnabled(true);
}


void ErrorCodeWidget::paintEvent(QPaintEvent *)
{
    for(int i = 0; i<3; i++)
        ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()/3);

}


void ErrorCodeWidget::ReadInfo()
{
    GetAllCode();
    for(int i = 0; i< erronList.count(); ++i)
    {

        ui->tableWidget->insertRow(i);
        ui->tableWidget->setRowHeight(i,60);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(/*erronList.at(i).eCode*/));
        ui->tableWidget->item(i,0)->setData(Qt::DisplayRole,QVariant(erronList.at(i).eCode.toInt()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(erronList.at(i).eTitle));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(erronList.at(i).eDesc));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem( erronList.at(i).PriKey));
        for(int cout=0; cout<4; cout++)
         ui->tableWidget->item(i,cout)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}


void ErrorCodeWidget::GetAllCode()
{
    ERRORINFO erronInfo;
    erronList.clear();
    QSqlQuery query;
    query.exec(QString("select %1,%2,%3,%4 from %5 order by %6").arg(fCode,fTitle, fDesc, fKeyId, errorTable,fCode));

    while(query.next())
    {
           erronInfo.eCode = query.value(0).toString();
           erronInfo.eTitle = query.value(1).toString();
           erronInfo.eDesc = query.value(2).toString();
           erronInfo.PriKey = query.value(3).toString();
           erronList.append(erronInfo);
    }


}


void ErrorCodeWidget::EditErrorItem(int x, int y)
{
    ui->tableWidget->editItem(ui->tableWidget->item(x,y));

}


void ErrorCodeWidget::GetErrorItem(int x, int y)
{
    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));

//   ui->tableWidget->setSortingEnabled(false);
    QString key = ui->tableWidget->item(x,3)->text();
    QString value = ui->tableWidget->item(x,y)->text();
    QSqlQuery query;
    QString field;
    switch(y)
    {
        case 0:
        field = fCode;
        if(value.contains(QRegExp("[^0-9]")))
        {
            QMessageBox::warning(this,"失败",tr("告警代码仅支持数字格式"),QMessageBox::tr("确认"));
            query.exec(QString("select %1 from %2 where %3 = '%4'").arg(fCode, errorTable,fKeyId,key));
            while(query.next())
                value = query.value(0).toString();
             ui->tableWidget->item(x,y)->setData(Qt::DisplayRole,QVariant(value.toInt()));
             connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));
//              ui->tableWidget->setSortingEnabled(true);
            return;


        }
        ui->tableWidget->item(x,y)->setData(Qt::DisplayRole,QVariant(value.toInt()));
        break;


    case 1:
          field = fTitle;
        break;
    case 2:
        field = fDesc;
        break;
    default:
        break;
    }

    if(query.exec(QString("update %1 set %2 = '%3' where %4 = '%5'").arg(errorTable,field,value,fKeyId,key)))
    {
        qDebug()<<"更新成功";
    }
    else
    {
         QMessageBox::warning(this,"失败",tr("告警代码有重复,编辑失败"),QMessageBox::tr("确认"));
         query.exec(QString("select %1 from %2 where %3 = '%4'").arg(fCode, errorTable,fKeyId,key));
         while(query.next())
             value = query.value(0).toString();
         ui->tableWidget->item(x,y)->setData(Qt::DisplayRole,QVariant(value.toInt()));
        qDebug()<<"更新失败";
    }
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));
//    ui->tableWidget->setSortingEnabled(true);
}


void ErrorCodeWidget::AddErrorCode()
{
     disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));
    QString keyid = SysUtils::uuid();//QUuid::createUuid().toString();//
    QSqlQuery query;
    if(query.exec(QString("insert into %1 (%2,%3) values ('%4', '')").arg(errorTable,fKeyId,fCode,keyid)))
    {
    qDebug()<<QString("insert into %1 (%2,%3) values ('%4', '')").arg(errorTable,fKeyId,fCode,keyid);


    ui->tableWidget->insertRow(0);
    ui->tableWidget->setRowHeight(0,60);
    for(int i = 0; i<4; i++)
    {

        ui->tableWidget->setItem(0,i,new QTableWidgetItem());
        ui->tableWidget->item(0,i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    }
    ui->tableWidget->item(0,0)->setData(Qt::DisplayRole, QVariant(QString("").toInt()));
    ui->tableWidget->selectRow(0);
    ui->tableWidget->item(0,3)->setText(keyid);
    }
    else
    {
//        QMessageBox::warning(this,"失败",tr("有告警代码为0"),QMessageBox::tr("确认"));
        QSqlQuery query;
        if(query.exec(QString("create table AlarmLibrary(id char(24) not null,code varchar(256),title varchar(256),desc varchar(512),primary key (id))")))
        {
            AddErrorCode();
        } else {
            QMessageBox::warning(this,"失败",tr("有告警代码为0"),QMessageBox::tr("确认"));
        }


    }
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));

}



void ErrorCodeWidget::RemoveErrorCode()
{

    int row = ui->tableWidget->currentRow();
    if(row < 0)
        return;
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(tr("提示"));
    msgBox->setText(tr("确认删除该条记录?"));
    QPushButton *affirmButton = msgBox->addButton(tr("确认"), QMessageBox::ActionRole);
    QPushButton *cancalButton = msgBox->addButton(tr("取消"),QMessageBox::ActionRole);
    msgBox->exec();
    if(msgBox->clickedButton() == (QAbstractButton*)affirmButton)
    {
        QString KeyId = ui->tableWidget->item(row,3)->text();
        ui->tableWidget->removeRow(row);
        if(row >0)
        {
            ui->tableWidget->selectRow(row-1);
        }
        else
        {
            ui->tableWidget->selectRow(0);
        }
        //DelData(KeyId );
        QSqlQuery query;
        query.exec(QString("delete from %1 where %2 = '%3'").arg(errorTable,fKeyId,KeyId));


    }
    else if(msgBox->clickedButton() == (QAbstractButton*)cancalButton)
        return;
}

/******************update8 **********************/
void ErrorCodeWidget::refreshErrorCode()
{
    ui->tableWidget->clearContents();
    while(ui->tableWidget->rowCount()){ui->tableWidget->removeRow(0);}
    qDebug()<<ui->tableWidget->rowCount();
    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));
    disconnect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(EditErrorItem(int,int)));
    ReadInfo();
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(GetErrorItem(int,int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(EditErrorItem(int,int)));
}
/*****************************end************************/



bool ErrorCodeWidget::exportErrorCode(const QString &path)
{


   /*if(solutionSet->exportSolution(path,ALARMCODEEXPORT))
    return true;
   else
     return false;*/
    if(solutionSet->exportErrorCodeToCsv(path))
        return true;
    else
        return false;
}


bool ErrorCodeWidget::importErrorCode(const QString &path)
{/*
    if(solutionSet->importSolution(path,ALARMCODEIMPORT))
        return true;
    else
        return false;*/
    if(solutionSet->importCsvToErrorCode(path))
        return true;
    else
        return false;
}
