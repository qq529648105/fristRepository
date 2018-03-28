#include "FindDialog.h"
#include "ui_FindDialog.h"
#include <QPlainTextEdit>
#include <QMessageBox>


//FindDialog* FindDialog::s_FindDialog = NULL;

//FindDialog* FindDialog::initFindDialog(){

//    return NULL;
//}

//bool FindDialog::isDialogShow = false;

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),isFindup(false),isCheckBig(false),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
//    this->setWindowTitle(tr("²éÕÒÌæ»»"));
    ui->radioButton_down->setChecked(true);
    ui->tabWidget->setCurrentIndex(0);
    connect(ui->radioButton_up,SIGNAL(clicked(bool)),this,SLOT(checkFindUpDown()));
    connect(ui->radioButton_up2,SIGNAL(clicked(bool)),this,SLOT(checkFindUpDown()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(showSetting(int)));
//    isDialogShow = true;
}



FindDialog::FindDialog( QPlainTextEdit *edit,QWidget *parent) :
    QDialog(parent,Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint),isFindup(false),isCheckBig(false),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("²éÕÒÌæ»»"));

    ui->radioButton_down->setChecked(true);
    this->textEdit = edit;
    ui->tabWidget->setCurrentIndex(0);
    if(textEdit->textCursor().hasSelection())
        ui->lineEdit_find->setText(textEdit->textCursor().selectedText());
    connect(ui->radioButton_up,SIGNAL(clicked(bool)),this,SLOT(checkFindUpDown()));
    connect(ui->radioButton_up2,SIGNAL(clicked(bool)),this,SLOT(checkFindUpDown()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(showSetting(int)));
//    isDialogShow = true;
}

FindDialog::~FindDialog()
{
//    isDialogShow = false;
    delete ui;
}

void FindDialog::on_pushButton_find_clicked()
{
    QString findStr= ui->lineEdit_find->text().trimmed();
    if(ui->checkBox_biglittle->isChecked())
        isCheckBig = true;
    else
        isCheckBig = false;
    if(ui->radioButton_up->isChecked())
        isFindup = true;
    else
        isFindup = false;
    if(!searchString(findStr))
        QMessageBox::warning(this,tr("²éÕÒÊ§°Ü"),tr("Î´ÕÒµ½£º")+ findStr, QMessageBox::tr("È·ÈÏ"));

}

void FindDialog::checkFindUpDown()
{
    if(ui->tabWidget->currentIndex() == 0){
        if(ui->radioButton_up->isChecked()){
            ui->radioButton_down->setChecked(false);
            isFindup = true;
        }
        else{
            ui->radioButton_down->setChecked(true);
            isFindup = false;
        }
    }
    if(ui->tabWidget->currentIndex()==1){
        if(ui->radioButton_up2->isChecked()){
            ui->radioButton_down2->setChecked(false);
            isFindup = true;
        }
        else{
            ui->radioButton_down2->setChecked(true);
            isFindup = false;
        }

    }
}


bool FindDialog::searchString(const QString& str)
{

    bool isFound;
    if(isFindup){
        isFound =  (isCheckBig)?textEdit->find(str,QTextDocument::FindBackward |QTextDocument:: FindCaseSensitively ):textEdit->find(str,QTextDocument::FindBackward);
        if(!isFound){
            QTextCursor cursor = textEdit->textCursor();
            cursor.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
            textEdit->setTextCursor(cursor);
            isFound =  (isCheckBig)?textEdit->find(str,QTextDocument::FindBackward |QTextDocument:: FindCaseSensitively ):textEdit->find(str,QTextDocument::FindBackward);
        }


    }
    else{
        isFound =  (isCheckBig)?textEdit->find(str,QTextDocument:: FindCaseSensitively ):textEdit->find(str);
        if(!isFound){
            QTextCursor cursor = textEdit->textCursor();
            cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
            textEdit->setTextCursor(cursor);
            isFound =  (isCheckBig)?textEdit->find(str,QTextDocument:: FindCaseSensitively ):textEdit->find(str);
        }

    }
    return isFound;

}

void FindDialog::on_pushButton_cancel_clicked()
{
    this->hide();
//    closeFindDialog();

}

void FindDialog::on_pushButton_findr_clicked()
{

    QString findStr= ui->lineEdit_find2->text().trimmed();
    if(ui->checkBox_biglittle2->isChecked())
        isCheckBig = true;
    else
        isCheckBig = false;
    if(ui->radioButton_up2->isChecked())
        isFindup = true;
    else
        isFindup = false;
    if(!searchString(findStr))
       QMessageBox::warning(this,tr("²éÕÒÊ§°Ü"),tr("Î´ÕÒµ½£º")+ findStr, QMessageBox::tr("È·ÈÏ"));

}

void FindDialog::on_pushButton_replace_clicked()
{
    QTextCursor cursor = textEdit->textCursor();
    if(cursor.hasSelection()){
        cursor.insertText(ui->lineEdit_replace->text().trimmed());
    }
    searchString(ui->lineEdit_find2->text().trimmed());
}

void FindDialog::on_pushButton_reolaceall_clicked()
{
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
    textEdit->setTextCursor(cursor);
    if(ui->checkBox_biglittle2->isChecked()){
        while(textEdit->find(ui->lineEdit_find2->text().trimmed(),QTextDocument::FindBackward | QTextDocument::FindCaseSensitively)){
            cursor = textEdit->textCursor();
            if(cursor.hasSelection()){
                cursor.insertText(ui->lineEdit_replace->text().trimmed());
            }

        }
    }else{
        while(textEdit->find(ui->lineEdit_find2->text().trimmed(),QTextDocument::FindBackward)){
            cursor = textEdit->textCursor();
            if(cursor.hasSelection()){
                cursor.insertText(ui->lineEdit_replace->text().trimmed());
            }

        }
    }
}


void FindDialog::showSetting(int index)
{
    switch(index){
    case 0:
        ui->lineEdit_find->setText(ui->lineEdit_find2->text().trimmed());
        if(isFindup){
            ui->radioButton_down->setChecked(false);
            ui->radioButton_up->setChecked(true);
        }else{
            ui->radioButton_down->setChecked(true);
            ui->radioButton_up->setChecked(false);
        }
        if(isCheckBig)
            ui->checkBox_biglittle->setChecked(true);
        else
            ui->checkBox_biglittle->setChecked(false);
        break;
    case 1:
        ui->lineEdit_find2->setText(ui->lineEdit_find->text().trimmed());
        if(isFindup){
            ui->radioButton_down2->setChecked(false);
            ui->radioButton_up2->setChecked(true);
        }else{
            ui->radioButton_down2->setChecked(true);
            ui->radioButton_up2->setChecked(false);
        }
        if(isCheckBig)
            ui->checkBox_biglittle2->setChecked(true);
        else
            ui->checkBox_biglittle2->setChecked(false);
        break;
    default:
        break;
    }

}

void FindDialog::on_pushButton_cancel2_clicked()
{
    this->hide();
//  closeFindDialog();
}


void FindDialog::closeFindDialog()
{
//    isDialogShow = false;
    QTextCursor cursor = textEdit->textCursor();
    if(cursor.hasSelection()){
        cursor.clearSelection();
        textEdit->setTextCursor(cursor);
    }
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    this->done(0);

}


void FindDialog::closeEvent(QCloseEvent *event)
{
//    closeFindDialog();
//    event->accept();
}

void FindDialog::showEvent(QShowEvent *)
{
    ui->tabWidget->setCurrentIndex(0);
    ui->lineEdit_find->selectAll();
//    ui->lineEdit_find->setFocus();
}
