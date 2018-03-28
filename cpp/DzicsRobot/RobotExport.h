#ifndef ROBOTEXPORT_H
#define ROBOTEXPORT_H

#include <QDialog>



class QListWidgetItem;

namespace Ui {
class RobotExport;
}

class RobotExport : public QDialog
{
    Q_OBJECT
    
public:
    explicit RobotExport(QWidget *parent = 0);
    ~RobotExport();
    void showRobot();

    static QString getChoseRobot(){return robotId;}

private slots:
    void on_pushButtonAffirm_clicked();
    void on_pushButtoncancel_clicked();

private:
    Ui::RobotExport *ui;
    static QString robotId;
private slots:
    void choseRobot(QListWidgetItem*);

};

#endif // ROBOTEXPORT_H
