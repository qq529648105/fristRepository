#ifndef PAGESTATION_H
#define PAGESTATION_H

#include <QWidget>
#include "ui_PageStation.h"
#include "qtvariantproperty.h"
#include "SolutionSet.h"
//rong lk
#include "ModuleObject.h"
//end
#include <QSqlQueryModel>
#include <QFileInfo>

//rong lk
#include <gp_Trsf.hxx>
#include <Handle_AIS_Shape.hxx>
//#include <Python.h>
//end
#include <Qtimer>

//#include <gp_Trsf.hxx>
//#include <Handle_AIS_Shape.hxx>

//struct AxisStruct
//{
//    QString id;
//    QString sign;
//    QString name;
//    QString parentId;
//    int type;
//    double start[3];
//    double end[3];
//    double stepValue[3];
//    double move[3];
//    double originOffset;    // 零坐标偏移值
//    double value;
//    int symbol;
//    double minValue;
//    double maxValue;
//    QList<QString> subAxisList;
//    QList<QString> moduleList;
//};
//struct ModuleStruct
//{
//    QString id;
//    QString code;
//    QString name;
//    int type;
//    QString fileId;
//    QString bindAxis;
//    double pos[3];
//    double rotatePoint[3];
//    double rotateDirection[3];
//    double rotateAngle;
//    double transparency;
//    int material;
//    int color;
//    bool isShow;
//    QString parentId;
//    gp_Trsf transform;
//    Handle_AIS_Shape aisShape;
//};
//rong lk
typedef void (*positiveSolutionDLL)(int RobotType, double RobotParamsX[6], double RobotParamsZ[6], double RobotParamsAS[6], double RobotParamsAE[6], double Angles[6], double Solution[6]);
typedef void (*angleToQuaternionDLL)(double WPR[3], double Quaternion[4]);
//end
class PageStation : public QWidget
{
	Q_OBJECT

public:
	PageStation(QWidget *parent = 0);
	~PageStation();
    int robotNum();
    QString currentStationId;
    /***************update7 by cxs**************/
        QString robotStationId;
        QString robot2StationId;
        QString robot3StationId;
    /******************end*****************/
        //rong lk
            void sendCalibrationData(); //发送信息
        //end

protected:
    void showEvent( QShowEvent *event );

private:
	Ui::PageStation ui;

    QSqlQueryModel *moduleSql;
    QTreeWidgetItem *rootModuleItem;

    QStringList axisNameList;

    QtVariantPropertyManager *variantManagerModule, *variantManagerAxis, *variantManagerAction, *variantManagerTrajectory, *variantManagerCalibration;
    QtProperty *topPropertyModule, *topPropertyAxis, *topPropertyAction, *topPropertyTrajectory, *topPropertyCalibration;

    QtBrowserItem *actItemParent;
    QtBrowserItem *currentItem;
    //rong lk
        QtBrowserItem *trajectoryItemParent;
        QtBrowserItem *calibrationItemParent;
    //end
    QString lastSelectedPath;

    int itemPos;
    bool isActionChange; //动作设置参数发生变化标志

    //rong lk
        bool isTrajectoryChange; //轨迹设置参数发生变化标志
        bool isCalibrationChange; //轨迹设置参数发生变化标志
        int trajectoryNum; //轨迹编号
        int calibrationNum; //校准编号
    //end
//    QMap<QtProperty *, QString> propertyToId;
//    QMap<QString, QtVariantProperty *> idToProperty;

    /*********update6 by cxs**********/
        SolutionSet *importSolThread;
        SolutionSet *exportSolThread;
    /**************end**************/
    /***************update8 by  cxs************/
        SolutionSet *exportRobotThread;
        SolutionSet *importRobotThread;
    /********************end******************/

    //rong lk
        HINSTANCE hModule;
    int cur_areaIndex;
    int areaX,areaY,areaZ;
    QMap<QString,QString> areaList;
    QString curArea;
    QTimer *t1;
private slots:
    void currentTabChanged(int index);
    void textChanged(const QString &text);
    void memoTextChanged();
    void stationListChanged(int index);
    void on_pushButton_add_clicked();
    void on_pushButton_del_clicked();
    void moduleItemChanged(QTreeWidgetItem *item, int column);
    void moduleSelectionChanged();
    void axisSelectionChanged();
    void actionSelectionChanged();

    void moduleNewGroup();
    void moduleNewMotion();
    void moduleImport();
    void moduleRename();
    void moduleDuplicate();
    void moduleRemove();
    void moduleReplace();
    void moduleTranslate();
//    void moduleProperty();

    /********update13***********/
     void showModuleAction();

    void axisNew();
    void axisRename();
    void axisRemove();
    void axisSetPosition();

    void axisSetCurRobot();
//    void axisProperty();

    /************update4 by cxs 2014 4.28***********/
        void axisDuplicate();
        void actionDuplicate();
    /*******************end********************/

    void actionNew();
    void actionRename();
    void actionRemove();
//    void actionProperty();
    //rong lk
        void trajectorySelectionChanged();
        void trajectoryNew();
        void selectPath();
        void trajectoryRename();
        void trajectoryRemove();
        void calibrationSelectionChanged();
        void calibrationNew();
        void calibrationPropertyChanged(QtProperty *property, const QVariant &value);
        void updateCalibrationPoint(gp_Pnt pnt);
        void updateCalibrationFlange();
        void calibrationFlange();
        int generateRapidToolData();
        void AddModuleLable();
        void trajectoryPropertyChanged(QtProperty *property, const QVariant &value);

        void calibrationRename();
        void calibrationDuplicate();
        void calibrationRemove();

    //end
    void on_treeWidgetModule_itemActivated(QTreeWidgetItem *item, int column);
    void on_treeWidgetModule_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void modulePropertyChanged(QtProperty *property, const QVariant &value);
    void axisPropertyChanged(QtProperty *property, const QVariant &value);
    void actionPropertyChanged(QtProperty *property, const QVariant &value);

    void QuerModuleId();
    void QuerKnifeName();

    /***********update6 by cxs ***************/
        void on_pushButton_export_clicked();
        void on_pushButton_import_clicked();
        void GetImportData();
        void GetExportData();
    /**************end*********************/

    /***********update7 by cxs***************/
       void robotChanged(int);
       void robot2Changed(int);
       void robot3Changed(int);
    /******************end******************/

    /*******************update8 by cxs*****************/
        void on_pushButton_importRobot_clicked();
//        void on_pushButton_robotexport_clicked();
        void exportRobotResult();
        void importRobotResult();
        void readRobotInfo();

    /******************end***********************************/

       void postOpen();

       /***********update12***************/
       void on_pushButton_exportRobot_clicked();
       /******************end*************/





       void on_pushButton_3_clicked();

       void on_pushButton_4_clicked();

       void on_pushButton_5_clicked();

       void on_pushButton_6_clicked();

       void on_pushButton_clicked();

       void on_pushButton_2_clicked();

       void on_pushButton_7_clicked();

       void on_pushButton_8_clicked();

       void on_com_area_currentIndexChanged(int index);

       void on_pushButton_9_clicked();


       void on_ySlider_valueChanged(int value);

       void on_xSlider_valueChanged(int value);

       void on_zSlider_valueChanged(int value);



       void on_pushButton_11_clicked();

public slots:
       //rong lk
           void shapeSelected(QStringList, bool);
       //end

    void changeStationTab(int index);
    void moduleAllLoaded( int count );
    void moveEnd(double x, double y, double z);
    void rotateEnd(double x, double y, double z, double rx, double ry,double rz, double angle);
    void on_cur_robot_currentIndexChanged(int index);
    bool querySafeArea();
    bool updateArea(QString id);
    void safeAreaSend(int index);
    void showAllArea();
    void axisSetPosition_servo(double d);

signals:
    void importModuleFile(QString filename, QString id, QString name);
    void moduleChanged(QString id, bool isRemoved);
    void stationChanged(QString stationId);

    void postOpenSignal();
    void addLog(QString message, QString time);
    void signalVMState();
    //rong lk
    void selectionEdge();
    void calibrationFlangeSuccess();
    //end
};

#endif // PAGESTATION_H
