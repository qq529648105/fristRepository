#include "InstructForm.h"
#include "ui_InstructForm.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QDebug>

InstructForm::InstructForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstructForm)
{
    ui->setupUi(this);
    ui->instructTextEdit->setStyleSheet("selection-background-color:lightblue;font: 14pt;");
    connect(ui->lineEdit_find,SIGNAL(textChanged(QString)),this,SLOT(findMatchingStr()));
}

InstructForm::~InstructForm()
{
    delete ui;
}

void InstructForm::on_pushButton_clear_clicked()
{
    ui->lineEdit_find->clear();
}

void InstructForm::findMatchingStr()
{
    QTextCursor cursor = ui->instructTextEdit->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::KeepAnchor);
    ui->instructTextEdit->setTextCursor(cursor);
    QString findStr= ui->lineEdit_find->text().trimmed();
    if(!ui->instructTextEdit->find(findStr)){
        QTextCursor cursor = ui->instructTextEdit->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        ui->instructTextEdit->setTextCursor(cursor);
        ui->instructTextEdit->find(findStr);
     }

}

void InstructForm::on_pushButton_replace_clicked()
{
    QTextCursor cursor = ui->instructTextEdit->textCursor();
    if(cursor.hasSelection()){
        cursor.insertText(ui->lineEdit_replace->text().trimmed());
    }
}

void InstructForm::on_pushButton_forward_clicked()
{
    QString findStr= ui->lineEdit_find->text().trimmed();
    if(!ui->instructTextEdit->find(findStr)){
        QTextCursor cursor = ui->instructTextEdit->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        ui->instructTextEdit->setTextCursor(cursor);
        ui->instructTextEdit->find(findStr);
    }

}

void InstructForm::on_pushButton_back_clicked()
{
    QString findStr= ui->lineEdit_find->text().trimmed();
    if(!ui->instructTextEdit->find(findStr,QTextDocument::FindBackward)){
        QTextCursor cursor = ui->instructTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
        ui->instructTextEdit->setTextCursor(cursor);
        ui->instructTextEdit->find(findStr,QTextDocument::FindBackward);}
}

void InstructForm::on_pushButton_replaceall_clicked()
{
    QTextCursor cursor = ui->instructTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
    ui->instructTextEdit->setTextCursor(cursor);
    while(ui->instructTextEdit->find(ui->lineEdit_find->text().trimmed(),QTextDocument::FindBackward)){
        cursor = ui->instructTextEdit->textCursor();
        if(cursor.hasSelection()){
            cursor.insertText(ui->lineEdit_replace->text().trimmed());
        }

    }

}
