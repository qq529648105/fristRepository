#include <QDebug>
#include <string.h>
#include <QTime>
#include <QFile>
#include "project3.h"
#include <QRegExp>
#include <QStringList>
#include <QDir>
#include <algorithm>
extern Mat g_dstImage1;
extern Mat g_dstImage2;
extern int g_nElementShape;
extern int thresh;
extern int max_thresh;
extern int offsetSign;
extern bool DEBUG;
#define SHAPE_FILENAME "temp2/n245.png"
#define SHAPE_AMOUNT 20
std::vector<std::vector<Point>> s_model;
std::vector<Point> s_model_cen;
std::vector<int> s_model_len1;
std::vector<int> s_model_radius;


project3::project3(QObject *parent) :
    QObject(parent)
{


}
void topBlack(Mat &src,bool black)
{

    int offset =black?10:-10;//偏移量

    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(src, src, MORPH_TOPHAT , element);
    else
        morphologyEx(src, src, MORPH_BLACKHAT, element);

}
void project3::insertFeature(std::vector<int> &srcArr,int index,Point cen,std::vector<Point> &contours,int amount,int radius)
{



    float num=contours.size()*1.0/amount;

    int index1,index2,index3;
    int d1,d2,d3;

    int base=90;
    bool isAbs=0;
    for(float i=0;i<contours.size();i+=num)
    {
        index1=i-num;
        index2=i;
        index3=i+num;
        if(index1<0)
            index1+=contours.size();
        if(index3>=contours.size())
            index3%=contours.size();

        d1=common::calcAngle(contours[index1],contours[index2]);
        d2=common::calcAngle(contours[index2],contours[index3]);
        d3=common::angleDif1(d1,d2,base,isAbs);


        //角度
        srcArr.push_back(d3);

        //距离


        tempDisArr[index].push_back(common::getLineLen(contours[index2],cen)*1.0/contours.size());

        tempDisArr2[index].push_back(common::getLineLen(contours[index2],cen)*1.0/radius);

    }


}

void createCpk3(QStringList list)
{
    QString fileName="D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk";
    if(QFile::exists(fileName))
    {
        return;
    }

    int cols=list.size();
    QString str;
    for(int i=0;i<cols;i++)
    {
        str+=QString("<Item R0=\"%2\">C%1</Item>").arg(i).arg(list[i]);
    }

    QString s=QString("<?xml version=\"1.0\" encoding=\"utf-8\"?><TyCPKToolsFileData><Config USL_E=\"-1\" USL_V=\"0.5\" Target_E=\"-1\" Target_V=\"0\" LSL_E=\"-1\" LSL_V=\"-0.5\" SubGroupSize=\"16\" DataGroupSize=\"0\" SampleSize=\"8\" DecimalSize=\"2\" CalculateSTDKind=\"0\"/><Cells><ColCount>%1</ColCount><RowCount>1</RowCount>%2</Cells></TyCPKToolsFileData>").arg(cols).arg(str);
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);

    f.write(s.toUtf8().data());
    f.close();
}

void addCpk3(QStringList title,QStringList list)
{
    createCpk3(title);

    QFile f1("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk");
    f1.open(QIODevice::ReadOnly);
    QRegExp rx("\\d+</RowCount");
    QRegExp rx1("\\d+");

    QString txt=QString::fromUtf8(f1.readAll());

    f1.close();
    QFile f("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk");
    f.open(QIODevice::WriteOnly);

    rx.indexIn(txt,0);
    qDebug()<<rx.cap(0);
    rx1.indexIn(rx.cap(0),0);
    int count=rx1.cap(0).toInt();


    for(int i=list.size()-1;i>=0;i--)
    {

     txt=txt.replace(QString(">C%1<").arg(i),QString(" R%1=\"%2\">C%1<").arg(i).arg(list[i]));

    }
     txt=txt.replace(QString(">%1</RowCount").arg(count),QString(">%1</RowCount").arg(count+1));

    f.write(txt.toUtf8().data());
    f.close();



}
void project3::regex()
{
    QDir dir("D:/DzicsRobot/CPK/");
    dir.mkdir("D:/DzicsRobot/CPK/");
    dir.setFilter(QDir::Files);
    QFileInfoList list3=dir.entryInfoList();
    if(list3.size()>20)
    {
        for(int i=0;i<list3.size();i++)
        {
            if(list3[i].fileName().right(3)=="cpk"&&list3[i].fileName().length()>10)
            {

                if(i<list3.size()/2)
                    QFile::remove("D:/DzicsRobot/CPK/"+list3[i].fileName());
            }
        }
    }

return;


    QStringList title,list;
    title<<"日期"<<"最否合格"<<"最小直径"<<"最大直径"<<"贴边";
    list<<QDateTime::currentDateTime().toString("hh:mm:ss yyyy-MM-dd")<<"合格"<<"1.323mm"<<"2.333mm"<<"无";
    addCpk3(title,list);


    return;




    QRegExp rx("\\d+</ColCount");

    QString str = "afd<ColCount>6</ColCount>";
    qDebug()<<rx.indexIn(str, 0);
  //  qDebug() << rx.matchedLength();     // 5, length of the last matched string
                                                // or -1 if there was no match
 // qDebug() << rx.capturedTexts();     // QStringList("a=100", "a", "100"),
                                                //   0: text matching pattern
                                                //   1: text captured by the 1st ()
                                                //   2: text captured by the 2nd ()
    qDebug() << rx.cap(0);
}
bool getTrangleP(Mat &src,std::vector<Point> &contours,Point cen,int r,int sideLen,int pIndex[],int &allP)
{
    pIndex[0]=pIndex[1]=pIndex[2]=0;
    bool flag=false;
    if(cen.x+r>=src.cols||cen.y+r>=src.rows)
        return flag;
    int indexTmp;
    int index[3];
    int times=0,j=0;

    for(int i=0;i<contours.size();i++)
    {
        if(abs(common::getLineLen(contours[i],cen)-r)<3)
        {

            if(times>0)
            {
                qDebug()<<"aaa"<<common::getLineLen(contours[i],contours[indexTmp])<<sideLen;
               if(common::getLineLen(contours[i],contours[indexTmp])>sideLen)
               {

                   if(j<2) index[j]=indexTmp;
                   ++j;
                   if(j==2) index[j]=i;
               }
            }
            ++times;
            indexTmp=i;
        }
    }
    if(j>1)
    {
        pIndex[0]=index[0];
        pIndex[1]=index[1];
        pIndex[2]=index[2];
    }
    if(j==2) flag=true;

    allP=j+1;
    return flag;
}
bool getTrangleP2(Mat &src,std::vector<Point> &contours,Point cen,int r,int sideLen,int pIndex[])
{
    pIndex[0]=pIndex[1]=0;
    bool flag=false;
    if(cen.x+r>=src.cols||cen.y+r>=src.rows)
        return flag;
    int indexTmp;
    int index[3];
    int times=0,j=0;

    for(int i=0;i<contours.size();i++)
    {
        if(abs(common::getLineLen(contours[i],cen)-r)<2)
        {

            if(times>0)
            {
               // qDebug()<<"aaa"<<common::getLineLen(contours[i],contours[indexTmp])<<sideLen;
               if(common::getLineLen(contours[i],contours[indexTmp])>sideLen)
               {

                   index[0]=indexTmp;
                   index[1]=i;

                   ++j;
                   break;

               }
            }

            ++times;
            indexTmp=i;
        }
    }
    if(j==1)
    {
        pIndex[0]=index[0];
        pIndex[1]=index[1];
        flag=true;

    }

    return flag;
}

int trangleFeature2(Mat &src,std::vector<Point> &contours,int sideLen,int pIndex[],Rect &roi)
{
    int num=0;
    //num+=common::trangleCalc2(src,contours,sideLen,step,roi);
    num+=common::trangleCalc(src,contours,sideLen,pIndex[0],roi);
    num+=common::trangleCalc(src,contours,sideLen,pIndex[1],roi);
    //qDebug()<<"num"<<num;
    return num;
}

void project3::test()
{
    unsigned char c=200;
    int a=c;
    unsigned char c1=a;
    qDebug()<<a<<"c1"<<(int)c1;
}
Point line6thArr[100];
int line6Feature[100];
bool findTrangle(int num)
{
    bool flag=false;
    for(int i=0;i<num;i++)
    {
        for(int j=i+1;j<num;j++)
        {
           // qDebug()<<"aaa"<<abs(line6thArr[i].x-line6thArr[j].x)<<abs(line6thArr[i].y-line6thArr[j].y);
            if(abs(line6thArr[i].x-line6thArr[j].x)<9&&abs(line6thArr[i].y-line6thArr[j].y)<9)
            {
                if(line6Feature[i]>3||line6Feature[j]>3)
                flag=true;
            }
        }
    }

    return flag;
}
void project3::zt_oneTrangle(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//三角形外接矩形len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//三角形外接矩形角度
    int rAngle2=param.var1[11];
    int sideLen=param.var1[12];//三角形最小边长
    int featureNum=param.var1[13];//特征数量
    double angleOffset=param.var2[0];//角度偏移
    double angle1,area;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;
    medianBlur(m5, m5, 3);

    int error=0;


    for(int i=thresh;i<thresh2;i+=10)
    //for(int i=thresh2;i>thresh;i-=10)
    {
        error=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;
            common::mostVal(error,1);
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            //qDebug()<<"distance1"<<distance1<<minLen<<maxLen;
            if(distance1<minLen||distance1>maxLen)
                continue;

            common::mostVal(error,2);
            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                if( abs(common::calcAngle(rectpoint[0],rectpoint[1]))<rAngle2)
                    continue;
            }
            else
            {
                if( abs(common::calcAngle(rectpoint[1],rectpoint[2]))<rAngle1)
                    continue;
            }
            common::mostVal(error,3);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            qDebug()<<"len1"<<len1<<len2;
            if(len1<rLen1||len2>rLen2)
                continue;

            common::mostVal(error,4);

            //area=contourArea(contours[k]);
            if(common::contourType(m5,contours[k],i,false))
                continue;

            common::mostVal(error,5);
            area=contourArea(contours[k]);

            if(area/(len1*len2)<0.4)
                continue;



            common::mostVal(error,6);


            Point2f center;
            float radius;
            minEnclosingCircle(contours[k],center,radius);


            int pIndex[2];

            if(!getTrangleP2(src,contours[k],center,radius,sideLen,pIndex))
                continue;

            common::mostVal(error,7);

            int num= trangleFeature2(rgb,contours[k],sideLen,pIndex,roi);
            qDebug()<<"num"<<num<<"i"<<i<<"size:"<<contours[k].size()<<"area"<<area/(len1*len2)<<"len1 len2"<<len1<<len2;
            if(num<featureNum)
                continue;

            common::mostVal(error,8);
            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            param.var2[19]=angle1;
            param.var1[28]=x1;
            param.var1[29]=y1;
            if(DEBUG)
                circle(rgb,Point(x1+roi.x,y1+roi.y),5,Scalar(255,0,0),2);


            loop=false;
            break;



        }

        qDebug()<<"error"<<error<<"i:"<<i;
    }

}

void project3::zt_Iline(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[20];//检测最小值
    unsigned int objMaxSize=param.var1[21];//检测最大值
    int rLen1=param.var1[22];//矩形len
    int rLen2=param.var1[23];
    int step=param.var1[24];//step
    int offsetX=param.var1[25];//x距离
    int startCalcDis=param.var1[26];//开始位置
    int thresh=param.var1[27];//二值
    Mat m5=src(roi).clone();

    Mat m1;
    std::vector<std::vector<Point>> contours;
    int x1,y1,len1,len2;
    param.var2[19]=360;
    medianBlur(m5, m5, 5);
    int n1,j2;
    for(int i=thresh;i>80;i-=10)
    {

        threshold(m5,m1,i,255,THRESH_BINARY);

        int errorFlag=0;


        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            n1=0;

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
            {
                common::mostVal(errorFlag,1);
                continue;
            }
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;


            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }
            common::mostVal(errorFlag,2);

            if(len1<rLen1||len2>rLen2)
                continue;
            common::mostVal(errorFlag,3);
            if(common::contourType(m5,contours[k],i,true))
                continue;
            common::mostVal(errorFlag,4);
            for(int j=0;j<contours[k].size()-step;j++)
            {
                if(abs(common::calcAngle2(Point(x1,y1),contours[k][j]))<25||abs(common::calcAngle2(Point(x1,y1),contours[k][j]))>155)
                //if(abs(contours[k][j].x-cenX)>startCalcDis&&abs(contours[k][j+step].x-cenX)>startCalcDis)
                {
                    if(abs(contours[k][j].x-contours[k][j+step].x)>15)
                        qDebug()<<"i:"<<i<<"aaaa"<<abs(contours[k][j].x-contours[k][j+step].x)<<n1;
                    if(abs(contours[k][j].x-contours[k][j+step].x)>offsetX)
                    {

                        circle(g_dstImage2,Point(contours[k][j].x+roi.x,contours[k][j].y+roi.y),5,Scalar(255,0,0),3);
                        circle(g_dstImage2,Point(contours[k][j+step].x+roi.x,contours[k][j+step].y+roi.y),5,Scalar(255,0,0),3);
                        ++n1;
                        j2=j;
                        j+=150;
                    }
                }
            }

            if(n1==1)
            {
                //circle(g_dstImage2,Point(contours[k][70].x+roi.x,contours[k][70].y+roi.y),9,Scalar(255,0,0),3);
                //circle(g_dstImage2,Point(contours[k][j2+step].x+roi.x,contours[k][j2+step].y+roi.y),5,Scalar(255,0,0),3);

                param.var2[19]=common::calcAngle2(Point(cenX,cenY),contours[k][j2]);
                return;
            }

        }

        qDebug()<<"error:"<<errorFlag<<"i:"<<i;
    }

}


void project3::zt_6thLine(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int rLen1=param.var1[7];//三角形外接矩形len
    int rLen2=param.var1[8];
    int sideLen=param.var1[9];//三角形最小边长

    double angleOffset=param.var2[0];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;
    medianBlur(m5, m5, 3);


    int num1=0;
    int errorFlag;
    for(int i=30;i<thresh;i+=10)
      //  for(int i=130;i<135;i+=10)
    {
        if(!loop)
            break;
        int a1=0,a2=0;

        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        qDebug()<<"contours.size"<<contours.size();
        for (unsigned int k=0 ; k<contours.size(); k++)
        {

//            if(k>0&&DEBUG&&contours[k-1].size()>40)
//            {
//                qDebug()<<"size:"<<contours[k-1].size()<<"i:"<<i<<"error"<<errorFlag;
//            }

            errorFlag=-1;


            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
            {

                errorFlag=0;
                continue;
            }
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(distance1<minLen||distance1>maxLen)
            {
                //qDebug()<<"aaa"<<distance1<<minLen<<maxLen;
                //circle( rgb,Point(x1+roi.x,y1+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
                errorFlag=1;
                continue;
            }
//            if(contours[k].size()==59)
//            {
//                circle( rgb,Point(contours[k][0].x+roi.x,contours[k][0].y+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
//                qDebug()<<"aaa"<<contours[k].size();
//            }
            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }
//            if(contours[k].size()==59)
//                qDebug()<<"len1"<<len1<<len2<<rLen1<<rLen2;
//qDebug()<<"aaa"<<len1<<rLen1<<len2<<rLen2<<x1+roi.x<<y1+roi.y;
            if(len1<rLen1||len2>rLen2)
            {
                errorFlag=2;
                continue;
            }



//            if(m5.at<uchar>(contours[k][0].y-1,contours[k][0].x)>i)
//                continue;

            Point2f center;
            float radius;
            minEnclosingCircle(contours[k],center,radius);

            int pIndex[2];
            getTrangleP2(src,contours[k],center,radius,sideLen,pIndex);
            //qDebug()<<"find 2 point"<<b;
            //circle( rgb,Point(center.x+roi.x,center.y+roi.y), radius, Scalar(193,255,3), 2, CV_AA);

            int num= trangleFeature2(rgb,contours[k],sideLen,pIndex,roi);

            line6thArr[a1]=End_Rage2D1.center;
            line6Feature[a1]=num;
            ++a1;
            qDebug()<<"i:"<<i<<"num"<<num<<"size"<<contours[k].size()<<"cen"<<x1+roi.x<<y1+roi.y<<"a1"<<a1;

            errorFlag=4;
            if(a1>1&&a1<100)
            {
                //circle( rgb,Point(x1+roi.x,y1+roi.y), 3, Scalar(193,255,3), 2, CV_AA);

                errorFlag=5;
                if(!findTrangle(a1))
                    continue;

                    // qDebug()<<"aaa";
                    circle( rgb,Point(contours[k][pIndex[0]].x+roi.x,contours[k][pIndex[0]].y+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
                    circle( rgb,Point(contours[k][pIndex[1]].x+roi.x,contours[k][pIndex[1]].y+roi.y), 3, Scalar(193,255,3), 2, CV_AA);


                    angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
                    if(angleOffset+angle1>180)
                        angle1=angleOffset+angle1-360;
                    else if(angleOffset+angle1<-180)
                        angle1=angleOffset+angle1+360;
                    else
                        angle1+=angleOffset;

                    param.var2[19]=angle1;
                    param.var1[28]=x1;
                    param.var1[29]=y1;


                    loop=false;
                    break;

            }






//            rectangle(rgb,Rect(x1-30+roi.x,y1-30+roi.y,70,60),Scalar(0,0,255),3,8,0);//用矩形画矩形窗
//            namedWindow("rgb",0);
//            resizeWindow("rgb",700,500);
//            imshow("rgb",rgb);


        }



    }

}
void project3::zt_zt01Match(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int rLen1=param.var1[7];//三角形外接矩形len
    int rLen2=param.var1[8];
    int sideLen=param.var1[9];//三角形最小边长

    double angleOffset=param.var2[0];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;
    medianBlur(m5, m5, 3);


    int num1=0;
    int errorFlag;
    for(int i=30;i<thresh;i+=10)
      //  for(int i=130;i<135;i+=10)
    {
        if(!loop)
            break;
        int a1=0,a2=0;

        threshold(m5,m1,i,255,THRESH_BINARY);

        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];

        for (unsigned int k=0 ; k<contours.size(); k++)
        {



            errorFlag=-1;


            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
            {

                errorFlag=0;
                continue;
            }
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(distance1<minLen||distance1>maxLen)
            {

                errorFlag=1;
                continue;
            }

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<rLen1||len2>rLen2)
            {
                errorFlag=2;
                continue;
            }


            if(common::contourType(m5,contours[k],i,true))
                continue;


            int matchVal=matchShapes(contours[k],contours[k],1,31);
            if(matchVal<80)
                continue;

            qDebug()<<"matchVal"<<matchVal;
            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            param.var2[19]=angle1;
            param.var1[28]=x1;
            param.var1[29]=y1;



            loop=false;
            break;




        }



    }

}


bool Comp(const Point &a,const Point &b)
{
    return a.y<b.y;
}
bool zt_gn5(Point p[],int maxAngleDif,int num1)
{
    std::vector<Point> arr;
    for(int i=0;i<num1;i++)
    {
        arr.push_back(p[i]);
    }

    std::sort(arr.begin(),arr.end(),Comp);


   // qDebug()<<"common::angleDif(d1,d2,90)"<<common::angleDif(d1,d2,90);
    if(abs(common::calcAngle(arr[0],arr[num1-1]))<70)
        return false;

    return true;
}
void project3::zt_GLine(Mat &src, Mat &rgb, Rect roi, params &param)
{
    //未使用
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int minRectLen=param.var1[8];//最小边长
    int maxRectLen=param.var1[9];//最小边长
    int maxFontDistance=param.var1[10];//字体最大间距
    double angleOffset=param.var2[0];//角度偏移
    double ratioWH=param.var2[1];//长宽比例


    int maxAngleDif=25;
    double angle1,angleTmp;

    int x1,y1,len1,len2,distance1;

    Point p1[3],p2[3],p3;
    Mat m5=src.clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;


    int num1,num2;
    for(int i=thresh;i<thresh2;i+=2)
    //for(int i=90;i<91;i+=2)
    {
        num1=0;
        num2=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY_INV);

        imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<minRectLen||len2>maxRectLen)
                continue;




            if(m5.at<uchar>(contours[k][0].y-1,contours[k][0].x)<i)
                continue;


            if(len2*1.0/len1>ratioWH)
                continue;

            angleTmp=common::calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
            if(abs(angleTmp)<45)
            {
                p1[num1++]=End_Rage2D1.center;
                circle( rgb,Point(x1+roi.x,y1+roi.y), 3, Scalar(193,255,3), 2, CV_AA);


            }

            if(abs(angleTmp)>135&&abs(angleTmp)<=180)
            {
                p2[num2++]=End_Rage2D1.center;
                circle( rgb,Point(x1+roi.x,y1+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
            }
            qDebug()<<"i:"<<i<<"num1 num2"<<num1<<num2<<"size"<<contours[k].size();
            if(num1>2||num2>2)
            {
                int fontWidth1,fontWidth2,fontNum=0;

                Point upPos(1000,1000),downPos(0,0);
                for(int i=0;i<num1;i++)
                {
                    if(p1[i].y<upPos.y)
                        upPos=p1[i];
                    if(p1[i].y>downPos.y)
                        downPos=p1[i];
                }
                fontWidth1=common::getLineLen(upPos,downPos);
                if(fontWidth1<maxFontDistance&&num1>2)
                {
                    p3.x=(p1[0].x+p1[1].x+p1[2].x)/3;
                    p3.y=(p1[0].y+p1[1].y+p1[2].y)/3;


                    ++fontNum;
                    if(!zt_gn5(p1,maxAngleDif,num1))
                        continue;
                }
                upPos.y=1000;
                downPos.y=0;
                for(int i=0;i<num2;i++)
                {
                    if(p2[i].y<upPos.y)
                        upPos=p2[i];
                    if(p2[i].y>downPos.y)
                        downPos=p2[i];
                }

                fontWidth2=common::getLineLen(upPos,downPos);
                if(fontWidth2<maxFontDistance&&num2>2)
                {
                    p3.x=(p2[0].x+p2[1].x+p2[2].x)/3;
                    p3.y=(p2[0].y+p2[1].y+p2[2].y)/3;

                    ++fontNum;
                    if(!zt_gn5(p2,maxAngleDif,num2))
                        continue;
                }

                if(fontNum>1)
                    continue;


                angle1=common::calcAngle2(Point(cenX,cenY),p3);
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

                param.var2[19]=angle1;
                param.var1[28]=p3.x;
                param.var1[29]=p3.y;
                qDebug()<<"p3"<<p3.x<<p3.y;
                rectangle(rgb,Rect(p3.x-30+roi.x,p3.y-80+roi.y,70,160),Scalar(0,0,255),3,8,0);//用矩形画矩形窗
                namedWindow("rgb",0);
                resizeWindow("rgb",800,600);
                imshow("rgb",rgb);
                loop=false;
                break;
            }

        }

    }
}

void project3::zt_GLineGN5(Mat &src, Mat &rgb,Mat &src2, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    volatile unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int minRectLen=param.var1[8];//最小边长
    int maxRectLen=param.var1[9];//最小边长
    int maxFontDistance=param.var1[10];//字体最大间距
    int minFontDistance=91;
    int allNum=37;
    double angleOffset=param.var2[0];//角度偏移
    double ratioWH=param.var2[1];//长宽比例
    double angle1,angleTmp;

    int x1,y1,len1,len2,distance1;

    int err;
    int maxAngleDif=25;
    Point p1[10],p2[10],p3;
    Mat m5=src.clone();
    Mat m1,m6;


    param.var2[19]=360;
    bool loop=true;


    int num1,num2,num3;

    for(int i=100;i>35;i--)
    //for(int i=50;i<51;i+=2)
    {
        num1=0;
        num2=0;
        num3=0;
        err=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY_INV);

        imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()>50&&contours[k].size()<objMaxSize)
            {
                ++num3;
            }

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;

            if(num3>allNum)
            {
                //qDebug()<<"jump out"<<num3<<"i:"<<i;// std::cout<<"jump out"<<std::endl;
                //return;
            }
 //circle( rgb,Point(x1+roi.x,y1+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;


            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<minRectLen||len2>maxRectLen)
                continue;
common::mostVal(err,1);
            if(src2.at<uchar>(contours[k][0].y-1,contours[k][0].x)<i)
                continue;
common::mostVal(err,2);

            if(len2*1.0/len1>ratioWH)
                continue;

            common::mostVal(err,3);
            angleTmp=common::calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
            if(abs(angleTmp)<45)
            {
                p1[num1++]=End_Rage2D1.center;
                circle( rgb,Point(x1+roi.x,y1+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
                if(num1>9)
                    num1=9;

            }




            if(abs(angleTmp)>135&&abs(angleTmp)<=180)
            {
                p2[num2++]=End_Rage2D1.center;
                //circle( rgb,Point(x1+roi.x,y1+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
                if(num2>9)
                    num2=9;
            }
           // qDebug()<<"i:"<<i<<"num1 num2"<<num1<<num2<<"size"<<contours[k].size();
            if(num1>2||num2>2)
            {
                int fontWidth1,fontWidth2,fontNum=0;

                Point upPos(1000,1000),downPos(0,0);
                for(int i1=0;i1<num1;i1++)
                {
                    if(p1[i1].y<upPos.y)
                        upPos=p1[i1];
                    if(p1[i1].y>downPos.y)
                        downPos=p1[i1];

                    circle( rgb,Point(p1[i1].x+roi.x,p1[i1].y+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
                }
                //qDebug()<<"aaa"<<upPos.y+roi.y<<downPos.y+roi.y<<minFontDistance<<maxFontDistance<<"i:"<<i;
                //circle( rgb,Point(upPos.x+roi.x,upPos.y+roi.y), 8, Scalar(53,255,3), 2, CV_AA);
                //circle( rgb,Point(downPos.x+roi.x,downPos.y+roi.y), 3, Scalar(193,75,3), 2, CV_AA);
                fontWidth1=common::getLineLen(upPos,downPos);
                //qDebug()<<"fontWidth1"<<fontWidth1<<minFontDistance<<maxFontDistance<<num1;


                if(fontWidth1>minFontDistance&&fontWidth1<maxFontDistance&&num1>2)
                {
                    p3.x=(p1[0].x+p1[1].x+p1[2].x)/3;
                    p3.y=(p1[0].y+p1[1].y+p1[2].y)/3;



                    if(zt_gn5(p1,maxAngleDif,num1))
                        ++fontNum;

                }

                upPos.y=1000;
                downPos.y=0;
                 qDebug()<<"bbbb"<<i<<num2;
                for(int i1=0;i1<num2;i1++)
                {
                    if(p2[i1].y<upPos.y)
                        upPos=p2[i1];
                    if(p2[i1].y>downPos.y)
                        downPos=p2[i1];
                    //circle( rgb,Point(p2[i].x+roi.x,p2[i].y+roi.y), 3, Scalar(193,255,3), 2, CV_AA);
                }

                fontWidth2=common::getLineLen(upPos,downPos);

                if(fontWidth2>minFontDistance&&fontWidth2<maxFontDistance&&num2>2)
                {
                    p3.x=(p2[0].x+p2[1].x+p2[2].x)/3;
                    p3.y=(p2[0].y+p2[1].y+p2[2].y)/3;


                    if(zt_gn5(p2,maxAngleDif,num2))
                        ++fontNum;

                }

common::mostVal(err,4);
                if(fontNum!=1)
                    continue;

common::mostVal(err,5);

                if(!loop)
                    continue;

common::mostVal(err,6);
                angle1=common::calcAngle2(Point(cenX,cenY),p3);
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

                param.var2[19]=angle1;
                param.var1[28]=p3.x;
                param.var1[29]=p3.y;
                qDebug()<<"GN5 detected num3:"<<num3<<"i:"<<i;
                rectangle(rgb,Rect(p3.x-30+roi.x,p3.y-80+roi.y,70,160),Scalar(0,0,255),3,8,0);//用矩形画矩形窗


                loop=false;

            }

        }

        qDebug()<<"i"<<i<<"err"<<err;
    }
}

void project3::zt_GLineGN5_2(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    volatile unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值

    double angleOffset=param.var2[0];//角度偏移

    double angle1,angleTmp;

    int x1,y1,len1,len2,distance1;


    Mat m5=src.clone();
    Mat m1;


    param.var2[19]=360;
    bool loop=true;


    int num1,num2,num3;

    for(int i=50;i<110;i+=2)
    //for(int i=50;i<51;i+=2)
    {
        num1=0;
        num2=0;
        num3=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY_INV);

        imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {


            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;



            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;

            if(common::contourType(m5,contours[k],i,true))
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }





            angleTmp=common::calcAngle2(Point(cenX,cenY),End_Rage2D1.center);

            if(abs(angleTmp)>45&&abs(angleTmp)<135)
                continue;



            int matchVal=matchShapes(contours[k],contours[k],1,31);

            if(matchVal<80)
                continue;


            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

                param.var2[19]=angle1;

                qDebug()<<"i:"<<i<<"matchVal"<<matchVal;
                rectangle(rgb,Rect(x1-30+roi.x,y1-80+roi.y,70,160),Scalar(0,0,255),3,8,0);//用矩形画矩形窗


                loop=false;
                break;



        }

    }
}


bool project3::calcI(Mat &src, Point &cen, Point &font, int area, int len, Rect &rect, int difVal)
{
    int y,num=0;
    if(font.x>cen.x)
        y=rect.y-len;
    else
        y=rect.y;




    Rect roi(rect.x,y,rect.width,rect.height+len);
    Mat m=src(roi);
    num=countNonZero(m);

    qDebug()<<"countNonZero"<<num<<"area"<<area;

//    if(area==573)
//    {
//    rectangle(src,roi,Scalar(255,255,255),1,8,0);//用矩形画矩形窗
//    imshow("sr111",src);
//    }
    if(num-area<difVal)
        return true;
    else
        return false;


}


void project3::zt_GLineNI(Mat &src, Mat &rgb, Rect roi, params &param)
{

    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[15];//检测字体最小值
    unsigned int objMaxSize= param.var1[16];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int minRectLen=param.var1[17];//最小边长
    int maxRectLen=param.var1[18];//最大边长
    double angleOffset=180;//角度偏移
    double ratioWH=param.var2[2];//长宽比例
    double ratioArea=param.var2[3];//面积比例
    double angle1,angleTmp,fontAngle,area;

    int x1,y1,len1,len2,distance1;


    Mat m5=src.clone();
    Mat m1,m6,m7;

    param.var2[19]=360;
    bool loop=true;


    int num1,num2;
    //for(int i=thresh;i<thresh2;i+=2)
    for(int i=100;i>21;i-=2)
    {
        num1=0;
        num2=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        threshold(m5,m7,i,255,THRESH_BINARY);


        imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];

        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
                fontAngle=common::calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                fontAngle=common::calcAngle(rectpoint[1],rectpoint[2]);
            }

//qDebug()<<"aaa"<<len1<<len2<<minRectLen<<maxRectLen;
            if(len1<minRectLen||len1>26||len2>maxRectLen||len2<40)
                continue;



//qDebug()<<"1111"<<m5.at<uchar>(contours[k][0].y-1,contours[k][0].x)<<i<<x1+roi.x<<y1+roi.y;
//            if(m5.at<uchar>(contours[k][0].y-1,contours[k][0].x)<i)
//                continue;

//qDebug()<<"1111"<<len2*1.0/len1<<ratioWH;
            if(len2*1.0/len1<ratioWH)
                continue;

            area=contourArea(contours[k]);
            Rect rectpoint2=boundingRect(contours[k]);
            if(!calcI(m7,Point(cenX,cenY),Point(x1,y1),area,50,rectpoint2,100))
                continue;


 //           qDebug()<<"aaa"<<x1<<y1<<len1*len2<<area<<"i"<<i;
//            if(len1*len2/area>ratioArea)
//                continue;

            angleTmp=common::calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
           // qDebug()<<"aaa"<<x1+roi.x<<y1+roi.y<<"abs(fontAngle)"<<abs(fontAngle);
            if(abs(fontAngle)<10)
            {

                angle1=common::calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

                if(abs(angle1)<20||abs(angle1)>160)
                {

                    param.var2[19]=angle1;
                    param.var1[28]=x1;
                    param.var1[29]=y1;

                    if(DEBUG) qDebug()<<"i:"<<i<<"area"<<area<<"len1*len2/area"<<len1*len2/area<<"size"<<contours[k].size();
                    for(int i = 0;i< 4;i++)
                    {

                        line(rgb,cvPointFrom32f(Point(rectpoint[i].x+roi.x,rectpoint[i].y+roi.y)),cvPointFrom32f(Point(rectpoint[(i+1)%4].x+roi.x,rectpoint[(i+1)%4].y+roi.y)),Scalar(0,0,255),3);
                    }

                    loop=false;
                    break;
                }
            }

        }

    }
}

void project3::loadShapePoint(int amount=30)
{


    QString fileName=SHAPE_FILENAME;

    Mat m1;
    Mat m = imread( fileName.toStdString(), 0);
    if(m.total()==0)
        return;

    threshold(m,m1,200,255,THRESH_BINARY);


    int len1,len2;
    s_model.clear();
    s_model_cen.clear();
    tempArr.clear();
    tempDisArr.clear();
    tempDisArr2.clear();
    s_model_radius.clear();
    //s_modelNum=0;
    int count=0;
    std::vector<std::vector<Point>> contours;
    RotatedRect     End_Rage2D1;
    CvPoint2D32f rectpoint[4];
    findContours(m1,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


    for (unsigned int k=0 ; k<contours.size(); k++)
    {
        if(contours[k].size()<30)
            continue;

        End_Rage2D1 = minAreaRect(Mat(contours[k]));
        cvBoxPoints(End_Rage2D1,rectpoint);

        float modelRadius;
        Point2f modelCen;

        minEnclosingCircle(contours[k], modelCen, modelRadius );
        //double ratio1=contourArea(contours[k])*1.0/(modelRadius*modelRadius*3.14);



        len1=common::getLineLen(rectpoint[0],rectpoint[1]);
        len2=common::getLineLen(rectpoint[1],rectpoint[2]);

        if(len1+len2+10>m.cols+m.rows)
            continue;


        s_model_len1.push_back(len1>len2?len1:len2);
        s_model.push_back(contours[k]);
        std::vector<int> v1;
        tempArr.push_back(v1);
        std::vector<double> v2;
        tempDisArr.push_back(v2);
        std::vector<double> v3;
        tempDisArr2.push_back(v3);

        s_model_cen.push_back(End_Rage2D1.center);
        s_model_radius.push_back(modelRadius);

        //qDebug()<<"model size"<<contours[k].size()<<"modelRadius"<<modelRadius;
        insertFeature(tempArr[count],count,End_Rage2D1.center,contours[k],amount,modelRadius);

        ++count;
        //qDebug()<<"aaa"<<contours[k].size()<<m.cols<<m.rows<<"len1 2"<<len1<<len2;
        //s_model[j].swap(contours[k]);


    }

    qDebug()<<"tempArr size"<<tempArr.size()<<"amount"<<amount<<"model name:"<<fileName;
    //s_modelNum=s_model.size();

}

//输入图像
Mat src_hsv;
//灰度值归一化
Mat bgr_hsv;
//HSV图像
Mat hsv_1;
//色相
int hmin = 0;
int hmin_Max = 360;
int hmax = 360;
int hmax_Max = 360;
//饱和度
int smin = 0;
int smin_Max = 255;
int smax = 255;
int smax_Max = 255;
//亮度
int vmin = 106;
int vmin_Max = 255;
int vmax = 250;
int vmax_Max = 255;
//显示原图的窗口
string windowName = "src";
//输出图像的显示窗口
string dstName = "dst";
//输出图像
Mat dst_hsv;
//回调函数

void callBackHSV(int, void*)
{
    //输出图像分配内存
    dst_hsv = Mat::zeros(src_hsv.size(), CV_32FC3);
    //掩码
    Mat mask;
    inRange(hsv_1, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)), Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
    //inRange(hsv_1, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)), Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
    //只保留
//    for (int r = 0; r < bgr_hsv.rows; r++)
//    {
//        for (int c = 0; c < bgr_hsv.cols; c++)
//        {
//            if (mask.at<uchar>(r, c) == 255)
//            {
//                dst_hsv.at<Vec3f>(r, c) = bgr_hsv.at<Vec3f>(r, c);
//            }
//        }
//    }
    //输出图像
    imshow(dstName, mask);
    //保存图像

}

void project3::rgbPic(IplImage *img,bool isHsv)
{

     IplImage *imgChannel[4]={0,0,0,0};
     IplImage *imgColor[4]={0,0,0,0};

     if(!img)
     {
     // printf("无法读取 %s 图像数据！\n",filename);
      return;
     }
     for(int i=0;i<img->nChannels;i++)
     {
      imgChannel[i]=cvCreateImage(cvGetSize(img),img->depth,1);
      imgColor[i]=cvCreateImage(cvGetSize(img),img->depth,3);
      cvZero(imgColor[i]);
     }
     IplImage *hsv=cvCreateImage(cvGetSize(img),img->depth,3);

        cvCvtColor(img,hsv,CV_BGR2HSV);

     IplImage *dst=cvCreateImage(cvGetSize(img),img->depth,1);
     IplImage *img2;
     if(isHsv)
         img2=hsv;
     else
         img2=img;
     cvSplit(img2,imgChannel[0],imgChannel[1],imgChannel[2],imgChannel[3]);

//     cvMerge(imgChannel[0],NULL,NULL,NULL,imgColor[0]);
//     cvMerge(NULL,imgChannel[1],NULL,NULL,imgColor[1]);
//     cvMerge(NULL,NULL,imgChannel[2],NULL,imgColor[2]);

     //cvThreshold(imgChannel[0],dst,140,255,THRESH_BINARY);
     //cvShowImage("thresh1",dst);

     //cvThreshold(imgChannel[1],dst,140,255,THRESH_BINARY);
     //cvShowImage("thresh2",dst);


     if(hsv)
     {

         src_hsv=img2;

         //彩色图像的灰度值归一化
         src_hsv.convertTo(bgr_hsv, CV_32FC3, 1.0 / 255, 0);
         //颜色空间转换
         cvtColor(bgr_hsv, hsv_1, COLOR_BGR2HSV);
         //定义输出图像的显示窗口
         namedWindow(dstName, WINDOW_NORMAL);
         //调节色相 H
         createTrackbar("hmin", dstName, &hmin, hmin_Max, callBackHSV);
         createTrackbar("hmax", dstName, &hmax, hmax_Max, callBackHSV);
         //调节饱和度 S
         createTrackbar("smin", dstName, &smin, smin_Max, callBackHSV);
         createTrackbar("smax", dstName, &smax, smax_Max, callBackHSV);
         //调节亮度 V
         createTrackbar("vmin", dstName, &vmin, vmin_Max, callBackHSV);
         createTrackbar("vmax", dstName, &vmax, vmax_Max, callBackHSV);
         callBackHSV(0, 0);

     }

     cvNamedWindow("显示图片",1);
     cvShowImage("显示图片",img);
     cvShowImage("通道1",imgChannel[0]);
     cvShowImage("通道2",imgChannel[1]);
     cvShowImage("通道3",imgChannel[2]);


}

void moveStraight(std::vector<Point> &contours,Point &cur,int distance,double angle3,Point &out)
{

    int xLen=distance*sin(angle3*3.1415/180)*-1;
    int yLen=distance*cos(angle3*3.1415/180);

    Point p2,p3;
    p2.x=cur.x+xLen;
    p2.y=cur.y+yLen;
    p3.x=cur.x+xLen*-1;
    p3.y=cur.y+yLen*-1;

    if(pointPolygonTest(contours,p2,false)==1)
        out=p2;
    else if(pointPolygonTest(contours,p3,false)==1)
        out=p3;
    else
    {
        circle( g_dstImage2, p2, 5, Scalar(255,255,0), 3, CV_AA);
        circle( g_dstImage2, p3, 5, Scalar(255,0,0), 3, CV_AA);
        out.x=0;
        out.y=0;
    }



}
void project3::clothEdge(Mat &src,Rect roi,params &param)
{
    Mat m1;
    Point p1,center(0,0);
    double angle2;
    int len1,len2,len3,x1,y1,beginIndex=-1,endIndex=-1,index1,index2,leftLen,rightLen;
    int minSize=param.var1[0];//size范围
    int i=param.var1[1];//二值

    int step=param.var1[2];//step
    int drawStep=15;
    int difVal=3;
     threshold(src,m1,i,255,THRESH_BINARY_INV);

//     namedWindow("thresh",0);
//     resizeWindow("thresh",800,600);
//     imshow("thresh",m1);

//     Mat m5=m1(Rect(1672,2283,200,200));
//     imwrite("d:/bbb.jpg",m5);

     std::vector<std::vector<Point>> contours;
     std::vector<Point> recordPoint;
     std::vector<Point> contours2;
     std::vector<int> indexList;
     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours



     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;


     for (unsigned int k=0 ; k<contours.size(); k++)
     {
        if(contours[k].size()<minSize)
             continue;

        if(src.at<uchar>(contours[k][0].y-1,contours[k][0].x)<i)
            continue;



         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         //area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;
         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);
         len3=len1>len2?len1:len2;

         if(len3+100>src.cols)
             continue;


         center.x=x1;
         center.y=y1;
         contours2=contours[k];
          qDebug()<<"size"<<contours[k].size();

             for(int i = 0;i< 4;i++)
             {
                 line(g_dstImage2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
             }

//             QString words=QString("%1").arg(contours[k].size());

//             putText( g_dstImage2, words.toStdString(), Point(End_Rage2D1.center.x-10+roi.x,End_Rage2D1.center.y+roi.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

//                  drawContours(g_dstImage2,contours,
//                               -1, // draw all contours
//                               Scalar(125), // in black
//                               1); // with a thickness of 2


             for(int j=0;j<contours[k].size();j+=5)
             {



                 index1=j+step;
                 index2=j-step;
                 if(index1>contours[k].size())
                     index1-=contours[k].size();
                 if(index2<0)
                     index2+=contours[k].size();



                 leftLen=common::getLineLen(contours[k][j],contours[k][index1]);
                 rightLen=common::getLineLen(contours[k][j],contours[k][index2]);

                 if(beginIndex!=-1)
                 {
                     if(common::getLineLen(contours[k][index1],contours[k][endIndex])>step)
                     {
                         indexList.push_back(beginIndex);
                         indexList.push_back(endIndex);
                         recordPoint.push_back(contours[k][beginIndex]);
                         recordPoint.push_back(contours[k][endIndex]);
                         beginIndex=-1;
                     }
                 }

                 if(leftLen+rightLen-common::getLineLen(contours[k][index1],contours[k][index2])>difVal)
                 {

                    if(beginIndex!=-1)
                    {
                        if(common::getLineLen(contours[k][index1],contours[k][endIndex])<step)
                            endIndex=index1;

                    }

                    if(beginIndex==-1)
                    {
                        beginIndex=index2;
                        endIndex=index1;
                    }

                    endIndex=index1;


                 }
             }
//circle( g_dstImage2, Point(contours[k][1458].x,contours[k][1458].y ), 1, Scalar(0,0,255), 1, CV_AA);
//circle( g_dstImage2, Point(contours[k][1410].x,contours[k][1410].y ), 1, Scalar(0,0,255), 1, CV_AA);
//circle( g_dstImage2, Point(contours[k][1386].x,contours[k][1386].y ), 1, Scalar(0,0,255), 1, CV_AA);
//circle( g_dstImage2, Point(contours[k][1366].x,contours[k][1366].y ), 1, Scalar(0,0,255), 1, CV_AA);

             //qDebug()<<"len"<<common::getLineLen(Point(0,0),Point(3,3))<<common::getLineLen(contours[k][1386],contours[k][1410])<<common::getLineLen(contours[k][1370],contours[k][1410]);

//             Mat m6=g_dstImage2(Rect(1672,2283,200,200));
//             imwrite("d:/aaa.jpg",m6);
             break;



     }


     for(int j=0;j<recordPoint.size();j++)
     {

         //circle( g_dstImage2, Point(recordPoint[j].x,recordPoint[j].y ), 25, Scalar(0,0,255), 3, CV_AA);

     }
int count=1,end2=contours2.size();
unsigned int j=0,k=0;
if(common::getLineLen(contours2[0],contours2[step])+common::getLineLen(contours2[0],contours2[contours2.size()-step])-common::getLineLen(contours2[step],contours2[contours2.size()-step])>difVal)
{j=step*2;end2=contours2.size()-step*2;}

    for (; j<end2; j+=drawStep)
     {
         index1=j+step;
         index2=j-step;
         if(index1>contours2.size())
             index1-=contours2.size();
         if(index2<0)
             index2+=contours2.size();

         angle2=common::calcAngle(contours2[index1],contours2[index2]);
         moveStraight(contours2,contours2[j],80,angle2,p1);

         if(k+1<indexList.size())
         {
             if(j>indexList[k]&&j<indexList[k+1])
             {

                 Point p2,p3,p4;
                 double angleTmp=common::calcAngle(recordPoint[k],recordPoint[k+1]);
                 moveStraight(contours2,recordPoint[k],80,angleTmp,p3);

                 moveStraight(contours2,recordPoint[k+1],80,angleTmp,p4);

                 p2.x=p3.x+(p4.x-p3.x)/drawStep*1.0*count;
                 p2.y=p3.y+(p4.y-p3.y)/drawStep*1.0*count;
                 ++count;

                 bool flag=true;
                 for(int n=j-15*drawStep;n>0&&n<contours2.size()-15*drawStep;n+=drawStep)
                 {
                     if(common::getLineLen(contours2[n],p2)<65)
                     {
                         flag=false;

                     }
                 }
                 if(flag)
                 circle( g_dstImage2, p2, 5, Scalar(0,0,255), 3, CV_AA);


                 continue;
             }

             if(j>indexList[k+1])
             {
                 k+=2;
                 count=1;
             }
         }
         bool flag=true;
         for(int n=j-15*drawStep;n>0&&n<contours2.size()-15*drawStep;n+=drawStep)
         {
             if(common::getLineLen(contours2[n],p1)<75)
             {
                 flag=false;
                 break;
             }
         }

         if(flag)
         circle( g_dstImage2, p1, 5, Scalar(0,0,255), 3, CV_AA);


     }




}



double project3::contoursFeature1(std::vector<Point> &dstContours,std::vector<Point> &_contour2, int mIndex, double parameter)
{
    //模板循环
    int m=parameter;
    int amount =30;
    int objSize=dstContours.size();

    bool flag[5];
    flag[0] = m&1==1;//距离
    flag[1] = (m>>1)&1==1;//角度
    flag[2] =(m>>2)&1==1;//面积特征
    flag[3] = (m>>3)&1==1;//缩放固定
    if((m>>4)&1)//次数
        amount=20;

    float num=objSize*1.0/amount;
    if(flag[3])
        num=s_model[mIndex].size()*1.0/amount;
    int index1,index2,index3,index4,index5,index6,n1,n2,n3,n4,j2Index,areaScore;
    int d1,d2,d3,d4,cur_d1,cur_d2;
    double ratio1,ratio2,cur_d3,cur_d4;
    int maxResult1=0,maxResult2=0;
    int step=num;
    if(step<2) step=2;
    if(step>4) step=4;
    ratio1=contourArea(s_model[mIndex])*1.0/arcLength(s_model[mIndex],true);
    ratio2=contourArea(dstContours)*1.0/arcLength(dstContours,true);
    bool isAbs1=0,isAbs2=1;
    int base=90;
    areaScore=amount-abs(ratio1-ratio2)/0.5;
    if(areaScore<0)
        areaScore=0;


    RotatedRect     End_Rage2D1;
    CvPoint2D32f rectpoint[4];
    End_Rage2D1 = minAreaRect(Mat(dstContours));
    cvBoxPoints(End_Rage2D1,rectpoint);
    Point dstCenter=End_Rage2D1.center;

    for(float i=0;i<objSize;i+=step*4)
    {

        for(int j2=0;j2<amount;j2++)
        {
            n1=n2=n3=n4=0;


            for(int j=0;j<amount;j++)
            {
                j2Index=j2+j;

                if(j2Index>=amount)
                    j2Index%=amount;

                index1=i+j*num-num;
                index2=i+j*num;
                index3=i+j*num+num;
                if(index1<0)
                    index1+=objSize;
                if(index1>=objSize)
                    index1%=objSize;
                if(index2<0)
                    index2+=objSize;
                if(index2>=objSize)
                    index2%=objSize;
                if(index3<0)
                    index3+=objSize;
                if(index3>=objSize)
                    index3%=objSize;

                index4=objSize-index1;
                index5=objSize-index2;
                index6=objSize-index3;

                if(index4<0)
                    index4+=objSize;
                if(index4>=objSize)
                    index4%=objSize;
                if(index5<0)
                    index5+=objSize;
                if(index5>=objSize)
                    index5%=objSize;
                if(index6<0)
                    index6+=objSize;
                if(index6>=objSize)
                    index6%=objSize;

                if(flag[0])
                {
                    cur_d3=common::getLineLen1(dstCenter,dstContours[index2])*1.0/objSize;
                    cur_d4=common::getLineLen1(dstCenter,dstContours[index5])*1.0/objSize;

                    if(abs(tempDisArr[mIndex][j2Index]-cur_d3)<0.05)
                    {
                        ++n3;
                    }

                    if(abs(tempDisArr[mIndex][j2Index]-cur_d4)<0.05)
                    {
                        ++n4;
                    }
                }


                if(flag[1])
                {



                    d1=common::calcAngle1(dstContours[index1],dstContours[index2]);
                    d2=common::calcAngle1(dstContours[index2],dstContours[index3]);
                    cur_d1=common::angleDif1(d1,d2,base,isAbs1);
                    d3=common::calcAngle1(dstContours[index4],dstContours[index5]);
                    d4=common::calcAngle1(dstContours[index5],dstContours[index6]);

                    cur_d2=common::angleDif1(d3,d4,base,isAbs1);
                    if(common::angleDif1(cur_d1,tempArr[mIndex][j2Index],base,isAbs2)<10)
                    {
                        ++n1;
                    }
                    if(common::angleDif1(cur_d2,tempArr[mIndex][j2Index],base,isAbs2)<10)
                    {
                        ++n2;
                    }
                }



//                if(i==40)
//                {

//                    if(j==0)
//                    {
//                        //qDebug()<<"first"<<common::getLineLen(cen,dstContours[index2])<<"p1 p2"<<cen.x<<cen.y<<dstContours[index2].x<<dstContours[index2].y;
//                        circle( g_dstImage2, dstContours[index2],3, Scalar(255,0,0), 1, CV_AA);
//                    }
//                    else
//                        circle( g_dstImage2, dstContours[index2],3, Scalar(0,0,255), 1, CV_AA);
//                }


            }
            if(flag[0])
            {
                if(n3>maxResult1)
                    maxResult1=n3;
                if(n4>maxResult1)
                    maxResult1=n4;

            }

            if(flag[1])
            {
                if(n1>maxResult2)
                    maxResult2=n1;
                if(n2>maxResult2)
                    maxResult2=n2;

            }

        }



    }

      int num1=0,allResult=0;
      for(int i=0;i<3;i++)
      {
          if(flag[i])
              num1+=amount;
      }


      if(flag[0]) allResult+=maxResult1;
      if(flag[1]) allResult+=maxResult2;
      if(flag[2]) allResult+=areaScore;


      return allResult*100./num1;

}


double project3::contoursFeature2(std::vector<Point> &dstContours,std::vector<Point> &_contour2, int mIndex, double parameter)
{
    //模板循环
    int m=parameter;
    int amount =30;
    int objSize=dstContours.size();

    bool flag[5]={0};
    flag[0] = (m&1)==1;//距离
    flag[1] = (m>>1)&1==1;//角度
    flag[2] =(m>>2)&1==1;//角点和面积特征
    flag[3] = (m>>3)&1==1;//缩放固定
    if((m>>4)&1)//次数
        amount=20;


    float num=objSize*1.0/amount;
    if(flag[3])
        num=s_model[mIndex].size()*1.0/amount;

    int index1,index2,index3,index4,index5,index6,n1,n2,n3,n4,n5,j2Index,areaScore;
    int d1,d2,d3,d4,cur_d1,cur_d2,dif_2;
    double ratio1,ratio2,cur_d3;
    float objRadius_1;
    int maxResult1=0,maxResult2=0,maxResult3=0;
    int step=num;
    if(step<2) step=2;
    if(step>4) step=4;
    bool isAbs1=0,isAbs2=1;
    int base=90;


    RotatedRect     End_Rage2D1;
    CvPoint2D32f rectpoint[4];
    End_Rage2D1 = minAreaRect(Mat(dstContours));
    cvBoxPoints(End_Rage2D1,rectpoint);
    Point dstCenter=End_Rage2D1.center;
    Point2f objCen;

    minEnclosingCircle(dstContours , objCen, objRadius_1 );
    int objRadius=objRadius_1;
    ratio1=contourArea(s_model[mIndex])*1.0/(s_model_radius[mIndex]*s_model_radius[mIndex]*3.14);
    ratio2=contourArea(dstContours)*1.0/(objRadius*objRadius*3.14);

    //qDebug()<<"objSize 1111"<<objSize<<s_model[mIndex].size()<<"Mraius"<<s_model_radius[mIndex]<<"dstradius"<<objRadius;

    areaScore=amount-abs(ratio1-ratio2)*amount/0.5;
    if(areaScore<0)
        areaScore=0;
    for(float i=0;i<objSize;i+=step*4)
        {

            for(int j2=0;j2<amount;j2++)
            {
                n1=n2=n3=n4=n5=0;


                for(int j=0;j<amount;j++)
                {
                    j2Index=j2+j;

                    if(j2Index>=amount)
                        j2Index%=amount;

                    index1=i+j*num-num;
                    index2=i+j*num;
                    index3=i+j*num+num;
                    if(index1<0)
                        index1+=objSize;
                    if(index1>=objSize)
                        index1%=objSize;
                    if(index2<0)
                        index2+=objSize;
                    if(index2>=objSize)
                        index2%=objSize;
                    if(index3<0)
                        index3+=objSize;
                    if(index3>=objSize)
                        index3%=objSize;

//                    index4=objSize-index1;
//                    index5=objSize-index2;
//                    index6=objSize-index3;

//                    if(index4<0)
//                        index4+=objSize;
//                    if(index4>=objSize)
//                        index4%=objSize;
//                    if(index5<0)
//                        index5+=objSize;
//                    if(index5>=objSize)
//                        index5%=objSize;
//                    if(index6<0)
//                        index6+=objSize;
//                    if(index6>=objSize)
//                        index6%=objSize;

                if(flag[0])
                {
                    cur_d3=common::getLineLen1(dstCenter,dstContours[index2])*1.0/objRadius;

                    if(abs(tempDisArr2[mIndex][j2Index]-cur_d3)<0.2)
                    {
                        ++n3;
                    }
                }


                d1=common::calcAngle1(dstContours[index1],dstContours[index2]);
                d2=common::calcAngle1(dstContours[index2],dstContours[index3]);
                cur_d1=common::angleDif1(d1,d2,base,isAbs1);
//                d3=common::calcAngle1(dstContours[index4],dstContours[index5]);
//                d4=common::calcAngle1(dstContours[index5],dstContours[index6]);

//                cur_d2=common::angleDif1(d3,d4,base,isAbs1);
                if(flag[1])
                {


                    if(common::angleDif1(cur_d1,tempArr[mIndex][j2Index],base,isAbs2)<10)
                    {
                        ++n1;
                    }
//                    if(common::angleDif1(cur_d2,tempArr[mIndex][j2Index],base,isAbs2)<10)
//                    {
//                        ++n2;
//                    }
                }


                if(flag[2])
                {
                    if(abs(cur_d1)>30)
                    {
                        ++n5;
                    }
                    if(abs(tempArr[mIndex][j2Index])>30)
                    {
                        ++n4;
                    }
                }


            }
            if(flag[0])
            {
                if(n3>maxResult1)
                    maxResult1=n3;
            }

            if(flag[1])
            {
                if(n1>maxResult2)
                    maxResult2=n1;
//                if(n2>maxResult2)
//                    maxResult2=n2;
            }
            if(flag[2])
            {
                dif_2=abs(n5-n4)*2;

                if(dif_2>amount)
                    dif_2=amount;
                if(amount-dif_2>maxResult3)
                    maxResult3=amount-dif_2;


            }

        }

    }

      int num1=0,allResult=0;
      for(int i=0;i<3;i++)
      {
          if(flag[i])
              num1+=amount;
      }


      if(flag[0]) allResult+=maxResult1;
      if(flag[1]) allResult+=maxResult2; 
      if(flag[2])
      {
          //  allResult+=maxResult3;

          allResult+=areaScore<maxResult3?areaScore:maxResult3;
          //qDebug()<<"maxResult3"<<maxResult3;
      }
     // qDebug()<<"maxResult2"<<allResult<<num1<<"maxResult1"<<maxResult1<<"maxResult3"<<(areaScore<maxResult3?areaScore:maxResult3);
      return allResult*100./num1;

}


double calcArr(std::vector<int> &src,int num)
{
    return 0;
}

int project3::matchShapes2(std::vector<Point> &contours, int m)
{
    int bestNum=0,out2,bestIndex;
    for(int j=0;j<s_model.size();j++)
    {
        out2=contoursFeature2(contours,contours,j,m);
        if(out2>bestNum)
        {
            bestIndex=j;
            bestNum=out2;
        }
    }
    //qDebug()<<"pos:"<<s_model_cen[bestIndex].x<<s_model_cen[bestIndex].y;
    return bestNum;
}
Point moveData[100],moveData2[100];//x起始  y结束
void project3::cmpPicPixel(Mat &src1, Mat &src2,Mat &src3, Rect roi, int val)
{

    Mat m(src1.rows,src1.cols,CV_8U,Scalar(0,0,0));
    Mat m2=m.clone();



    for(int i=roi.x;i<roi.width;i++)
    {
        for(int j=roi.y;j<roi.height;j++)
        {
            if(abs(src1.at<uchar>(j,i)-src2.at<uchar>(j,i))>val)
                m.at<uchar>(j,i)=255;
            if(abs(src1.at<uchar>(j,i)-src3.at<uchar>(j,i))>val)
                m2.at<uchar>(j,i)=255;

        }
    }
    int k=0,k2=0;
    for(int i=roi.y;i<roi.height;i+=3)
    {
        for(int j=roi.x+3;j<roi.width;j++)
        {
            // qDebug()<<"aaaa"<<src1.at<uchar>(i,j);
            if(m.at<uchar>(i,j)==255&&m.at<uchar>(i,j-1)==255&&m.at<uchar>(i,j-2)==255)
            {
                moveData[k].x=j-3;
                for(int j2=j;j2<roi.width;j2++)
                {
                    if(m.at<uchar>(i,j2)==0)
                    {
                        //qDebug()<<j2;
                        j=roi.width-1;
                        moveData[k].y=j2;
                        ++k;
                        if(k==100)
                            k=99;

                        break;
                    }
                }
            }

        }
    }


    for(int i=roi.y;i<roi.height;i+=3)
    {
        for(int j=roi.x+3;j<roi.width;j++)
        {
            if(m2.at<uchar>(i,j)==255&&m2.at<uchar>(i,j-1)==255&&m2.at<uchar>(i,j-2)==255)
            {
                moveData2[k2].x=j-3;
                for(int j2=j;j2<roi.width;j2++)
                {
                    if(m2.at<uchar>(i,j2)==0)
                    {
                        j=roi.width-1;
                        moveData2[k2].y=j2;
                        ++k2;
                        if(k2==100)
                            k2=99;
                        break;
                    }
                }
            }

        }
    }

    int count=k<k2?k:k2;
    int num=0,num2=0;
    for(int i=0;i<count;i++)
    {
        if(abs(moveData[i].x-moveData2[i].x)<3)
        {
            ++num;
            if(abs(moveData[i].y-moveData2[i].y)>1)
                ++num2;
        }
        qDebug()<<"moveData[i].x-moveData2[i].x"<<moveData[i].x-moveData2[i].x;
    }

    qDebug()<<"k"<<k<<"k2"<<k2<<"num"<<num<<num2;
    imshow("m1",m);
    imshow("m2",m2);


}




void project3::similarity()
{


//    if(s_model.size()==0)
//        return;

    Mat img1;
    threshold(g_dstImage1,img1,200,255,THRESH_BINARY);

    std::vector<std::vector<Point>> contours;


    findContours(img1,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


     for (unsigned int k=0 ; k<contours.size(); k++)
     {


         QTime t;
         t.start();
         qDebug()<<"obj size:"<<contours[k].size()<<"result:"<<matchShapes2(contours[k],31);
         //qDebug()<<"obj size:"<<contours[k].size()<<"result:"<<matchShapes(contours[k],contours[k],1,31);//matchShapes2(contours[k],31);
         qDebug("Time elapsed: %d ms", t.elapsed());

     }



}

void project3::splitRGB(Mat &rgb)
{
    int b=0;//162;
    int g=115;//188;
    int r=208;//154;
//qDebug()<<"aaa"<<rgb.at<Vec3b>(23,74)[0]<<rgb.at<Vec3b>(23,74)[1]<<rgb.at<Vec3b>(23,74)[2];
Mat mask(rgb.rows,rgb.cols,CV_8UC1,Scalar(0));
    for(int i=0;i<rgb.rows;i++)
    {
        for(int j=0;j<rgb.cols;j++)
        {

            if(abs(rgb.at<Vec3b>(i,j)[0]-r)+ abs(rgb.at<Vec3b>(i,j)[1]-g)+ abs(rgb.at<Vec3b>(i,j)[2]-b)<50)
            {
                rgb.at<Vec3b>(i,j)[0]=0;
                rgb.at<Vec3b>(i,j)[1]=0;
                rgb.at<Vec3b>(i,j)[2]=0;
                mask.at<uchar>(i,j)=255;
            }
        }
    }
    //imshow("rgbSplit",rgb);
    imshow("rgbMask",mask);

}

extern Point QC_Arr[];
extern bool calcFont_QC(int num,Point arr[]);
void project3::zt_3rdLineQC(Mat &src,Rect roi,params& param,bool blackHat)
{

    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测NI最小值
    unsigned int objMaxSize=param.var1[3];//检测NI最大值
    int distance=param.var1[4];//字体到中心点距离
    int thresh=param.var1[5];//二值
    double angleOffset=param.var2[0];//角度偏移

    param.var2[19]=360;//工件角度
    int px=0;//字体坐标
    int py=0;
    int matchVal,j1;
    int fontGrayVal,fontGrayDif;
    int angle1,angle2,x1,y1,len1,len2,distance1,min1=255,max1=0;
    bool loop=true;
    double area;

    Mat m5;
    m5=src(roi).clone();

    medianBlur(m5, m5,3);
    Mat m6=m5.clone();


    topBlack(m5,false);

    Mat m2;




    Mat m1;
    int a1,a2,a3,b1,c1,v1,v2,smoothNum,a4;
    int errorFlag=0,matchCount=0;


    std::vector<std::vector<Point>> contours;
    for(int i=thresh;i>3;)
      //for(int i=10;i>8;)
    {

        errorFlag=0;
        if(i>70)
        {

            i-=20;
        }
        else if(i>40)
        {
            i-=10;
        }
        else if(i>25)
        {
            i-=5;
        }
        else if(i>15)
        {
            i-=2;
        }
        else
            i--;


        j1=0;
        a1=0;
        a2=0;
        a3=0;
        b1=0;
        c1=0;
        max1=0;
        min1=255;
        //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
        threshold(m5,m1,i,255,THRESH_BINARY);
        threshold(m5,m2,i,255,THRESH_BINARY);

        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        //for ( ; itContours!=contours.end(); ++itContours)
        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            if(a1>=99||c1>499)
                break;

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(abs(distance-distance1)>35)
                continue;


            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));

            //字体在中心点相连角度
            if(abs(angle1)>25&&abs(angle1)<155)
                continue;




            if(contours[k].size()>objMinSize-15)
                ++b1;


            if(abs(angle1)<160&&abs(angle1)>20)
            {
                if(contours[k].size()<55)
                    continue;
            }

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*4)
                continue;

            if(m2.at<uchar>(contours[k][0].y-1,contours[k][0].x)==255)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                angle2=common::calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                angle2=common::calcAngle(rectpoint[1],rectpoint[2]);
            }

            if(abs(angle2)>40&&contours[k].size()<70)
                continue;

            if(abs(angle2)>60)
                continue;


//            if(abs(angle2)>30&&i<=10)
//                continue;
            if(len1<len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;

            }


            if(len1>44||len1<19)
            {
                errorFlag=3;
                continue;
            }

            matchVal=matchShapes(contours[k],contours[k],1,31);

            ++matchCount;
            param.var1[29]=matchCount;
            if(matchCount>29)
                return;
            if(matchVal>80)
            {
                QC_Arr[j1++]=Point(x1,y1);
                if(DEBUG)
                {
                qDebug()<<"x1 y1"<<x1<<y1<<"angle1"<<angle1<<"angle2"<<angle2<<"obj size:"<<contours[k].size()<<"result:"<<matchVal<<"i"<<i;
                circle( m6, Point(x1,y1), 3, Scalar(255,255,255), 1, CV_AA);
                }

            }

            if(matchVal>90&&contours[k].size()>70)
            {
                qDebug()<<"size"<<contours[k].size()<<"matchVal"<<matchVal;
                circle( m6, Point(x1,y1), 10, Scalar(255,255,255), 1, CV_AA);
                goto END;
            }
            if(j1<2)
                continue;


            if(calcFont_QC(j1,QC_Arr))
            {
END:
                param.var2[19]=angle1;
                if(DEBUG)
                {
                common::drawRect(m6,Point(cenX,cenY),distance,angle1,90,Scalar(255,255,255));


                imshow("m6",m6);
                imshow("m2",m2);
                }
                return;
            }




        }
    }
}


void project3::zt_not3rdLine(Mat &src, Rect roi, params& param)
{

    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测NI最小值
    unsigned int objMaxSize=param.var1[3];//检测NI最大值
    int distance=param.var1[4];//字体到中心点距离
    int thresh=param.var1[5];//二值


    param.var2[19]=360;//工件角度

    param.var2[10]=0;
    int angle1,angle2,x1,y1,len1,len2,distance1,j1;

    Mat m5;
    m5=src(roi).clone();
    double fontAngleArr[3];
    Mat m1;



    std::vector<std::vector<Point>> contours;
    for(int i=thresh;i>29;i-=10)
    {


        j1=0;

        //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
        threshold(m5,m1,i,255,THRESH_BINARY);


        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];

        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(abs(distance-distance1)>35)
                continue;

            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));

            //字体在中心点相连角度
            if(abs(angle1)>35&&abs(angle1)<145)
                continue;
//circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),5,Scalar(255,255,255));
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*4)
                continue;

            if(common::contourType(m5,contours[k],i,false))
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                angle2=common::calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                angle2=common::calcAngle(rectpoint[1],rectpoint[2]);
            }


            if(abs(angle2)>30)
                continue;



            if(len1<len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;

            }

            if(len1>44||len1<30)
            {

                continue;
            }

            if(len1*1.0/len2<5)
                continue;


            if(contourArea(contours[k])/len1/len2<0.5)
                continue;

            fontAngleArr[j1];
            ++j1;

            if(j1>1)
            {
            qDebug()<<"found"<<contours[k].size();
            double d1=abs(fontAngleArr[0]-fontAngleArr[1]);



            if(d1<8)
                 param.var2[10]=82000;
            return;
            }




        }
    }
}



void project3::zt_JLine(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//外接矩形len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//外接矩形角度

    double lenRatio=param.var2[0];//长度
    double areaRatio=param.var2[1];//面积
    double angleOffset=param.var2[2];//角度偏移
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1;

    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

    param.var2[19]=360;
    bool loop=true;

    int error=0;


    for(int i=thresh;i<thresh2;i+=10)
    //for(int i=thresh2;i>thresh;i-=10)
    {
        int a1=0;
        arr.clear();
        error=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        //imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;
            common::mostVal(error,1);
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(distance1<minLen||distance1>maxLen)
                continue;

            common::mostVal(error,2);
            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=common::calcAngle(rectpoint[0],rectpoint[1]);

                if(abs(r1Ang)<rAngle1)
                    continue;
            }
            else
            {
                r1Ang=common::calcAngle(rectpoint[1],rectpoint[2]);

                if(abs(r1Ang)<rAngle1)
                    continue;
            }

            common::mostVal(error,3);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;

            common::mostVal(error,4);


//            if(common::contourType(m5,contours[k],i,false))
//                continue;

            common::mostVal(error,5);
            area=contourArea(contours[k]);

            if(area/(len1*len2)<areaRatio)
                continue;
            common::mostVal(error,6);

            if(len2*1.0/len1<lenRatio)
                continue;

            common::mostVal(error,7);


            arr.push_back(Point(x1,y1));

            if(arr.size()<1)
                continue;

            common::mostVal(error,8);

            qDebug()<<"arr"<<arr.size()<<"size"<<contours[k].size();
            if(common::nearbyPos(arr,7,1)==false)
                continue;

            common::mostVal(error,9);

            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));


            param.var2[19]=common::anlgeOffset(angle1,angleOffset);;
            param.var1[28]=x1;
            param.var1[29]=y1;
            if(DEBUG)
                circle(rgb,Point(x1+roi.x,y1+roi.y),5,Scalar(255,0,0),2);


            loop=false;
            break;

        }

        qDebug()<<"error"<<error<<"i:"<<i;
    }

}

int calc_JLineQC(vector<Point> &contours,int x1,int y1,int maxDis,int dif,Rect &roi)
{
    Point cen(x1,y1);
    for(int i=0;i<contours.size()-15;i++)
    {
        if(contours[i].x>x1&&contours[i].y<y1)
        {
//            if(contours[i].x-contours[i+15].x>7)
//                return 0;

            if(common::getLineLen(cen,contours[i])-maxDis>dif)
            {
                circle(g_dstImage2,Point(contours[i].x+roi.x,contours[i].y+roi.y),5,Scalar(200,100,100),3);
                return 0;
            }
        }
        if(contours[i].x<x1&&contours[i].y>y1)
        {
            if(common::getLineLen(cen,contours[i])-maxDis>dif)
                return 180;
//            if(contours[i].x-contours[i+15].x<-7)
//                return 180;
        }

    }
    return 360;
}

void project3::zt_JLineQC(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize=500;//检测字体最小值
    unsigned int objMaxSize=1600;//检测字体最大值
    int maxToCen=30;//中心点偏离
    int thresh=73;//二值
    int thresh2=254;//二值
    int rLen1=220;//外接矩形len
    int rLen2=240;
    int rLenDif=50;//矩形差


    double angleOffset=param.var2[2];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src.clone();
    Mat m1;


    param.var2[19]=360;
    bool loop=true;


    int error=0;


    for(int i=thresh;i<thresh2;i+=10)
    //for(int i=thresh2;i>thresh;i-=10)
    {
        int a1=0;

        error=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("rrr",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;
            common::mostVal(error,1);
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            common::mostVal(error,2);
            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);



            common::mostVal(error,3);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;

            common::mostVal(error,4);



            if(abs(len1-len2)>rLenDif)
                continue;

            common::mostVal(error,5);


            if(common::getLineLen(Point(x1,y1),Point(cenX,cenY))>maxToCen)
                continue;


            common::mostVal(error,6);
            if(common::contourType(m5,contours[k],i,true))
            {
                continue;
            }



            common::mostVal(error,7);

            angle1=calc_JLineQC(contours[k],x1,y1,len2/2,10,roi);
            if(angle1==360)
                continue;

            common::mostVal(error,8);


            param.var2[19]=common::anlgeOffset(angle1,angleOffset);

            if(DEBUG)
                circle(rgb,Point(x1+roi.x,y1+roi.y),5,Scalar(255,0,0),2);


            loop=false;
            break;

        }

        qDebug()<<"error"<<error<<"i:"<<i;
    }

}

void project3::zt_JLineMatch(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//外接矩形len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//外接矩形角度

    double lenRatio=param.var2[0];//长度
    double areaRatio=param.var2[1];//面积
    double angleOffset=param.var2[2];//角度偏移
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1;

    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

    param.var2[19]=360;
    bool loop=true;


    int error=0;
    int matchVal;

    for(int i=thresh;i<thresh2;i+=15)
    //for(int i=thresh2;i>thresh;i-=10)
    {
        int a1=0;
        arr.clear();
        error=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        //imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*2)
                continue;
            common::mostVal(error,1);
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),7,Scalar(255,0,0),3);
            if(distance1<minLen||distance1>maxLen)
                continue;

            common::mostVal(error,2);
            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=common::calcAngle(rectpoint[0],rectpoint[1]);

                if(abs(r1Ang)<rAngle1)
                    continue;
            }
            else
            {
                r1Ang=common::calcAngle(rectpoint[1],rectpoint[2]);

                if(abs(r1Ang)<rAngle1)
                    continue;
            }

            common::mostVal(error,3);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;

            common::mostVal(error,4);


            if(common::contourType(m5,contours[k],i,true))
                continue;

            common::mostVal(error,5);



            if(len2*1.0/len1<lenRatio)
                continue;

            common::mostVal(error,7);


            matchVal=matchShapes(contours[k],contours[k],1,31);
            if(matchVal<80)
                continue;


            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));


            param.var2[19]=common::anlgeOffset(angle1,angleOffset);;
            param.var1[28]=x1;
            param.var1[29]=y1;
            if(DEBUG)
                circle(rgb,Point(x1+roi.x,y1+roi.y),5,Scalar(255,0,0),2);



            loop=false;
            break;


        }

        qDebug()<<"error"<<error<<"i:"<<i<<"matchVal"<<matchVal<<distance1;
    }

}

void project3::commonMatch(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//外接矩形len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//外接矩形角度默认360   <45保留 or >45保留
    int angleRange=param.var1[11];//角度范围20  标识0附近保留  标识90附近保留
    int angleRangeFlag=param.var1[12];//范围标识0 90
    int step=param.var1[13];//i+=step;
    bool isBlack=param.var1[14]==1?true:false;//1黑色  0白色
    int noisy=param.var1[15];//噪点
    int bestVal=param.var1[16];//相似度
    int fg_1=param.var1[17];
    int fg_method=(param.var1[18]>200&&param.var1[18]<210)?param.var1[18]:2;

    double lenRatio=param.var2[0];//长度默认-1    <100 <过滤  or >100  >过滤  长/宽
    double areaRatio=param.var2[1];//面积默认-1   <100 <过滤  or >100  >过滤  area/rectArea
    double angleOffset=param.var2[18];//角度偏移
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1,a3;



    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

    param.var2[19]=360;
    bool loop=true;


    int error=0;
    int matchVal;
    int i=thresh;
    int sign=1;
    if(thresh>thresh2)
        sign=-1;

    while(1)
    //for(int i=thresh2;i>thresh;i-=10)
    {
        if(!loop)
            break;
        i+=sign*step;
        if(DEBUG)
            qDebug()<<"i:"<<i<<"noisy"<<a3;
        if(abs(i-thresh2)<step)
            break;
        arr.clear();
        error=0;


        a3=0;

        threshold(m5,m1,i,255,THRESH_BINARY);
        //imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<20)
                continue;
            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                {}
                else
                {
                    ++a3;
                }

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                {}
                else
                {
                    ++a3;
                }

            }

            if(a3>noisy)
            {
                loop=false;
                break;
            }

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;

            common::mostVal(error,1);
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));


            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                    continue;

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                    continue;

            }

            common::mostVal(error,2);
            //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),7,Scalar(255,0,0),3);
            //qDebug()<<"distance1"<<distance1<<minLen<<maxLen<<"x y"<<x1<<y1<<m1.cols<<m1.rows;

            if(distance1<minLen||distance1>maxLen)
                continue;

            common::mostVal(error,3);

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=common::calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                r1Ang=common::calcAngle(rectpoint[1],rectpoint[2]);
            }

            if(rAngle1>0&&rAngle1<90)
            {
                if(rAngle1<45)
                {
                    if(abs(r1Ang)>rAngle1)
                        continue;
                }
                else
                {
                    if(abs(r1Ang)<rAngle1)
                        continue;
                }
            }
            common::mostVal(error,4);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

           // qDebug()<<"len1"<<len1<<len2<<rLen1<<rLen2;
        //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),7,Scalar(255,255,255),3);
            if(len1<rLen1||len2>rLen2)
                continue;

            common::mostVal(error,5);


            if(common::contourType(m5,contours[k],i,isBlack))
                continue;

            common::mostVal(error,6);




            if(lenRatio<100&&len2*1.0/len1<lenRatio)
                continue;

            common::mostVal(error,7);
            if(lenRatio>100)
            {
                if(len2*1.0/len1>lenRatio-100)
                    continue;
            }

            common::mostVal(error,8);
            area=contourArea(contours[k]);

            if(areaRatio<100&&area/(len1*len2)<areaRatio)
                continue;

            common::mostVal(error,9);
            if(areaRatio>100)
            {

                if(area/(len1*len2)>areaRatio-100)
                    continue;
            }

            common::mostVal(error,10);


            matchVal=matchShapes(contours[k],contours[k],fg_method,fg_1);

            if(DEBUG)
                qDebug()<<"size:"<<contours[k].size()<<"matchVal"<<matchVal<<"bestVal"<<bestVal<<"fg_method"<<fg_method;
            if(matchVal<bestVal)
                continue;







            param.var2[19]=common::anlgeOffset(angle1,angleOffset);;
            param.var1[28]=x1;
            param.var1[29]=y1;
            param.var1[27]=matchVal;
            if(DEBUG)
            {


                qDebug()<<"hello"<<matchVal<<bestVal;
                //threshold(m5,m1,i,255,THRESH_BINARY);
                //imshow("thresh",m1);

            }



            loop=false;
            break;


        }
        if(DEBUG)
            qDebug()<<"error"<<error<<"i:"<<i;
    }


}


void project3::countMatch(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//外接矩形len
    int rLen2=param.var1[9];
    int rLen3=param.var1[10];//外接矩形len
    int rLen4=param.var1[11];
    int rAngle1=param.var1[12];//外接矩形角度默认360   <45保留 or >45保留
    int angleRange=param.var1[13];//角度范围20  标识0附近保留  标识90附近保留
    int angleRangeFlag=param.var1[14];//范围标识0 90
    int step=param.var1[15];//i+=step;
    bool isBlack=param.var1[16]==1?true:false;//1黑色  0白色
    int noisy=param.var1[17];//噪点

    double lenRatio=param.var2[0];//长度默认-1    <100 <过滤  or >100  >过滤  长/宽
    double areaRatio=param.var2[1];//面积默认-1   <100 <过滤  or >100  >过滤  area/rectArea
    double angleOffset=param.var2[18];//角度偏移
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1,a1,a2,a3;



    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

    param.var2[19]=360;
    bool loop=true,found=false;


    int error=0;

    int i=thresh;
    int sign=1;
    if(thresh>thresh2)
        sign=-1;

    while(1)
    {
        if(!loop)
            break;
        i+=sign*step;
        if(DEBUG)
            qDebug()<<"i:"<<i<<"noisy"<<a3;
        if(abs(i-thresh2)<step)
            break;
        arr.clear();
        error=0;


        a1=a2=a3=0;

        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("m1",m1);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<20)
                continue;
            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                {}
                else
                {
                    ++a3;
                }

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                {}
                else
                {
                    ++a3;
                }

            }

            if(a3>noisy)
            {
                loop=false;
                break;
            }

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;

            common::mostVal(error,1);
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));


            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                    continue;

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                    continue;

            }

            common::mostVal(error,2);
            //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),7,Scalar(255,0,0),3);
            //qDebug()<<"distance1"<<distance1<<minLen<<maxLen<<"x y"<<x1<<y1<<m1.cols<<m1.rows;

            if(distance1<minLen||distance1>maxLen)
                continue;

            common::mostVal(error,3);

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=common::calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                r1Ang=common::calcAngle(rectpoint[1],rectpoint[2]);
            }

            if(rAngle1>0&&rAngle1<90)
            {
                if(rAngle1<45)
                {
                    if(abs(r1Ang)>rAngle1)
                        continue;
                }
                else
                {
                    if(abs(r1Ang)<rAngle1)
                        continue;
                }
            }
            common::mostVal(error,4);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

           // qDebug()<<"len1"<<len1<<len2<<rLen1<<rLen2;
        //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),7,Scalar(255,255,255),3);
            if(len1<rLen1||len1>rLen2)
                continue;

            if(len2<rLen3||len2>rLen4)
                continue;

            common::mostVal(error,5);


            if(common::contourType(m5,contours[k],i,isBlack))
                continue;

            common::mostVal(error,6);




            if(lenRatio<100&&len2*1.0/len1<lenRatio)
                continue;

            common::mostVal(error,7);
            if(lenRatio>100)
            {
                if(len2*1.0/len1>lenRatio-100)
                    continue;
            }

            common::mostVal(error,8);
            area=contourArea(contours[k]);

            if(areaRatio<100&&area/(len1*len2)<areaRatio)
                continue;

            common::mostVal(error,9);
            if(areaRatio>100)
            {

                if(area/(len1*len2)>areaRatio-100)
                    continue;
            }

            common::mostVal(error,10);





            if(!found)
            {
                found=true;
                break;
            }


            if(angleRangeFlag==90)
            {
                if(angle1<0)
                    a1++;
                else
                    a2++;

            }
            else
            {
                if(abs(angle1)<90)
                    a1++;
                else
                    a2++;
            }


            param.var2[19]=common::anlgeOffset(angle1,angleOffset);
            param.var1[28]=x1;
            param.var1[29]=y1;

            if(DEBUG)
            {

                circle(rgb,Point(x1+roi.x,y1+roi.y),5,Scalar(0,0,255),2);
                qDebug()<<"hello"<<param.var2[19];
                //threshold(m5,m1,i,255,THRESH_BINARY);
                //imshow("thresh",m1);

            }



            loop=false;


        }
        if(DEBUG)
            qDebug()<<"error"<<error<<"i:"<<i;
    }

    if(loop==false)
    {
        if(a1>0&&a2>0)
        {qDebug()<<"a1 "<<a1<<"a2 "<<a2;
            param.var2[19]=360;
        }

    }


}


Point getRobotPos(Point obj,double lineAngle,double angleDif,int sideLen,bool inRobotRight)
{
    double angle=lineAngle+angleDif;
    int offx=cos(angle)*3.1415/180*sideLen;
    int offy=sin(angle)*3.1415/180*sideLen;
    if(inRobotRight)
        return Point(obj.x-abs(offx),obj.y-offy);
    else
        return Point(obj.x+abs(offx),obj.y-abs(offy));
}

bool findCornerAndAngle(std::vector<Point> &contours,int leftDis,int step,int len,int disLimit,int maxLine,double &outAngle,Point &cornerPoint)
{
    double angle,angle2,angle3,a1,a2,a3,a4;
    bool f;
    //circle(g_dstImage2,contours[351+200],18,Scalar(0,255,0),3);


    for(int i=100;i<contours.size()-2*len;i+=step)
    {

        //if(common::getLineLen(contours[i],contours[i+len])<disLimit)
        a1=common::calcAngle(contours[i-len],contours[i]);
        a2=common::calcAngle(contours[i+len],contours[i]);

        if(common::angleDif(a1,a2,90,true)>disLimit)
        {



            if(i+maxLine+len*3<contours.size())
            {
                f=true;
                angle=common::calcAngle(contours[i+len*3],contours[i+maxLine+len*3]);
                //直线较短边
                if(common::angleDif(angle,common::calcAngle(contours[i-20],contours[i-40]),90,true)>8)
                    f=false;

                for(int j=i+len*3;j<i+len*3+maxLine;j+=20)
                {
                    angle2=common::calcAngle(contours[j],contours[j+20]);

                    //直线较长边
                    if(common::angleDif(angle,angle2,90,true)>5)
                    {
                        f=false;

                        break;
                    }

                }
                //长度加250确定角点
//                if(contours.size()<2000)
//                    qDebug()<<"size"<<contours.size()<<"222"<<"i:"<<i<<f;
                for(int k=i+200;k+len<contours.size()&&k<i+leftDis;k+=len)
                {
                    if(k==i+200)
                        f=false;
                    a3=common::calcAngle(contours[k],contours[k+len]);
                    a4=common::calcAngle(contours[k],contours[k-len]);
                    //qDebug()<<"size"<<contours.size()<<common::angleDif(a3,a4,90,true)<<"k"<<k;
                    //circle(g_dstImage2,contours[k],8,Scalar(255,255,0),3);

                    if(common::angleDif(a3,a4,90,true)>30)
                    {
                        f=true;

                        break;
                    }

                }


                //确定较长边直线


                for(int k1=i+40;k1<contours.size()&&k1<i+230-len;k1+=len)
                {

                    a3=common::calcAngle(contours[k1],contours[k1+len]);
                    a4=common::calcAngle(contours[k1],contours[k1-len]);
//                    if(k1>1100&&k1<1350&&common::angleDif(a3,a4,90,true)>20)
//                        circle(g_dstImage2,contours[k1],8,Scalar(255,255,0),3);

                    //qDebug()<<"aaa"<<contours.size()<<common::angleDif(a3,a4,90,true)<<"k1"<<k1;
                    if(common::angleDif(a3,a4,90,true)>20)
                    {
                        f=false;

                        //circle(g_dstImage2,contours[k1],10,Scalar(0,0,255),3);
                        break;
                    }
                }

                //确定较短边直线

                bool b1=false;
                for(int k2=i-len;k2-len>0&&k2>i-120&&k2+len<contours.size();k2-=len)
                {

                    a3=common::calcAngle(contours[k2],contours[k2+len]);
                    a4=common::calcAngle(contours[k2],contours[k2-len]);
                   // qDebug()<<"aaa"<<contours.size()<<common::angleDif(a3,a4,90,true);
                    if(common::angleDif(a3,a4,90,true)>20)
                    {
                        b1=true;
                        break;
                    }

                }

                if(b1==false)
                    f=false;

                //确定较短边角点
                if(i+30+len<contours.size()&&i-30-len>0)
                {
                    a3=common::calcAngle(contours[i+30],contours[i+30+len]);
                    a4=common::calcAngle(contours[i-30],contours[i-30-len]);
                    if(common::angleDif(a3,a4,90,true)>20)
                    {
                        f=false;

                    }
                }

                if(f)
                {

                    circle(g_dstImage2,contours[i],3,Scalar(0,0,255),2);
                    //circle(g_dstImage2,contours[i+270],8,Scalar(0,0,255),3);
                    outAngle=angle;
                    cornerPoint=contours[i];

                    return true;
                }
            }

            circle(g_dstImage2,contours[i],5,Scalar(255,0,0),3);
//            QString words=QString("%1").arg(i);
//            putText( g_dstImage2, words.toStdString(), contours[i],CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

            //if(contours.size()<2000)
            //qDebug()<<"contours[]"<<i<<contours[i].x<<contours[i].y;
            i+=len*2;

        }
    }
    return false;
}
void project3::sz_cloth2(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int objMinSize=param.var1[0];
    int objMaxSize=param.var1[1];
    int thresh=param.var1[2];//二值
    double setAngleDif=param.var2[0];//标定点到特征点与布角度差
    int featureToCen=param.var1[2];//布特征点到标定点长度




    param.var2[19]=0;
    param.var2[10]=360;
    param.var2[13]=360;
    Mat m1;
    int x1,y1;
    Point cornerPoint;
    double retAngle;
    threshold(src,m1,thresh,255,THRESH_BINARY);
    int count=0;
    std::vector<std::vector<Point>> contours;
    findContours(m1,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


    RotatedRect     End_Rage2D1;
    CvPoint2D32f rectpoint[4];
    for (unsigned int k=0 ; k<contours.size(); k++)
    {
        if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
            continue;
        End_Rage2D1 = minAreaRect(Mat(contours[k]));
        cvBoxPoints(End_Rage2D1,rectpoint);
        x1=End_Rage2D1.center.x;
        y1=End_Rage2D1.center.y;

        if(findCornerAndAngle(contours[k],330,1,8,28,200,retAngle,cornerPoint))
        {
            ++count;
            //Point obj(1859,1130);
            //Point obj(2019,959);
            //double lineAngle=-28.2463;//-9.53937;

            //Point ret=getRobotPos(cornerPoint,retAngle,setAngleDif,featureToCen,true);

            Point ret=cornerPoint;
            if(contours[k].size()<2000)
            {

                //小布
                param.var2[10]=retAngle;
                param.var2[11]=ret.x;
                param.var2[12]=ret.y;
            }
            else
            {

                //大布
                param.var2[13]=retAngle;
                param.var2[14]=ret.x;
                param.var2[15]=ret.y;
            }

           // circle(rgb,ret,10,Scalar(125,125,125),3);
            //qDebug()<<"retAngle"<<retAngle<<"size"<<contours[k].size();


            if(count>1)
            {
                param.var2[19]=1;
                return;
            }

        }

    }

}


