#include "PageWork.h"
#include "ui_PageWork.h"
//#include "ui_WorkInfo.h"

PageWork::PageWork(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::PageWork)
{

//	QString lpRootPathName = "C:\\";
//    LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
//    DWORD nVolumeNameSize=12;// 卷标的字符串长度
//    DWORD VolumeSerialNumber;//硬盘序列号
//    DWORD MaximumComponentLength;// 最大的文件长度
//    LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
//    DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
//    DWORD FileSystemFlags;// 文件系统的一此标志
//    GetVolumeInformation((LPTSTR)lpRootPathName.utf16(),
//      lpVolumeNameBuffer, nVolumeNameSize,
//      &VolumeSerialNumber, &MaximumComponentLength,
//      &FileSystemFlags,
//      lpFileSystemNameBuffer, nFileSystemNameSize);

//	//if (VolumeSerialNumber != 1234) {
//	//	qApp->quit();
//	//	return;
//	//}


    m_ui->setupUi(this);

//	m_ui->label_4->setVisible(false);
//	m_ui->labelDis2->setVisible(false);
//	m_ui->label_10->setVisible(false);
//	m_ui->labelDis4->setVisible(false);

//    m_ui->label->setVisible(false);
//    m_ui->labelDis1->setVisible(false);
//    m_ui->label_6->setVisible(false);
//    m_ui->labelDis3->setVisible(false);


logListModel = new QStringListModel(logList);
m_ui->listMessage->setModel(logListModel);

//    m_ui->frame->setVisible(false);

//    startTimer(50);
viewer = new Viewer(this);
////connect(this, SIGNAL(syncPosition(double,double,double,double,double,double)), viewer, SLOT(syncPosition(double,double,double,double,double,double)));
//connect(this, SIGNAL(syncDisplayState( int, int )), viewer, SLOT(syncDisplayState( int, int )), Qt::QueuedConnection);
//connect(this, SIGNAL(syncUnitData(QByteArray)), viewer, SLOT(updateUnitData(QByteArray)), Qt::QueuedConnection);
//connect(this, SIGNAL(syncPosition(QStringList)), viewer, SLOT(syncPosition(QStringList)), Qt::QueuedConnection );

connect(viewer, SIGNAL(shapeSelected(QString)), this, SIGNAL(shapeSelected(QString)), Qt::QueuedConnection );
connect(viewer, SIGNAL(moduleAllLoaded(int)), this, SIGNAL(moduleAllLoaded(int)), Qt::QueuedConnection);
connect(viewer, SIGNAL(moduleAllLoaded(int)), this, SLOT(moduleAllLoadedSlot(int)), Qt::QueuedConnection);

connect(this, SIGNAL(stationChanged(QString)), viewer, SLOT(stationChanged(QString)), Qt::QueuedConnection );
connect(this, SIGNAL(importModuleFile(QString,QString,QString)), viewer, SLOT(importModuleFile(QString,QString,QString)), Qt::QueuedConnection );
connect(this, SIGNAL(moduleChanged(QString,bool)), viewer, SLOT(moduleChanged(QString,bool)), Qt::QueuedConnection );

connect(this->parent(), SIGNAL(syncPosition(QStringList)), viewer, SLOT(syncPosition(QStringList)), Qt::DirectConnection);
connect(this->parent(), SIGNAL(syncDisplayState( int, int )), viewer, SLOT(syncDisplayState( int, int )), Qt::DirectConnection);
connect(this->parent(), SIGNAL(showAll()), viewer, SLOT(showAll()), Qt::QueuedConnection);


//    PageWork *pageWork = (PageWork*)m_ui->stackedWidget->widget(0);
//    connect(workView, SIGNAL(updateAxisValue(QString,double)), this, SLOT(updateAxisValue(QString,double)));
//            m_ui->stackedWidget->addWidget(workView);
}

PageWork::~PageWork()
{
    delete m_ui;
}

void PageWork::keyPressed(int key)
{
//    workView->keyPressed( key );
}

void PageWork::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PageWork::timerEvent( QTimerEvent *e )
{
//    switch(e->timerId())
//    {
//        case 1:
            killTimer(e->timerId());
//            workView = new WorkView(this);
//            connect(workView, SIGNAL(updateAxisValue(QString,double)), this, SLOT(updateAxisValue(QString,double)));

viewer = new Viewer(this);
////connect(this, SIGNAL(syncPosition(double,double,double,double,double,double)), viewer, SLOT(syncPosition(double,double,double,double,double,double)));
//connect(this, SIGNAL(syncDisplayState( int, int )), viewer, SLOT(syncDisplayState( int, int )), Qt::QueuedConnection);
//connect(this, SIGNAL(syncUnitData(QByteArray)), viewer, SLOT(updateUnitData(QByteArray)), Qt::QueuedConnection);
//connect(this, SIGNAL(syncPosition(QStringList)), viewer, SLOT(syncPosition(QStringList)), Qt::QueuedConnection );

connect(viewer, SIGNAL(shapeSelected(QString)), this, SIGNAL(shapeSelected(QString)), Qt::QueuedConnection );
connect(viewer, SIGNAL(moduleAllLoaded(int)), this, SIGNAL(moduleAllLoaded(int)), Qt::QueuedConnection);
connect(viewer, SIGNAL(moduleAllLoaded(int)), this, SLOT(moduleAllLoadedSlot(int)), Qt::QueuedConnection);

connect(this, SIGNAL(stationChanged(QString)), viewer, SLOT(stationChanged(QString)), Qt::QueuedConnection );
connect(this, SIGNAL(importModuleFile(QString,QString,QString)), viewer, SLOT(importModuleFile(QString,QString,QString)), Qt::QueuedConnection );
connect(this, SIGNAL(moduleChanged(QString,bool)), viewer, SLOT(moduleChanged(QString,bool)), Qt::QueuedConnection );

//            QWidget *info = new QWidget(this);
//            info->setMaximumSize(QSize(16777215, 130));
//            Ui::WorkInfo *m_info = new Ui::WorkInfo;
//            m_info->setupUi(info);
//
//            m_ui->verticalLayout->addWidget(info);

 //           m_ui->verticalLayout->insertWidget(0, workView);


            m_ui->verticalLayout->insertWidget(0, viewer);
//            m_ui->verticalLayout->removeWidget(m_ui->widget);
//            m_ui->widget->deleteLater();

//            m_ui->frame->setVisible(true);

            //            m_ui->verticalLayout->addWidget();
//            m_ui->stackedWidget->setCurrentIndex(1);
//            break;

//    }
}

//std::map<QString, QLabel*> axisValueMap;
void PageWork::updateAxisValue ( QString axisSign, double value )
{
 /*   QLabel *axisName;
    QLabel *axisValue;
    axisValue = axisValueMap[axisSign];
    if (!axisValue) {
        axisName = new QLabel(m_ui->frame);
        axisName->setObjectName("axisName_"+axisSign);
        axisName->setMinimumSize(QSize(40, 0));

        QPalette palette;
        QBrush brush(QColor(0, 85, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        axisName->setPalette(palette);
        QFont font;
        font.setFamily("Monospace");
        font.setPointSize(15);
        axisName->setFont(font);
        axisName->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        axisName->setText(axisSign);

        axisValue = new QLabel(m_ui->frame);
        axisValue->setObjectName("axisValue_"+axisSign);
        axisValue->setMinimumSize(QSize(80, 0));
        axisValue->setPalette(palette);
        axisValue->setFont(font);
        axisValue->setText(QString().sprintf("%8.3f", value));

        m_ui->axisLayout->addRow(axisName, axisValue);
        axisValueMap[axisSign] = axisValue;

        QLabel *speedName = new QLabel(m_ui->frame);
        speedName->setObjectName("speedName_"+axisSign);
        speedName->setText(axisSign + tr("轴速度"));
        QLabel *speedValue = new QLabel(m_ui->frame);
        speedValue->setObjectName("speedValue_"+axisSign);
        speedValue->setText("500  (100%)");
        m_ui->speedLayout->addRow(speedName, speedValue);




    } else {
        QString text;
        text.sprintf("%8.3f", value);
        axisValue->setText(text);
    }*/

}

#define GET_BIT(a, b)	((a)>>(b)&1)

void PageWork::updateUnitData( QByteArray data )
{
    if (data[1] == (char)0x01) { // unit input/output signal 1
//		emit syncUnitData(data);
//		char t = data[2];
		
//        m_ui->labelInput1->setText(QString().sprintf("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", (uchar)data.at(2), (uchar)data[3], (uchar)data[4], (uchar)data[5], (uchar)data[6], (uchar)data[7], (uchar)data.at(8), (uchar)data.at(9), (uchar)data.at(10), (uchar)data.at(11), (uchar)data[12], (uchar)data[2 + 11], (uchar)data[2 + 12], (uchar)data[2 + 13], (uchar)data[2 + 14], (uchar)data[2 + 15]));
//	//	t = data[2 + 8];
//        m_ui->labelOutput1->setText(QString().sprintf("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", (uchar)data[2 + 16], (uchar)data[2 + 17], (uchar)data[2 + 18], (uchar)data[2 + 19], (uchar)data[2 + 20], (uchar)data[2 + 21], (uchar)data[2 + 22], (uchar)data[2 + 23], 0, 0, 0, 0, 0, 0, 0, 0));

//        m_ui->labelDis5->setText( QString().sprintf("%07.3fmm", (((uchar)data.at(8) << 16 | (uchar)data.at(7) << 8 | (uchar)data.at(6)) * 0.001 )));

//		m_ui->labelDis3->setText( QString().sprintf("%07.3f", (((uchar)data.at(10) & 0x03) << 8 | (uchar)data.at(9))  * 0.01 ));
//		m_ui->labelDis1->setText( QString().sprintf("%07.3f", (((uchar)data.at(11) & 0x0F) << 6 | ((uchar)data.at(10) & 0xFC) >> 2) * 0.01 ));
//		m_ui->labelDis4->setText( QString().sprintf("%07.3f", (((uchar)data.at(12) & 0x3F) << 4 | ((uchar)data.at(11) & 0xF0) >> 4) * 0.01 ));
//		m_ui->labelDis2->setText( QString().sprintf("%07.3f", (((uchar)data.at(13) & 0xFF) << 2 | ((uchar)data.at(12) & 0xC0) >> 6) * 0.01 ));

//		m_ui->labelDis5->setText( QString().sprintf("%07.3f", (((uchar)data.at(15)) << 8 | ((uchar)data.at(14))) * 0.01 ));

        m_ui->labelInput1->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput1->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));

    } else if (data[1] == (char)0x02) { // unit input/output signal 2
        m_ui->labelInput2->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput2->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));

        m_ui->labelDis5->setText( QString().sprintf("%07.3fmm", (((uchar)data.at(8) << 16 | (uchar)data.at(7) << 8 | (uchar)data.at(6)) * 0.001 )));

    } else if (data[1] == (char)0x03) { // unit input/output signal 3
        m_ui->labelInput3->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput3->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));
    } else if (data[1] == (char)0x04) { // unit input/output signal 4
        m_ui->labelInput4->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2], 0), GET_BIT((uchar)data[2], 1), GET_BIT((uchar)data[2], 2), GET_BIT((uchar)data[2], 3), GET_BIT((uchar)data[2], 4), GET_BIT((uchar)data[2], 5), GET_BIT((uchar)data[2], 6), GET_BIT((uchar)data[2], 7), GET_BIT((uchar)data[3], 0), GET_BIT((uchar)data[3], 1), GET_BIT((uchar)data[3], 2), GET_BIT((uchar)data[3], 3), GET_BIT((uchar)data[3], 4), GET_BIT((uchar)data[3], 5), GET_BIT((uchar)data[3], 6), GET_BIT((uchar)data[3], 7) ));
        m_ui->labelOutput4->setText(QString().sprintf("%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d", GET_BIT((uchar)data[2 + 16], 0), GET_BIT((uchar)data[2 + 16], 1), GET_BIT((uchar)data[2 + 16], 2), GET_BIT((uchar)data[2 + 16], 3), GET_BIT((uchar)data[2 + 16], 4), GET_BIT((uchar)data[2 + 16], 5), GET_BIT((uchar)data[2 + 16], 6), GET_BIT((uchar)data[2 + 16], 7), GET_BIT((uchar)data[3 + 16], 0), GET_BIT((uchar)data[3 + 16], 1), GET_BIT((uchar)data[3 + 16], 2), GET_BIT((uchar)data[3 + 16], 3), GET_BIT((uchar)data[3 + 16], 4), GET_BIT((uchar)data[3 + 16], 5), GET_BIT((uchar)data[3 + 16], 6), GET_BIT((uchar)data[3 + 16], 7) ));
    }
}

void PageWork::updateProgramRow( QByteArray data )
{
	int row = 0;
	row = (data[1]<<24) & (data[2]<<16) & (data[3]<<8) & data[4];
}

void PageWork::addLog(QString message, QString time)
{
//    qDebug(message.toLocal8Bit());
    if (time == "CLEARLOG") {
        logList.clear();
        logListModel->setStringList(logList);
    }

    logList += message.split("\r\n").first();
	logListModel->setStringList(logList);
	m_ui->listMessage->scrollToBottom();

}

void PageWork::updateSpeedRatio( int speedRatio )
{
    m_ui->speedLabel->setText(QString::number(speedRatio) + "%");
}

void PageWork::updateProcessTime( float sec )
{
    m_ui->processTimeLabel->setText( QString().sprintf("%07.3fs", sec) );
}

void PageWork::moduleAllLoadedSlot( int count )
{
//    m_ui->verticalLayout->insertWidget(0, viewer);
//    if (m_ui->label_7->isVisible()) {
//        qDebug("removeWidget");
        m_ui->verticalLayout->removeWidget(m_ui->widget);
        m_ui->label_7->setVisible(false);
//        m_ui->widget->deleteLater();
//    }
}
