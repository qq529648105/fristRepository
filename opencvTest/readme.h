#ifndef README_H
#define README_H

#include <QMainWindow>

namespace Ui {
class readMe;
}

class readMe : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit readMe(QWidget *parent = 0);
    ~readMe();
    
private:
    Ui::readMe *ui;
};

#endif // README_H
