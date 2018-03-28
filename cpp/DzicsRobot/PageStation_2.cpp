#include "PageStation_all.h"

void PageStation::on_pushButton_6_clicked()
{
    mainWindow->connectService();

}
//updated by zhugz

void PageStation::on_pushButton_clicked()
{
    mainWindow->connectService(mainWindow->curRobotIndex);

}


bool PageStation::querySafeArea()
{
    QSqlQuery quer;
    QString sql = QString("select * from safeArea where solutionId='%1' and robotIndex=%2").arg(currentStationId).arg(ui.cur_robot->currentIndex()+1);

    if(!quer.exec(sql))
    {
        return false;
    }

    ui.com_area->clear();
    int n=0;
    while(quer.next())
    {
        if(n==0)
        {
            updateArea(quer.value(0).toString());
            curArea=quer.value(0).toString();
        }

        areaList[quer.value(0).toString()]=quer.value(4).toString();
        ui.com_area->addItem(QString::number(++n),quer.value(0).toString());

    }

    return true;
}

bool PageStation::updateArea(QString id)
{
    QSqlQuery quer;

    QString sql = QString("select * from safeArea where id='%1'").arg(id);


    if(!quer.exec(sql))
    {

        return false;
    }
    if(!quer.next())
        return false;
    QString str=quer.value(3).toString();
    QStringList list=str.split("|");

    if(list.count()>2)
    {
    ui.safeTxt1->setText(list[0]);
    ui.safeTxt2->setText(list[1]);
    ui.safeTxt3->setText(list[2]);

    }

    return true;
}

void PageStation::safeAreaSend(int index)
{
    QSqlQuery query;
    QString str;
    QString sql=QString("select * from safeArea where robotIndex='%1' and solutionId='%2'").arg(index).arg(currentStationId);
    if(!query.exec(sql))
        return;
    int n=0;
    while(query.next())
    {
        ++n;
        str+=QString("{%1}").arg(query.value(3).toString());
    }

    QString data1="###"+str;
    qDebug()<<"data1"<<data1;
    QByteArray b=data1.toLocal8Bit();
    b[0]=CMD_SAFEAREA_DATA;
    b[1]=index;
    b[2]=n;
    mainWindow->mysendService(b.data(),cur_areaIndex+1);
}

void PageStation::showAllArea()
{
    QSqlQuery query;

    QString sql=QString("select * from safeArea where solutionId='%1'").arg(currentStationId);
    if(!query.exec(sql))
        return;

    while(query.next())
    {
        QStringList var=query.value(4).toString().split(",");
        if(var.size()==6)
        {
            //qDebug()<<"aaa"<<var[0].toInt()<<var[1].toInt()<<var[2].toInt();
            pageViewer->createArea(query.value(0).toString(),var[0].toInt(),var[1].toInt(),var[2].toInt(),var[3].toInt(),var[4].toInt(),var[5].toInt());

        }
    }
}


void PageStation::on_pushButton_2_clicked()
{//send

    safeAreaSend(ui.cur_robot->currentIndex()+1);
    ui.label_areaInfo->setText("已发送！");

}

void PageStation::on_pushButton_7_clicked()
{
    int len1=ui.len1->text().toInt();
    int len2=ui.len2->text().toInt();
    int len3=ui.len3->text().toInt();

    if(!len1||!len2||!len3)
    {
        ui.label_areaInfo->setText("error:请输入正确的长宽高");
        return;
    }


    QString str=ui.safeTxt1->text()+"|"+ui.safeTxt2->text()+"|"+ui.safeTxt3->text();
    QSqlQuery quer;
    QString uuid=SysUtils::uuid();
    QString obj=QString("%1,%2,%3,60,60,0").arg(len1).arg(len2).arg(len3);
    if ( quer.exec(QString("INSERT INTO safeArea(id,solutionId,robotIndex,value1,value2) VALUES ('%1','%2',%3,'%4','%5')").arg(uuid).arg(currentStationId).arg(ui.cur_robot->currentIndex()+1).arg(str).arg(obj)) )
    {
        ui.label_areaInfo->setText("增加成功！");

        pageViewer->createArea(uuid,len1,len2,len3,60,60,0);
        ui.com_area->addItem(QString::number(ui.com_area->count()+1),uuid);

        ui.com_area->setCurrentIndex(ui.com_area->count()-1);


    }
    else
    {
        ui.label_areaInfo->setText("增加失败！");

    }

}

void PageStation::on_pushButton_8_clicked()
{
    QSqlQuery quer;
    if ( quer.exec(QString("delete from safeArea where id='%1'").arg(ui.com_area->itemData(cur_areaIndex).toString())))
    {

        pageViewer->removeArea(curArea);
        ui.com_area->removeItem(ui.com_area->count()-1);
        ui.com_area->setCurrentIndex(0);
        ui.label_areaInfo->setText("删除成功！");

    }

}

void PageStation::on_com_area_currentIndexChanged(int index)
{

     cur_areaIndex=index;
     curArea=ui.com_area->itemData(index).toString();
     updateArea(ui.com_area->itemData(index).toString());
}

void PageStation::on_pushButton_9_clicked()
{
    //save

    QString inputData=ui.safeTxt1->text()+"|"+ui.safeTxt2->text()+"|"+ui.safeTxt3->text();

    QSqlQuery query;
    int len1=ui.len1->text().toInt();
    int len2=ui.len2->text().toInt();
    int len3=ui.len3->text().toInt();

    if(!len1||!len2||!len3)
    {
        ui.label_areaInfo->setText("error:请输入正确的长宽高");
        return;
    }

    QString obj=QString("%1,%2,%3,%4,%5,%6").arg(len1).arg(len2).arg(len3).arg(areaX).arg(areaY).arg(areaZ);
    QString str1=QString("UPDATE safeArea SET value1='%1', value2='%2' WHERE id='%3'").arg(inputData).arg(obj).arg(ui.com_area->itemData(cur_areaIndex).toString());

    if ( query.exec(str1)) {

        ui.label_areaInfo->setText("更新成功！");
    } else {
        ui.label_areaInfo->setText("更新失败！");

    }
}








void PageStation::on_ySlider_valueChanged(int value)
{

    areaX=value*10*ui.moveRatio->text().toInt();

    pageViewer->moveRect(curArea,areaX,areaY,areaZ);
}

void PageStation::on_xSlider_valueChanged(int value)
{
    areaY=value*10*ui.moveRatio->text().toInt();
    pageViewer->moveRect(curArea,areaX,areaY,areaZ);
}

void PageStation::on_zSlider_valueChanged(int value)
{
    areaZ=value*10*ui.moveRatio->text().toInt();
    pageViewer->moveRect(curArea,areaX,areaY,areaZ);
}

void PageStation::on_pushButton_11_clicked()
{
    mainWindow->rebootSeviceFunc();
}

//end


