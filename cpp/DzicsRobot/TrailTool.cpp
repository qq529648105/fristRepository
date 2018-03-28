#include "TrailTool.h"
#include "ui_TrailTool.h"
#include <QMessageBox>

TrailTool::TrailTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrailTool)
{

    // 设置窗体样式
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

    // 定位
    QRect widgetRect=parent->rect();
    QPoint panelPos=QPoint(widgetRect.left(), widgetRect.top());
    panelPos=parent->mapToGlobal(panelPos);
    move(panelPos);

    ui->setupUi(this);
}

TrailTool::~TrailTool()
{
    delete ui;
}



// 显示窗体的时候触发
void TrailTool::showEvent(QShowEvent *){
    ui->stackedWidget->setCurrentWidget(ui->page);
    this->setWindowTitle(tr("选择插补走法"));
    resetValue();
}

// 完成的时候调用
void TrailTool::complete(){
    // 正走
    if(ui->forwardRBtn->isChecked()){
        trail go;
        // 姿态角
        go.attitudeAngleX = ui->rxTxt->text().toDouble();
        go.attitudeAngleY = ui->ryTxt->text().toDouble();
        go.attitudeAngleZ = ui->rzTxt->text().toDouble();
        // 姿态角类型
        if(ui->attitudeTypeRBtn1->isChecked()){
            go.attitudeAngleType = 1;
        }else{
            go.attitudeAngleType = 2;
            // 切线方向 不变
            if(ui->tangentTypeRBtn1->isChecked()){
                go.tangentType = 1;
            }else{
                go.tangentType = 2;
            }
            // 面方向 不变
            if(ui->directionTypeRBtn1->isChecked()){
                go.faceType = 1;
            }
            // 保持
            else if(ui->directionTypeRBtn2->isChecked()){
                go.faceType = 2;
            }
            // 朝点
            else if(ui->directionTypeRBtn3->isChecked()){
                go.faceType = 3;
                go.attitudePointX = ui->lineXEditTxt->text().toDouble();
                go.attitudePointY = ui->lineYEditTxt->text().toDouble();
                go.attitudePointZ = ui->lineZEditTxt->text().toDouble();
            }
            // 朝轴
            else{
                go.faceType = 4;
                // 轴两点坐标
                go.attitudeAxisFirstX = ui->axisFirstXTxt->text().toDouble();
                go.attitudeAxisFirstY = ui->axisFirstYTxt->text().toDouble();
                go.attitudeAxisFirstZ = ui->axisFirstZTxt->text().toDouble();

                go.attitudeAxisSecondX = ui->axisSecondXTxt->text().toDouble();
                go.attitudeAxisSecondY = ui->axisSecondYTxt->text().toDouble();
                go.attitudeAxisSecondZ = ui->axisSecondZTxt->text().toDouble();
            }
        }
        emit interpolationGo(go);
    }
    // 反走
    else if(ui->backwardRBtn->isChecked()){
        trailback back;
        // 通用
        if(ui->generalRBtn->isChecked()){
            back.backType = 1;
            //姿态角
            back.attitudeAngleX = ui->backRxTxt->text().toDouble();
            back.attitudeAngleY = ui->backRyTxt->text().toDouble();
            back.attitudeAngleZ = ui->backRzTxt->text().toDouble();
            // 姿态角类型 不变
            if(ui->backAttitudeTypeRBtn1->isChecked()){
                back.attitudeAngleType = 1;
            }
            // 保持
            else{
                back.attitudeAngleType = 2;
            }
            // 接触点坐标
            back.contactPointX = ui->backTouchXTxt->text().toDouble();
            back.contactPointY = ui->backTouchYTxt->text().toDouble();
            back.contactPointZ = ui->backTouchZTxt->text().toDouble();
        }
        // 去毛刺
        else{
            back.backType = 2;
            // 姿态角类型 手动
            if(ui->handTypeRBtn->isChecked()){
                back.attitudeAngleType = 3;
                back.attitudeAngleX = ui->backRxTxt_2->text().toDouble();
                back.attitudeAngleY = ui->backRyTxt_2->text().toDouble();
                back.attitudeAngleZ = ui->backRzTxt_2->text().toDouble();
            }
            // 自动
            else{
                back.attitudeAngleType = 4;
                back.cutAngle = ui->backCutRadiusTxt->text().toDouble();
                if(ui->adapterForwardRBtn->isChecked()){
                    back.adapterType = 1;
                }else{
                    back.adapterType = 2;
                }
                if(ui->angleForwardRBtn->isChecked()){
                    back.inclinedAngleType = 1;
                }else{
                    back.inclinedAngleType = 2;
                }
            }
            // 法兰盘坐标
            back.flangePlateX = ui->plateXTxt->text().toDouble();
            back.flangePlateY = ui->plateYTxt->text().toDouble();
            back.flangePlateZ = ui->plateZTxt->text().toDouble();
            // 切割半径
            back.cutRadius = ui->cutRadiuTxt->text().toDouble();
            back.cutCenterX = ui->cutCenterXTxt->text().toDouble();
            back.cutCenterY = ui->cutCenterYTxt->text().toDouble();
            back.cutCenterZ = ui->cutCenterZTxt->text().toDouble();
            if(ui->closeTrueRBtn->isChecked()){
                back.closeGo = 1;
                back.goPercent = ui->percentTxt->text().toDouble();
            }else{
                back.closeGo = 2;
            }
        }
        emit interpolationReverse(back);
    }
}

// 重置值
void TrailTool::resetValue(){
    ui->forwardRBtn->setChecked(true);
    ui->attitudeTypeRBtn1->setChecked(true);
    ui->generalRBtn->setChecked(true);


    ui->rxTxt->setText("");
    ui->ryTxt->setText("");
    ui->rzTxt->setText("");

    ui->rxTxt_2->setText("");
    ui->ryTxt_2->setText("");
    ui->rzTxt_2->setText("");

    ui->tangentTypeRBtn1->setChecked(true);
    ui->directionTypeRBtn1->setChecked(true);

    ui->lineXEditTxt->setText("");
    ui->lineYEditTxt->setText("");
    ui->lineZEditTxt->setText("");

    ui->axisFirstXTxt->setText("");
    ui->axisFirstYTxt->setText("");
    ui->axisFirstZTxt->setText("");
    ui->axisSecondXTxt->setText("");
    ui->axisSecondYTxt->setText("");
    ui->axisSecondZTxt->setText("");

    ui->backRxTxt->setText("");
    ui->backRyTxt->setText("");
    ui->backRzTxt->setText("");

    ui->backAttitudeTypeRBtn1->setChecked(true);
    ui->backTouchXTxt->setText("");
    ui->backTouchYTxt->setText("");
    ui->backTouchZTxt->setText("");

    ui->handTypeRBtn->setChecked(true);

    ui->plateXTxt->setText("");
    ui->plateYTxt->setText("");
    ui->plateZTxt->setText("");

    ui->cutRadiuTxt->setText("");
    ui->cutCenterXTxt->setText("");
    ui->cutCenterYTxt->setText("");
    ui->cutCenterZTxt->setText("");

    ui->closeTrueRBtn->setChecked(true);
    ui->percentTxt->setText("");
    ui->backRxTxt_2->setText("");
    ui->backRyTxt_2->setText("");
    ui->backRzTxt_2->setText("");
    ui->backCutRadiusTxt->setText("");

    ui->adapterForwardRBtn->setChecked(true);
    ui->angleForwardRBtn->setChecked(true);
}


// 插补类型选择 下一步
void TrailTool::on_pushButton_8_clicked()
{
    // 正向选中
    if(ui->forwardRBtn->isChecked()){
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        this->setWindowTitle(tr("正走"));
    }
    // 反向选中
    else{
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        this->setWindowTitle(tr("反走"));
    }
}

// 取消
void TrailTool::on_pushButton_9_clicked()
{
    this->hide();
}

// 姿态角 类型选择
void TrailTool::on_pushButton_10_clicked()
{
    // 不变
    if(ui->attitudeTypeRBtn1->isChecked()){

        this->setWindowTitle(tr("正走->不变"));
        this->ui->stackedWidget->setCurrentWidget(ui->page_4);
    }
    // 保持
    else{
        this->setWindowTitle(tr("正走->保持"));
        this->ui->stackedWidget->setCurrentWidget(ui->page_5);
    }
}

// 取消
void TrailTool::on_pushButton_11_clicked()
{
    this->hide();
}

// 反走种类选择
void TrailTool::on_pushButton_12_clicked()
{
    if(ui->generalRBtn->isChecked()){
        this->setWindowTitle(tr("反走->通用"));
        ui->stackedWidget->setCurrentWidget(ui->page_9);
    }
    else{
        ui->stackedWidget->setCurrentWidget(ui->page_10);
        this->setWindowTitle(tr("反走->去毛刺"));
    }
}

// 取消
void TrailTool::on_pushButton_13_clicked()
{
    this->hide();
}


// 正走 -> 不变 -> 姿态角 确定
void TrailTool::on_pushButton_14_clicked()
{
    complete();
    this->hide();
}

// 取消
void TrailTool::on_pushButton_15_clicked()
{
    this->hide();
}

// 正走 -> 保持 -> 参数  确定
void TrailTool::on_pushButton_16_clicked()
{
    // 选中了朝点
    if(ui->directionTypeRBtn3->isChecked()){
        ui->stackedWidget->setCurrentWidget(ui->page_6);
    }
    // 选中了朝轴
    else if(ui->directionTypeRBtn4->isChecked()){
        ui->stackedWidget->setCurrentWidget(ui->page_7);
    }
    else{
        complete();
        this->hide();
    }
}

void TrailTool::on_pushButton_17_clicked()
{
    this->hide();
}



// 判断选择的面方向 根据面方向来 判断下一步的处理
void TrailTool::on_directionTypeRBtn1_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("完成");
    }
}


void TrailTool::on_directionTypeRBtn2_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("完成");
    }
}

void TrailTool::on_directionTypeRBtn3_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("下一步");
    }
}

void TrailTool::on_directionTypeRBtn4_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("下一步");
    }
}


// 正走 -> 保持 -> 参数 -> 姿态朝向点  确定
void TrailTool::on_pushButton_18_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_19_clicked()
{
    this->hide();
}


// 正走 -> 保持 -> 参数 -> 姿态朝向轴  确定
void TrailTool::on_pushButton_20_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_21_clicked()
{
    this->hide();
}




// 反走 -> 通用 完成
void TrailTool::on_pushButton_24_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_25_clicked()
{
    this->hide();
}



// 反走 -> 去毛刺 -> 姿态角选择 下一步
void TrailTool::on_pushButton_26_clicked()
{
    // 手动
    if(ui->handTypeRBtn->isChecked()){
        this->setWindowTitle(tr("反走->去毛刺->手动"));
        ui->stackedWidget->setCurrentWidget(ui->page_11);
    }
    // 自动
    else{
        this->setWindowTitle(tr("反走->去毛刺->自动"));
        ui->stackedWidget->setCurrentWidget(ui->page_12);
    }
}

void TrailTool::on_pushButton_27_clicked()
{
    this->hide();
}

// 反走-> 去毛刺 -> 手动 -> 姿态角 完成
void TrailTool::on_pushButton_28_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_29_clicked()
{
    this->hide();
}

// 反走 -> 去毛刺 -> 自动 -> 切割 完成
void TrailTool::on_pushButton_30_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_31_clicked()
{
    this->hide();
}

// 封闭曲线是否多走
void TrailTool::on_closeTrueRBtn_clicked(bool checked)
{
    if(checked){
        ui->percentTxt->setEnabled(true);
    }else{
        ui->percentTxt->setEnabled(false);
    }
}


// 封闭曲线是否多走
void TrailTool::on_closeFalseRBtn_clicked(bool checked)
{
    if(checked){
        ui->percentTxt->setEnabled(false);
    }else{
        ui->percentTxt->setEnabled(true);
    }
}




