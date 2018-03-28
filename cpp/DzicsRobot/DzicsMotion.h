#ifndef TESTDLL_H
#define TESTDLL_H

#include<windows.h>

//bsplines 数据结构的说明：
typedef struct bsplines {
    double **poles;//控制顶点 数组 等同于 poles[][3]
    int polesLen;// poles二维数组第一维的长度
    double *weights;//权重  数组 等同于 weights[]
    int weightLen;// weights数组的长度
    double *knots;//节点 数组 等同于 knots[]
    int knotsLen;// knots数组的长度
    int *multiplicities;//重数 数组 等同于 multiplicities[]
    int multipLen;// multiplicities数组的长度
    int degree; // 次数
    bool periodic;// 周期性
    bool rational;// 有理性
} bsplines;

//实际可调用的函数：前六个函数返回值 int 0 为还没有插补完成，1 为已经插补完成
//----------------------------------------------------------------------------------------------------------------------
//1.直线插补说明:
//part1：开始：
int WINAPI operate(double initialP[6], double endP[6], double TiSpe[4], double sample[7],int acc);
//initialP[0],initialP[1],initialP[2]输入初始点xyz坐标，initialP[3],initialP[4],initialP[5]输入初始点姿态角RXYZ值
//endP[0],endP[1],endP[2]输入结束点xyz坐标，endP[3],endP[4],endP[5]输入结束点姿态角RXYZ值
//TiSpe[0]点移动速度（最大值1000），TiSpe[1]姿态角转动速度（最大值30）,TiSpe[2]加速时间，TiSpe[3]减速时间
//sample[0],sample[1],sample[2] 返回xyz坐标，sample[3],sample[4],sample[5]返回RXYZ姿态角值， 通常情况下 为初始点信息
//acc= 0 为匀速方案， acc= 1 为梯形加速方案，acc= 2 为S形加速方案

//part2：实时取点：
int WINAPI getValueLine (double * sample);
//sample[0],sample[1],sample[2] 返回xyz坐标，sample[3],sample[4],sample[5]返回RXYZ姿态角值
//----------------------------------------------------------------------------------------------------------------------
//2.圆弧插补说明:
//part1：开始：
int WINAPI operateArc(double *initialP, double *midP, double *endP,double * TiSpe,double * sample,int acc);
//initialP[0],initialP[1],initialP[2]输入初始点xyz坐标，initialP[3],initialP[4],initialP[5]输入初始点姿态角RXYZ值
//midP[0],midP[1],midP[2]为 圆弧中间点 xyz坐标，无需输入姿态角。
//endP[0],endP[1],endP[2]输入结束点xyz坐标，endP[3],endP[4],endP[5]输入结束点姿态角RXYZ值
//TiSpe[0]点移动速度（最大值1000），TiSpe[1]姿态角转动速度（最大值30）,TiSpe[2]加速时间，TiSpe[3]减速时间
//sample[0],sample[1],sample[2] 返回xyz坐标，sample[3],sample[4],sample[5]返回RXYZ姿态角值， 通常情况下 为初始点信息
//acc= 0 为匀速方案， acc= 1 为梯形加速方案，acc= 2 为S形加速方案
//part2：实时取点：
int WINAPI getValueArc (double * sample);
//sample[0],sample[1],sample[2] 返回xyz坐标，sample[3],sample[4],sample[5]返回RXYZ姿态角值
//----------------------------------------------------------------------------------------------------------------------

//3.Nurbs曲线插补说明:
//part1：开始：
int WINAPI dllNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],double *sample,int acc,int secNbs,int flagDui,double toolxyz[3]);
//nbsData已在开头说明
//TiSpe[0]点移动速度（最大值1000）,TiSpe[1]加速时间，TiSpe[2]减速时间
//u0u1[0]，u0u1[1] 为调节运算速度，和精确程度  degree 大时，u0u1[0]=0.01，u0u1[1]=0.001； degree 小时，u0u1[0]=0.0001，u0u1[1]=0.00001
//zixyz[0]zixyz[1]zixyz[2] 输入初始点姿态角RXYZ值
//sample[0],sample[1],sample[2] 返回xyz坐标，sample[3],sample[4],sample[5]返回RXYZ姿态角值， 通常情况下 为初始点信息
//acc= 0 为匀速方案， acc= 1 为梯形加速方案，acc= 2 为S形加速方案
//secNbs未开发完整，暂时请输入0；

//part2：实时取点：
int WINAPI getValueNbs(double * sample);
//sample[0],sample[1],sample[2] 返回xyz坐标，sample[3],sample[4],sample[5]返回RXYZ姿态角值


//独立等分点  （此函数返回值意义不同）
double ** WINAPI divideNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],int * XYZLen,double timesP,int secNbs);
//数据输入同上面一样不做解释。补充说明
//timesP 每1毫米距离 等分几个点。 通常这个值 1到0.1 之间 ， 0.1意味着 1厘米等分一个点。（当整个插补距离大时，此值如果大会造成运算时间严重增大）
//XYZ[][6] 返回每个点的值，例如：XYZ[i][6] //XYZ[i][0],XYZ[i][1],XYZ[i][2] 返回xyz坐标，XYZ[i][3],XYZ[i][4],XYZ[i][5]返回RXYZ姿态角值
//此函数返回 int 总共的点数
//----------------------------------------------------------------------------------------------------------------------

double ** WINAPI divideLine(double * initialP, double * endP,int * XYZLen, double timesP);
// 参数解释在divideNbs，operate下方都有， 需要说明的是此运算相对较快。timesP 可以取大，（但是大了机器就运动慢了）

double ** WINAPI PdivideNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],int * XYZLen, double timesP,int secNbs,int flagZI[3],double ziMian[9]);
// 除了后面两个参数别的和divideNbs一样。
//int flagZI[0]=1 姿态角和初始一样 ，flagZI[0]=0 使用保持姿态角算法。  flagZI[1]=0 当保持姿态角时，收切线变化参数的影响。flagZI[1]=1，切线方向始终为初始切线方向。
//  flagZI[2]=0 面的方向自动改变。 flagZI[2]=1 面的方向 由两个点确定。flagZI[2]=2 面的方向始终为初始面的方向。 flagZI[2]=3 面的方向 由一个点确定。
// ziMian[0]，ziMian[1]，ziMian[2] 面由一个点确定的xyz坐标。 ziMian[3，4,5,6,7,8]面由两个点确定的xyz坐标
double ** WINAPI NdivideNbs(bsplines nbsData, double TiSpe[], double u0u1[], double zixyz[], int * XYZLen, double timesP, int flagNbsReverse[4], double toolANDzuan[14], int flagZAngle[2], double ZincluAngle);
//参数int flagNbsReverse 之前的 和 divideNbs一样， 把int secNbs删除。
//int flagNbsReverse[0] = 1 选择反走模式加去毛刺模式。 int flagNbsReverse[1] =0 自己输入姿态角并姿态角不变手动模式。 int flagNbsReverse[1] =1 输入姿态角无效，自动给出合适姿态角模式。 int flagNbsReverse[2]= 1 如果 结束点回到原点则多跑toolANDzuan[7]参数中的百分比。  int flagNbsReverse[2]= 0 此功能失效。
//double toolANDzuan[0]，toolANDzuan[1],toolANDzuan[2] 为发那盘坐标， toolANDzuan[3]加工钻头半径， toolANDzuan[4]，toolANDzuan[5],toolANDzuan[6]加工钻头半径圆的圆心坐标  toolANDzuan[7]当int flagNbsReverse[2]= 1，超出百分比，比如0.1
//int flagZAngle[0] = 0 或者1 钻头接触点切换   flagZAngle[1] = 0 或者1 钻头里或外倾斜
//倾斜角度 double ZincluAngle;


//VB调用的函数（不做解释）：
int WINAPI operateArcC(double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed,double speed2,double * sample,int acc,double a1t,double a2t);
int WINAPI operateC(double x1,double y1,double z1,double x2,double y2,double z2,double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed,double speed2 , double * sample,int acc,double a1t,double a2t);
int WINAPI operateNbsC(double speed,double a1t,double a2t,double u1,double u2,double zx,double zy,double zz,double*sample,int acc,int secNbs);
int WINAPI operateNbs(double TiSpe[],double u0u1[],double zixyz[],double *sample,int acc,int secNbs);
int WINAPI operateEA(double * sample,int flagzi);
int WINAPI operateDiNbs(double * sample,int secNbs);
int WINAPI VBdivideLine(double * sample);
#endif

