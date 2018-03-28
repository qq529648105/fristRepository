#ifndef STACKTOOL_H
#define STACKTOOL_H

#include <QMainWindow>
#include <QShowEvent>

namespace Ui {
class StackTool;
}

struct stack{
    int stackType; //码垛类型 1 平面 2 线 3 盒子
    QString plain; // 平面
    QString line;  // 线
    QString box; // 盒子
    QString offset;// 偏移量
    QString name; // 初始名称
    int increasingDirection; // 递增方向 1 横向 2 纵向
    QString variableName; // 变量名
    int conditionOperator;// 条件操作符 1 >  ,2 >= , 3 = ,4 < ,5 <=
    QString variableValue; // 变量值
    int actionType; // 动作类型 1设定显示， 2显示切换
    int hideType; // 隐藏类型 1 影藏 ， 2显示
};

class StackTool : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit StackTool(QWidget *parent = 0);
    ~StackTool();

    void showEvent(QShowEvent *);

signals:
    void stackEnd(stack s);

private :
    void resetValue();

    void complete();
    
private slots:
    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_stackTypeCombo_currentIndexChanged(const QString &arg1);

private:
    Ui::StackTool *ui;
};

#endif // STACKTOOL_H
