#ifndef PROJECT4_H
#define PROJECT4_H

#include <QObject>
#include "Global.h"
class project4 : public QObject
{
    Q_OBJECT
public:
    explicit project4(QObject *parent = 0);
    void defect(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_LLine(Mat &src,Mat &rgb,Rect roi,params& param);
    
signals:
    
public slots:
    
};

#endif // PROJECT4_H
