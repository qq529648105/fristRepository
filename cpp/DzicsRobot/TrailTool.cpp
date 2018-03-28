#include "TrailTool.h"
#include "ui_TrailTool.h"
#include <QMessageBox>

TrailTool::TrailTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrailTool)
{

    // ���ô�����ʽ
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

    // ��λ
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



// ��ʾ�����ʱ�򴥷�
void TrailTool::showEvent(QShowEvent *){
    ui->stackedWidget->setCurrentWidget(ui->page);
    this->setWindowTitle(tr("ѡ��岹�߷�"));
    resetValue();
}

// ��ɵ�ʱ�����
void TrailTool::complete(){
    // ����
    if(ui->forwardRBtn->isChecked()){
        trail go;
        // ��̬��
        go.attitudeAngleX = ui->rxTxt->text().toDouble();
        go.attitudeAngleY = ui->ryTxt->text().toDouble();
        go.attitudeAngleZ = ui->rzTxt->text().toDouble();
        // ��̬������
        if(ui->attitudeTypeRBtn1->isChecked()){
            go.attitudeAngleType = 1;
        }else{
            go.attitudeAngleType = 2;
            // ���߷��� ����
            if(ui->tangentTypeRBtn1->isChecked()){
                go.tangentType = 1;
            }else{
                go.tangentType = 2;
            }
            // �淽�� ����
            if(ui->directionTypeRBtn1->isChecked()){
                go.faceType = 1;
            }
            // ����
            else if(ui->directionTypeRBtn2->isChecked()){
                go.faceType = 2;
            }
            // ����
            else if(ui->directionTypeRBtn3->isChecked()){
                go.faceType = 3;
                go.attitudePointX = ui->lineXEditTxt->text().toDouble();
                go.attitudePointY = ui->lineYEditTxt->text().toDouble();
                go.attitudePointZ = ui->lineZEditTxt->text().toDouble();
            }
            // ����
            else{
                go.faceType = 4;
                // ����������
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
    // ����
    else if(ui->backwardRBtn->isChecked()){
        trailback back;
        // ͨ��
        if(ui->generalRBtn->isChecked()){
            back.backType = 1;
            //��̬��
            back.attitudeAngleX = ui->backRxTxt->text().toDouble();
            back.attitudeAngleY = ui->backRyTxt->text().toDouble();
            back.attitudeAngleZ = ui->backRzTxt->text().toDouble();
            // ��̬������ ����
            if(ui->backAttitudeTypeRBtn1->isChecked()){
                back.attitudeAngleType = 1;
            }
            // ����
            else{
                back.attitudeAngleType = 2;
            }
            // �Ӵ�������
            back.contactPointX = ui->backTouchXTxt->text().toDouble();
            back.contactPointY = ui->backTouchYTxt->text().toDouble();
            back.contactPointZ = ui->backTouchZTxt->text().toDouble();
        }
        // ȥë��
        else{
            back.backType = 2;
            // ��̬������ �ֶ�
            if(ui->handTypeRBtn->isChecked()){
                back.attitudeAngleType = 3;
                back.attitudeAngleX = ui->backRxTxt_2->text().toDouble();
                back.attitudeAngleY = ui->backRyTxt_2->text().toDouble();
                back.attitudeAngleZ = ui->backRzTxt_2->text().toDouble();
            }
            // �Զ�
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
            // ����������
            back.flangePlateX = ui->plateXTxt->text().toDouble();
            back.flangePlateY = ui->plateYTxt->text().toDouble();
            back.flangePlateZ = ui->plateZTxt->text().toDouble();
            // �и�뾶
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

// ����ֵ
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


// �岹����ѡ�� ��һ��
void TrailTool::on_pushButton_8_clicked()
{
    // ����ѡ��
    if(ui->forwardRBtn->isChecked()){
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        this->setWindowTitle(tr("����"));
    }
    // ����ѡ��
    else{
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        this->setWindowTitle(tr("����"));
    }
}

// ȡ��
void TrailTool::on_pushButton_9_clicked()
{
    this->hide();
}

// ��̬�� ����ѡ��
void TrailTool::on_pushButton_10_clicked()
{
    // ����
    if(ui->attitudeTypeRBtn1->isChecked()){

        this->setWindowTitle(tr("����->����"));
        this->ui->stackedWidget->setCurrentWidget(ui->page_4);
    }
    // ����
    else{
        this->setWindowTitle(tr("����->����"));
        this->ui->stackedWidget->setCurrentWidget(ui->page_5);
    }
}

// ȡ��
void TrailTool::on_pushButton_11_clicked()
{
    this->hide();
}

// ��������ѡ��
void TrailTool::on_pushButton_12_clicked()
{
    if(ui->generalRBtn->isChecked()){
        this->setWindowTitle(tr("����->ͨ��"));
        ui->stackedWidget->setCurrentWidget(ui->page_9);
    }
    else{
        ui->stackedWidget->setCurrentWidget(ui->page_10);
        this->setWindowTitle(tr("����->ȥë��"));
    }
}

// ȡ��
void TrailTool::on_pushButton_13_clicked()
{
    this->hide();
}


// ���� -> ���� -> ��̬�� ȷ��
void TrailTool::on_pushButton_14_clicked()
{
    complete();
    this->hide();
}

// ȡ��
void TrailTool::on_pushButton_15_clicked()
{
    this->hide();
}

// ���� -> ���� -> ����  ȷ��
void TrailTool::on_pushButton_16_clicked()
{
    // ѡ���˳���
    if(ui->directionTypeRBtn3->isChecked()){
        ui->stackedWidget->setCurrentWidget(ui->page_6);
    }
    // ѡ���˳���
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



// �ж�ѡ����淽�� �����淽���� �ж���һ���Ĵ���
void TrailTool::on_directionTypeRBtn1_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("���");
    }
}


void TrailTool::on_directionTypeRBtn2_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("���");
    }
}

void TrailTool::on_directionTypeRBtn3_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("��һ��");
    }
}

void TrailTool::on_directionTypeRBtn4_clicked(bool checked)
{
    if(checked){
        ui->pushButton_16->setText("��һ��");
    }
}


// ���� -> ���� -> ���� -> ��̬�����  ȷ��
void TrailTool::on_pushButton_18_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_19_clicked()
{
    this->hide();
}


// ���� -> ���� -> ���� -> ��̬������  ȷ��
void TrailTool::on_pushButton_20_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_21_clicked()
{
    this->hide();
}




// ���� -> ͨ�� ���
void TrailTool::on_pushButton_24_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_25_clicked()
{
    this->hide();
}



// ���� -> ȥë�� -> ��̬��ѡ�� ��һ��
void TrailTool::on_pushButton_26_clicked()
{
    // �ֶ�
    if(ui->handTypeRBtn->isChecked()){
        this->setWindowTitle(tr("����->ȥë��->�ֶ�"));
        ui->stackedWidget->setCurrentWidget(ui->page_11);
    }
    // �Զ�
    else{
        this->setWindowTitle(tr("����->ȥë��->�Զ�"));
        ui->stackedWidget->setCurrentWidget(ui->page_12);
    }
}

void TrailTool::on_pushButton_27_clicked()
{
    this->hide();
}

// ����-> ȥë�� -> �ֶ� -> ��̬�� ���
void TrailTool::on_pushButton_28_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_29_clicked()
{
    this->hide();
}

// ���� -> ȥë�� -> �Զ� -> �и� ���
void TrailTool::on_pushButton_30_clicked()
{
    complete();
    this->hide();
}

void TrailTool::on_pushButton_31_clicked()
{
    this->hide();
}

// ��������Ƿ����
void TrailTool::on_closeTrueRBtn_clicked(bool checked)
{
    if(checked){
        ui->percentTxt->setEnabled(true);
    }else{
        ui->percentTxt->setEnabled(false);
    }
}


// ��������Ƿ����
void TrailTool::on_closeFalseRBtn_clicked(bool checked)
{
    if(checked){
        ui->percentTxt->setEnabled(false);
    }else{
        ui->percentTxt->setEnabled(true);
    }
}




