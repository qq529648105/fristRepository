#ifndef TRAILTOOL_H
#define TRAILTOOL_H

#include <QMainWindow>
#include <QShowEvent>

namespace Ui {
class TrailTool;
}

// 正走  PdivideNbs
struct trail{
    int attitudeAngleType; // 姿态角类型 ：1  不变 flagZI[0]=1,  2 保持 flagZI[0]=0， 3 手动 ， 4 自动
    double attitudeAngleX; // 姿态角 rx zixyz[0]
    double attitudeAngleY; // 姿态角 ry zixyz[1]
    double attitudeAngleZ; // 姿态角 rz zixyz[2]

    int tangentType; // 1 切线选择 不变 flagZI[1]=1, 2 切线选择 保持 flagZI[1]=0
    int faceType; // 1 面方向选择 不变 flagZI[2]=2, 2 面方向选择 保持 flagZI[2]=0,3 面方向选择 朝点flagZI[2]=3 ,4 面方向选择 朝轴flagZI[2]=2
    double attitudePointX; // 姿态朝向点坐标X  ziMian[0]
    double attitudePointY; // 姿态朝向点坐标Y  ziMian[1]
    double attitudePointZ; // 姿态朝向点坐标Y  ziMian[2]

    double attitudeAxisFirstX; // 姿态朝轴第一点坐标X   ziMian[3]
    double attitudeAxisFirstY; // 姿态朝轴第一点坐标Y    ziMian[4]
    double attitudeAxisFirstZ; // 姿态朝轴第一点坐标Z    ziMian[5]
    double attitudeAxisSecondX; // 姿态朝轴第二点坐标X   ziMian[6]
    double attitudeAxisSecondY; // 姿态朝轴第二点坐标Y  ziMian[7]
    double attitudeAxisSecondZ; // 姿态朝轴第二点坐标Z  ziMian[8]

};

// 反走 NdivideNbs
struct trailback{
    int backType;	// 反走种类： 1 通用 flagNbsReverse[0] = 2 ，2 去毛刺  flagNbsReverse[0] = 1
    int attitudeAngleType; // 姿态角类型 ：1  不变 flagNbsReverse[1] =2,  2 保持 flagNbsReverse[1] =3， 3 手动 flagNbsReverse[1] =0， 4 自动 flagNbsReverse[1] =1
    double attitudeAngleX; // 姿态角 rx  zixyz[0]
    double attitudeAngleY; // 姿态角 ry  zixyz[1]
    double attitudeAngleZ; // 姿态角 rz  zixyz[2]

    double contactPointX;	// 接触点X坐标  toolANDzuan[0]
    double contactPointY;	// 接触点Y坐标  toolANDzuan[1]
    double contactPointZ;	// 接触点Z坐标  toolANDzuan[2]

    double flangePlateX;	// 法兰盘接触坐标X  toolANDzuan[0]
    double flangePlateY;	// 法兰盘接触坐标Y  toolANDzuan[1]
    double flangePlateZ;	// 法兰盘接触坐标Z  toolANDzuan[2]
    double cutRadius;	// 切割半径    toolANDzuan[3]
    double cutCenterX;	// 切割圆心坐标X  toolANDzuan[4]
    double cutCenterY;	// 切割圆心坐标Y  toolANDzuan[5]
    double cutCenterZ;	// 切割圆心坐标Z  toolANDzuan[6]
    int closeGo;			// 封闭曲线是否多走 1 是 int flagNbsReverse[2]= 1  2 不是  flagNbsReverse[2]= 0
    double goPercent;	// 多走百分比 toolANDzuan[7]

    double cutAngle;		// 切割角度  ZincluAngle
    int adapterType;		// 转接头触电方向 1 朝X方向 flagZAngle[0]=0 2 朝X反方向flagZAngle[0]=1
    int inclinedAngleType;	//倾斜角度类型 1 顺时针flagZAngle[1]=0	2 逆时针 flagZAngle[1]=1
};
class TrailTool : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TrailTool(QWidget *parent = 0);
    ~TrailTool();

    void showEvent(QShowEvent *);

signals:
    // 反走插补
    void interpolationReverse(trailback back);
    // 正走插补
    void interpolationGo(trail go);

private :
    void resetValue();
    void complete();
    
private slots:
    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_directionTypeRBtn1_clicked(bool checked);

    void on_directionTypeRBtn2_clicked(bool checked);

    void on_directionTypeRBtn3_clicked(bool checked);

    void on_directionTypeRBtn4_clicked(bool checked);

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();


    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_closeTrueRBtn_clicked(bool checked);

    void on_closeFalseRBtn_clicked(bool checked);

private:
    Ui::TrailTool *ui;
};

#endif // TRAILTOOL_H
