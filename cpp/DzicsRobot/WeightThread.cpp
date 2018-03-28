#include "WeightThread.h"
#include <SerialCom/qextserialport.h>
#include <QDebug>
#include <QTime>

union{
    float value;
    unsigned char array[4];
}data;


 unsigned char cmd[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x02,0xc4,0x0b};

//a001
unsigned int CRC16Check ( uchar *arr_buff, uchar len)
{
    uint crc=0xFFFF;
    uchar i, j;
    for ( j=0; j<len;j++)
    {
        crc=crc ^*arr_buff++;
        for ( i=0; i<8; i++)
        {
            if((crc&0x0001) >0)
            {
                crc=crc>>1;
                crc=crc^0xa001;
            }
            else
                crc=crc>>1;
        }
    }
    return (crc);
}



WeightThread::WeightThread():interMsec(100)
{
    serialCom = QextSerialPort::InitSerial();
    recvOutMsec= 1000;
    interTime = new QTime();
    recvTime = new QTime();
}

bool WeightThread::openSerial(QString& comName, int bandRet)
{
    if(serialCom->OpenSerial(comName,bandRet)){
        connect(serialCom,SIGNAL(readyRead()),this,SLOT(getSerialData()));
        return true;
    }
    else
        return false;
}

void WeightThread::getSerialData()
{
    interTime->restart();
}



void WeightThread::startWeightThread()
{
    isThreadStart = true;
    start();
}


void WeightThread::stopWeightThread()
{
    isThreadStart = false;

}

int WeightThread::sendSerialCmd(unsigned char *cmd, int cmdLen)
{
     int wNum = serialCom->WriteSerial((char*)cmd,cmdLen);
     int isData = readSerailData(readBuf);
//     for(int i =0;i<isData;i++){
//         printf("%x ",readBuf[i]);
//     }
//     printf("\n");
     if(isData >0)
        praseReadData(readBuf, isData);
     return 0;
}

int WeightThread::readSerailData(char *readBuf){
    recvTime->restart();
    interTime->restart();
    int recvNum = 0;
    memset(readBuf,0,sizeof(readBuf));
    for(;;){
        char buf[1024]={0};
        int reNum = serialCom->ReadSerial(buf);
        if(reNum > 0){
            if(recvNum ==0)
                memcpy(readBuf,buf,reNum);
            else
                memcpy(&readBuf[recvNum],buf,reNum);
            recvNum+= reNum;

        }
        if(recvTime->elapsed()>recvOutMsec)
        {
            return 0;
        }
        if(interTime->elapsed()>interMsec){

            return recvNum;
        }
        usleep(100);
    }

}

int WeightThread::praseReadData(char *buf, int bufLen)
{

    unsigned short crcSum  = CRC16Check((unsigned char*)buf,bufLen-2);
    if(crcSum>>8 == (unsigned char)buf[bufLen-1] && crcSum%256 == (unsigned char)buf[bufLen-2])
    {/*
        unsigned int value = buf[3]<<8 ||buf[4];
        qDebug()<<value<<"fff";
        int value1 = value/10;
        float realValue = float((value1%value)/10) +value1;*/
        data.array[0] = (unsigned char)buf[4];
        data.array[1] = (unsigned char)buf[3];
        data.array[2]= (unsigned char)buf[6];
        data.array[3]= (unsigned char)buf[5];
        float realValue= data.value;
        if(realValue < 5.0)
            realValue = 0.0;
//        qDebug()<<realValue<<"ddddddddd";
//        printf("%02x %02x %02x %02x",(unsigned char)buf[3],(unsigned char)buf[4],(unsigned char)buf[5],(unsigned char)buf[6]);
        emit sendValue(realValue);
        return 0;
    }else
        return -1;

}


void WeightThread::setRecvTimeOut(int msec)
{
    recvOutMsec = msec;  //
}



void WeightThread::run()
{

    if(taskId== 1){
       for(;;){
        sendSerialCmd(cmd, 8);
        msleep(250);
       }
    }

}




void WeightThread::startTask(int id)
{
    taskId  = id;
    if(!isRunning())
        start();

}


