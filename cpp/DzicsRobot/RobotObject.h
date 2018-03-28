#ifndef ROBOT0BJECT_H
#define ROBOT0BJECT_H

#include <QObject>

class RobotObject : public QObject
{
    Q_OBJECT
public:
    explicit RobotObject(QObject *parent = 0);
    QString id;
    QString name;
    int type;
    QString preview;
    int displayOrder;
    int comboIndex;
    
signals:
    
public slots:
    
};

#endif // ROBOT0BJECT_H
