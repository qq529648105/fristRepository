#include "RobotExport.h"
#include "ui_RobotExport.h"
#include <QSqlQuery>
#include "RobotObject.h"
#include <QListWidgetItem>
#include <QMap>
#include <QDebug>

QMap<int,RobotObject*>robotChoseMap;

QString RobotExport::robotId = "";

RobotExport::RobotExport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RobotExport)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("选择导出机器人"));
    ui->listWidgetRobot->setViewMode(QListView::IconMode);
    ui->listWidgetRobot->setDragEnabled(false);
    ui->listWidgetRobot->setSpacing(30);
    ui->listWidgetRobot->setIconSize(QSize(120,120));
    showRobot();
    connect(ui->listWidgetRobot,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(choseRobot(QListWidgetItem*)));
}

RobotExport::~RobotExport()
{
    delete ui;
}

void RobotExport::on_pushButtonAffirm_clicked()
{
    int robotIndex = ui->listWidgetRobot->currentRow();
    if(robotIndex>=0)
        robotId = robotChoseMap[robotIndex]->id;
    else
        robotId = "";
    this->accept();
}

void RobotExport::on_pushButtoncancel_clicked()
{
    robotId = "";
    this->done(0);
}


void RobotExport::showRobot()
{
    int index =0;
    QSqlQuery query;
    query.exec(QString("SELECT id,name,type,preview FROM RobotLibrary"));
    while(query.next()){
        RobotObject *robotObject = new RobotObject(this);
        robotObject->id = query.value(0).toString();
        robotObject->name = query.value(1).toString();
        robotObject->type = query.value(2).toInt();
        robotObject->preview = query.value(3).toString();
        robotChoseMap.insert(index,robotObject);
//        QListWidgetItem*item = new QListWidgetItem(QIcon(robotObject->preview),robotObject->name);
        ui->listWidgetRobot->addItem(new QListWidgetItem(QIcon(robotObject->preview),robotObject->name));
        index++;
    }

}


void RobotExport::choseRobot(QListWidgetItem *item)
{

}
