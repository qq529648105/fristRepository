#ifndef SYSMANAGER_H
#define SYSMANAGER_H

#include <QWidget>
#include <QString>
#include <QTableWidget>
namespace Ui {
class SysManager;
}
enum ConfigValueType{
    BOOL_TYPE=0,   //检测区域设置
    NUM_TYPE,      //移动相机标定模式
    ROBTARGET_TYPE,    //固定相机标定模式 (中心点标定)
    TOOLDATA_TYPE,
    WOBJDATA_TYPE
};
typedef struct _stationConfiguration
{
    QString id;
    QString stationId;
    QString name;
    int classIndex;
    QString value1;
    QString value2;
    QString value3;
    QString value4;
    QString value5;

}StationConfiguration;
class SysManager : public QWidget
{
    Q_OBJECT
    
public:
    explicit SysManager(QWidget *parent = 0);
    QString cfg1;
    ~SysManager();
    bool queryStationConfigurationData();
    bool deleteStationConfigurationData(QString &id);
    void readstationConfigurationInfo();
    bool validateData(const QString &str,int row, int column);

    void sysManagerDataUpdate();
    QString data;
    QString f9Index;

protected:
    void mouseMoveEvent(QMouseEvent *);
    
private slots:




    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void moveData();
    void swapRow(int row1,int row2);
    void setBtn();
    void configurationPropertyChanged(int row, int column);

    void dataTypechangedSlot(int index);
    void register_CMD_COMMON_TRIGGER();

    void on_comboBox_f9_currentIndexChanged(int index);
    void clickSlot();

    void on_pushButton_6_clicked();

private:
    Ui::SysManager *ui;
    void initUi();
    void initData();
    bool initData_2();
    void updateUi();
    void update();
    QString cur_solutionId;
    int curEdit;

    void insertNullData();
    QStringList rType;
    QList<StationConfiguration> stationConfigurationList;



};

#endif // SYSMANAGER_H
