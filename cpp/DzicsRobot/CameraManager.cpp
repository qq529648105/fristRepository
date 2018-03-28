#include "CameraManager.h"
#include "ui_CameraManager.h"
#include "CameraProperty.h"
#include "SysUtils.h"
#include <QComboBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSpinBox>
#include <QMessageBox>
#include <QSqlError>

extern QMap<QString,CameraProperty*>cameraMap;

QMap<int,QString>snMap;

CameraManager::CameraManager(QString id, QWidget *parent):
    QDialog(parent,Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint),

    ui(new Ui::CameraManager)
{
    ui->setupUi(this);
    solutionId = id;
    this->setWindowTitle(tr("相机属性管理"));
    ui->tableWidgetCamera->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidgetCamera->hideColumn(0);
    ui->tableWidgetCamera->verticalHeader()->setVisible(false);

    initCameraTable();

   // connect(ui->tableWidgetCamera,SIGNAL(cellChanged(int,int)),this,SLOT(updateValue(int,int)));

}

CameraManager::~CameraManager()
{
    delete ui;
}

// cxs update
//void CameraManager::addCameraRow(const QString &cameraId)
//{
//    ui->tableWidgetCamera->insertRow(ui->tableWidgetCamera->rowCount());
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,0, new QTableWidgetItem(cameraId));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,1, new QTableWidgetItem(cameraMap[cameraId]->name));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,2, new QTableWidgetItem(cameraMap[cameraId]->model));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,3, new QTableWidgetItem(QString::number(cameraMap[cameraId]->code)));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,4, new QTableWidgetItem(QString::number(cameraMap[cameraId]->maxExposure)));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,5, new QTableWidgetItem(QString::number(cameraMap[cameraId]->minExposure)));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,6, new QTableWidgetItem(QString::number(cameraMap[cameraId]->maxGain)));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,7, new QTableWidgetItem(QString::number(cameraMap[cameraId]->minGain)));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,8, new QTableWidgetItem(QString::number(cameraMap[cameraId]->cameraWidth)));
//    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,9, new QTableWidgetItem(QString::number(cameraMap[cameraId]->cameraHeight)));

//}
void CameraManager::addCameraRow(const QString &cameraId)
{
    ui->tableWidgetCamera->insertRow(ui->tableWidgetCamera->rowCount());
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,0, new QTableWidgetItem(cameraId));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,1, new QTableWidgetItem(cameraMap[cameraId]->name));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,2, new QTableWidgetItem(cameraMap[cameraId]->model));

    //  ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,3, new QTableWidgetItem(QString::number(cameraMap[cameraId]->code)));
    QComboBox *comBox1=new QComboBox;
    comBox1->addItem("MV 120M");
    comBox1->addItem("MV 500M");
    comBox1->addItem("Manta G-201b");
    comBox1->addItem("USB 640*480");
    comBox1->addItem("Hik Cam");

    comBox1->setCurrentIndex(cameraMap[cameraId]->code);
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,3,new QTableWidgetItem());
    ui->tableWidgetCamera->setCellWidget(ui->tableWidgetCamera->rowCount()-1,3,comBox1);

    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,4, new QTableWidgetItem(QString::number(cameraMap[cameraId]->maxExposure)));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,5, new QTableWidgetItem(QString::number(cameraMap[cameraId]->minExposure)));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,6, new QTableWidgetItem(QString::number(cameraMap[cameraId]->maxGain)));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,7, new QTableWidgetItem(QString::number(cameraMap[cameraId]->minGain)));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,8, new QTableWidgetItem(QString::number(cameraMap[cameraId]->cameraWidth)));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,9, new QTableWidgetItem(QString::number(cameraMap[cameraId]->cameraHeight)));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,10, new QTableWidgetItem(QString::number(cameraMap[cameraId]->picNum)));
    ui->tableWidgetCamera->setItem(ui->tableWidgetCamera->rowCount()-1,11, new QTableWidgetItem(QString::number(cameraMap[cameraId]->interval)));

}



void CameraManager::initCameraTable()
{
    snMap.clear();
    QMap<QString,CameraProperty*>::iterator it;
    for(it = cameraMap.begin(); it!=cameraMap.end(); ++it){
        QString cameraId = it.key();

        addCameraRow(cameraId);
        snMap.insert(cameraMap[cameraId]->sn,cameraId);
    }

}

void CameraManager::on_ButtonAdd_clicked()
{
   // disconnect(ui->tableWidgetCamera,SIGNAL(cellChanged(int,int)),this,SLOT(updateValue(int,int)));
    CameraProperty *cameraProperty  = new CameraProperty(this);
    QString cameraId =  SysUtils::uuid();
    cameraProperty->id = cameraId;
    cameraProperty->name ="tp"+QString::number(cameraMap.size()+1);
    cameraProperty->model ="model",
    cameraProperty->code = 0;
    cameraProperty->maxGain = 0;
    cameraProperty->maxExposure =0;
    cameraProperty->minExposure =0;
    cameraProperty->minGain = 0;
    cameraProperty->cameraWidth =0;
    cameraProperty->cameraHeight =0;
    cameraProperty->picNum =0;
    cameraProperty->interval =0;
    int index;
    for(index=1; index<100; index++ ){
        if(!snMap.contains(index))
            break;
    }
    cameraProperty->sn = index;

  //  connect(ui->tableWidgetCamera,SIGNAL(cellChanged(int,int)),this,SLOT(updateValue(int,int)));
    QSqlQuery query;
   // query.prepare(QString("INSERT INTO CameraProperty (id,name,model,code,maxExposure,maxGain,minExposure,minGain,cameraWidth,cameraHeight,sn,solutionId) VALUES(?,?,?,?,?,?,?,?,?,?,?,?)"));
     query.prepare(QString("INSERT INTO CameraProperty (id,sn,solutionId,name,model,code,maxExposure,maxGain,minExposure,minGain,cameraWidth,cameraHeight,picNum,interval) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)"));
    query.addBindValue(cameraId);
     query.addBindValue(cameraProperty->sn);
     query.addBindValue(solutionId);
    query.addBindValue(cameraProperty->name);
    query.addBindValue(cameraProperty->model);
    query.addBindValue(cameraProperty->code);
    query.addBindValue( cameraProperty->maxExposure);
    query.addBindValue(cameraProperty->maxGain);
    query.addBindValue(cameraProperty->minExposure );
    query.addBindValue(cameraProperty->minGain);
   query.addBindValue(cameraProperty->cameraWidth);
   query.addBindValue(cameraProperty->cameraHeight);
   query.addBindValue(cameraProperty->picNum);
   query.addBindValue(cameraProperty->interval);
    if(!query.exec()){
        qDebug()<<query.lastError().text()<<query.lastError().number()<<query.lastQuery();
        return;
    }

    cameraMap.insert(cameraId,cameraProperty);
    snMap.insert(cameraProperty->sn,cameraId);
    addCameraRow(cameraId);
    emit signalAddCamera(cameraId);
}

void CameraManager::on_ButtonDel_clicked()
{
    if(ui->tableWidgetCamera->currentRow()<0)
        return;
    disconnect(ui->tableWidgetCamera,SIGNAL(cellChanged(int,int)),this,SLOT(updateValue(int,int)));
    QString id = ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),0)->text();
    cameraMap.remove(id);
    ui->tableWidgetCamera->removeRow(ui->tableWidgetCamera->currentRow());
    QSqlQuery query;
    if(!query.exec(QString("DELETE FROM CameraProperty WHERE id ='%1'").arg(id)))
        qDebug()<<QString("DELETE FROM CameraProperty WHERE id ='%1'").arg(id);
     connect(ui->tableWidgetCamera,SIGNAL(cellChanged(int,int)),this,SLOT(updateValue(int,int)));
     emit signalDelCamera(id);


}

void CameraManager::on_ButtonUpdate_clicked()
{

    if(ui->tableWidgetCamera->currentRow()<0)
        return;
    int maxExposure = ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),4)->text().toInt();
    int minExposure = ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),5)->text().toInt();
    int maxGain=   ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),6)->text().toInt();
    int minGain =  ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),7)->text().toInt();
    if(maxExposure < minExposure || maxGain < minGain){
        QMessageBox::warning(NULL,"系统提示",tr("最大值最小值范围设置错误，请重新设置!"),QMessageBox::tr("确认"));
        return;
    }

    QComboBox *comBox=(QComboBox*)ui->tableWidgetCamera->cellWidget(ui->tableWidgetCamera->currentRow(),3);
    QString cameraId = ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),0)->text();
    cameraMap[cameraId]->name = ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),1)->text();
    cameraMap[cameraId]->model =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),2)->text();
    //update by zhugz
    cameraMap[cameraId]->code = comBox->currentIndex();//ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),3)->text().toInt();
    //end
    cameraMap[cameraId]->maxExposure =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),4)->text().toInt();
    cameraMap[cameraId]->minExposure =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),5)->text().toInt();
    cameraMap[cameraId]->maxGain =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),6)->text().toInt();
    cameraMap[cameraId]->minGain = ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),7)->text().toInt();
    cameraMap[cameraId]->cameraWidth =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),8)->text().toInt();
    cameraMap[cameraId]->cameraHeight =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),9)->text().toInt();
    cameraMap[cameraId]->picNum =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),10)->text().toInt();
    cameraMap[cameraId]->interval =ui->tableWidgetCamera->item(ui->tableWidgetCamera->currentRow(),11)->text().toInt();
    if(cameraMap[cameraId]->exposure > maxExposure)
        cameraMap[cameraId]->exposure = maxExposure;
    else if(cameraMap[cameraId]->exposure <minExposure)
        cameraMap[cameraId]->exposure = minExposure;

    if(cameraMap[cameraId]->gain >maxGain)
        cameraMap[cameraId]->gain = maxGain;
    else if(cameraMap[cameraId]->gain <minGain)
        cameraMap[cameraId]->gain = minGain;
    emit signalUpdateCamera(cameraId);
    QSqlQuery query;
    query.prepare(QString("UPDATE CameraProperty SET name=?,model=?,code=?,maxExposure=?,maxGain=?,minExposure=?,minGain=?,cameraWidth=?,cameraHeight=?,exposure=?,gain=?,picNum=?,interval=? WHERE id='%1'").arg(cameraId));
    query.addBindValue(cameraMap[cameraId]->name);
    query.addBindValue(cameraMap[cameraId]->model);
    query.addBindValue(cameraMap[cameraId]->code);
    query.addBindValue(cameraMap[cameraId]->maxExposure);
    query.addBindValue(cameraMap[cameraId]->maxGain);
    query.addBindValue(cameraMap[cameraId]->minExposure);
    query.addBindValue(cameraMap[cameraId]->minGain);
    query.addBindValue(cameraMap[cameraId]->cameraWidth);
    query.addBindValue(cameraMap[cameraId]->cameraHeight);
    query.addBindValue(cameraMap[cameraId]->exposure);
    query.addBindValue(cameraMap[cameraId]->gain);
    query.addBindValue(cameraMap[cameraId]->picNum);
    query.addBindValue(cameraMap[cameraId]->interval);
    query.exec();

}

void CameraManager::updateValue(int row, int column)
{


}
