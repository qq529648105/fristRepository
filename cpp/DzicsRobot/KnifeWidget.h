#ifndef KNIFEWIDGET_H
#define KNIFEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QTableWidgetItem>
#include <QStringListModel>
#include <QStringList>

typedef struct _CUTINFO
{
    QString CutName;
    QString CutNo;
    QString CutSpe;
    int CutTotNum;
    int CutUseNum;
    QString PriKey;
}CUTINFO;

typedef struct _Knife
{
    QString id;
    QString name1;
    QString name2;
    QString name3;
    QString name4;
    float x;
    float z;
    float r;
    float y;
    float max;
    float standard;
    float v1;//当前值
    float v2;
    QString s1;
    QString s2;

}Knife;

typedef struct _Knife2
{
    QString id;
    QString parId;
    QString xi;
    QString var;
    QString name1;
    QString name2;
    float v1;
    float v2;
    float v3;
    float v4;
    float v5;
    float v6;
    QString bind;

}Knife2;

namespace Ui {
class KnifeWidget;
}

class KnifeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KnifeWidget(QWidget *parent = 0);
    ~KnifeWidget();

    //数据库操作接口
//     bool OpenDb(const QString& db);
     bool GetDbConnect(){return isDbCon;}
     bool ExecSql(QString &sql);
     bool QuerAllData();
     bool QuerAllData2();
     bool DelData(QString &keyId );
     bool InsertNullData(QString& keyid);
     unsigned int QueLastKeyId();
     int QueTotNum(QString& keyId);
     int QueUseNum(QString& keyId);
     void setCurTab(int index);

private:
    Ui::KnifeWidget *ui;
    bool isDbCon;
    void InitDataTable(void);
    void ReadCutInfo();
    QStringList fieldName,fieldName2;
    QStringListModel *listModel;
    QStringList listLog;


private slots:
    void InitKinMenu(void);
    virtual void ShowKniInfo(void);
    void ShowKniInfo(int ,int);
    void EditDjItem(int, int);
    void GetItem( int, int);
    void GetItem2( int, int);

    void selectAllDataToSwitchList();
    void selectAllDataToAbrasionList();

    void on_pushButton_3_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);
    void modifyON(int index);
    void sendKnifeParam();

    void on_comboBox_activated(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

protected:
    void paintEvent(QPaintEvent *e);
signals:
    void currentIndexChanged(int index);
    void addLog(QString message, QString time);
//接口
public slots:
    void addKnife(void);          //添加刀具操作
    void addKnife1(void);
    void addKnife2(void);
    void removeKnife(void);       //删除刀具操作
    void removeKnife1(void);
    void removeKnife2(void);
    int useKnife(void);           //使用刀具操作
    int resetKnife(void);         //重置刀具操作
    void clearData();
    void writeKnife1(QString data);
    void writeKnife1Log(QString& data);
    void updateTab1();
    int autoKnife(QString, double data, QString &out);


};

#endif // KNIFEWIDGET_H
