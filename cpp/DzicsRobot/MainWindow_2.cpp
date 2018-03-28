#include "MainWindow_all.h"
QString visionOutput[5];
params visionInput[5];
Rect visionRoi[5];
void drawRect(Mat &src,Point cen,int distance,int angleVal,int angleFlag,Scalar &color)
{

    if(angleVal==360)
        return;
    Rect r1;
    r1.width=100;
    r1.height=100;
    if(angleFlag==90)
    {
        if(abs(angleVal)<90)
        {
            r1.x=cen.x-50-distance;
            r1.y=cen.y-50;
            if(r1.x<0) r1.x=0;
            rectangle(src,r1,color,3);

        }
        else
        {
            r1.x=cen.x-50+distance;
            r1.y=cen.y-50;
            if(r1.x+r1.width>src.cols) r1.x=src.cols-r1.width;
            rectangle(src,r1,color,3);

        }
        qDebug()<<"r1.x"<<r1.x<<r1.y<<r1.width<<r1.height<<cen.x<<cen.y<<"dis"<<distance;
    }
    else if(angleFlag==0)
    {
        if(angleVal<0)
        {
            r1.x=cen.x-50;
            r1.y=cen.y-50-distance;
            if(r1.y<0) r1.y=0;
            rectangle(src,r1,color,3);

        }
        else
        {
            r1.x=cen.x-50;
            r1.y=cen.y-50+distance;
            if(r1.y+r1.height>src.rows) r1.y=src.rows-r1.height;
            rectangle(src,r1,color,3);
        }
    }

}
void MainWindow::visionAssemble(QStringList &tmpList,CvRect areaRect[],QStringList &areaRectId,int method,int camIndex)
{

        if(method == 0){  //工件检测


            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<8) inputList.append("");
            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  开始工件检测", QString());

            if(!DEBUG)
            {
            mylock[0].lock();
            cvCopy(IpImgCache[0],m_camImage);

            mylock[0].unlock();
            }
            else
            {
                m_camImage=cvLoadImage("d:/1.jpg",0);
            }
            //相机1重连
            static int dectedNum1=0;
            static bool connCam1=true;
            if(!DEBUG&&dectedNum1==fNum[0])
            {
                Sleep(1000);
                emit addLog("相机1异常正在重新连接...","");
                if(connCam1&&fNum[0]>2)
                {
                    connCam1=false;
                    m_ui->page_8->captureCamera[0]->resumeCamera();

                }
                return;
            }
            dectedNum1=fNum[0];
            if(!DEBUG&&fNum[0]==0)
            {
                emit addLog("相机正在启动...","");
                return;
            }
            connCam1=true;

            DETECTRESULT result =  zt_visulLocation(m_camImage,areaRect[0], areaRect[1],inputList[0].toInt(), inputList[1].toInt(),inputList[2].toInt(),inputList[3].toInt(),inputList[4].toDouble(),inputList[5].toDouble(),inputList[6].toDouble(),inputList[7].toDouble());

            if(result.result==-1)
            {
                result.angle=100;
                result.offsetX=100;
                result.offsetY=100;
                m_ui->page_8->workParam[0]=result.angle;
                m_ui->page_8->workParam[1]=result.offsetX;
                m_ui->page_8->workParam[2]=result.offsetY;

            }
            m_ui->page_8->workParam[0]=result.angle;
            m_ui->page_8->workParam[1]=result.offsetX;
            m_ui->page_8->workParam[2]=result.offsetY;
            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<3) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString str;
                    QByteArray dataArray;



                    str = "??" + outputVarList[1] + "|" + QString::number(result.offsetX*1000,'f',0);
                    str+=";"+outputVarList[0] + "|" + QString::number(result.angle*1000,'f',0);
                    str+=";"+outputVarList[2] + "|" + QString::number(result.offsetY*1000,'f',0);
                    dataArray = str.toLocal8Bit();
                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;
                    mysendService(dataArray.data(),dataArray.length(),1);

                    //保存检测图片
                    QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+QString("_%1_%2_%3").arg(result.angle).arg(result.offsetX).arg(result.offsetY);
                    savedDetectImg(m_camImage,"savedImage1",fileName);

                    if(result.result)
                        emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  视觉检测结果：" + QString("angle:%1,x:%2,y:%3").arg(result.angle).arg(result.offsetX).arg(result.offsetY), QString());
                    else
                        emit addLog("工件无法检测请更换","");
                }
            }


        }else if(method == 1){  //工件检测反转
            //emit addLog("模板参数个数"+QString::number(areaRectId.size()),"");

            static bool loadImg1=false;
            if(areaRectId.size()<2)
            {
                emit addLog("模板参数个数小于2","");
                return;
            }else{
                if(!loadImg1)
                {
                    loadImg1=true;

                    QDir dir2("temp/");
                    dir2.setFilter(QDir::Files);
                    QFileInfoList list2= dir2.entryInfoList();

                    for(int i=0;i<list2.size();i++)
                    {
                        QString path="temp/"+list2[i].fileName();
                        templateImage.append(cvLoadImage(path.toLocal8Bit().data(),0));
                    }



                }
            }

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<2) inputList.append("");
            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  开始反转检测" /* + tmpList[2]*/, QString());

            //
            if(!DEBUG)
            {
            mylock[1].lock();
            cvCopy(IpImgCache[1],m_camImage2);
            mylock[1].unlock();
            }
            else
            {
                m_camImage2=cvLoadImage("d:/1.jpg");
            }


            //相机2重连
            static int dectedNum2=0;
            static bool connCam2=true;
            if(!DEBUG&&dectedNum2==fNum[1])
            {
                Sleep(1000);
                emit addLog("相机2异常正在重新连接...","");
                if(connCam2&&fNum[1]>2)
                {
                    connCam2=false;
                    m_ui->page_8->captureCamera[1]->resumeCamera();

                }
                return;
            }
            dectedNum2=fNum[1];
            if(!DEBUG&&fNum[1]==0)
            {
                emit addLog("相机2正在启动...","");
                return;
            }
            connCam2=true;

            //保存检测图片
            QString fileName2=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            savedDetectImg(m_camImage2,"savedImage2",fileName2);


            //long int result = zt_matchTemplate(m_camImage2,templateImage[0],areaRect[1],inputList[1].toInt());
            long int result=-1;
            int bestMatch=1000000;


            for(int j=0;j<templateImage.size();j++)
            {
                result =zt_matchTemplate(m_camImage2,templateImage[j],areaRect[1],inputList[1].toInt());

                if(result<bestMatch)
                    bestMatch=result;

            }


            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString str;// = "??";// + outputVarList[0] + (result?"|1":"|0");
                    QByteArray dataArray;// = str.toLocal8Bit();

                    str = "??" + outputVarList[0] + "|" + QString::number(bestMatch);
                    dataArray = str.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;


                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QString("正反检测:%1").arg(bestMatch),"");
                    m_ui->page_8->isAjdust=true;
                }
            }
            // emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  视觉检测结果：" + QString("是否反转:%1").arg(result), QString());

        }else if(method == 2){  //雪龙圆孔检测

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<22) inputList.append("");


            //start....
            emit addLog("开始圆孔检测:","");
            //相机重连
            static int dectedNum1=0;
            if(dectedNum1==fNum[0])
            {
                emit addLog("相机1异常正在重新连接...","");
                return;
            }
            dectedNum1=fNum[0];


            //检测
            xy_size_1 xy_arr[50];
            int otherNum=0;
            params param;
            param.var1[0]=inputList[0].toInt();//小圆孔size
            param.var1[1]=inputList[1].toInt();//大圆孔size
            param.var1[2]=inputList[2].toInt();//最大圆size
            param.var1[3]=inputList[3].toInt();//最小内圆环半径
            param.var1[4]=inputList[4].toInt();//最大内圆环半径
            param.var1[5]=inputList[5].toInt();//实际圆孔间距和测量圆孔间距差值,越小越匹配度越低
            param.var1[6]=inputList[6].toInt();//圆孔离圆心距离(所有风扇小孔的平均值)
            param.var1[7]=inputList[7].toInt();//最大圆孔间距
            param.var1[8]=inputList[8].toInt();//小圆间距
            param.var1[9]=inputList[9].toInt();//只有6个小圆孔间距
            param.var1[10]=inputList[10].toInt();//机器人基准点X;
            param.var1[11]=inputList[11].toInt();//机器人基准点Y;
            param.var1[12]=inputList[12].toInt();//圆心X
            param.var1[13]=inputList[13].toInt();//圆心Y
            param.var1[14]=inputList[14].toInt();//毛刺灰度值
            param.var1[15]=inputList[15].toInt();//二值参数
            param.var1[16]=inputList[16].toInt();//圆孔离圆心距离与cen_distance1差值范围
            param.var2[0]=inputList[17].toDouble();//像素比
            param.var2[1]=inputList[18].toDouble();//圆外接矩形长宽比
            param.var2[2]=inputList[19].toDouble();//坐标角度偏差
            param.var2[3]=inputList[20].toDouble();//X偏移毫米
            param.var2[4]=inputList[21].toDouble();//Y偏移毫米

            //通过风扇模板设置检测半径
            int min_r= m_ui->page_8->getTemplateIndex();
            if(min_r>0)
            {
                param.var1[3]=min_r/2/param.var2[0]+25;
            }


            //end

            int num=0,num2=0;
            mylock[0].lock();
            cvCopy(IpImgCache[0],m_camImage);
            mylock[0].unlock();
            //m_camImage=cvLoadImage("C:/Users/Teemo/Desktop/20161117_133647.bmp",0);



            //保存检测图片
            QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            savedDetectImg(m_camImage,"savedImage",fileName);

            QTime t;
            t.start();

            Rect roi;
            roi.x= areaRect[0].x;
            roi.y= areaRect[0].y;
            roi.width= areaRect[0].width;
            roi.height= areaRect[0].height;
            //qDebug()<<roi.x<<roi.y<<roi.width<<roi.height;
            xl_circle(Mat(m_camImage),roi,num,num2,xy_arr,otherNum,param);



            QString str=QString("%1|").arg(num);
            emit addLog(QString("数量:%1  NG数量:%2  污点数量:%3 耗时:%4 ms").arg(num).arg(num2).arg(otherNum).arg(t.elapsed()),"");
            //x
            for(int i=0;i<num2;i++)
            {
                emit addLog(QString("x:%1 y:%2").arg(xy_arr[i].x).arg(xy_arr[i].y),"");//<<"x:"<<xy_arr[i].x<<"y:"<<xy_arr[i].y<<"size:"<<xy_arr[i].size;
                str+=QString("%1,").arg(QString::number(xy_arr[i].moveY*-1,'f',3));
            }

            //y
            if(num2>0)
                str.remove(str.length()-1,1);
            str+="|";
            for(int i=0;i<num2;i++)
            {
                str+=QString("%1,").arg(QString::number(xy_arr[i].moveX*-1,'f',3));
            }
            if(num2>0)
                str.remove(str.length()-1,1);
            //qDebug()<<"move:"<<str;



            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString retStr=QString("??");
                    QByteArray dataArray;
                    retStr += str;
                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR_QJK;
                    dataArray[1] = 1;
                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"send data"+retStr.remove(0,2),"");

                }
            }





        }else if(method == 3){  //风扇圆孤
            addLog("风扇圆孤开始检测","");
            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            QByteArray dataArray="visual_001";
            serviceSocket[0]->writeDatagram(dataArray,dataArray.length(),  QHostAddress("192.168.125.100"), 8888);
            outputName1= "??" + outputVarList[0] + "|";
        }else if(method == 4){  //风扇侧面
            addLog("风扇侧面开始检测","");
            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            QByteArray dataArray="visual_002";
            serviceSocket[0]->writeDatagram(dataArray,dataArray.length(),  QHostAddress("192.168.125.101"), 8888);
            outputName2= "??" + outputVarList[0] + "|";
        }else if(method == 5){//振挺B线


            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<10) inputList.append("");


            //start....
            emit addLog("开始工件检测1:","");
            //相机重连
            static int dectedNum1=0;
            if(dectedNum1==fNum[0]&&!DEBUG)
            {
                emit addLog("相机1异常正在重新连接...","");
                return;
            }
            dectedNum1=fNum[0];


            //检测

            params param;
            param.var1[0]=inputList[0].toInt();//圆心X
            param.var1[1]=inputList[1].toInt();//圆心Y
            param.var1[2]=inputList[2].toInt();//圆心与字距离
            param.var1[3]=inputList[3].toInt();//圆心与字距离
            param.var1[4]=inputList[4].toInt();//小半圆size
            param.var1[5]=inputList[5].toInt();//大半圆size
            param.var1[6]=inputList[6].toInt();//二值
            param.var1[7]=inputList[7].toInt();//外接矩形长小半圆
            param.var1[8]=inputList[8].toInt();//外接矩形长大半圆
            param.var2[0]=inputList[9].toDouble();//angleOffset

            if(!DEBUG)
            {
                mylock[0].lock();
                cvCopy(IpImgCache[0],m_camImage);
                mylock[0].unlock();
            }
            else
            {
                m_camImage=cvLoadImage("D:/1.png",0);
            }


            //保存检测图片
            QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            savedDetectImg(m_camImage,"savedImage",fileName);

            QTime t;
            t.start();
            //Rect roi(482,382,450,450);
            Rect roi;



            if(DEBUG)
            {
                roi.x=0;
                roi.y=0;
                roi.width=1280;
                roi.height=960;
            }
            else
            {
                roi.x= areaRect[0].x;
                roi.y= areaRect[0].y;
                roi.width= areaRect[0].width;
                roi.height= areaRect[0].height;
            }



            detectWork(Mat(m_camImage),roi,param,17);

            emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


            if(param.var2[19]!=360)
            {
                Mat m3=Mat(m_camImage);
                if(abs(param.var2[19])<90)
                    rectangle(m3,Point(param.var1[0]-param.var1[2]-50,param.var1[1]-100),Point(param.var1[0]-param.var1[2]+50,param.var1[1]+100),Scalar(255,255,255),2);
                else
                    rectangle(m3,Point(param.var1[0]+param.var1[2]-50,param.var1[1]-100),Point(param.var1[0]+param.var1[2]+50,param.var1[1]+100),Scalar(255,255,255),2);
                savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
            }
            else
            {
                emit addLog("工件无法检测请更换","");
            }



            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                    QByteArray dataArray;


                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");
                    g_camera->setLabelText(QString::number(param.var2[19]));
                    QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));
                }
            }




        }else if(method== 6){//振挺C线

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<7) inputList.append("");

            //start....
            emit addLog("开始工件检测2:","");
            //相机重连
            static int dectedNum1=0;
            if(dectedNum1==fNum[0])
            {
                emit addLog("相机1异常正在重新连接...","");
                if(!DEBUG) return;
            }
            dectedNum1=fNum[0];


            //检测

            params param;
            param.var1[0]=inputList[0].toInt();//中心点X
            param.var1[1]=inputList[1].toInt();//中心点Y
            param.var1[2]=inputList[2].toInt();//检测NI最小值
            param.var1[3]=inputList[3].toInt();//检测NI最大值
            param.var1[4]=inputList[4].toInt();//字体到中心点距离
            param.var1[5]=inputList[5].toInt();//二值
            param.var2[0]=inputList[6].toDouble();//角度偏移


            if(!DEBUG)
            {
            mylock[0].lock();
            cvCopy(IpImgCache[0],m_camImage);
            mylock[0].unlock();
            }
            else
            m_camImage=cvLoadImage("d:/1.png",0);



            //保存检测图片
            QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            savedDetectImg(m_camImage,"savedImage",fileName);

            QTime t;
            t.start();
            //Rect roi(313,332,1001-313,916-332);
            Rect roi;
            roi.x= areaRect[0].x;
            roi.y= areaRect[0].y;
            roi.width= areaRect[0].width;
            roi.height= areaRect[0].height;

            if(DEBUG)
            {
                roi.x=0;
                roi.y=0;
                roi.width=1280;
                roi.height=960;

            }


            param.var1[28]=100000;//得到首次检测到NI坐标
            param.var1[29]=100000;
            param.var2[19]=360;//角度

            detectWork(Mat(m_camImage),roi,param,1);

            emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


            if(param.var2[19]!=360)
            {
                Mat m3=Mat(m_camImage);
                if(abs(param.var2[19])<90)
                    rectangle(m3,Point(395,517),Point(515,713),Scalar(0,0,0),2);
                else
                    rectangle(m3,Point(744,517),Point(887,713),Scalar(0,0,0),2);
                savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
            }
            else
            {
                emit addLog("工件无法检测请更换","");
            }



            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                    QByteArray dataArray;


                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");

                    QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));
                }
            }




        }else if(method == 7){//华东大垫片检测

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<21) inputList.append("");


            //start....

            //相机重连
            static int dectedNum2=0;
            if(dectedNum2==fNum[1])
            {
                emit addLog("相机2异常正在重新连接...","");
                if(!DEBUG) return;
            }
            dectedNum2=fNum[1];


            //检测


            params param;
            param.var1[0]=inputList[0].toInt();//设定圆心坐标
            param.var1[1]=inputList[1].toInt();
            param.var1[2]=inputList[2].toInt();//圆心二值
            param.var1[3]=inputList[3].toInt();
            param.var1[4]=inputList[4].toInt();//size
            param.var1[5]=inputList[5].toInt();
            param.var1[6]=inputList[6].toInt();//圆心偏离x最大值
            param.var1[7]=inputList[7].toInt();//rectLen
            param.var1[8]=inputList[8].toInt();

            param.var1[9]= inputList[9].toInt();//圆环上小圆半径范围
            param.var1[10]= inputList[10].toInt();//圆环上小圆半径范围
            param.var1[11]= inputList[11].toInt();//小圆环到圆心距离最小
            param.var1[12]= inputList[12].toInt();//小圆环到圆心距离最大
            param.var1[13]= inputList[13].toInt();//小圆size范围
            param.var1[14]= inputList[14].toInt();
            param.var1[15]= inputList[15].toInt();//二值最大
            param.var1[16]= inputList[16].toInt();//所有数量
            param.var1[17]=inputList[17].toInt();//二值最小
            param.var1[18]=inputList[18].toInt();//小圆边离圆心最小距离
            param.var1[19]=inputList[19].toInt();//小圆边离圆心最大距离
            param.var1[20]=inputList[20].toInt();//污点size


            if(!DEBUG)
            {
                mylock[1].lock();
                cvCopy(IpImgCache[1],m_camImage2);
                mylock[1].unlock();
            }
            else
            {
                m_camImage=cvLoadImage("D:/1.png",0);
            }





            QTime t;
            t.start();
            //Rect roi(350,70,450,450);
            Rect roi;
            roi.x= areaRect[0].x;
            roi.y= areaRect[0].y;
            roi.width= areaRect[0].width;
            roi.height= areaRect[0].height;

            if(DEBUG)
            {
                roi.x= 30;
                roi.y= 1;
                roi.width= 1247-30;
                roi.height= 960-1;
            }

            Mat m= Mat(m_camImage2).clone();

            detectWork(m,roi,param,2);

            //emit addLog(QString("垫片检测耗时:%1 ms").arg(t.elapsed()),"");



            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString val1=param.var1[29]?"1":"2";
                    QString retStr= "??" + outputVarList[0] + "|" + val1;
                    QByteArray dataArray;


                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 2;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"视觉垫片结果:"+retStr.remove(0,2),"");

                    //保存检测图片
                    QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+QString("_%1").arg(param.var1[29]);
                    savedDetectImg(m_camImage2,"savedImage3",fileName);

                }
            }

        }else if(method == 8){//华东角度检测

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<17) inputList.append("");


            //start....

            //相机重连
            static int dectedNum1=0;
            if(dectedNum1==fNum[0])
            {
                emit addLog("相机1异常正在重新连接...","");
                if(!DEBUG) return;
            }
            dectedNum1=fNum[0];


            //检测  480,600,1000,10,240,290,550,330,390,4,800,1200,50,40,300,25000,-96

            params param;
            //中心点检测
            param.var1[0]=inputList[0].toInt();//最小矩形长度
            param.var1[1]=inputList[1].toInt();
            param.var1[2]=inputList[2].toInt();//最小size
            param.var1[3]=inputList[3].toInt();//二值

            //角度检测
            param.var1[10]=inputList[4].toInt();//最小矩形长度
            param.var1[11]=inputList[5].toInt();
            param.var1[12]=inputList[6].toInt();//最小size
            param.var1[13]=inputList[7].toInt();//离中心点长度
            param.var1[14]=inputList[8].toInt();
            param.var1[15]=inputList[9].toInt();//二值

            //角度确认
            param.var1[16]=inputList[10].toInt();//size
            param.var1[17]=inputList[11].toInt();
            param.var1[18]=inputList[12].toInt();//矩形长宽差
            param.var1[19]=inputList[13].toInt();//二值
            param.var1[20]=inputList[14].toInt();//矩形长
            param.var1[21]=inputList[15].toInt();//面积


            param.var2[0]=inputList[16].toDouble();//角度偏移






            if(!DEBUG)
            {
                mylock[0].lock();
                cvCopy(IpImgCache[0],m_camImage);
                mylock[0].unlock();
            }
            else
                m_camImage=cvLoadImage("D:/1.png",0);





            QTime t;
            t.start();
            //Rect roi(482,382,450,450);
            Rect roi;
            if(!DEBUG)
            {
                roi.x= areaRect[0].x;
                roi.y= areaRect[0].y;
                roi.width= areaRect[0].width;
                roi.height= areaRect[0].height;
            }
            else
            {
                roi.x= 3;
                roi.y= 10;
                roi.width= 1278-3;
                roi.height= 946-10;

            }



            //保存检测图片
            QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            savedDetectImg(m_camImage,"savedImage1",fileName);


            detectWork(Mat(m_camImage),roi,param,3);
            emit addLog(QString("角度检测耗时:%1 ms").arg(t.elapsed()),"");


            QFile::rename("D:/savedImage1/"+fileName+picSuff,QString("D:/savedImage1/%1_%2%3").arg(fileName).arg(QString::number(param.var2[19],'f',3)).arg(picSuff));




            if(param.var2[19]!=500)
            {
                if(param.var2[0]+param.var2[19]>180)
                    param.var2[19]=param.var2[0]+param.var2[19]-360;
                else if(param.var2[0]+param.var2[19]<-180)
                    param.var2[19]=param.var2[0]+param.var2[19]+360;
                else
                    param.var2[19]+=param.var2[0];
                emit addLog("角度结果"+QString::number(param.var2[19],'f',3),"");
            }
            else
            {
                emit addLog("视觉检测失败","");
            }



            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                    QByteArray dataArray;


                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    //emit addLog(QTime::currentTime().toString("hh:mm:ss")+"视觉角度结果:"+retStr.remove(0,2),"");





                }
            }




        }else if(method == 9){//华东有无检测

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<4) inputList.append("");


            //start....
            //emit addLog("有无检测:","");
            //相机重连
            static int dectedNum1=0;

            if(dectedNum1==fNum[0])
            {
                emit addLog("相机1异常正在重新连接...","");
                return;
            }
            dectedNum1=fNum[0];


            //检测

            params param;
            param.var1[0]=inputList[0].toInt();//二值
            param.var1[1]=inputList[1].toInt();//xy偏移
            param.var1[2]=inputList[2].toInt();//size
            param.var1[3]=inputList[3].toInt();//size



            mylock[0].lock();
            cvCopy(IpImgCache[0],m_camImage);
            mylock[0].unlock();


            QTime t;
            t.start();
            //Rect roi(440,300,350,350);

            Rect roi;
            roi.x= areaRect[0].x;
            roi.y= areaRect[0].y;
            roi.width= areaRect[0].width;
            roi.height= areaRect[0].height;

            detectWork(Mat(m_camImage),roi,param,4);

            emit addLog(QString("有无检测耗时:%1 ms").arg(t.elapsed()),"");

            qDebug()<<"param.var1[29]"<<param.var1[29];
            if(param.var1[29])
            {
                emit addLog("视觉检测料盘空","");
            }
            else
            {
                emit addLog("视觉检测料盘有","");
            }



            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString val1=param.var1[29]?"1":"2";
                    QString retStr= "??" + outputVarList[0] + "|" + val1 ;
                    QByteArray dataArray;


                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"视觉有无结果:"+retStr.remove(0,2),"");

                    //保存检测图片
                    QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+QString("_%1").arg(param.var1[29]);
                    savedDetectImg(m_camImage,"savedImage2",fileName);
                }
            }

        }else if(method == 10){//华东小垫片检测
            //emit addLog("正反垫片开始检测","");

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<10) inputList.append("");
            //start....
            //emit addLog("开始工件检测2:","");
            //相机重连
            static int dectedNum2=0;
            if(dectedNum2==fNum[1])
            {
                emit addLog("相机2异常正在重新连接...","");
                if(!DEBUG) return;
            }
            dectedNum2=fNum[1];


            //检测

            params param;

            param.var1[0]=inputList[0].toInt();//圆心
            param.var1[1]=inputList[1].toInt();
            param.var1[2]=inputList[2].toInt();//半径
            param.var1[3]=inputList[3].toInt();
            param.var1[4]=inputList[4].toInt();//小圆size范围最小
            param.var1[5]=inputList[5].toInt();//小圆size范围最大
            param.var1[6]=inputList[6].toInt();//与圆心偏离
            param.var1[7]=inputList[7].toInt();//特征数量


            if(!DEBUG)
            {
                mylock[1].lock();
                cvCopy(IpImgCache[1],m_camImage2);
                mylock[1].unlock();
            }
            else
            {

                m_camImage2=cvLoadImage("1.png",0);

            }

            //保存检测图片
            QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            savedDetectImg(m_camImage2,"savedImage5",fileName);

            QTime t;
            t.start();
            //Rect roi(482,382,450,450);
            Rect roi;
            roi.x= areaRect[0].x;
            roi.y= areaRect[0].y;
            roi.width= areaRect[0].width;
            roi.height= areaRect[0].height;

            if(DEBUG)
            {
                roi=Rect(0,0,1280,960);
            }


            detectWork(Mat(m_camImage2),roi,param,18);
            emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");




            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString retStr= "??" + outputVarList[0] + "|";
                    if(param.var1[29]==82000)
                        retStr+="2";
                    else if(param.var1[29]==90000)
                        retStr+="1";
                    QByteArray dataArray;
                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 2;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    if(param.var1[29]==90000)
                        emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:正面","");
                    else if(param.var1[29]==82000)
                        emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:反面","");

                    QFile::rename("D:/savedImage5/"+fileName+picSuff,QString("D:/savedImage5/%1_%2%3").arg(fileName).arg(param.var1[29]).arg(picSuff));


                }
            }

        }else if(method == 11){//华东裂缝检测

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<9) inputList.append("");


            //start....
            emit addLog("华东裂缝检测:","");
            //相机重连
            static int dectedNum1=0;

            if(dectedNum1==fNum[0]&&!DEBUG)
            {
                emit addLog("相机1异常正在重新连接...","");
                return;
            }
            dectedNum1=fNum[0];


            //检测

            params param;

            param.var2[2]=inputList[0].toDouble();//像素比
            param.var1[0]=inputList[1].toInt();//调用方法


            param.var2[3]=inputList[2].toInt();//二值
            param.var1[20]=inputList[3].toInt();//size
            param.var1[21]=inputList[4].toInt();
            param.var1[22]=inputList[5].toInt();//isBlack
            param.var1[23]=inputList[6].toInt();//长宽差
            param.var1[24]=inputList[7].toInt();//x
            param.var1[25]=inputList[8].toInt();//y


            //分别表示crack_num(裂缝数目)、 max_radius(最大直径)、min_radius(最小直径)

            if(!DEBUG)
            {
            mylock[0].lock();
            cvCopy(IpImgCache[0],m_camImage);
            mylock[0].unlock();
            }
            else
            {
                m_camImage=cvLoadImage("D:/1.png",0);
            }


            QTime t;
            t.start();


            // Rect roi(0,0,1280,960);

            Rect roi;
            if(!DEBUG)
            {
            roi.x= areaRect[0].x;
            roi.y= areaRect[0].y;
            roi.width= areaRect[0].width;
            roi.height= areaRect[0].height;
            }
            else
            {
                roi.x=0;
                roi.y=0;
                roi.width=1280;
                roi.height=960;
            }
            QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            savedDetectImg(m_camImage,"savedImage4",fileName);


            detectWork(Mat(m_camImage),roi,param,5);

            if(param.var1[0]==2000)
            {

                param.var1[1]=0;
                param.var2[0]=param.var2[4];
                param.var2[1]=param.var2[6];
            }

            emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");

            //qDebug()<<"param.var1[29]"<<param.var1[29];


            emit addLog(QString("视觉检测裂缝数量%1 直径%2 %3").arg(param.var1[1]).arg(QString::number(param.var2[0],'f',3)).arg(QString::number(param.var2[1],'f',3)),"");

            m_ui->checkDataLabel->setText(QString::number(param.var2[0],'f',3)+"mm");
            m_ui->maodingLabel->setText(QString::number(param.var2[1],'f',3)+"mm");





            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<3) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString val1;
                    if(param.var1[0]>0)
                        val1="1";
                    else
                        val1="2";
                    QString retStr= "??" + outputVarList[0] + "|" + val1 ;
                    QByteArray dataArray;
                    dataArray = retStr.toLocal8Bit();
                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;
                    mysendService(dataArray.data(),dataArray.length(),1);
                    //最小直径
                    retStr= "??" + outputVarList[1] + "|" + QString::number(param.var2[0],'f',3) ;
                    dataArray = retStr.toLocal8Bit();
                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;
                    mysendService(dataArray.data(),dataArray.length(),1);

                    //最大直径
                    retStr= "??" + outputVarList[2] + "|" + QString::number(param.var2[1],'f',3) ;
                    dataArray = retStr.toLocal8Bit();
                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;
                    mysendService(dataArray.data(),dataArray.length(),1);


                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"视觉结果:"+val1,"");

                    //保存检测图片

                    if(param.var2[0]<3||param.var2[1]<3)
                    {
                        savedDetectImg(m_camImage,"wrong",QString("%1_%2").arg(fileName).arg(param.var2[0]));

                    }
                    QFile::rename("D:/savedImage4/"+fileName+picSuff,QString("D:/savedImage4/%1_%2_%3%4").arg(fileName).arg(param.var1[0]).arg(param.var2[0]).arg(picSuff));


                }
            }




        }else if(method == 12){//振挺D线

            QStringList inputList = tmpList[2].split(",");
            while (inputList.length()<9) inputList.append("");


            //start....
            emit addLog("开始工件检测:","");
            //相机重连
            static int dectedNum1=0;
            if(dectedNum1==fNum[0])
            {
                emit addLog("相机1异常正在重新连接...","");
                if(!DEBUG) return;
            }
            dectedNum1=fNum[0];


            //检测  550,505,100,500,108,168,30,22,0

            params param;
            param.var1[0]=inputList[0].toInt();//中心点X
            param.var1[1]=inputList[1].toInt();//中心点Y
            param.var1[2]=inputList[2].toInt();//检测字体最小值
            param.var1[3]=inputList[3].toInt();//检测字体最大值
            param.var1[4]=inputList[4].toInt();//字体到中心点距离
            param.var1[5]=inputList[5].toInt();
            param.var1[6]=inputList[6].toInt();//二值
            param.var1[7]=inputList[7].toInt();//小三角形max len
            param.var2[0]=inputList[8].toDouble();//角度偏移

            param.var1[28]=10000;//得到首次检测到NI坐标
            param.var1[29]=10000;
            param.var2[19]=360;//角度


            if(!DEBUG)
            {
            mylock[0].lock();
            cvCopy(IpImgCache[0],m_camImage);
            mylock[0].unlock();
            }
            else
            {
                m_camImage=cvLoadImage("D:/1.png",0);
            }

            QTime t;
            t.start();
            // Rect roi(270,250,550,550);
            Rect roi;
            if(!DEBUG)
            {
                roi.x= areaRect[0].x;
                roi.y= areaRect[0].y;
                roi.width= areaRect[0].width;
                roi.height= areaRect[0].height;
            }
            else
            {
                roi.x=0;
                roi.y=0;
                roi.width=1280;
                roi.height=960;
            }

            param.var1[28]=10000;//得到检测到字体坐标
            param.var1[29]=10000;
            param.var2[19]=360;//角度

            detectWork(Mat(m_camImage),roi,param,6);

            emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");

            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                    QByteArray dataArray;


                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");

                    //保存检测图片
                    QString fileName=QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")).arg(param.var2[19]);
                    savedDetectImg(m_camImage,"savedImage",fileName);
                    if(param.var2[19]!=360)
                    {
                        Mat m3=Mat(m_camImage);
                        if(abs(param.var2[19])<90)
                            rectangle(m3,Point(350,430),Point(480,600),Scalar(0,0,0),2);
                        else
                            rectangle(m3,Point(640,430),Point(740,600),Scalar(0,0,0),2);
                        savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
                    }
                    else
                    {
                        emit addLog("工件无法检测请更换","");
                    }

                }
            }




        }else if(method == 13){//华东大垫片环形光 [戎鲁凯]

            QStringList inputList = tmpList[2].split(",");
            //while (inputList.length()<18) inputList.append("");


            //start....
            emit addLog("视觉环形光垫片检测:","");
            //相机重连
            static int dectedNum2=0;
            if(dectedNum2==fNum[1])
            {
                emit addLog("相机1异常正在重新连接...","");
                if(!DEBUG) return;
            }
            dectedNum2=fNum[1];


            //检测

            params param;


            if(!DEBUG)
            {
                mylock[1].lock();
                cvCopy(IpImgCache[1],m_camImage2);
                mylock[1].unlock();
            }else
            {
                m_camImage2=cvLoadImage("D:/1.jpg",0);

            }

            QTime t;
            t.start();
            //Rect roi(489,306,277,277);
            Rect roi;
            roi.x= areaRect[0].x;
            roi.y= areaRect[0].y;
            roi.width= areaRect[0].width;
            roi.height= areaRect[0].height;



            Mat m3= Mat(m_camImage2);
            detectWork(m3,roi,param,7);

            emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


            if(param.var1[0]>0)
            {
                emit addLog("正面","");
            }
            else
            {
                emit addLog("反面","");
            }



            QStringList outputVarList = tmpList[3].split(",");
            while (outputVarList.length()<1) outputVarList.append("");
            if (!outputVarList[0].isEmpty()) {
                if (serviceSocket[0]) {
                    QString val1=param.var1[0]>0?"1":"2";
                    QString retStr= "??" + outputVarList[0] + "|" + val1;
                    QByteArray dataArray;


                    dataArray = retStr.toLocal8Bit();

                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 2;

                    mysendService(dataArray.data(),dataArray.length(),1);
                    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"视觉垫片结果:"+retStr.remove(0,2),"");

                    //保存检测图片
                    QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+QString("_%1").arg(param.var1[0]);
                    savedDetectImg(m_camImage2,"savedImage3",fileName);

                }
            }

        }


}
void MainWindow::visionAssemble2(QStringList &tmpList,CvRect areaRect[],QStringList &areaRectId,int method,int camIndex)
{
           if(method == 14){//振挺E线

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<12) inputList.append("");

               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];


               //检测  552,552,100,300,130,168,200,30,50,12,30,0

               params param;
               param.var1[0]=inputList[0].toInt();//中心点X
               param.var1[1]=inputList[1].toInt();//中心点Y
               param.var1[2]=inputList[2].toInt();//检测字体最小值
               param.var1[3]=inputList[3].toInt();//检测字体最大值
               param.var1[4]=inputList[4].toInt();//字体到中心点距离
               param.var1[5]=inputList[5].toInt();
               param.var1[6]=inputList[6].toInt();//二值
               param.var1[7]=inputList[7].toInt();//三角形外接矩形 len
               param.var1[8]=inputList[8].toInt();
               param.var1[9]=inputList[9].toInt();//三角形长宽差
               param.var1[10]=inputList[10].toInt();//三角形最小边长
               param.var2[0]=inputList[11].toDouble();//角度偏移

               param.var1[28]=10000;//得到首次检测到NI坐标
               param.var1[29]=10000;
               param.var2[19]=360;//角度


               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);





               QTime t;
               t.start();

               Rect roi;
               roi.x= areaRect[0].x;
               roi.y= areaRect[0].y;
               roi.width= areaRect[0].width;
               roi.height= areaRect[0].height;

               if(DEBUG)
               {
                   roi.x=0;
                   roi.y=0;
                   roi.width=1280;
                   roi.height=960;
               }



               param.var1[28]=10000;//得到检测到字体坐标
               param.var1[29]=10000;
               param.var2[19]=360;//角度

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);
               detectWork(Mat(m_camImage),roi,param,8);

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


                       QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));

                       if(param.var2[19]!=360)
                       {
                           Mat m3=Mat(m_camImage);
                           if(abs(param.var2[19])<90)
                               rectangle(m3,Point(640,470),Point(740,650),Scalar(0,0,0),2);
                           else
                               rectangle(m3,Point(350,470),Point(480,650),Scalar(0,0,0),2);
                           savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
                       }
                       else
                       {
                           emit addLog("工件无法检测请更换","");
                       }

                   }
               }


           }else if(method == 15){//rong无锡料方向检测

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<26) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];

               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);


               QTime t;
               t.start();

               Rect roi(0,0,1280,960);
               //                                    roi.x= areaRect[0].x;
               //                                    roi.y= areaRect[0].y;
               //                                    roi.width= areaRect[0].width;
               //                                    roi.height= areaRect[0].height;

               //                                    if(DEBUG)
               //                                    {
               //                                        roi.x=470;
               //                                        roi.y=220;
               //                                        roi.width=400;
               //                                        roi.height=400;
               //                                    }

               params param_2;
               //有料方向
               param_2.var1[0]=inputList[0].toInt();//坐标1
               param_2.var1[1]=inputList[1].toInt();
               param_2.var1[2]=inputList[2].toInt();//坐标2
               param_2.var1[3]=inputList[3].toInt();
               param_2.var1[4]=inputList[4].toInt();//次数
               param_2.var1[5]=inputList[5].toInt();//小于灰度值
               //空料方向
               param_2.var1[10]=inputList[6].toInt();//位置上
               param_2.var1[11]=inputList[7].toInt();
               param_2.var1[12]=inputList[8].toInt();//位置左
               param_2.var1[13]=inputList[9].toInt();
               param_2.var1[14]=inputList[10].toInt();//二值
               param_2.var1[15]=inputList[11].toInt();//size
               param_2.var1[16]=inputList[12].toInt();
               param_2.var1[17]=inputList[13].toInt();//最小长度
               param_2.var1[18]=inputList[14].toInt();

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage1",fileName);
               detectWork(Mat(m_camImage),roi,param_2,9);


               //检测  933,928,1190,411,70,70,643,139,415,542,200,170,260,40,80,150,700,110,190,8,130,160,0.04516,6,7,0.27

               params param;



               //缺陷  150,700,50,190,8,160,160,0.04516,6,8,0.27
               param.var1[20]=inputList[15].toInt();//size
               param.var1[21]=inputList[16].toInt();
               param.var1[22]=inputList[17].toInt();//圆len
               param.var1[23]=inputList[18].toInt();
               param.var1[24]=inputList[19].toInt();//圆心偏差
               param.var1[25]=inputList[20].toInt();//二值
               param.var1[26]=inputList[21].toInt();//外圆len

               param.var2[0]=inputList[22].toDouble();//像素比
               param.var2[1]=inputList[23].toDouble();//最小直径
               param.var2[2]=inputList[24].toDouble();//最大直径
               param.var2[3]=inputList[25].toDouble();//贴边最小距离

               //out
               param.var2[17]=0;
               param.var2[18]=0;
               param.var2[19]=0;

               if(param_2.var1[0]!=360)
               {
                   detectWork(Mat(m_camImage),roi,param,11);
               }

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");



               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<2) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr;
                       retStr= "??" + outputVarList[0] + "|" + QString::number(param_2.var1[0]);
                       QByteArray dataArray;
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       dataArray[1] = 2;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");

                       //第二次发送
                       retStr= "??" + outputVarList[1] + "|" + QString::number(param.var1[27]);
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果2:"+retStr.remove(0,2),"");

                       if(param.var1[28]!=1)
                           emit addLog("未检测到铆头","");
                       if(param.var1[28]==1)
                       {
                           emit addLog(QString("直径 %1mm %2mm 小最贴边 %3mm").arg(param.var2[19]).arg(param.var2[18]).arg(param.var2[16]),"");
                       }
                       m_ui->yuanxinjuLabel->setText(QString::number(param.var2[17],'f',3)+"mm");
                       if(param_2.var1[0]==1)
                           m_ui->directLabel->setText("方向右");
                       if(param_2.var1[0]==2)
                           m_ui->directLabel->setText("方向左");

                       m_ui->maodingLabel->setText(QString::number((param.var2[18]+param.var2[19])/2,'f',3)+"mm");
                       QStringList title,cpkList;
                       title<<"日期"<<"最否合格"<<"最小直径"<<"最大直径"<<"贴边";
                       QString NG="不合格";
                       if(param_2.var1[1]==1)
                       {
                           if(param.var1[28])
                           {


                               if(param.var1[27]==0) NG="合格";


                           }

                           cpkList<<QDateTime::currentDateTime().toString("hh:mm:ss yyyy-MM-dd")<<NG<<QString::number(param.var2[19],'f',3)<<QString::number(param.var2[18],'f',3)<<QString::number(param.var2[16],'f',3);

                           common::addCpk3(title,cpkList);
                       }

                       QFile::rename("D:/savedImage1/"+fileName+picSuff,QString("D:/savedImage1/%1_%2_%3_%4%5").arg(fileName).arg(param.var1[27]).arg(QString::number(param.var2[19],'f',3)).arg(QString::number(param.var2[18],'f',3)).arg(picSuff));

                       if(param_2.var1[0]==360)
                       {
                           emit addLog("工件无法检测请更换","");
                       }

                   }
               }




           }else if(method == 16){//无锡缺陷

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<26) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];


               //检测  933,928,1190,411,70,70,643,139,415,542,200,170,260,40,80,150,700,110,190,8,130,160,0.04516,6,7,0.27

               params param;

               //有料方向
               param.var1[0]=inputList[0].toInt();//坐标1
               param.var1[1]=inputList[1].toInt();
               param.var1[2]=inputList[2].toInt();//坐标2
               param.var1[3]=inputList[3].toInt();
               param.var1[4]=inputList[4].toInt();//次数
               param.var1[5]=inputList[5].toInt();//小于灰度值
               //空料方向
               param.var1[10]=inputList[6].toInt();//位置上
               param.var1[11]=inputList[7].toInt();
               param.var1[12]=inputList[8].toInt();//位置左
               param.var1[13]=inputList[9].toInt();
               param.var1[14]=inputList[10].toInt();//二值
               param.var1[15]=inputList[11].toInt();//size
               param.var1[16]=inputList[12].toInt();
               param.var1[17]=inputList[13].toInt();//最小长度
               param.var1[18]=inputList[14].toInt();

               //缺陷
               param.var1[20]=inputList[15].toInt();//size
               param.var1[21]=inputList[16].toInt();
               param.var1[22]=inputList[17].toInt();//len
               param.var1[23]=inputList[18].toInt();
               param.var1[24]=inputList[19].toInt();//圆心偏差
               param.var1[25]=inputList[20].toInt();//二值
               param.var1[26]=inputList[21].toInt();//外圆len

               param.var2[0]=inputList[22].toDouble();//像素比
               param.var2[1]=inputList[23].toDouble();//最小直径
               param.var2[2]=inputList[24].toDouble();//最大直径
               param.var2[3]=inputList[25].toDouble();//贴边最小距离
               //out
               param.var2[17]=0;
               param.var2[18]=0;
               param.var2[19]=0;

               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);





               QTime t;
               t.start();

               Rect roi;
               roi.x= areaRect[0].x;
               roi.y= areaRect[0].y;
               roi.width= areaRect[0].width;
               roi.height= areaRect[0].height;

               if(DEBUG)
               {
                   //Rect roi(308,53,420,580);
                   roi.x=308;
                   roi.y=53;
                   roi.width=420;
                   roi.height=580;
               }


               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);
               detectWork(Mat(m_camImage),roi,param,10);

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<2) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr;
                       retStr= "??" + outputVarList[0] + "|" + QString::number(param.var1[29]);
                       QByteArray dataArray;
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       dataArray[1] = 2;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");

                       //第二次发送
                       retStr= "??" + outputVarList[1] + "|" + QString::number(param.var1[27]);
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果2:"+retStr.remove(0,2),"");

                       if(param.var1[28]!=1)
                           emit addLog("未检测到铆头","");
                       if(param.var1[28]==1)
                       {
                           emit addLog(QString("直径 %1mm %2mm 圆心距 %3mm").arg(param.var2[19]).arg(param.var2[18]).arg(param.var2[17]),"");
                       }
                       m_ui->yuanxinjuLabel->setText(QString::number(param.var2[17],'f',3)+"mm");
                       if(param.var1[29]==1)
                           m_ui->directLabel->setText("方向右");
                       if(param.var1[29]==2)
                           m_ui->directLabel->setText("方向左");

                       m_ui->maodingLabel->setText(QString::number((param.var2[18]+param.var2[19])/2,'f',3)+"mm");

                       if(param.var1[28])
                       {
                           QStringList title,cpkList;
                           title<<"日期"<<"最否合格"<<"最小直径"<<"最大直径"<<"贴边";
                           QString NG="不合格",NG1="无";
                           if(param.var1[27]==0) NG="合格";
                           if(param.var1[27]==5) NG1="贴边";

                           cpkList<<QDateTime::currentDateTime().toString("hh:mm:ss yyyy-MM-dd")<<NG<<QString::number(param.var2[19],'f',3)<<QString::number(param.var2[18],'f',3)<<NG1;

                           common::addCpk3(title,cpkList);
                       }
                       QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2-%3_%4%5").arg(fileName).arg(param.var1[27]).arg(QString::number(param.var2[19],'f',3)).arg(QString::number(param.var2[18],'f',3)).arg(picSuff));

                       if(param.var1[29]==360)
                       {
                           emit addLog("工件无法检测请更换","");
                       }

                   }
               }




           }else if (method == 17) {  // "铆钉检测算法"
               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<5) inputList.append("");
               emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  开始铆钉视觉检测" /* + tmpList[2]*/, QString());
               cvResetImageROI(cameraIpImg);
               //                                    cvSaveImage("TempImage1.PNG", cameraIpImg);
               //                                    IplImage *tmpImage =cvLoadImage("TempImage1.PNG", 0);
               IplImage* tmpImage = cvCreateImage(cvGetSize(cameraIpImg),8,1);
               cvCvtColor(cameraIpImg, tmpImage, CV_RGB2GRAY);
               bool result = detecitonWorkPiece(tmpImage, areaRect[0], inputList[0].toFloat(), inputList[1].toFloat(), inputList[2].toFloat(), inputList[3].toInt(), inputList[4].toFloat());//6.0, 6.5, 0.2, 150, 0.0252);
               m_ui->maodingLabel->setText(QString().sprintf("%07.3fmm", rivetDia));  //工件内部铆钉直径
               //                                    m_ui->yuanpanLabel->setText(QString().sprintf("%07.3fmm", diskDia));  //圆盘直径
               m_ui->yuanxinjuLabel->setText(QString().sprintf("%07.3fmm", rivetDceDiff)); //铆钉圆心位置和圆盘圆心位置距离差值
               m_ui->directLabel->setText(armDirection==2?"方向右":"方向左"); //手臂方向，0：无，1：方向左，2：方向右
               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<5) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket) {
                       QString str = "?" + outputVarList[0] + (result?"|1":"|0");
                       QByteArray dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[1] + "|" + QString().sprintf("%07.3f", rivetDia);
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[2] + "|" + QString().sprintf("%07.3f", diskDia);
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[3] + "|" + QString().sprintf("%07.3f", rivetDceDiff);
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[4] + "|" + QString::number(armDirection);
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                   }
               }
               emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  视觉检测结果：" + (result?"合格":"不合格"), QString());
           } else if (method == 18) {   // 铆钉位置1检测"
               emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  开始位置1检测", QString());
               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<3) inputList.append("");
               cvResetImageROI(cameraIpImg);
               //                                    cvSaveImage("TempImage2.PNG", cameraIpImg);
               //                                    IplImage *tmpImage =cvLoadImage("TempImage2.PNG", 0);
               IplImage* tmpImage = cvCreateImage(cvGetSize(cameraIpImg),8,1);
               cvCvtColor(cameraIpImg, tmpImage, CV_RGB2GRAY);
               CICLEPAR ciclepar = getBaseCicularDce(tmpImage, areaRect[0], inputList[0].toInt(), inputList[1].toFloat(), inputList[2].toFloat());
               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<3) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket) {
                       QString str = "?" + outputVarList[0] + "|" + ciclepar.discx;
                       QByteArray dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[1] + "|" + ciclepar.discy;
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[2] + "|" + ciclepar.r;
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                   }
               }
               emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  完成视觉检测", QString());
           } else if (method == 19) {   // "铆钉位置2检测"
               emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  开始位置2检测", QString());
               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<3) inputList.append("");
               cvResetImageROI(cameraIpImg);
               //                                    cvSaveImage("TempImage3.PNG", cameraIpImg);
               //                                    IplImage *tmpImage =cvLoadImage("TempImage3.PNG", 0);
               IplImage* tmpImage = cvCreateImage(cvGetSize(cameraIpImg),8,1);
               cvCvtColor(cameraIpImg, tmpImage, CV_RGB2GRAY);
               CICLEPAR ciclepar = getCicularDce(tmpImage, areaRect[0], inputList[0].toInt(), inputList[1].toFloat(), inputList[2].toFloat());
               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<3) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket) {
                       QString str = "?" + outputVarList[0] + "|" + ciclepar.discx;
                       QByteArray dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[1] + "|" + ciclepar.discy;
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                       /*QString*/ str = "?" + outputVarList[2] + "|" + ciclepar.r;
                       /*QByteArray*/ dataArray = str.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       mysendService(dataArray.data(), dataArray.length(),1);
                   }
               }
               emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  完成视觉检测", QString());
           } else if (method == 20) {   // "间隙检测"
               emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  开始间隙检测", QString());
               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<6) inputList.append("");
               emit getTMMeasureGap( inputList[0].toInt(), inputList[1].toInt(), inputList[2].toInt(), inputList[3].toInt(), inputList[4].toInt(), inputList[5].toInt(), tmpList[3] );
           }else if(method == 21){//振挺F线

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<12) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];


               //检测  656,515,50,120,225,257,170,20,47,19,0

               params param;
               param.var1[0]=inputList[0].toInt();//中心点X
               param.var1[1]=inputList[1].toInt();//中心点Y
               param.var1[2]=inputList[2].toInt();// param.var1[2];//检测字体最小值
               param.var1[3]=inputList[3].toInt();//param.var1[3];//检测字体最大值
               param.var1[4]=inputList[4].toInt();//字体到中心点距离
               param.var1[5]=inputList[5].toInt();
               param.var1[6]=inputList[6].toInt();//二值
               param.var1[7]=inputList[7].toInt();//三角形外接矩形len
               param.var1[8]=inputList[8].toInt();
               param.var1[9]=inputList[9].toInt();//三角形最小边长
               param.var2[0]=inputList[10].toDouble();//角度偏移


               param.var1[28]=10000;//得到首次检测到NI坐标
               param.var1[29]=10000;
               param.var2[19]=360;//角度


               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);

               QTime t;
               t.start();

               Rect roi;
               roi.x= areaRect[0].x;
               roi.y= areaRect[0].y;
               roi.width= areaRect[0].width;
               roi.height= areaRect[0].height;

               if(DEBUG)
               {
                   roi.x=510;
                   roi.y=180;
                   roi.width=320;
                   roi.height=630;
               }



               param.var1[28]=10000;//得到检测到字体坐标
               param.var1[29]=10000;
               param.var2[19]=360;//角度

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);
               detectWork(Mat(m_camImage),roi,param,12);

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");
                       QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));

                       if(param.var2[19]!=360)
                       {
                           Mat m3=Mat(m_camImage);
                           if(param.var2[19]<0)
                               rectangle(m3,Point(590,215),Point(775,333),Scalar(0,0,0),2);
                           else
                               rectangle(m3,Point(555,658),Point(784,772),Scalar(0,0,0),2);
                           savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
                       }
                       else
                       {
                           emit addLog("工件无法检测请更换","");
                       }

                   }
               }


           }else if(method == 22){//模板匹配

               visionTime=QTime::currentTime();
               static bool loadImg1=false;
               if(!DEBUG&&areaRectId.size()<3)
               {
                   emit addLog("模板参数个数小于2","");
                   return;
               }else{
                   if(!loadImg1)
                   {
                       loadImg1=true;

                       QDir dir2("temp/");
                       dir2.setFilter(QDir::Files);
                       QFileInfoList list2= dir2.entryInfoList();

                       for(int i=0;i<list2.size();i++)
                       {
                           QString path="temp/"+list2[i].fileName();
                           modelNameList.append(list2[i].fileName());
                           templateImage2.append(imread(path.toLocal8Bit().data(),0));
                       }
                   }
               }
               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<4) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];

               //0,5,30,0;

               params param;

               param.var1[0]=inputList[0].toInt();//模板图标定位置
               param.var1[1]=inputList[1].toInt();//匹配方法
               param.var1[2]=inputList[2].toInt();//偏差范围

               param.var2[0]=inputList[3].toDouble();//角度偏移
               param.var2[19]=360;


               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();


               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);

               QTime t;
               t.start();

               Rect roi(areaRect[0].x,areaRect[0].y,areaRect[0].width,areaRect[0].height);
               Rect roi1(areaRect[1].x,areaRect[1].y,areaRect[1].width,areaRect[1].height);
               Rect roi2(areaRect[2].x,areaRect[2].y,areaRect[2].width,areaRect[2].height);
               Rect roi3;

               if(DEBUG)
               {
                   roi.x=450;
                   roi.y=200;
                   roi.width=400;
                   roi.height=600;

                   roi1.x=574;
                   roi1.y=244;
                   roi1.width=165;
                   roi1.height=60;

                   roi2.x=574;
                   roi2.y=720;
                   roi2.width=165;
                   roi2.height=60;
               }



               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);

               showVisionRet[0]=true;

               // Rect roi(450,200,400,600);
               int templatePos=param.var1[0];//模板图标定位置
               int method=param.var1[1];//匹配方法
               int difVal=param.var1[2];//匹配偏差范围


               if(templatePos==0)
                   roi3=roi1.y>roi2.y?roi1:roi2;
               else if(templatePos==1){
                   roi3=roi1.y<roi2.y?roi1:roi2;
               }else if(templatePos==2){
                   roi3=roi1.x>roi2.x?roi1:roi2;
               }else if(templatePos==3){
                   roi3=roi1.x<roi2.x?roi1:roi2;
               }

               Point cen;
               Point t1Cen(roi1.x+roi1.width/2,roi1.y+roi1.height/2);
               Point t2Cen(roi2.x+roi2.width/2,roi2.y+roi2.height/2);

               cen.x=(t1Cen.x+t2Cen.x)/2;
               cen.y=(t1Cen.y+t2Cen.y)/2;

               int minLen=10000;
               int len;
               Mat m2=Mat(m_camImage);
               rotateRoi(m2,roi3);
               int findModelPic=-1;
               for(int j=0;j<templateImage2.size();j++)
               {
                   Point p1;

                   cppTemplateMatch(m2,templateImage2[j],p1,roi,method);


                   p1.x+=roi.x+templateImage2[j].cols/2;
                   p1.y+=roi.y+templateImage2[j].rows/2;
                   int len1=common::getLineLen(t1Cen,p1);
                   int len2=common::getLineLen(t2Cen,p1);

                   len=len1<len2?len1:len2;

                   if(len<difVal&&len<minLen)
                   {
                       findModelPic=j;
                       minLen=len;
                       param.var2[19]=common::calcAngle2(cen,p1);
                       param.var1[29]=minLen;
                       param.var1[28]=p1.x;
                       param.var1[27]=p1.y;
                   }
               }

               if(param.var2[19]!=360&&param.var2[0]!=0)
               {
                   param.var2[19]=common::anlgeOffset(param.var2[19],param.var2[0]);
               }

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


                       QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));

                       if(param.var2[19]!=360)
                       {

                           //if(param.var1[3]<5)
                           {
                               showRect[0]=Point(param.var1[28],param.var1[27]);
                           }
                           Mat m3=Mat(m_camImage);
                           rectangle( m3, Point(param.var1[28]-roi1.width/2,param.var1[27]-roi1.height/2), Point( param.var1[28]+roi1.width/2 ,  param.var1[27]+roi1.height/2 ), Scalar(0,0,0),2 );
                           //                                                if(param.var2[19]<0)
                           //                                                    rectangle(m3,Point(590,215),Point(775,333),Scalar(0,0,0),2);
                           //                                                else
                           //                                                    rectangle(m3,Point(555,658),Point(784,772),Scalar(0,0,0),2);
                           savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName+QString("name_%1").arg(modelNameList[findModelPic]));
                       }
                       else
                       {
                           emit addLog("工件无法检测请更换","");
                       }

                   }
               }

           }else if(method == 23){//振挺G线

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<19) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];

               //检测

               //663,482,150,450,140,180,40,220,26,70,118,75,190,11,70,0,2,2.2,1.8
               params param;
               param.var1[0]=inputList[0].toInt();//中心点X
               param.var1[1]=inputList[1].toInt();//中心点Y
               param.var1[2]=inputList[2].toInt();// param.var1[2];//检测字体最小值
               param.var1[3]=inputList[3].toInt();//param.var1[3];//检测字体最大值
               param.var1[4]=inputList[4].toInt();//字体到中心点距离
               param.var1[5]=inputList[5].toInt();
               param.var1[6]=inputList[6].toInt();//二值
               param.var1[7]=inputList[7].toInt();
               param.var1[8]=inputList[8].toInt();//最小边长
               param.var1[9]=inputList[9].toInt();//最大边长
               param.var1[10]=inputList[10].toInt();//字体最大间距

               param.var1[15]=inputList[11].toInt();//NI最小size
               param.var1[16]=inputList[12].toInt();//NI最大size
               param.var1[17]=inputList[13].toInt();//NI最小边长
               param.var1[18]=inputList[14].toInt();//NI最大边长

               param.var2[0]=inputList[15].toDouble();//角度偏移
               param.var2[1]=inputList[16].toDouble();//长宽比例
               param.var2[2]=inputList[17].toDouble();//NI长宽比例
               param.var2[3]=inputList[18].toDouble();//NI面积比例


               param.var1[28]=10000;//得到首次检测到NI坐标
               param.var1[29]=10000;
               param.var2[19]=360;//角度


               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);

               QTime t;
               t.start();

               Rect roi;
               roi.x= areaRect[0].x;
               roi.y= areaRect[0].y;
               roi.width= areaRect[0].width;
               roi.height= areaRect[0].height;

               if(DEBUG)
               {
                   roi.x=0;
                   roi.y=0;
                   roi.width=1280;
                   roi.height=960;
               }

               param.var1[28]=10000;//得到检测到字体坐标
               param.var1[29]=10000;
               param.var2[19]=360;//角度

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);

               detectWork(Mat(m_camImage),roi,param,13);

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


                       QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));

                       if(param.var2[19]!=360)
                       {
                           Mat m3=Mat(m_camImage);
                           if(abs(param.var2[19])<90)
                               rectangle(m3,Point(param.var1[0]-200,param.var1[1]-100),Point(param.var1[0]-120,param.var1[1]+100),Scalar(0,0,0),2);
                           else
                               rectangle(m3,Point(param.var1[0]+120,param.var1[1]-100),Point(param.var1[0]+200,param.var1[1]+100),Scalar(0,0,0),2);
                           savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
                       }
                       else
                       {
                           emit addLog("工件无法检测请更换","");
                       }

                   }
               }

           }else if(method == 24){//间隙检测20170613  zhugz


               //start....
               emit addLog("开始工件检测:","");

               emit tmGap(tmpList[2],tmpList[3]);


           }else if(method == 25){//预留

                         QStringList inputList = tmpList[2].split(",");
                         while (inputList.length()<55) inputList.append("");


                         //start....
                         emit addLog("开始工件检测:","");
                         //相机重连
                         static int dectedNum1=0;
                         if(dectedNum1==fNum[0])
                         {
                             emit addLog("相机1异常正在重新连接...","");
                             if(!DEBUG) return;
                         }
                         dectedNum1=fNum[0];

                         //检测

//100,100,100,1,2,3,4,5,0.0
                         params param;
                         param.var1[0]=inputList[0].toInt();
                         param.var1[1]=inputList[1].toInt();
                         param.var1[2]=inputList[7].toInt();
                         param.var1[3]=inputList[8].toInt();
                         param.var1[4]=inputList[9].toInt();
                         param.var1[5]=inputList[10].toInt();
                         param.var1[6]=inputList[11].toInt();
                         param.var1[7]=inputList[12].toInt();
                         param.var1[8]=inputList[13].toInt();
                         param.var1[9]=inputList[14].toInt();
                         param.var1[10]=inputList[15].toInt();
                         param.var1[11]=inputList[16].toInt();
                         param.var1[12]=inputList[17].toInt();
                         param.var1[13]=inputList[18].toInt();
                         param.var1[14]=inputList[19].toInt();
                         param.var1[15]=inputList[20].toInt();
                         param.var1[16]=inputList[21].toInt();
                         param.var1[17]=inputList[22].toInt();
                         param.var1[18]=inputList[23].toInt();
                         param.var1[19]=inputList[24].toInt();
                         param.var1[20]=inputList[25].toInt();
                         param.var1[21]=inputList[26].toInt();
                         param.var1[22]=inputList[27].toInt();
                         param.var1[23]=inputList[28].toInt();
                         param.var1[24]=inputList[29].toInt();
                         param.var1[25]=inputList[30].toInt();
                         param.var1[26]=inputList[31].toInt();
                         param.var1[27]=inputList[32].toInt();
                         param.var1[28]=inputList[33].toInt();
                         param.var1[29]=inputList[34].toInt();
                         param.var2[0]=inputList[35].toDouble();
                         param.var2[1]=inputList[36].toDouble();
                         param.var2[2]=inputList[37].toDouble();
                         param.var2[3]=inputList[38].toDouble();
                         param.var2[4]=inputList[39].toDouble();
                         param.var2[5]=inputList[40].toDouble();
                         param.var2[6]=inputList[41].toDouble();
                         param.var2[7]=inputList[42].toDouble();
                         param.var2[8]=inputList[43].toDouble();
                         param.var2[9]=inputList[44].toDouble();
                         param.var2[10]=inputList[45].toDouble();
                         param.var2[11]=inputList[46].toDouble();
                         param.var2[12]=inputList[47].toDouble();
                         param.var2[13]=inputList[48].toDouble();
                         param.var2[14]=inputList[49].toDouble();
                         param.var2[15]=inputList[50].toDouble();
                         param.var2[16]=inputList[51].toDouble();
                         param.var2[17]=inputList[52].toDouble();
                         param.var2[18]=inputList[53].toDouble();


                         if(!DEBUG)
                         {
                             mylock[0].lock();
                             cvCopy(IpImgCache[0],m_camImage);
                             mylock[0].unlock();
                         }
                         else
                         {
                             m_camImage=cvLoadImage("D:/1.png",0);
                         }

                         QTime t;
                         t.start();

                         Rect roi;
                         qDebug()<<"aaa"<<inputList.size();

                         if(!DEBUG)
                         {
                         roi.x= areaRect[0].x;
                         roi.y= areaRect[0].y;
                         roi.width= areaRect[0].width;
                         roi.height= areaRect[0].height;
                         }
                         else
                         {
                             roi.x=0;
                             roi.y=0;
                             roi.width=1280;
                             roi.height=960;
                         }



                         int detectMethod=inputList[2].toInt();//方法
                         int num1=inputList[3].toInt(); //参数个数
                         int angleFlag=inputList[4].toInt();//画矩形
                         int distance=inputList[5].toInt();//距离
                         int sqaure2=inputList[6].toInt();//预留

                         if(num1>0&&num1<55)
                            param.var2[0]=inputList[num1-1].toDouble();//角度偏移


                         detectWork(Mat(m_camImage),roi,param,detectMethod);

                         emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");
                         qDebug()<<"ccc"<<inputList.size();


                         QStringList outputVarList = tmpList[3].split(",");
                         while (outputVarList.length()<1) outputVarList.append("");
                         if (!outputVarList[0].isEmpty()) {
                             if (serviceSocket[0]) {
                                 QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                                 QByteArray dataArray;


                                 dataArray = retStr.toLocal8Bit();

                                 dataArray[0] = CMD_WRITE_VAR;
                                 dataArray[1] = 1;

                                 mysendService(dataArray.data(),dataArray.length(),1);
                                 emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


                                 QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+QString("_%1").arg(param.var2[19]);
                                 savedDetectImg(m_camImage,"savedImage",fileName);


                                 if(param.var2[19]!=360)
                                 {
                                     Mat m3=Mat(m_camImage);
                                     drawRect(m3,Point(param.var1[0],param.var1[1]),distance,param.var2[19],angleFlag,Scalar(0,0,0));
                                     savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
                                 }
                                 else
                                 {
                                     emit addLog("工件无法检测请更换","");
                                 }

                             }
                         }

           }else if(method == 26){//zhu无锡料方向检测

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<26) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];

               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);


               QTime t;
               t.start();

               Rect roi(0,0,1280,960);
               //                                    roi.x= areaRect[0].x;
               //                                    roi.y= areaRect[0].y;
               //                                    roi.width= areaRect[0].width;
               //                                    roi.height= areaRect[0].height;

               //                                    if(DEBUG)
               //                                    {
               //                                        roi.x=470;
               //                                        roi.y=220;
               //                                        roi.width=400;
               //                                        roi.height=400;
               //                                    }

               params param_2;
               //有料方向
               param_2.var1[0]=inputList[0].toInt();//坐标1
               param_2.var1[1]=inputList[1].toInt();
               param_2.var1[2]=inputList[2].toInt();//坐标2
               param_2.var1[3]=inputList[3].toInt();
               param_2.var1[4]=inputList[4].toInt();//次数
               param_2.var1[5]=inputList[5].toInt();//小于灰度值
               //空料方向
               param_2.var1[10]=inputList[6].toInt();//位置上
               param_2.var1[11]=inputList[7].toInt();
               param_2.var1[12]=inputList[8].toInt();//位置左
               param_2.var1[13]=inputList[9].toInt();
               param_2.var1[14]=inputList[10].toInt();//二值
               param_2.var1[15]=inputList[11].toInt();//size
               param_2.var1[16]=inputList[12].toInt();
               param_2.var1[17]=inputList[13].toInt();//最小长度
               param_2.var1[18]=inputList[14].toInt();

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage1",fileName);
               detectWork(Mat(m_camImage),roi,param_2,10);


               //检测  933,928,1190,411,70,70,643,139,415,542,200,170,260,40,80,150,700,110,190,8,130,160,0.04516,6,7,0.27

               params param;



               //缺陷  150,700,50,190,8,160,160,0.04516,6,8,0.27
               param.var1[20]=inputList[15].toInt();//size
               param.var1[21]=inputList[16].toInt();
               param.var1[22]=inputList[17].toInt();//圆len
               param.var1[23]=inputList[18].toInt();
               param.var1[24]=inputList[19].toInt();//圆心偏差
               param.var1[25]=inputList[20].toInt();//二值
               param.var1[26]=inputList[21].toInt();//外圆len

               param.var2[0]=inputList[22].toDouble();//像素比
               param.var2[1]=inputList[23].toDouble();//最小直径
               param.var2[2]=inputList[24].toDouble();//最大直径
               param.var2[3]=inputList[25].toDouble();//贴边最小距离

               //out
               param.var2[17]=0;
               param.var2[18]=0;
               param.var2[19]=0;

               if(param_2.var1[29]!=360)
               {
                   detectWork(Mat(m_camImage),roi,param,11);
               }

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");



               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<2) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr;
                       retStr= "??" + outputVarList[0] + "|" + QString::number(param_2.var1[29]);
                       QByteArray dataArray;
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       dataArray[1] = 2;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");

                       //第二次发送
                       retStr= "??" + outputVarList[1] + "|" + QString::number(param.var1[27]);
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果2:"+retStr.remove(0,2),"");

                       if(param.var1[28]!=1)
                           emit addLog("未检测到铆头","");
                       if(param.var1[28]==1)
                       {
                           emit addLog(QString("直径 %1mm %2mm 小最贴边 %3mm").arg(param.var2[19]).arg(param.var2[18]).arg(param.var2[16]),"");
                       }
                       m_ui->yuanxinjuLabel->setText(QString::number(param.var2[17],'f',3)+"mm");
                       if(param_2.var1[29]==1)
                           m_ui->directLabel->setText("方向右");
                       if(param_2.var1[29]==2)
                           m_ui->directLabel->setText("方向左");

                       m_ui->maodingLabel->setText(QString::number((param.var2[18]+param.var2[19])/2,'f',3)+"mm");
                       QStringList title,cpkList;
                       title<<"日期"<<"最否合格"<<"最小直径"<<"最大直径"<<"贴边";
                       QString NG="不合格";
                       if(param_2.var1[29]!=360)
                       {
                           if(param.var1[28])
                           {


                               if(param.var1[27]==0) NG="合格";


                           }

                           cpkList<<QDateTime::currentDateTime().toString("hh:mm:ss yyyy-MM-dd")<<NG<<QString::number(param.var2[19],'f',3)<<QString::number(param.var2[18],'f',3)<<QString::number(param.var2[16],'f',3);

                           common::addCpk3(title,cpkList);
                       }

                       QFile::rename("D:/savedImage1/"+fileName+picSuff,QString("D:/savedImage1/%1_%2_%3_%4%5").arg(fileName).arg(param.var1[27]).arg(QString::number(param.var2[19],'f',3)).arg(QString::number(param.var2[18],'f',3)).arg(picSuff));

                       if(param_2.var1[29]==360)
                       {
                           emit addLog("工件无法检测请更换","");
                       }

                   }
               }




           }else if(method == 27){//灰度值检测

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<2) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];

               //检测

               params param;
               param.var1[0]=inputList[0].toInt();//grayVal

               param.var1[29]=0;//out


               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);

               QTime t;
               t.start();

               Rect roi;
               roi.x= areaRect[0].x;
               roi.y= areaRect[0].y;
               roi.width= areaRect[0].width;
               roi.height= areaRect[0].height;

               if(DEBUG)
               {
                   roi.x=0;
                   roi.y=0;
                   roi.width=1280;
                   roi.height=960;
               }

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);

               detectWork(Mat(m_camImage),roi,param,15);

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var1[29]);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");
                       if(param.var1[29])
                           emit addLog("有料","");
                       else
                           emit addLog("无料","");

                       QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[29]).arg(picSuff));



                   }
               }

           }else if(method == 28){//三角形工件I线

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<24) inputList.append("");


               //start....
               emit addLog("开始工件检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];

               //检测 496,480,57,150,103,143,70,254,20,42,-1,-1,18,5,0

               params param;

               param.var1[0]=inputList[0].toInt();//中心点X
               param.var1[1]=inputList[1].toInt();//中心点Y
               param.var1[2]=inputList[2].toInt();//检测字体最小值
               param.var1[3]=inputList[3].toInt();//检测字体最大值
               param.var1[4]=inputList[4].toInt();//字体到中心点距离
               param.var1[5]=inputList[5].toInt();
               param.var1[6]=inputList[6].toInt();//二值
               param.var1[7]=inputList[7].toInt();//二值
               param.var1[8]=inputList[8].toInt();//三角形外接矩形 len
               param.var1[9]=inputList[9].toInt();
               param.var1[10]=inputList[10].toInt();//三角形外接矩形较短边角度
               param.var1[11]=inputList[11].toInt();
               param.var1[12]=inputList[12].toInt();//三角形最小边长
               param.var1[13]=inputList[13].toInt();//特征数量

               param.var1[20]=inputList[14].toInt();//检测最小值
               param.var1[21]=inputList[15].toInt();//检测最大值
               param.var1[22]=inputList[16].toInt();//矩形len
               param.var1[23]=inputList[17].toInt();
               param.var1[24]=inputList[18].toInt();//step
               param.var1[25]=inputList[19].toInt();//x距离
               param.var1[26]=inputList[20].toInt();//开始位置
               param.var1[27]=inputList[21].toInt();//二值
               param.var2[0]=inputList[22].toDouble();//角度偏移


               param.var2[19]=360;//角度
               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);

               QTime t;
               t.start();

               Rect roi;
               roi.x= areaRect[0].x;
               roi.y= areaRect[0].y;
               roi.width= areaRect[0].width;
               roi.height= areaRect[0].height;

               if(DEBUG)
               {
                   roi.x=0;
                   roi.y=0;
                   roi.width=1280;
                   roi.height=960;
               }

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);

               detectWork(Mat(m_camImage),roi,param,20);

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19]);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


                       QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));

                       if(param.var2[19]!=360)
                       {
                           Mat m3=Mat(m_camImage);
                           drawRect(m3,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],90,Scalar(0,0,0));
                           savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
                       }
                       else
                       {
                           emit addLog("工件无法检测请更换","");
                       }


                   }
               }

           }else if(method == 29){//雪龙异物

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<10) inputList.append("");


               //start....
               emit addLog("异物检测:","");
               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];

               //检测

               params param;


               param.var1[0]=inputList[0].toInt();//圆心X
               param.var1[1]=inputList[1].toInt();//圆心Y
               param.var1[2]=inputList[2].toInt();//小圆孔size
               param.var1[3]=inputList[3].toInt();//半径
               param.var1[4]=inputList[4].toInt();//异物灰度值
               param.var1[5]=inputList[5].toInt();//过滤最小长度大于
               param.var1[6]=inputList[6].toInt();//过滤最小长度小于
               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
               {
                   m_camImage=cvLoadImage("D:/1.png",0);
               }
               //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);

               QTime t;
               t.start();

               Rect roi;
               roi.x= areaRect[0].x;
               roi.y= areaRect[0].y;
               roi.width= areaRect[0].width;
               roi.height= areaRect[0].height;

               if(DEBUG)
               {
                   roi.x=0;
                   roi.y=0;
                   roi.width=2500;
                   roi.height=1900;
               }

               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage",fileName);

               detectWork(Mat(m_camImage),roi,param,16);

               emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var1[29]);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


                       QFile::rename("D:/savedImag/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[29]).arg(picSuff));

                       if(param.var1[29]>0)
                       {
                           emit addLog("有异物","");
                       }
                       else
                       {
                           emit addLog("无异物","");
                       }



                   }
               }

           }else if(method == 30){//华东小垫片角度检测

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<21) inputList.append("");


               //start....

               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];


               //检测0,0,800,1200,19,361,272,50,2,50,200,19,323,421,73,0
               params param;
               param.var1[0]=inputList[0].toInt();//中心点
               param.var1[1]=inputList[1].toInt();
               param.var1[2]=inputList[2].toInt();//最小size
               param.var1[3]=inputList[3].toInt();//最小size
               param.var1[4]=inputList[4].toInt();//二值
               param.var1[5]=inputList[5].toInt();//长
               param.var1[6]=inputList[6].toInt();//宽
               param.var1[7]=inputList[7].toInt();//长宽偏差
               param.var2[0]=inputList[8].toDouble();//比例


               param.var1[10]=inputList[9].toInt();//最小size
               param.var1[11]=inputList[10].toInt();//最小size
               param.var1[12]=inputList[11].toInt();//二值
               param.var1[13]=inputList[12].toInt();//中心点距离
               param.var1[14]=inputList[13].toInt();//
               param.var1[15]=inputList[14].toInt();//偏差距离
               param.var2[1]=inputList[15].toDouble();//角度偏移


               param.var2[2]=inputList[16].toDouble();//比例,测量最大直径
               param.var2[3]=inputList[17].toInt();//二值
               param.var1[20]=inputList[18].toInt();//size
               param.var1[21]=inputList[19].toInt();//size
               param.var2[4]=0;//输出直径

               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
                   m_camImage=cvLoadImage("D:/1.png",0);


               QTime t;
               t.start();
               //Rect roi(482,382,450,450);
               Rect roi;
               if(!DEBUG)
               {
                   roi.x= areaRect[0].x;
                   roi.y= areaRect[0].y;
                   roi.width= areaRect[0].width;
                   roi.height= areaRect[0].height;
               }
               else
               {
                   roi.x= 3;
                   roi.y= 10;
                   roi.width= 1278-3;
                   roi.height= 946-10;

               }



               //保存检测图片
               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage1",fileName);


               detectWork(Mat(m_camImage),roi,param,19);
               if(param.var2[19]==360)
                   param.var2[19]=500;
               emit addLog(QString("角度检测耗时:%1 ms").arg(t.elapsed()),"");

               QFile::rename("D:/savedImage1/"+fileName+picSuff,QString("D:/savedImage1/%1_%2%3").arg(fileName).arg(QString::number(param.var2[19],'f',3)).arg(picSuff));


               if(param.var2[19]!=500)
               {
                   emit addLog(QString("角度:%1 直径%2").arg(QString::number(param.var2[19],'f',3)).arg(QString::number(param.var2[4],'f',3)),"");
               }
               else
               {
                   emit addLog("角度无法检测","");
               }



               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<2) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       //emit addLog(QTime::currentTime().toString("hh:mm:ss")+"视觉角度结果:"+retStr.remove(0,2),"");

                       //第二次发送
                       retStr= "??" + outputVarList[1] + "|" + QString::number(param.var2[4]);
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);




                   }
               }


           }else if(method == 31){//自定义检测

               if(camIndex==-1)
               {
                   emit addLog("请填写正确的视觉相机编号","");
                   if(!DEBUG) return;
               }
               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<10) inputList.append("");



               //start....

               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[camIndex])
               {
                   emit addLog(QString("相机%1异常正在重新连接...").arg(camIndex+1),"");
                   if(!DEBUG) return;
               }
               if(!DEBUG)
                   dectedNum1=fNum[camIndex];

               if(!DEBUG)
               {
                   mylock[camIndex].lock();
                   cvCopy(IpImgCache[camIndex],m_camImage);
                   mylock[camIndex].unlock();
               }
               else
                   m_camImage=cvLoadImage("D:/1.png",0);


               QTime t;
               t.start();
               //Rect roi(482,382,450,450);
               Rect roi;
               if(!DEBUG)
               {
                   roi.x= areaRect[0].x;
                   roi.y= areaRect[0].y;
                   roi.width= areaRect[0].width;
                   roi.height= areaRect[0].height;
               }
               else
               {
                   roi.x= 0;
                   roi.y= 0;
                   roi.width= 1280;
                   roi.height= 960;

               }



               int detectMethod=inputList[2].toInt();//方法
               int savePicIndex=inputList[3].toInt();//保存文件夹名
               int angleFlag=inputList[4].toInt();//画矩形
               int distance=inputList[5].toInt();//距离
               int doubleNum=inputList[6].toInt();//double个数,最后一个角度偏移不算
               int sendType=inputList[7].toInt();//数据发送类型 //1000以上
               int temp2=inputList[8].toInt();//保留
               if(doubleNum>0)
               {
                   if(doubleNum>inputList.size()-10)
                   {
                       emit addLog("double个数不正确","");
                       return;
                   }

               }

               params param;
               for(int i=9;i<inputList.size()-doubleNum-1&&i<30;i++)
                   param.var1[i-7]=inputList[i].toInt();
               for(int i=inputList.size()-doubleNum-1,j=0;i<inputList.size()-1&&j<19;i++,j++)
                   param.var2[j]=inputList[i].toDouble();

               param.var1[0]=inputList[0].toInt();//x
               param.var1[1]=inputList[1].toInt();//y
               param.var2[18]=inputList[inputList.size()-1].toDouble();//angleOffset

               //addLog(QString("last param %1").arg(param.var2[0]),"");
               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               if(savePicIndex>0&&savePicIndex<10)
                    savedDetectImg(m_camImage,QString("savedImage%1").arg(savePicIndex),fileName);

               detectWork(Mat(m_camImage),roi,param,detectMethod);

               if(savePicIndex>0&&savePicIndex<10)
                    QFile::rename(QString("D:/savedImage%1/%2%3").arg(savePicIndex).arg(fileName).arg(picSuff),QString("D:/savedImage%1/%2_%3%4").arg(savePicIndex).arg(fileName).arg(QString::number(param.var2[19],'f',3)).arg(picSuff));

               emit addLog(QString("工件检测耗时:%1 ms").arg(t.elapsed()),"");



               if(param.var2[19]!=360)
               {
                   emit addLog("检测结果:"+QString::number(param.var2[19],'f',3),"");
               }
               else
               {
                   emit addLog("工件无法检测","");
               }



               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<1) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                       QByteArray dataArray;
                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       if(sendType<1000)
                            mysendService(dataArray.data(),dataArray.length(),1);
                       else
                            visionSendRet(outputVarList,param,sendType);
                       QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+QString("_%1").arg(param.var2[19]);


                       if(param.var2[19]!=360&&savePicIndex>0&&savePicIndex<10)
                       {
                           Mat m3=Mat(m_camImage);
                           drawRect(m3,Point(param.var1[0],param.var1[1]),distance,param.var2[19],angleFlag,Scalar(0,0,0));
                           savedDetectImg(&IplImage(m3(roi)),QString("savedImage%1_ret").arg(savePicIndex),fileName);
                       }


                   }
               }


                      }else if(method == 32){//华东第3套角度检测

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<17) inputList.append("");


               //start....

               //相机重连
               static int dectedNum1=0;
               if(dectedNum1==fNum[0])
               {
                   emit addLog("相机1异常正在重新连接...","");
                   if(!DEBUG) return;
               }
               dectedNum1=fNum[0];


               //检测0,0,800,1200,19,361,272,50,2,50,200,19,323,421,73,0
               params param;
               param.var1[0]=inputList[0].toInt();//中心点
               param.var1[1]=inputList[1].toInt();
               param.var1[2]=inputList[2].toInt();//最小size
               param.var1[3]=inputList[3].toInt();//最小size
               param.var1[4]=inputList[4].toInt();//二值
               param.var1[5]=inputList[5].toInt();//
               param.var1[6]=inputList[6].toInt();//长
               param.var1[7]=inputList[7].toInt();//宽



               param.var2[2]=inputList[8].toDouble();//比例,测量最大直径
               param.var2[3]=inputList[9].toInt();//二值
               param.var1[20]=inputList[10].toInt();//size
               param.var1[21]=inputList[11].toInt();//size
               param.var1[22]=inputList[12].toInt();//isBlack
               param.var1[23]=inputList[13].toInt();//len偏差
               param.var1[24]=inputList[14].toInt();//x
               param.var1[25]=inputList[15].toInt();//y
               param.var2[0]=inputList[16].toDouble();//角度偏移


               param.var2[5]=0;//输出直径 标准 7.98
               //qDebug()<<"aaaa"<<param.var2[2]<<param.var2[3]<<param.var1[20]<<param.var1[21]<<param.var1[22]<<param.var1[23];
               if(!DEBUG)
               {
                   mylock[0].lock();
                   cvCopy(IpImgCache[0],m_camImage);
                   mylock[0].unlock();
               }
               else
                   m_camImage=cvLoadImage("D:/1.png",0);


               QTime t;
               t.start();
               //Rect roi(482,382,450,450);
               Rect roi;
               if(!DEBUG)
               {
                   roi.x= areaRect[0].x;
                   roi.y= areaRect[0].y;
                   roi.width= areaRect[0].width;
                   roi.height= areaRect[0].height;
               }
               else
               {
                   roi.x= 3;
                   roi.y= 10;
                   roi.width= 1278-3;
                   roi.height= 946-10;

               }



               //保存检测图片
               QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
               savedDetectImg(m_camImage,"savedImage1",fileName);


               detectWork(Mat(m_camImage),roi,param,21);
               if(param.var2[19]==360)
                   param.var2[19]=500;
               emit addLog(QString("角度检测耗时:%1 ms").arg(t.elapsed()),"");

               QFile::rename("D:/savedImage1/"+fileName+picSuff,QString("D:/savedImage1/%1_%2%3").arg(fileName).arg(QString::number(param.var2[19],'f',3)).arg(picSuff));

               if(param.var2[5]<5)
               {
                   savedDetectImg(m_camImage,"wrong",QString("%1_%2").arg(fileName).arg(param.var2[5]));

               }

               if(param.var2[19]!=500)
               {
                   emit addLog(QString("角度:%1 直径%2").arg(QString::number(param.var2[19],'f',3)).arg(QString::number(param.var2[5],'f',3)),"");
               }
               else
               {
                   emit addLog("角度无法检测","");
               }



               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<2) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
                       QByteArray dataArray;


                       dataArray = retStr.toLocal8Bit();

                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;

                       mysendService(dataArray.data(),dataArray.length(),1);
                       //emit addLog(QTime::currentTime().toString("hh:mm:ss")+"视觉角度结果:"+retStr.remove(0,2),"");

                       //第二次发送
                       retStr= "??" + outputVarList[1] + "|" + QString::number(param.var2[5]);
                       dataArray = retStr.toLocal8Bit();
                       dataArray[0] = CMD_WRITE_VAR;
                       dataArray[1] = 1;
                       mysendService(dataArray.data(),dataArray.length(),1);




                   }
               }


           }else if(method == 33){//arm检测

               QStringList inputList = tmpList[2].split(",");
               while (inputList.length()<3) inputList.append("");

               armRatio1=inputList[0].toDouble();//像素比
               armRatio2=inputList[1].toDouble();//像素比



               QString str="visual_001";
               mainWindow->sendVisualRes(str,"192.168.125.100");

               str="visual_002";
               mainWindow->sendVisualRes(str,"192.168.125.101");


               QStringList outputVarList = tmpList[3].split(",");
               while (outputVarList.length()<2) outputVarList.append("");
               if (!outputVarList[0].isEmpty()) {
                   if (serviceSocket[0]) {
                       armVarName1=outputVarList[0];
                       armVarName2=outputVarList[1];
//                                  QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
//                                  QByteArray dataArray;

//                                  dataArray = retStr.toLocal8Bit();

//                                  dataArray[0] = CMD_WRITE_VAR;
//                                  dataArray[1] = 1;

//                                  mysendService(dataArray.data(),dataArray.length(),1);

//                                  //第二次发送
//                                  retStr= "??" + outputVarList[1] + "|" + QString::number(param.var2[5]);
//                                  dataArray = retStr.toLocal8Bit();
//                                  dataArray[0] = CMD_WRITE_VAR;
//                                  dataArray[1] = 1;
//                                  mysendService(dataArray.data(),dataArray.length(),1);

                   }
               }


           }
}

void MainWindow::visionAssemble3(QStringList &tmpList, CvRect areaRect[], QStringList &areaRectId, int method, int camIndex)
{
    if(method == 34){//毛刺检测

        QStringList inputList = tmpList[2].split(",");
        while (inputList.length()<12) inputList.append("");

        //start....
        //emit addLog("开始工件检测:","");
        //相机重连
//        static int dectedNum1=0;
//        if(dectedNum1==fNum[0])
//        {
//            emit addLog("相机1异常正在重新连接...","");
//            if(!DEBUG) return;
//        }
//        dectedNum1=fNum[0];


        saveCache[camIndex]=true;
        for(int k=0;k<12;k++)
            visionInput[camIndex].var1[k]=inputList[k].toInt();


//        if(!DEBUG)
//        {
//            mylock[0].lock();
//            cvCopy(IpImgCache[0],m_camImage);
//            mylock[0].unlock();
//        }
//        else
//        {
//            m_camImage=cvLoadImage("D:/1.png",0);
//        }
        //m_camImage=cvLoadImage("F:/CPP/QT/opencvTest/image/aa.bmp",0);





//        QTime t;
//        t.start();


        visionRoi[camIndex].x= areaRect[0].x;
        visionRoi[camIndex].y= areaRect[0].y;
        visionRoi[camIndex].width= areaRect[0].width;
        visionRoi[camIndex].height= areaRect[0].height;



        if(DEBUG)
        {
            visionRoi[camIndex].x=0;
            visionRoi[camIndex].y=0;
            visionRoi[camIndex].width=847;
            visionRoi[camIndex].height=845;
        }





        //QString fileName=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        //savedDetectImg(m_camImage,"savedImage",fileName);
       // detectWork(Mat(m_camImage),roi,param,8);

       // emit addLog(QString("耗时:%1 ms").arg(t.elapsed()),"");


        QStringList outputVarList = tmpList[3].split(",");
        while (outputVarList.length()<1) outputVarList.append("");
        if (!outputVarList[0].isEmpty()) {
            if (serviceSocket[0]) {
                visionOutput[camIndex]=outputVarList[0];
//                QString retStr= "??" + outputVarList[0] + "|" + QString::number(param.var2[19],'f',3);
//                QByteArray dataArray;


//                dataArray = retStr.toLocal8Bit();

//                dataArray[0] = CMD_WRITE_VAR;
//                dataArray[1] = 1;

//                mysendService(dataArray.data(),dataArray.length(),1);
//                emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


//                QFile::rename("D:/savedImage/"+fileName+picSuff,QString("D:/savedImage/%1_%2%3").arg(fileName).arg(param.var2[19]).arg(picSuff));

//                if(param.var2[19]!=360)
//                {
//                    Mat m3=Mat(m_camImage);
//                    if(abs(param.var2[19])<90)
//                        rectangle(m3,Point(640,470),Point(740,650),Scalar(0,0,0),2);
//                    else
//                        rectangle(m3,Point(350,470),Point(480,650),Scalar(0,0,0),2);
//                    savedDetectImg(&IplImage(m3(roi)),"savedImage2",fileName);
//                }
//                else
//                {
//                    emit addLog("工件无法检测请更换","");
//                }

            }
        }


    }
}


void MainWindow::triggerValueChanged(QStringList idList){
    QMap<QString, bool> displayMap;

    foreach(QString id, idList) {
        try {
            qDebug()<<QString("SELECT id,type,code,operator,value,extraTrigger,code2,operator2,value2,extraTrigger2,code3,operator3,value3 FROM StationAction WHERE id='%1' AND stationId='%2'").arg(id, currentStationId);
            QSqlQuery query;
            query.exec(QString("SELECT id,type,code,operator,value,extraTrigger,code2,operator2,value2,extraTrigger2,code3,operator3,value3 FROM StationAction WHERE id='%1' AND stationId='%2'").arg(id, currentStationId));
            if(query.next()) {
                QSqlRecord record = query.record();

                bool checkResult = false, tempResult = false;
                QString value = "";

                if (signalTriggerCache.contains(record.value("code").toString())) {
                    value = signalTriggerCache.value(record.value("code").toString());
                } else {
                    value = varTriggerCache.value(record.value("code").toString());
                }
                checkResult = SysUtils::evalCondition(value, record.value("operator").toString(), record.value("value").toString());

                int extraTrigger = record.value("extraTrigger").toInt();
                if (/*checkResult && */extraTrigger && extraTrigger>0) {
                    value = "";
                    //                    if (record.value("code2").toString() == "Stack_IN_3") {
                    //                        qDebug(record.value("code2").toString().toLocal8Bit());
                    //                    }
                    if (signalTriggerCache.contains(record.value("code2").toString())) {
                        value = signalTriggerCache.value(record.value("code2").toString());
                    } else {
                        value = varTriggerCache.value(record.value("code2").toString());
                    }
                    tempResult = SysUtils::evalCondition(value, record.value("operator2").toString(), record.value("value2").toString());
                    switch(extraTrigger){
                    case 1: // 与
                        checkResult = (checkResult && tempResult);
                        break;
                    case 2: // 或
                        checkResult = (checkResult || tempResult);
                        break;
                    case 3: // 非
                        checkResult = (checkResult && !tempResult);
                        break;
                    }
                    extraTrigger = record.value("extraTrigger2").toInt();
                    if (/*checkResult && */extraTrigger && extraTrigger>0) {
                        value = "";
                        //                        qDebug(record.value("code3").toString().toLocal8Bit());
                        if (signalTriggerCache.contains(record.value("code3").toString())) {
                            value = signalTriggerCache.value(record.value("code3").toString());
                        } else {
                            value = varTriggerCache.value(record.value("code3").toString());
                        }
                        tempResult = SysUtils::evalCondition(value, record.value("operator3").toString(), record.value("value3").toString());
                        switch(extraTrigger){
                        case 1: // 与
                            checkResult = (checkResult && tempResult);
                            break;
                        case 2: // 或
                            checkResult = (checkResult || tempResult);
                            break;
                        case 3: // 非
                            checkResult = (checkResult && !tempResult);
                            break;
                        }
                    }
                }

                //                if (checkResult) {
                if (checkResult && record.value("type").toInt() == 100) {  // 复位动作
                    foreach(QString id, moduleMap.keys()){
                        ModuleObject *module = moduleMap[id];
                        if (module) {
                            displayMap[id] = !(module->isHidden);
                        }
                    }
                } else if (checkResult && record.value("type").toInt() == 31) {  // 测量保存动作
                    query.exec(QString("SELECT id,moduleId,type,value FROM StationActionDetail WHERE actionId='%1' ORDER BY type,value DESC").arg(id));
                    if (query.next()) {
                        QSqlRecord record = query.record();
                        bool b=getTMMeasure( record.value("value").toInt());
                        if(b)
                            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  保存测量数据" + QString::number(record.value("value").toInt()), QString());
                        else
                            emit addLog("保存测量数据失败基因士打开失败","");
                    }
                } else if(checkResult && record.value("type").toInt() == 34){//伺服电机


                    query.exec(QString("SELECT id,moduleId,type,value FROM StationActionDetail WHERE actionId='%1' ORDER BY type,value DESC").arg(id));
                    if (query.next()) {
                        QSqlRecord record = query.record();

                        QStringList tmpList = record.value("value").toString().split("|");

                        while (tmpList.length()<4) tmpList.append("");

                        QStringList outputVarList = tmpList[3].split(",");
                        while (outputVarList.length()<1) outputVarList.append("");

                        if(tmpList[1].toInt() == 0){  //距离移动

                            if(!outputVarList[0].isEmpty())
                            {

                                //qDebug()<<"aaaa";

                                //  static int cur_pos=s_val;
                                //  int val=1000000;
                                //  if(s_val>=-10000&&s_val<=10000)
                                //  {
                                //      val=s_val-cur_pos;
                                //      cur_pos=s_val;
                                //      QString str;
                                //      QByteArray dataArray;

                                //      str= QString("?%1|%2").arg(outputVarList[0]).arg(val);
                                //      dataArray = str.toLocal8Bit();
                                //      dataArray[0] = CMD_SERVO_DISTANCE;
                                //      servoSocket->writeDatagram(dataArray,  QHostAddress::LocalHost,8905);
                                //      emit addLog(QString("移动距离%1").arg(val),"");

                                //  }





                            }
                        }else if(tmpList[1].toInt() == 1){//参数配置

                            g_servo->on_pushButton_2_clicked();
                            addLog("伺服参数发送成功","");
                        }else if(tmpList[1].toInt() == 2){//动作1-10

                            g_servo->on_pushButton_9_clicked();
                            addLog("伺服动作发送成功","");
                        }else if(tmpList[1].toInt() == 3){//冲压动作

                            g_servo->on_pushButton_10_clicked();
                            addLog("冲压动作发送成功","");
                        }

                    }

                }else if(checkResult && record.value("type").toInt() == 35){//通用


                    query.exec(QString("SELECT id,moduleId,type,value FROM StationActionDetail WHERE actionId='%1' ORDER BY type,value DESC").arg(id));
                    if (query.next()) {
                        QSqlRecord record = query.record();

                        QStringList tmpList = record.value("value").toString().split("|");

                        while (tmpList.length()<4) tmpList.append("");

                        QStringList outputVarList = tmpList[3].split(",");
                        while (outputVarList.length()<1) outputVarList.append("");

                        if(tmpList[1].toInt() == 0){  //博技探针检测

                            int ret=0;

                            if(KnifeList.size()>0&&Knife2List.size()>0)
                            {
                                //emit addLog("开始探针检测","");


                                /*
                                  刀补最大不能超过0.02
                                  上下超出0.005开始刀补
                                  探针值越小，直径越大
                                */
                                double setVal=Knife2List[0].v2-Knife2List[0].v1+Knife2List[0].v6;//探针设定值
                                double cur_val=neddleValue[0];//实际值
                                double dif=(setVal-cur_val);//差值
                                double limitVal=Knife2List[0].v3;//偏差值
                                double knifeSetVal=Knife2List[0].v5;//刀补值

                                double curWork=Knife2List[0].v1+setVal-cur_val;//工件实际值

                                qDebug()<<setVal<<cur_val<<dif<<limitVal;
                                //qDebug()<<"test "<<KnifeList[0].name2<<KnifeList[0].name3<<Knife2List[0].name1<<Knife2List[0].v1<<Knife2List[0].xi;

                                m_ui->label->setText("当前值:");
                                m_ui->label_4->setText("上次值:");
                                m_ui->label_6->setText("相差值:");
                                m_ui->label_10->setText("上次差值:");

                                m_ui->labelMeasure1->setText(QString::number(curWork,'f',3));
                                m_ui->labelMeasure2->setText(QString::number(preNeddleValue[0],'f',3));
                                m_ui->labelMeasure3->setText(QString::number(dif,'f',3));
                                m_ui->labelMeasure4->setText(QString::number(preNeddleValue[1] ,'f',3));

                                static double knifeRecord=0;

                                QString logStr=QString("当前值:%1  相差值:%2").arg(curWork,0,'f',3).arg(dif,0,'f',3);

                                writeLog(logStr);
                                cpkArray.append(curWork);
                                QString fileName=QString("d:/dzicsrobot/cpk/%1.cpk").arg(QDate::currentDate().toString("yyyyMMdd"));
                                updateCpk(fileName,curWork);

                                qDebug()<<"dif"<<dif<<"limitVal"<<limitVal;
                                if(Knife2List[0].xi=="开"&&curWork<54.996&&curWork>54.976)
                                {

                                    if(dif<-limitVal||dif>limitVal)
                                    {



                                        //?Fanuc_A,11,AXIS_X_WEAR,0.01
                                        double k_val=dif<0?-knifeSetVal:knifeSetVal;
                                        k_val*=Knife2List[0].v4;
                                        QString data=QString("?%1,%2,%3,%4")
                                                .arg(KnifeList[0].name2)
                                                .arg(KnifeList[0].name3)
                                                .arg(Knife2List[0].name1)
                                                .arg(QString::number(k_val));
                                        qDebug()<<data;
                                        QByteArray b=data.toLocal8Bit();
                                        b[0]=FCMD_TOOL_OFFSET;
                                        serviceSocket[0]->writeDatagram(b,  QHostAddress::LocalHost, 9666);
                                        emit addLog(QString("发送刀补值:%1").arg(k_val),"");

                                        knifeRecord+=k_val;
                                        logStr=QString("刀补值:%1").arg(knifeRecord);
                                        writeLog(logStr);



                                    }

                                }
                                if(curWork<54.976||curWork>54.996)
                                {
                                    logStr=QString("差值异常:%1").arg(dif);
                                    writeLog(logStr);
                                    ret=1;
                                }

                                preNeddleValue[0]=curWork;
                                preNeddleValue[1]=dif;

                            }


                            if (!outputVarList[0].isEmpty()) {
                                if (serviceSocket[0]) {
                                    QString retStr= "??" + outputVarList[0] + "|" + QString::number(ret);
                                    QByteArray dataArray;

                                    dataArray = retStr.toLocal8Bit();

                                    dataArray[0] = CMD_WRITE_VAR;
                                    dataArray[1] = 1;

                                    mysendService(dataArray.data(),dataArray.length(),1);

                                    if(ret==1)
                                    {
                                        emit addLog("探针值异常","");
                                    }


                                }
                            }



                        }else if(tmpList[1].toInt() == 1){//华东数据转发

                            QStringList outputVarList = tmpList[3].split(",");
                            while (outputVarList.length()<1) outputVarList.append("");
                            if (!outputVarList[0].isEmpty()) {
                                if (serviceSocket[0]) {
                                    QString retStr= "??" + outputVarList[0] + "|" + value;
                                    QByteArray dataArray;


                                    dataArray = retStr.toLocal8Bit();

                                    int toRobot=outputVarList[0].right(1).toInt();

                                    dataArray[0] = CMD_WRITE_VAR;
                                    dataArray[1] = toRobot;

                                    mysendService(dataArray.data(),dataArray.length(),1);
                                    emit addLog(QString("数据:%1 转发到机器人%2").arg(value).arg(toRobot),"");



                                }
                            }


                        }else if(tmpList[1].toInt() == 2){//预留2


                        }

                    }

                }else if (checkResult && record.value("type").toInt() == 32) {  // 视觉分析动作
                    /*
                            视觉相机编号：相机管理->相机编号[整形]
                            视觉模板编号：固定摄像头->模板编号
                            输入参数列表：例 (模板编号,参数1,参数2) 1004,1,2
                            输出变量列表：x1,x2

                            update by zhugz 20160624
*/

                    query.exec(QString("SELECT id,moduleId,type,value,name FROM StationActionDetail WHERE actionId='%1' ORDER BY type,value DESC").arg(id));
                    if (query.next()) {
                        QSqlRecord record = query.record();
                        QStringList tmpList = record.value("value").toString().split("|");
                        // 视觉模板编号 视觉算法 输入参数列表 输出变量列表
                        while (tmpList.length()<4) tmpList.append("");
                        QStringList areaRectId=tmpList[0].split(",");
                        QSqlQuery query2;
                        QSqlRecord record2;
                        CvRect areaRect[10];
                        int camIndex=g_camera->getCamIndex(record.value("name").toString());
                        bool flag=false;
                        for(int i=0;i<areaRectId.size();i++)
                        {
                            qDebug(QString("SELECT leftUpX,leftUpY,rightDownX,rightDownY /*cp.id,cp.code,ct.id,ct.code*/ FROM CameraProperty cp,CameraTemplate ct WHERE solutionId='%1' AND ct.cameraId=cp.id AND cp.name='%2' AND ct.code='%3' ORDER BY ct.id").arg(currentStationId,record.value("moduleId").toString(),areaRectId[i]).toLocal8Bit());
                            query2.exec(QString("SELECT leftUpX,leftUpY,rightDownX,rightDownY /*cp.id,cp.code,ct.id,ct.code*/ FROM CameraProperty cp,CameraTemplate ct WHERE solutionId='%1' AND ct.cameraId=cp.id AND cp.name='%2' AND ct.code='%3' ORDER BY ct.id").arg(currentStationId,record.value("moduleId").toString(),areaRectId[i]));
                            if (query2.next()) {
                                flag=true;
                                record2 = query2.record();
                                areaRect[i]=cvRect(record2.value("leftUpX").toDouble(),record2.value("leftUpY").toDouble(),record2.value("rightDownX").toDouble() - record2.value("leftUpX").toDouble(),record2.value("rightDownY").toDouble() - record2.value("leftUpY").toDouble());

                            }
                        }

                        if(DEBUG) flag=true;

                        if(flag)
                        {

                            visionAssemble(tmpList,areaRect,areaRectId,tmpList[1].toInt(),camIndex);
                            visionAssemble2(tmpList,areaRect,areaRectId,tmpList[1].toInt(),camIndex);
                            visionAssemble3(tmpList,areaRect,areaRectId,tmpList[1].toInt(),camIndex);

                        }
                    }
                } else if (checkResult && record.value("type").toInt() == 33) {  // 称重动作
                    query.exec(QString("SELECT id,moduleId,type,value FROM StationActionDetail WHERE actionId='%1' ORDER BY type,value DESC").arg(id));
                    if (query.next()) {
                        QSqlRecord record = query.record();
                        //                            weightThread->startTask(1);
                        emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  开始称重" + QString::number(record.value("value").toInt()), QString());
                    }
                } else {
                    query.exec(QString("SELECT id,moduleId,type,value FROM StationActionDetail WHERE actionId='%1' ORDER BY type,value DESC").arg(id));
                    while(query.next()) {


                        QSqlRecord record = query.record();
                        //                        ModuleObject *module = moduleMap[record.value("moduleId").toString()];
                        //                        if (viewer.IsNull()) {
                        //                            viewer = module->aisShape->GetContext()->CurrentViewer();
                        //                        }
                        if (checkResult && record.value("type").toInt() == 1) { // 显示设定
                            if (record.value("value").toInt() == 0) {
                                if (!displayMap[record.value("moduleId").toString()]) {
                                    displayMap[record.value("moduleId").toString()] = false;
                                }
                                ////                                if (module->aisShape->GetContext()->IsDisplayed(module->aisShape)) {
                                //                                    module->aisShape->GetContext()->Erase(module->aisShape, false);
                                //                                    qDebug((module->name + " 隐藏").toLocal8Bit());
                                ////                                }
                            } else {
                                ////                                if (module->aisShape->GetContext()->IsDisplayed(module->aisShape)) {
                                ////                                    module->aisShape->GetContext()->Redisplay(module->aisShape, false);
                                ////                                } else {
                                //                                    module->aisShape->GetContext()->Display(module->aisShape, false);
                                ////                                }
                                //                                qDebug((module->name + " 显示").toLocal8Bit());
                                displayMap[record.value("moduleId").toString()] = true;
                            }
                        } else if (record.value("type").toInt() == 2) { // 显示切换
                            if (record.value("value").toInt() == 0) {
                                if (!displayMap[record.value("moduleId").toString()]) {
                                    displayMap[record.value("moduleId").toString()] = checkResult?false:true;
                                }
                                ////                                if (module->aisShape->GetContext()->IsDisplayed(module->aisShape)) {
                                //                                    module->aisShape->GetContext()->Erase(module->aisShape, false);
                                //                                    qDebug((module->name + " 隐藏").toLocal8Bit());
                                ////                                }
                            } else {
                                ////                                if (module->aisShape->GetContext()->IsDisplayed(module->aisShape)) {
                                ////                                    module->aisShape->GetContext()->Redisplay(module->aisShape, false);
                                ////                                } else {
                                //                                    module->aisShape->GetContext()->Display(module->aisShape, false);
                                ////                                }
                                //                                qDebug((module->name + " 显示").toLocal8Bit());
                                displayMap[record.value("moduleId").toString()] = checkResult?true:false;
                            }
                        } else if (checkResult && record.value("type").toInt() == 3) { // 轴运动
                            AxisObject *axis = axisMap[moduleMap[record.value("moduleId").toString()]->bindAxis];
                            //                                QStringList tmpList = record.value("value").toString().split(",");
                            ////                                axis->value = tmpList[0].toDouble();

                            //                                 startAxisAutoMove(axis, tmpList[0].toDouble(), tmpList[1].toDouble());

                            autoMoveMap[axis] = record.value("value").toString() + "," + QString::number(clock());

                            //                                autoMoveLastTime = clock();
                            //qDebug()<<record.value("id").toString();


                            axisAutoMoveTimerId = startTimer(40);

                        }
                    }
                }
                //                }
            }
        } catch(...) {
            emit addLog( QTime::currentTime().toString("hh:mm:ss") + "  动作执行错误", QString());
            // Ignore
        }
    }
    try {
        Handle_V3d_Viewer viewer;
        foreach(QString id, displayMap.keys()){

            ModuleObject *module = moduleMap[id];
            if (!module) {
                continue;
            }
            if (viewer.IsNull()) {
                viewer = module->aisShape->GetContext()->CurrentViewer();
            }
            if (displayMap[id]) {

                if (!module->aisShape->GetContext()->IsDisplayed(module->aisShape)) {
                    module->aisShape->GetContext()->Display(module->aisShape, false);
                }
            } else {

                if (module->aisShape->GetContext()->IsDisplayed(module->aisShape)) {
                    module->aisShape->GetContext()->Erase(module->aisShape, false);
                }
            }
        }

        if (!viewer.IsNull()) {
            viewer->Update();
        }
    }catch(...){
        // Ignore
    }
}

void MainWindow::calcTmValue(double val)
{


    if(val>10000)
        return;


    m_ui->checkDataLabel->setText(QString("%1 mm").arg(QString::number(val,'f',3)));
    m_ui->maodingLabel->setText(QString("%1 mm").arg(QString::number(preKnifeVal,'f',3)));
    preKnifeVal=val;

    double standard=29.913;
    m_ui->yuanxinjuLabel->setText(QString("%1 mm").arg(QString::number(standard,'f',3)));
    double difVal=abs(standard-val);

    m_ui->weightLabel->setText(QString("%1 mm").arg(QString::number(difVal,'f',3)));

}


void MainWindow::callVision(int camIndex,int num)
{


    //addLog(QString("结束%1 param %2").arg(num).arg(visionInput[camIndex].var1[3]),"");

    visionRoi[camIndex];

    for(int k=0;k<num;k++)
    {
        //if(visionInput[camIndex].var1[2]==200)
        {
            if(DEBUG)
            {
                Mat m1=imread("d:/1.png",0);
                detectWork(m1,visionRoi[camIndex],visionInput[camIndex],200);
                addLog(QString("num %1 ret %2").arg(k).arg(visionInput[camIndex].var1[29]),"");
            }
            else
                detectWork(Mat(IpImgArray[camIndex][k]),visionRoi[camIndex],visionInput[camIndex],200);
        }
    }

    QString retStr= "??" + visionOutput[camIndex] + "|" + QString::number(visionInput[camIndex].var1[29]);
    QByteArray dataArray;


    dataArray = retStr.toLocal8Bit();

    dataArray[0] = CMD_WRITE_VAR;
    dataArray[1] = 1;

    mysendService(dataArray.data(),dataArray.length(),1);
    emit addLog(QTime::currentTime().toString("hh:mm:ss")+"检测结果:"+retStr.remove(0,2),"");


}
