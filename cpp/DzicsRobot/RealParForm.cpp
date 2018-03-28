#include "RealParForm.h"
#include "ui_RealParForm.h"
#include "EditForm.h"
#include <QLabel>
#include <QSqlQuery>
#include "ControlProperty.h"
#include <QDebug>


QMap<QLabel*,ControlProperty*>realControlMap;


bool RealParForm::isShow = false;

RealParForm::RealParForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealParForm)
{
    ui->setupUi(this);
}

RealParForm::~RealParForm()
{
    delete ui;
}


void RealParForm::saveParameter(int height,int width)
{
   isShow = false;
   queryAllControl();
   this->setFixedHeight(height);
   qDebug()<<"height="<<height;
    QMap<QLabel*,ControlProperty*>::const_iterator it;
    for(it =realControlMap.constBegin();it !=realControlMap.constEnd();++it){
        QLabel *controlLabel = it.key();

            if(realControlMap[controlLabel]->startY + realControlMap[controlLabel]->height > height){
               this->setFixedHeight(realControlMap[controlLabel]->startY + realControlMap[controlLabel]->height+50);

               // ui->scrollAreaEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            }
            if(realControlMap[controlLabel]->startX + realControlMap[controlLabel]->width > width){
                this->setFixedWidth(realControlMap[controlLabel]->startX + realControlMap[controlLabel]->width+50);
                // ui->scrollAreaEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            }

      if(realControlMap[controlLabel]->type==3){
                QPixmap pix(realControlMap[controlLabel]->name);
                controlLabel->setPixmap(pix);
            }else{
             if(realControlMap[controlLabel]->type == 2 ){
                controlLabel->setText(realControlMap[controlLabel]->name);}
            QPalette pal;
            pal.setColor(QPalette::WindowText,QColor(realControlMap[controlLabel]->color &255,realControlMap[controlLabel]->color>>8 &255 ,realControlMap[controlLabel]->color>>16 & 255));
            controlLabel->setPalette(pal);
            QFont font;
            font.setFamily(realControlMap[controlLabel]->fontFamily);
            font.setPointSize(realControlMap[controlLabel]->fontSize);
            font.setBold(realControlMap[controlLabel]->fontBold);
            font.setItalic(realControlMap[controlLabel]->fontItalic);
            font.setStrikeOut(realControlMap[controlLabel]->fontStrikeout);
            font.setUnderline(realControlMap[controlLabel]->fontUnderline);
            controlLabel->setFont(font);
            if(realControlMap[controlLabel]->aligan == 0)
                controlLabel->setAlignment(Qt::AlignLeft);
            else if(realControlMap[controlLabel]->aligan == 1)
                controlLabel->setAlignment(Qt::AlignHCenter);
            else if(realControlMap[controlLabel]->aligan == 2)
                controlLabel->setAlignment(Qt::AlignRight);
            }
            controlLabel->setGeometry(realControlMap[controlLabel]->startX,realControlMap[controlLabel]->startY,realControlMap[controlLabel]->width, realControlMap[controlLabel]->height);
            controlLabel->show();
            if(realControlMap[controlLabel]->type == 3){
                QPixmap photo;
                if(!photo.loadFromData(realControlMap[controlLabel]->image, "PNG")){
                    controlLabel->setText(realControlMap[controlLabel]->name);
                   // qDebug()<<"¶ÁÈ¡Í¼Æ¬Ê§°Ü";
                }else
                   controlLabel->setPixmap(photo);

           }
        }
}




void RealParForm::queryAllControl()
{
    QLabel *conLabel = NULL;
    QMap<QLabel*,ControlProperty*>::const_iterator it;
    for(it = realControlMap.constBegin();it != realControlMap.constEnd();++it){
        conLabel = it.key();
        conLabel->deleteLater();
        conLabel = NULL;
    }
    realControlMap.clear();
    QSqlQuery query;
    query.exec(QString("SELECT id,name,value,unit,type,startX,startY,width,height,color,fontFamily,fontSize,fontBold,fontItalic,fontStrikeout,fontUnderline,image,align FROM EditControl ORDER BY startX"));
    while(query.next()){
        ControlProperty* controlProperty = new ControlProperty();
        controlProperty->id = query.value(0).toString();
        controlProperty->name = query.value(1).toString();
        controlProperty->value = query.value(2).toString();
        controlProperty->unit = query.value(3).toString();
        controlProperty->type = query.value(4).toInt();
        controlProperty->startX = query.value(5).toInt();
        controlProperty->startY = query.value(6).toInt();
        controlProperty->width = query.value(7).toInt();
        controlProperty->height = query.value(8).toInt();
        controlProperty->color = query.value(9).toInt();
        controlProperty->fontFamily = query.value(10).toString();
        controlProperty->fontSize = query.value(11).toInt();
        controlProperty->fontBold = query.value(12).toInt();
        controlProperty->fontItalic = query.value(13).toInt();
        controlProperty->fontStrikeout = query.value(14).toInt();
        controlProperty->fontUnderline = query.value(15).toInt();
        controlProperty->image = query.value(16).toByteArray();
        controlProperty->aligan = query.value(17).toInt();
        QLabel *controlLabel = new QLabel(this);
        realControlMap.insert(controlLabel,controlProperty);
    }
}




void RealParForm::showEvent(QShowEvent *)
{
    if(!isShow){
        saveParameter(this->parentWidget()->height(),this->parentWidget()->width());
        isShow = true;
    }
}
