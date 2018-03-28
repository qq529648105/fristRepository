#ifndef DRAGFORM_H
#define DRAGFORM_H

#include <QWidget>

namespace Ui {
class DragForm;
}

class DragForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit DragForm(QWidget *parent = 0);
    ~DragForm();
    
private:
    Ui::DragForm *ui;
    QPointF dragStartPosition;
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
private slots:

};

#endif // DRAGFORM_H
