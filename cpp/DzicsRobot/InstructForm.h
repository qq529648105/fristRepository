#ifndef INSTRUCTFORM_H
#define INSTRUCTFORM_H

#include <QWidget>

namespace Ui {
class InstructForm;
}

class InstructForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit InstructForm(QWidget *parent = 0);
    ~InstructForm();
    
private slots:
    void on_pushButton_clear_clicked();
    void findMatchingStr();

    void on_pushButton_replace_clicked();

    void on_pushButton_forward_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_replaceall_clicked();

private:
    Ui::InstructForm *ui;
};

#endif // INSTRUCTFORM_H
