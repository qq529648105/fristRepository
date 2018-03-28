#ifndef ERRORCODEWIDGET_H
#define ERRORCODEWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include "SolutionSet.h"


typedef struct _ERRORINFO
{
    QString eCode;
    QString eTitle;
    QString eDesc;
    QString PriKey;

}ERRORINFO;

namespace Ui {
class ErrorCodeWidget;
}

class ErrorCodeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ErrorCodeWidget(QWidget *parent = 0);
    ~ErrorCodeWidget();
    
private:
    Ui::ErrorCodeWidget *ui;
    void InitTableWidget();
    void ReadInfo();
    void GetAllCode();
    QVector<ERRORINFO>erronList;
    SolutionSet *solutionSet;

private slots:
    void EditErrorItem(int,int);
    void GetErrorItem(int,int);

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void AddErrorCode();
    void RemoveErrorCode();
    /***********update8****************/
        void refreshErrorCode();
    /*************end***************/

    bool exportErrorCode(const QString& path);
    bool importErrorCode(const QString& path);

};

#endif // ERRORCODEWIDGET_H
