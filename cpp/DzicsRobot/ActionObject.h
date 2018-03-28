#ifndef ACTIONOBJECT_H
#define ACTIONOBJECT_H

#include <QObject>

class ActionObject : public QObject
{
    Q_OBJECT
public:
    explicit ActionObject(QObject *parent = 0);
    ~ActionObject();
    QString id;
    QString stationId;
    QString name;
    int type;
    QString code;
    QString oper;
    QString value;
    int extTrg;
    QString code2;
    QString oper2;
    QString value2;
    int extTrg2;
    QString code3;
    QString oper3;
    QString value3;
    int count;
    int updateType;
signals:
public slots:
    
};

class ActionDetail : public QObject
{
    Q_OBJECT
public:
    ActionDetail(QObject *parent = 0);
    ~ActionDetail();
    QString id;
    QString actionId;
    QString name;
    int  type;
    QString moduleId;
    QString value;

};

#endif // ACTIONOBJECT_H
