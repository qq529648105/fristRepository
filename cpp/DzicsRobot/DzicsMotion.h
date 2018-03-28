#ifndef TESTDLL_H
#define TESTDLL_H

#include<windows.h>

//bsplines ���ݽṹ��˵����
typedef struct bsplines {
    double **poles;//���ƶ��� ���� ��ͬ�� poles[][3]
    int polesLen;// poles��ά�����һά�ĳ���
    double *weights;//Ȩ��  ���� ��ͬ�� weights[]
    int weightLen;// weights����ĳ���
    double *knots;//�ڵ� ���� ��ͬ�� knots[]
    int knotsLen;// knots����ĳ���
    int *multiplicities;//���� ���� ��ͬ�� multiplicities[]
    int multipLen;// multiplicities����ĳ���
    int degree; // ����
    bool periodic;// ������
    bool rational;// ������
} bsplines;

//ʵ�ʿɵ��õĺ�����ǰ������������ֵ int 0 Ϊ��û�в岹��ɣ�1 Ϊ�Ѿ��岹���
//----------------------------------------------------------------------------------------------------------------------
//1.ֱ�߲岹˵��:
//part1����ʼ��
int WINAPI operate(double initialP[6], double endP[6], double TiSpe[4], double sample[7],int acc);
//initialP[0],initialP[1],initialP[2]�����ʼ��xyz���꣬initialP[3],initialP[4],initialP[5]�����ʼ����̬��RXYZֵ
//endP[0],endP[1],endP[2]���������xyz���꣬endP[3],endP[4],endP[5]�����������̬��RXYZֵ
//TiSpe[0]���ƶ��ٶȣ����ֵ1000����TiSpe[1]��̬��ת���ٶȣ����ֵ30��,TiSpe[2]����ʱ�䣬TiSpe[3]����ʱ��
//sample[0],sample[1],sample[2] ����xyz���꣬sample[3],sample[4],sample[5]����RXYZ��̬��ֵ�� ͨ������� Ϊ��ʼ����Ϣ
//acc= 0 Ϊ���ٷ����� acc= 1 Ϊ���μ��ٷ�����acc= 2 ΪS�μ��ٷ���

//part2��ʵʱȡ�㣺
int WINAPI getValueLine (double * sample);
//sample[0],sample[1],sample[2] ����xyz���꣬sample[3],sample[4],sample[5]����RXYZ��̬��ֵ
//----------------------------------------------------------------------------------------------------------------------
//2.Բ���岹˵��:
//part1����ʼ��
int WINAPI operateArc(double *initialP, double *midP, double *endP,double * TiSpe,double * sample,int acc);
//initialP[0],initialP[1],initialP[2]�����ʼ��xyz���꣬initialP[3],initialP[4],initialP[5]�����ʼ����̬��RXYZֵ
//midP[0],midP[1],midP[2]Ϊ Բ���м�� xyz���꣬����������̬�ǡ�
//endP[0],endP[1],endP[2]���������xyz���꣬endP[3],endP[4],endP[5]�����������̬��RXYZֵ
//TiSpe[0]���ƶ��ٶȣ����ֵ1000����TiSpe[1]��̬��ת���ٶȣ����ֵ30��,TiSpe[2]����ʱ�䣬TiSpe[3]����ʱ��
//sample[0],sample[1],sample[2] ����xyz���꣬sample[3],sample[4],sample[5]����RXYZ��̬��ֵ�� ͨ������� Ϊ��ʼ����Ϣ
//acc= 0 Ϊ���ٷ����� acc= 1 Ϊ���μ��ٷ�����acc= 2 ΪS�μ��ٷ���
//part2��ʵʱȡ�㣺
int WINAPI getValueArc (double * sample);
//sample[0],sample[1],sample[2] ����xyz���꣬sample[3],sample[4],sample[5]����RXYZ��̬��ֵ
//----------------------------------------------------------------------------------------------------------------------

//3.Nurbs���߲岹˵��:
//part1����ʼ��
int WINAPI dllNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],double *sample,int acc,int secNbs,int flagDui,double toolxyz[3]);
//nbsData���ڿ�ͷ˵��
//TiSpe[0]���ƶ��ٶȣ����ֵ1000��,TiSpe[1]����ʱ�䣬TiSpe[2]����ʱ��
//u0u1[0]��u0u1[1] Ϊ���������ٶȣ��;�ȷ�̶�  degree ��ʱ��u0u1[0]=0.01��u0u1[1]=0.001�� degree Сʱ��u0u1[0]=0.0001��u0u1[1]=0.00001
//zixyz[0]zixyz[1]zixyz[2] �����ʼ����̬��RXYZֵ
//sample[0],sample[1],sample[2] ����xyz���꣬sample[3],sample[4],sample[5]����RXYZ��̬��ֵ�� ͨ������� Ϊ��ʼ����Ϣ
//acc= 0 Ϊ���ٷ����� acc= 1 Ϊ���μ��ٷ�����acc= 2 ΪS�μ��ٷ���
//secNbsδ������������ʱ������0��

//part2��ʵʱȡ�㣺
int WINAPI getValueNbs(double * sample);
//sample[0],sample[1],sample[2] ����xyz���꣬sample[3],sample[4],sample[5]����RXYZ��̬��ֵ


//�����ȷֵ�  ���˺�������ֵ���岻ͬ��
double ** WINAPI divideNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],int * XYZLen,double timesP,int secNbs);
//��������ͬ����һ���������͡�����˵��
//timesP ÿ1���׾��� �ȷּ����㡣 ͨ�����ֵ 1��0.1 ֮�� �� 0.1��ζ�� 1���׵ȷ�һ���㡣���������岹�����ʱ����ֵ�������������ʱ����������
//XYZ[][6] ����ÿ�����ֵ�����磺XYZ[i][6] //XYZ[i][0],XYZ[i][1],XYZ[i][2] ����xyz���꣬XYZ[i][3],XYZ[i][4],XYZ[i][5]����RXYZ��̬��ֵ
//�˺������� int �ܹ��ĵ���
//----------------------------------------------------------------------------------------------------------------------

double ** WINAPI divideLine(double * initialP, double * endP,int * XYZLen, double timesP);
// ����������divideNbs��operate�·����У� ��Ҫ˵�����Ǵ�������ԽϿ졣timesP ����ȡ�󣬣����Ǵ��˻������˶����ˣ�

double ** WINAPI PdivideNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],int * XYZLen, double timesP,int secNbs,int flagZI[3],double ziMian[9]);
// ���˺�������������ĺ�divideNbsһ����
//int flagZI[0]=1 ��̬�Ǻͳ�ʼһ�� ��flagZI[0]=0 ʹ�ñ�����̬���㷨��  flagZI[1]=0 ��������̬��ʱ�������߱仯������Ӱ�졣flagZI[1]=1�����߷���ʼ��Ϊ��ʼ���߷���
//  flagZI[2]=0 ��ķ����Զ��ı䡣 flagZI[2]=1 ��ķ��� ��������ȷ����flagZI[2]=2 ��ķ���ʼ��Ϊ��ʼ��ķ��� flagZI[2]=3 ��ķ��� ��һ����ȷ����
// ziMian[0]��ziMian[1]��ziMian[2] ����һ����ȷ����xyz���ꡣ ziMian[3��4,5,6,7,8]����������ȷ����xyz����
double ** WINAPI NdivideNbs(bsplines nbsData, double TiSpe[], double u0u1[], double zixyz[], int * XYZLen, double timesP, int flagNbsReverse[4], double toolANDzuan[14], int flagZAngle[2], double ZincluAngle);
//����int flagNbsReverse ֮ǰ�� �� divideNbsһ���� ��int secNbsɾ����
//int flagNbsReverse[0] = 1 ѡ����ģʽ��ȥë��ģʽ�� int flagNbsReverse[1] =0 �Լ�������̬�ǲ���̬�ǲ����ֶ�ģʽ�� int flagNbsReverse[1] =1 ������̬����Ч���Զ�����������̬��ģʽ�� int flagNbsReverse[2]= 1 ��� ������ص�ԭ�������toolANDzuan[7]�����еİٷֱȡ�  int flagNbsReverse[2]= 0 �˹���ʧЧ��
//double toolANDzuan[0]��toolANDzuan[1],toolANDzuan[2] Ϊ���������꣬ toolANDzuan[3]�ӹ���ͷ�뾶�� toolANDzuan[4]��toolANDzuan[5],toolANDzuan[6]�ӹ���ͷ�뾶Բ��Բ������  toolANDzuan[7]��int flagNbsReverse[2]= 1�������ٷֱȣ�����0.1
//int flagZAngle[0] = 0 ����1 ��ͷ�Ӵ����л�   flagZAngle[1] = 0 ����1 ��ͷ�������б
//��б�Ƕ� double ZincluAngle;


//VB���õĺ������������ͣ���
int WINAPI operateArcC(double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed,double speed2,double * sample,int acc,double a1t,double a2t);
int WINAPI operateC(double x1,double y1,double z1,double x2,double y2,double z2,double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed,double speed2 , double * sample,int acc,double a1t,double a2t);
int WINAPI operateNbsC(double speed,double a1t,double a2t,double u1,double u2,double zx,double zy,double zz,double*sample,int acc,int secNbs);
int WINAPI operateNbs(double TiSpe[],double u0u1[],double zixyz[],double *sample,int acc,int secNbs);
int WINAPI operateEA(double * sample,int flagzi);
int WINAPI operateDiNbs(double * sample,int secNbs);
int WINAPI VBdivideLine(double * sample);
#endif

