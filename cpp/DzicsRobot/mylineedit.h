#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>
class mylineedit : public QLineEdit
{
    Q_OBJECT
public:
    explicit mylineedit(QWidget  *parent = 0);
    int index;
protected:
    virtual void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

signals:
    void clicked(int index);
    void focusOut();
public slots:


    
};

#endif // MYLINEEDIT_H
