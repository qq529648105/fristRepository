#include "ParmeterShow.h"
#include "ControlProperty.h"
#include  <QLabel>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>


ParmeterShow* ParmeterShow::parmeterShow = NULL;

ParmeterShow *ParmeterShow::initialize()
{

    if(!parmeterShow){
       parmeterShow = new ParmeterShow();
    }
    return parmeterShow;
}

ParmeterShow::ParmeterShow(QObject *parent):
    QObject(parent)
{
    realControlMap.clear();
    variateLabelMap.clear();
    signalLabelMap.clear();
}

void ParmeterShow::queryAllControl(QWidget *widget)
{
    QLabel *conLabel = NULL;
    QMap<QLabel*,ControlProperty*>::const_iterator it;
    for(it = realControlMap.constBegin();it != realControlMap.constEnd();++it){
        conLabel = it.key();
        conLabel->deleteLater();
        conLabel = NULL;
    }
    realControlMap.clear();
    variateLabelMap.clear();
    signalLabelMap.clear();
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
        QLabel *controlLabel = new QLabel(widget);
        realControlMap.insert(controlLabel,controlProperty);
        if(controlProperty->type == 0)
            variateLabelMap.insertMulti(controlProperty->name,controlLabel);
        else if(controlProperty->type == 1)
            signalLabelMap.insertMulti(controlProperty->name,controlLabel);

    }



}



void ParmeterShow::loadParmeterControl(QWidget *widget)
{
    widget->setFixedHeight(widget->parentWidget()->height());
    widget->setFixedWidth(widget->parentWidget()->width());
  // qDebug()<<widget->height()<<widget->width()<<widget->objectName();
    QMap<QLabel*,ControlProperty*>::const_iterator it;
     for(it =realControlMap.constBegin();it !=realControlMap.constEnd();++it){
         QLabel *controlLabel = it.key();

             if(realControlMap[controlLabel]->startY + realControlMap[controlLabel]->height > widget->height()){
               widget->setFixedHeight(realControlMap[controlLabel]->startY + realControlMap[controlLabel]->height+50);

             }
             if(realControlMap[controlLabel]->startX + realControlMap[controlLabel]->width > widget->width()){
                // if(widget->objectName() == "editForm" )
                 widget->setFixedWidth(realControlMap[controlLabel]->startX + realControlMap[controlLabel]->width+50);

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
                 controlLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
             else if(realControlMap[controlLabel]->aligan == 1)
                 controlLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
             else if(realControlMap[controlLabel]->aligan == 2)
                 controlLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
             }
             controlLabel->setGeometry(realControlMap[controlLabel]->startX,realControlMap[controlLabel]->startY,realControlMap[controlLabel]->width, realControlMap[controlLabel]->height);
             controlLabel->show();
             if(realControlMap[controlLabel]->type == 3){
                 QPixmap photo;
                 if(!photo.loadFromData(realControlMap[controlLabel]->image, "PNG")){
                     controlLabel->setText(realControlMap[controlLabel]->name);
                    // qDebug()<<"��ȡͼƬʧ��";
                 }else
                    controlLabel->setPixmap(photo);

            }
         }



}



/************************
 *ˢ�½������
 *parName :�������ź���
 *type   ������ 0����  1�ź�
 *value  ����ֵ �ַ���
 *
 **********************/

void ParmeterShow::setParmeterValue(const QString &parName, const QString &value, int type)
{
    QList<QLabel*>labelList;
    if(type == 0)
       labelList  = variateLabelMap.values(parName);
    else if(type == 1)
        labelList  = signalLabelMap.values(parName);

    for(int i = 0; i<labelList.count(); ++i)
        labelList.at(i)->setText(value);
    labelList.clear();
}



/********************
�ӿڣ� ˢ����ʾ�༭����

************************/
void ParmeterShow::showParmeterControl(QWidget *widget)
{

    queryAllControl(widget);
    loadParmeterControl(widget);



}



/******************************
 �õ���Ҫ��ʾ��������
 type 0 ����  1�ź�
 ˢ����ʾ�����õ����¶���
 **********************************/
QList<QString> ParmeterShow::getParmeterList(int type)
{
    QList<QString>list;
    if(type == 0)
        list = variateLabelMap.keys();
    else if(type ==1)
        list = signalLabelMap.keys();
    return list;


}
