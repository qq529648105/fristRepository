#include "DragForm.h"
#include "ui_DragForm.h"
#include <QMouseEvent>
#include <QDebug>

DragForm::DragForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DragForm)
{
    ui->setupUi(this);
    ui->labelSignal->setFrameShape(QFrame::Box);
    ui->labelVariate->setFrameShape(QFrame::Box);
    ui->labelText->setFrameShape(QFrame::Box);
    ui->labelImage->setFrameShape(QFrame::Box);

}

DragForm::~DragForm()
{
    delete ui;
}


void DragForm::mousePressEvent(QMouseEvent *event)
{

//    if (event->button() == Qt::LeftButton){
         dragStartPosition = event->pos();
         if(ui->labelSignal->geometry().contains(event->pos()))
            ui->labelSignal->setFrameShape(QFrame::WinPanel);
         else if(ui->labelVariate->geometry().contains(event->pos()))
             ui->labelVariate->setFrameShape(QFrame::WinPanel);
         else if(ui->labelText->geometry().contains(event->pos()))
             ui->labelText->setFrameShape(QFrame::WinPanel);
         else if(ui->labelImage->geometry().contains(event->pos()))
             ui->labelImage->setFrameShape(QFrame::WinPanel);
//         }
}

void DragForm::mouseMoveEvent(QMouseEvent *event)
{
    QString value;
    if(ui->labelSignal->geometry().contains(event->pos())){
        ui->labelSignal->setFrameShape(QFrame::Box);
        value = ui->labelSignal->text();
    }
    else if(ui->labelVariate->geometry().contains(event->pos())){
        ui->labelVariate->setFrameShape(QFrame::Box);
        value = ui->labelVariate->text();
    }
    else if(ui->labelText->geometry().contains(event->pos())){
        ui->labelText->setFrameShape(QFrame::Box);
        value = ui->labelText->text();
    }
    else if(ui->labelImage->geometry().contains(event->pos())){
        ui->labelImage->setFrameShape(QFrame::Box);
        value = ui->labelImage->text();
    }
    else
        return;
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(value);
    drag->setMimeData(mimeData);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);


}


void DragForm::mouseReleaseEvent(QMouseEvent *)
{

    ui->labelSignal->setFrameShape(QFrame::Box);
    ui->labelVariate->setFrameShape(QFrame::Box);
    ui->labelText->setFrameShape(QFrame::Box);
    ui->labelImage->setFrameShape(QFrame::Box);
}


