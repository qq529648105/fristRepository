#ifndef PAGEWORK_H
#define PAGEWORK_H

#include <QtGui/QWidget>
#include <QStringListModel>

//#include "WorkView.h"
#include "Viewer.h"

namespace Ui {
    class PageWork;
}

class PageWork : public QWidget {
    Q_OBJECT
public:
    PageWork(QWidget *parent = 0);
    ~PageWork();
    void keyPressed(int key);
    Viewer *viewer;

protected:
    void changeEvent( QEvent *e );
    void timerEvent( QTimerEvent *e );

private:
    Ui::PageWork *m_ui;

QStringList logList;
QStringListModel *logListModel;

//    WorkView *workView;

public slots:
    void updateAxisValue ( QString axisSign, double value );
	void updateUnitData( QByteArray data );
	void updateProgramRow( QByteArray data );
	void addLog(QString message, QString time);
    void updateSpeedRatio( int speedRatio );
    void updateProcessTime( float sec );
    void moduleAllLoadedSlot( int count );

signals:
	//void syncPosition(double axis1, double axis2, double axis3, double axis4, double axis5, double axis6 );
    void moduleAllLoaded( int count );

    void syncPosition( QStringList posList );
	void syncUnitData( QByteArray data );
	void syncDisplayState( int displayState, int pickCount );

    void importModuleFile(QString filename, QString id, QString name);
    void stationChanged(QString stationId);
    void moduleChanged(QString id, bool isRemoved);

    void shapeSelected(QString currentModuleId);

};

#endif // PAGEWORK_H
