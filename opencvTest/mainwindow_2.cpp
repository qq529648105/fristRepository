//#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include <QDebug>
//#include <QMessageBox>
//#include <vector>
//#include <QByteArray>
//#include <QDir>
//#include <math.h>
//#include <QDateTime>
//#include <QListView>
//#include <QDesktopServices>
//#include "opencv300.h"
//#include <QProcess>
#include "mainwindow.h"
#include "mainwindow_all.h"
void mySort(Point p[],int num)
{
    Point tmp2;
    int index,tmp;

    for(int i=0;i<num;i++)
    {
        tmp=p[i].y;
        index=i;
        for(int j=i+1;j<num;j++)
        {
            if(tmp>p[j].y)
            {
                index=j;
                tmp=p[j].y;
            }
        }

        if(index!=i)
        {
            tmp2=p[i];
            p[i]=p[index];
            p[index]=tmp2;
        }

    }
}

void MainWindow::createXml(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(instruction);

    QDomElement root=doc.createElement("root");

    QDomElement checkBox=doc.createElement("checkBox");
    QDomElement checkBox1=doc.createElement("checkBox1");
    QDomElement checkBox2=doc.createElement("checkBox2");
    QDomElement fileIndex=doc.createElement("fileIndex");
    QDomElement path=doc.createElement("path");
    QDomElement checkBoxDll=doc.createElement("checkBoxDll");
    text=doc.createTextNode("0");
    checkBox.appendChild(text);

    text=doc.createTextNode("1");
    checkBox1.appendChild(text);
    text=doc.createTextNode("1");
    checkBox2.appendChild(text);

    text=doc.createTextNode("0");
    fileIndex.appendChild(text);

    text=doc.createTextNode("./");
    path.appendChild(text);

    text=doc.createTextNode("0");
    checkBoxDll.appendChild(text);

    root.appendChild(checkBox);
    root.appendChild(checkBox1);
    root.appendChild(checkBox2);
    root.appendChild(fileIndex);
    root.appendChild(path);
    root.appendChild(checkBoxDll);
    doc.appendChild(root);

    QTextStream out(&file);
    doc.save(out,4);
    file.close();
}


void MainWindow::readXml(QString fileName,QStringList &nodeList,QStringList &txtList)
{

    QFile file(fileName);
    if(!file.exists())
    {
        createXml(fileName);
    }
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    if (!doc.setContent(&file))
    {\
        qDebug()<<"set error";
        return;
    }

    file.close();
    for(int i=0;i<nodeList.size();i++)
    {
        QDomNode node= doc.elementsByTagName(nodeList[i]).at(0);
        txtList.append(node.toElement().text());


    }

}
void MainWindow::updateXml(QString fileName,QString node1,QString txt)
{
    QFile file(fileName);
    if(!file.exists())
    {
        createXml(fileName);
    }
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return ;
    }
    QDomDocument doc;

    if (!doc.setContent(&file))
    {\
        qDebug()<<"set error";
        return;
    }
    file.close();
    for(int i=0;i<nodeList.size();i++)
    {
        QDomNode node= doc.elementsByTagName(node1).at(0);
        node.firstChild().setNodeValue(txt);
    }


    if(!file.open(QIODevice::WriteOnly))
        qDebug() << "open for add error!";


    QTextStream out(&file);
    doc.save(out,4);
    file.close();

}
void MainWindow::updateXml(QString fileName,QStringList &nodeList,QStringList &txtList)
{
    QFile file(fileName);
    if(!file.exists())
    {
        createXml(fileName);
    }
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return ;
    }
    QDomDocument doc;
    QDomText text;
    if (!doc.setContent(&file))
    {\
        qDebug()<<"set error";
        return;
    }
    file.close();
    for(int i=0;i<nodeList.size();i++)
    {
        QDomNode node= doc.elementsByTagName(nodeList[i]).at(0);
        node.firstChild().setNodeValue(txtList[i]);
    }


    if(!file.open(QIODevice::WriteOnly))
        qDebug() << "open for add error!";


    QTextStream out(&file);
    doc.save(out,4);
    file.close();

}
void test3(InputArray contour1)
{

    vector<Point> v1=Mat_<Point>(contour1.getMat(),false);
    qDebug()<<v1.size();
}

int calcGrayArea(Mat &src,Rect &roi,int grayVal)
{
    int num=0;
    Mat m=src(roi);
    for(int i=0;i<m.cols;i++)
    {
        for(int j=0;j<m.rows;j++)
        {
            if(m.at<uchar>(j,i)<grayVal)
            {
                circle(g_dstImage2,Point(i+roi.x,j+roi.y),1,Scalar(255,0,0));
                ++num;
            }
        }
    }
    return num;
}


void MainWindow::on_pushButton_70_clicked()
{

    string s11=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s11,0);
    g_dstImage2=imread(s11,1);
    qDebug()<<calcGrayArea(g_dstImage1,Rect(52,184,284,90),200);
    imshow("rgb",g_dstImage2);
    QTime t2=QTime(0,0,0);
    QTime t1=QTime(0,0,10);
    qDebug()<<"aaa"<<t2.secsTo(t1);
    qDebug()<<"bbb"<<common::angleDif(178,-177,180,true);
    return;
    Mat MyMat=imread("d:/aaa.png",0);
    Rect roi(0,0,100,100);
    Scalar meanValue=mean(MyMat(roi));
qDebug()<<"mean"<<meanValue[0];
    std::vector<int> v1;
    std::vector<int> v2;
    v1.reserve(10);
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);
    v2.push_back(1);
    v2.push_back(2);
    v2.push_back(3);
    v2.push_back(4);
    v2.push_back(5);
    qDebug()<<v1.size()<<"aaaaa";

return;
    int aa1=1;
    int aa2=2;
    int aa3;

    QTime t;
    t.start();


    for(int i=0;i<10000;i++)
    {
        for(int j=0;j<100000;j++)
aa2=j;
                //aa3=calc70(aa1,aa2);


    }

//    for(int i=0;i<1000;i++)
//    {
//        for(int j=0;j<1000;j++)
//        {
//            for(int j1=0;j1<1000;j1++)
//            {
//                //for(int j2=0;j2<1000;j2++)
//                {
//aa2=j;
//                //aa3=calc70(aa1,aa2);

//                }
//            }
//        }
//    }
     qDebug("Time elapsed: %d  ms", t.elapsed());
     qDebug()<<"aa4"<<aa2;
    //qDebug()<<common::calcAngle1(p1,p2)<<common::calcAngle(p1,p2);




    return;

    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    Point p[3];
    p[0].y=1;
    p[1].y=3;
    p[2].y=2;
    mySort(p,3);
    qDebug()<<p[0].y<<p[1].y<<p[2].y;
       return;
    double a=1;
    double b=0;

    double c=-0.000002;
    qDebug()<<_finite(a/b)<<_isnan(a/b)<<_finite(b/a)<<_isnan(b/a);
    double ff=1.23423;
    QString str1=QString("%1").arg(ff,0,'f',3);
    QString str2=QString("%1").arg(11,0,16);
    qDebug()<<str2;



    QString s2="aabc,adfs";
    QStringList list=s2.split(",");
    if(list.size()<2) return;
    qDebug()<<"toDouble"<<list[1].toDouble();


}

void MainWindow::on_pushButton_111_clicked()
{
    btnNext=ui->pushButton_111;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //540,572,35,140,179,100,0
     params param;
      //E线参数
     Rect roi(249,251,849-249,876-251);
     //Rect roi(0,0,1280,960);
     param.var1[0]=518;//中心点X
     param.var1[1]=575;//中心点Y
     param.var1[2]=35;//检测字体最小值
     param.var1[3]=140;//检测字体最大值
     param.var1[4]=179;//字体到中心点距离
     param.var1[5]=100;//二值
     param.var2[0]=0;//角度偏移
     param.var1[28]=100000;//得到首次检测到NI坐标
     param.var1[29]=100000;
     param.var2[19]=360;//角度

     if(!ui->checkBox2->isChecked())
     {
         //C线
         param.var1[0]=656;//中心点X
         param.var1[1]=655;//中心点Y
         roi.x=313;
         roi.y=332;
         roi.width=1001-313;
         roi.height=916-332;

     }

     QTime t;
     t.start();

     if(!ui->checkBox_dll->isChecked())
        pro3->zt_3rdLineQC(g_dstImage1,roi,param,false);


     //call dll
     if(ui->checkBox_dll->isChecked())
     {
          detectWork(g_dstImage1,roi,param,1);

          namedWindow("src",0);
          resizeWindow("src",800,600);
          imshow("src",g_dstImage1);
     }
     //end


     ui->label_3->setText(ui->comboBox->currentText().right(22)+"   angle:"+QString::number(param.var2[19]));
    qDebug()<<"fileName"<<ui->comboBox->currentText().right(22)<<"matchCount"<<param.var1[29]<<"angle:"<<param.var2[19];
    qDebug("Time elapsed: %d ms", t.elapsed());
}
void MainWindow::timer8()
{
    params param;
    Rect roi;
    //E线
    if(ui->checkBox2->isChecked())
    {

        roi.x=249;
        roi.y=251;
        roi.width=849-249;
        roi.height=876-251;
        param.var1[0]=536;//中心点X
        param.var1[1]=575;//中心点Y

    }
    else
    {

    //C线

    roi.x=313;
    roi.y=332;
    roi.width=1001-313;
    roi.height=916-332;
    param.var1[0]=656;//中心点X
    param.var1[1]=655;//中心点Y

    }



    param.var1[2]=35;//检测字体最小值
    param.var1[3]=140;//检测字体最大值
    param.var1[4]=179;//字体到中心点距离
    param.var1[5]=100;//二值
    param.var2[0]=0;//角度偏移

    static int i=0,i3=0;
    remainingTime(i,fileList.size());
    if(i==fileList.size())
    {

       i=0;
       i3=0;
       t8->stop();
       return;
    }

    ui->label->setText(QString::number(i));
    param.var1[28]=100000;//得到首次检测到NI坐标
    param.var1[29]=100000;
    param.var2[19]=360;//角度



    string strPic=(loadPath+fileList[i]).toLocal8Bit();
    Mat pic=imread(strPic,0);

    QTime t;
    t.start();
    if(!ui->checkBox_dll->isChecked())
        pro3->zt_3rdLineQC(pic,roi,param,false);
    else
    {
         //call dll
        detectWork(pic,roi,param,1);
    }


    if(param.var2[19]==360)
    {

        ui->label_2->setText(QString::number(++i3));

    }

    qDebug("Time elapsed: %d ms", t.elapsed());
    // qDebug()<<"x1 y1"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19]<<"特征数量"<<param.var1[26]<<"i:"<<param.var1[27];

    QString s=fileList[i];
    s.remove(0,16);
    s.split('.');
    double angle=s.split('.').at(0).toDouble();
    if(abs(abs(angle)-abs(param.var2[19]))>50&&param.var2[19]!=360)
    {

        bool b= QFile::copy(loadPath+fileList[i],"image/"+fileList[i]);
        qDebug()<<"angle"<<param.var2[19]<<"copy state"<<b<<fileList[i];


        //QFile::remove(loadPath+fileList[i]);

        //rename

        if(param.var2[19]!=360)
        {
//            QString newName=fileList[i].left(16)+QString("%1.png").arg(param.var2[19]);
//            QFile::rename("image/"+fileList[i],"image/"+newName);
//            qDebug()<<"newName"<<newName;

        }

    }

    ++i;
}
void MainWindow::copyImg(int i, int value)
{
    static int cp1=0;

    QString s=fileList[i];
    s.remove(0,16);
    s.split('.');
    int fileVal=s.split('.').at(0).toInt();

    if(value)//!=fileVal)//&&param.var2[19]!=360)
    {

        bool b= QFile::copy(loadPath+fileList[i],"image/"+fileList[i]);
        qDebug()<<"copy state"<<b<<fileList[i]<<"value"<<value;

        ui->label_2->setText(QString::number(++cp1));

        //QFile::remove(loadPath+fileList[i]);

        //rename

        if(value==0)
        {
//            QString newName=fileList[i].left(16)+QString("%1.png").arg(param.var2[19]);
//            QFile::rename("image/"+fileList[i],"image/"+newName);
//            qDebug()<<"newName"<<newName;

        }
    }

}

void MainWindow::remainingTime(int index,int allNum)
{


    if(index==0)
        startTime.start();


    ui->label_3->setText(QString("remaining time:%1 min").arg(startTime.elapsed()*1.0/index*(allNum-index)/60000));

}


void MainWindow::timer9()
{
    static int i=0,i3=0;
    ui->label->setText(QString::number(i));
    //remainingTime(i,fileList.size());
    if(i==fileList.size())
    {

       i=0;
       i3=0;
       t9->stop();
       DEBUG=true;
       ui->comboBox->setEnabled(true);
       return;
    }
//    string strPic=(loadPath+fileList[i]).toLocal8Bit();
//    g_dstImage1=imread(strPic,0);
    ui->comboBox->setCurrentIndex(i);

    QTime t;
    t.start();

   //on_pushButton_129_clicked();
   on_pushButton_55_clicked();
   QString s=fileList[i];
   s.remove(0,16);
   s.split('.');
   double angle=s.split('.').at(0).toDouble();

//    if(paramRet==360||common::angleDif(angle,paramRet,180,true)>30)
//    {
//        qDebug()<<"hello"<<common::angleDif(angle,paramRet,180,true)<<s.split('.').at(0)<<paramRet<<angle;
//        QFile::copy(loadPath+fileList[i],"image/"+fileList[i]);
//        ui->label_2->setText(QString::number(++i3));
//    }

       if(abs(paramRet-angle)>100)
       {
           qDebug()<<"paramRet"<<paramRet<<"angle"<<angle<<"fileName"<<fileList[i];
           //qDebug()<<"hello"<<common::angleDif(angle,paramRet,180,true)<<s.split('.').at(0)<<paramRet<<angle;
           QFile::copy(loadPath+fileList[i],"image/"+fileList[i]);
           ui->label_2->setText(QString::number(++i3));
       }
    //copyImg(i,param.var1[29]);
    remainingTime(i,fileList.size());
    qDebug()<<"耗时:"<<t.elapsed();
    ++i;


}
void MainWindow::on_pushButton_110_clicked()
{
    DEBUG=false;
    t8=new QTimer(this);
    connect(t8,SIGNAL(timeout()),this,SLOT(timer8()));
    t8->start(100);
}
void MainWindow::on_pushButton_102_clicked()
{
    if(!DEBUG)
    {
        t9->stop();
        ui->comboBox->setEnabled(true);
        DEBUG=true;
        return;
    }
    DEBUG=false;
    ui->comboBox->setEnabled(false);
    t9=new QTimer(this);
    connect(t9,SIGNAL(timeout()),this,SLOT(timer9()));
    t9->start(60);
}
void MainWindow::on_pushButton_113_clicked()
{
    DEBUG=true;
    string s1=ui->comboBox->currentText().toLocal8Bit();

     g_dstImage1=imread(s1,0);
     g_dstImage2=imread(s1,1);


    int otherNum=0;

    //雪龙6种圆环尺寸 76  86  125  127  164  180

    //三维视觉参数
    //1120,743,140,620,140,1300,40
    params param;

    param.var1[0]=1120;//圆心X
    param.var1[1]=743;//圆心Y
    param.var1[2]=140;//小圆孔size
    param.var1[3]=620;//半径
    param.var1[4]=140;//异物灰度值
    param.var1[5]=1300;//过滤最小长度大于
    param.var1[6]=40;//过滤最小长度小于


    int num=0,num2=0;
    QTime t;
    t.start();
    //Rect roi(1000,750,1000,1000);
    Rect roi(0,0,2500,1900);
    bool callDll=true;
    if(callDll)
        detectWork(g_dstImage1,roi,param,16);
    else
        pro1->xl_blackObj(g_dstImage1,g_dstImage2,roi,param);

    qDebug("Time elapsed: %d ms", t.elapsed());
    qDebug()<<"ret:"<<param.var1[29];

    QString words="has something";
    if(param.var1[29])
    {
        putText( g_dstImage2, words.toStdString(), Point(param.var1[0],param.var1[1]),CV_FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 0, 255),6 );
    }
    else
    {
        circle( g_dstImage2, Point(param.var1[27],param.var1[28]), 30, Scalar(0,0,255), 3, CV_AA);
        words="nothing";
        putText( g_dstImage2, words.toStdString(), Point(param.var1[0],param.var1[1]),CV_FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 0, 255),6 );
    }

    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    imshow("rgb",g_dstImage2);
}
void MainWindow::on_pushButton_114_clicked()
{
    btnNext=ui->pushButton_114;

    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //551,505,40,150,210,250,230,10,47,10,0
    params param;
    param.var1[0]=551;//中心点X
    param.var1[1]=505;//中心点Y
    param.var1[2]=40;// param.var1[2];//检测字体最小值
    param.var1[3]=150;//param.var1[3];//检测字体最大值
    param.var1[4]=210;//字体到中心点距离
    param.var1[5]=250;
    param.var1[6]=230;//二值
    param.var1[7]=10;//18;//三角形外接矩形len
    param.var1[8]=47;
    param.var1[9]=10;//19;//三角形最小边长
    param.var2[0]=0;//角度偏移
    //Rect roi(510,180,320,630);
    Rect roi(0,0,1280,960);
    QTime t;
    t.start();


    namedWindow("rgb",0);

    resizeWindow("rgb",800,600);
    if(!ui->checkBox_dll->isChecked())
    {
        namedWindow("m1",0);
        resizeWindow("m1",800,600);
        pro3->zt_6thLine(g_dstImage1,g_dstImage2,roi,param);
    }
    else
    {
        detectWork(g_dstImage1,roi,param,12);
    }


    circle( g_dstImage2,Point(param.var1[28]+roi.x,param.var1[29]+roi.y), 25, Scalar(0,0,255), 1, CV_AA);
    imshow("rgb",g_dstImage2);
    qDebug()<<"angle:"<<param.var2[19];
    qDebug("Time elapsed: %d ms", t.elapsed());
}
void MainWindow::on_pushButton_115_clicked()
{
    btnNext=ui->pushButton_115;

    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    Point p1(51,209);
    Point p2(367,374);
    Point out(0,0);
    Rect roi(p1.x,p1.y,p2.x-p1.x,p2.y-p1.y);
    circle(g_dstImage2,p1,3,Scalar(0,255,0),3);
    circle(g_dstImage2,p2,3,Scalar(0,255,0),3);

    int num=pro1->icecream3(g_dstImage1,roi,200,out);

    circle(g_dstImage2,out,10,Scalar(255,0,0),3);
    if(num>10)
    {

        qDebug()<<"found"<<out.x<<out.y;


    }
    imshow("rgb",g_dstImage2);
    ui->label_3->setText(QString("num:%1").arg(num));
}

void MainWindow::on_pushButton_116_clicked()
{
    if(DEBUG)
    {
    btnNext=ui->pushButton_116;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    }
    params param;
    //735,556,103,280,390,170,25,0,102,132

    param.var1[0]=715;//725;//740;//圆心X
    param.var1[1]=681;//圆心Y
    param.var1[2]=25;//圆心与字距离
    param.var1[3]=370;//大半圆size
    param.var1[4]=550;//大半圆size
    param.var1[5]=240;//152;//二值
    param.var1[6]=25;//计算NI step值
    param.var2[0]=0;//角度偏差
    param.var1[7]=203;//外接矩形长
    param.var1[8]=230;//外接矩形长
    param.var1[9]=1000;//圆心与字距离最大偏差


    //Rect r1(0,0,1000,800);
    Rect r1(520,580,450,250);
    QTime t;

    if(DEBUG)
        t.start();
    if(!ui->checkBox_dll->isChecked())
        pro1->zt_lineA2(g_dstImage1,r1,param);
    else
        detectWork(g_dstImage1,r1,param,17);

    paramRet=param.var1[29];

    if(DEBUG)
    {
    namedWindow("show",0);
    resizeWindow("show",800,600);
     imshow("show",g_dstImage2);
     qDebug()<<"found:"<<param.var1[29]<<"耗时:"<<t.elapsed();

     ui->label_3->setText(QString("found:%1").arg(param.var1[29]));
    }
}

void MainWindow::on_pushButton_118_clicked()
{
    btnNext=ui->pushButton_118;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //540,572,35,140,179,100,0
     params param;


     Rect roi(249,251,849-249,876-251);
     //Rect roi(0,0,1280,960);
     param.var1[0]=658;//中心点X
     param.var1[1]=656;//中心点Y
     param.var1[2]=35;//检测字体最小值
     param.var1[3]=140;//检测字体最大值
     param.var1[4]=179;//字体到中心点距离
     param.var1[5]=100;//二值
     param.var2[0]=0;//角度偏移
     param.var1[28]=100000;//得到首次检测到NI坐标
     param.var1[29]=100000;
     param.var2[19]=360;//角度

     if(ui->checkBox2->isChecked())
     {
         param.var1[0]=545;//中心点X
         param.var1[1]=577;//中心点Y
         roi.x=313;
         roi.y=332;
         roi.width=1001-313;
         roi.height=916-332;

     }

     QTime t;
     t.start();

     if(!ui->checkBox_dll->isChecked())
        pro3->zt_not3rdLine(g_dstImage1,roi,param);
     else
     {
          detectWork(g_dstImage1,roi,param,1);

//          namedWindow("src",0);
//          resizeWindow("src",800,600);
//          imshow("src",g_dstImage1);
     }
     //end



     ui->label_3->setText(QString::number(param.var2[10]));
     //imshow("rgb",g_dstImage2);
//    qDebug()<<"fileName"<<ui->comboBox->currentText().right(22)<<"matchCount"<<param.var1[29]<<"angle:"<<param.var2[19];
//    qDebug("Time elapsed: %d ms", t.elapsed());
     paramRet=param.var2[10];
}
void MainWindow::on_pushButton_112_clicked()
{
    btnNext=ui->pushButton_112;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //  572,480,57,120,103,143,70,254,20,42,-1,-1,18,5,700,900,230,260,20,15,90,254,0
    params param;
    //I线三角形
    param.var1[0]=572;//中心点X
    param.var1[1]=480;//中心点Y
    param.var1[2]=57;//检测字体最小值
    param.var1[3]=120;//检测字体最大值
    param.var1[4]=103;//字体到中心点距离
    param.var1[5]=143;
    param.var1[6]=70;//二值
    param.var1[7]=254;//二值
    param.var1[8]=20;//三角形外接矩形 len
    param.var1[9]=42;
    param.var1[10]=-1;//三角形外接矩形较短边角度
    param.var1[11]=-1;
    param.var1[12]=18;//三角形最小边长
    param.var1[13]=5;//特征数量


    param.var1[20]=700;//检测最小值
    param.var1[21]=900;//检测最大值
    param.var1[22]=230;//矩形len
    param.var1[23]=260;
    param.var1[24]=20;//step
    param.var1[25]=18;//x距离
    param.var1[26]=90;//开始位置未使用
    param.var1[27]=254;//二值
    param.var2[0]=0;//角度偏移


    param.var2[19]=360;//角度


    Rect roi(200,250,650,450);
    QTime t;
    t.start();

    if(ui->checkBox_dll->isChecked())
    {
        detectWork(g_dstImage1,roi,param,20);
    }
    else
    {
        pro3->zt_oneTrangle(g_dstImage1,g_dstImage2,roi,param);
        if(param.var2[19]==360)
        {

            pro3->zt_Iline(g_dstImage1,roi,param);
        }

    }

    qDebug("Time elapsed: %d ms", t.elapsed());
    qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];

    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],90,Scalar(0,0,255));

    imshow("rgb",g_dstImage2);

}

void MainWindow::on_pushButton_122_clicked()
{
    btnNext=ui->pushButton_122;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    //101,19,90,180,0
    //  588,523,101,19,90,180,0,57,150,200,230,70,200,5,65,70,2,0.5,0
    params param;
    param.var1[0]=584;//中心点X
    param.var1[1]=523;//中心点Y
    param.var1[2]=57;//检测字体最小值
    param.var1[3]=150;//检测字体最大值
    param.var1[4]=200;//字体到中心点距离
    param.var1[5]=250;
    param.var1[6]=70;//二值
    param.var1[7]=254;//二值
    param.var1[8]=5;//外接矩形 len
    param.var1[9]=65;
    param.var1[10]=70;//矩形角度

    param.var2[0]=2;//长度
    param.var2[1]=0.4;//面积
    param.var2[2]=0;//角度偏移

    param.var2[19]=360;//角度


    Rect roi(260,250,650,550);
    QTime t;
    t.start();

    if(ui->checkBox_dll->isChecked())
    {
        detectWork(g_dstImage1,roi,param,101);
    }
    else
    {
        Mat m5;
        medianBlur(g_dstImage1(roi), m5, 3);
        pro3->zt_JLine(m5,g_dstImage2,roi,param);
        if(param.var2[19]==360)
            pro3->zt_JLineQC(m5,g_dstImage2,roi,param);
        if(param.var2[19]==360)
            pro3->zt_JLineMatch(m5,g_dstImage2,roi,param);

    }

    qDebug("Time elapsed: %d ms", t.elapsed());
    qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];

    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],90,Scalar(0,0,255));

    ui->label_3->setText(QString("angle:%1").arg(param.var2[19]));
    imshow("rgb",g_dstImage2);
}

void MainWindow::on_pushButton_124_clicked()
{
    btnNext=ui->pushButton_124;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    params param;
    param.var1[0]=200;
    param.var1[1]=3400;
    param.var1[2]=180;//二值



    QTime t;
    t.start();
    pro3->sz_cloth2(g_dstImage1,g_dstImage2,Rect(0,0,100,100),param);

    double modelAngle=-37.8588;//模板角度

    Point modelP(824,1940);//模板标定点
    double camAngle=0;

    double r_data[6]={-1000};
    double ratio=0.01;

    if(param.var2[19]==1)
    {



        Point p1=common::xyCalibration(camAngle,modelP,Point(param.var2[11],param.var2[12]));
        //第一块布矫正
        r_data[0]=common::angleDif(param.var2[10],modelAngle,90,false);
        r_data[1]=(p1.x-modelP.x)*ratio;
        r_data[2]=(p1.y-modelP.y)*ratio;
        //重叠两块布
        Point p2=common::xyCalibration(camAngle,Point(param.var2[11],param.var2[12]),Point(param.var2[14],param.var2[15]));
        r_data[3]=common::angleDif(param.var2[10],param.var2[13],90,false);
        r_data[4]=(p2.x-param.var2[11])*ratio;
        r_data[5]=(p2.y-param.var2[12])*ratio;

    }
    ui->label_3->setText(QString("small angle:%1 Point %2 %3 bigAngle:%4 Point %5 %6").arg(param.var2[10]).arg(param.var2[11]).arg(param.var2[12]).arg(param.var2[13]).arg(param.var2[14]).arg(param.var2[15]));

    qDebug("Time elapsed: %d ms", t.elapsed());
    namedWindow("rgb",0);
    resizeWindow("rgb",1200,960);
    imshow("rgb",g_dstImage2);
    //qDebug()<<"angle"<<common::calcAngle(Point(1592,1170),Point(1955,1109))<<"len"<<common::getLineLen(Point(1859,1130),Point(1174,1208));

}

bool cmpBlackObj(Mat &src,Mat &dst,Point setLeft,Point setRight,int width)
{
    bool f=false;
    int num1,num2;
    int n1=0,n2=0;
    double ratio=(setRight.y-setLeft.y)*1.0/(setRight.x-setLeft.x);
    Point left;
    left.x=(setRight.x-setLeft.x)/2-width/2+setLeft.x;
    left.y=((setRight.x-setLeft.x)/2-width/2)*ratio+setLeft.y;
    int startY=left.y+ratio*(left.x-setLeft.x);

    //circle(g_dstImage4,left,5,Scalar(255,0,0));
    //circle(g_dstImage4,Point(left.x+width,left.y),5,Scalar(255,0,0));

    for(int i=left.x+10;i<left.x+width-10;i+=3)
    {
        num1=0;
        num2=0;
        for(int j=startY+(i-left.x)*ratio-5;j<startY+(i-left.x)*ratio+20;j++)
        {
//增加地槛判断,防止把阴影计算在内,增加与模板对比,同一位置值小于20视为阴影
           if(dst.at<uchar>(j,i)<70&&abs(dst.at<uchar>(j,i)-dst.at<uchar>(j+3,i))<40&&abs(dst.at<uchar>(j,i)-src.at<uchar>(j,i))>20)
               ++num1;
           else
           {
               if(num1<10)
                    num1=0;
           }

           if(src.at<uchar>(j,i)<70&&abs(src.at<uchar>(j,i)-src.at<uchar>(j+3,i))<40)
               ++num2;
           else
           {
               if(num2<10)
                    num2=0;
           }


        }
        if(num1>10)
        {
            ++n1;
            circle(g_dstImage4,Point(i,startY+(i-left.x)*ratio-5),3,Scalar(255,255,255));
        }
        if(num2>10)
            ++n2;
    }
   //qDebug()<<"n1:"<<n1<<"n2:"<<n2;
    if(n1-n2>2)
        f=true;
    return f;
}


bool MainWindow::cmpBlackObj_2(Mat &src,Mat &dst,Point setLeft,Point setRight,int width)
{
    bool f=false;
    int num1,num2;
    int max1,min1,max2;
    double ratio=(setRight.y-setLeft.y)*1.0/(setRight.x-setLeft.x);
    Point left;
    left.x=(setRight.x-setLeft.x)/2-width/2+setLeft.x;
    left.y=((setRight.x-setLeft.x)/2-width/2)*ratio+setLeft.y;
    int startY=left.y+ratio*(left.x-setLeft.x);


    max1=-1;
    min1=1000;

    for(int i=left.x+10;i<left.x+width-10;i+=3)
    {
        num1=0;
        num2=0;
        max2=-1;
        circle(g_dstImage2,Point(i,startY+(i-left.x)*ratio+10),3,Scalar(255,0,0));
        for(int j=startY+(i-left.x)*ratio-5;j<startY+(i-left.x)*ratio+20;j++)
        {

           if(dst.at<uchar>(j,i)>max2)
           {
               max2=dst.at<uchar>(j,i);

           }

        }

        if(max2>max1)
        {
            max1=max2;
        }
        if(max2<min1)
        {
            min1=max2;
        }
    }
    if(max1>220)
        max1=220;
    if(min1>220)
        min1=220;
    //qDebug()<<"max1:"<<max1<<"min1:"<<min1<<"dif"<<max1-min1;
    ui->label_3->setText(QString("max1:%1 min1:%2 dif:%3").arg(max1).arg(min1).arg(max1-min1));
    if(abs(max1-min1)>67)
        f=true;
    return f;
}

bool MainWindow::cmpBlackObj_3(Mat &src,Mat &dst,Point setLeft,Point setRight,int width)
{
    //普通电梯
    bool f=false;
    int num1,num2;
    int max1,min1,max2,min2,maxDst,maxSrc;
    double ratio=(setRight.y-setLeft.y)*1.0/(setRight.x-setLeft.x);
    Point left;
    left.x=(setRight.x-setLeft.x)/2-width/2+setLeft.x;
    left.y=((setRight.x-setLeft.x)/2-width/2)*ratio+setLeft.y;
    int startY=left.y+ratio*(left.x-setLeft.x);


    max1=-1;
    min1=1000;
    max2=-1;
    min2=1000;


    for(int i=left.x+10;i<left.x+width-10;i+=3)
    {

        maxDst=-1;
        maxSrc=-1;


        for(int j=startY+(i-left.x)*ratio-5;j<startY+(i-left.x)*ratio+20;j++)
        {

           if(dst.at<uchar>(j,i)>maxDst)
           {
               maxDst=dst.at<uchar>(j,i);

           }

           if(src.at<uchar>(j,i)>maxSrc)
           {
               maxSrc=src.at<uchar>(j,i);

           }

        }
//src
        if(maxSrc>max2)
        {
            max2=maxSrc;
        }
        if(maxSrc<min2)
        {
            min2=maxSrc;
        }
//dst
        if(maxDst>max1)
        {
            max1=maxDst;
        }
        if(maxDst<min1)
        {
            min1=maxDst;
        }

    }
    //dst
    if(max1>220)
        max1=220;
    if(min1>220)
        min1=220;
    //src
    if(max2>220)
        max2=220;
    if(min2>220)
        min2=220;

    if(max1-min1-(max2-min2)>40)
        f=true;
    ui->label_3->setText(QString("dst:%1 src:%2 dif:%3").arg(max1-min1).arg(max2-min2).arg(max1-min1-(max2-min2)));
    return f;
}


bool dynamicObj(Mat &src,Mat &dst,int val,Point setLeft,Point setRight,int width)
{
    //width=53;
    bool f=false;
    int num1=0,num2=0,num3=0,num4=0;

    double ratio=(setRight.y-setLeft.y)*1.0/(setRight.x-setLeft.x);
    Point left;
    left.x=(setRight.x-setLeft.x)/2-width/2+setLeft.x;
    left.y=((setRight.x-setLeft.x)/2-width/2)*ratio+setLeft.y;
    int startY=left.y+ratio*(left.x-setLeft.x)+10;
    int pos_x=setLeft.x+((setRight.x-setLeft.x)/2-width/2);
    for(int i=pos_x+10;i<pos_x+width-10;i+=3)
    {

        for(int j=startY+(i-left.x)*ratio;j<startY+(i-left.x)*ratio+40;j+=3)
        {
            ++num2;
            if(abs(dst.at<uchar>(j,i)-src.at<uchar>(j,i))<val)
                continue;
            if(abs(dst.at<uchar>(j,i+1)-src.at<uchar>(j,i+1))<val)
                continue;
            if(abs(dst.at<uchar>(j,i+2)-src.at<uchar>(j,i+2))<val)
                continue;
            if(abs(dst.at<uchar>(j+1,i)-src.at<uchar>(j+1,i))<val)
                continue;
            if(abs(dst.at<uchar>(j+2,i)-src.at<uchar>(j+2,i))<val)
                continue;
            if(abs(dst.at<uchar>(j+1,i+1)-src.at<uchar>(j+1,i+1))<val)
                continue;
            if(abs(dst.at<uchar>(j+1,i+2)-src.at<uchar>(j+1,i+2))<val)
                continue;
            if(abs(dst.at<uchar>(j+2,i+2)-src.at<uchar>(j+2,i+2))<val)
                continue;
            ++num1;
           // qDebug()<<dst.at<uchar>(j,i)<<","<<src.at<uchar>(j,i)<<",";
            if(dst.at<uchar>(j,i)<src.at<uchar>(j,i))
                ++num3;

            if(dst.at<uchar>(j,i)>src.at<uchar>(j,i))
                ++num4;




        }
    }
    if(num1>2)
        f=true;

    if(num1*1.0/num2>0.8||num3*1.0/num1>0.85||num4*1.0/num1>0.85)
    {
        //图片瞬间变亮则不进行运动检测
        f=false;
    }

    //qDebug()<<"num1"<<num1<<"num2"<<num2<<"num3"<<num3<<"num4"<<num4;

    return f;
}

int avgGray_roi(Mat &src,Rect &roi)
{
    int step=2;
    int v1=0,num=0;

    for(int i=roi.y;i<roi.y+roi.height;i+=step)
    {
        for(int j=roi.x;j<roi.x+roi.width;j+=step)
        {
            v1+=src.at<uchar>(i,j);
            ++num;
        }
    }

    return v1*1.0/num;
}

void MainWindow::on_pushButton_125_clicked()
{
    btnNext=ui->pushButton_125;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage4=imread(s1,1);
    if(g_dstImage2.cols==0)
        return;
    Point p_left(233,211),p_right(362,213);
    Rect roi(p_left.x-40,p_left.y,p_right.x+80-p_left.x,30);
    Rect roi4(p_left.x,(p_left.y+p_right.y)/2-5,p_right.x-p_left.x,35);
    //rectangle(g_dstImage4,roi4,Scalar(200,200,200));
    int doorWidth;
    Point getLeft(0,0);
    Point getRight(0,0);
    int dif= wk->cmpPicPixel2(g_dstImage1,g_dstImage2,g_dstImage4,p_left,p_right,30,doorWidth,getLeft,getRight);
    bool blackLine=wk->findBlackLine(g_dstImage1,g_dstImage4,0,3,p_left,p_right);
    if(dif>18)
    {
        qDebug()<<"open the door len:"<<dif;
    }
    Rect roi3(0,p_left.y<p_right.y?p_left.y-10:p_right.y-10,640,60);
    //qDebug()<<"avg gray"<<avgGray_roi(g_dstImage1,roi3)<<avgGray_roi(g_dstImage2,roi3);

    cmpBlackObj(g_dstImage2,g_dstImage1,p_left,p_right,139);
    int staticBlackObj= wk->staticBlackObj(g_dstImage1,g_dstImage4,45,p_left,p_right,1);
    qDebug()<<"dif"<<dif<<"doorWidth"<<doorWidth<<"staticBlackObj"<<staticBlackObj<<"blackLine"<<blackLine;

    Rect roi2(p_left.x,(p_left.y+p_right.y)/2+10,p_right.x-p_left.x,40);
    //rectangle(g_dstImage4,roi2,Scalar(150,150,150));
    dynamicObj(g_dstImage2,g_dstImage1,40,p_left,p_right,doorWidth);

    imshow("rgb",g_dstImage4);



}

void MainWindow::on_pushButton_132_clicked()
{
    btnNext=ui->pushButton_132;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);

    g_dstImage3=imread(s1,1);

    Point p_left(239,244),p_right(394,241);

    cmpBlackObj_3(g_dstImage2,g_dstImage1,p_left,p_right,ui->lineEdit_2->text().toInt());
    imshow("rgb",g_dstImage3);
}

void MainWindow::on_pushButton_126_clicked()
{
    btnNext=ui->pushButton_126;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
//651,512,2200,2450,15,25,740,690,0
    Rect roi(0,0,1280,960);
    params param;
    param.var1[0]=651;//中心点
    param.var1[1]=512;
    param.var1[2]=2200;//最小size
    param.var1[3]=2450;//最小size
    param.var1[4]=15;//二值
    param.var1[5]=25;
    param.var1[6]=740;//长
    param.var1[7]=690;//宽

    param.var2[2]=0.03354;//像素比
    param.var2[3]=120;//二值
    param.var1[20]=550;//size
    param.var1[21]=800;//size
    param.var1[22]=1;//isBlack
    param.var1[23]=20;//长宽差
    param.var1[24]=660;//x
    param.var1[25]=500;//y

    param.var2[4]=0;//输出直径
    param.var2[0]=0;

    QTime t;
    t.start();

    Mat m2;
     medianBlur(g_dstImage1, m2, 3);
    if(ui->checkBox_dll->isChecked())
    {
        detectWork(m2,roi,param,21);

    }
    else
    {
        pro2->hd_vision3_angle(m2,roi,param);
        pro2->hd_maxDiameter2(m2,roi,param);
    }
    qDebug()<<"aaa"<<param.var2[4];

    ui->label_3->setText(QString("angle:%1 diameter %2 diameter2 %3 i:%4 Time elapsed:%5").arg(param.var2[19]).arg(param.var2[5]).arg(param.var2[6]).arg(param.var1[29]).arg(t.elapsed()));
    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    imshow("rgb",g_dstImage2);
}

void MainWindow::on_pushButton_127_clicked()
{
    btnNext=ui->pushButton_127;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
//651,512,2200,2450,15,25,740,690,0
    Rect roi(0,0,1280,960);
    params param;

    param.var2[2]=0.03354;//像素比
    param.var2[3]=200;//二值
    param.var1[20]=400;//size
    param.var1[21]=900;//size
    param.var1[22]=0;//isBlack
    param.var1[23]=50;//长宽差
    param.var1[24]=660;//x
    param.var1[25]=500;//y
    param.var2[4]=0;//输出外接矩形较小直径
    param.var2[5]=0;//输出外接矩形较大直径
    param.var2[6]=0;//输出外接圆直径
    QTime t;
    t.start();

    if(ui->checkBox_dll->isChecked())
    {
        param.var1[0]=2000;
        detectWork(g_dstImage1,roi,param,21);

    }
    else
    {

        pro2->hd_maxDiameter2(g_dstImage1,roi,param);
    }


    ui->label_3->setText(QString("diameter %1  diameter2 %2 Time elapsed:%3").arg(param.var2[4]).arg(param.var2[6]).arg(t.elapsed()));
    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    imshow("rgb",g_dstImage2);
}

void MainWindow::on_pushButton_128_clicked()
{
    btnNext=ui->pushButton_128;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    qDebug()<<">255 is " <<pro1->elevatorInit(g_dstImage1,g_dstImage2);


}
void MainWindow::on_pushButton_104_clicked()
{
    btnNext=ui->pushButton_104;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    params param;
     //毛刺检测

    Rect roi(0,0,847,845);
    param.var1[0]=3000;
    param.var1[1]=6000;
    param.var1[2]=230;//二值
    param.var1[3]=10;
    param.var1[4]=9;//下一位置
    param.var1[5]=20;//角度差
    param.var1[6]=6;//偏差
    param.var1[7]=4;//x结束
    param.var1[8]=4;//y结束
    QTime t;
    t.start();


    if(!ui->checkBox_dll->isChecked())
    {
            pro4->defect(g_dstImage1(roi),g_dstImage2,roi,param);

    }
    else
    {
            detectWork(g_dstImage1,roi,param,200);

    }


   qDebug("Time elapsed: %d ms", t.elapsed());
   ui->label_3->setText(QString("num:%1").arg(param.var1[29]));

   //common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),170,param.var2[19],90,Scalar(255,0,0));

   namedWindow("rgb",0);
   resizeWindow("rgb",800,600);
   imshow("rgb",g_dstImage2);

}
void MainWindow::on_pushButton_129_clicked()
{


    btnNext=ui->pushButton_129;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //620,570,102,1,90,200,2,-1,-1,35,140,149,209,254,70,3,52,17,30,0,15,0,40,80,31,2,0,180
     params param;
      //E线NI
//620,570,102,1,90,200,2,-1,-1,35,140,149,209,254,70,3,52,17,30,0,15,0,40,80,31,2,0,180
     Rect roi(350,270,550,550);
     //Rect roi(0,0,1280,960);
     param.var1[0]=620;//中心点X
     param.var1[1]=570;//中心点Y
     param.var1[2]=35;//检测字体最小值
     param.var1[3]=140;//检测字体最大值
     param.var1[4]=149;//字体到中心点距离
     param.var1[5]=209;//字体到中心点距离
     param.var1[6]=254;//二值
     param.var1[7]=70;//二值
     param.var1[8]=3;//外接矩形len
     param.var1[9]=52;//外接矩形len
     param.var1[10]=17;//外接角度
     param.var1[11]=30;//角度范围
     param.var1[12]=0;//范围标识0 90
     param.var1[13]=15;//i+=step;
     param.var1[14]=0?true:false;//黑色
     param.var1[15]=40;//噪点
     param.var1[16]=80;//相似度
     param.var1[17]=31;


     param.var2[0]=2;//长度
     param.var2[1];//面积


     param.var2[18]=180;//角度偏移
     param.var2[19]=360;//角度




     QTime t;
     t.start();


     if(!ui->checkBox_dll->isChecked())
     {
         Mat m5;
         medianBlur(g_dstImage1(roi), m5, 3);
         pro3->commonMatch(m5,g_dstImage2,roi,param);

     }
     else
     {
         //call dll
         detectWork(g_dstImage1,roi,param,102);

     }



    qDebug()<<"fileName"<<ui->comboBox->currentText().right(22)<<"x1 y1"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19]<<"特征数量"<<param.var1[26]<<"i:"<<param.var1[27];
    qDebug("Time elapsed: %d ms", t.elapsed());
    ui->label_3->setText(QString("method:%1 angle:%2").arg(param.var1[25]).arg(param.var2[19]));

    if(DEBUG)
    {
    common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),170,param.var2[19],90,Scalar(255,0,0));


    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    imshow("rgb",g_dstImage2);
    }
    paramRet=param.var2[19];

}

void MainWindow::on_pushButton_130_clicked()
{
    //C线三角形
    btnNext=ui->pushButton_130;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    if(DEBUG)
    {
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    }

    //101,19,90,180,0

    //493,426,1000,1,90,200,2,-1,-1,80,230,183,244,50,200,17,52,360,30,0,5,1,1000,80,31,2,-1,-1,0
    params param;
    param.var1[0]=493;//中心点X
    param.var1[1]=426;//中心点Y
    param.var1[2]=80;//检测字体最小值
    param.var1[3]=230;//检测字体最大值
    param.var1[4]=183;//字体到中心点距离
    param.var1[5]=244;//字体到中心点距离
    param.var1[6]=50;//二值
    param.var1[7]=230;//二值
    param.var1[8]=17;//外接矩形len
    param.var1[9]=52;//外接矩形len
    param.var1[10]=360;//外接角度
    param.var1[11]=20;//角度范围
    param.var1[12]=0;//范围标识0 90
    param.var1[13]=5;//i+=step;
    param.var1[14]=1?true:false;//黑色
    param.var1[15]=1000;//噪点
    param.var1[16]=80;//相似度
    param.var1[17]=23;//fg
    param.var1[18]=2;//method
    param.var2[0]=-1;//长度
    param.var2[1]=-1;//面积

    param.var2[18]=0;//角度偏移


    param.var2[19]=360;//角度


    Rect roi(145,266,620,360);
    QTime t;
    t.start();

    if(ui->checkBox_dll->isChecked())
    {
        detectWork(g_dstImage1,roi,param,1000);
    }
    else
    {
        Mat m5;
        medianBlur(g_dstImage1(roi), m5, 3);

        pro3->commonMatch(m5,g_dstImage2,roi,param);

    }

    if(DEBUG)
    {
        qDebug("Time elapsed: %d ms", t.elapsed());
        qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19]<<"i:"<<param.var1[26];

        namedWindow("rgb",0);
        resizeWindow("rgb",800,600);
        common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],90,Scalar(0,0,255));
        imshow("rgb",g_dstImage2);
        ui->label_3->setText(QString("angle:%1 matchVal:%2").arg(param.var2[19]).arg(param.var1[27]));
    }


    paramRet=param.var2[19];


}
void MainWindow::on_pushButton_131_clicked()
{
    //振挺I线CG12-02
    btnNext=ui->pushButton_131;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    if(DEBUG)
    {
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    }

    //101,19,90,180,0

    //572,495,1001,1,0,150,2,-1,-1,80,230,130,190,110,240,4,95,360,20,90,5,1,1000,80,31,2,1.3,-1,0
    params param;
        param.var1[0]=572;//中心点X
        param.var1[1]=495;//中心点Y
        param.var1[2]=80;//检测字体最小值
        param.var1[3]=230;//检测字体最大值
        param.var1[4]=130;//字体到中心点距离
        param.var1[5]=190;//字体到中心点距离
        param.var1[6]=110;//二值
        param.var1[7]=240;//二值
        param.var1[8]=4;//外接矩形len
        param.var1[9]=95;//外接矩形len
        param.var1[10]=360;//外接矩形角度默认360   <45保留 or >45保留
        param.var1[11]=20;//角度范围20  标识0附近保留  标识90附近保留
        param.var1[12]=90;//范围标识0 90
        param.var1[13]=5;//i+=step;
        param.var1[14]=1;//1黑色  0白色
        param.var1[15]=1000;//噪点
        param.var1[16]=80;//相似度
        param.var1[17]=31;//fg
        param.var1[18]=2;//method

        param.var2[0]=1.3;//长度默认-1    <100 <过滤  or >100  >过滤  长/宽
        param.var2[1]=-1;//面积默认-1     <100 <过滤  or >100  >过滤  area/rectArea

        param.var2[18]=0;//角度偏移


        param.var2[19]=360;//角度


        Rect roi(302,191,874-302,765-191);
        QTime t;
        t.start();

        if(ui->checkBox_dll->isChecked())
        {
            qDebug()<<"call DLL";
            detectWork(g_dstImage1,roi,param,1001);

        }
        else
        {
            params param_2;
            param_2.var1[0]=param.var1[0];//中心点X
            param_2.var1[1]=param.var1[1];//中心点Y
            param_2.var1[2]=340;//检测字体最小值
            param_2.var1[3]=410;//检测字体最大值
            param_2.var1[4]=160;//字体到中心点距离
            param_2.var1[5]=230;//字体到中心点距离
            param_2.var1[6]=80;//二值
            param_2.var1[7]=180;//二值
            param_2.var1[8]=38;//外接矩形len
            param_2.var1[9]=190;//外接矩形len
            param_2.var1[10]=360;//外接矩形角度默认360   <45保留 or >45保留
            param_2.var1[11]=60;//角度范围20  标识0附近保留  标识90附近保留
            param_2.var1[12]=0;//范围标识0 90
            param_2.var1[13]=5;//i+=step;
            param_2.var1[14]=1;//1黑色  0白色
            param_2.var1[15]=1000;//噪点
            param_2.var1[16]=80;//相似度
            param_2.var1[17]=31;//fg
            param_2.var1[18]=209;//method

            param_2.var2[0]=2;//长度默认-1    <100 <过滤  or >100  >过滤  长/宽
            param_2.var2[1]=-1;//面积默认-1     <100 <过滤  or >100  >过滤  area/rectArea

            param_2.var2[18]=0;//角度偏移


            param_2.var2[19]=360;//角度
            Mat m5;
            medianBlur(g_dstImage1(roi), m5, 3);
            pro3->commonMatch(m5,g_dstImage2,roi,param_2);
            if(param_2.var2[19]!=360)
            {
                param.var1[26]=param_2.var1[26];
                param.var1[27]=param_2.var1[27];
                param.var1[28]=param_2.var1[28];
                param.var1[29]=param_2.var1[29];
                if(abs(param_2.var2[19])<90)
                    param.var2[19]=90;
                else
                    param.var2[19]=-90;
            }
            if(param.var2[19]==360)
                pro3->commonMatch(m5,g_dstImage2,roi,param);

        }

        if(DEBUG)
        {
            qDebug("Time elapsed: %d ms", t.elapsed());
            qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];

            namedWindow("rgb",0);
            resizeWindow("rgb",800,600);
            common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],0,Scalar(0,0,255));
            circle(g_dstImage2,Point(param.var1[28]+roi.x,param.var1[29]+roi.y),5,Scalar(0,0,255),2);
            imshow("rgb",g_dstImage2);
            ui->label_3->setText(QString("angle:%1 matchVal:%2 i:%3").arg(param.var2[19]).arg(param.var1[27]).arg(param.var1[26]));
        }


        paramRet=param.var2[19];
}


void MainWindow::on_pushButton_123_clicked()
{

    //K线三角形
    btnNext=ui->pushButton_123;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    //493,426,1000,1,90,150,2,-1,-1,80,230,210,280,50,230,17,60,360,20,0,5,1,1000,80,23,2,-1,-1,0
    //642,486,1000,1,90,150,2,-1,-1,80,230,210,280,50,230,17,60,360,20,0,5,1,1000,80,23,2,-1,-1,0
    params param;
    param.var1[0]=642;//中心点X
    param.var1[1]=486;//中心点Y
    param.var1[2]=80;//检测字体最小值
    param.var1[3]=230;//检测字体最大值
    param.var1[4]=210;//字体到中心点距离
    param.var1[5]=280;//字体到中心点距离
    param.var1[6]=50;//二值
    param.var1[7]=230;//二值
    param.var1[8]=17;//外接矩形len
    param.var1[9]=60;//外接矩形len
    param.var1[10]=360;//外接矩形角度默认360   <45保留 or >45保留
    param.var1[11]=20;//角度范围20  标识0附近保留  标识90附近保留
    param.var1[12]=0;//范围标识0 90
    param.var1[13]=5;//i+=step;
    param.var1[14]=1;//1黑色  0白色
    param.var1[15]=1000;//噪点
    param.var1[16]=80;//相似度
    param.var1[17]=23;//fg
    param.var1[18]=2;//method
    param.var2[0]=-1;//长度默认-1    <100 <过滤  or >100  >过滤  长/宽
    param.var2[1]=-1;//面积默认-1     <100 <过滤  or >100  >过滤  area/rectArea

    param.var2[18]=0;//角度偏移


    param.var2[19]=360;//角度


    Rect roi(250,300,750,380);
    QTime t;
    t.start();

    if(ui->checkBox_dll->isChecked())
    {
        detectWork(g_dstImage1,roi,param,1000);
    }
    else
    {
        Mat m5;
        medianBlur(g_dstImage1(roi), m5, 3);

        pro3->commonMatch(m5,g_dstImage2,roi,param);

    }

    if(DEBUG)
    {
        qDebug("Time elapsed: %d ms", t.elapsed());
        qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];

        namedWindow("rgb",0);
        resizeWindow("rgb",800,600);
        common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],90,Scalar(0,0,255));
        imshow("rgb",g_dstImage2);
        ui->label_3->setText(QString("angle:%1 matchVal:%2").arg(param.var2[19]).arg(param.var1[27]));
    }


    paramRet=param.var2[19];



//************** old ********************//
//     //642,486,80,300,210,280,200,30,70,25,30,0
//     params param;
//     param.var1[0]=642;//中心点X
//     param.var1[1]=486;//中心点Y
//     param.var1[2]=80;//检测字体最小值
//     param.var1[3]=300;//检测字体最大值
//     param.var1[4]=210;//字体到中心点距离
//     param.var1[5]=280;
//     param.var1[6]=200;//二值
//     param.var1[7]=30;//三角形外接矩形 len
//     param.var1[8]=70;
//     param.var1[9]=25;//三角形长宽差
//     param.var1[10]=30;//三角形最小边长
//     param.var2[0]=0;//角度偏移

//     param.var1[28]=10000;//得到首次检测到NI坐标
//     param.var1[29]=10000;
//     param.var2[19]=360;//角度



//     Rect roi(170,170,900,580);
//     QTime t;
//     t.start();
//call dll
//    // detectWork(g_dstImage1,roi,param,8);
//     //end


//     pro2->zt_5thLine(g_dstImage1,g_dstImage2,roi,param);

//     qDebug("Time elapsed: %d ms", t.elapsed());
//     qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];
//     ui->label_3->setText(QString("angle:%1 time:%2").arg(param.var2[19]).arg(t.elapsed()));
//************************* old end ****************************//



}

void MainWindow::on_pushButton_133_clicked()
{
    //振挺L线
    btnNext=ui->pushButton_133;
    string s1=ui->comboBox->currentText().toLocal8Bit();

    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);


    //101,19,90,180,0

    //600,536,104,1,0,150,0,-1,-1,1600,2300,230,300,400,119,80,190,176,144,0
    params param;
    param.var1[0]=600;//中心点X
    param.var1[1]=536;//中心点Y
    param.var1[2]=1600;//检测字体最小值
    param.var1[3]=2300;//检测字体最大值
    param.var1[4]=100;//thresh
    param.var1[5]=300;//rLen
    param.var1[6]=400;//rLen
    param.var1[7]=119;//半径
    param.var1[8]=80;//
    param.var1[9]=190;//
    param.var1[10]=176;//
    param.var1[11]=144;//
    param.var2[18]=0;//角度偏移


    param.var2[19]=360;//角度

    QByteArray bbb="abc,;ddd";
    QList<QByteArray> list2=bbb.split(',;');
    qDebug()<<list2[0]<<list2[1];

    Rect roi(320,250,660,660);
    QTime t;
    t.start();

    if(ui->checkBox_dll->isChecked())
    {
        detectWork(g_dstImage1,roi,param,104);
    }
    else
    {


        pro4->zt_LLine(g_dstImage1,g_dstImage2,roi,param);


    }

    if(DEBUG)
    {
        qDebug("Time elapsed: %d ms", t.elapsed());
        qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];

        namedWindow("rgb",0);
        resizeWindow("rgb",800,600);
        common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),150,param.var2[19],0,Scalar(0,0,255));
        imshow("rgb",g_dstImage2);
        ui->label_3->setText(QString("angle:%1 matchVal:%2").arg(param.var2[19]).arg(param.var1[27]));
    }


    paramRet=param.var2[19];
}

