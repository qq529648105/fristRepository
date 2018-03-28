#ifndef STACKTOOL_H
#define STACKTOOL_H

#include <QMainWindow>
#include <QShowEvent>

namespace Ui {
class StackTool;
}

struct stack{
    int stackType; //������� 1 ƽ�� 2 �� 3 ����
    QString plain; // ƽ��
    QString line;  // ��
    QString box; // ����
    QString offset;// ƫ����
    QString name; // ��ʼ����
    int increasingDirection; // �������� 1 ���� 2 ����
    QString variableName; // ������
    int conditionOperator;// ���������� 1 >  ,2 >= , 3 = ,4 < ,5 <=
    QString variableValue; // ����ֵ
    int actionType; // �������� 1�趨��ʾ�� 2��ʾ�л�
    int hideType; // �������� 1 Ӱ�� �� 2��ʾ
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
