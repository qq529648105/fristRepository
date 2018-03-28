#include "EditForm.h"
#include "ui_EditForm.h"
#include <QDragEnterEvent>
#include <QMenu>
#include <QLabel>
#include <QPainter>
#include "ControlProperty.h"
#include <QDebug>
#include <QSqlQuery>
#include "SysUtils.h"




//extern QMap<QLabel*,ControlProperty*>controlMap;


//ControlProperty::ControlProperty()
//{

//}




EditForm::EditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditForm)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    s_ParmeterShow = ParmeterShow::initialize();
    editLabel = NULL;
    isPressed = false;
    isMoving = false;

}

EditForm::~EditForm()
{
    delete ui;
}





void EditForm::dragEnterEvent(QDragEnterEvent *event)
{

    if(event->mimeData()->hasFormat("text/plain")) {
        event->acceptProposedAction();}
}


void EditForm::dropEvent(QDropEvent *event)
{

     int controlHeight ,controlWidth;
     int posX = event->pos().x();
     int posY = event->pos().y();
     int type;
     QString text = event->mimeData()->text();
     QLabel *controlLabel = new QLabel(text,this);
     if(text == "图像"){
        controlHeight = 100;
        controlWidth = 100;
        type = 3;

     }
     else if(text == "变量"){
             type = 0;
             controlWidth = 100;
             controlHeight = 20;

     }else if(text == "信号"){
             type = 1;
             controlWidth = 200;
             controlHeight = 20;

     }else if(text == "文本"){
             type = 2;
             controlWidth = 60;
             controlHeight = 20;
     }

     controlLabel->setGeometry(posX,posY,controlWidth,controlHeight);
     controlLabel->show();
     ControlProperty *controlProperty = new ControlProperty();
     controlProperty->id = SysUtils::uuid();
     controlProperty->name = text;
     controlProperty->type = type;
     controlProperty->startX = posX;
     controlProperty->startY = posY;
     controlProperty->width = controlWidth;
     controlProperty->height = controlHeight;
     controlProperty->color = 0;
     controlProperty->fontFamily = "宋体";
     controlProperty->fontBold = 0;
     controlProperty->fontItalic = 0;
     controlProperty->fontSize = 11;
     controlProperty->fontStrikeout= 0;
     controlProperty->fontUnderline = 0;
     controlProperty->aligan = 0;
     s_ParmeterShow->realControlMap.insert(controlLabel,controlProperty);
     QSqlQuery query;
     if(!query.exec(QString("INSERT INTO EditControl (id, name,type,startX,startY,width,height,color,fontFamily,fontSize,fontItalic,fontStrikeout,fontBold,fontUnderline,align) VALUES('%1','%2',%3,%4,%5,%6,%7,0,'宋体',9,0,0,0,0,0)").arg(controlProperty->id,text,QString::number(type),QString::number(posX),QString::number(posY),QString::number(controlWidth),QString::number(controlHeight))))
         qDebug()<<QString("INSERT INTO EditControl (id, name,type,startX,startY,width,height,color) VALUES('%1','%2',%3,%4,%5,%6,%7,0)").arg(controlProperty->id,text,QString::number(type),QString::number(posX),QString::number(posY),QString::number(controlWidth),QString::number(controlHeight));


}


void EditForm::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<s_ParmeterShow->realControlMap.count()<<" eee";
    if(event->button() != Qt::LeftButton)
        return;
    QLabel *pressLabel;
    QMap<QLabel*,ControlProperty*>::const_iterator it;
    for(it = s_ParmeterShow->realControlMap.constBegin();it != s_ParmeterShow->realControlMap.constEnd();++it){
        pressLabel = it.key();
        if(pressLabel->geometry().contains(event->pos())){
            isPressed = true;
            editLabel = pressLabel;
            lastPos = event->pos();
            this->update();
            emit s_editProperty(editLabel);
            return;

        }
    }
    editLabel = NULL;
    isPressed = false;
    this->update();
    emit s_editProperty(editLabel);


}


void EditForm::mouseMoveEvent(QMouseEvent *event)
{
    if(!isPressed)
        return;
    isMoving = true;
    newPos = event->pos();
    if(editLabel->x()+newPos.x()-lastPos.x() <=0 ||editLabel->y()+newPos.y()-lastPos.y() <=0 ||       \
       editLabel->x()+newPos.x()-lastPos.x()+editLabel->width() >this->width() || editLabel->y()+newPos.y()     \
        -lastPos.y()+editLabel->height() >this->height())
        return;
    editLabel->move(editLabel->x()+newPos.x()-lastPos.x(),editLabel->y()+newPos.y()-lastPos.y());
    lastPos = newPos;
    this->update();

}


void EditForm::mouseReleaseEvent(QMouseEvent *event)
{

    if(!isPressed | !editLabel | !isMoving)
        return;
    isMoving = false;
    QMap<QLabel*,ControlProperty*>::const_iterator it;
    qDebug()<<s_ParmeterShow->realControlMap.size();
    for(it = s_ParmeterShow->realControlMap.constBegin();it != s_ParmeterShow->realControlMap.constEnd();it++){
        if(it.key() == editLabel)
            continue;
        if(qAbs(editLabel->x() - s_ParmeterShow->realControlMap[it.key()]->startX) <15){
            qDebug()<<"x="<<s_ParmeterShow->realControlMap[it.key()]->startX;
            editLabel->move(s_ParmeterShow->realControlMap[it.key()]->startX,editLabel->y()+newPos.y()-lastPos.y());
            //break;
        }
        if(qAbs(editLabel->y() - s_ParmeterShow->realControlMap[it.key()]->startY) <15 ){
            qDebug()<<"y="<<s_ParmeterShow->realControlMap[it.key()]->startY;
            editLabel->move(editLabel->x()+newPos.x()-lastPos.x(),s_ParmeterShow->realControlMap[it.key()]->startY);
            //break;
        }
    }
    this->update();
    s_ParmeterShow->realControlMap[editLabel]->startX = editLabel->x();
    s_ParmeterShow->realControlMap[editLabel]->startY = editLabel->y();

    QSqlQuery query;
    if(!query.exec(QString("UPDATE EditControl SET startX= %1, startY= %2 WHERE id ='%3'").arg(QString::number(editLabel->x()),QString::number(editLabel->y()),s_ParmeterShow->realControlMap[editLabel]->id))){
        qDebug()<<QString("UPDATE EditControl SET startX= %1, startY= %2 WHERE id ='%3'").arg(QString::number(editLabel->x()),QString::number(editLabel->y()),s_ParmeterShow->realControlMap[editLabel]->id);
    }
    emit s_editProperty(editLabel);

}

void EditForm::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    QPen pen;
    pen.setStyle(Qt::DashDotDotLine);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    if(editLabel){
      painter.drawRect(editLabel->x()-1,editLabel->y()-1,editLabel->width()+2,editLabel->height()+2);

    }
    event->accept();
}




void EditForm::removeControl()
{

    QSqlQuery query;
    query.exec(QString("DELETE FROM EditControl WHERE id = '%1'").arg(s_ParmeterShow->realControlMap[editLabel]->id));
    s_ParmeterShow->realControlMap.remove(editLabel);
    if(editLabel){
      editLabel->deleteLater();
      editLabel = NULL;
      isPressed = false;
      this->update();
    }
}

