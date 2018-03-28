#ifndef TRAJECTORYOBJECT_H
#define TRAJECTORYOBJECT_H

#include <QObject>
#include "DzicsMotion.h"
class TrajectoryObject : public QObject
{
    Q_OBJECT
public:
    explicit TrajectoryObject(QObject *parent = 0);
    ~TrajectoryObject();
    void serialize(bsplines nurbs, char fileName[256]);
    void deserialize(bsplines &nurbs, char fileName[256]);
public:
    QString id;
    QString stationId;
    int trajectoryNum;   				    //�켣����
    QString name;   				    //�켣����

signals:
    
public slots:
    
};


class TrajectoryDetail : public QObject
{
    Q_OBJECT
public:
    TrajectoryDetail(QObject *parent = 0);
    ~TrajectoryDetail();
    QString id;
    QString trajectoryId;
    QString name;                                 //·������
    //int trajectoryNum;                            //�켣��
    int pathType;                                 //·������

    QString pathFile;                           //·���ļ�λ��(����nurbs��������л�����ļ�λ��)
    int knifeType;
    double pointIndensity;                            //���ܶ�

    double cutRadius;                             //�и�뾶 tool[3]
    double cutDepth;        			         //�и���� tool[10]
    double cutAngle;        			      //�и�Ƕ�ZincluAngle
    int in1;                                 //�и��(1��ʾ���У�0��ʾ����)
    int out1;                                 //�и��(1��ʾ���У�0��ʾ����)
    double precision1;                            //����1��u0��
    double precision2;                            //����2��u1��

    double knifeX;                                //����X���� tool[4]
    double knifeY;                                //����Y���� tool[5]
    double knifeZ;                                //����Z���� tool[6]
    double knifeDirectionX;                            //���߷���X tool[17]
    double knifeDirectionY;                            //���߷���Y tool[18]
    double knifeDirectionZ;                            //���߷���Z tool[19]
    double longBase;                              //�������ݳ��� tool[7]
    double height;                                //�������ݸ� tool[8]
    double shortBase;                             //�������ݶ̱� tool[9]

    double FalaX;                                 //������X���� tool[0]
    double FalaY;                                 //������Y���� tool[1]
    double FalaZ;                                 //������Z���� tool[2]
    double FalaRX;                                //������RX��̬�� tool[14]
    double FalaRY;                                //������RY��̬��
    double FalaRZ;                                //������RZ��̬��

    double others1;                                 //����1 tool[11]
    double others2;                                 //����2 tool[12]
    double others3;                                 //����3 tool[13]
};

#endif // TRAJECTORYOBJECT_H
