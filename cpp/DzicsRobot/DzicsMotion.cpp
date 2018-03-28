#include "DzicsMotion.h"
#include <math.h>
#include "time.h"
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include <iostream>
using namespace std;
double localTime1 = 0;
double recordTime1 = 0 ;	
double localTime2 = 0;
double recordTime2 = 0 ;	
static time_t start;
time_t finish = 0;
time_t difference =0 ;
int seconds = 0;
static int toggle= 0;
#define N 3
#define M 3
#define K 4
double Lx1,Ly1,Lz1,Lx2,Ly2,Lz2,Lvx,Lvy,Lvz,l_square,Ll,Lt,accT,localS,Lspeed;
double PI = 3.1415926535898;
double Ax0,Ay0,Az0,Ax1,Ay1,Az1,Ax2,Ay2,Az2,Aspeed,Axc,Ayc,Azc,anglepq,anglerq,anglepr,angle,perimeter,arcLength,periodAngle,Nangle,Tperiod,radius;
double acctt1,acctt2 ,acctt3,acceleration,deceleration;
double St1,aGradient,Svc2,Sl1,Slc2,Sdt,Sl2,Stt1,Stt2,Stt3,Stt4,Stt5,Stt,acceleration2,deceleration2,dGradient;
int status ;
double Edx,Edy,Edz,Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,Edl,Lspeed2,Elt;
int flagOver;
double e2angle,fx1,fy1,fz1,RsMatrix[3][3],EccT,Ecctt1,Ecctt2,Ecctt3,Ett,Ett1,Ett2,Ett3,Ett4,Ett5;
double getEulerAg(double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed2);
void operateE(double ddangle,double * sample);
double findStayTime(double speed,double a1t,double a2t,int acc,double Ll_local);
int WPR0=0;
double glou[3]={-10,-10,-10};
double gloacmi = 0;
double Ndets = 0.0001;
int Ntimesu,NnbsK,NnbsN;
double * Nstoremi;
double Nminu,Nmaxu,Ntryu,Ndtryu;
int vbjj=0;
//double NxPoles[],NyPoles[],NzPoles[];

/*
double Nweights[5] = {1,0.707107,1,0.707107,1};
double Nknots[8] ={3.14159,3.14159,3.14159,4.71239,4.71239,6.28319,6.28319,6.28319};
double NdPoles[5][3]={{-1,-1.22465e-016,0},{-1,1,0},{-1.83697e-016,1,0},{1,1,0},{1,2.44929e-016,0}};
double NxPoles[5]={-10,-10,-1.83697e-015,10,10};
double NyPoles[5]={-1.22465e-015,10,10,10,2.44929e-015};
double NzPoles[5]={0,0,0,0,0};

double Nweights[7] = {1,1,1,1,1,1,1};
double Nknots[14] ={0,0,0,0,0,0,0,1,1,1,1,1,1,1};
double NdPoles[7][3]={{879,-51,493},{872.981,-3.27981,409.016},{999.266,-127.95,389.191},{671.048,790.412,251.568},{1019.14,-643.413,241.455},{768.737,290.01,114.09},{835,-51,50}};
double NxPoles[7]={879,872.981,999.266,671.048,1019.14,768.737,835};
double NyPoles[7]={-51,-3.27981,-127.95,790.412,-643.413,290.01,-51};
double NzPoles[7]={493,409.016,389.191,251.568,241.455,114.09,50};
double Nweights[8] = {1,1,1,1,1,1,1,1};
double Nknots[16] ={0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
double NdPoles[8][3]={{885,-35,417},{885,-12.6181,416.984},{885,9.69331,411.299},{885,30.9787,399.046},{885,45.8919,383.194},{885,60.9466,361.281},{885,63.8384,339.748},{885,65,317}};
double NxPoles[8]={885,885,885,885,885,885,885,885};
double NyPoles[8]={-35,-12.6181,9.69331,30.9787,45.8919,60.9466,63.8384,65};
double NzPoles[8]={417,416.984,411.299,399.046,383.194,361.281,339.748,317};
*/

double NNweights[22] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
double NNknots[34] ={0,0,0,0,0,0,0,0,0,0,0,0,6.28319,6.28319,6.28319,6.28319,6.28319,6.28319,6.28319,6.28319,6.28319,6.28319,12.5664,12.5664,12.5664,12.5664,12.5664,12.5664,12.5664,12.5664,12.5664,12.5664,12.5664,12.5664};
double NNdPoles[22][3]={{7.66693,-1.79828,75.1858},{6.67742,-6.01704,75.0326},{3.13477,-9.63686,74.8793},{-2.56207,-10.9407,74.7261},{-8.69245,-8.51377,74.5728},{-12.577,-2.38241,74.4195},{-11.7007,5.19145,74.2663},{-6.51864,10.2738,74.113},{0.00395772,11.2367,73.9598},{5.25255,8.6664,73.8065},{7.875,4.33325,73.6533},{7.875,-4.33325,73.3467},{5.25255,-8.6664,73.1935},{0.00395772,-11.2367,73.0402},{-6.51864,-10.2738,72.887},{-11.7007,-5.19145,72.7337},{-12.577,2.38241,72.5805},{-8.69245,8.51377,72.4272},{-2.56207,10.9407,72.2739},{3.13477,9.63686,72.1207},{6.67742,6.01704,71.9674},{7.66693,1.79828,71.8142}};
double NNxPoles[22]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double NNyPoles[22]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double NNzPoles[22]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

double *Nweights;
double *Nknots;
double **NdPoles;
double *NxPoles;
double *NyPoles;
double *NzPoles;



double ** XYZ = (double**)malloc(10000*sizeof(double*));
double Fq[3];
double Cq[3];
double Fzi[3];
double Fm[3];
double Cm[3];
double jax1,jay1,jaz1,jax2,jay2,jaz2,jax3,jay3,jaz3;
int jj = 0;
//--------------------------------------------------
void updateNbs(double xplus,double yplus,double zplus,double times,double ** Zxyz,int n){
NxPoles= (double*) malloc(sizeof(double)*n);
NyPoles= (double*) malloc(sizeof(double)*n);
NzPoles= (double*) malloc(sizeof(double)*n);
for (int iii=0;iii<n;iii++){
NxPoles[iii]=Zxyz[iii][0]*times+xplus;
NyPoles[iii]=Zxyz[iii][1]*times+yplus;
NzPoles[iii]=Zxyz[iii][2]*times+zplus;
}
}

void thirdline(double xline[],double yline[],double zline[]){
zline[0]= xline[1]*yline[2]-xline[2]*yline[1];
zline[1]= xline[2]*yline[0]-xline[0]*yline[2];
zline[2]= xline[0]*yline[1]-xline[1]*yline[0];
}

double angleCal(double Angle)
{
	return Angle * PI / 180;
}
void transpose(double a[M][M], double b[M][M])
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			b[i][j]=a[j][i];
}
void quaternionToRMatrix(double Quaternion[4], double RMatrix[3][3])
{
	double q0, q1, q2, q3;
	q0 = Quaternion[0]; q1 = Quaternion[1]; q2 = Quaternion[2]; q3 = Quaternion[3];
	double TRMatrix[3][3] = {};
	TRMatrix[0][0] = pow(q0,2) + pow(q1,2) - pow(q2,2) - pow(q3,2);
	TRMatrix[0][1] = 2*(q1*q2 - q0*q3);
	TRMatrix[0][2] = 2*(q1*q3 + q0*q2);
	TRMatrix[1][0] = 2*(q1*q2 + q0*q3);
	TRMatrix[1][1] = pow(q0,2) + pow(q2,2) - pow(q1,2) - pow(q3,2);
	TRMatrix[1][2] = 2*(q2*q3 - q0*q1);
	TRMatrix[2][0] = 2*(q1*q3 - q0*q2);
	TRMatrix[2][1] = 2*(q2*q3 + q0*q1);
	TRMatrix[2][2] = pow(q0,2) + pow(q3,2) - pow(q1,2) - pow(q2,2);

	double TTRMatrix[3][3] = {};
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			TTRMatrix[i][j] = TRMatrix[2-i][2-j];

	transpose(TTRMatrix, RMatrix);
}

void rmatrixToWPR(double RMatrix[3][3], double WPR[3])
{
	double Nx,Ny,Nz,Ox,Oy,Oz,Ax,Ay,Az,WAngle,PAngle,RAngle;
	Nx = RMatrix[0][0]; Ny = RMatrix[1][0]; Nz = RMatrix[2][0];
	Ox = RMatrix[0][1]; Oy = RMatrix[1][1]; Oz = RMatrix[2][1];
	Ax = RMatrix[0][2]; Ay = RMatrix[1][2]; Az = RMatrix[2][2];

	RAngle = 0;
	/*if (fabs(Nx2)>1.0e-5)*/
	RAngle = atan2(Ny, Nx) * 180 / PI;
	WAngle = atan2(Ax * sin(angleCal(RAngle)) - Ay * cos(angleCal(RAngle)), Oy * cos(angleCal(RAngle)) - Ox * sin(angleCal(RAngle))) * 180 / PI;
	PAngle = atan2(-1*Nz, Nx * cos(angleCal(RAngle)) + Ny * sin(angleCal(RAngle))) * 180 / PI;

	WPR[0] = WAngle; WPR[1] = PAngle; WPR[2] = RAngle;
}
void WPRZXZTormatrix(double WPR[3],double RMatrix[3][3]){
double A1,A2,A3;
A1 = angleCal(WPR[0]);
A2 = angleCal(WPR[1]);
A3 = angleCal(WPR[2]);
RMatrix[0][0]= cos(A3)*cos(A1)*cos(A2)-sin(A3)*sin(A1);
RMatrix[0][1]= -cos(A3)*sin(A1)-cos(A1)*cos(A2)*sin(A3);
RMatrix[0][2]= cos(A1)*sin(A2);
RMatrix[1][0]= cos(A1)*sin(A3)+cos(A3)*cos(A2)*sin(A1);
RMatrix[1][1]= cos(A3)*cos(A1)-cos(A2)*sin(A3)*sin(A1);
RMatrix[1][2]= sin(A1)*sin(A2);
RMatrix[2][0]= -cos(A3)*sin(A2);
RMatrix[2][1]= sin(A3)*sin(A2);
RMatrix[2][2]= cos(A2);
}
void WPRZYXTormatrix(double WPR[3],double RMatrix[3][3]){
double A1,A2,A3;
A3 = angleCal(WPR[0]);
A2 = angleCal(WPR[1]);
A1 = angleCal(WPR[2]);
RMatrix[0][0]= cos(A1)*cos(A2);
RMatrix[0][1]= cos(A1)*sin(A2)*sin(A3)-cos(A3)*sin(A1);
RMatrix[0][2]= sin(A1)*sin(A3)+cos(A1)*cos(A3)*sin(A2);
RMatrix[1][0]= cos(A2)*sin(A1);
RMatrix[1][1]= cos(A1)*cos(A3)+sin(A1)*sin(A2)*sin(A3);
RMatrix[1][2]= cos(A3)*sin(A1)*sin(A2)-cos(A1)*sin(A3);
RMatrix[2][0]= -sin(A2);
RMatrix[2][1]= cos(A2)*sin(A3);
RMatrix[2][2]= cos(A2)*cos(A3);
}

void WPRZXYTormatrix(double WPR[3],double RMatrix[3][3]){
double A1,A2,A3;
A1 = angleCal(WPR[0]);
A2 = angleCal(WPR[1]);
A3 = angleCal(WPR[2]);
RMatrix[0][0]= cos(A1)*cos(A3)-sin(A1)*sin(A2)*sin(A3);
RMatrix[0][1]= -cos(A2)*sin(A1);
RMatrix[0][2]= cos(A1)*sin(A3)+cos(A3)*sin(A1)*sin(A2);
RMatrix[1][0]= cos(A3)*sin(A1)+cos(A1)*sin(A2)*sin(A3);
RMatrix[1][1]= cos(A1)*cos(A2);
RMatrix[1][2]= sin(A1)*sin(A3)-cos(A1)*cos(A3)*sin(A2);
RMatrix[2][0]= -cos(A2)*sin(A3);
RMatrix[2][1]= sin(A2);
RMatrix[2][2]= cos(A2)*cos(A3);
}

void angleToQuaternion(double WPR[3], double Quaternion[4])
{
	double VW,VP,VR;
	VW = WPR[0]; VP = WPR[1]; VR = WPR[2];
	Quaternion[0] = cos(angleCal(VR/2))*cos(angleCal(VP/2))*cos(angleCal(VW/2)) + sin(angleCal(VR/2))*sin(angleCal(VP/2))*sin(angleCal(VW/2));
	Quaternion[1] = sin(angleCal(VR/2))*cos(angleCal(VP/2))*cos(angleCal(VW/2)) - cos(angleCal(VR/2))*sin(angleCal(VP/2))*sin(angleCal(VW/2));
	Quaternion[2] = cos(angleCal(VR/2))*sin(angleCal(VP/2))*cos(angleCal(VW/2)) + sin(angleCal(VR/2))*cos(angleCal(VP/2))*sin(angleCal(VW/2));
	Quaternion[3] = cos(angleCal(VR/2))*cos(angleCal(VP/2))*sin(angleCal(VW/2)) - sin(angleCal(VR/2))*sin(angleCal(VP/2))*cos(angleCal(VW/2));
} 

void quaternionToAngle(double Quaternion[4], double WPR[3])
{
	double RMatrix[3][3] = {};
	quaternionToRMatrix(Quaternion, RMatrix);
	rmatrixToWPR(RMatrix, WPR);
}


//-----------------------------------------------------
//time counter 
int seccounter(void){

	if(toggle==0){
		start= clock();
		toggle=1;
		return 0;
	}

	if (toggle==1){
		finish=clock();
		difference= start-finish;
		toggle=1;
		if (difference<0){
			difference= 0 - difference;
		}
		return (int)difference ;
	}
}
//--------------------------------------------------
// 3*3 martix multiplying

void mult_matrices(double a[3][3], double b[3], double result[3])   //3*3  *  3*1 matrix 
{
    result[0]=0;
	result[1]=0;
	result[2]=0;
	int i, j;
    for(i = 0; i < 3; i++)
    {
		for(j = 0; j < 3; j++)
		{
		result[i] +=  a[i][j] *  b[j];
		}    
    }
}
//-----------------------------------------------------
void mult_matrices2(double a[4][4], double b[4], double result[4])   //4*4  *  4*1 matrix 
{
	result[0]=0;
	result[1]=0;
	result[2]=0;
	result[3]=0;
    int i, j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++)
		{
		result[i] +=  a[i][j] *  b[j];
		}    
    }
}
//--------------------------------------------------
void inverseMatrix(double a[3][3], double result[3][3]){
 
  int i,j;
  double determinant=0;

  for(i=0;i<3;i++){
      determinant = determinant + (a[0][i]*(a[1][(i+1)%3]*a[2][(i+2)%3] - a[1][(i+2)%3]*a[2][(i+1)%3]));
  }
   for(i=0;i<3;i++){
      for(j=0;j<3;j++)
           result[i][j] += ((a[(i+1)%3][(j+1)%3] * a[(i+2)%3][(j+2)%3]) - (a[(i+1)%3][(j+2)%3]*a[(i+2)%3][(j+1)%3]))/ determinant;
      
   }
}
//--------------------------------------------------
// calculate the cofactor of element (row,col)
int GetMinor(double **src, double **dest, int row, int col, int order)
{
    // indicate which col and row is being copied to dest
    int colCount=0,rowCount=0;

    for(int i = 0; i < order; i++ )
    {
        if( i != row )
        {
            colCount = 0;
            for(int j = 0; j < order; j++ )
            {
                // when j is not the element
                if( j != col )
                {
                    dest[rowCount][colCount] = src[i][j];
                    colCount++;
                }
            }
            rowCount++;
        }
    }

    return 1;
}
//----------------------------------------------------
// Calculate the determinant recursively.
double CalcDeterminant( double **mat, int order)
{
    // order must be >= 0
	// stop the recursion when matrix is a single element
    if( order == 1 )
        return mat[0][0];

    // the determinant value
    double det = 0;

    // allocate the cofactor matrix
    double **minor;
    minor = new double*[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = new double[order-1];

    for(int i = 0; i < order; i++ )
    {
        // get minor of element (0,i)
        GetMinor( mat, minor, 0, i , order);
        // the recusion is here!

        det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,order-1);
        //det += pow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
    }

    // release memory
    for(int i=0;i<order-1;i++)
        delete [] minor[i];
    delete [] minor;

    return det;
}
//---------------------------------------------------
void MatrixInversion(double **A, int order, double **Y)
{
    // get the determinant of a
    double det = 1.0/CalcDeterminant(A,order);

    // memory allocation
    double *temp = new double [(order-1)*(order-1)];
    double **minor = new double *[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = temp+(i*(order-1));

    for(int j=0;j<order;j++)
    {
        for(int i=0;i<order;i++)
        {
            // get the co-factor (matrix) of A(j,i)
            GetMinor(A,minor,j,i,order);
            Y[i][j] = det*CalcDeterminant(minor,order-1);
            if( (i+j)%2 == 1)
                Y[i][j] = -Y[i][j];
        }
    }

    // release memory
    //delete [] minor[0];
    delete [] temp;
    delete [] minor;
}
//--------------------------------------------------

int brinv(double a[], int n)
{
	int *is,*js,i,j,k,l,u,v;
	double d,p;
	is=(int*)malloc(n*sizeof(int));
    js=(int*)malloc(n*sizeof(int));
	for (k=0; k<=n-1; k++)
	{
		d=0.0;
        for (i=k; i<=n-1; i++)
			for (j=k; j<=n-1; j++)
			{
				l=i*n+j; p=fabs(a[l]);
				if (p>d) 
				{ 
					d=p; 
					is[k]=i; 
					js[k]=j;
				}
			}
		if (d+1.0==1.0)
        { 
			free(is); 
			free(js); 
			printf("err**not inv\n");
            return(0);
        }
        if (is[k]!=k)
        for (j=0; j<=n-1; j++)
        { 
			u=k*n+j; 
			v=is[k]*n+j;
            p=a[u]; 
			a[u]=a[v]; 
			a[v]=p;
        }
        if (js[k]!=k)
        for (i=0; i<=n-1; i++)
        { 
			u=i*n+k; 
			v=i*n+js[k];
            p=a[u];
			a[u]=a[v];
			a[v]=p;
        }
		l=k*n+k;
        a[l]=1.0/a[l];
        for (j=0; j<=n-1; j++)
			if (j!=k)
            { 
				u=k*n+j; 
				a[u]=a[u]*a[l];
			}
        for (i=0; i<=n-1; i++)
			if (i!=k)
				for (j=0; j<=n-1; j++)
					if (j!=k)
						{ 
							u=i*n+j;
							a[u]=a[u]-a[i*n+k]*a[k*n+j];
						}
		for (i=0; i<=n-1; i++)
			if (i!=k)
            { 
				u=i*n+k; 
				a[u]=-a[u]*a[l];
			}
	}

	for (k=n-1; k>=0; k--)
	{ 
		if (js[k]!=k)
			for (j=0; j<=n-1; j++)
			{	
				u=k*n+j; 
				v=js[k]*n+j;
				p=a[u]; 
				a[u]=a[v];
				a[v]=p;
			}
		if (is[k]!=k)
			for (i=0; i<=n-1; i++)
			{ 
				u=i*n+k; 
				v=i*n+is[k];
				p=a[u]; 
				a[u]=a[v]; 
				a[v]=p;
			}
	}

    free(is); 
	free(js);

    return(1);
}

void brmul(double a[], double b[],int m,int n,int k,double c[])
{ 
	int i,j,l,u;
    for (i=0; i<=m-1; i++)
    for (j=0; j<=k-1; j++)
	{ 
		u=i*k+j; 
		c[u]=0.0;
		for (l=0; l<=n-1; l++)
			c[u]=c[u]+a[i*n+l]*b[l*k+j];
	}

    return;
}

void matrixMultiply(double M1[N][N], double M2[N][N], double MMResult[N][N])
{
	double temp;
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
		{
			temp = 0.0;
			for (int k=0; k<N; k++)
			{
				temp += double(M1[i][k])*double(M2[k][j]);
			}
			MMResult[i][j] = temp;
 		}	
	}				
}

void matrixMultiply2(double M1[M][M], double M2[M][M], double MMResult[M][M])
{
	double temp;
	for (int i=0; i<M; i++)
	{
		for (int j=0; j<M; j++)
		{
			temp = 0.0;
			for (int k=0; k<M; k++)
			{
				temp += double(M1[i][k])*double(M2[k][j]);
			}
			MMResult[i][j] = temp;
 		}	
	}				
}
void matrixMultiply3(double K1[K][K], double K2[K][K], double KKResult[K][K])
{
	double temp;
	for (int i=0; i<K; i++)
	{
		for (int j=0; j<K; j++)
		{
			temp = 0.0;
			for (int k=0; k<K; k++)
			{
				temp += double(K1[i][k])*double(K2[k][j]);
			}
			KKResult[i][j] = temp;
 		}	
	}				
}
//--------------------------------------------------------------------
double * calCircle(double point1[3], double point2[3], double point3[3])    //根据在3D的三点，求得 三点共圆的圆心坐标 和 半径。
{
	double  x1, x2, x3, y1, y2, y3, z1, z2, z3, A1, B1, C1, D1, A2, B2, C2, D2, A3, B3, C3, D3;
	x1 = point1[0]; y1 = point1[1]; z1 = point1[2];
	x2 = point2[0]; y2 = point2[1]; z2 = point2[2];
	x3 = point3[0]; y3 = point3[1]; z3 = point3[2];
	double * result = (double*)malloc(4*sizeof(double));
	double m12 = sqrt(pow(x2-x1, 2) + pow(y2-y1, 2) + pow(z2 - z1, 2)); 
    double m13 = sqrt(pow(x3-x1, 2) + pow(y3-y1, 2) + pow(z3 - z1, 2)); 
	double vv = (x2-x1)*(x3-x1) + (y2-y1)*(y3-y1) + (z2-z1)*(z3-z1);
	if (0==m12 || 0==m13 || 1==vv/(m12*m13) || -1==vv/(m12*m13))
	{
		result[0] = -1000;
		result[1] = -1000;
		result[2] = -1000;
		result[3] = -1000;
		return result;
	}

	A1 = y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2;
    B1 = -x1*z2 + x2*z1 + x1*z3 - x3*z1 - x2*z3 + x3*z2;
    C1 = x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2;
    D1 = - x1*y2*z3 + x1*y3*z2 + x2*y1*z3 - x2*y3*z1 - x3*y1*z2 + x3*y2*z1;
	A2 = 2*(x2-x1);
    B2 = 2*(y2-y1);
    C2 = 2*(z2-z1);
    D2 = pow(x1,2) + pow(y1,2) + pow(z1,2) - pow(x2,2) - pow(y2,2) - pow(z2,2);
	A3 = 2*(x3-x1);
    B3 = 2*(y3-y1);
    C3 = 2*(z3-z1);
    D3 = pow(x1,2) + pow(y1,2) + pow(z1,2) - pow(x3,2) - pow(y3,2) - pow(z3,2);

	double ABC[3][3] = { {A1, B1, C1}, {A2, B2, C2}, {A3, B3, C3} };
	double D[3] = { -D1, -D2, -D3 };
	brinv(ABC[0],3);
	
	double S[3] = {};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			S[i] += ABC[i][j] * D[j];

	double R = sqrt(pow(x1-S[0], 2) + pow(y1-S[1], 2) + pow(z1-S[2], 2));
	result[0] = S[0];
	result[1] = S[1];
	result[2] = S[2];
	result[3] = R;

	return result;
}

//----------------------------------------------------     
double calAngle(double point1[3], double point2[3],double CentrePoint3[3])   //  根据两点和圆心，求的初始点Point1和末位点point2之间形成的角度。
{
double x0,y0,z0,x2,y2,z2,xc,yc,zc;
x0=point1[0];
y0=point1[1];
z0=point1[2];
x2=point2[0];
y2=point2[1];
z2=point2[2];
xc=CentrePoint3[0];
yc=CentrePoint3[1];
zc=CentrePoint3[2];
double dx1,dy1,dz1,dx2,dy2,dz2;
dx1=(xc-x0);
dy1=(yc-y0);
dz1=(zc-z0);
dx2=(xc-x2);
dy2=(yc-y2);
dz2=(zc-z2);
double uv = dx1*dx2 + dy1*dy2 + dz1*dz2;
double uSquare = dx1*dx1 + dy1*dy1 + dz1*dz1;
double vSquare = dx2*dx2 + dy2*dy2 + dz2*dz2;
double cosangle= uv / (sqrt(uSquare)* sqrt(vSquare));
double angle = acos(cosangle);
return angle;
}
//
double correctAngle(double angle1,double angle2,double angle3)    //  根据三点之间相互的夹角，判断出准确的初始点和末位点之间的夹角。
{
double PI = 3.141592653;
	if (((angle1+angle2)< (angle3+0.000001)) &&((angle1+angle2)> (angle3-0.000001)) )  {
		return angle1+angle2;
	}
	if((((angle1+angle2+angle3)/2)< (PI+0.0000001)) &&(((angle1+angle2+angle3)/2)> (PI-0.0000001)))
	{
		return angle1+angle2;
	}
	else {
		return 2*PI-angle3;
	}
}
//---------------------------------------------------
double * newArcXYZ (double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,double xi,double yi,double zi,double xc,double yc,double zc,double R,double periodT,double speed)
//根据 三点和圆点， 求得当前点（x0,y0,z0）在单位时间内的位移后的坐标
{
	double u= (y1-y0)*(z2-z1)-(z1-z0)*(y2-y1);
	double v= (z1-z0)*(x2-x1)-(x1-x0)*(z2-z1);
	double w= (x1-x0)*(y2-y1)-(y1-y0)*(x2-x1);
	double mi= v*(zi-zc)-w*(yi-yc);
	double ni= w*(xi-xc)-u*(zi-zc);
	double li= u*(yi-yc)-v*(xi-xc);
	double unitMNL = 1/ sqrt(mi*mi+ni*ni+li*li) ;
	double d = periodT * speed;
	double dx= periodT * speed * mi * unitMNL ;
	double dy= periodT * speed * ni * unitMNL ;
	double dz= periodT * speed * li * unitMNL ;
	double newx = dx + xi ;
	double newy = dy + yi ;
	double newz = dz + zi ;
	double G = R/sqrt(R*R+d*d);
	double cx = xc + G* (newx - xc);
	double cy = yc + G* (newy - yc);
	double cz = zc + G* (newz - zc);
	double * newXYZ = (double*)malloc(3*sizeof(double));
	newXYZ[0]= cx;
	newXYZ[1]= cy;
	newXYZ[2]= cz;
	return newXYZ;
}
//--------------------------------------------------
double * newArcXYZ2 (double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,double xi,double yi,double zi,double xc,double yc,double zc,double R,double angle)
//根据 三点和圆点， 求得当前点（x0,y0,z0）在单位时间内的位移后的坐标
{
	double u= (y1-y0)*(z2-z1)-(z1-z0)*(y2-y1);
	double v= (z1-z0)*(x2-x1)-(x1-x0)*(z2-z1);
	double w= (x1-x0)*(y2-y1)-(y1-y0)*(x2-x1);
	double mi= v*(zi-zc)-w*(yi-yc);
	double ni= w*(xi-xc)-u*(zi-zc);
	double li= u*(yi-yc)-v*(xi-xc);
	double unitMNL = 1/ sqrt(mi*mi+ni*ni+li*li) ;
	double d = tan(angle)*R ;
	double dx= d * mi * unitMNL ;
	double dy= d * ni * unitMNL ;
	double dz= d * li * unitMNL ;
	double newx = dx + xi ;
	double newy = dy + yi ;
	double newz = dz + zi ;
	double G = R/sqrt(R*R+d*d);
	double cx = xc + G* (newx - xc);
	double cy = yc + G* (newy - yc);
	double cz = zc + G* (newz - zc);
	double * newXYZ = (double*)malloc(3*sizeof(double));
	newXYZ[0]= cx;
	newXYZ[1]= cy;
	newXYZ[2]= cz;
	return newXYZ;
//-----------------------------------------------------
}double getL (double dt,int acc){

if (acc==0){
	double resultL;
	resultL= dt *Lspeed;
	return resultL;
}

if (acc== 1){         // 梯形加速
	double resultL;
	if (dt <= acctt1){             //在加速阶段
		resultL =acceleration* dt *0.5 *dt;
	}
	if ((dt > acctt1)&&(dt < acctt2)){         //在匀速阶段
		resultL = (dt-acctt1)*Lspeed +Lspeed*0.5*acctt1;
	}
	if (dt>= acctt2){                           //在减速阶段
		resultL = Lspeed*0.5*acctt1 + accT*Lspeed + ((Lspeed+(Lspeed - (dt - acctt2)*deceleration))/2)*(dt-acctt2);
	}
	return resultL;
}
if (status==2){
		double dl,Sac4,Svc4,Slc4,Sac5,Svc5,Slc5;
		if (dt <= Stt1){  
			dl = (1.0/6.0)*aGradient* pow(dt,3);
		}
		if ((dt > Stt1)&&(dt < Stt2)){   
			dl = -(1.0/6.0)*aGradient*pow(dt,3)+acceleration2*pow(dt,2)+Svc2*dt+Slc2;
		}
		if ((dt >= Stt2)&&(dt <= Stt3)){  
			dl = Stt2 * Lspeed *0.5 + (dt-Stt2)*Lspeed;
		}
		if ((dt > Stt3)&&(dt < Stt4)){
			Sac4 = dGradient *Stt3 ;
			Svc4 = Lspeed + 0.5*dGradient* pow(Stt3,2)-Sac4*Stt3;
			Slc4 =  Stt2 * Lspeed *0.5 + (Stt3-Stt2)*Lspeed  + (1.0/6.0)*dGradient* pow(Stt3,3) - 0.5*Sac4*pow(Stt3,2)-Svc4*Stt3 ;
			dl = -(1.0/6.0)*dGradient* pow(dt,3)+ 0.5*Sac4*pow(dt,2)+Svc4*dt +Slc4;
		}
		if ((dt >= Stt4)&&(dt <= Stt5)){
			 Sac5 = -Stt5 *dGradient;
			 Svc5 = 0-0.5*dGradient*pow(Stt5,2)-Sac5*Stt5;
			 Slc5 = Ll - (1.0/6.0)*dGradient* pow(Stt5,3)-0.5*Sac5*pow(Stt5,2)-Svc5*Stt5;
			 dl = (1.0/6.0)*dGradient*pow(dt,3)+0.5*Sac5*pow(dt,2)+Svc5*dt +Slc5 ;
		}

		return dl;
	}
}


//--------------------------------------------------
int WINAPI operateArc(double *initialP, double *midP, double *endP,double * TiSpe,double * sample,int acc)//当前点，中间点，目标点，速度
{
	double x0 =initialP[0];
	double y0 =initialP[1];
	double z0 =initialP[2];
	Ex0 =initialP[3];
	Ey0 =initialP[4];
	Ez0 =initialP[5];
	double x1 = midP[0];
	double y1 = midP[1];
	double z1 = midP[2];
	double x2 =endP[0];
	double y2 =endP[1];
	double z2 =endP[2];
	Ex1 =endP[3];
	Ey1 =endP[4];
	Ez1 =endP[5];
	double speed = TiSpe[0];
	double speed2 = TiSpe[1];
	speed2 = angleCal(speed2);
	Lspeed2  = speed2;
	double a1t = TiSpe[2];
	double a2t = TiSpe[3];
	double a,d,Ea,Ed;
	WPR0 = 0;
	

	if (acc ==1){
	if (a1t>-0.0000001 && a1t< 0.0000001){
		 acceleration = 0;
		 a=0;
		 Ea = 0;
	}
	else{
		acceleration = speed/a1t;
		a = speed/a1t;
		Ea  = speed2/a1t;
	}
	if (a2t>-0.0000001 && a2t< 0.0000001){
		 deceleration = 0;
		 d =0;
		 Ed=0;
	}
	else{
		deceleration = speed/a2t;
		d = speed/a2t;
		Ed = speed2/a2t;
	}
	}
	if (acc ==2){
	if (a1t>-0.0000001 && a1t< 0.0000001){
		 acceleration2= 0;
		 a = 0;
		 Ea = 0;
	}
	else{
		acceleration2 = 2.0*speed/a1t;
		a = 2.0*speed/a1t;
		Ea = 2.0*speed2/a1t;
	}
	if (a2t>-0.0000001 && a2t< 0.0000001){
		deceleration2= 0;
		d=0;
		Ed=0;
	}
	else{
		deceleration2= 2.0*speed/a2t;
		d = 2.0*speed/a2t;
		Ed=2.0*speed2/a2t;
	}
	}


	status = acc;
	Ax0= x0;
	Ay0= y0;
	Az0= z0;
	Ax1= x1;
	Ay1= y1;
	Az1= z1;
	Ax2= x2;
	Ay2= y2;
	Az2= z2;
	Aspeed = speed;
	Lspeed = speed;
	double p[3] = {x0, y0, z0};
    double q[3] = {x1, y1, z1};
    double r[3] = {x2 ,y2, z2};
	double * s = calCircle(p,q,r);
	Axc = s[0];
	Ayc = s[1];
	Azc = s[2];
	radius = s[3];
	double c[3] = {Axc ,Ayc, Azc};
	anglepq  = calAngle(p,q,c);
	anglerq  = calAngle(r,q,c);
	anglepr  = calAngle(p,r,c);
	angle = correctAngle(anglepq,anglerq,anglepr);
	perimeter = 2 * PI * radius;
	arcLength = radius * angle;
	Ll = arcLength;
	Lspeed2  = speed2;
	
	if (acc==0){                //   匀加速
		Lt = Ll / speed;
		double e3angle = getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,speed2);
		Elt = e3angle/speed2;
		if (Elt>-0.00000001 && Elt< 0.00000001){
			WPR0= 1;
		}
		if (Lt>=Elt){
			Elt = Lt;
			speed2 = e2angle/Elt;
			Lspeed2 = speed2;
		}
		else{
			Lt = Elt;
			speed = Ll/Lt;
			Lspeed = speed;
		}
		if (Lt>-0.00000001 && Lt< 0.00000001){
		sample[0]=Lx2;
		sample[1]=Ly2;
		sample[2]=Lz2;
		sample[3]=Ex1;
		sample[4]=Ey1;
		sample[5]=Ez1;
		flagOver = 1;
		return flagOver;
		}
		Lvx = (Lx2-Lx1)/ Lt ;
		Lvy = (Ly2-Ly1)/ Lt ;
		Lvz = (Lz2-Lz1)/ Lt ;
		sample[6]=Lt;
	}
	if(acc==1){                     //梯形加速
		acctt1 = a1t;
		acctt3 = a2t;
		Ecctt1 = a1t;
		Ecctt3 = a2t;
		accT = findStayTime(speed,a1t,a2t,1,Ll);
		double e3angle = getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,speed2);
		EccT = findStayTime(speed2,a1t,a2t,1,e3angle);
			if (accT >=0){                        // 完整梯形 点位
				acctt2 = accT + acctt1 ;
				Lt = accT + acctt1 + acctt3;
			}
			if (EccT >=0){                        // 完整梯形 姿态角
				Ecctt2 = EccT + Ecctt1 ;
				Elt = EccT + Ecctt1 + Ecctt3;
			}
			if (accT<0){                    // 不能达到最高速度
				if (a>-0.0000001 && a< 0.0000001){	
				acctt1 = 0;
				acctt2=0;
				accT = 0;
				acctt3 = sqrt(2*Ll/d);
				Lt = acctt3;
				Lspeed= acctt3*d;
				}
				else if (d>-0.0000001 && d< 0.0000001){	
				acctt1 = sqrt(2*Ll/a);
				acctt2=  sqrt(2*Ll/a);
				acctt3 = 0;
				accT = 0;
				Lt = acctt2;
				Lspeed= acctt1*a;
				}
				else{
				acctt1 = sqrt(Ll/(0.5*a+0.5*a*a/d));
				acctt2 = acctt1;
				acctt3 = acctt1*a/d;
				accT = 0;
				Lspeed= acctt1*a;
				Lt = acctt1+acctt3;
				}
			}
			
			if (EccT<0){                    // 不能达到最高速度
				if (Ea>-0.0000001 && Ea< 0.0000001){	
				Ecctt1 = 0;
				Ecctt2=0;
				EccT = 0;
				Ecctt3 = sqrt(2*e3angle/Ed);
				Elt = Ecctt3;
				Lspeed2= Ecctt3*Ed;
				}
				else if (Ed>-0.0000001 && Ed< 0.0000001){	
				Ecctt1 = sqrt(2*e3angle/Ea);
				Ecctt2=  sqrt(2*e3angle/Ea);
				Ecctt3 = 0;
				EccT = 0;
				Elt = Ecctt2;
				Lspeed2= Ecctt1*Ea;
				}
				else{
				Ecctt1 = sqrt(e3angle/(0.5*Ea+0.5*Ea*Ea/Ed));
				Ecctt2 = Ecctt1;
				Ecctt3 = Ecctt1*Ea/Ed;
				EccT = 0;
				Lspeed2= Ecctt1*Ea;
				Elt = Ecctt1+Ecctt3;
				}
			}
			if (Elt>-0.00000001 && Elt< 0.00000001){
			WPR0= 1;
			}
			if(Lt>=Elt){
					Ecctt1 = acctt1;
					Ecctt2 = acctt2;
					Ecctt3 = acctt3 ;
					EccT = accT;
					Elt = Lt;
					Lspeed2 = e3angle/((Ecctt1+Ecctt3)*0.5+EccT);
					speed2  = e3angle/((Ecctt1+Ecctt3)*0.5+EccT);
				}
			else{
					acctt1 = Ecctt1;
					acctt2 = Ecctt2;
					acctt3 = Ecctt3;
					accT = EccT ;
					Lt = Elt;
					Lspeed = Ll/((acctt1+acctt3)*0.5+accT);
					speed = Ll/((acctt1+acctt3)*0.5+accT);
					if (acctt1>-0.0000001 && acctt1< 0.0000001){
					 acceleration = 0;
					}
					else{
					acceleration = speed/acctt1;
					}
					if (acctt3>-0.0000001 && acctt3< 0.0000001){
						deceleration = 0;
					}
					else{
					deceleration = speed/acctt3;
					}
			}
		Aspeed = Lspeed;
	}
	
		if (acc==2){
		Stt = findStayTime(speed,a1t,a2t,2,Ll);
		double e3angle = getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,speed2);
		Ett = findStayTime(speed2,a1t,a2t,2,e3angle);

		if (Ett>=0){
		Ett1 = a1t*0.5;
		Ett2 = a1t;
		Ett3 = Ett2 + Ett;
		Ett4 = Ett3 + a2t*0.5;
		Ett5 = Ett3 + a2t;
		Elt = Ett5;
		}
		else{
			if (Ea>-0.0000001 && Ea< 0.0000001){
			Ett1 = 0;
			Ett2 = 0;
			Ett3 = 0;
			Ett4 = sqrt(4*e3angle/Ed)/2;
			Ett5 = sqrt(4*e3angle/Ed);
			Elt = Ett5;
			speed2 = Ett4 *Ed;
			Lspeed2 = speed2;
			}
			else if (Ed>-0.0000001 && Ed< 0.0000001){
			Ett1 = sqrt(4*e3angle/Ea)/2;
			Ett2 = 2*Ett1;
			Ett3 = Ett2;
			Ett4 = Ett2;
			Ett5= Ett2;
			Elt = Ett2;
			speed2 = Ett1 *Ea;
			Lspeed2 = speed2;
			}
			else{
			speed2 = sqrt(e3angle/((1/Ea)+(1/Ed)));
			Lspeed2 = sqrt(e3angle/((1/Ea)+( 1/Ed)));
			Ett1 = speed2/Ea;
			Ett2 = 2*Ett1;
			Ett3 = Ett2;
			Ett4 = Ett3 + speed2/Ed;
			Ett5 = Ett3 + 2*speed2/Ed;
			Elt = Ett5;
		}
		}
		if (Stt>=0){
		St1= a1t*0.5;
		aGradient= a/St1;
		dGradient = d/(a2t*0.5);
		Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
		Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
		Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
		Sdt = a1t;
		Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;

		Stt1 = a1t*0.5;
		Stt2 = a1t;
		Stt3 = Stt2 + Stt;
		Stt4 = Stt3 + a2t*0.5;
		Stt5 = Stt3 + a2t;
		Lt = Stt5;
		}
		else{
			if (a>-0.0000001 && a< 0.0000001){
			Stt1 = 0;
			Stt2 = 0;
			Stt3 = 0;
			Stt4 = sqrt(4*Ll/d)/2;
			Stt5 = sqrt(4*Ll/d);
			Lt = Stt5;
			speed = Stt4 *d;
			Lspeed = speed;
			dGradient = d/(speed/d);
			}
			else if (d>-0.0000001 && d< 0.0000001){
			Stt1 = sqrt(4*Ll/a)/2;
			Stt2 = 2*Stt1;
			Stt3 = Stt2;
			Stt4 = Stt2;
			Stt5= Stt2;
			Lt = Stt2;
			St1 = Stt1;
			speed = Stt1 *a;
			Lspeed = speed;
			aGradient= a/(speed/a);
			Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
			Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
			Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
			Sdt = 2*speed/a;
			Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;
			}
			else{
			speed = sqrt(Ll/((1/a)+(1/d)));
			Lspeed = sqrt(Ll/((1/a)+( 1/d)));
		
			St1= speed/a;
			aGradient= a/St1;
			dGradient = d/(speed/d);
			Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
			Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
			Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
			Sdt = 2*speed/a;
			Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;

			Stt1 = speed/a;
			Stt2 = 2*Stt1;
			accT = 0;
			Stt3 = Stt2;
			Stt4 = Stt3 + speed/d;
			Stt5 = Stt3 + 2*speed/d;
			Lt = Stt5;
			}
		}


		if (Elt>-0.00000001 && Elt< 0.00000001){
			WPR0= 1;
		}
			if(Lt>=Elt){
				Ett1 = Stt1;
				Ett2 = Stt2;
				Ett3 = Stt3;
				Ett4 = Stt4;
				Ett5= Stt5;
				Elt = Lt;
				speed2 = Ett1 *Ea;
				Lspeed2 = speed2;
				}
			else{
					Stt1 = Ett1;
					Stt2 = Ett2;
					Stt3 = Ett3;
					Stt4 = Ett4;
					Stt5 = Ett5;
					Lt = Elt;
					if(Ll>-0.00000001 && Ll< 0.00000001){
						Ll = e3angle;
					}
					speed = Ll/(0.5*Stt2+(Stt5-Stt3)*0.5+(Stt3-Stt2));
					Lspeed = speed;

					if (Stt1>-0.0000001 && Stt1< 0.0000001){
					 acceleration2 = 0;
					 a=0;
					}
					else{
					acceleration2 = speed/Stt1;
					a=speed/Stt1;
					}
					if ((Stt5-Stt3)>-0.0000001 && (Stt5-Stt3)< 0.0000001){
					deceleration2 = 0;
					d=0;
					}
					else{
					deceleration2 = speed/(Stt5-Stt3);
					d=speed/(Stt5-Stt3);
					}
					St1= Stt1;
					aGradient= a/St1;
					dGradient = d/((Stt4-Stt3)*0.5);
					Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
					Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
					Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
					Sdt = Stt2;
					Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;
			}
			Aspeed = Lspeed;
	}
	if (Lt>-0.0000001 && Lt< 0.0000001){
		flagOver = 1;
	}
	else{
		flagOver = 0;
	}
	toggle= 0 ;
	localTime2 = seccounter();
	sample[0] =	 Ax0;
	sample[1] =  Ay0;
	sample[2] =  Az0;
	sample[3] =  Ex0;
	sample[4] =  Ey0;
	sample[5] =  Ez0;
	return flagOver;
}
//--------------------------------------------------
int WINAPI getValueArc (double * sample){
		double Ex,Ey,Ez;
		localTime2 = seccounter();
		double dt = localTime2/1000.0;
		if (dt > Lt){
			flagOver = 1;
			sample[0]=Ax2;
			sample[1]=Ay2;
			sample[2]=Az2;
			sample[3]=Ex1;
			sample[4]=Ey1;
			sample[5]=Ez1;
			return flagOver ;
		}
		else{
		double Adl= getL(dt,status);

		getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,Lspeed2);                    //姿态角处理
		double loAngle = (Adl/Ll)*e2angle;
		double * Exyz = (double*)malloc(3*sizeof(double));
		operateE(loAngle,Exyz);
		Ex = Exyz[0];
		Ey = Exyz[1];
		Ez = Exyz[2];
		if (WPR0 == 1){
		Ex=Ex1;
		Ey=Ey1;
		Ez=Ez1;
		}
		double GangleArc = Adl/radius;
		double lx = Ax0;
		double ly = Ay0;
		double lz = Az0;
		double * newxyz = (double*)malloc(3*sizeof(double));
		while (GangleArc >0){
			if (GangleArc>1.4){
			newxyz = newArcXYZ2(Ax0,Ay0,Az0,Ax1,Ay1,Az1,Ax2,Ay2,Az2,lx,ly,lz,Axc,Ayc,Azc,radius,1.4);   
			}
			else{
			newxyz = newArcXYZ2(Ax0,Ay0,Az0,Ax1,Ay1,Az1,Ax2,Ay2,Az2,lx,ly,lz,Axc,Ayc,Azc,radius,GangleArc);   
			}
			lx = newxyz[0];
			ly = newxyz[1];
			lz = newxyz[2];
			GangleArc = GangleArc - 1.4;
		}
			sample[0]=lx;
			sample[1]=ly;
			sample[2]=lz;
			sample[3] =Ex;
			sample[4] =Ey;
			sample[5] =Ez;
		}
	return flagOver;
}
//--------------------------------------------------
int WINAPI VBdivideLine(double * sample){
	int countt1;

	double * initialP = (double*)malloc(6*sizeof(double));
	initialP[0]=455.6693;
	initialP[1]=-50.9828;
	initialP[2]=351.858;
	initialP[3]=-170;
	initialP[4]=20;
	initialP[5]=-175;
	double * endP = (double*)malloc(6*sizeof(double));
	endP[0]=400.6693;
	endP[1]=0;
	endP[2]=400;
	endP[3]=0;
	endP[4]=-150;
	endP[5]=0;
	double timesP=5;

	if (vbjj<1){
	for(int i1 = 0 ; i1 < 10000 ; i1++){
	XYZ[i1] = (double*)malloc(6*sizeof(double));
	}
	countt1 = divideLine(initialP,endP,XYZ,timesP);
	}

sample[0]=XYZ[vbjj][0];
sample[1]=XYZ[vbjj][1];
sample[2]=XYZ[vbjj][2];
sample[3]=XYZ[vbjj][3];
sample[4]=XYZ[vbjj][4];
sample[5]=XYZ[vbjj][5];
vbjj = vbjj +1;
return countt1;
}

int WINAPI divideLine(double * initialP, double * endP,double ** XYZ, double timesP){
double TiSpe[4]={20,5,0,0};
double * sample = (double*)malloc(6*sizeof(double));
operate(initialP,endP,TiSpe,sample,0);
XYZ[0][0]=sample[0];
XYZ[0][1]=sample[1];
XYZ[0][2]=sample[2];
XYZ[0][3]=sample[3];
XYZ[0][4]=sample[4];
XYZ[0][5]=sample[5];

double nbsDt = 0;
int totalpoints = int(Ll*timesP)+1;
double longadd = 1/timesP;
double nbsll = 0;
double Ex,Ey,Ez,dx,dy,dz;

for (int ii=1;ii<(totalpoints+1);ii++){

	nbsll = nbsll + longadd;
	nbsDt= nbsll/Lspeed;
	dx = ((Lx2-Lx1)/Ll)*nbsll+Lx1;
	dy = ((Ly2-Ly1)/Ll)*nbsll+Ly1;
	dz = ((Lz2-Lz1)/Ll)*nbsll+Lz1;
	double * Exyz = (double*)malloc(3*sizeof(double));
	double loAngle1 = e2angle*(nbsll/Ll); 
	operateE(loAngle1,Exyz);
	Ex = Exyz[0];
	Ey = Exyz[1];
	Ez = Exyz[2];
	if (WPR0 == 1){
		Ex=Ex1;
		Ey=Ey1;
		Ez=Ez1;
	}

if (nbsll >= Ll){
	flagOver = 1;
	XYZ[ii][0]=Lx2;
	XYZ[ii][1]=Ly2;
	XYZ[ii][2]=Lz2;
	XYZ[ii][3]=Ex1;
	XYZ[ii][4]=Ey1;
	XYZ[ii][5]=Ez1;
	
}
else{
	XYZ[ii][0]=dx;
	XYZ[ii][1]=dy;
	XYZ[ii][2]=dz;
	XYZ[ii][3]=Ex;
	XYZ[ii][4]=Ey;
	XYZ[ii][5]=Ez;

}

}
return totalpoints;
}
int WINAPI operate(double * initialP, double * endP, double * TiSpe, double * sample,int acc)
{
	double x1 =initialP[0];
	double y1 =initialP[1];
	double z1 =initialP[2];
	Ex0 =initialP[3];
	Ey0 =initialP[4];
	Ez0 =initialP[5];
	double x2 =endP[0];
	double y2 =endP[1];
	double z2 =endP[2];
	Ex1 =endP[3];
	Ey1 =endP[4];
	Ez1 =endP[5];
	double speed = TiSpe[0];
	double speed2 = TiSpe[1];
	speed2 = angleCal(speed2);
	Lspeed2  = speed2;
	double a1t = TiSpe[2];
	double a2t = TiSpe[3];
	double a,d,Ea,Ed;
	WPR0 = 0;

	if (acc ==1){
	if (a1t>-0.0000001 && a1t< 0.0000001){
		 acceleration = 0;
		 a=0;
		 Ea = 0;
	}
	else{
		acceleration = speed/a1t;
		a = speed/a1t;
		Ea  = speed2/a1t;
	}
	if (a2t>-0.0000001 && a2t< 0.0000001){
		 deceleration = 0;
		 d =0;
		 Ed =0;
	}
	else{
		deceleration = speed/a2t;
		d = speed/a2t;
		Ed = speed2/a2t;
	}
	}
	if (acc ==2){
	if (a1t>-0.0000001 && a1t< 0.0000001){
		 acceleration2 = 0;
		 a = 0;
		 Ea =0;
	}
	else{
		acceleration2 = 2.0*speed/a1t;
		a = 2.0*speed/a1t;
		Ea = 2.0*speed2/a1t;
	}
	if (a2t>-0.0000001 && a2t< 0.0000001){
		deceleration2 = 0;
		d=0;
		Ed=0;
	}
	else{
		deceleration2 = 2.0*speed/a2t;
		d = 2.0*speed/a2t;
		Ed=2.0*speed2/a2t;
	}
	}
	Lx1 = x1 ;
	Ly1 = y1 ;
	Lz1 = z1 ;
	Lx2 = x2 ;
	Ly2 = y2 ;
	Lz2 = z2 ;
	l_square = ((Lx2-Lx1)*(Lx2-Lx1)+(Ly2-Ly1)*(Ly2-Ly1)+(Lz2-Lz1)*(Lz2-Lz1));                                                                                                                                                                                                                                       
	Ll = sqrt((double)l_square);
	status = acc;
	Lspeed = speed;

	if (acc==0){                //   匀加速
		Lt = Ll / speed;
		double e3angle = getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,speed2);
		Elt = e3angle/speed2;
		if (Elt>-0.00000001 && Elt< 0.00000001){
			WPR0= 1;
		}
		if (Lt>=Elt){
			Elt = Lt;
			speed2 = e2angle/Elt;
			Lspeed2 = speed2;
		}
		else{
			Lt = Elt;
			speed = Ll/Lt;
			Lspeed = speed;
		}
		if (Lt>-0.00000001 && Lt< 0.00000001){
		sample[0]=Lx2;
		sample[1]=Ly2;
		sample[2]=Lz2;
		sample[3]=Ex1;
		sample[4]=Ey1;
		sample[5]=Ez1;
		flagOver = 1;
		return flagOver;
		}
		Lvx = (Lx2-Lx1)/ Lt ;
		Lvy = (Ly2-Ly1)/ Lt ;
		Lvz = (Lz2-Lz1)/ Lt ;
		sample[6]=Lt;
	}
	if(acc==1){                     //梯形加速
		acctt1 = a1t;
		acctt3 = a2t;
		Ecctt1 = a1t;
		Ecctt3 = a2t;
		accT = findStayTime(speed,a1t,a2t,1,Ll);
		double e3angle = getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,speed2);
		EccT = findStayTime(speed2,a1t,a2t,1,e3angle);
			if (accT >=0){                        // 完整梯形 点位
				acctt2 = accT + acctt1 ;
				Lt = accT + acctt1 + acctt3;
			}
			if (EccT >=0){                        // 完整梯形 姿态角
				Ecctt2 = EccT + Ecctt1 ;
				Elt = EccT + Ecctt1 + Ecctt3;
			}
			if (accT<0){                    // 不能达到最高速度
				if (a>-0.0000001 && a< 0.0000001){	
				acctt1 = 0;
				acctt2=0;
				accT = 0;
				acctt3 = sqrt(2*Ll/d);
				Lt = acctt3;
				Lspeed= acctt3*d;
				}
				else if (d>-0.0000001 && d< 0.0000001){	
				acctt1 = sqrt(2*Ll/a);
				acctt2=  sqrt(2*Ll/a);
				acctt3 = 0;
				accT = 0;
				Lt = acctt2;
				Lspeed= acctt1*a;
				}
				else{
				acctt1 = sqrt(Ll/(0.5*a+0.5*a*a/d));
				acctt2 = acctt1;
				acctt3 = acctt1*a/d;
				accT = 0;
				Lspeed= acctt1*a;
				Lt = acctt1+acctt3;
				}
			}
			
			if (EccT<0){                    // 不能达到最高速度
				if (Ea>-0.0000001 && Ea< 0.0000001){	
				Ecctt1 = 0;
				Ecctt2=0;
				EccT = 0;
				Ecctt3 = sqrt(2*e3angle/Ed);
				Elt = Ecctt3;
				Lspeed2= Ecctt3*Ed;
				}
				else if (Ed>-0.0000001 && Ed< 0.0000001){	
				Ecctt1 = sqrt(2*e3angle/Ea);
				Ecctt2=  sqrt(2*e3angle/Ea);
				Ecctt3 = 0;
				EccT = 0;
				Elt = Ecctt2;
				Lspeed2= Ecctt1*Ea;
				}
				else{
				Ecctt1 = sqrt(e3angle/(0.5*Ea+0.5*Ea*Ea/Ed));
				Ecctt2 = Ecctt1;
				Ecctt3 = Ecctt1*Ea/Ed;
				EccT = 0;
				Lspeed2= Ecctt1*Ea;
				Elt = Ecctt1+Ecctt3;
				}
			}
			if (Elt>-0.00000001 && Elt< 0.00000001){
			WPR0= 1;
			}
			if(Lt>=Elt){
					Ecctt1 = acctt1;
					Ecctt2 = acctt2;
					Ecctt3 = acctt3 ;
					EccT = accT;
					Elt = Lt;
					Lspeed2 = e3angle/((Ecctt1+Ecctt3)*0.5+EccT);
					speed2  = e3angle/((Ecctt1+Ecctt3)*0.5+EccT);
				}
			else{
					acctt1 = Ecctt1;
					acctt2 = Ecctt2;
					acctt3 = Ecctt3;
					accT = EccT ;
					Lt = Elt;
					if(Ll>-0.00000001 && Ll< 0.00000001){
						Ll = e3angle;
					}
					Lspeed = Ll/((acctt1+acctt3)*0.5+accT);
					speed = Ll/((acctt1+acctt3)*0.5+accT);
					if (acctt1>-0.0000001 && acctt1< 0.0000001){
					 acceleration = 0;
					}
					else{
					acceleration = speed/acctt1;
					}
					if (acctt3>-0.0000001 && acctt3< 0.0000001){
					deceleration = 0;
					}
					else{
					deceleration = speed/acctt3;
					}
			}
	}
	if (acc==2){
		Stt = findStayTime(speed,a1t,a2t,2,Ll);
		double e3angle = getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,speed2);
		Ett = findStayTime(speed2,a1t,a2t,2,e3angle);

		if (Ett>=0){
		Ett1 = a1t*0.5;
		Ett2 = a1t;
		Ett3 = Ett2 + Ett;
		Ett4 = Ett3 + a2t*0.5;
		Ett5 = Ett3 + a2t;
		Elt = Ett5;
		}
		else{
			if (Ea>-0.0000001 && Ea< 0.0000001){
			Ett1 = 0;
			Ett2 = 0;
			Ett3 = 0;
			Ett4 = sqrt(4*e3angle/Ed)/2;
			Ett5 = sqrt(4*e3angle/Ed);
			Elt = Ett5;
			speed2 = Ett4 *Ed;
			Lspeed2 = speed2;
			}
			else if (Ed>-0.0000001 && Ed< 0.0000001){
			Ett1 = sqrt(4*e3angle/Ea)/2;
			Ett2 = 2*Ett1;
			Ett3 = Ett2;
			Ett4 = Ett2;
			Ett5= Ett2;
			Elt = Ett2;
			speed2 = Ett1 *Ea;
			Lspeed2 = speed2;
			}
			else{
			speed2 = sqrt(e3angle/((1/Ea)+(1/Ed)));
			Lspeed2 = sqrt(e3angle/((1/Ea)+( 1/Ed)));
			Ett1 = speed2/Ea;
			Ett2 = 2*Ett1;
			Ett3 = Ett2;
			Ett4 = Ett3 + speed2/Ed;
			Ett5 = Ett3 + 2*speed2/Ed;
			Elt = Ett5;
		}
		}
		if (Stt>=0){
		St1= a1t*0.5;
		aGradient= a/St1;
		dGradient = d/(a2t*0.5);
		Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
		Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
		Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
		Sdt = a1t;
		Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;

		Stt1 = a1t*0.5;
		Stt2 = a1t;
		Stt3 = Stt2 + Stt;
		Stt4 = Stt3 + a2t*0.5;
		Stt5 = Stt3 + a2t;
		Lt = Stt5;
		}
		else{
			if (a>-0.0000001 && a< 0.0000001){
			Stt1 = 0;
			Stt2 = 0;
			Stt3 = 0;
			Stt4 = sqrt(4*Ll/d)/2;
			Stt5 = sqrt(4*Ll/d);
			Lt = Stt5;
			speed = Stt4 *d;
			Lspeed = speed;
			dGradient = d/(speed/d);
			}
			else if (d>-0.0000001 && d< 0.0000001){
			Stt1 = sqrt(4*Ll/a)/2;
			Stt2 = 2*Stt1;
			Stt3 = Stt2;
			Stt4 = Stt2;
			Stt5= Stt2;
			Lt = Stt2;
			St1 = Stt1;
			speed = Stt1 *a;
			Lspeed = speed;
			aGradient= a/(speed/a);
			Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
			Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
			Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
			Sdt = 2*speed/a;
			Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;
			}
			else{
			speed = sqrt(Ll/((1/a)+(1/d)));
			Lspeed = sqrt(Ll/((1/a)+( 1/d)));
		
			St1= speed/a;
			aGradient= a/St1;
			dGradient = d/(speed/d);
			Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
			Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
			Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
			Sdt = 2*speed/a;
			Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;

			Stt1 = speed/a;
			Stt2 = 2*Stt1;
			accT = 0;
			Stt3 = Stt2;
			Stt4 = Stt3 + speed/d;
			Stt5 = Stt3 + 2*speed/d;
			Lt = Stt5;
			}
		}


		if (Elt>-0.00000001 && Elt< 0.00000001){
			WPR0= 1;
		}
			if(Lt>=Elt){
				Ett1 = Stt1;
				Ett2 = Stt2;
				Ett3 = Stt3;
				Ett4 = Stt4;
				Ett5= Stt5;
				Elt = Lt;
				speed2 = Ett1 *Ea;
				Lspeed2 = speed2;
				}
			else{
					Stt1 = Ett1;
					Stt2 = Ett2;
					Stt3 = Ett3;
					Stt4 = Ett4;
					Stt5 = Ett5;
					Lt = Elt;
					if(Ll>-0.00000001 && Ll< 0.00000001){
						Ll = e3angle;
					}
					speed = Ll/(0.5*Stt2+(Stt5-Stt3)*0.5+(Stt3-Stt2));
					Lspeed = speed;

					if (Stt1>-0.0000001 && Stt1< 0.0000001){
					 acceleration2 = 0;
					 a=0;
					}
					else{
					acceleration2 = speed/Stt1;
					a=speed/Stt1;
					}
					if ((Stt5-Stt3)>-0.0000001 && (Stt5-Stt3)< 0.0000001){
					deceleration2 = 0;
					d=0;
					}
					else{
					deceleration2 = speed/(Stt5-Stt3);
					d=speed/(Stt5-Stt3);
					}
					St1= Stt1;
					aGradient= a/St1;
					dGradient = d/((Stt4-Stt3)*0.5);
					Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
					Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
					Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
					Sdt = Stt2;
					Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;
			}
	}
	if (Lt>-0.0000001 && Lt< 0.0000001){
		flagOver = 1;
	}
	else{
		flagOver = 0;
	}
	toggle= 0 ;
	localTime1 = seccounter();       //计时开始
	sample[0]=x1;
	sample[1]=y1;
	sample[2]=z1;
	sample[3]=Ex0;
	sample[4]=Ey0;
	sample[5]=Ez0;
	return flagOver;
	
}

//---------------------------------------------------------------

int WINAPI getValueLine (double * sample){
	localTime1 = seccounter();
	double dt = localTime1/1000.0;
	double ddx,ddy,ddz,Ex,Ey,Ez;

	if (status== 0){            //匀加速
	double dx = dt*Lvx+Lx1;
	double dy = dt*Lvy+Ly1;
	double dz = dt*Lvz+Lz1;
	double * Exyz = (double*)malloc(3*sizeof(double));
	double loAngle1 = dt *Lspeed2; 
	operateE(loAngle1,Exyz);
	Ex = Exyz[0];
	Ey = Exyz[1];
	Ez = Exyz[2];
	if (WPR0 == 1){
		Ex=Ex1;
		Ey=Ey1;
		Ez=Ez1;
	}
	if (dt>Lt){   
		flagOver = 1;//到达目标点
		sample[0]=Lx2;
		sample[1]=Ly2;
		sample[2]=Lz2;
		sample[3]=Ex1;
		sample[4]=Ey1;
		sample[5]=Ez1;
	return flagOver;
	}
	else{
		sample[0]=dx;
		sample[1]=dy;
		sample[2]=dz;
		sample[3]=Ex;
		sample[4]=Ey;
		sample[5]=Ez;
	return flagOver;
	}
	}

	if (status== 1){         // 梯形加速
		if (dt>= Lt){           //到达目标点
			flagOver = 1;
			sample[0]=Lx2;
			sample[1]=Ly2;
			sample[2]=Lz2;
			sample[3]=Ex1;
			sample[4]=Ey1;
			sample[5]=Ez1;
			return flagOver;
		}
		double loL = getL(dt,1);
		getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,Lspeed2);
		double loAngle = (loL/Ll)*e2angle;
		double * Exyz = (double*)malloc(3*sizeof(double));
		operateE(loAngle,Exyz);
		Ex = Exyz[0];
		Ey = Exyz[1];
		Ez = Exyz[2];
	if (WPR0 == 1){
		Ex=Ex1;
		Ey=Ey1;
		Ez=Ez1;
	}
		ddx=((Lx2-Lx1)/Ll)*loL +Lx1;
		ddy=((Ly2-Ly1)/Ll)*loL +Ly1;
		ddz=((Lz2-Lz1)/Ll)*loL +Lz1;
		sample[0]=ddx;
		sample[1]=ddy;
		sample[2]=ddz;
		sample[3]=Ex;
		sample[4]=Ey;
		sample[5]=Ez;
		return flagOver;
	}


	if (status==2){
		if (dt>= Lt){           //到达目标点
			flagOver = 1;
			sample[0]=Lx2;
			sample[1]=Ly2;
			sample[2]=Lz2;
			sample[3]=Ex1;
			sample[4]=Ey1;
			sample[5]=Ez1;
			return flagOver;
		}

		double Ldl = getL (dt,2);
		getEulerAg(Ex0,Ey0,Ez0,Ex1,Ey1,Ez1,Lspeed2);
		double loAngle = (Ldl/Ll)*e2angle;
		double * Exyz = (double*)malloc(3*sizeof(double));
		operateE(loAngle,Exyz);
		Ex = Exyz[0];
		Ey = Exyz[1];
		Ez = Exyz[2];
		if (WPR0 == 1){
		Ex=Ex1;
		Ey=Ey1;
		Ez=Ez1;
		}
		ddx = Lx1+Ldl * ((Lx2-Lx1)/Ll);
		ddy = Ly1+Ldl * ((Ly2-Ly1)/Ll);
		ddz = Lz1+Ldl * ((Lz2-Lz1)/Ll);

		sample[0]=ddx;
		sample[1]=ddy;
		sample[2]=ddz;
		sample[3]=Ex;
		sample[4]=Ey;
		sample[5]=Ez;
		return flagOver;
	}

}

double findStayTime(double speed,double a1t,double a2t,int acc,double Ll_local)  //找到梯形或S型加速中，匀速阶段持续时间
{

	if (acc ==1){
	double distance1 = a1t * speed * 0.5;
	double distance2 = a2t * speed *0.5;
	double fd1= Ll_local - distance1 - distance2;
	if (fd1 < 0){              //路程不够加速和减速的路程
	return -500;
	}
	else{
	return fd1/speed;          //路程包括了完整的梯形加速
	}
	} 

	if (acc==2){
	double distance1 = speed*a1t*0.5;
	double distance2 = speed*a2t*0.5;
	double fd1= Ll_local - distance1 - distance2 ;

	if (fd1 < 0){              //路程只够加速和部分减速阶段
		return -500;
	}
	else{
		return fd1/speed ;          //路程包括了完整的S型加速
	}
	}
}


int WINAPI operateC(double x1,double y1,double z1,double x2,double y2,double z2,double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed,double speed2 , double * sample,int acc,double a1t,double a2t)
{
	double * arr1 = (double*)malloc(6*sizeof(double));
	arr1[0] = x1;
	arr1[1] = y1;
	arr1[2] = z1;
	arr1[3] = Ex0;
	arr1[4] = Ey0;
	arr1[5] = Ez0;
	double * arr2 = (double*)malloc(6*sizeof(double));
	arr2[0] = x2;
	arr2[1] = y2;
	arr2[2] = z2;
	arr2[3] = Ex1;
	arr2[4] = Ey1;
	arr2[5] = Ez1;
	double* arr3 = (double*)malloc(4*sizeof(double));
	arr3[0] = speed;
	arr3[1] = speed2;
	arr3[2] = a1t;
	arr3[3] = a2t;
	int l = operate(arr1,arr2,arr3,sample,acc);
	return l;
}
int WINAPI operateArcC(double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed,double speed2,double * sample,int acc,double a1t,double a2t)//当前点，中间点，目标点，速度
{
	double * arr1 = (double*)malloc(6*sizeof(double));
	arr1[0] = x0;
	arr1[1] = y0;
	arr1[2] = z0;
	arr1[3] = Ex0;
	arr1[4] = Ey0;
	arr1[5] = Ez0;	

	double * arr2 = (double*)malloc(6*sizeof(double));
	arr2[0] = x2;
	arr2[1] = y2;
	arr2[2] = z2;
	arr2[3] = Ex1;
	arr2[4] = Ey1;
	arr2[5] = Ez1;
	
	double* arr3 = (double*)malloc(4*sizeof(double));
	arr3[0] = speed;
	arr3[1] = speed2;
	arr3[2] = a1t;
	arr3[3] = a2t;

	double*arr4 = (double*)malloc(3*sizeof(double));
	arr4[0] = x1;
	arr4[1] = y1;
	arr4[2] = z1;
	int l = operateArc(arr1,arr4,arr2,arr3,sample,acc);
	return l;
}

double getEulerAg(double Ex0,double Ey0,double Ez0,double Ex1,double Ey1,double Ez1,double speed2){
double eWRP1[3], eQuaternion1[4],eWRP2[3], eQuaternion2[4];
eWRP1[0]=Ex0;
eWRP1[1]=Ey0;
eWRP1[2]=Ez0;
eWRP2[0]=Ex1;
eWRP2[1]=Ey1;
eWRP2[2]=Ez1;
angleToQuaternion(eWRP1,eQuaternion1);
angleToQuaternion(eWRP2,eQuaternion2);
double deWRP1[3],ReMatrix[3][3],dRmatrix[3][3];
quaternionToRMatrix(eQuaternion1,RsMatrix);
quaternionToRMatrix(eQuaternion2,ReMatrix);
brinv(ReMatrix[0],3);
matrixMultiply(ReMatrix,RsMatrix,dRmatrix);
double cosangle= 0.5*(dRmatrix[0][0]+dRmatrix[1][1]+dRmatrix[2][2]-1);
e2angle = acos(cosangle);
double sinangle = sin(e2angle);
fx1 = (dRmatrix[2][1]-dRmatrix[1][2])/(2.0*sinangle);
fy1 = (dRmatrix[0][2]-dRmatrix[2][0])/(2.0*sinangle);
fz1 = (dRmatrix[1][0]-dRmatrix[0][1])/(2.0*sinangle);
return e2angle;
}


void operateE(double ddangle,double * sample)
{
double dangle;
dangle = ddangle;
double newq0 = cos(dangle/2);
double newq1 = fx1*sin(dangle/2);
double newq2 = fy1*sin(dangle/2);
double newq3 = fz1*sin(dangle/2);
double dQuaternion[4] ={newq0,newq3,newq2,newq1};
double dnRMatrix[3][3],cerRMatrix[3][3];
quaternionToRMatrix(dQuaternion,dnRMatrix);
brinv(dnRMatrix[0],3);
matrixMultiply(RsMatrix,dnRMatrix,cerRMatrix);
double dWPR[3];
rmatrixToWPR(cerRMatrix,dWPR);
sample[0]=dWPR[0];
sample[1]=dWPR[1];
sample[2]=dWPR[2];

//double eq10 = eQuaternion1[0]; //a=0
//double eq11 = eQuaternion1[1]; //b	
//double eq12 = eQuaternion1[2]; //c
//double eq13 = eQuaternion1[3]; //d
//double eq20 = eQuaternion2[0];    
//double eq21 = eQuaternion2[1];	 
//double eq22 = eQuaternion2[2];	
//double eq23 = eQuaternion2[3];	  
//double uniteq1 = sqrt(pow(eq11,2)+pow(eq12,2)+pow(eq13,2));
//double uniteq2 = sqrt(pow(eq21,2)+pow(eq22,2)+pow(eq23,2));
//double q11 = eq11/uniteq1;
//double q12 = eq12/uniteq1;
//double q13 = eq13/uniteq1;
//double q21 = eq21/uniteq2;
//double q22 = eq22/uniteq2;
//double q23 = eq23/uniteq2;


//double s1ABC[4][4] = { {0, -q11, q13, -q12}, {q11, 0, -q12,-q13}, {-q13, q12, 0,-q11},{q12, q13, q11,0} };
//double s2ABC[4][4] = { {0, -eq21, -eq22, -eq23}, {eq21, 0, -eq23,eq22}, {eq22, eq23, 0,-eq21},{eq23, -eq22, eq21,0} };
//double qABC[4][4];
//brinv(s1ABC[0],4);
//double A[4][4]= { {0, -q21, -q22, -q23}, {q21, 0, -q23,q22}, {q22, q23, 0,-q21},{q23, -q22, q21,0} };
//double B[4] = {0,-q11,-q12,-q13};
//double C[4];

//double A[4][4]= { {0, -eq21, -eq22, -eq23}, {eq21, 0, -eq23,eq22}, {eq22, eq23, 0,-eq21},{eq23, -eq22, eq21,0} };
//double B[4] = {0,-eq11,-eq12,-eq13};
//double C[4];
//mult_matrices2(A,B,C); 
//matrixMultiply3(s2ABC,s1ABC,qABC);
//double Nq0 = C[0];
//double Nq1 = C[1];
//double Nq2 = C[2];
//double Nq3 = C[3];
//double eangle = acos(Nq0);
//double Eq1 = Nq1/sin(eangle);
//double Eq2 = Nq2/sin(eangle);
//double Eq3 = Nq3/sin(eangle);


//WPRZXYTormatrix(eWRP1,RsMatrix);
//WPRZXYTormatrix(eWRP2,ReMatrix);

//rmatrixToWPR(dRmatrix, dWPR);
//angleToQuaternion(dWPR,dQuaternion);


//double dq0 = cos(dangle);
//double dq1 = Eq1*sin(dangle);
//double dq2 = Eq2*sin(dangle);
//double dq3 = Eq3*sin(dangle);
//double dq[3]={dq0,dq1,dq2};
//double s1[3][3]= {{q11,q12,q13}, {0,-q13,q12}, {q13,0,-q11}};
//brinv(s1[0],3);
//double resultss[3];
//mult_matrices(s1,dq,resultss);

//resultq0 = sqrt(1-pow(resultss[0],2)-pow(resultss[1],2)-pow(resultss[2],2));
//double newQuaternion[4] = {newq0,newq1,newq2,newq3},newWPR[3];
//quaternionToAngle(newQuaternion, newWPR);


}

/*
void BasisFuns(int i,double u,int p,double U[],double result[]){
result[0]=1.0;
int j,r;
double saved,left[100],right[100],temp;

for (j=1;j<=p;j++){
	left[j]= u - U[i+1-j];
	right[j]= U[i+j]-u;
	saved = 0.0;
	for (r=0;r<j;r++){
		temp=result[r]/(right[r+1]+left[j-r]);
		result[r] =saved+right[r+1]*temp;
		saved = left[j-r] *temp;
		}
	result[j] = saved;
	}
}
*/

double aCal(int l,int j,int k,double u,double knots[]){
double alj ;
if ((knots[j+k+1-l]-knots[j])>-0.00000001 && (knots[j+k+1-l]-knots[j])< 0.00000001){
alj = 0.0;
}
else{
alj=  (u-knots[j])/(knots[j+k+1-l]-knots[j]);
}
return alj;
}


double wCal(int l,int j,int k,double u,double weights[],double knots[]){
if (l==0){
	return weights[j];		
	}
else{
	double wlj = (1-aCal(l,j,k,u,knots))*wCal(l-1,j-1,k,u,weights,knots)+aCal(l,j,k,u,knots)*wCal(l-1,j,k,u,weights,knots);
	return wlj;
	}
}	

double dCal(int l, int j,int k,double u,double weights[],double knots[],double dPoles[]){
	if (l==0){
	return dPoles[j];
	}
	else{
	double dlj = ((1-aCal(l,j,k,u,knots))*wCal(l-1,j-1,k,u,weights,knots)*dCal(l-1,j-1,k,u,weights,knots,dPoles)+aCal(l,j,k,u,knots)*wCal(l-1,j,k,u,weights,knots)*dCal(l-1,j,k,u,weights,knots,dPoles))/wCal(l,j,k,u,weights,knots);
	return dlj;
	}
}
double nCal(int i,int k,double u,double knots[]){
	if (k==0){
		if ((knots[i+1]>=u)&&(u>=knots[i])){
			return 1.0;
		}
		else{
			return 0.0;
		}
	}
	else{
		double niku;
		double uc1,uc2;
		if(((knots[i+k]-knots[i])>-0.00000001) && ((knots[i+k]-knots[i])< 0.00000001)){
			uc1 = 0;
		}
		else{
			uc1 = (u-knots[i])/(knots[i+k]-knots[i]);
		}
		if (((knots[i+k+1]-knots[i+1])>-0.00000001) && ((knots[i+k+1]-knots[i+1])< 0.00000001)){
			uc2 = 0;
		}
		else{
			uc2 = ((knots[i+k+1]-u)/(knots[i+k+1]-knots[i+1]));
		}
		niku = uc1*nCal(i,k-1,u,knots)+uc2*nCal(i+1,k-1,u,knots);
		return niku;
	}
}
double ppuCal(int n,int k, double u,double weights[],double knots[],double dPoles[]){
int i;
double pu1= 0.0;
double pu2= 0.0;
double nResult;
for (i=0 ;i<(n+1) ;i++){
nResult= nCal(i,k,u,knots);
pu1 = weights[i]*dPoles[i]*nResult +pu1;
pu2 = weights[i]*nResult +pu2;
}
return  pu1/pu2;
}
double detuCal(int n,int k, double u,double du,double weights[],double knots[],double xPoles[],double yPoles[],double zPoles[]){
double tryx1 = ppuCal(n,k,u,weights,knots,xPoles);
double tryy1 = ppuCal(n,k,u,weights,knots,yPoles);
double tryz1 = ppuCal(n,k,u,weights,knots,zPoles);
double tryx2 = ppuCal(n,k,u+du,weights,knots,xPoles);
double tryy2 = ppuCal(n,k,u+du,weights,knots,yPoles);
double tryz2 = ppuCal(n,k,u+du,weights,knots,zPoles);
double li = sqrt(pow((tryx2-tryx1),2)+pow((tryy2-tryy1),2)+pow((tryz2-tryz1),2));
return li;
}
double nrbsL(int timesu,double minu,double maxu,double lastu,double tryu,double dets,int nn,int kk,double weights[],double knots[],double xPoles[],double yPoles[],double zPoles[],double * storemi){
double li;
double mi = 0;
double accm= 0;
storemi[0] = 0;
for (int i=1; i<timesu;i++){
li = detuCal(nn,kk,minu,tryu,weights,knots,xPoles,yPoles,zPoles);
minu = minu + tryu;
mi = li/dets;
storemi[i]=storemi[i-1]+mi;
accm = accm + mi;
}
li = detuCal(nn,kk,maxu-lastu-tryu,tryu,weights,knots,xPoles,yPoles,zPoles);
minu = minu + tryu;
mi = li/dets;
storemi[timesu]=storemi[timesu-1]+mi;
accm = accm + mi;

li = detuCal(nn,kk,maxu-lastu,lastu,weights,knots,xPoles,yPoles,zPoles);
mi = li/dets;
minu = minu + lastu;
storemi[timesu+1]=storemi[timesu]+mi;
accm = accm + mi;
double totl = accm*dets;
return totl;
}
/* 
int WINAPI puCal(double *sample){
普通曲线
double weights[5] = {1,1,1,1,1};
double knots[10] ={0,0,0,0,0,1,1,1,1,1};
double dPoles[5][3]={{0,0,1},{1.39641,0.763197,1.17608},{0.0754466,6.93397,4.06177},{6.02681,-0.194719,3.76274},{5,5,5}};
double xPoles[5]={0,1.39641,0.0754466,6.02681,5};
double yPoles[5]={0,0.763197,6.93397,-0.194719,5};
double zPoles[5]={1,1.17608,4.06177,3.76274,5};

类似大半圆曲线
double weights[4] = {1,1,1,1};
double knots[8] ={0,0,0,0,1,1,1,1};
double dPoles[4][3]={{540,84,54},{188.063,1127.64,-362.595},{-234.492,165.088,251.964},{84,0,54}};
double xPoles[4]={540,188.063,-234.492,84};
double yPoles[4]={84,1127.64,165.088,0};
double zPoles[4]={54,-362.595,251.964,54};

double weights[5] = {1,0.707107,1,0.707107,1};
double knots[8] ={3.14159,3.14159,3.14159,4.71239,4.71239,6.28319,6.28319,6.28319};
double dPoles[5][3]={{-1,-1.22465e-016,0},{-1,1,0},{-1.83697e-016,1,0},{1,1,0},{1,2.44929e-016,0}};
//double xPoles[5]={-1,-1,-1.83697e-016,1,1};
//double yPoles[5]={-1.22465e-016,1,1,1,2.44929e-016};
//double zPoles[5]={0,0,0,0,0};
double xPoles[5]={-1000,-1000,-1.83697e-013,1000,1000};
double yPoles[5]={-1.22465e-013,1000,1000,1000,2.44929e-013};
double zPoles[5]={0,0,0,0,0};
/* 整个圆
double weights[6] = {1,0.5,1,0.5,1,0.5};
//double weights[6] = {1/4.5,0.5/4.5,1/4.5,0.5/4.5,1/4.5,0.5/4.5};
double knots[10] ={0,0,0,2.0944,2.0944,4.18879,4.18879,6.28319,6.28319,6.28319};
double dPoles[6][3]= {{0,0,1},{0,-1.73205,1},{0,-0.866025,-0.5},{0,-2.44929e-016,-2},{0,0.866025,-0.5},{0,1.73205,1}};
double xPoles[6]={0,0,0,0,0,0};
double yPoles[6]={0,-1.73205,-0.866025,-2.44929e-016,0.866025,1.73205};
double zPoles[6]={1,1,-0.5,-2,-0.5,1};


int kk=2;
int nn=4;
double dets = 0.0001;
double tryu = 0.0005;// !!!!!!!!!!!!!!!!!!减小此值能 精确总距离
double accm=0;
double minu = 3.14159;
double maxu = 6.28319;
int timesu = int((maxu-minu)/tryu);
double lastu= (maxu-minu) - tryu*timesu;
double * storemi = (double*)malloc(timesu*sizeof(double));

double totl =nrbsL(timesu,minu,maxu,lastu,tryu,dets,nn,kk,weights,knots,xPoles,yPoles,zPoles,storemi);

double speed = 10;
double nrbsT = totl/speed;
double nbsDt = glou;
double nbsDs = nbsDt*speed;
double nbsDm = nbsDs/dets;
int nbsDi;
for (int i=0; i<timesu;i++){
	if ((storemi[i]<=nbsDm)&&(nbsDm<=storemi[i+1])){
		nbsDi = i;
		break;
	}
}


double dminu = minu +tryu*nbsDi;
double dmaxu = minu +tryu*(nbsDi+1);
double dtryu = 0.0000001; //!!!!!!!!!!!!!!!!!!!!!!减小此值 直接提升点的准确度 
int dtimesu = int((dmaxu-dminu)/dtryu);
double dlastu= (dmaxu-dminu) - dtryu*dtimesu;
double * dstoremi = (double*)malloc(dtimesu*sizeof(double));
double dtotl =nrbsL(dtimesu,dminu,dmaxu,dlastu,dtryu,dets,nn,kk,weights,knots,xPoles,yPoles,zPoles,dstoremi);

double dnewDm = nbsDm - storemi[nbsDi];
int NnbsDi;
for (int j=0; j<dtimesu;j++){
	if ((dstoremi[j]<=dnewDm)&&(dnewDm<dstoremi[j+1])){
		NnbsDi = j;
		break;
	}
}
double nbsDu = minu + tryu*nbsDi+dtryu*NnbsDi;

double tem1 = storemi[nbsDi];
double tem2 = storemi[nbsDi+1];

double p1x = ppuCal(4,2,nbsDu,weights,knots,xPoles);
double p1y = ppuCal(4,2,nbsDu,weights,knots,yPoles);
double p1z = ppuCal(4,2,nbsDu,weights,knots,zPoles);


sample[0]=p1x;
sample[1]=p1y;
sample[2]=p1z;
sample[3]=nbsDu;
sample[4]=NnbsDi;
sample[5]=dstoremi[dtimesu];

double u = 1;	
int k = 2;
int j = 5;
int l = k;
double p1x = dCal(l,j,k,1,weights,knots,xPoles);
double p1y = dCal(l,j,k,1,weights,knots,yPoles);
double p1z = dCal(l,j,k,1,weights,knots,zPoles);
double p2x = dCal(l,j,k,2,weights,knots,xPoles);
double p2y = dCal(l,j,k,2,weights,knots,yPoles);
double p2z = dCal(l,j,k,2,weights,knots,zPoles);

sample[0]=p1x;
sample[1]=p1y;
sample[2]=p1z;
sample[3]=p2x;
sample[4]=p2y;
sample[5]=p2z;

//double ** dPoles = (double**)malloc(5*sizeof(double*));
//for(int i = 0 ; i < 5 ; i++){
//	dPoles[i] = (double*)malloc(3*sizeof(double));
//}
//dPoles = {{0,0,1},{1.39641,0.763197,1.17608},{0.0754466,6.93397,4.06177},{6.02681,-0.194719,3.76274},{5,5,5}}
return 1;
}
*/

//int WINAPI divideNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],double ** PXYZ ,double timesP,int secNbs);

int WINAPI dllNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],double *sample,int acc,int secNbs){
NdPoles = nbsData.poles;
Nweights=nbsData.weights;
NnbsN=nbsData.weightLen -1;
NnbsK=nbsData.degree;
Nminu = nbsData.knots[0];
Nmaxu = nbsData.knots[nbsData.knotsLen-1];
updateNbs(380,-33,-400,10,NdPoles,NnbsN+1);
int midt = 0;
Nknots = (double*) malloc(sizeof(double)*34);
for (int i=0;i<NnbsK+1;i++){
Nknots[i] =nbsData.knots[0];
midt = midt+1;
}

for (int j=2;j<(nbsData.multipLen);j++){
	for (int k=0;k<(nbsData.multiplicities[j-1]);k++){
		Nknots[midt]=nbsData.knots[j-1];
		midt = midt+1;
	}
}
for (int i=0;i<NnbsK+1;i++){
Nknots[midt] =nbsData.knots[nbsData.knotsLen-1];
midt = midt+1;
}

int flagg = operateNbs(TiSpe,u0u1,zixyz,sample,acc,secNbs);
return flagg;

}

int WINAPI operateNbsC(double speed,double a1t,double a2t,double u1,double u2,double zx,double zy,double zz,double*sample,int acc,int secNbs){
double saa[3]={speed,a1t,a2t};
double u12[2]={u1,u2};
double zzz[3]={zx,zy,zz};
bsplines line;

line.poles = (double **)malloc(sizeof(double*)*22);
for(int i = 0 ;  i < 22 ; i++){
	line.poles[i] = (double*)malloc(sizeof(double) * 3);
}

for(int i = 0 ;  i < 22 ; i++){
	line.poles[i][0]= NNdPoles[i][0];
	line.poles[i][1]= NNdPoles[i][1];
	line.poles[i][2]= NNdPoles[i][2];
}
line.polesLen = 22;


line.weights = (double*)malloc(sizeof(double) *22);
line.weightLen = 22;
for(int i = 0 ;  i < 22 ; i++){
line.weights[i]=NNweights[i];
}
line.knots = (double*) malloc(sizeof(double)*3);
line.knotsLen = 3;
line.knots[0] = 0;
line.knots[1] = 6.28319;
line.knots[2] = 12.5664;
;
line.multiplicities = (int*)malloc(sizeof(int)*3);
line.multipLen = 3;
line.multiplicities[0] = 12;
line.multiplicities[1] = 10;
line.multiplicities[2] = 12;

line.degree = 11;
line.periodic = false;
line.rational = false;


int saau12 = dllNbs(line,saa,u12,zzz,sample,acc,secNbs);
return saau12;
}

int WINAPI operateNbs(double TiSpe[],double u0u1[],double zixyz[],double *sample,int acc,int secNbs){
//updateNbs(380,-33,-400,10,NdPoles,22);
double speed = TiSpe[0];
double a1t = TiSpe[1];
double a2t = TiSpe[2];
double u0 = u0u1[0];
double u1 = u0u1[1];
Ntryu = u0;// !!!!!!!!!!!!!!!!!!减小此值能 精确总距离
Ndtryu = u1; //!!!!!!!!!!!!!!!!!!!!!!减小此值 直接提升点的准确度 
double a,d;
if (secNbs==0){
Fzi[0] = zixyz[0];
Fzi[1] = zixyz[1];
Fzi[2] = zixyz[2];
}

status = acc;
if (acc ==1){
	if (a1t>-0.0000001 && a1t< 0.0000001){
		 acceleration = 0;
		 a=0;
	}
	else{
		acceleration = speed/a1t;
		a = speed/a1t;
	}
	if (a2t>-0.0000001 && a2t< 0.0000001){
		 deceleration = 0;
		 d =0;
		 
	}
	else{
		deceleration = speed/a2t;
		d = speed/a2t;
	}
	}
if (acc ==2){
	if (a1t>-0.0000001 && a1t< 0.0000001){
		 acceleration2 = 0;
		 a = 0;
	}
	else{
		acceleration2 = 2.0*speed/a1t;
		a = 2.0*speed/a1t;
	}
	if (a2t>-0.0000001 && a2t< 0.0000001){
		deceleration2 = 0;
		d=0;
	}
	else{
		deceleration2 = 2.0*speed/a2t;
		d = 2.0*speed/a2t;
	}
}


Ndets = 0.0001;

//Ntryu = 0.01;// !!!!!!!!!!!!!!!!!!减小此值能 精确总距离
//Ndtryu = 0.001; //!!!!!!!!!!!!!!!!!!!!!!减小此值 直接提升点的准确度 

Ntimesu = int((Nmaxu-Nminu)/Ntryu);
double Nlastu= (Nmaxu-Nminu) - Ntryu*Ntimesu;
Nstoremi = (double*)malloc((Ntimesu+1)*sizeof(double));
double totl =nrbsL(Ntimesu,Nminu,Nmaxu,Nlastu,Ntryu,Ndets,NnbsN,NnbsK,Nweights,Nknots,NxPoles,NyPoles,NzPoles,Nstoremi);
Ll = totl;
Lspeed = speed;

	if (acc==0){                //   匀加速
		Lt = Ll / speed;
	}

	if(acc==1){                     //梯形加速
		acctt1 = a1t;
		acctt3 = a2t;
		accT = findStayTime(speed,a1t,a2t,1,Ll);
			if (accT >=0){                        // 完整梯形 点位
				acctt2 = accT + acctt1 ;
				Lt = accT + acctt1 + acctt3;
			}
			if (accT<0){                    // 不能达到最高速度
				if (a>-0.0000001 && a< 0.0000001){	
				acctt1 = 0;
				acctt2=0;
				accT = 0;
				acctt3 = sqrt(2*Ll/d);
				Lt = acctt3;
				Lspeed= acctt3*d;
				}
				else if (d>-0.0000001 && d< 0.0000001){	
				acctt1 = sqrt(2*Ll/a);
				acctt2=  sqrt(2*Ll/a);
				acctt3 = 0;
				accT = 0;
				Lt = acctt2;
				Lspeed= acctt1*a;
				}
				else{
				acctt1 = sqrt(Ll/(0.5*a+0.5*a*a/d));
				acctt2 = acctt1;
				acctt3 = acctt1*a/d;
				accT = 0;
				Lspeed= acctt1*a;
				Lt = acctt1+acctt3;
				}
			}
	}
	if (acc==2){
		Stt = findStayTime(speed,a1t,a2t,2,Ll);
		if (Stt>=0){
		St1= a1t*0.5;
		aGradient= a/St1;
		dGradient = d/(a2t*0.5);
		Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
		Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
		Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
		Sdt = a1t;
		Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;

		Stt1 = a1t*0.5;
		Stt2 = a1t;
		Stt3 = Stt2 + Stt;
		Stt4 = Stt3 + a2t*0.5;
		Stt5 = Stt3 + a2t;
		Lt = Stt5;
		}
		else{
			if (a>-0.0000001 && a< 0.0000001){
			Stt1 = 0;
			Stt2 = 0;
			Stt3 = 0;
			Stt4 = sqrt(4*Ll/d)/2;
			Stt5 = sqrt(4*Ll/d);
			Lt = Stt5;
			speed = Stt4 *d;
			Lspeed = speed;
			dGradient = d/(speed/d);
			}
			else if (d>-0.0000001 && d< 0.0000001){
			Stt1 = sqrt(4*Ll/a)/2;
			Stt2 = 2*Stt1;
			Stt3 = Stt2;
			Stt4 = Stt2;
			Stt5= Stt2;
			Lt = Stt2;
			St1 = Stt1;
			speed = Stt1 *a;
			Lspeed = speed;
			aGradient= a/(speed/a);
			Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
			Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
			Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
			Sdt = 2*speed/a;
			Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;
			}
			else{
			speed = sqrt(Ll/((1/a)+(1/d)));
			Lspeed = sqrt(Ll/((1/a)+( 1/d)));
		
			St1= speed/a;
			aGradient= a/St1;
			dGradient = d/(speed/d);
			Svc2 = (speed/2)+0.5*aGradient*pow(St1,2)-2*a*St1;
			Sl1 = (1.0/6.0) * aGradient * pow(St1,3);
			Slc2 = Sl1+ (1.0/6.0)*aGradient*pow(St1,3) -a*pow(St1,2)-Svc2*St1;
			Sdt = 2*speed/a;
			Sl2 = -(1.0/6.0)*aGradient*pow(Sdt,3)+a*pow(Sdt,2)+Svc2*Sdt+Slc2;

			Stt1 = speed/a;
			Stt2 = 2*Stt1;
			accT = 0;
			Stt3 = Stt2;
			Stt4 = Stt3 + speed/d;
			Stt5 = Stt3 + 2*speed/d;
			Lt = Stt5;
			}
		}


	}

if (Lt>-0.0000001 && Lt< 0.0000001){
		flagOver = 1;
}
else{
		flagOver = 0;
}

if (flagOver== 1){
double p3x = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NxPoles);
double p3y = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NyPoles);
double p3z = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NzPoles);
sample[0]=p3x;
sample[1]=p3x;
sample[2]=p3x;
sample[3]=Fzi[0];
sample[4]=Fzi[1];
sample[5]=Fzi[2];
return flagOver;
}


double p1x = ppuCal(NnbsN,NnbsK,Nminu,Nweights,Nknots,NxPoles);
double p1y = ppuCal(NnbsN,NnbsK,Nminu,Nweights,Nknots,NyPoles);
double p1z = ppuCal(NnbsN,NnbsK,Nminu,Nweights,Nknots,NzPoles);

double p2x = ppuCal(NnbsN,NnbsK,Nminu+0.00001,Nweights,Nknots,NxPoles);
double p2y = ppuCal(NnbsN,NnbsK,Nminu+0.00001,Nweights,Nknots,NyPoles);
double p2z = ppuCal(NnbsN,NnbsK,Nminu+0.00001,Nweights,Nknots,NzPoles);

double p5x = ppuCal(NnbsN,NnbsK,Nminu+0.00002,Nweights,Nknots,NxPoles);
double p5y = ppuCal(NnbsN,NnbsK,Nminu+0.00002,Nweights,Nknots,NyPoles);
double p5z = ppuCal(NnbsN,NnbsK,Nminu+0.00002,Nweights,Nknots,NzPoles);
Fq[0] =p2x-p1x;
Fq[1] =p2y-p1y;
Fq[2] =p2z-p1z;
double xline1[3] ={p1x-p2x,p1y-p2y,p1z-p2z};
double yline1[3] ={p5x-p2x,p5y-p2y,p5z-p2z};
double zline1[3];
thirdline(xline1,yline1,zline1);
Fm[0]=zline1[0];
Fm[1]=zline1[1];
Fm[2]=zline1[2];

toggle= 0 ;
localTime1 = seccounter();
sample[0]=p1x;
sample[1]=p1y;
sample[2]=p1z;
sample[3]=Fzi[0];
sample[4]=Fzi[1];
sample[5]=Fzi[2];
return flagOver;
}










int WINAPI getValueNbs(double * sample){
localTime1 = seccounter();
double nbsDt = localTime1/1000.0;
double p1x,p1y,p1z;

if (nbsDt >= Lt){
	flagOver = 1;
	p1x = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NxPoles);
	p1y = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NyPoles);
	p1z = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NzPoles);
	double p3x = ppuCal(NnbsN,NnbsK,Nmaxu-0.00001,Nweights,Nknots,NxPoles);
	double p3y = ppuCal(NnbsN,NnbsK,Nmaxu-0.00001,Nweights,Nknots,NyPoles);
	double p3z = ppuCal(NnbsN,NnbsK,Nmaxu-0.00001,Nweights,Nknots,NzPoles); 
	double p5x = ppuCal(NnbsN,NnbsK,Nmaxu-0.00002,Nweights,Nknots,NxPoles);
	double p5y = ppuCal(NnbsN,NnbsK,Nmaxu-0.00002,Nweights,Nknots,NyPoles);
	double p5z = ppuCal(NnbsN,NnbsK,Nmaxu-0.00002,Nweights,Nknots,NzPoles); 
	Cq[0] =p1x-p3x;
	Cq[1] =p1y-p3y;
	Cq[2] =p1z-p3z;
	double xline1[3] ={p5x-p3x,p5y-p3y,p5z-p3z};
	double yline1[3] ={p1x-p3x,p1y-p3y,p1z-p3z};
	double zline1[3];
	thirdline(xline1,yline1,zline1);
	Cm[0]=zline1[0];
	Cm[1]=zline1[1];
	Cm[2]=zline1[2];

	double * NNnewzi = (double*)malloc(3*sizeof(double));
	operateEA(NNnewzi);
	sample[0]=p1x;
	sample[1]=p1y;
	sample[2]=p1z;
	sample[3]=NNnewzi[0];
	sample[4]=NNnewzi[1];
	sample[5]=NNnewzi[2];
	return flagOver;
}

double nbsDs = getL(nbsDt,status);

double nbsDm = nbsDs/Ndets;
int nbsDi;
int maxN = Ntimesu+1;
int minN = 0;
int toN = (maxN+minN)/2 ;
for (int i=1;i<13;i++){

if (Nstoremi[toN]>=nbsDm){
	if (maxN>toN){
	maxN = toN;
	}
}
else{
	if (minN<toN){
	minN = toN;
	}
}
toN = (maxN+minN)/2 ;
}
for (int i3=minN;i3<(maxN+1);i3++){
	if ((Nstoremi[i3]<=nbsDm)&&(nbsDm<=Nstoremi[i3+1])){
		nbsDi = i3;
		break;
	}
}

/*
int nbsDii;
for (int i4=0; i4<(Ntimesu+1);i4++){
	if ((Nstoremi[i4]<=nbsDm)&&(nbsDm<=Nstoremi[i4+1])){
		nbsDii = i4;
		break;
	}

}
*/

double dminu = Nminu +Ntryu*nbsDi;
double dmaxu = Nminu +Ntryu*(nbsDi+1);


int dtimesu = int((dmaxu-dminu)/Ndtryu);
double dlastu= (dmaxu-dminu) - Ndtryu*dtimesu;
double * dstoremi = (double*)malloc((dtimesu+1)*sizeof(double));
double dtotl =nrbsL(dtimesu,dminu,dmaxu,dlastu,Ndtryu,Ndets,NnbsN,NnbsK,Nweights,Nknots,NxPoles,NyPoles,NzPoles,dstoremi);
double dnewDm = nbsDm - Nstoremi[nbsDi];
int NnbsDi;

for (int j=0; j<(dtimesu+1);j++){
	if ((dstoremi[j]<=dnewDm)&&(dnewDm<dstoremi[j+1])){
		NnbsDi = j;
		break;
	}
}
double nbsDu = Nminu + Ntryu*nbsDi+Ndtryu*NnbsDi;

p1x = ppuCal(NnbsN,NnbsK,nbsDu,Nweights,Nknots,NxPoles);
p1y = ppuCal(NnbsN,NnbsK,nbsDu,Nweights,Nknots,NyPoles);
p1z = ppuCal(NnbsN,NnbsK,nbsDu,Nweights,Nknots,NzPoles);

double p2x = ppuCal(NnbsN,NnbsK,nbsDu+0.00001,Nweights,Nknots,NxPoles);
double p2y = ppuCal(NnbsN,NnbsK,nbsDu+0.00001,Nweights,Nknots,NyPoles);
double p2z = ppuCal(NnbsN,NnbsK,nbsDu+0.00001,Nweights,Nknots,NzPoles); 

double p5x = ppuCal(NnbsN,NnbsK,nbsDu-0.00001,Nweights,Nknots,NxPoles);
double p5y = ppuCal(NnbsN,NnbsK,nbsDu-0.00001,Nweights,Nknots,NyPoles);
double p5z = ppuCal(NnbsN,NnbsK,nbsDu-0.00001,Nweights,Nknots,NzPoles); 
Cq[0] =p2x-p1x;
Cq[1] =p2y-p1y;
Cq[2] =p2z-p1z;

double xline2[3] ={p5x-p1x,p5y-p1y,p5z-p1z};
double yline2[3] ={p2x-p1x,p2y-p1y,p2z-p1z};
double zline2[3];
thirdline(xline2,yline2,zline2);
Cm[0]=zline2[0];
Cm[1]=zline2[1];
Cm[2]=zline2[2];

double * Nnewzi = (double*)malloc(3*sizeof(double));
operateEA(Nnewzi);


sample[0]=p1x;
sample[1]=p1y;
sample[2]=p1z;
sample[3]=Nnewzi[0];
sample[4]=Nnewzi[1];
sample[5]=Nnewzi[2];


return 0;

}


double NangleCal(double fxyz[],double sfxz[]){

double dx1 =fxyz[0];
double dy1 =fxyz[1];
double dz1 =fxyz[2];
double dx2 =sfxz[0];
double dy2 =sfxz[1];
double dz2 =sfxz[2];
double uv = dx1*dx2 + dy1*dy2 + dz1*dz2;
double uSquare = dx1*dx1 + dy1*dy1 + dz1*dz1;
double vSquare = dx2*dx2 + dy2*dy2 + dz2*dz2;
double cosangle= uv / (sqrt(uSquare)* sqrt(vSquare));
double angle = acos(cosangle);
return angle;
}

void buildxyz(double fv[],double ox[],double oy[],double oz[]){
double i1,j1, k1;
i1 = fv[0];
j1 = fv[1];
k1 = fv[2];
ox[0]=i1;
ox[1]=j1;
ox[2]=k1;
if (((( -j1>-0.00000001) && ( -j1< 0.00000001)))&&((( i1>-0.00000001) && ( i1< 0.00000001)))){
oy[0]=0;
oy[1]=k1;
oy[2]=-j1;
oz[0]=-j1*j1-k1*k1;
oz[1]=i1*j1;
oz[2]=i1*k1;
}
else{
oy[0]=-j1;
oy[1]=i1;
oy[2]=0;
oz[0]=-i1*k1;
oz[1]=-j1*k1;
oz[2]=i1*i1+j1*j1;
}
}
void nineAngleCal(double ox1[],double oy1[],double oz1[],double ox2[],double oy2[],double oz2[],double changeijk[][3]){
double a1=NangleCal(ox2,ox1);
double b1=NangleCal(ox2,oy1);
double c1=NangleCal(ox2,oz1);
double a2=NangleCal(oy2,ox1);
double b2=NangleCal(oy2,oy1);
double c2=NangleCal(oy2,oz1);
double a3=NangleCal(oz2,ox1);
double b3=NangleCal(oz2,oy1);
double c3=NangleCal(oz2,oz1);
changeijk[0][0]=cos(a1);
changeijk[0][1]=cos(b1);
changeijk[0][2]=cos(c1);
changeijk[1][0]=cos(a2);
changeijk[1][1]=cos(b2);
changeijk[1][2]=cos(c2);
changeijk[2][0]=cos(a3);
changeijk[2][1]=cos(b3);
changeijk[2][2]=cos(c3);
}

int WINAPI operateEA(double * sample){

double Fox[3]={Fq[0],Fq[1],Fq[2]};
double Foy[3]={Fm[0],Fm[1],Fm[2]};
double Foz[3];
thirdline(Fox,Foy,Foz);

double zitmaxtrix[3][3];
WPRZYXTormatrix(Fzi,zitmaxtrix);
double Fzv1[3] = {zitmaxtrix[0][0],zitmaxtrix[1][0],zitmaxtrix[2][0]};
double Fzv2[3] = {zitmaxtrix[0][1],zitmaxtrix[1][1],zitmaxtrix[2][1]};
double Fzv3[3] = {zitmaxtrix[0][2],zitmaxtrix[1][2],zitmaxtrix[2][2]};

jax1 = cos(NangleCal(Fox,Fzv1));
jay1 = cos(NangleCal(Foy,Fzv1));
jaz1 = cos(NangleCal(Foz,Fzv1));

jax2 = cos(NangleCal(Fox,Fzv2));
jay2 = cos(NangleCal(Foy,Fzv2));
jaz2 = cos(NangleCal(Foz,Fzv2));

jax3 = cos(NangleCal(Fox,Fzv3));
jay3 = cos(NangleCal(Foy,Fzv3));
jaz3 = cos(NangleCal(Foz,Fzv3));

double Cox[3]={Cq[0],Cq[1],Cq[2]};
double Coy[3]={Cm[0],Cm[1],Cm[2]};
double Coz[3];
thirdline(Cox,Coy,Coz);
double sqrtCox =sqrt(pow(Cox[0],2)+pow(Cox[1],2)+pow(Cox[2],2));
double sqrtCoy =sqrt(pow(Coy[0],2)+pow(Coy[1],2)+pow(Coy[2],2));
double sqrtCoz =sqrt(pow(Coz[0],2)+pow(Coz[1],2)+pow(Coz[2],2));
double sqrtandja1[3]={sqrtCox*jax1,sqrtCoy*jay1,sqrtCoz*jaz1};
double sqrtandja2[3]={sqrtCox*jax2,sqrtCoy*jay2,sqrtCoz*jaz2};
double sqrtandja3[3]={sqrtCox*jax3,sqrtCoy*jay3,sqrtCoz*jaz3};

double jaxyz1[3][3]={{Cox[0],Cox[1],Cox[2]},{Coy[0],Coy[1],Coy[2]},{Coz[0],Coz[1],Coz[2]}};
brinv(jaxyz1[0],3);
double newzi1[3];
double newzi2[3];
double newzi3[3];
mult_matrices(jaxyz1,sqrtandja1,newzi1);
mult_matrices(jaxyz1,sqrtandja2,newzi2);
mult_matrices(jaxyz1,sqrtandja3,newzi3);


double newzimatrix[3][3]={{newzi1[0],newzi2[0],newzi3[0]},{newzi1[1],newzi2[1],newzi3[1]},{newzi1[2],newzi2[2],newzi3[2]}};

double newzi[3];
rmatrixToWPR(newzimatrix,newzi);

sample[0]=newzi[0];
sample[1]=newzi[1];
sample[2]=newzi[2];
return 0;
}



int WINAPI operateDiNbs(double * sample,int secNbs){
double TiSpe[3]={5,0,0};
double u0u1[2] = {0.01,0.001};
double zixyz[3]= {0,-150,0};
double timesP = 0.1;
int temtime = 0;


if (jj<1){
for(int i1 = 0 ; i1 < 10000 ; i1++){
	XYZ[i1] = (double*)malloc(6*sizeof(double));
}
bsplines line1;
line1.poles = (double **)malloc(sizeof(double*)*22);
for(int i = 0 ;  i < 22 ; i++){
	line1.poles[i] = (double*)malloc(sizeof(double) * 3);
}

for(int i = 0 ;  i < 22 ; i++){
	line1.poles[i][0]= NNdPoles[i][0];
	line1.poles[i][1]= NNdPoles[i][1];
	line1.poles[i][2]= NNdPoles[i][2];
}
line1.polesLen = 22;


line1.weights = (double*)malloc(sizeof(double) *22);
line1.weightLen = 22;
for(int i = 0 ;  i < 22 ; i++){
line1.weights[i]=NNweights[i];
}
line1.knots = (double*) malloc(sizeof(double)*3);
line1.knotsLen = 3;
line1.knots[0] = 0;
line1.knots[1] = 6.28319;
line1.knots[2] = 12.5664;
;
line1.multiplicities = (int*)malloc(sizeof(int)*3);
line1.multipLen = 3;
line1.multiplicities[0] = 12;
line1.multiplicities[1] = 10;
line1.multiplicities[2] = 12;

line1.degree = 11;
line1.periodic = false;
line1.rational = false;
temtime = divideNbs(line1,TiSpe,u0u1,zixyz,XYZ,timesP,secNbs);
}

sample[0]=XYZ[jj][0];
sample[1]=XYZ[jj][1];
sample[2]=XYZ[jj][2];
sample[3]=XYZ[jj][3];
sample[4]=XYZ[jj][4];
sample[5]=XYZ[jj][5];
jj = jj +1;
return temtime;
}



int WINAPI divideNbs(bsplines nbsData,double TiSpe[],double u0u1[],double zixyz[],double ** XYZ, double timesP,int secNbs){

//double ** dPoles = (double**)malloc(5*sizeof(double*));
double * sample = (double*)malloc(6*sizeof(double));
dllNbs(nbsData,TiSpe,u0u1,zixyz,sample,0,secNbs);
XYZ[0][0]=sample[0];
XYZ[0][1]=sample[1];
XYZ[0][2]=sample[2];
XYZ[0][3]=sample[3];
XYZ[0][4]=sample[4];
XYZ[0][5]=sample[5];

double nbsDt = 0;
int totalpoints = int(Ll*timesP)+1;
double longadd = 1/timesP;
double nbsll = 0;


for (int ii=1;ii<(totalpoints+1);ii++){

nbsll = nbsll + longadd;
nbsDt= nbsll/Lspeed;
double p1x,p1y,p1z;
if (nbsDt >= Lt){
	flagOver = 1;
	p1x = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NxPoles);
	p1y = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NyPoles);
	p1z = ppuCal(NnbsN,NnbsK,Nmaxu,Nweights,Nknots,NzPoles);
	double p3x = ppuCal(NnbsN,NnbsK,Nmaxu-0.00001,Nweights,Nknots,NxPoles);
	double p3y = ppuCal(NnbsN,NnbsK,Nmaxu-0.00001,Nweights,Nknots,NyPoles);
	double p3z = ppuCal(NnbsN,NnbsK,Nmaxu-0.00001,Nweights,Nknots,NzPoles); 
	double p5x = ppuCal(NnbsN,NnbsK,Nmaxu-0.00002,Nweights,Nknots,NxPoles);
	double p5y = ppuCal(NnbsN,NnbsK,Nmaxu-0.00002,Nweights,Nknots,NyPoles);
	double p5z = ppuCal(NnbsN,NnbsK,Nmaxu-0.00002,Nweights,Nknots,NzPoles); 
	Cq[0] =p1x-p3x;
	Cq[1] =p1y-p3y;
	Cq[2] =p1z-p3z;
	double xline1[3] ={p5x-p3x,p5y-p3y,p5z-p3z};
	double yline1[3] ={p1x-p3x,p1y-p3y,p1z-p3z};
	double zline1[3];
	thirdline(xline1,yline1,zline1);
	Cm[0]=zline1[0];
	Cm[1]=zline1[1];
	Cm[2]=zline1[2];

	double * NNnewzi = (double*)malloc(3*sizeof(double));
	operateEA(NNnewzi);
	XYZ[ii][0]=p1x;
	XYZ[ii][1]=p1y;
	XYZ[ii][2]=p1z;
	XYZ[ii][3]=NNnewzi[0];
	XYZ[ii][4]=NNnewzi[1];
	XYZ[ii][5]=NNnewzi[2];
}
else{
double nbsDs = getL(nbsDt,status);

double nbsDm = nbsDs/Ndets;
int nbsDi;
int maxN = Ntimesu+1;
int minN = 0;
int toN = (maxN+minN)/2 ;
for (int i=1;i<13;i++){

if (Nstoremi[toN]>=nbsDm){
	if (maxN>toN){
	maxN = toN;
	}
}
else{
	if (minN<toN){
	minN = toN;
	}
}
toN = (maxN+minN)/2 ;
}
for (int i3=minN;i3<(maxN+1);i3++){
	if ((Nstoremi[i3]<=nbsDm)&&(nbsDm<=Nstoremi[i3+1])){
		nbsDi = i3;
		break;
	}
}

double dminu = Nminu +Ntryu*nbsDi;
double dmaxu = Nminu +Ntryu*(nbsDi+1);

int dtimesu = int((dmaxu-dminu)/Ndtryu);
double dlastu= (dmaxu-dminu) - Ndtryu*dtimesu;
double * dstoremi = (double*)malloc((dtimesu+1)*sizeof(double));
double dtotl =nrbsL(dtimesu,dminu,dmaxu,dlastu,Ndtryu,Ndets,NnbsN,NnbsK,Nweights,Nknots,NxPoles,NyPoles,NzPoles,dstoremi);

double dnewDm = nbsDm - Nstoremi[nbsDi];

int NnbsDi;
for (int j=0; j<(dtimesu+1);j++){
	if ((dstoremi[j]<=dnewDm)&&(dnewDm<dstoremi[j+1])){
		NnbsDi = j;
		break;
	}
}
double nbsDu = Nminu + Ntryu*nbsDi+Ndtryu*NnbsDi;

p1x = ppuCal(NnbsN,NnbsK,nbsDu,Nweights,Nknots,NxPoles);
p1y = ppuCal(NnbsN,NnbsK,nbsDu,Nweights,Nknots,NyPoles);
p1z = ppuCal(NnbsN,NnbsK,nbsDu,Nweights,Nknots,NzPoles);

double p2x = ppuCal(NnbsN,NnbsK,nbsDu+0.00001,Nweights,Nknots,NxPoles);
double p2y = ppuCal(NnbsN,NnbsK,nbsDu+0.00001,Nweights,Nknots,NyPoles);
double p2z = ppuCal(NnbsN,NnbsK,nbsDu+0.00001,Nweights,Nknots,NzPoles); 

double p5x = ppuCal(NnbsN,NnbsK,nbsDu-0.00001,Nweights,Nknots,NxPoles);
double p5y = ppuCal(NnbsN,NnbsK,nbsDu-0.00001,Nweights,Nknots,NyPoles);
double p5z = ppuCal(NnbsN,NnbsK,nbsDu-0.00001,Nweights,Nknots,NzPoles); 
Cq[0] =p2x-p1x;
Cq[1] =p2y-p1y;
Cq[2] =p2z-p1z;

double xline2[3] ={p5x-p1x,p5y-p1y,p5z-p1z};
double yline2[3] ={p2x-p1x,p2y-p1y,p2z-p1z};
double zline2[3];
thirdline(xline2,yline2,zline2);
Cm[0]=zline2[0];
Cm[1]=zline2[1];
Cm[2]=zline2[2];

double * Nnewzi = (double*)malloc(3*sizeof(double));
operateEA(Nnewzi);
XYZ[ii][0]=p1x;
XYZ[ii][1]=p1y;
XYZ[ii][2]=p1z;
XYZ[ii][3]=Nnewzi[0];
XYZ[ii][4]=Nnewzi[1];
XYZ[ii][5]=Nnewzi[2];
}

}

localTime1 = seccounter();
return totalpoints+1;
}

