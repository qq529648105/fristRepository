#include "MainWindow_all.h"
void MainWindow::readRapidFromPythonService() //读取信息
{

    while (pythonSocket->hasPendingDatagrams()) {

        QString programName;
        QByteArray result;
        result.resize(pythonSocket->pendingDatagramSize());

        //qDebug()<<"开始读取python计算的工件标定结果";
        pythonSocket->readDatagram(result.data(), result.size());

        int pos = result.indexOf('|');
        int len = result.length();

        uchar command = result[pos-1];
        if(command == '*')
        {
            qDebug()<<"cmd_rapid_end";
            emit addLog("从workObjCal.exe读取rapid指令结束","");
        }else
        {
           programName = QString::fromUtf8(result.mid(pos+1), len - pos-2);
        }
        m_ui->textEdit->appendPlainText(programName);
    }
    //m_ui->textEdit->setPlainText("start");

    //qDebug()<<programName;
    //m_ui->textEdit->setPlainText("end");
//    QString rapidData;
//    while(m_tcpSocket->bytesAvailable()>0)
//        {
//            QByteArray datagram;
//            datagram.resize(tcpSocket->bytesAvailable());
//            tcpSocket->read(datagram.data(),datagram.size());
//            rapidData += datagram.data();
//            //statusLabel->setText(msg);
//
//            //getFortuneButton->setEnabled(true);
//        }
//    emit updateProgram(QString);
}
//end



//rong lk
int MainWindow::sendToPythonService(const char *data,int len)
{
    int r = -1;
    r = pythonSocket->writeDatagram(data, len, QHostAddress::LocalHost, 8812);
    return r;
}
//end


//rong lk
void MainWindow::vertexPointSelectSlot()
{
    if(btn_vertexPointSelect->isChecked())
    {
        diableOtherSelectButton(PntSelectType);
        pageViewer->setGeometricElementSelectMode(0);
    }else{
        pageViewer->cancelGeometricElementSelectMode();
        enableOtherSelectButton(PntSelectType);
    }
}
void MainWindow::anyPointSelectSlot()
{
    if(btn_anyPointSelect->isChecked())
    {
        diableOtherSelectButton(AnyPntSelectType);
        pageViewer->setGeometricElementSelectMode(1);

    }else{
        enableOtherSelectButton(AnyPntSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}
void MainWindow::endPointSelectSlot()
{
    if(btn_endPointSelect->isChecked())
    {
        diableOtherSelectButton(EndPntSelectType);
        pageViewer->setGeometricElementSelectMode(2);
    }else{
        enableOtherSelectButton(EndPntSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}
void MainWindow::middlePointSelectSlot()
{
    if(btn_middlePointSelect->isChecked())
    {
        diableOtherSelectButton(MiddlePntSelectType);
        pageViewer->setGeometricElementSelectMode(3);
    }else{
        enableOtherSelectButton(MiddlePntSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}
void MainWindow::centrePointSelectSlot()
{
    if(btn_centrePointSelect->isChecked())
    {
        diableOtherSelectButton(CircleCenterPntSelectType);
        pageViewer->setGeometricElementSelectMode(4);
    }else{
        enableOtherSelectButton(CircleCenterPntSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}

void MainWindow::edgePointSelectSlot()
{
    if(btn_edgePointSelect->isChecked())
    {
        diableOtherSelectButton(EdgePntSelectType);
        pageViewer->setGeometricElementSelectMode(5);
    }else{
        enableOtherSelectButton(EdgePntSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}

void MainWindow::surfaceSelectSlot()
{
    if(btn_surfaceSelect->isChecked())
    {
        diableOtherSelectButton(FaceSelectType);
        pageViewer->setGeometricElementSelectMode(6);
    }else{
        enableOtherSelectButton(FaceSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}

void MainWindow::edgeSelectSlot()
{
    if(btn_edgeSelect->isChecked())
    {
        diableOtherSelectButton(LinSelectType);
        pageViewer->setGeometricElementSelectMode(7);
    }else{
        enableOtherSelectButton(LinSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}
void MainWindow::componentSelectSlot()
{
    if(btn_componentSelect->isChecked())
    {
        diableOtherSelectButton(ComponentSelectType);
        pageViewer->setGeometricElementSelectMode(9);
    }else{
        enableOtherSelectButton(ComponentSelectType);
        pageViewer->cancelGeometricElementSelectMode();
    }
}
void MainWindow:: cancelButtonDownSignal()
{
    btn_unlock->setDown(false);
    btn_vertexPointSelect->setDown(false);
    btn_anyPointSelect->setDown(false);
    btn_endPointSelect->setDown(false);
    btn_middlePointSelect->setDown(false);
    btn_centrePointSelect->setDown(false);
    btn_surfaceSelect->setDown(false);
    btn_edgePointSelect->setDown(false);
    btn_edgeSelect->setDown(false);
    btn_componentSelect->setDown(false);
}
void MainWindow::diableOtherSelectButton(int selectMode)
{
    switch(selectMode){
        case PntSelectType  :
            btn_anyPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case AnyPntSelectType  :
            btn_vertexPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case EndPntSelectType :
            btn_vertexPointSelect->setEnabled(false);
            btn_anyPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case MiddlePntSelectType :
            btn_vertexPointSelect->setEnabled(false);
            btn_anyPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case CircleCenterPntSelectType  :
            btn_vertexPointSelect->setEnabled(false);
            btn_anyPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case EdgePntSelectType  :
            btn_vertexPointSelect->setEnabled(false);
            btn_anyPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case FaceSelectType  :
            btn_vertexPointSelect->setEnabled(false);
            btn_anyPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case LinSelectType  :
            btn_vertexPointSelect->setEnabled(false);
            btn_anyPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_componentSelect->setEnabled(false);
            break;
        case ComponentSelectType :
            btn_vertexPointSelect->setEnabled(false);
            btn_anyPointSelect->setEnabled(false);
            btn_endPointSelect->setEnabled(false);
            btn_middlePointSelect->setEnabled(false);
            btn_centrePointSelect->setEnabled(false);
            btn_surfaceSelect->setEnabled(false);
            btn_edgePointSelect->setEnabled(false);
            btn_edgeSelect->setEnabled(false);
            break;
    }
}
void MainWindow::enableOtherSelectButton(int selectMode)
{
    switch(selectMode){
        case PntSelectType  :
            btn_anyPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case AnyPntSelectType  :
            btn_vertexPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case EndPntSelectType :
            btn_vertexPointSelect->setEnabled(true);
            btn_anyPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case MiddlePntSelectType :
            btn_vertexPointSelect->setEnabled(true);
            btn_anyPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case CircleCenterPntSelectType  :
            btn_vertexPointSelect->setEnabled(true);
            btn_anyPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case EdgePntSelectType  :
            btn_vertexPointSelect->setEnabled(true);
            btn_anyPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case FaceSelectType  :
            btn_vertexPointSelect->setEnabled(true);
            btn_anyPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case LinSelectType  :
            btn_vertexPointSelect->setEnabled(true);
            btn_anyPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_componentSelect->setEnabled(true);
            break;
        case ComponentSelectType :
            btn_vertexPointSelect->setEnabled(true);
            btn_anyPointSelect->setEnabled(true);
            btn_endPointSelect->setEnabled(true);
            btn_middlePointSelect->setEnabled(true);
            btn_centrePointSelect->setEnabled(true);
            btn_surfaceSelect->setEnabled(true);
            btn_edgePointSelect->setEnabled(true);
            btn_edgeSelect->setEnabled(true);
            break;
    }
}
//end
QString MainWindow::positionRand()
{
    //随机数
    int s1=5000;
    int s2=18000;

    //-50  50          -50  180
    //5000  18000
   // QString s="?11,-20,0,-30";
    qsrand(time(0));

    int b1=0;
    int a1,a2,a3,a4;
    b1=qrand()%2;

    a1=qrand()%s1;
    if(b1){a1*=-1;}

    b1=qrand()%2;

    a2=qrand()%s1;
    if(b1){a2*=-1;}

    b1=qrand()%2;

    a3=qrand()%s1;
    if(b1){a3*=-1;}

    b1=qrand()%2;

    a4=qrand()%s2;
    if(b1){a4*=-1;}

    return QString("?%1,%2,%3,%4").arg(QString::number(a1)).arg(QString::number(a2)).arg(QString::number(a3)).arg(QString::number(a4));

}
