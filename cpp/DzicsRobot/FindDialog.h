#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QPlainTextEdit;
namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindDialog(QWidget *parent = 0);
    FindDialog(QPlainTextEdit *edit,QWidget *parent = 0);
    ~FindDialog();
    static bool isDialogShow;
//    static FindDialog *s_FindDialog;
//    static FindDialog* initFindDialog();
private slots:
    void checkFindUpDown();
    void showSetting(int);
    void on_pushButton_find_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_findr_clicked();
    void on_pushButton_replace_clicked();
    void on_pushButton_reolaceall_clicked();
    void on_pushButton_cancel2_clicked();


private:
    Ui::FindDialog *ui;
    bool isFindup;
    bool isCheckBig;
    QPlainTextEdit *textEdit;
    bool searchString(const QString& str);
    void closeFindDialog();
protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);
};

#endif // FINDDIALOG_H
