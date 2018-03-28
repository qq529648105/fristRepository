#ifndef TRAILTOOL_H
#define TRAILTOOL_H

#include <QMainWindow>
#include <QShowEvent>

namespace Ui {
class TrailTool;
}

// ����  PdivideNbs
struct trail{
    int attitudeAngleType; // ��̬������ ��1  ���� flagZI[0]=1,  2 ���� flagZI[0]=0�� 3 �ֶ� �� 4 �Զ�
    double attitudeAngleX; // ��̬�� rx zixyz[0]
    double attitudeAngleY; // ��̬�� ry zixyz[1]
    double attitudeAngleZ; // ��̬�� rz zixyz[2]

    int tangentType; // 1 ����ѡ�� ���� flagZI[1]=1, 2 ����ѡ�� ���� flagZI[1]=0
    int faceType; // 1 �淽��ѡ�� ���� flagZI[2]=2, 2 �淽��ѡ�� ���� flagZI[2]=0,3 �淽��ѡ�� ����flagZI[2]=3 ,4 �淽��ѡ�� ����flagZI[2]=2
    double attitudePointX; // ��̬���������X  ziMian[0]
    double attitudePointY; // ��̬���������Y  ziMian[1]
    double attitudePointZ; // ��̬���������Y  ziMian[2]

    double attitudeAxisFirstX; // ��̬�����һ������X   ziMian[3]
    double attitudeAxisFirstY; // ��̬�����һ������Y    ziMian[4]
    double attitudeAxisFirstZ; // ��̬�����һ������Z    ziMian[5]
    double attitudeAxisSecondX; // ��̬����ڶ�������X   ziMian[6]
    double attitudeAxisSecondY; // ��̬����ڶ�������Y  ziMian[7]
    double attitudeAxisSecondZ; // ��̬����ڶ�������Z  ziMian[8]

};

// ���� NdivideNbs
struct trailback{
    int backType;	// �������ࣺ 1 ͨ�� flagNbsReverse[0] = 2 ��2 ȥë��  flagNbsReverse[0] = 1
    int attitudeAngleType; // ��̬������ ��1  ���� flagNbsReverse[1] =2,  2 ���� flagNbsReverse[1] =3�� 3 �ֶ� flagNbsReverse[1] =0�� 4 �Զ� flagNbsReverse[1] =1
    double attitudeAngleX; // ��̬�� rx  zixyz[0]
    double attitudeAngleY; // ��̬�� ry  zixyz[1]
    double attitudeAngleZ; // ��̬�� rz  zixyz[2]

    double contactPointX;	// �Ӵ���X����  toolANDzuan[0]
    double contactPointY;	// �Ӵ���Y����  toolANDzuan[1]
    double contactPointZ;	// �Ӵ���Z����  toolANDzuan[2]

    double flangePlateX;	// �����̽Ӵ�����X  toolANDzuan[0]
    double flangePlateY;	// �����̽Ӵ�����Y  toolANDzuan[1]
    double flangePlateZ;	// �����̽Ӵ�����Z  toolANDzuan[2]
    double cutRadius;	// �и�뾶    toolANDzuan[3]
    double cutCenterX;	// �и�Բ������X  toolANDzuan[4]
    double cutCenterY;	// �и�Բ������Y  toolANDzuan[5]
    double cutCenterZ;	// �и�Բ������Z  toolANDzuan[6]
    int closeGo;			// ��������Ƿ���� 1 �� int flagNbsReverse[2]= 1  2 ����  flagNbsReverse[2]= 0
    double goPercent;	// ���߰ٷֱ� toolANDzuan[7]

    double cutAngle;		// �и�Ƕ�  ZincluAngle
    int adapterType;		// ת��ͷ���緽�� 1 ��X���� flagZAngle[0]=0 2 ��X������flagZAngle[0]=1
    int inclinedAngleType;	//��б�Ƕ����� 1 ˳ʱ��flagZAngle[1]=0	2 ��ʱ�� flagZAngle[1]=1
};
class TrailTool : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TrailTool(QWidget *parent = 0);
    ~TrailTool();

    void showEvent(QShowEvent *);

signals:
    // ���߲岹
    void interpolationReverse(trailback back);
    // ���߲岹
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
