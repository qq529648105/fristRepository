#ifndef EDITFORM_H
#define EDITFORM_H

#include <QWidget>
#include <QByteArray>
#include <QMap>
#include "ControlProperty.h"
#include "ParmeterShow.h"


namespace Ui {
class EditForm;
}

class QLabel;
class EditForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditForm(QWidget *parent = 0);
    ~EditForm();
     QLabel *editLabel;



    
private:
    Ui::EditForm *ui;
    bool isPressed;
    bool isMoving;
    QPoint lastPos;
    QPoint newPos;
    ParmeterShow *s_ParmeterShow;
   // QMap<QLabel*,ControlProperty*>controlMap;

public slots:
    void removeControl();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:
    void s_editProperty(QLabel *editLabel);


};

#endif // EDITFORM_H
