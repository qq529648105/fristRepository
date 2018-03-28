#ifndef REALPARFORM_H
#define REALPARFORM_H

#include <QWidget>

namespace Ui {
class RealParForm;
}

class RealParForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit RealParForm(QWidget *parent = 0);
    ~RealParForm();
    void queryAllControl();
private:
    Ui::RealParForm *ui;
    static bool isShow;
public slots:
    void saveParameter(int,int);
protected:
    void showEvent(QShowEvent *);
};

#endif // REALPARFORM_H
