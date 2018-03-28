#include "ParmeterForm.h"
#include "ui_ParmeterForm.h"
#include <QDrag>
#include <QMimeData>
#include <QListWidgetItem>
#include <QTimer>
#include <QLabel>
#include <QSqlQuery>
#include <QPalette>
#include <QFont>
#include <QFontInfo>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QSqlError>
#include <QBuffer>
#include <QList>
#include "ControlProperty.h"
#include "ParmeterShow.h"




QMap<QLabel*,ControlProperty*>controlMap;


ParmeterForm::ParmeterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParmeterForm)
{
    ui->setupUi(this);
    ui->pushButtonImage->setEnabled(false);
    this->variantManagerControl = new QtVariantPropertyManager(this);
    topPropertyControl = variantManagerControl->addProperty(QtVariantPropertyManager::groupTypeId(), tr("�༭�ؼ�����"));
    this->controlItem = ui->propertyControl->addProperty(topPropertyControl);
    QtVariantEditorFactory *variantFactoryControl = new QtVariantEditorFactory(this);
    ui->propertyControl->setFactoryForManager(variantManagerControl, variantFactoryControl);
    ui->propertyControl->setPropertiesWithoutValueMarked(true);
    ui->propertyControl->setRootIsDecorated(false);
    parmeterShow = new ParmeterShow(this);
    s_parmeterShowEdit = ParmeterShow::initialize();


    connect(ui->editForm,SIGNAL(s_editProperty(QLabel*)),this,SLOT(editProperty(QLabel*)));
    connect(this,SIGNAL(saveSignal(int,int)),this,SLOT(showParmeter()));
   // connect(this,SIGNAL(saveSignal(int,int)),ui->realControlForm,SLOT(saveParameter(int,int)));
}


ParmeterForm::~ParmeterForm()
{
    delete ui;
}





void ParmeterForm::editProperty(QLabel* editLabel)
{
    disconnect(variantManagerControl, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
               this, SLOT(controlPropertyChanged(QtProperty *, const QVariant &)));
     currentLabel = editLabel;
     ui->pushButtonImage->setEnabled(false);
    if(!s_parmeterShowEdit->realControlMap.contains(editLabel)){
       topPropertyControl->setEnabled(false);
       return;
    }
    topPropertyControl->setEnabled(true);
    foreach (QtProperty *property, topPropertyControl->subProperties())
        topPropertyControl->removeSubProperty(property);
    QtVariantProperty *itemPro;
    if(s_parmeterShowEdit->realControlMap[editLabel]->type == 0){
        itemPro = variantManagerControl->addProperty(QVariant::String, tr("������"));
        itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->name);
        topPropertyControl->addSubProperty(itemPro);
//        itemPro =  variantManagerControl->addProperty(QVariant::String, tr("����ֵ"));
//        topPropertyControl->addSubProperty(itemPro);
//        itemPro =  variantManagerControl->addProperty(QVariant::String, tr("��λ"));
//        topPropertyControl->addSubProperty(itemPro);
//        itemPro->setValue(controlMap[editLabel]->unit);
    }else if(s_parmeterShowEdit->realControlMap[editLabel]->type == 1){
        itemPro = variantManagerControl->addProperty(QVariant::String, tr("�ź���"));
        itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->name);
        topPropertyControl->addSubProperty(itemPro);
//        itemPro =  variantManagerControl->addProperty(QVariant::String, tr("�ź�ֵ"));
//        topPropertyControl->addSubProperty(itemPro);
    }else if(s_parmeterShowEdit->realControlMap[editLabel]->type == 2){
        itemPro = variantManagerControl->addProperty(QVariant::String, tr("�ı�"));
        itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->name);
        topPropertyControl->addSubProperty(itemPro);
       // itemPro =  variantManagerControl->addProperty(QVariant::String, tr("�ź�ֵ"));
        //topPropertyControl->addSubProperty(itemPro);

    }else if(s_parmeterShowEdit->realControlMap[editLabel]->type == 3){
        itemPro = variantManagerControl->addProperty(QVariant::String, tr("ͼ��"));
        itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->name);
        //itemPro->setEnabled(false);
        topPropertyControl->addSubProperty(itemPro);
        ui->pushButtonImage->setEnabled(true);
    }

    itemPro = variantManagerControl->addProperty(QVariant::Int, tr("��ʼ��X"));
    topPropertyControl->addSubProperty(itemPro);
    itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->startX);
    qDebug()<<s_parmeterShowEdit->realControlMap[editLabel]->startX;
    itemPro = variantManagerControl->addProperty(QVariant::Int, tr("��ʼ��Y"));
    topPropertyControl->addSubProperty(itemPro);
    itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->startY);
    itemPro = variantManagerControl->addProperty(QVariant::Int, tr("���"));
    topPropertyControl->addSubProperty(itemPro);
    itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->width);
    itemPro = variantManagerControl->addProperty(QVariant::Int, tr("�߶�"));
    topPropertyControl->addSubProperty(itemPro);
    itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->height);
    if(s_parmeterShowEdit->realControlMap[editLabel]->type != 3){
        itemPro = variantManagerControl->addProperty(QVariant::Color, tr("��ɫ"));
        topPropertyControl->addSubProperty(itemPro);
        itemPro->setValue(QColor(s_parmeterShowEdit->realControlMap[editLabel]->color &255,s_parmeterShowEdit->realControlMap[editLabel]->color>>8 &255 ,s_parmeterShowEdit->realControlMap[editLabel]->color>>16 & 255));
        itemPro = variantManagerControl->addProperty(QVariant::Font, tr("�����С"));
        topPropertyControl->addSubProperty(itemPro);
        if(s_parmeterShowEdit->realControlMap[editLabel]->fontFamily != ""){
            QFont font;
            font.setFamily(s_parmeterShowEdit->realControlMap[editLabel]->fontFamily);
            font.setPointSize(s_parmeterShowEdit->realControlMap[editLabel]->fontSize);
            font.setBold(s_parmeterShowEdit->realControlMap[editLabel]->fontBold);
            font.setItalic(s_parmeterShowEdit->realControlMap[editLabel]->fontItalic);
            font.setStrikeOut(s_parmeterShowEdit->realControlMap[editLabel]->fontStrikeout);
            font.setUnderline(s_parmeterShowEdit->realControlMap[editLabel]->fontUnderline);
            itemPro->setValue(font);
        }
        itemPro = variantManagerControl->addProperty(QtVariantPropertyManager::enumTypeId(), tr("���뷽ʽ"));
        QStringList enumNames;
        enumNames <<"�����"<<"����"<<"�Ҷ���";
        itemPro->setAttribute(QLatin1String("enumNames"), enumNames);
        itemPro->setValue(s_parmeterShowEdit->realControlMap[editLabel]->aligan);
        topPropertyControl->addSubProperty(itemPro);
        ui->propertyControl->editItem(this->controlItem->children().at(0));

    }
    connect(variantManagerControl, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                   this, SLOT(controlPropertyChanged(QtProperty *, const QVariant &)));

}


void ParmeterForm::controlPropertyChanged(QtProperty *property, const QVariant &value)
{
    QSqlQuery query;
    if(property->propertyName() == "������" || property->propertyName() == "�ź���" ||property->propertyName() == "�ı�" ||property->propertyName() =="ͼ��"){
       s_parmeterShowEdit->realControlMap[currentLabel]->name = value.toString();
       if(property->propertyName() !="ͼ��")
       currentLabel->setText(value.toString());
       query.exec(QString("UPDATE EditControl SET name = '%1' WHERE id ='%2'").arg(value.toString(), s_parmeterShowEdit->realControlMap[currentLabel]->id));
    }else if(property->propertyName() == "��ʼ��X" ){
       s_parmeterShowEdit->realControlMap[currentLabel]->startX = value.toInt();
       currentLabel->setGeometry( s_parmeterShowEdit->realControlMap[currentLabel]->startX, s_parmeterShowEdit->realControlMap[currentLabel]->startY, s_parmeterShowEdit->realControlMap[currentLabel]->width, s_parmeterShowEdit->realControlMap[currentLabel]->height);
       query.exec(QString("UPDATE EditControl SET startX = %1 WHERE id ='%2'").arg(QString::number(value.toInt()),s_parmeterShowEdit->realControlMap[currentLabel]->id));
    }else if(property->propertyName() == "��ʼ��Y" ){
       s_parmeterShowEdit->realControlMap[currentLabel]->startY = value.toInt();
       currentLabel->setGeometry(s_parmeterShowEdit->realControlMap[currentLabel]->startX, s_parmeterShowEdit->realControlMap[currentLabel]->startY, s_parmeterShowEdit->realControlMap[currentLabel]->width, s_parmeterShowEdit->realControlMap[currentLabel]->height);
       query.exec(QString("UPDATE EditControl SET startY = %1 WHERE id ='%2'").arg(QString::number(value.toInt()),s_parmeterShowEdit->realControlMap[currentLabel]->id));
    }else if(property->propertyName() == "���" ){
       s_parmeterShowEdit->realControlMap[currentLabel]->width = value.toInt();
       currentLabel->setGeometry(s_parmeterShowEdit->realControlMap[currentLabel]->startX, s_parmeterShowEdit->realControlMap[currentLabel]->startY, s_parmeterShowEdit->realControlMap[currentLabel]->width, s_parmeterShowEdit->realControlMap[currentLabel]->height);
       query.exec(QString("UPDATE EditControl SET width = %1 WHERE id ='%2'").arg(QString::number(value.toInt()),s_parmeterShowEdit->realControlMap[currentLabel]->id));
    }else if(property->propertyName() == "�߶�" ){
        s_parmeterShowEdit->realControlMap[currentLabel]->height = value.toInt();
        currentLabel->setGeometry(s_parmeterShowEdit->realControlMap[currentLabel]->startX, s_parmeterShowEdit->realControlMap[currentLabel]->startY, s_parmeterShowEdit->realControlMap[currentLabel]->width, s_parmeterShowEdit->realControlMap[currentLabel]->height);
        query.exec(QString("UPDATE EditControl SET height = %1 WHERE id ='%2'").arg(QString::number(value.toInt()),s_parmeterShowEdit->realControlMap[currentLabel]->id));
     }else if(property->propertyName() == "��ɫ" ){
        QColor color = value.value<QColor>();
        int colorInt = ((unsigned int)color.blue()<<16 | (unsigned int)color.green()<<8 | (unsigned int)color.red());
        s_parmeterShowEdit->realControlMap[currentLabel]->color = colorInt;
        query.exec(QString("UPDATE EditControl SET color = %1 WHERE id ='%2'").arg(QString::number(colorInt),s_parmeterShowEdit->realControlMap[currentLabel]->id));
        QPalette pal;
        pal.setColor(QPalette::WindowText,color);
        currentLabel->setPalette(pal);
    }else if(property->propertyName() == "��λ" ){
        s_parmeterShowEdit->realControlMap[currentLabel]->unit = value.toString();
        query.exec(QString("UPDATE EditControl SET unit = '%1' WHERE id ='%2'").arg(value.toString(),s_parmeterShowEdit->realControlMap[currentLabel]->id));

    }
    else if(property->propertyName() == "�����С" ){
        QFont font = value.value<QFont>();
        s_parmeterShowEdit->realControlMap[currentLabel]->fontFamily = font.family();
        s_parmeterShowEdit->realControlMap[currentLabel]->fontSize = font.pointSize();
        qDebug()<<"pointsize"<<font.pointSize();
        s_parmeterShowEdit->realControlMap[currentLabel]->fontBold = font.bold();
        s_parmeterShowEdit->realControlMap[currentLabel]->fontItalic = font.italic();
        s_parmeterShowEdit->realControlMap[currentLabel]->fontUnderline = font.underline();
        s_parmeterShowEdit->realControlMap[currentLabel]->fontStrikeout = font.strikeOut();
        currentLabel->setFont(font);
        if(!query.exec(QString("UPDATE EditControl SET fontFamily='%1',fontSize=%2,fontBold=%3,fontItalic=%4,fontUnderline=%5,fontStrikeout=%6 WHERE id ='%7'").arg(font.family(),QString::number(font.pointSize()),QString::number(font.bold()),QString::number(font.italic()),QString::number(font.underline()),QString::number(font.strikeOut()),s_parmeterShowEdit->realControlMap[currentLabel]->id)))
            qDebug()<<QString("UPDATE EditControl SET fontFamily='%1',fontSize=%2,fontBold=%3,fontItalic=%4,fontUnderline=%5,fontStrikeout=%6 WHERE id ='%7'").arg(font.family(),QString::number(font.pointSize()),QString::number(font.bold()),QString::number(font.italic()),QString::number(font.underline()),QString::number(font.strikeOut()),s_parmeterShowEdit->realControlMap[currentLabel]->id);
    }else if(property->propertyName() == tr("���뷽ʽ")){
        s_parmeterShowEdit->realControlMap[currentLabel]->aligan = value.toInt();
        if(!query.exec(QString("UPDATE EditControl SET align ='%1' WHERE id ='%2' ").arg(QString::number(s_parmeterShowEdit->realControlMap[currentLabel]->aligan),s_parmeterShowEdit->realControlMap[currentLabel]->id)))
            qDebug()<<query.lastError().text();
        if(0 == value.toInt())
            currentLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        else if(1 == value.toInt())
            currentLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        else if(2 == value.toInt())
            currentLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    }

    ui->editForm->update();


}


void ParmeterForm::on_pushButtonRemove_clicked()
{
    if(ui->editForm->editLabel)
        ui->editForm->removeControl();
    if(ui->pushButtonImage->isEnabled())
        ui->pushButtonImage->setEnabled(false);
}

void ParmeterForm::on_pushButtonSave_clicked()
{
    emit saveSignal(ui->scrollAreaPreview->height(),ui->scrollAreaPreview->width());
}

void ParmeterForm::on_pushButtonImage_clicked()
{
    QString imagePath  = QFileDialog::getOpenFileName(NULL,"ѡ��ͼƬ","./", tr("*.png *.jpg  *.jpeg"));
    if(imagePath == "")
        return;
    QFile file(imagePath);
    if(file.size() >102400 *2){
        QMessageBox::warning(NULL,tr("����ͼƬʧ��"),tr("ͼƬ����200K������ʧ��"),QMessageBox::tr("ȷ��"));
        return;
    }
    editProperty(currentLabel);
    QPixmap pix(imagePath);
    currentLabel->setPixmap(pix);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    currentLabel->pixmap()->save(&buffer, "PNG");
    //QByteArray imageByte = file.readAll();
    //qDebug()<<"  s"<<imageByte<<file.size()<<imageByte.size()<<imageByte.count();
    //file.close();
    s_parmeterShowEdit->realControlMap[currentLabel]->image = bytes;
    s_parmeterShowEdit->realControlMap[currentLabel]->width = pix.width();
    s_parmeterShowEdit->realControlMap[currentLabel]->height = pix.height();
    currentLabel->resize(pix.width(),pix.height());
    ui->editForm->update();
    QVariant data(bytes);
    QSqlQuery query;
    query.exec(QString("DELETE FROM EditControl WHERE id = '%1'").arg(s_parmeterShowEdit->realControlMap[currentLabel]->id));
    query.prepare(QString("INSERT INTO EditControl (id,name,type,image,startX,startY,width,height) VALUES('%1','%2',3,?,%4,%5,%6,%7)").arg(s_parmeterShowEdit->realControlMap[currentLabel]->id,s_parmeterShowEdit->realControlMap[currentLabel]->name,QString::number(s_parmeterShowEdit->realControlMap[currentLabel]->startX),QString::number(s_parmeterShowEdit->realControlMap[currentLabel]->startY),QString::number(s_parmeterShowEdit->realControlMap[currentLabel]->width),QString::number(s_parmeterShowEdit->realControlMap[currentLabel]->height)));
    query.addBindValue(data);
    if(!query.exec())
        qDebug()<<query.lastError().text();

}


void ParmeterForm::loadControl(QMap<QLabel*,ControlProperty*> &map,QWidget *widget)
{

}


void ParmeterForm::queryAllControl()
{

}


void ParmeterForm::showEvent(QShowEvent* /*event*/)
{
    static bool isShow = false;
    if(!isShow){
        ui->editForm->editLabel = NULL;
        s_parmeterShowEdit->showParmeterControl(ui->editForm);
        isShow = true;
    }
}


void ParmeterForm::paintEvent(QPaintEvent * /*event*/)
{

    static int screenWidth = ui->scrollAreaPreview->width();
    static int screenHeight = ui->scrollAreaPreview->height();
    if(ui->scrollAreaPreview->width()!= screenWidth || ui->scrollAreaPreview->height() != screenHeight){

        s_parmeterShowEdit->loadParmeterControl(ui->editForm);
        parmeterShow->loadParmeterControl(ui->realControlForm);
        screenWidth = ui->scrollAreaPreview->width();
        screenHeight = ui->scrollAreaPreview->height();
    }


}


void ParmeterForm::showParmeter()
{
    parmeterShow->showParmeterControl(ui->realControlForm);
}
