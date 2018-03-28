#ifndef CONTROLPROPERTY_H
#define CONTROLPROPERTY_H

#include <QObject>

class ControlProperty : public QObject
{
    Q_OBJECT
public:
    explicit ControlProperty(QObject *parent = 0);
    QString id;
    QString name;
    QString value;
    QString unit;
    int type;
    int startX;
    int startY;
    int width;
    int height;
    int color;
    QString fontFamily;
    int fontSize;
    int fontItalic;
    int fontStrikeout;
    int fontBold;
    int fontUnderline;
    int aligan;
    QByteArray image;
    
signals:
    
public slots:
    
};

#endif // CONTROLPROPERTY_H
