#ifndef PARMETERFORM_H
#define PARMETERFORM_H

#include <QWidget>
#include "property/QtVariantProperty"
#include "property/qteditorfactory.h"


class QLabel;
class ControlProperty;
class ParmeterShow;
namespace Ui {
class ParmeterForm;
}

class ParmeterForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParmeterForm(QWidget *parent = 0);
    ~ParmeterForm();
    
private:
    Ui::ParmeterForm *ui;
    QtVariantPropertyManager *variantManagerControl;
    QtProperty *topPropertyControl;
    QtBrowserItem *controlItem;
    QLabel *currentLabel;
    ParmeterShow * parmeterShow;
    ParmeterShow *s_parmeterShowEdit;
private slots:
    void editProperty(QLabel*);
    void controlPropertyChanged(QtProperty *, const QVariant &);
    void on_pushButtonRemove_clicked();
    void on_pushButtonSave_clicked();
    void on_pushButtonImage_clicked();
    void showParmeter();


public slots:
    void loadControl(QMap<QLabel*,ControlProperty*> &map,QWidget *widget);
public:
    void queryAllControl();

protected:
   void showEvent(QShowEvent *);
   void paintEvent(QPaintEvent *);
signals:
   void saveSignal(int, int);
};

#endif // PARMETERFORM_H
