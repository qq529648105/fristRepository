#ifndef PARMETERSHOW_H
#define PARMETERSHOW_H

#include <QObject>
#include <QMap>
class QLabel;
class ControlProperty;

class ParmeterShow:public QObject
{
    Q_OBJECT
public:
    static ParmeterShow *parmeterShow;
    static ParmeterShow* initialize();
    explicit ParmeterShow(QObject *parent = 0);
    QMap<QLabel*,ControlProperty*>realControlMap;

    QMap<QString, QLabel*>variateLabelMap;
    QMap<QString, QLabel*>signalLabelMap;

    QList<QString> getParmeterList(int type);


private:
     void queryAllControl(QWidget *widget);

public slots:
    void loadParmeterControl(QWidget* widget);
    void showParmeterControl(QWidget* widget);
    void setParmeterValue(const QString &parName, const QString& value, int type);

};

#endif // PARMETERSHOW_H
