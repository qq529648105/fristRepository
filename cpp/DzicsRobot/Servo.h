#ifndef SERVO_H
#define SERVO_H

#include <QWidget>
#include <QTableWidgetItem>

namespace Ui {
class Servo;
}

class Servo : public QWidget
{
    Q_OBJECT
protected:


public:
    explicit Servo(QWidget *parent = 0);
    ~Servo();
    void setTabIndex(int index);
    void sendParams();

    void updateUi();
    void lockServo();
    void unlockServo();
    QStringList servoNameList();
    void tabwidget_show(int index);
    void tabwidget_hide(int index);
    int firsttabshow();
    QString cur_solutionId;
    QString cfg1,cfg2,cfg3,cfg4,cfg5;
public slots:
    void on_pushButton_9_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_10_clicked();
private slots:

    void setPosEdit(int index,double val);
    void setWidthEdit(int index,double val);
    void setHeightEdit(int index,double val);
    void setCurEdit(int type,int index,double val);
    void pressActionSlot();
    void on_pushButton_clicked();

    void setParam(QString btnName);

    void clickSlot();
    void pressSlot();
    void releaseSlot();
    void singleStep();
    void actionExec();

    void singleSend();
    void actionSend();

    void setPressFlag();


    void posChangeSlot();
    void widthChangeSlot();
    void checkWidth(int index);
    void checkPos(int index);

    void getdeviation();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_comboBox_BanjinModel_currentIndexChanged(int index);



    //void on_pushButton_7_clicked();

    void on_btnpyx_add_big_clicked();

    void on_btnpyx_add_small_clicked();

    void on_btnpyx_reduce_big_clicked();

    void on_btnpyx_reduce_small_clicked();

    void on_btnpyy_add_big_clicked();

    void on_btnpyy_add_small_clicked();

    void on_btnpyy_reduce_big_clicked();

    void on_btnpyy_reduce_small_clicked();

    void on_comboBox_pianyiX_currentIndexChanged(int index);





    void on_tableWidget_gjmodel_itemDoubleClicked(QTableWidgetItem *item);

    void on_btnpyz_add_big_clicked();

    void on_btnpyz_add_small_clicked();

    void on_btnpyz_reduce_big_clicked();

    void on_btnpyz_reduce_small_clicked();

private:
    Ui::Servo *ui;
    bool manualFlag,pressFlag;
    int manualIndex,actionIndex;
    void initUi();
    int allCount,editType,firstClick;
    double changeVal;
    double curRow[3];

};

#endif // SERVO_H
