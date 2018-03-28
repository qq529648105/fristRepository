#include "StackTool.h"
#include "ui_StackTool.h"

#include <QRect>
#include <QPoint>

StackTool::StackTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StackTool)
{

    // 设置窗体样式
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

    // 定位
    QRect widgetRect=parent->rect();
    QPoint panelPos=QPoint(widgetRect.left(), widgetRect.top());
    panelPos=parent->mapToGlobal(panelPos);
    move(panelPos);

    ui->setupUi(this);



    // 设置初始不可用
    ui->lineTxt->setEnabled(false);
    ui->boxTxt->setEnabled(false);

}

StackTool::~StackTool()
{
    delete ui;
}

// 显示窗体的时候触发
void StackTool::showEvent(QShowEvent *){
    ui->stackedWidget->setCurrentWidget(ui->page);
    resetValue();
}

void StackTool::complete(){
    stack s;
    if(ui->stackTypeCombo->currentText() == "平面"){
        s.stackType = 1;
    }else if(ui->stackTypeCombo->currentText() == "线"){
        s.stackType = 2;
    }else{
        s.stackType = 3;
    }
    s.plain = ui->planTxt->text();
    s.line = ui->lineTxt->text();
    s.box = ui->boxTxt->text();
    s.offset = ui->offsetTxt->text();
    s.name = ui->nameTxt->text();
    if(ui->comboBox->currentText() == "横向"){
        s.increasingDirection = 1;
    }else{
        s.increasingDirection = 2;
    }
    s.variableName = ui->variableNameTxt->text();
    s.variableValue = ui->variableValTxt->text();

    if(ui->conditionOptCombo->currentText() == ">"){
        s.conditionOperator = 1;
    }else if(ui->conditionOptCombo->currentText() == ">="){
        s.conditionOperator = 2;
    }else if(ui->conditionOptCombo->currentText() == "="){
        s.conditionOperator = 3;
    }else if(ui->conditionOptCombo->currentText() == "<"){
        s.conditionOperator = 4;
    }else if(ui->conditionOptCombo->currentText() == "<="){
        s.conditionOperator = 5;
    }

    if(ui->actionTypeCombo->currentText() == "显示设定"){
        s.actionType = 1;
    }else{
        s.actionType = 2;
    }

    if(ui->hideOptCombo->currentText() == "影藏"){
        s.hideType = 1;
    }else{
        s.hideType = 2;
    }

    emit stackEnd(s);
}

// 重置值
void StackTool::resetValue(){
    ui->actionTypeCombo->setCurrentIndex(0);
    ui->boxTxt->setText("");
    ui->comboBox->setCurrentIndex(0);
    ui->conditionOptCombo->setCurrentIndex(0);
    ui->hideOptCombo->setCurrentIndex(0);
    ui->lineTxt->setText("");
    ui->nameTxt->setText("");
    ui->offsetTxt->setText("");
    ui->planTxt->setText("");
    ui->stackTypeCombo->setCurrentIndex(0);
    ui->variableNameTxt->setText("");
    ui->variableValTxt->setText("");
}

// 下一步点击
void StackTool::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


// 取消点击
void StackTool::on_pushButton_9_clicked()
{
    this->hide();
}

// 确定点击
void StackTool::on_pushButton_10_clicked()
{

}

// 取消点击
void StackTool::on_pushButton_11_clicked()
{
    this->hide();
}

// 码垛类型切换
void StackTool::on_stackTypeCombo_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "平面"){
        ui->lineTxt->setEnabled(false);
        ui->boxTxt->setEnabled(false);
        ui->planTxt->setEnabled(true);
    }else if(arg1 == "线"){
        ui->lineTxt->setEnabled(true);
        ui->boxTxt->setEnabled(false);
        ui->planTxt->setEnabled(false);
    }else{
        ui->lineTxt->setEnabled(false);
        ui->boxTxt->setEnabled(true);
        ui->planTxt->setEnabled(false);
    }
}
