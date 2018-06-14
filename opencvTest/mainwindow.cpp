#define _CRT_SECURE_NO_WARNINGS
#include "mainwindow.h"
#include "mainwindow_all.h"

QList<IplImage*> tImageList;
QList<Mat> tImageList2;
QList<QString> tempName;
int tNum[200];
int g_threshold=96;
bool DEBUG=true;
int offsetSign=-10;
Point contourP1(0,0);
MainWindow *mainWindow;
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadPath="image/";
    xmlName="config.xml";
    mainWindow=this;
    ui->comboBox->setStyleSheet("QComboBox QAbstractItemView::item{height:40px;}");
    ui->comboBox->setView(new QListView());
    wk=new project();//微科光幕

    pro1=new project1();
    pro2=new project2();
    pro3=new project3();
    pro4=new project4();
    readme=new readMe(this);

   // QStringList list=ui->lineEdit->text().split(",");

    connect(pro2,SIGNAL(addLog(QString)),this,SLOT(writeLabel(QString)));
    connect(pro3,SIGNAL(addLog(QString)),this,SLOT(writeLabel(QString)));


    //initInterface(ui->pushButton_92);

    btnNext=NULL;


    on_pushButton_29_clicked();


    nodeList<<"checkBox"<<"checkBox1"<<"checkBox2"<<"fileIndex"<<"path"<<"checkBoxDll";
    if(!QFile::exists(xmlName))
        createXml(xmlName);


    readXml(xmlName,nodeList,nodeVal);




    if(nodeVal[0]=="1")
    {
        ui->lineEdit->setText(nodeVal[4]);
        on_pushButton_15_clicked();
        ui->comboBox->clear();

        fileList.clear();
        QDir dir(loadPath);

        dir.setFilter(QDir::Files);
        QFileInfoList list= dir.entryInfoList();
        for(int i=0;i<list.size();i++)
        {

            ui->comboBox->insertItem(0,loadPath+list[i].fileName());
            fileList.insert(0,list[i].fileName());
        }


        ui->checkBox->setChecked(nodeVal[0].toInt());
        ui->checkBox1->setChecked(nodeVal[1].toInt());
        ui->checkBox2->setChecked(nodeVal[2].toInt());
        ui->comboBox->setCurrentIndex(nodeVal[3].toInt());
        ui->lineEdit->setText(nodeVal[4]);
        ui->checkBox_dll->setChecked(nodeVal[5].toInt());

    }


}

MainWindow::~MainWindow()
{
    if(ui->checkBox->isChecked())
    {
        QStringList txtList;
        txtList<<"1";
        txtList<<(ui->checkBox1->isChecked()?"1":"0");
        txtList<<(ui->checkBox2->isChecked()?"1":"0");
        txtList<<QString::number(ui->comboBox->currentIndex());
        txtList<<ui->lineEdit->text();
        txtList<<(ui->checkBox_dll->isChecked()?"1":"0");

        updateXml(xmlName,nodeList,txtList);

    }
    delete ui;
}
void createAlphaMat(Mat &mat)
{
    for(int i = 0; i < mat.rows; ++i) {
        for(int j = 0; j < mat.cols; ++j) {
            Vec4b&rgba = mat.at<Vec4b>(i, j);
            rgba[0]= UCHAR_MAX;
            rgba[1]= saturate_cast<uchar>((float (mat.cols - j)) / ((float)mat.cols) *UCHAR_MAX);
            rgba[2]= saturate_cast<uchar>((float (mat.rows - i)) / ((float)mat.rows) *UCHAR_MAX);
            rgba[3]= saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));
        }
    }
}



void MainWindow::on_pushButton_clicked()
{
    //btnNext=ui->pushButton;

      // IplImage *img2=cvLoadImage("C:/Users/Teemo/Desktop/1116.bmp",1);

//        cvNamedWindow( "Window1", 0 ); //创建窗口
//        cvResizeWindow("Window1",500,300);
//        cvShowImage( "Window1", img ); //显示图像
 string s1=ui->comboBox->currentText().toLocal8Bit();
 //Mat img1=imread(s1,0);


 Mat img=imread(s1);

   // 初始化字体
 string words= "good luck";

 putText( img, words, Point(img.cols-img.cols/3,img.rows-img.rows/4),CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0) );



 //line(img,Point(550-90,240),Point(550-90,720),Scalar(255,255,255),6);
// line(img,Point(550+90,240),Point(550+90,720),Scalar(255,255,255),6);

  //  Mat mat(480, 640, CV_8UC4);
//    createAlphaMat(mat);
//     Rect rect(img.cols/4, img.rows/4, img.cols/2, img.rows/2);
//     Mat img1=img(rect);
//    vector<int>compression_params;
//    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//    compression_params.push_back(9);


    //namedWindow("Window1",0);
    //resizeWindow("Window1",800,600);
    qDebug()<<"channels    width:"<< img.channels()<<img.cols;
    waitKey(30);
    imshow("Window1",img);
    //imshow("Window2",img1);
   // qDebug()<<"channels    depth:"<< img.channels()<<img.depth();

  //Mat result;
  // Canny(img, result, 50, 150);




 // bool b= imwrite("C:/Users/Administrator/Desktop/12.jpg",img);
//   qDebug()<<"bool "<<b;
//   if(!b)
//   {
//       QMessageBox::about(this,"a","保存失败");
//   }


}

void MainWindow::on_pushButton_2_clicked()
{
    char *s1=ui->comboBox->currentText().toLocal8Bit().data();
    IplImage *src=cvLoadImage(s1);

    uchar *data = (uchar*)src->imageData;
    int step=src->widthStep;
    int channels=src->nChannels;
    qDebug()<<channels;

    for(int j=src->width/2-50; j<src->width/2+50; j++)
    {
       data[src->height/2*step+j*channels]=0;
       data[src->height/2*step+j*channels+1]=0;
       data[src->height/2*step+j*channels+2]=255;
    }
    for(int i=src->height/2-50; i<src->height/2+50; i++)
    {

          data[i*step+src->width/2*channels]=0;
          data[i*step+src->width/2*channels+1]=0;
          data[i*step+src->width/2*channels+2]=255;

    }
    cvNamedWindow("src",1);
    cvShowImage("src",src);
    CvSize size;
    size.width=src->width/2;
    size.height=src->height/2;
    IplImage *dst=cvCreateImage(size,src->depth,src->nChannels);
    cvResize(src,dst,CV_INTER_AREA);
    cvNamedWindow("dst",1);
    cvShowImage("dst",dst);

}

void MainWindow::on_pushButton_3_clicked()
{
     Mat img=imread("destop.png");
     Mat logo=imread("logo.jpg");
     Mat roi=img(Rect(300,300,logo.cols,logo.rows));
     qDebug()<<"cols: rows:"<<logo.cols<<logo.rows;
     Mat mask=imread("logo.jpg",0);

     //logo.copyTo(roi,mask);
     addWeighted(roi,0.5,logo,0.5,0,roi);

     namedWindow("w1",1);
     imshow("w1",img);


}

void MainWindow::on_pushButton_4_clicked()
{
     string s1=ui->comboBox->currentText().toLocal8Bit();
     Mat img=imread(s1,1);
     Mat logo=imread("logo.jpg",0);

     std::vector<Mat> c1;
      split(img,c1);
      Mat green=c1[1];
      addWeighted(green(Rect(0,0,logo.cols,logo.rows)),0.5,logo,0.5,0,green(Rect(0,0,logo.cols,logo.rows)));
     qDebug()<<"c1"<<c1.size();




     merge(c1,img);
     namedWindow("w1",1);
      imshow("w1",img);


}
Mat img;
int threshval=160;

void on_trackbar(int,void*)
{

    Mat bw =img < threshval;// threshval < 128 ? (img < threshval) : (img > threshval);
    //imshow("bw",bw);
       //定义点和向量
       vector<vector<Point> > contours;
       vector<Vec4i> hierarchy;

       //Mat img2=img.clone();
       //查找轮廓
       findContours( bw, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
       //初始化dst
       Mat dst = Mat::zeros(img.size(), CV_8UC3);

       //开始处理
       if( !contours.empty() && !hierarchy.empty() )
       {
           //遍历所有顶层轮廓，随机生成颜色值绘制给各连接组成部分
           int idx = 0;
           for( ; idx >= 0; idx = hierarchy[idx][0] )
           {
               Scalar color( (rand()&255), (rand()&255), (rand()&255) );

               //绘制填充轮廓

               drawContours( dst, contours, idx, color, /*CV_FILLED*/0, 8, hierarchy );
           }
       }
       //显示窗口
       imshow( "Connected Components", dst );
}

void MainWindow::on_pushButton_5_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    img = imread(s1, 0);
    //显示原图
    namedWindow( "Image", 1 );
    imshow( "Image", img );

    //创建处理窗口
    namedWindow( "Connected Components", 1 );
    //创建轨迹条
    createTrackbar( "Threshold", "Connected Components", &threshval, 255, on_trackbar );
    on_trackbar(threshval, 0);//轨迹条回调函数
}

int g_nContrastValue; //对比度值
int g_nBrightValue;  //亮度值
Mat g_srcImage,g_srcImage1,g_dstImage,g_grayImage;
void ContrastAndBright(int, void *)
{


    qDebug()<<g_nContrastValue;
       //三个for循环，执行运算 g_dstImage(i,j) =a*g_srcImage(i,j) + b
       for(int y = 0; y < g_srcImage.rows; y++ )
       {
              for(int x = 0; x < g_srcImage.cols; x++ )
              {
                     for(int c = 0; c < 3; c++ )
                     {
                            g_dstImage.at<Vec3b>(y,x)[c]= saturate_cast<uchar>( (g_nContrastValue*0.01)*(g_srcImage.at<Vec3b>(y,x)[c] ) + g_nBrightValue );
                     }
              }
       }

       //显示图像
       //imshow("【原始图窗口】", g_srcImage);
       imshow("【效果图窗口】", g_dstImage);
}
void MainWindow::on_pushButton_6_clicked()
{
    //读入用户提供的图像
    string s1=ui->comboBox->currentText().toLocal8Bit();
         g_srcImage= imread( s1);
                if(!g_srcImage.data )
                {
                    qDebug()<<"read error";
                    return;
                }
         g_dstImage= Mat::zeros( g_srcImage.size(), g_srcImage.type() );

         //设定对比度和亮度的初值
         g_nContrastValue=80;
         g_nBrightValue=80;

         //创建窗口
         namedWindow("【效果图窗口】", CV_WINDOW_NORMAL);
         resizeWindow("【效果图窗口】",800,600);

         //创建轨迹条
         createTrackbar("对比度：", "【效果图窗口】",&g_nContrastValue,300,ContrastAndBright );
         createTrackbar("亮   度：","【效果图窗口】",&g_nBrightValue,200,ContrastAndBright );

         //调用回调函数
         ContrastAndBright(g_nContrastValue,0);
         ContrastAndBright(g_nBrightValue,0);
         waitKey(0);
}

Mat g_dstImage1,g_dstImage2,g_dstImage3,g_dstImage4,g_dstImage5;//存储图片的Mat类型
int g_nBoxFilterValue=3;  //方框滤波参数值
int g_nMeanBlurValue=3;  //均值滤波参数值
int g_nGaussianBlurValue=3;  //高斯滤波参数值
int g_nMedianBlurValue=10;  //中值滤波参数值
int g_nBilateralFilterValue=10;  //双边滤波参数值

int g_nTrackbarNumer = 0;//0表示腐蚀erode, 1表示膨胀dilate
int g_nStructElementSize = 3; //结构元素(内核矩阵)的尺寸
void on_BoxFilter(int, void *)
{
       //方框滤波操作
       boxFilter(g_srcImage, g_dstImage1, -1,Size( g_nBoxFilterValue+1, g_nBoxFilterValue+1));
       //显示窗口
       imshow("【<1>方框滤波】", g_dstImage1);
}

//-----------------------------【on_MeanBlur( )函数】------------------------------------
//     描述：均值滤波操作的回调函数
//-----------------------------------------------------------------------------------------------
void on_MeanBlur(int, void *)
{
       //均值滤波操作
       blur(g_srcImage, g_dstImage2, Size( g_nMeanBlurValue+1, g_nMeanBlurValue+1),Point(-1,-1));
       //显示窗口
       imshow("【<2>均值滤波】", g_dstImage2);
}


//-----------------------------【on_GaussianBlur( )函数】------------------------------------
//     描述：高斯滤波操作的回调函数
//-----------------------------------------------------------------------------------------------
void on_GaussianBlur(int, void *)
{
       //高斯滤波操作
       GaussianBlur(g_srcImage, g_dstImage3, Size( g_nGaussianBlurValue*2+1,g_nGaussianBlurValue*2+1 ), 0, 0);
       //显示窗口
       imshow("【<3>高斯滤波】", g_dstImage3);
}

//-----------------------------【on_MedianBlur( )函数】------------------------------------
//            描述：中值滤波操作的回调函数
//-----------------------------------------------------------------------------------------------
void on_MedianBlur(int, void *)
{
    qDebug()<<"MedianBlue val"<<g_nMedianBlurValue*2+1;
       medianBlur( g_srcImage, g_dstImage4, g_nMedianBlurValue*2+1 );
       imshow("【<4>中值滤波】", g_dstImage4);
}


//-----------------------------【on_BilateralFilter( )函数】------------------------------------
//            描述：双边滤波操作的回调函数
//-----------------------------------------------------------------------------------------------
void on_BilateralFilter(int, void *)
{

    qDebug()<<"双边val"<<g_nBilateralFilterValue;
    bilateralFilter( g_srcImage, g_dstImage5, g_nBilateralFilterValue, g_nBilateralFilterValue*2,g_nBilateralFilterValue/2 );
    imshow("【<5>双边滤波】", g_dstImage5);
}




void MainWindow::on_pushButton_7_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_srcImage= imread(s1, 1 );
           if(!g_srcImage.data ) { qDebug()<<"Oh，no，读取srcImage错误~！\n"; return; }

           //克隆原图到三个Mat类型中
           g_dstImage1= g_srcImage.clone( );

//           //显示原图
//           namedWindow("【<0>原图窗口】", 1);
//           imshow("【<0>原图窗口】",g_srcImage);


//           //=================【<1>方框滤波】==================
//           //创建窗口
//           namedWindow("【<1>方框滤波】", 1);
//           //创建轨迹条
//           createTrackbar("内核值：", "【<1>方框滤波】",&g_nBoxFilterValue, 40,on_BoxFilter );
//           on_MeanBlur(g_nBoxFilterValue,0);
//           imshow("【<1>方框滤波】", g_dstImage1);
//           //================================================

//           //=================【<2>均值滤波】==================
//           //创建窗口
//           namedWindow("【<2>均值滤波】", 1);
//           //创建轨迹条
//           createTrackbar("内核值：", "【<2>均值滤波】",&g_nMeanBlurValue, 40,on_MeanBlur );
//           on_MeanBlur(g_nMeanBlurValue,0);
//           //================================================

//           //=================【<3>高斯滤波】=====================
//           //创建窗口
//           namedWindow("【<3>高斯滤波】", 1);
//           //创建轨迹条
//           createTrackbar("内核值：", "【<3>高斯滤波】",&g_nGaussianBlurValue, 40,on_GaussianBlur );
//           on_GaussianBlur(g_nGaussianBlurValue,0);


           //=================【<4>中值滤波】===========================
           //创建窗口
           namedWindow("【<4>中值滤波】", 1);
           //创建轨迹条
           createTrackbar("参数值：", "【<4>中值滤波】",&g_nMedianBlurValue, 50,on_MedianBlur );
           on_MedianBlur(g_nMedianBlurValue,0);
           //=======================================================


           //=================【<5>双边滤波】===========================
           //创建窗口
           namedWindow("【<5>双边滤波】", 1);
           //创建轨迹条
           createTrackbar("参数值：", "【<5>双边滤波】",&g_nBilateralFilterValue, 50,on_BilateralFilter);
           on_BilateralFilter(g_nBilateralFilterValue,0);
           //=======================================================

}
//获取自定义核
void Processing()
{
     Mat element = getStructuringElement(MORPH_RECT, Size(2*g_nStructElementSize+1,2*g_nStructElementSize+1),Point( g_nStructElementSize, g_nStructElementSize ));

     //进行腐蚀或膨胀操作
     if(g_nTrackbarNumer== 0) {
            erode(g_srcImage,g_dstImage, element);
     }
     else{
            dilate(g_srcImage,g_dstImage, element);
     }

     //显示效果图
     qDebug()<<2*g_nStructElementSize+1<<2*g_nStructElementSize+1<<g_nStructElementSize<<g_nStructElementSize<<g_nTrackbarNumer;
     imshow("【效果图】", g_dstImage);
}
void on_TrackbarNumChange(int,void*)
{
    Processing();
}
void on_ElementSizeChange(int,void*)
{
    Processing();
}


void MainWindow::on_pushButton_8_clicked()
{
    //载入原图
    string s1=ui->comboBox->currentText().toLocal8Bit();
          g_srcImage= imread(s1);
//medianBlur(g_srcImage, g_srcImage,9);
          if(!g_srcImage.data ) { qDebug()<<"Oh，no，读取srcImage错误~！\n"; return; }

          //显示原始图
//          namedWindow("【原始图】");
//          imshow("【原始图】", g_srcImage);

          //进行初次腐蚀操作并显示效果图
          namedWindow("【效果图】");
          resizeWindow("【效果图】",500,400);
          //获取自定义核
          Mat element = getStructuringElement(MORPH_RECT, Size(2*g_nStructElementSize+1,2*g_nStructElementSize+1),Point( g_nStructElementSize, g_nStructElementSize ));
          erode(g_srcImage,g_dstImage, element);
          imshow("【效果图】", g_dstImage);

          //创建轨迹条
          createTrackbar("腐蚀/膨胀", "【效果图】", &g_nTrackbarNumer, 1, on_TrackbarNumChange);
          createTrackbar("内核尺寸", "【效果图】",&g_nStructElementSize, 21, on_ElementSizeChange);

}

void MainWindow::on_pushButton_9_clicked()
{

    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat img = imread(s1, 0);

    Mat cimg;
    medianBlur(img, img, 9);
    //GaussianBlur( img, img, Size(9, 9), 2, 2 );
    cvtColor(img, cimg, COLOR_GRAY2BGR);

    vector<Vec3f> circles;
//    HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, 50,
//                 100, 30,
//                 300, 900 // change the last two parameters
//                                // (min_radius & max_radius) to detect larger circles
//                 );


    HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, 50,
                 100,50,//100, 30,
              365,420// 180, 500 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 );


    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
        qDebug()<<QString("point %1,%2").arg(c[0]).arg(c[1]);
    }
    namedWindow("detected circles",0);
    resizeWindow("detected circles",800,600);
    imshow("detected circles", cimg);
    waitKey();

}
//变量接收的TrackBar位置参数
int g_nElementShape = MORPH_RECT;//元素结构的形状//MORPH_RECT=0, MORPH_CROSS=1, MORPH_ELLIPSE=2
int g_nMaxIterationNum = 10;
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;
int g_nTopBlackHatNum = 0;
//-----------------------------------【on_OpenClose( )函数】----------------------------------
//      描述：【开运算/闭运算】窗口的回调函数
//-----------------------------------------------------------------------------------------------
void on_OpenClose(int, void*)
{
    //偏移量的定义
    int offset = g_nOpenCloseNum - g_nMaxIterationNum;//偏移量
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    qDebug()<<offset;
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(g_srcImage, g_dstImage, CV_MOP_OPEN, element);
    else
        morphologyEx(g_srcImage, g_dstImage, CV_MOP_CLOSE, element);
    //显示图像
    imshow("【开运算/闭运算】",g_dstImage);
}

void on_TopBlackHat(int, void*)
{
    //偏移量的定义
    int offset = g_nTopBlackHatNum - g_nMaxIterationNum;//偏移量
    qDebug()<<"offset:"<<offset;
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(g_srcImage, g_dstImage, MORPH_TOPHAT , element);
    else
        morphologyEx(g_srcImage, g_dstImage, MORPH_BLACKHAT, element);
    //显示图像

    imshow("【顶帽/黑帽】",g_dstImage);
}
void MainWindow::on_pushButton_10_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_srcImage = imread(s1,0);//工程目录下需要有一张名为1.jpg的素材图
     if( !g_srcImage.data ) {qDebug()<<"Oh，no，读取srcImage错误~！ \n"; return; }


     //显示原始图
//     namedWindow("【原始图】",0);
//     resizeWindow("【原始图】",1200,900);
//     imshow("【原始图】", g_srcImage);

     //创建三个窗口
     namedWindow("【开运算/闭运算】",0);
     resizeWindow("【开运算/闭运算】",1200,900);
    namedWindow("【顶帽/黑帽】",0);
    resizeWindow("【顶帽/黑帽】",1200,900);

     //参数赋值
     g_nOpenCloseNum=9;
     g_nErodeDilateNum=9;
     g_nTopBlackHatNum=2;

     //分别为三个窗口创建滚动条
     createTrackbar("迭代值", "【开运算/闭运算】",&g_nOpenCloseNum,g_nMaxIterationNum*2+1,on_OpenClose);

     createTrackbar("迭代值", "【顶帽/黑帽】",&g_nTopBlackHatNum,g_nMaxIterationNum*2+1,on_TopBlackHat);

     on_OpenClose(g_nOpenCloseNum, 0);

     on_TopBlackHat(g_nTopBlackHatNum,0);
}

void MainWindow::on_pushButton_15_clicked()
{
    ui->comboBox->clear();
    loadPath=ui->lineEdit->text().replace('\\','/');
    if(!loadPath.endsWith('/'))
        loadPath+="/";



}




Mat  g_srcGrayImage;

//Canny边缘检测相关变量
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold=1;//TrackBar位置参数

//Sobel边缘检测相关变量
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize=1;//TrackBar位置参数

//Scharr滤波器相关变量
Mat g_scharrGradient_X, g_scharrGradient_Y;
Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;


//-----------------------------------【on_Canny( )函数】----------------------------------
//      描述：Canny边缘检测窗口滚动条的回调函数
//-----------------------------------------------------------------------------------------------
void on_Canny(int, void*)
{

    // 先使用 3x3内核来降噪
    blur( g_srcGrayImage, g_cannyDetectedEdges, Size(3,3) );

    // 运行我们的Canny算子
    Canny( g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold*3, 3 );

    //先将g_dstImage内的所有元素设置为0
    g_dstImage = Scalar::all(0);

    //使用Canny算子输出的边缘图g_cannyDetectedEdges作为掩码，来将原图g_srcImage拷到目标图g_dstImage中
    g_srcImage.copyTo( g_dstImage, g_cannyDetectedEdges);

    //imwrite("b13.jpg",g_dstImage);
    //显示效果图
    imshow( "【效果图】Canny边缘检测", g_dstImage );
}


void MainWindow::on_pushButton_11_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_srcImage = imread(s1);
        // 创建与src同类型和大小的矩阵(dst)
        g_dstImage.create( g_srcImage.size(), g_srcImage.type() );
        // 将原图像转换为灰度图像
        cvtColor( g_srcImage, g_srcGrayImage, CV_BGR2GRAY );
        // 创建显示窗口
        namedWindow( "【效果图】Canny边缘检测", CV_WINDOW_AUTOSIZE );
        // 创建trackbar
        createTrackbar( "参数值：", "【效果图】Canny边缘检测", &g_cannyLowThreshold, 255, on_Canny );
        // 调用回调函数
        on_Canny(0, 0);
}
//-----------------------------------【on_Sobel( )函数】----------------------------------
//      描述：Sobel边缘检测窗口滚动条的回调函数
//-----------------------------------------------------------------------------------------
void on_Sobel(int, void*)
{
    // 求 X方向梯度
    Sobel( g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2*g_sobelKernelSize+1), 1, 1, BORDER_DEFAULT );
    convertScaleAbs( g_sobelGradient_X, g_sobelAbsGradient_X );//计算绝对值，并将结果转换成8位

    // 求Y方向梯度
    Sobel( g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2*g_sobelKernelSize+1), 1, 1, BORDER_DEFAULT );
    convertScaleAbs( g_sobelGradient_Y, g_sobelAbsGradient_Y );//计算绝对值，并将结果转换成8位

    // 合并梯度
    addWeighted( g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage );

    //显示效果图
    imshow("【效果图】Sobel边缘检测", g_dstImage);

}


//-----------------------------------【Scharr( )函数】----------------------------------
//      描述：封装了Scharr边缘检测相关代码的函数
//-----------------------------------------------------------------------------------------
void Scharr( )
{
    // 求 X方向梯度
    Scharr( g_srcImage, g_scharrGradient_X, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT );
    convertScaleAbs( g_scharrGradient_X, g_scharrAbsGradient_X );//计算绝对值，并将结果转换成8位

    // 求Y方向梯度
    Scharr( g_srcImage, g_scharrGradient_Y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT );
    convertScaleAbs( g_scharrGradient_Y, g_scharrAbsGradient_Y );//计算绝对值，并将结果转换成8位

    // 合并梯度
    addWeighted( g_scharrAbsGradient_X, 0.5, g_scharrAbsGradient_Y, 0.5, 0, g_dstImage );

    //显示效果图
    imshow("【效果图】Scharr滤波器", g_dstImage);
}

void MainWindow::on_pushButton_12_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();

    g_srcImage = imread(s1);


        //显示原始图
        namedWindow("【原始图】",0);
        resizeWindow("【原始图】",1200,900);
        imshow("【原始图】", g_srcImage);

        // 创建与src同类型和大小的矩阵(dst)
        g_dstImage.create( g_srcImage.size(), g_srcImage.type() );

        // 将原图像转换为灰度图像
        cvtColor( g_srcImage, g_srcGrayImage, CV_BGR2GRAY );

        // 创建显示窗口
        namedWindow("【效果图】Scharr滤波器", 0 );
        resizeWindow("【效果图】Scharr滤波器",1200,900);
        namedWindow( "【效果图】Canny边缘检测", 0 );
        resizeWindow("【效果图】Canny边缘检测",1200,900);
        namedWindow("【效果图】Sobel边缘检测", 0 );
        resizeWindow("【效果图】Sobel边缘检测",1200,900);

        // 创建trackbar
        createTrackbar( "参数值：", "【效果图】Canny边缘检测", &g_cannyLowThreshold, 180, on_Canny );
        createTrackbar( "参数值：", "【效果图】Sobel边缘检测", &g_sobelKernelSize, 3, on_Sobel );

        // 调用回调函数
        on_Canny(0, 0);
        on_Sobel(0, 0);

        //调用封装了Scharr边缘检测代码的函数
        Scharr();
}

void MainWindow::on_pushButton_13_clicked()
{
    //【0】创建 grad_x 和 grad_y 矩阵
       Mat grad_x, grad_y;
       Mat abs_grad_x, abs_grad_y,dst;
       string s1=ui->comboBox->currentText().toLocal8Bit();

       //【1】载入原始图
       Mat src = imread(s1);  //工程目录下应该有一张名为1.jpg的素材图

       //【2】显示原始图
       imshow("【原始图】sobel边缘检测", src);

       //【3】求 X方向梯度
       Sobel( src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT );
       convertScaleAbs( grad_x, abs_grad_x );
       imshow("【效果图】 X方向Sobel", abs_grad_x);


       //【4】求Y方向梯度
       Sobel( src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT );
       convertScaleAbs( grad_y, abs_grad_y );
       imshow("【效果图】Y方向Sobel", abs_grad_y);

       //【5】合并梯度(近似)
       addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst );

       imshow("【效果图】整体方向Sobel", dst);

}

void MainWindow::on_pushButton_14_clicked()
{
    Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y,dst;
    string s1=ui->comboBox->currentText().toLocal8Bit();

    //【1】载入原始图
    Mat src = imread(s1);  //工程目录下应该有一张名为1.jpg的素材图
    //【2】显示原始图
       imshow("【原始图】Scharr滤波器", src);

       //【3】求 X方向梯度
       Scharr( src, grad_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT );
       convertScaleAbs( grad_x, abs_grad_x );
       imshow("【效果图】 X方向Scharr", abs_grad_x);

       //【4】求Y方向梯度
       Scharr( src, grad_y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT );
       convertScaleAbs( grad_y, abs_grad_y );
       imshow("【效果图】Y方向Scharr", abs_grad_y);

       //【5】合并梯度(近似)
       addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst );

       //【6】显示效果图
       imshow("【效果图】合并梯度后Scharr", dst);
}

void MainWindow::on_pushButton_16_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat dst;
    Mat src=imread(s1);
     //指定fx和fy，让函数计算出目标图像的大小。
    cv::resize(src, dst, Size(), 0.8, 0.8);
    //cv::resize(src, dst, cv::Size(src.cols-100, src.rows-100), (0, 0), (0, 0), cv::CV_INTER_AREA);
    imshow("src16",src);
    imshow("dst16",dst);

}

void MainWindow::on_pushButton_17_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat srcImage = imread(s1);
    Mat tmpImage,dstImage;//临时变量和目标图的定义
    tmpImage=srcImage;//将原始图赋给临时变量

    //显示原始图
    imshow("【原始图】", srcImage);
    //进行向上取样操作
    //pyrUp( tmpImage, dstImage, Size( tmpImage.cols*2, tmpImage.rows*2 ) );
    pyrUp( tmpImage, dstImage, Size( tmpImage.cols, tmpImage.rows ) );
    imshow("up", dstImage);
    //pyrDown( tmpImage, dstImage, Size( tmpImage.cols/2, tmpImage.rows/2 ) );
    pyrDown( tmpImage, dstImage, Size( tmpImage.cols, tmpImage.rows ) );
    imshow("down", dstImage);


}
Mat srcImage,midImage,dstImage,dstImage2;
void on_cvCanny(int,void*)
{

dstImage=srcImage.clone();



//medianBlur(dstImage, dstImage, 9);
//GaussianBlur( dstImage, dstImage, Size(9, 9), 2, 2 );
    Canny(dstImage, midImage, g_threshold, g_threshold*3, 3);
    //Canny(srcImage, midImage, 50, 200, 3);
    g_srcImage1=srcImage.clone();
   cvtColor(midImage,dstImage2, CV_GRAY2BGR);
//    vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
//    HoughLines(midImage, lines, 1, CV_PI/180, 150, 0, 0 );

//    //【4】依次在图中绘制出每条线段
//    for( size_t i = 0; i < lines.size(); i++ )
//    {
//        float rho = lines[i][0], theta = lines[i][1];
//        Point pt1, pt2;
//        double a = cos(theta), b = sin(theta);
//        double x0 = a*rho, y0 = b*rho;
//        pt1.x = cvRound(x0 + 1000*(-b));
//        pt1.y = cvRound(y0 + 1000*(a));
//        pt2.x = cvRound(x0 - 1000*(-b));
//        pt2.y = cvRound(y0 - 1000*(a));
//        line( dstImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);

//    }

   vector<Vec4i> linesP;

   //HoughLinesP(midImage, linesP, 1, CV_PI/180, 80, 50,10 );
   //HoughLinesP(midImage, linesP, 1, CV_PI/180,80, 20,5 );//最后一个参数同一条直线最大间隙如：_____ _____
HoughLinesP(midImage, linesP, 1, CV_PI/180, 80, 100,30 );
   //【4】依次在图中绘制出每条线段
   for( size_t i = 0; i < linesP.size(); i++ )
   {

       Vec4i l = linesP[i];


       line( g_srcImage1, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
   }


   //【5】显示原始图
   imshow("【效果图】", dstImage2);
   imshow("18_1", g_srcImage1);


}
void MainWindow::on_pushButton_18_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    srcImage = imread(s1);
    namedWindow("18_1", 0 );
    createTrackbar("0", "18_1",&g_threshold, 400,on_cvCanny );
    on_cvCanny(0,0);

}
int floodFillParam3=0;
int num1=0;
void on_floodFill4(int,void*)
{
    Rect ccomp;
    Mat m;
    g_srcImage.copyTo(m);
    floodFill(m, Point(50,300), Scalar(0, 0,0), &ccomp, Scalar(floodFillParam3, floodFillParam3, floodFillParam3),Scalar(floodFillParam3, floodFillParam3, floodFillParam3));
    int *rgb=m.ptr<int>(100);
    char *c=(char*)rgb;
    qDebug()<<QString::number(c[0])<<QString::number(c[1])<<QString::number(c[2])<<QString::number(c[3]);
    imshow("19",m);
}
void MainWindow::on_pushButton_19_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_srcImage= imread(s1);
    namedWindow("19",1);
    createTrackbar("0", "19",&floodFillParam3, 20,on_floodFill4 );
    on_floodFill4(floodFillParam3,0);

}
int thresh = 140; //当前阈值
int max_thresh = 255; //最大阈值
void on_CornerHarris( int, void* )
{
    //---------------------------【1】定义一些局部变量-----------------------------
    Mat dstImage;//目标图
    Mat normImage;//归一化后的图
    Mat scaledImage;//线性变换后的八位无符号整型的图

    //---------------------------【2】初始化---------------------------------------
    //置零当前需要显示的两幅图，即清除上一次调用此函数时他们的值
    dstImage = Mat::zeros( g_srcImage.size(), CV_32FC1 );
    g_srcImage1=g_srcImage.clone( );

    //---------------------------【3】正式检测-------------------------------------
    //进行角点检测
    cornerHarris( g_grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT );

    // 归一化与转换
    normalize( dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( normImage, scaledImage );//将归一化后的图线性变换成8位无符号整型

    //---------------------------【4】进行绘制-------------------------------------
    // 将检测到的，且符合阈值条件的角点绘制出来
    for( int j = 0; j < normImage.rows ; j++ )
    { for( int i = 0; i < normImage.cols; i++ )
    {
        if(  normImage.at<float>(j,i) > thresh+80 )
        {
            circle( g_srcImage1, Point( i, j ), 5,  Scalar(10,10,255), 2, 8, 0 );
            circle( scaledImage, Point( i, j ), 5,  Scalar(0,10,255), 2, 8, 0 );
        }
    }
    }
    //---------------------------【4】显示最终效果---------------------------------
    imshow( "20", g_srcImage1 );
   // imshow( "20_2", scaledImage );

}

void MainWindow::on_pushButton_20_clicked()
{
        string s1=ui->comboBox->currentText().toLocal8Bit();
        g_srcImage = imread( s1, 1 );

        // imshow("原始图",g_srcImage);
        g_srcImage1=g_srcImage.clone( );

        //【2】存留一张灰度图
        cvtColor( g_srcImage1, g_grayImage, CV_BGR2GRAY );

        //【3】创建窗口和滚动条
        namedWindow( "20", 0 );
        resizeWindow("20",800,600);
        createTrackbar( "阈值: ","20", &thresh, max_thresh, on_CornerHarris );

        //【4】调用一次回调函数，进行初始化
        on_CornerHarris( 0, 0 );
}

void on_threshold(int,void*)
{
    threshold(g_srcImage, g_dstImage, g_threshold, 255, THRESH_BINARY);
    imshow("21", g_dstImage);
}
void MainWindow::on_pushButton_21_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_srcImage= imread(s1, 0);
    medianBlur(g_srcImage, g_srcImage, 3);
    namedWindow("21", 0 );
    cvResizeWindow("21",1280,960);
    g_dstImage.create(g_srcImage.size(),g_srcImage.type());

    namedWindow("src", 0 );
    cvResizeWindow("src",1280,960);
    createTrackbar("0", "src",&g_threshold, 255,on_threshold );
    on_threshold(0,0);
    imshow("src",g_srcImage);
}

void MainWindow::on_pushButton_22_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    //【0】变量定义
     Mat srcImage, dstImage, dstImage2;
     Mat map_x, map_y,map2_x,map2_y;

     //【1】载入原始图
     srcImage = imread( s1, 1 );

     imshow("原始图",srcImage);

     //【2】创建和原始图一样的效果图，x重映射图，y重映射图
     dstImage.create( srcImage.size(), srcImage.type() );
     map_x.create( srcImage.size(), CV_32FC1 );
     map_y.create( srcImage.size(), CV_32FC1 );

     map2_x.create( srcImage.size(), CV_32FC1 );
     map2_y.create( srcImage.size(), CV_32FC1 );

     //【3】双层循环，遍历每一个像素点，改变map_x & map_y的值
     for( int j = 0; j < srcImage.rows;j++)
     {
         for( int i = 0; i < srcImage.cols;i++)
         {


             map_x.at<float>(j,i) = static_cast<float>(srcImage.cols - i);
             map_y.at<float>(j,i) = static_cast<float>(j);

             map2_x.at<float>(j,i) = static_cast<float>(/*srcImage.cols- 上下反转并左右反转*/i);
             map2_y.at<float>(j,i) = static_cast<float>(srcImage.rows - j);


         }
     }

     //【4】进行重映射操作
     remap( srcImage, dstImage, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0) );

     remap( srcImage, dstImage2, map2_x, map2_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0) );

     //【5】显示效果图
     imshow( "左右", dstImage );
     imshow( "上下", dstImage2 );
     imwrite("77.jpg",dstImage);
}

void MainWindow::on_pushButton_23_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat srcImage1 = imread(s1, 1 );



    //【2】定义需要用到的变量和类
    int minHessian = 1000;//定义SURF中的hessian阈值特征点检测算子
    SurfFeatureDetector detector( minHessian );//定义一个SurfFeatureDetector（SURF） 特征检测类对象
    std::vector<KeyPoint> keypoints_1;//vector模板类是能够存放任意类型的动态数组，能够增加和压缩数据

    //【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
    detector.detect( srcImage1, keypoints_1 );

    //【4】绘制特征关键点
    Mat img_keypoints_1;
    drawKeypoints( srcImage1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    cv::resize(img_keypoints_1, img_keypoints_1, Size(), 0.7, 0.7);
    //【5】显示效果图
    int angleArr[360]={0};

    for(unsigned int i=0;i<keypoints_1.size();i++)
    {
        int tmp=keypoints_1[i].angle;
        angleArr[tmp]++;
    }
    int t1=0;
    for(int i=0;i<360;i++)
    {
        if(angleArr[i]>t1)
            t1=angleArr[i];

    }
    qDebug()<<"max num:"<<t1;
    for(int i=0;i<360;i++)
    {
        if(angleArr[i]==t1)
            qDebug()<<"angle:"<<i;
    }



    imshow(s1, img_keypoints_1 );

}

void MainWindow::on_pushButton_24_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Point2f srcTriangle[3];
       Point2f dstTriangle[3];
       //定义一些Mat变量
       Mat rotMat( 2, 3, CV_32FC1 );
       Mat warpMat( 2, 3, CV_32FC1 );
       Mat srcImage, dstImage_warp, dstImage_warp_rotate;

       //【2】加载源图像并作一些初始化
       srcImage = imread(s1, 1 );

       // 设置目标图像的大小和类型与源图像一致
       dstImage_warp = Mat::zeros( srcImage.rows, srcImage.cols, srcImage.type() );

       //【3】设置源图像和目标图像上的三组点以计算仿射变换
       srcTriangle[0] = Point2f( 0,0 );
       srcTriangle[1] = Point2f( static_cast<float>(srcImage.cols - 1), 0 );
       srcTriangle[2] = Point2f( 0, static_cast<float>(srcImage.rows - 1 ));

       dstTriangle[0] = Point2f( static_cast<float>(srcImage.cols*0.0), static_cast<float>(0));
       dstTriangle[1] = Point2f( static_cast<float>(srcImage.cols*0.5), static_cast<float>(0));
       dstTriangle[2] = Point2f( static_cast<float>(0), static_cast<float>(srcImage.rows*0.5));

       //【4】求得仿射变换
       warpMat = getAffineTransform( srcTriangle, dstTriangle );

       //【5】对源图像应用刚刚求得的仿射变换
       warpAffine( srcImage, dstImage_warp, warpMat, dstImage_warp.size() );

       //【6】对图像进行缩放后再旋转
       // 计算绕图像中点顺时针旋转50度缩放因子为0.6的旋转矩阵
       Point center = Point( dstImage_warp.cols/2, dstImage_warp.rows/2 );
       double angle = -30.0;
       double scale = 0.8;
       // 通过上面的旋转细节信息求得旋转矩阵
       rotMat = getRotationMatrix2D( center, angle, scale );
       // 旋转已缩放后的图像
       warpAffine( dstImage_warp, dstImage_warp_rotate, rotMat, dstImage_warp.size() );


       //【7】显示结果

       imshow( "24_1", dstImage_warp );
       imshow( "24_2", dstImage_warp_rotate );
}

void MainWindow::on_pushButton_25_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_srcImage=imread("E:/qtProject/opencvTest/image/04.jpg",1);
    g_dstImage=imread("E:/qtProject/opencvTest/image/03.jpg",1);
    Mat srcImage1, srcImage2;
    srcImage1.create(g_srcImage.size(),g_srcImage.type());
    srcImage2.create(g_srcImage.size(),g_srcImage.type());
    cv::resize(g_srcImage, srcImage1, Size(), 0.5, 0.5);
    cv::resize(g_dstImage, srcImage2, Size(), 0.5, 0.5);
        //【2】使用SURF算子检测关键点
        int minHessian = 1000;//SURF算法中的hessian阈值
        SurfFeatureDetector detector( minHessian );//定义一个SurfFeatureDetector（SURF） 特征检测类对象
        std::vector<KeyPoint> keyPoint1, keyPoints2;//vector模板类，存放任意类型的动态数组

        //【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
        detector.detect( srcImage1, keyPoint1 );
        detector.detect( srcImage2, keyPoints2 );

        //【4】计算描述符（特征向量）
        SurfDescriptorExtractor extractor;
        Mat descriptors1, descriptors2;
        extractor.compute( srcImage1, keyPoint1, descriptors1 );
        extractor.compute( srcImage2, keyPoints2, descriptors2 );

        //【5】使用BruteForce进行匹配
        // 实例化一个匹配器
        BruteForceMatcher< L2<float> > matcher;
        std::vector< DMatch > matches;
        //匹配两幅图中的描述子（descriptors）
        matcher.match( descriptors1, descriptors2, matches );

        //【6】绘制从两个图像中匹配出的关键点
        Mat imgMatches;
        drawMatches( srcImage1, keyPoint1, srcImage2, keyPoints2, matches, imgMatches );//进行绘制

        //【7】显示效果图
        imshow("匹配图", imgMatches );
}
Mat g_templateImage,g_resultImage;
int g_nMatchMethod;
string winName="平方差,归一化平方差,相关,归一化相关,相关系数,归一化相关系数";
void on_Matching( int, void* )
{
    //g_nMatchMethod=5;
    qDebug()<<"g_nMatchMethod"<<g_nMatchMethod;
    //【1】给局部变量初始化
    Mat srcImage;
    g_srcImage.copyTo( srcImage );
\
    //【2】初始化用于结果输出的矩阵
    int resultImage_cols =  g_srcImage.cols - g_templateImage.cols + 1;
    int resultImage_rows = g_srcImage.rows - g_templateImage.rows + 1;
    g_resultImage.create( resultImage_cols, resultImage_rows, CV_32FC1 );

    //【3】进行匹配和标准化
   // matchTemplate( g_srcImage, g_templateImage, g_resultImage, g_nMatchMethod );
    matchTemplate( g_srcImage, g_templateImage, g_resultImage, g_nMatchMethod );
    //normalize( g_resultImage, g_resultImage, 0, 1, NORM_MINMAX, -1, Mat() );

    //【4】通过函数 minMaxLoc 定位最匹配的位置
    double minValue; double maxValue; Point minLocation; Point maxLocation;
    Point matchLocation;
    minMaxLoc( g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation );
    long long min1=minValue/10000;
    long long max1=maxValue/10000;

    //【5】对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值有着更高的匹配结果. 而其余的方法, 数值越大匹配效果越好
    if( g_nMatchMethod  == CV_TM_SQDIFF || g_nMatchMethod == CV_TM_SQDIFF_NORMED )
    { matchLocation = minLocation; }
    else
    { matchLocation = maxLocation; }

    //【6】绘制出矩形，并显示最终结果
    rectangle( srcImage, matchLocation, Point( matchLocation.x + g_templateImage.cols , matchLocation.y + g_templateImage.rows ), Scalar(0,0,255), 2, 8, 0 );
    rectangle( g_resultImage, matchLocation, Point( matchLocation.x + g_templateImage.cols , matchLocation.y + g_templateImage.rows ), Scalar(0,0,255), 2, 8, 0 );

    //        CV_TM_SQDIFF        =0,
    //        CV_TM_SQDIFF_NORMED =1,
    //        CV_TM_CCORR         =2,
    //        CV_TM_CCORR_NORMED  =3,
    //        CV_TM_CCOEFF        =4,
    //        CV_TM_CCOEFF_NORMED =5
    switch(g_nMatchMethod)
    {
    case CV_TM_SQDIFF:
        minValue/(g_templateImage.cols * g_templateImage.cols);
        break;
    case CV_TM_CCOEFF:
        maxValue/(g_templateImage.cols * g_templateImage.cols);
        break;
    }
    qDebug()<<"method"<<g_nMatchMethod<<"min max"<<min1<<max1<<"pos"<<matchLocation.x + g_templateImage.cols<<matchLocation.y + g_templateImage.rows;
    //qDebug()<<"min max"<<minValue/(g_templateImage.cols*g_templateImage.cols)<<maxValue/(g_templateImage.cols*g_templateImage.cols);
    imshow( winName, srcImage );
    imshow( "26_2", g_resultImage );

}
void MainWindow::on_pushButton_26_clicked()
{
    btnNext=ui->pushButton_26;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat m1 = imread( s1, 0 );
    Rect roi(178,330,700,300);
    g_srcImage=m1(roi);
    g_templateImage = imread( "temp2/1.png", 0 );
    namedWindow(winName,0);
    resizeWindow(winName,800,600);

    createTrackbar( "方法", winName, &g_nMatchMethod, 5, on_Matching );
    //createTrackbar( "方法", winName, &g_nMatchMethod, 1, on_Matching );
    on_Matching( 0, 0 );
}

void MainWindow::on_pushButton_27_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    const char *pstrCascadeFileName = "haarcascade_frontalface_alt.xml";
        CvHaarClassifierCascade *pHaarCascade = NULL;
        pHaarCascade = (CvHaarClassifierCascade*)cvLoad(pstrCascadeFileName);

        // 载入图像

        IplImage *pSrcImage = cvLoadImage(s1.data(), CV_LOAD_IMAGE_UNCHANGED);

        IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
        cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);

        // 人脸识别与标记
        if (pHaarCascade != NULL)
        {
            CvScalar FaceCirclecolors[] =
            {
                {{0, 0, 255}},
                {{0, 128, 255}},
                {{0, 255, 255}},
                {{0, 255, 0}},
                {{255, 128, 0}},
                {{255, 255, 0}},
                {{255, 0, 0}},
                {{255, 0, 255}}
            };

            CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
            cvClearMemStorage(pcvMStorage);
            // 识别
            DWORD dwTimeBegin, dwTimeEnd;
            dwTimeBegin = GetTickCount();
            CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage);
            dwTimeEnd = GetTickCount();

            printf("人脸个数: %d   识别用时: %d ms\n", pcvSeqFaces->total, dwTimeEnd - dwTimeBegin);

            // 标记
            for(int i = 0; i <pcvSeqFaces->total; i++)
            {
                CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
                CvPoint center;
                int radius;
                center.x = cvRound((r->x + r->width * 0.5));
                center.y = cvRound((r->y + r->height * 0.5));
                radius = cvRound((r->width + r->height) * 0.25);
                cvCircle(pSrcImage, center, radius, FaceCirclecolors[i % 8], 2);
            }
            cvReleaseMemStorage(&pcvMStorage);
        }

        const char *pstrWindowsTitle = "人脸识别 (http://blog.csdn.net/MoreWindows)";
        cvNamedWindow(pstrWindowsTitle, CV_WINDOW_AUTOSIZE);
        cvShowImage(pstrWindowsTitle, pSrcImage);

//        cvWaitKey(0);
//        cvDestroyWindow(pstrWindowsTitle);
//        cvReleaseImage(&pSrcImage);
//        cvReleaseImage(&pGrayImage);
}

void MainWindow::on_pushButton_28_clicked()
{
     string s1=ui->comboBox->currentText().toLocal8Bit();
      IplImage *pSrcImage = cvLoadImage(s1.data(), 0);
      cvSetImageROI(pSrcImage,cvRect(263,265,30,30));
       CvScalar scalar = cvAvg(pSrcImage);

     double d= scalar.val[0];
     cvReleaseImage(&pSrcImage);
     QMessageBox::about(this,"a",QString::number(d));
}

void MainWindow::on_pushButton_29_clicked()
{
    ui->comboBox->clear();

    fileList.clear();
    QDir dir(loadPath);
    dir.setFilter(QDir::Files);
    QFileInfoList list= dir.entryInfoList();
    for(int i=0;i<list.size();i++)
    {

        ui->comboBox->insertItem(0,loadPath+list[i].fileName());
        fileList.insert(0,list[i].fileName());
    }


    tempName.clear();
    for(int i=0;i<tImageList.size();i++)
    {
        cvReleaseImage(&tImageList[i]);
    }
    tImageList.clear();
    QDir dir2("temp/");
    dir2.setFilter(QDir::Files);
    QFileInfoList listTemp= dir2.entryInfoList();
    for(int i=0;i<listTemp.size();i++)
    {
        QString path="temp/"+listTemp[i].fileName();
        tImageList.append(cvLoadImage(path.toLocal8Bit().data(),0));
        tImageList2.append(imread(path.toLocal8Bit().data(),0));
        tempName.append(listTemp[i].fileName());
    }

    //pro3->loadShapePoint(20);
    ui->comboBox->setCurrentIndex(0);





}
int bSums(Mat src)
{

    int counter = 0;
    //迭代器访问像素点
    Mat_<uchar>::iterator it = src.begin<uchar>();
    Mat_<uchar>::iterator itend = src.end<uchar>();
    for (; it!=itend; ++it)
    {
        if((*it)>0) counter+=1;//二值化后，像素点是0或者255
    }
    return counter;
}
void MainWindow::on_pushButton_30_clicked()
{
     string s1=ui->comboBox->currentText().toLocal8Bit();
     g_srcImage = imread( s1, 0);


     Mat roi=g_srcImage(Rect(687,669,45,35));
     threshold(roi, roi, 120, 255, THRESH_BINARY);
     int num=bSums(roi);
     imshow("30",roi);
     QMessageBox::about(this,QString::number(countNonZero(roi)),QString::number(num));
}

void MainWindow::on_pushButton_31_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat img = imread(s1, 0);
    float angle1=360;
    Point p1;
    pro1->calcWork(img,p1,angle1);
    qDebug()<<angle1<<p1.x<<p1.y;

    waitKey();
}

void MainWindow::on_pushButton_32_clicked()
{
    Mat M(300,300,CV_8UC3,Scalar(0,0,0));
       for(int i=140;i<160;i++)
       {
           for(int j=140;j<160;j++)
           {
               M.at<Vec3b>(i,j)[0]=81.0;
               M.at<Vec3b>(i,j)[1]=119.0;
               M.at<Vec3b>(i,j)[2]=191.0;

           }

       }

       imshow("a",M);



       Mat M1(300,500,CV_8UC1,Scalar::all(0));
          for(int i=M1.rows/2-M1.rows/10;i<M1.rows/2+M1.rows/10;i++)
          {
              for(int j=M1.cols/2-M1.cols/10;j<M1.cols/2+M1.cols/10;j++)
              {
                  M1.at<uchar>(i,j)=30;


              }

          }

          imshow("a1",M1);

        string s1=ui->comboBox->currentText().toLocal8Bit();
        Mat M2 = imread(s1, 0);
        qDebug()<< M2.at<uchar>(281,594);

}

QTimer *t1;
void MainWindow::on_pushButton_33_clicked()
{


    if(ui->pushButton_33->text()=="停止")
    {
        QString path=QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
        path.replace("\\","/");
        QString time=QTime::currentTime().toString("hhmmss");
        imwrite(QString("%1/%2.jpg").arg(path,time).toStdString(),frame);
        t1->stop();
        cap->release();
        ui->pushButton_33->setText("摄像机");
        return;
    }

    cap=new VideoCapture(1);

    if(!cap->isOpened())
    {
        return ;
    }
    t1=new QTimer(this);
    connect(t1,SIGNAL(timeout()),this,SLOT(timer1()));
    t1->start(300);
    ui->pushButton_33->setText("停止");
}

void MainWindow::timer1()
{
    *cap>>frame;
    static int i=0;
    qDebug()<<++i<<frame.cols<<frame.rows;
    imshow("当前视频",frame);
}

QStringList list1;
void MainWindow::timer3()
{
    static int i=0;
    if(i==fileList.size())
    {
        t3->stop();
        return;
    }

    char* temp;
    IplImage* srcImage=NULL;
    QByteArray ba = (loadPath+fileList[i]).toLocal8Bit();
    temp=ba.data();

    CvRect rect1;
    rect1.x = 179;
    rect1.y = 12;//60
    rect1.width = 1534 - 179;
    rect1.height =1234- 12;
    CvRect rect2;
    rect2.x = 446;
    rect2.y = 549;
    rect2.width = 623 -446;
    rect2.height = 757 -549;
    srcImage = cvLoadImage(temp,0);
    //std::cout<<list1[i].toStdString()<<std::endl;
    zt_visulLocation(srcImage,rect1,rect2,130,80,607,655,0.0,0.06179,4.6,-4);
    cvReleaseImage(&srcImage);
    qDebug()<<fileList[i];
    ++i;

    ui->label->setText(QString::number(i));

}

QFileInfoList TemplateName;//模板列表

void MainWindow::on_pushButton_34_clicked()
{
    btnNext=ui->pushButton_34;
    bool loop=false;

    if(loop)
    {


        t3=new QTimer(this);
        connect(t3,SIGNAL(timeout()),this,SLOT(timer3()));
        t3->start(50);
        return;
    }

    char* temp;
    IplImage* srcImage=NULL;
    QByteArray ba = ui->comboBox->currentText().toLocal8Bit();
    temp=ba.data();

    CvRect rect1;
    rect1.x = 179;
    rect1.y = 12;//60
    rect1.width = 1534 - 179;
    rect1.height =1234- 12;
    CvRect rect2;
    rect2.x = 446;
    rect2.y = 549;
    rect2.width = 623 -446;
    rect2.height = 757 -549;
    srcImage = cvLoadImage(temp,0);

    zt_visulLocation(srcImage,rect1,rect2,130,80,607,655,0.0,0.06179,4.6,-4);
    cvReleaseImage(&srcImage);
}
IplImage *templateImage,*templateImage2;

void MainWindow::timer2()
{

}

void MainWindow::on_pushButton_35_clicked()
{

    if(medianBlurVal==3)
    {
        ui->pushButton_35->setText("5");
        medianBlurVal=5;
    }
    else if(medianBlurVal==5)
    {
        ui->pushButton_35->setText("9");
        medianBlurVal=9;
    }
    else if(medianBlurVal==9)
    {
        ui->pushButton_35->setText("0");
        medianBlurVal=0;
    }else if(medianBlurVal==0)
    {
        ui->pushButton_35->setText("3");
        medianBlurVal=3;
    }
}

void MainWindow::on_pushButton_36_clicked()
{
    btnNext=ui->pushButton_36;
    CvRect r1=cvRect(648,659,132,75);
    //CvRect r1=cvRect(648,667,132,50);

    long int result=-1;
    int i=0;
    int bestMatch=1000000;
    CvPoint  minloc, maxloc;
    double   minval, maxval;
    IplImage *pSrcImage = cvLoadImage(ui->comboBox->currentText().toLocal8Bit().data(), 0);
    QTime t;
    t.start();

    for(int j=0;j<tImageList.size();j++)
    {
        cvSetImageROI(pSrcImage,r1);
        IplImage *outImage = cvCreateImage( cvSize(r1.width-tImageList[j]->width+1,r1.height-tImageList[j]->height+1 ), IPL_DEPTH_32F , 1 );

        cvMatchTemplate(pSrcImage,tImageList[j],outImage,CV_TM_SQDIFF );
        cvResetImageROI(pSrcImage);

        cvMinMaxLoc( outImage, &minval, &maxval, &minloc, &maxloc, 0 );
        cvRectangle( pSrcImage, minloc, cvPoint( minloc.x + tImageList[j]->width , minloc.y + tImageList[j]->height ), cvScalar(0,0,0), 2, 8, 0 );
        result= long int(minval/10000);

        qDebug()<<"result"<<result<<"template Image:"<<tempName[j];;
        if(result<bestMatch)
        {
            bestMatch=result;
            i=j;

        }
         cvReleaseImage(&outImage);
    }

    // 初始化字体
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 5,5,0,2);//初始化字体，准备写到图片上的
    // cvPoint 为起笔的x，y坐标
    cvPutText(pSrcImage,QString::number(bestMatch).toLocal8Bit().data(),cvPoint(500,900),&font,CV_RGB(255,255,255));//在图片中输出字符
    cvRectangle(pSrcImage,cvPoint(648,659),cvPoint(648+132,659+75),cvScalar(0,0,0));
    cvNamedWindow("aaa",0);
    cvResizeWindow("aaa",800,600);
    cvShowImage("aaa",pSrcImage);
    cvReleaseImage(&pSrcImage);
    qDebug("Time elapsed: %d ms", t.elapsed());
}

void MainWindow::on_pushButton_37_clicked()
{
    pro1->detectContours2();
}

Mat src_gray;
int contourArr[100], contourAngle[100],contourAngle2[100],contourAngle3[100];
Point contourPoint[100];
extern bool useBlackH;
void MainWindow::on_pushButton_38_clicked()
{
    //btnNext=ui->pushButton_38;
    string s1=ui->comboBox->currentText().toLocal8Bit();
     Rect roi(470,220,400,400);
     g_dstImage1=imread(s1,0);//(roi);
     g_dstImage2=imread(s1,1);//(roi);

      medianBlur(g_dstImage1, g_dstImage1, medianBlurVal);
     useBlackH=false;
//    Mat s2=imread(s1,0);
//    Mat s3=imread(s1,1);
//    Rect r(894,860,1477-894,1448-880);
//    g_dstImage1=s2(r);
//    g_dstImage2=s3(r);

    pro2->min_rect1();
}

void MainWindow::on_pushButton_39_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();

       //从文件中读入图像
       Mat img2 = imread(s1);
       Mat img=imread("image/3.png");


       //如果读入图像失败

       //显示图像
       imshow("image before", img);
       imshow("image2 before",img2);


       //sift特征检测
       SiftFeatureDetector  siftdtc;
       vector<KeyPoint>kp1,kp2;

       siftdtc.detect(img,kp1);
       Mat outimg1;
       drawKeypoints(img,kp1,outimg1);
       imshow("image1 keypoints",outimg1);
       KeyPoint kp;

       vector<KeyPoint>::iterator itvc;
       for(itvc=kp1.begin();itvc!=kp1.end();itvc++)
       {
           std::cout<<"angle:"<<itvc->angle<<"\t"<<itvc->class_id<<"\t"<<itvc->octave<<"\t"<<itvc->pt<<"\t"<<itvc->response<<std::endl;
       }

       siftdtc.detect(img2,kp2);
       Mat outimg2;
       drawKeypoints(img2,kp2,outimg2);
       imshow("image2 keypoints",outimg2);


       SiftDescriptorExtractor extractor;
       Mat descriptor1,descriptor2;
       BruteForceMatcher<L2<float>> matcher;
       vector<DMatch> matches;
       Mat img_matches;
       extractor.compute(img,kp1,descriptor1);
       extractor.compute(img2,kp2,descriptor2);


       imshow("desc",descriptor1);
       std::cout<<endl<<descriptor1<<std::endl;
       matcher.match(descriptor1,descriptor2,matches);

       drawMatches(img,kp1,img2,kp2,matches,img_matches);
       imshow("matches",img_matches);
}
int matchFlag;
void MainWindow::timer4()
{
    static int i=0,bestNum=10000,tFileNum=10000,n300=0;

    int i2;
    if(i==list1.size())
    {
        n300=0;
        qDebug()<<"bestNum:"<<bestNum;
        i=0;
        bestNum=10000;
        t4->stop();
        qDebug()<<"模板最高匹配图";
        for(int k=0;k<tImageList.size();k++)
        {
            qDebug()<<TemplateName[k].fileName()<<tNum[k];
        }
        return;
    }

    CvRect r1=cvRect(648,659,132,75);
    //CvRect r1=cvRect(648,667,132,50);
    long int result=-1;
    int bestMatch=1000000;

//    CvPoint  minloc, maxloc;
//    double   minval, maxval;
    IplImage *pSrcImage = cvLoadImage(list1[i].toLocal8Bit().data(), 0);
//qDebug()<<list1[i];
    for(int j=0;j<tImageList.size();j++)
    {
//        cvSetImageROI(pSrcImage,r1);
  //      IplImage *outImage = cvCreateImage( cvSize(r1.width-tImageList[j]->width+1,r1.height-tImageList[j]->height+1 ), IPL_DEPTH_32F , 1 );

//        cvMatchTemplate(tImageList[j],pSrcImage,outImage,CV_TM_SQDIFF );
//        cvResetImageROI(pSrcImage);

  //      cvMinMaxLoc( outImage, &minval, &maxval, &minloc, &maxloc, 0 );
//        //cvRectangle( pSrcImage, minloc, cvPoint( minloc.x + tImageList[j]->width , minloc.y + tImageList[j]->height ), cvScalar(0,0,0), 2, 8, 0 );
//        result= long int(minval/10000);
// cvReleaseImage(&outImage);


        result= zt_matchTemplate(pSrcImage,tImageList[j],r1,1);

        if(result<bestMatch)
        {
            bestMatch=result;
            i2=j;
        }

        if(bestMatch<bestNum)
        {
            bestNum=bestMatch;
            tFileNum=j;

        }

    }
    cvReleaseImage(&pSrcImage);

    tNum[i2]++;

    if(bestMatch<=300&&matchFlag==1)
    {
        ++n300;
        //qDebug()<<"copy state:"<< QFile::copy(list1[i],"image/bbb/"+list1[i].right(19));
        //QFile::remove(list1[i]);
        ui->label_2->setText(TemplateName[i2].fileName());
        qDebug()<<"template name"<<TemplateName[i2].fileName();
        qDebug()<<"fileName:"<<list1[i]<<"bestMatch"<<bestMatch<<"<=300"<<n300;
    }else if(bestMatch>300&&matchFlag==0)
    {
        ++n300;
        qDebug()<<"copy state:"<< QFile::copy(list1[i],"image/bbb/"+list1[i].right(19));
        //QFile::remove(list1[i]);
        qDebug()<<"template name"<<TemplateName[i2].fileName();
        qDebug()<<"copy:"<<list1[i]<<"bestMatch"<<bestMatch<<"<=300"<<n300;
    }else if(bestMatch<=300&&matchFlag==2)//分离图片
    {

        ++n300;
        qDebug()<<"copy state:"<< QFile::copy(list1[i],"image/"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"/"+list1[i].right(19));
        QFile::remove(list1[i]);
        qDebug()<<"template name"<<TemplateName[i2].fileName();
        qDebug()<<"copy:"<<list1[i]<<"bestMatch"<<bestMatch<<"<=300"<<n300;
    }

    ++i;
    ui->label->setText(QString::number(i));

}

void MainWindow::on_pushButton_40_clicked()
{
    matchFlag=0;
    ui->label->setText("0");
    QDir dir2("temp/");
    dir2.setFilter(QDir::Files);
    TemplateName= dir2.entryInfoList();
    tImageList.clear();
    list1.clear();


    for(int i=0;i<TemplateName.size();i++)
    {
        tNum[i]=0;
        QString path="temp/"+TemplateName[i].fileName();
        qDebug()<<"i:"<<i<<TemplateName[i].fileName();

        tImageList.append(cvLoadImage(path.toLocal8Bit().data(),0));  
    }

    qDebug()<<"模板数:"<<tImageList.size();
    QString path=ui->lineEdit->text();
    path.replace(QString("\\"),QString("/"));
    if(path[path.length()-1]!='/')
        path.append('/');


    QDir dir(path);
    dir.setFilter(QDir::Files);
    QFileInfoList list= dir.entryInfoList();

   for(int i=0;i<list.size();i++)
   {
       list1<<path+list[i].fileName();
   }

    t4=new QTimer(this);
    connect(t4,SIGNAL(timeout()),this,SLOT(timer4()));
    t4->start(20);
}

void MainWindow::on_pushButton_41_clicked()
{
    IplImage* image=cvCreateImage(cvSize(800,600),IPL_DEPTH_8U,1);
    IplImage* img2=cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
    CvPoint PointArray1[4];
    PointArray1[0]=cvPoint(100,100);
    PointArray1[1]=cvPoint(400,150);
    PointArray1[2]=cvPoint(320,300);

    PointArray1[3]=cvPoint(20,150);



    CvScalar Color=CV_RGB(255,255,255);//CV_RGB(0,0,0);
    cvZero(image);
    cvFillConvexPoly(image,PointArray1,4,Color);//4条边
    cvCopy(image,img2);

    CvSeq *seq=NULL;
    CvMemStorage *storage=cvCreateMemStorage();

    cvFindContours(image,storage,&seq);


    for(int i=0;seq;seq=seq->h_next,i++)
    {

    CvRect rect= cvBoundingRect(seq,1);
    cvRectangle(img2,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),Color,3);
    cvShowImage(QString::number(i).toLocal8Bit().data(),img2);

    }
    cvReleaseImage(&image);
    cvReleaseImage(&img2);
    cvReleaseMemStorage(&storage);
}

void drawDetectLines(Mat& image,const vector<Vec4i>& lines,Scalar & color)
{
    // 将检测到的直线在图上画出来
    vector<Vec4i>::const_iterator it=lines.begin();
    while(it!=lines.end())
    {
        Point pt1((*it)[0],(*it)[1]);
        Point pt2((*it)[2],(*it)[3]);
        line(image,pt1,pt2,color,2); //  线条宽度设置为2
        ++it;
    }
}
QList<Point> listDot;

void on_cvCanny2(int,void*)
{
   Canny(srcImage, midImage, g_threshold, g_threshold*3, 3);
   g_srcImage1=srcImage.clone();
   cvtColor(srcImage,g_grayImage, CV_BGR2GRAY);
   cvtColor(midImage,dstImage2, CV_GRAY2BGR);
   vector<Vec4i> linesP;

   //HoughLinesP(midImage, linesP, 1, CV_PI/180, 80, 50,10 );
   HoughLinesP(midImage, linesP, 1, CV_PI/180, 80, 100,30 );//最后一个参数同一条直线最大间隙如：_____ _____
   if(linesP.size()>34)
       return;
   int leftX,leftY,lineLen,dotVal,lineLen1,xLen,yLen,avgUp,avgMid,avgDown,avgTmp1,avgTmp2,blackSideX,blackSideY;
   float ratio;


//依次在图中绘制出每条线段

   for( size_t i = 0; i < linesP.size(); i++ )
   {

       int sign1=-1;
       listDot.clear();
       Vec4i l = linesP[i];
       //过于斜的线
       if(abs(l[0]-l[2])<70)
           continue;
       leftX=l[0]<l[2]?l[0]:l[2];
       leftY=leftX==l[0]?l[1]:l[3];
       if(abs(srcImage.cols/2-(abs(l[0]-l[2])/2+leftX))>80)
           continue;
       lineLen=sqrt((l[2]-l[0])*(l[2]-l[0]*1.0)+(l[1]-l[3])*(l[1]-l[3]));
       dotVal=20;//取20个点
       avgUp=avgMid=avgDown=0;
       lineLen1=lineLen/dotVal;

       for(int n=1;n<=dotVal;n++)
       {
           ratio=n*lineLen1*1.0/lineLen;
           xLen=abs(l[0]-l[2])*ratio;
           yLen=abs(l[1]-l[3])*ratio;
           blackSideX=leftX+xLen;
           blackSideY=leftY+yLen;
           //边界超出
           if(blackSideX>=g_grayImage.cols||blackSideY>=g_grayImage.rows)
               continue;

           listDot.insert(0,Point(blackSideX,blackSideY));

           avgTmp1=g_grayImage.at<uchar>(blackSideY-2,blackSideX);
           avgTmp2=g_grayImage.at<uchar>(blackSideY+2,blackSideX);
           avgMid+=avgTmp1;
           avgDown+=avgTmp2;
         //  qDebug()<<"灰度值 "<<avgTmp1<<avgTmp2;

       }

       avgMid/=dotVal;
       avgDown/=dotVal;
        //过滤小于2个像素黑线

       if(abs(avgDown-avgMid)<70)
           continue;
//avgDown为地槛 avgMid为黑缝
       if(avgDown<avgMid)
       {
           sign1=1;
           avgMid^=avgDown;
           avgDown^=avgMid;
           avgMid^=avgDown;
       }
      // qDebug()<<sign1;
//取黑缝厚度,范围取中间部分
       int tmpY,tmpDiff,tmpWid,metalBright,num;
       num=0;
       tmpWid=0;
       metalBright=0;
       for(int n=listDot.size()/4;n<=listDot.size()-listDot.size()/4;n++)
       {
           for(int j=1;j<10;j++)
           {
               tmpY=listDot[n].y+sign1*j;
               tmpDiff=abs(g_grayImage.at<uchar>(tmpY,listDot[n].x)-avgDown);
               //qDebug()<<listDot[n].x<<listDot[n].y<<avgMid<<tmpY<<g_grayImage.at<uchar>(tmpY,listDot[n].x)<<avgDown;
               if(tmpDiff<20)
               {

                   tmpWid+=j;
                   ++num;

                   break;
               }
           }

       }

       //确定一条中间黑，两边亮度差不多的线
       if(num<6)
           continue;
       tmpWid/=num;
//end
       qDebug()<<avgDown<<avgMid;
      // qDebug()<<"tmpWid"<<tmpWid<<"num:"<<num<<"sign1:"<<sign1;
        //line( g_srcImage1, Point(l[0], l[1]+sign1*tmpWid), Point(l[2], l[3]+sign1*tmpWid), Scalar(255,255,0), 1, CV_AA);
       for(int i=0;i<listDot.size();i++)
       {
           circle( g_srcImage1, Point(listDot[i].x,listDot[i].y ), 3, Scalar(0,0,255), 1, CV_AA);
       }

       line( g_srcImage1, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 1, CV_AA);
   }
   imshow("【效果图】", dstImage2);
   //imshow("aaaa", g_grayImage);
   imshow("18_1", g_srcImage1);

}
void MainWindow::on_pushButton_42_clicked()
{
    g_threshold=85;//237;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    srcImage = imread(s1);
    namedWindow("18_1", 0 );
    resizeWindow("18_1",1000,800);
    createTrackbar("0", "18_1",&g_threshold, 400,on_cvCanny2);
    on_cvCanny2(0,0);

}
int preX1,preY1;
void MainWindow::pixelVal(int x,int y,bool calcLen,int val)
{

    ui->label_2->setText(QString("(%1,%2) %3").arg(x).arg(y).arg(val));
    if(calcLen)
        ui->label_3->setText(QString("Len:%1  angle:%2").arg(common::getLineLen(Point(preX1,preY1),Point(x,y))).arg(common::calcAngle(Point(preX1,preY1),Point(x,y))));
}
void MainWindow::pixelVal(int x,int y,bool calcLen,int val1,int val2,int val3)
{

    ui->label_3->setText(QString("(%1,%2) %3 %4 %5").arg(x).arg(y).arg(val1).arg(val2).arg(val3));

}

void MainWindow::showResult(QString info)
{
    ui->label_3->setText(info);
}

bool MainWindow::getCheckBox1State()
{
    return ui->checkBox1->isChecked();
}


void onMouse(int event, int x, int y,int, void* handle)
{

    static int t=0;
    MainWindow *mainWindow=(MainWindow*)handle;
    switch (event)
    {
        case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度
        //qDebug()<<QString("(%1,%2) bright:%3").arg(x).arg(y).arg(g_dstImage5.at<uchar>(y,x));



        if(mainWindow->getCheckBox1State())
        {
            qDebug()<<g_dstImage5.at<uchar>(y,x);
            mainWindow->pixelVal(x,y,false,g_dstImage5.at<uchar>(y,x));

            if(++t>1)
            {
                mainWindow->pixelVal(x,y,true,g_dstImage5.at<uchar>(y,x));
            }
        }
        else
        {
            mainWindow->pixelVal(x,y,false,g_dstImage5.at<Vec3b>(y,x)[0],g_dstImage5.at<Vec3b>(y,x)[1],g_dstImage5.at<Vec3b>(y,x)[2]);

        }

        mousePosX=x;
        mousePosY=y;
        preX1=x;
        preY1=y;
        break;
        case CV_EVENT_RBUTTONDOWN:

        break;


    }
}
void MainWindow::on_pushButton_43_clicked()
{
    btnNext=ui->pushButton_43;
    string s1=ui->comboBox->currentText().toLocal8Bit();

    if(ui->checkBox1->isChecked())
        g_dstImage5 = imread(s1,0);
    else
        g_dstImage5 = imread(s1,1);
    namedWindow("on_pushButton_43_clicked",CV_WINDOW_NORMAL);
    resizeWindow("on_pushButton_43_clicked",1280,960);
    cv::setMouseCallback("on_pushButton_43_clicked",onMouse,this);

    imshow("on_pushButton_43_clicked",g_dstImage5);


}
float get_angle;

void MainWindow::on_pushButton_44_clicked()
{
    btnNext=ui->pushButton_44;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    IplImage *img=cvLoadImage(s1.data(),0);
    IplImage *rgb=cvLoadImage(s1.data());

    Point p1(0,0),p2(0,0);
    pro1->find2Circle(img,rgb,p1,p2);

    cvCircle(rgb,cvPoint(p1.x,p1.y),7,cvScalar(255,254,65),1);
    cvCircle(rgb,cvPoint(p2.x,p2.y),7,cvScalar(255,254,65),1);
    cvShowImage("img",rgb);
    cvReleaseImage(&img);
    cvReleaseImage(&rgb);


}
bool isSucess,isLoop;

void MainWindow::on_pushButton_45_clicked()
{
    btnNext=ui->pushButton_45;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    IplImage *img=cvLoadImage(s1.data(),0);
    IplImage *rgb=cvLoadImage(s1.data());
       bool NG=true;

       int offX=1000,offY=1000;

       pro1->calcMeter2(img,rgb,NG,offX,offY);
       qDebug()<<"NG"<<NG<<"offX Y"<<offX<<offY;
       cvReleaseImage(&img);
       cvReleaseImage(&rgb);

}

void MainWindow::on_pushButton_46_clicked()
{

    DEBUG=true;
    string s1=ui->comboBox->currentText().toLocal8Bit();
//    Mat img_1 = imread(s1, 0);
//    Mat img=img_1(Rect(840,832,668,607));

     g_dstImage1=imread(s1,0);
     g_dstImage2=imread(s1,1);

    xy_size_1 xy_arr[50];
    int otherNum=0;

    //雪龙6种圆环尺寸 76  86  125  127  164  180

    //三维视觉参数
    //170,360,4519,400,717,20,530,372,246,596,1193,959,1259,953,190,160,90,0.16014,0.14,-0.95788,0,0
    params param;
    param.var1[0]=140;//小圆孔size
    param.var1[1]=360;//大圆孔size
    param.var1[2]=4519;//最大圆size
    param.var1[3]=539;//最小内圆环半径
    param.var1[4]=717;//最大内圆环半径
    param.var1[5]=20;//实际圆孔间距和测量圆孔间距差值,越小越匹配度越低
    param.var1[6]=600;//圆孔离圆心距离(所有风扇小孔的平均值)
    param.var1[7]=372;//最大圆孔间距
    param.var1[8]=246;//小圆间距
    param.var1[9]=596;//只有6个小圆孔间距
    param.var1[10]=1120;//机器人基准点X;
    param.var1[11]=743;//机器人基准点Y;
    param.var1[12]=1120;//圆心X
    param.var1[13]=743;//圆心Y
    param.var1[14]=130;//毛刺灰度值
    param.var1[15]=130;//二值参数
    param.var1[16]=90;//圆孔离圆心距离与cen_distance1差值范围
    param.var2[0]=0.19349;//像素比
    param.var2[1]=0.14;//圆外接矩形长宽比
    param.var2[2]=-1;//坐标角度偏差
    param.var2[3]=0;//X偏移毫米
    param.var2[4]=0;//Y偏移毫米

    int num=0,num2=0;
    QTime t;
    t.start();
    //Rect roi(1000,750,1000,1000);
    Rect roi(0,0,2500,1900);

    pro1->xl_circle3(g_dstImage2,g_dstImage1,roi,num,num2,xy_arr,otherNum,param);

    qDebug("Time elapsed: %d ms", t.elapsed());
    QString str;

    qDebug()<<"数量:"<<num<<"NG"<<num2<<"污点数量"<<otherNum;
    for(int i=0;i<num2;i++)
    {
        qDebug()<<"x:"<<xy_arr[i].x<<"y:"<<xy_arr[i].y<<"size:"<<xy_arr[i].size;
        str+=QString("%1,%2|").arg(QString::number(xy_arr[i].moveY*-1,'f',3)).arg(QString::number(xy_arr[i].moveX*-1,'f',3));

    }

    str.remove(str.length()-1,1);
    qDebug()<<"move:"<<str;
    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    imshow("rgb",g_dstImage2);
}



void MainWindow::on_pushButton_47_clicked()
{

    btnNext=ui->pushButton_47;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //656,655,35,140,179,100,0
     params param;

//C线参数
     param.var1[0]=656;//中心点X
     param.var1[1]=655;//中心点Y
     param.var1[2]=35;//检测字体最小值
     param.var1[3]=140;//检测字体最大值
     param.var1[4]=169;//字体到中心点距离
     param.var1[5]=100;//二值
     param.var2[0]=0;//角度偏移
     Rect roi(313,332,1001-313,916-332);


     param.var1[28]=100000;//得到首次检测到NI坐标
     param.var1[29]=100000;
     param.var2[19]=360;//角度



     QTime t;
     t.start();

     if(!ui->checkBox_dll->isChecked())
     {
         pro1->zt_3rdLine(g_dstImage1,roi,param,false);

         if(param.var2[19]==360)
         {
             //qDebug()<<"第二次计算";
            // pro1->zt_3rdLine(g_dstImage1,roi,param,true);
         }
     }
     else
     {
     //call dll
        detectWork(g_dstImage1,roi,param,1);
        namedWindow("src",0);
        resizeWindow("src",800,600);
        imshow("src",g_dstImage1);
     //end
     }


     qDebug()<<"fileName"<<ui->comboBox->currentText().right(22)<<"x1 y1"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19]<<"特征数量"<<param.var1[26]<<"i:"<<param.var1[27];
    qDebug("Time elapsed: %d ms", t.elapsed());
    ui->label_3->setText(QString("method:%1 angle:%2").arg(param.var1[25]).arg(param.var2[19]));
}

void MainWindow::on_pushButton_48_clicked()
{

    btnNext=ui->pushButton_48;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    params param;
    //586,561,90,130,290,400,152,140,170,0

    //中间长方形工件
    //575,498,130,190,450,800,152,235,260,0

    //524,514,88,128,350,500,152,177,200,0

    //D线新产品  530,510,130,170,350,500,152,190,210,0
    param.var1[0]=530;//725;//740;//圆心X
    param.var1[1]=510;//圆心Y
    param.var1[2]=130;//圆心与字最小距离
    param.var1[3]=170;//圆心与字最大距离
    param.var1[4]=350;//大半圆size
    param.var1[5]=500;//大半圆size
    param.var1[6]=152;//152;//二值
    param.var1[7]=190;//外接矩形长
    param.var1[8]=210;//外接矩形长
    param.var2[0]=0;//角度偏差


    //Rect r1(0,0,1000,800);
    Rect r1(0,0,1280,960);
    QTime t;
    t.start();
    if(!ui->checkBox_dll->isChecked())
        pro1->zt_2ndLine(g_dstImage1,r1,param);
    else
        detectWork(g_dstImage1,r1,param,17);


    common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[2],param.var2[19],90,Scalar(0,0,255));


     imshow("show",g_dstImage2);
     qDebug()<<"angle:"<<param.var2[19]<<"耗时:"<<t.elapsed();

     ui->label_3->setText(QString("angle:%1").arg(param.var2[19]));
}

void MainWindow::on_pushButton_49_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
       g_dstImage1=imread(s1,0);

       bool NG=true;
       Rect rect(185,258,70,70);
       params param;
       param.var1[0]=30;//圆孔最小size
       param.var1[1]=150;//圆孔最大size
       param.var1[2]=6;//长宽像素差
       param.var1[3]=50;//圆孔与圆环亮度差
       param.var1[4]=30;//二值
       param.var1[5]=13;//圆心至圆环半径
       param.var1[6]=224;//标定中心点X
       param.var1[7]=288;//标定中心点Y
       param.var1[8]=4;//最大偏移像素
       int offX,offY;
       pro1->calcMeter(g_dstImage1,rect,NG,offX,offY,param);
       qDebug()<<"NG"<<NG<<"offX Y"<<offX<<offY;
}

void MainWindow::on_pushButton_50_clicked()
{
    btnNext=ui->pushButton_50;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

     namedWindow( "Contours", 0 );
     resizeWindow("Contours",800,600);
     namedWindow( "Threshold", 0 );
     resizeWindow("Threshold",800,600);

     float angle=360;
     Point cen;
     QTime t;
     t.start();
     int ret=pro1->zt_1stLine(g_dstImage1,cen,angle);
     if(ret==0)
     {
         qDebug()<<"angle:"<<angle<<"cen x y:"<<cen.x<<cen.y<<"time elapsed"<<t.elapsed()<<"ms";

     }
     else
     {
         qDebug()<<"无法计算";
     }
}

void MainWindow::on_pushButton_51_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);

    params param;
    double angle=360;
    param.var1[0]=694;//圆心X
    param.var1[1]=555;//圆心Y
    param.var1[2]=87;//圆心与字距离
    param.var1[3]=70;//小半圆size
    param.var1[4]=205;//大半圆size
    param.var1[5]=254;//二值
    param.var1[6]=25;//计算NI step值
    param.var1[7]=102;//外接矩形长
    param.var1[8]=132;//大半圆矩形长度
    param.var2[0]=0;//角度偏差
    //Rect r1(0,0,1000,800);
    Rect r1(432,288,1075-432,864-288);
    QTime t;
    t.start();
    pro1->zt_calcWork_6(g_dstImage1,r1,param,angle);

    qDebug()<<"angle:"<<angle<<"耗时:"<<t.elapsed();
}

void MainWindow::on_pushButton_52_clicked()
{
    btnNext=ui->pushButton_52;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat src = imread(s1,0);
    Mat rgb = imread(s1,1);
    //namedWindow("18_1", 0 );
    //resizeWindow("18_1",1000,800);

    wk->edvia(src,rgb);
}

void MainWindow::on_pushButton_53_clicked()
{
    btnNext=ui->pushButton_53;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    IplImage *img=cvLoadImage(s1.data(),0);
    IplImage *rgb=cvLoadImage(s1.data());
       bool NG=true;

       int offX=1000,offY=1000;

       pro1->calcMeter3(img,rgb,NG,offX,offY);
       qDebug()<<"NG"<<NG<<"offX Y"<<offX<<offY;
       cvReleaseImage(&img);
       cvReleaseImage(&rgb);

}


void MainWindow::on_pushButton_54_clicked()
{

    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    namedWindow( "Contours", 0 );
    resizeWindow("Contours",1200,800);
    pro2->hd_vision1();
}

void MainWindow::on_pushButton_56_clicked()
{
    btnNext=ui->pushButton_56;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    namedWindow( "Contours", 0 );
    resizeWindow("Contours",1200,800);
    pro2->hd_vision2();

}

void MainWindow::on_pushButton_57_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    namedWindow( "Contours", 0 );
    resizeWindow("Contours",1200,800);
    pro2->hd_vision3();
}

void MainWindow::on_pushButton_55_clicked()
{
    btnNext=ui->pushButton_55;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    params param;
    //702,508,267,320,200,2000,0,4
    param.var1[0]=702;//圆心
    param.var1[1]=508;
    param.var1[2]=267;//半径
    param.var1[3]=320;
    param.var1[4]=200;//小圆size范围最小
    param.var1[5]=2000;//小圆size范围最大
    param.var1[6]=0;//与圆心偏离
    param.var1[7]=4;//特征数量

    Rect roi(0,0,1280,960);
    QTime t;
    t.start();
    if(!ui->checkBox_dll->isChecked())
    {
        pro2->hd_smallWork_other(g_dstImage1,roi,param);
        if(param.var1[26]==82000)
            pro2->hd_smallWork(g_dstImage1,roi,param);
    }
    else
        detectWork(g_dstImage1,roi,param,18);
    if(param.var1[29]==82000)
    {
        ui->label_3->setText("反面");
        if(DEBUG)
            circle(g_dstImage2,Point(param.var1[27],param.var1[28]),5,Scalar(255,0,0),3);

    }
    else
    {
        ui->label_3->setText("正面");
    }
    qDebug()<<"aaaa"<<param.var1[29];
    if(DEBUG)
        imshow("rgb",g_dstImage2);
    paramRet=param.var1[29];
    qDebug("Time elapsed: %d ms", t.elapsed());
}

void MainWindow::on_pushButton_58_clicked()
{
    btnNext=ui->pushButton_58;
        string s1=ui->comboBox->currentText().toLocal8Bit();
        g_dstImage1=imread(s1,0);
        g_dstImage2=imread(s1,1);



    //550,505,70,500,108,168,60,90,0
    //592,529,70,500,108,168,60,105,0
         params param;
         param.var1[0]=592;//550;//中心点X
         param.var1[1]=529;//510;//中心点Y
         param.var1[2]=70;//检测字体最小值
         param.var1[3]=500;//检测字体最大值
         param.var1[4]=108;//字体到中心点距离
         param.var1[5]=168;
         param.var1[6]=60;//二值
         param.var1[7]=105;//90;//画直线长度
         param.var2[0]=0;//角度偏移

         param.var1[28]=10000;//得到首次检测到NI坐标
         param.var1[29]=10000;
         param.var2[19]=360;//角度



         Rect roi(270,250,550,550);
         QTime t;
         t.start();



        bool callDll=ui->checkBox_dll->isChecked();

        if(callDll)
        {
            detectWork(g_dstImage1,roi,param,6);
        }
        else
        {
            pro2->zt_ZT01_2(g_dstImage1,g_dstImage2,roi,param);
            if(param.var2[19]==360)
            {
                params param2;
                param2.var1[0]=param.var1[0];
                param2.var1[1]=param.var1[1];
                param2.var1[2]=40;// param.var1[2];//检测字体最小值
                param2.var1[3]=200;//param.var1[3];//检测字体最大值
                param2.var1[4]=param.var1[4];//字体到中心点距离
                param2.var1[5]=param.var1[4]+40;
                param2.var1[6]=230;//二值
                param2.var1[7]=15;//三角形外接矩形len
                param2.var1[8]=40;
                param2.var1[9]=15;//三角形最小边长

                param2.var2[0]=common::anlgeOffset(param.var2[0],180);//角度偏移

                qDebug()<<"zt_6thLine calc"<< param2.var2[0];
                pro3->zt_6thLine(g_dstImage1,g_dstImage2,roi,param2);

                param.var2[19]=param2.var2[19];

                param.var1[28]= param2.var1[28];
                param.var1[29]= param2.var1[29];
//                if(param.var2[19]==360)
//                {
//                    pro3->zt_zt01Match(g_dstImage1,g_dstImage2,roi,param2);
//                    param.var2[19]=param2.var2[19];
//                    param.var1[28]= param2.var1[28];
//                    param.var1[29]= param2.var1[29];
//                }

            }

        }
        common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],90,Scalar (255,0,0));


         namedWindow("rgb",0);
         resizeWindow("rgb",800,600);
         imshow("rgb",g_dstImage2);

         qDebug("Time elapsed: %d ms", t.elapsed());
         qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];

         ui->label_3->setText(QString("angle:%1 ").arg(param.var2[19]));
}


//始终保存１万张检测图片
void savedDetectImg(Mat &src,QString folderName,QString fileName)
{
    QDir dir("D:/"+folderName);
    dir.mkdir("D:/"+folderName);
    dir.setFilter(QDir::Files);
    qDebug()<<folderName<<dir.entryInfoList().size();

    QFileInfoList list2= dir.entryInfoList();
    for(int i=0;i<list2.count();i++)
    {
        qDebug()<<"filename"<<list2[i].fileName();
    }

    if(dir.entryInfoList().size()>10)
    {


        if(dir.exists("D:/"+folderName+"_backup"))
        {
            qDebug()<<"exists _backup";
            system(QString("rd/s/q d:\\%1_backup").arg(folderName).toLocal8Bit().data());
        }
        dir.rename("D:/"+folderName,"D:/"+folderName+"_backup");
        dir.mkdir("D:/"+folderName);
    }
    //int ret =imwrite("E:/abc.jpg",src);

   int ret= imwrite(QString("D:/%1/%2.jpg").arg(folderName).arg(fileName).toStdString(),src);
   qDebug()<<"ret"<<ret<<QString("D:/%1/%2").arg(folderName).arg(fileName);
}

void MainWindow::timer5()
{

    params param;
    Rect roi;
    //E线
    if(QC_flag=='E')
    {

        roi.x=249;
        roi.y=251;
        roi.width=849-249;
        roi.height=876-251;
        param.var1[0]=534;//中心点X
        param.var1[1]=590;//中心点Y
        param.var1[2]=35;//检测字体最小值
        param.var1[3]=140;//检测字体最大值
        param.var1[4]=179;//字体到中心点距离
        param.var1[5]=100;//二值
        param.var2[0]=0;//角度偏移

    }
    else
    {

    //C线

    roi.x=313;
    roi.y=332;
    roi.width=1001-313;
    roi.height=916-332;
    param.var1[0]=656;//中心点X
    param.var1[1]=655;//中心点Y
    param.var1[2]=35;//检测字体最小值
    param.var1[3]=140;//检测字体最大值
    param.var1[4]=169;//字体到中心点距离
    param.var1[5]=100;//二值
    param.var2[0]=0;//角度偏移

    }





    static int i=0,i3=0;
    if(i==fileList.size())
    {
       i=0;
       i3=0;
       t5->stop();
       return;
    }

    ui->label->setText(QString::number(i));
    param.var1[28]=100000;//得到首次检测到NI坐标
    param.var1[29]=100000;
    param.var2[19]=360;//角度



    string strPic=(loadPath+fileList[i]).toLocal8Bit();
    Mat pic=imread(strPic,0);

    QTime t;
    t.start();
    if(!ui->checkBox_dll->isChecked())
    {
        pro1->zt_3rdLine(pic,roi,param,false);
        if(param.var2[19]==360)
        {

            //pro1->zt_3rdLine(pic,roi,param,true);

        }
    }
    else
    {
        //call dll
        detectWork(pic,roi,param,1);

    }



        if(param.var2[19]==360)
        {

            ui->label_2->setText(QString::number(++i3));

        }

    qDebug("Time elapsed: %d ms", t.elapsed());
    // qDebug()<<"x1 y1"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19]<<"特征数量"<<param.var1[26]<<"i:"<<param.var1[27];

    QString s=fileList[i];
    s.remove(0,16);
    s.split('.');
    double angle=s.split('.').at(0).toDouble();
    if(abs(abs(angle)-abs(param.var2[19]))>50)//&&param.var2[19]!=360)
    {

        bool b= QFile::copy(loadPath+fileList[i],"image/"+fileList[i]);
        qDebug()<<"angle"<<abs(angle)<<param.var2[19]<<"copy state"<<b<<fileList[i];


        //QFile::remove(loadPath+fileList[i]);

        //rename

        if(param.var2[19]!=360)
        {
//            QString newName=fileList[i].left(16)+QString("%1.png").arg(param.var2[19]);
//            QFile::rename("image/"+fileList[i],"image/"+newName);
//            qDebug()<<"newName"<<newName;

        }

    }

    ++i;
}




void MainWindow::on_pushButton_59_clicked()
{
    QC_flag='C';
//    string s1=ui->comboBox->currentText().toLocal8Bit();
//    g_dstImage1=imread(s1,0);
//    g_dstImage2=imread(s1,1);
    DEBUG=false;
    t5=new QTimer(this);
    connect(t5,SIGNAL(timeout()),this,SLOT(timer5()));
    t5->start(50);

}

void MainWindow::on_pushButton_60_clicked()
{
    if(ui->pushButton_60->text()=="-10")
    {
        ui->pushButton_60->setText("10");
        offsetSign=10;
    }
    else
    {
        ui->pushButton_60->setText("-10");
        offsetSign=-10;
    }
}

void MainWindow::timer6()
{
    params param;
    //740,560,103,220,380,170,25,0,102,132

    param.var1[0]=740;//圆心X
    param.var1[1]=556;//圆心Y
    param.var1[2]=110;//圆心与字距离
    param.var1[3]=280;//大半圆size
    param.var1[4]=390;//大半圆size
    param.var1[5]=170;//152;//二值
    param.var1[6]=25;//计算NI step值
    param.var2[0]=0;//角度偏差
    param.var1[7]=132;//外接矩形长
    param.var1[8]=152;//外接矩形长
    param.var1[9]=30;//圆心与字距离最大偏差
    //Rect r1(0,0,1000,800);
    Rect r1(432,288,1075-432,864-288);



    static int i=0,i3=0;
    remainingTime(i,fileList.size());
    if(i==fileList.size())
    {
       i=0;
       i3=0;
       t6->stop();
       return;
    }

    ui->label->setText(QString::number(i));


    Mat pic=imread((loadPath+fileList[i]).toStdString(),0);

    QTime t;
    t.start();

    if(!ui->checkBox_dll->isChecked())
        pro1->zt_2ndLine(pic,r1,param);
    else
        detectWork(pic,r1,param,17);



     qDebug()<<"angle:"<<param.var2[19]<<"耗时:"<<t.elapsed();

    QString s=fileList[i];
    s.remove(0,16);
    s.split('.');
    double angle=s.split('.').at(0).toDouble();
    if(param.var2[19]==360)
    {
        ui->label_2->setText(QString::number(++i3));
    }
    if(abs(abs(angle)-abs(param.var2[19]))>50)
    {

       bool b= QFile::copy(loadPath+fileList[i],"image/"+fileList[i]);
       qDebug()<<"copy state"<<b<<fileList[i];
    }

    ++i;
}

void MainWindow::timer7()
{

    pro2->armPic();

}



void MainWindow::on_pushButton_61_clicked()
{
    DEBUG=false;
    t6=new QTimer(this);
    connect(t6,SIGNAL(timeout()),this,SLOT(timer6()));
    t6->start(50);
}

void MainWindow::on_pushButton_62_clicked()
{
    if(ui->comboBox->count()==0)
        return;
    if(btnNext==NULL)
        return;

    index=ui->comboBox->currentIndex();
    if(index>=ui->comboBox->count()-1)
    {
        QMessageBox::about(this,"提示","最后一张");
        ui->comboBox->setCurrentIndex(0);
        ui->pushButton_62->setStyleSheet("background-color:lightblue;");
        return;
    }
    else
    {
        ui->pushButton_62->setStyleSheet("");
    }
    ui->comboBox->setCurrentIndex(index+1);
    btnNext->click();
}

void MainWindow::on_pushButton_63_clicked()
{
    CvRect r1=cvRect(639,669,130,55);
    //CvRect r1=cvRect(648,667,132,50);


    long int result=-1;
    int i=0;
    int bestMatch=1000000;
    CvPoint  minloc, maxloc;
    double   minval, maxval;
    IplImage *pSrcImage = cvLoadImage(ui->comboBox->currentText().toLocal8Bit().data(), 0);

    IplImage *tImage=cvLoadImage("temp/78.png",0);

    int method=5;
//    for(int j=0;j<tImageList.size();j++)
//    {


        cvSetImageROI(pSrcImage,r1);
        IplImage *outImage = cvCreateImage( cvSize(r1.width-tImage->width+1,r1.height-tImage->height+1 ), IPL_DEPTH_32F , 1 );
//        CV_TM_SQDIFF        =0,
//        CV_TM_SQDIFF_NORMED =1,
//        CV_TM_CCORR         =2,
//        CV_TM_CCORR_NORMED  =3,
//        CV_TM_CCOEFF        =4,
//        CV_TM_CCOEFF_NORMED =5
        cvMatchTemplate(pSrcImage,tImage,outImage,method );
        //cvResetImageROI(pSrcImage);

        cvMinMaxLoc( outImage, &minval, &maxval, &minloc, &maxloc, 0 );
        cvRectangle( pSrcImage, minloc, cvPoint( minloc.x + tImage->width , minloc.y + tImage->height ), cvScalar(255,255,255), 2, 8, 0 );
        result= long int(minval/10000);

        if(method==1)
        {
            minval*=10000;

        }else if(method==0)
        {
            minval/=10000;
            maxval/=10000;
        }else if(method==2)
        {

            maxval/=10000;
        }
        qDebug()<<"min max "<<minval<<maxval;
        //qDebug()<<"min max "<<minval/10000<<maxval/10000;
        //qDebug()<<"result"<<result<<"template Image:"<<tempName[j];;
        if(result<bestMatch)
        {
            bestMatch=result;
          //  i=j;

        }

         cvReleaseImage(&outImage);


  //  }



         cvNamedWindow("aaaa",0);
    //cvResizeWindow("aaaa",1280,960);
    cvShowImage("aaaa",pSrcImage);
    cvReleaseImage(&pSrcImage);

}

void MainWindow::on_pushButton_64_clicked()
{
    IplImage *pSrcImage = cvLoadImage(ui->comboBox->currentText().toLocal8Bit().data(), 0);
    pro2->test1(pSrcImage);

}

void MainWindow::on_pushButton_65_clicked()
{

    DEBUG=true;
    btnNext=ui->pushButton_65;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    namedWindow( "Contours3", 0 );
    resizeWindow("Contours3",800,600);
    QTime t;
    t.start();

    //681,474,215,80,1000,2600,260,380,405,52,77,300,333,160,600,120,120,15,250,390,30
    params param;
    param.var1[0]=681;//设定圆心坐标
    param.var1[1]=474;
    param.var1[2]=215;//圆心二值
    param.var1[3]=80;
    param.var1[4]=1000;//size
    param.var1[5]=2600;
    param.var1[6]=260;//圆心偏离x最大值
    param.var1[7]=380;//rectLen
    param.var1[8]=405;

    param.var1[9]= 52;//圆环上小圆半径范围
    param.var1[10]= 77;//圆环上小圆半径范围
    param.var1[11]= 300;//小圆环到圆心距离最小
    param.var1[12]= 333;//小圆环到圆心距离最大
    param.var1[13]= 160;//小圆size范围
    param.var1[14]= 600;
    param.var1[15]= 120;//二值最大
    param.var1[16]= 120;//所有数量
    param.var1[17]=15;//二值最小
    param.var1[18]=250;//小圆边离圆心最小距离
    param.var1[19]=390;//小圆边离圆心最大距离
    param.var1[20]=30;//污点size


    Rect roi(0,0,1280,960);
    roi.x= 196;
    roi.y= 10;
    roi.width= 1236-196;
    roi.height= 959-10;

    pro2->hd_vision1_2(g_dstImage1,roi,param);
    qDebug()<<"耗时:"<<t.elapsed()<<"fileName"<<ui->comboBox->currentText().right(22);

}
void MainWindow::traversalControl(const QObjectList& q)
{

    for(int i=0;i<q.length();i++)
    {
        if(!q.at(i)->children().empty())
        {
            traversalControl(q.at(i)->children());
        }
        else
        {

            QObject* o = q.at(i);
            if (o->inherits("QPushButton")) {
                QPushButton* b = qobject_cast<QPushButton*>(o);
                b->hide();
            }
        }
    }

}

void MainWindow::initInterface(QPushButton *btn)
{

    traversalControl(ui->centralWidget->children());

//    ui->pushButton_62->setGeometry(450,10,140,35);
//    ui->pushButton_62->show();
//    ui->comboBox->setGeometry(10,10,400,35);

//    ui->lineEdit->hide();
//    ui->label->hide();
//    ui->label_2->hide();
//    ui->checkBox->hide();
//    ui->checkBox1->hide();
//    ui->checkBox2->hide();
//    ui->checkBox_dll->hide();
 //   btn->show();

    ui->pushButton_36->show();
    ui->pushButton_77->show();
    ui->pushButton_82->show();

 //**********zt_Aline
    ui->pushButton_38->show();
    ui->pushButton_78->show();
    ui->pushButton_81->show();
    ui->pushButton_40->show();
    ui->pushButton_62->show();
    ui->pushButton_15->show();
    ui->pushButton_29->show();
//************end



    //ui->gridLayout->setSizeConstraint(ui->gridLayout->SetFixedSize);

//    ui->pushButton_124->setParent(this);
//    ui->pushButton_124->setGeometry(30,150,150,35);
//    ui->pushButton_124->show();



    this->resize(1200,700);



}
void MainWindow::on_pushButton_66_clicked()
{
    initInterface(ui->pushButton_92);

}
#include <tlhelp32.h>
IplImage *armImg;
void MainWindow::on_pushButton_67_clicked()
{

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    bool isProcess=false;
    while(bMore)
    {

       // printf(" 进程名称：%s \n", pe32.szExeFile);
      QString name= QString::fromWCharArray( pe32.szExeFile);
      if(name=="EDVIA_2.exe")
      {
          isProcess=true;
          break;
      }
        bMore = ::Process32Next(hProcessSnap, &pe32);
    }

    if(isProcess==false)
    {

        //p.start("edvia/EDVIA_2.exe");
        //WinExec("edvia/EDVIA_2.exe", SW_NORMAL);
        //WinExec("D:/debug/EDVIA_2.exe", SW_NORMAL);
    }

    armImg=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U, 3);
    t7=new QTimer(this);
    connect(t7,SIGNAL(timeout()),this,SLOT(timer7()));

    t7->start(100);

//    if(ui->pushButton_67->text()=="openArmVideo")
//    {
//        ui->pushButton_67->setText("stop");
//        pro2->sendUdp("sendVideo");
//    }
//    else
//    {
//        ui->pushButton_67->setText("openArmVideo");
//        pro2->sendUdp("stopVideo");
//    }

}

void MainWindow::on_pushButton_68_clicked()
{
    btnNext=ui->pushButton_68;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);


     //655,520,100,300,130,168,200,30,70,25,30,0
     params param;
     param.var1[0]=546;//中心点X
     param.var1[1]=503;//中心点Y
     param.var1[2]=100;//检测字体最小值
     param.var1[3]=300;//检测字体最大值
     param.var1[4]=130;//字体到中心点距离
     param.var1[5]=168;
     param.var1[6]=200;//二值
     param.var1[7]=30;//三角形外接矩形 len
     param.var1[8]=70;
     param.var1[9]=25;//三角形长宽差
     param.var1[10]=30;//三角形最小边长
     param.var2[0]=0;//角度偏移

     param.var1[28]=10000;//得到首次检测到NI坐标
     param.var1[29]=10000;
     param.var2[19]=360;//角度



     Rect roi(380,270,550,550);
     QTime t;
     t.start();
//call dll
    // detectWork(g_dstImage1,roi,param,8);
     //end


     pro2->zt_5thLine(g_dstImage1,g_dstImage2,roi,param);

     qDebug("Time elapsed: %d ms", t.elapsed());
     qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];


}

void MainWindow::on_pushButton_69_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    params param;
    //3000,9000,150,1487,1146,1490,1083,1011,0,0,1.0,1.535,0
    param.var1[0]=3000;//size
    param.var1[1]=9000;
    param.var1[2]=150;//二值
    param.var1[3]=1119;//1496;//标定模板中心
    param.var1[4]=1171;//1101;
    param.var1[5]=1148;//1492;//计算出模板中心
    param.var1[6]=927;
    param.var1[7]=1248;//计算出中心与衣领垂直长度
    param.var1[8]=0;//XY偏移
    param.var1[9]=0;
    param.var2[0]=0.4235;//像素比
    param.var2[1]=6.43466;//模板角度
    param.var2[2]=0;//2.119相机角度

    Rect roi;
    pro2->sz_cloth(g_dstImage1,g_dstImage2,roi,param);

}



void MainWindow::on_pushButton_71_clicked()
{
   pro2->getSharedMem();
}


void MainWindow::on_pushButton_72_clicked()
{
    QFile f("mainwindow.cpp");
    f.open(QIODevice::WriteOnly|QIODevice::ReadOnly);
//    f.seek(2);
//    f.write("a");
     QByteArray array=f.readAll();

    f.close();

        QByteArray array1=qCompress(array);
        QByteArray array2=qUncompress(array1);
        qDebug()<<array.size()<<array1.size()<<array2.size();
}

void MainWindow::writeLabel(QString s)
{
    ui->label->setText(s);
}

void MainWindow::on_pushButton_73_clicked()
{
    btnNext=ui->pushButton_73;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    //933,928,1190,411,70,70,643,139,415,542,200,170,260,40,80,150,700,50,190,8,160,160,0.04516,6,8,0.27
    params param;
//有料方向
    param.var1[0]=933;//坐标1
    param.var1[1]=928;
    param.var1[2]=1190;//坐标2
    param.var1[3]=411;
    param.var1[4]=70;//次数
    param.var1[5]=70;//小于灰度值

//空料方向
    param.var1[10]=643;//位置上
    param.var1[11]=139;
    param.var1[12]=415;//位置左
    param.var1[13]=542;
    param.var1[14]=200;//二值
    param.var1[15]=170;//size
    param.var1[16]=260;
    param.var1[17]=40;//最小长度
    param.var1[18]=80;

    //缺陷  150,700,50,190,8,160,160,0.04516,6,8,0.27
    param.var1[20]=150;//size
    param.var1[21]=700;
    param.var1[22]=50;//110;//外圆len
    param.var1[23]=190;
    param.var1[24]=8;//圆心偏差
    param.var1[25]=160;//130;//二值
    param.var1[26]=160;//外圆len

    param.var2[0]=0.04516;//像素比
    param.var2[1]=6;//最小直径
    param.var2[2]=8;//最大直径
    param.var2[3]=0.27;//贴边最小距离
    Rect roi(308,53,420,580);
    QTime t;
    t.start();

    //call dll
    bool callDll=false;
    //end

    if(callDll)
    {
        detectWork(g_dstImage1,roi,param,10);
    }
    else
    {
    pro2->wx_direction0(g_dstImage1,g_dstImage2,param);
    if(param.var1[29]==360) pro2->wx_direction1(g_dstImage1,g_dstImage2,roi,param);
    }

    if(param.var1[29]!=360)
    {
        if(param.var1[29]==2)
        {
            //circle( g_dstImage2, Point(843,263), 40, Scalar(193,255,3), 2, CV_AA);
            //qDebug()<<"方向右";

        }
        else if(param.var1[29]==1)
        {
            //circle( g_dstImage2, Point(640,652), 40, Scalar(193,255,3), 2, CV_AA);
            //qDebug()<<"方向下";
        }
        else if(param.var1[29]==360)
        {
            qDebug()<<"无法计算";
        }

        //缺陷检测
        if(callDll==false)
        {
        Rect roi(490,240,350,350);
        pro2->wx_defect(g_dstImage1,roi,param);

        }
        if(param.var1[27]==-1)
        {
            qDebug()<<"有缺陷"<<param.var1[28];
        }
        else if(param.var1[27]==4)
        {
            qDebug()<<"直径缺陷";
        }
        else if(param.var1[27]==5)
        {
            qDebug()<<"贴边缺陷";
        }
        else
        {
            qDebug()<<"正常";
        }
        if(param.var1[28]==1)
        {
            qDebug()<<"直径"<<param.var2[19]<<param.var2[18]<<"圆心距"<<param.var2[17];
        }


        //end缺陷检测
    }
    qDebug("Time elapsed: %d ms", t.elapsed());
    namedWindow("g_dstImage2",0);
    resizeWindow("g_dstImage2",800,600);
    imshow("g_dstImage2",g_dstImage2);

}

void MainWindow::on_pushButton_74_clicked()
{
    btnNext=ui->pushButton_74;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    params param;
    param.var1[20]=150;//size
    param.var1[21]=700;
    param.var1[22]=50;//110;//圆len
    param.var1[23]=190;
    param.var1[24]=8;//圆心偏差
    param.var1[25]=160;//130;//二值
    param.var1[26]=160;//外圆len

    param.var2[0]=0.04516;//像素比
    param.var2[1]=6;//最小直径
    param.var2[2]=8;//最大直径
    param.var2[3]=0.27;//贴边最小距离


    //Rect roi(490,240,350,350);
Rect roi(470,220,400,400);
    pro2->wx_defect(g_dstImage1,roi,param);

    if(param.var1[27]==-1)
    {
        qDebug()<<"未检测到";
    }
     if(param.var1[27]==0)
    {
        qDebug()<<"正常";
    }
    if(param.var1[28]==1)
    {
        qDebug()<<"直径"<<param.var2[19]<<param.var2[18]<<"圆心距"<<param.var2[17]<<"贴边"<<param.var2[16];
    }
    namedWindow("g_dstImage2",0);
    resizeWindow("g_dstImage2",800,600);
    imshow("g_dstImage2",g_dstImage2);

}

void MainWindow::on_pushButton_75_clicked()
{
   pro3->regex();
}

void MainWindow::on_pushButton_76_clicked()
{
    btnNext=ui->pushButton_76;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //542,502,40,150,200,250,230,10,47,10,0
    params param;
    param.var1[0]=549;//656;//中心点X
    param.var1[1]=509;//515;//中心点Y
    param.var1[2]=40;// param.var1[2];//检测字体最小值
    param.var1[3]=150;//param.var1[3];//检测字体最大值
    param.var1[4]=200;//字体到中心点距离
    param.var1[5]=250;
    param.var1[6]=230;//二值
    param.var1[7]=10;//18;//三角形外接矩形len
    param.var1[8]=47;
    param.var1[9]=10;//19;//三角形最小边长
    param.var2[0]=0;//角度偏移
    //Rect roi(510,180,320,630);
    Rect roi(0,0,1280,960);
    QTime t;
    t.start();

    namedWindow("m1",0);
    namedWindow("rgb",0);
    resizeWindow("m1",800,600);
    resizeWindow("rgb",800,600);
    pro3->zt_6thLine(g_dstImage1,g_dstImage2,roi,param);

    //call dll
       //  detectWork(g_dstImage1,roi,param,12);
         //end

    circle( g_dstImage2,Point(param.var1[28]+roi.x,param.var1[29]+roi.y), 25, Scalar(0,0,255), 1, CV_AA);
    imshow("rgb",g_dstImage2);
    qDebug()<<"angle:"<<param.var2[19];
    qDebug("Time elapsed: %d ms", t.elapsed());

}

void MainWindow::on_pushButton_77_clicked()
{
    ui->label->setText("0");
    list1.clear();
    int fileIndex=1;

    QString path=ui->lineEdit->text();
    path.replace(QString("\\"),QString("/"));
    if(path[path.length()-1]!='/')
        path.append('/');

   QDir dir(path);
   dir.setFilter(QDir::Files);
   QFileInfoList list= dir.entryInfoList();
   QTime tSec2;

   for(int i=0;i<list.size();i++)
   {

       list1<<path+list[i].fileName();
       QString time1=list[i].fileName().mid(9,6);
       QTime tSec1(time1.mid(0,2).toInt(),time1.mid(2,2).toInt(),time1.mid(4,2).toInt());

       if(abs(tSec2.secsTo(tSec1))<5)
       {

       }
       else
       {
           //*****************在正反图片中找出不相连图片***********
           if(i>0&&i%2==fileIndex)
           {

               QFile::copy(list1[i-1],"image/"+list1[i-1].right(19));
               qDebug()<<"删除图片"<<list[i-1].fileName();
               fileIndex=fileIndex?0:1;
               QFile::remove(list1[i-1]);

           }
           //end**************************
       }

       tSec2=tSec1;
   }

   list= dir.entryInfoList();
   if(list.size()%2==1)
       QFile::remove(path+list[list.size()-1].fileName());

       qDebug()<<"相连图片计算//结束并删除";

}

void MainWindow::on_pushButton_78_clicked()
{
    ui->label->setText("0");
    list1.clear();

    QString path=ui->lineEdit->text();
    path.replace(QString("\\"),QString("/"));
    if(path[path.length()-1]!='/')
        path.append('/');

   QDir dir(path);
   dir.setFilter(QDir::Files);
   QFileInfoList list= dir.entryInfoList();
   QTime tSec2;

   for(int i=0;i<list.size();i++)
   {

       list1<<path+list[i].fileName();
       QString time1=list[i].fileName().mid(9,6);
       QTime tSec1(time1.mid(0,2).toInt(),time1.mid(2,2).toInt(),time1.mid(4,2).toInt());

       if(abs(tSec2.secsTo(tSec1))<5)
       {
           qDebug()<<"相连图片"<<list[i-1].fileName()<<tSec1.toString("hh:mm:ss")<<tSec2.toString("hh:mm:ss")<<tSec2.secsTo(tSec1);
           qDebug()<<"相连图片"<<list[i].fileName()<<tSec1.toString("hh:mm:ss")<<tSec2.toString("hh:mm:ss")<<tSec2.secsTo(tSec1);

           QFile::copy(list1[i-1],QString("image/%1").arg(list[i-1].fileName()));
           QFile::copy(list1[i],QString("image/%1").arg(list[i].fileName()));
           QFile::remove(list1[i-1]);
           QFile::remove(list1[i]);
           continue;
       }
       tSec2=tSec1;
   }
       qDebug()<<"相连图片计算//结束并删除";


}

void MainWindow::on_pushButton_79_clicked()
{
    ui->lineEdit->setText("F:\\CPP\\QT\\opencvTest\\image");
}

void MainWindow::on_pushButton_80_clicked()
{

   // string s1=ui->comboBox->currentText().toLocal8Bit();

    int max1=0;
    int min1=1000;
    for(int i=0;i<fileList.size();i++)
    {
        QString name=(fileList[i].split("."))[0];
        if(name.length()<4)
        {
            int n=name.toInt();
            if(n>max1)
                max1=n;
            if(n<min1)
                min1=n;
        }

    }

    if(min1>TemplateName.size())
        max1=0;

    for(int i=0;i<fileList.size();i++)
    {
        qDebug()<<"i"<<i;

        QString path=loadPath+fileList[i];
        QFile::rename(path,QString("%1%2.png").arg(loadPath).arg(i+max1+1));

    }
}

void MainWindow::on_pushButton_81_clicked()
{
    matchFlag=1;
    ui->label->setText("0");
    QDir dir2("temp/");
    dir2.setFilter(QDir::Files);
    TemplateName= dir2.entryInfoList();
    tImageList.clear();
    list1.clear();


    for(int i=0;i<TemplateName.size();i++)
    {
        tNum[i]=0;
        QString path="temp/"+TemplateName[i].fileName();
        qDebug()<<"i:"<<i<<TemplateName[i].fileName();

        tImageList.append(cvLoadImage(path.toLocal8Bit().data(),0));
    }

    qDebug()<<"模板数:"<<tImageList.size();
    QString path=ui->lineEdit->text();
    path.replace(QString("\\"),QString("/"));
    if(path[path.length()-1]!='/')
        path.append('/');


    QDir dir(path);
    dir.setFilter(QDir::Files);
    QFileInfoList list= dir.entryInfoList();

   for(int i=0;i<list.size();i++)
   {
       list1<<path+list[i].fileName();
   }


    t4=new QTimer(this);
    connect(t4,SIGNAL(timeout()),this,SLOT(timer4()));
    t4->start(20);
}

void MainWindow::on_pushButton_82_clicked()
{
    matchFlag=2;
    ui->label->setText("0");
    QDir dir2("temp/");
    dir2.setFilter(QDir::Files);
    TemplateName= dir2.entryInfoList();
    tImageList.clear();
    list1.clear();

    QDir dir3("image/"+QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    if(!dir3.exists())
    {
        qDebug()<<dir3.mkdir(dir3.currentPath()+"/image/"+QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    }

    for(int i=0;i<TemplateName.size();i++)
    {
        tNum[i]=0;
        QString path="temp/"+TemplateName[i].fileName();
        qDebug()<<"i:"<<i<<TemplateName[i].fileName();

        tImageList.append(cvLoadImage(path.toLocal8Bit().data(),0));
    }

    qDebug()<<"模板数:"<<tImageList.size();
    QString path=ui->lineEdit->text();
    path.replace(QString("\\"),QString("/"));
    if(path[path.length()-1]!='/')
        path.append('/');


    QDir dir(path);
    dir.setFilter(QDir::Files);
    QFileInfoList list= dir.entryInfoList();

   for(int i=0;i<list.size();i++)
   {
       list1<<path+list[i].fileName();
   }


    t4=new QTimer(this);
    connect(t4,SIGNAL(timeout()),this,SLOT(timer4()));
    t4->start(20);
}

void MainWindow::on_pushButton_83_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();

    Mat m1=imread(s1,0);
//    Mat m2= pro1->rotateImg(m1);
    //【7】显示结果
    pro1->rotateRoi(m1,Rect(580,650,250,100));

    imshow( "24_1", m1 );
    //imshow( "24_2", m2 );
}
void cppTemplateMatch(Mat &src,Mat &temp,Point &p,Rect &roi,int method)
{
   int  nMatchMethod=method;
    //【1】给局部变量初始化
    Mat resultImage;

    Mat srcImage=src(roi).clone();

    //【2】初始化用于结果输出的矩阵
    int resultImage_cols =  srcImage.cols - temp.cols + 1;
    int resultImage_rows = srcImage.rows - temp.rows + 1;
    resultImage.create( resultImage_cols, resultImage_rows, CV_32FC1 );

    //【3】进行匹配和标准化

    matchTemplate( srcImage, temp, resultImage, nMatchMethod );
    normalize( resultImage, resultImage, 0, 1, NORM_MINMAX, -1, Mat() );

    //【4】通过函数 minMaxLoc 定位最匹配的位置
    double minValue; double maxValue; Point minLocation; Point maxLocation;
    Point matchLocation;

    minMaxLoc( resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat() );

    //【5】对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值有着更高的匹配结果. 而其余的方法, 数值越大匹配效果越好
    if( nMatchMethod  == CV_TM_SQDIFF || nMatchMethod == CV_TM_SQDIFF_NORMED )
    { matchLocation = minLocation; }
    else
    { matchLocation = maxLocation; }
    p.x=matchLocation.x;
    p.y=matchLocation.y;

    //【6】绘制出矩形，并显示最终结果

    rectangle( resultImage, matchLocation, Point( matchLocation.x + temp.cols , matchLocation.y + temp.rows ), Scalar(0,0,255), 2, 8, 0 );

    //        CV_TM_SQDIFF        =0,
    //        CV_TM_SQDIFF_NORMED =1,
    //        CV_TM_CCORR         =2,
    //        CV_TM_CCORR_NORMED  =3,
    //        CV_TM_CCOEFF        =4,
    //        CV_TM_CCOEFF_NORMED =5
    switch(nMatchMethod)
    {
    case CV_TM_SQDIFF:
        minValue/(temp.cols * temp.cols);
        break;
    case CV_TM_CCOEFF:
        maxValue/(temp.cols * temp.cols);
        break;
    }
   // qDebug()<<"method"<<nMatchMethod<<"min max"<<minValue<<maxValue;
    //qDebug()<<"min max"<<minValue/(temp.cols*temp.cols)<<maxValue/(temp.cols*temp.cols);

    imshow( "84_2", resultImage );
}

void MainWindow::on_pushButton_84_clicked()
{
    btnNext=ui->pushButton_84;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    namedWindow("84_1",0);
    namedWindow("84_2",0);
    resizeWindow("84_1",800,600);
    resizeWindow("84_2",800,600);

    params param;
    param.var1[0]=2;//模板图标定位置
    param.var1[1]=5;//匹配方法
    param.var1[2]=50;//匹配偏差范围

    param.var2[19]=360;
    Rect roi1(222,396,111,111);
    Rect roi2(638,372,111,111);
    Rect roi3;
    Rect roi(178,330,700,300);
    //上下左右  0 1 2 3
    int templatePos=param.var1[0];//模板图标定位置
    int method=param.var1[1];//匹配方法
    int difVal=param.var1[2];//匹配偏差范围


    if(templatePos==0)
        roi3=roi1.y>roi2.y?roi1:roi2;
    else if(templatePos==1){
        roi3=roi1.y<roi2.y?roi1:roi2;
    }else if(templatePos==2){
        roi3=roi1.x>roi2.x?roi1:roi2;
    }else if(templatePos==3){
        roi3=roi1.x<roi2.x?roi1:roi2;
    }

     Mat m1=imread(s1,0);
    // Mat m2=imread("F:/CPP/QT/opencvTest/temp/555.png",0);

     pro1->rotateRoi(m1,roi3);

     Point cen;
     Point t1Cen(roi1.x+roi1.width/2,roi1.y+roi1.height/2);
     Point t2Cen(roi2.x+roi2.width/2,roi2.y+roi2.height/2);

     cen.x=(t1Cen.x+t2Cen.x)/2;
     cen.y=(t1Cen.y+t2Cen.y)/2;

     int minLen=10000;
     int len;
     for(int i=0;i<tImageList2.size();i++)
     {
         Point p1;
         cppTemplateMatch(m1,tImageList2[i],p1,roi,method);

         p1.x+=roi.x+tImageList2[i].cols/2;
         p1.y+=roi.y+tImageList2[i].rows/2;


         qDebug()<<"cen"<<cen.x<<cen.y<<t1Cen.x<<t1Cen.y<<t2Cen.x<<t2Cen.y<<p1.x<<p1.y;

         int len1=common::getLineLen(t1Cen,p1);
         int len2=common::getLineLen(t2Cen,p1);
         len=len1<len2?len1:len2;

         if(len<difVal&&len<minLen)
         {
             minLen=len;
             param.var2[19]=common::calcAngle2(cen,p1);
             param.var1[29]=len;
             param.var1[28]=p1.x;
             param.var1[27]=p1.y;
         }
     }
      rectangle( m1, Point(param.var1[28]-roi1.width/2,param.var1[27]-roi1.height/2), Point( param.var1[28]+roi1.width/2 ,  param.var1[27]+roi1.height/2 ), Scalar(0,0,0), 2, 8, 0 );
      imshow( "84_1", m1 );
     qDebug()<<"angle"<<param.var2[19]<<"minLen"<<param.var1[29];


}

void MainWindow::on_pushButton_85_clicked()
{

    Mat m1=imread("temp2/y1.png",0);
    Mat m2=imread("temp2/y2.png",0);
//    Mat m3=imread("image/aaa/s1.png",0);
//    Mat m4=imread("image/aaa/s2.png",0);
//    Mat m5=imread("image/aaa/a3.png",0);
    Mat img1,img2,img3,img4,img5;
    threshold(m1,img1,198,255,THRESH_BINARY);
    threshold(m2,img2,198,255,THRESH_BINARY);
//    threshold(m3,img3,198,255,THRESH_BINARY);
//    threshold(m4,img4,198,255,THRESH_BINARY);
//    threshold(m5,img5,198,255,THRESH_BINARY);
     std::vector<std::vector<Point>> contours1,contours2,contours3,contours4,contours5;
     findContours(img1,
                  contours1, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     findContours(img2,
                  contours2, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


//     findContours(img3,
//                  contours3, // a vector of contours
//                  CV_RETR_LIST, // retrieve the external contours
//                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

//     findContours(img4,
//                  contours4, // a vector of contours
//                  CV_RETR_LIST, // retrieve the external contours
//                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


//     findContours(img5,
//                  contours5, // a vector of contours
//                  CV_RETR_LIST, // retrieve the external contours
//                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     //method 1 2 3

     double ret1=matchShapes(contours1[0],contours2[0],1,0);
     //double ret2=matchShapes(contours1[0],contours3[0],1,1);
     //double ret3=matchShapes(contours1[0],contours4[0],1,1);
     //double ret4=matchShapes(contours1[0],contours5[0],1,1);
     qDebug()<<"size"<<contours1[0].size()<<contours2[0].size();//<<contours3[0].size()<<contours4[0].size()<<contours5[0].size();
     qDebug()<<"result"<<ret1;//<<ret2<<ret3<<ret4;

}

void MainWindow::on_pushButton_86_clicked()
{
    readme->show();
}

void MainWindow::on_pushButton_87_clicked()
{
     string s1=ui->comboBox->currentText().toLocal8Bit();
     Mat combine;
     Mat img(200, 200, CV_8UC1, Scalar(0, 0, 0));
     Mat img2(100, 100, CV_8UC1, Scalar(255, 255, 255));
     int width=img.cols<img2.cols?img.cols:img2.cols;
     int height=img.rows<img2.rows?img.rows:img2.rows;
     hconcat(img(Rect(0,0,width,height)),img2(Rect(0,0,width,height)),combine);
     namedWindow("Combine",CV_WINDOW_AUTOSIZE);
     imshow("Combine",combine);
}

void MainWindow::on_pushButton_88_clicked()
{

    btnNext=ui->pushButton_88;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    //540,572,35,140,179,100,0
     params param;
      //E线参数

     Rect roi(249,251,849-249,876-251);
     //Rect roi(0,0,1280,960);
     param.var1[0]=531;//中心点X
     param.var1[1]=590;//中心点Y
     param.var1[2]=35;//检测字体最小值
     param.var1[3]=140;//检测字体最大值
     param.var1[4]=179;//字体到中心点距离
     param.var1[5]=100;//二值
     param.var2[0]=0;//角度偏移
     param.var1[28]=100000;//得到首次检测到NI坐标
     param.var1[29]=100000;
     param.var2[19]=360;//角度


     QTime t;
     t.start();

     if(!ui->checkBox_dll->isChecked())
     {
         pro1->zt_3rdLine(g_dstImage1,roi,param,false);
//         if(param.var2[19]==360)
//         {
           //  qDebug()<<"第二次计算";
            // pro1->zt_3rdLine(g_dstImage1,roi,param,true);
//         }
     }
     else
     {
         //call dll
         detectWork(g_dstImage1,roi,param,1);
         namedWindow("src",0);
         resizeWindow("src",800,600);
         imshow("src",g_dstImage1);
     }


    qDebug()<<"fileName"<<ui->comboBox->currentText().right(22)<<"x1 y1"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19]<<"特征数量"<<param.var1[26]<<"i:"<<param.var1[27];
    qDebug("Time elapsed: %d ms", t.elapsed());
    ui->label_3->setText(QString("method:%1 angle:%2").arg(param.var1[25]).arg(param.var2[19]));

}

void MainWindow::on_pushButton_89_clicked()
{
    QC_flag='E';
    DEBUG=false;
    t5=new QTimer(this);
    connect(t5,SIGNAL(timeout()),this,SLOT(timer5()));
    t5->start(50);
}

void MainWindow::on_pushButton_90_clicked()
{
    detectWork1();
}

void MainWindow::on_pushButton_91_clicked()
{
    btnNext=ui->pushButton_91;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    Mat src = imread(s1,0);
    Mat rgb = imread(s1,1);
    //namedWindow("18_1", 0 );
    //resizeWindow("18_1",1000,800);

    wk->edvia(src,rgb);
}

void MainWindow::on_pushButton_92_clicked()
{
    btnNext=ui->pushButton_92;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);


    //663,482,150,450,140,180,40,220,26,70,118,75,190,11,70,0,2,2.2,3
    params param;
    param.var1[0]=680;//中心点X
    param.var1[1]=482;//中心点Y
    param.var1[2]=150;// param.var1[2];//检测字体最小值
    param.var1[3]=450;//param.var1[3];//检测字体最大值
    param.var1[4]=140;//字体到中心点距离
    param.var1[5]=190;
    param.var1[6]=40;//二值
    param.var1[7]=220;
    param.var1[8]=26;//最小边长
    param.var1[9]=70;//最大边长
    param.var1[10]=118;//字体最大间距
    param.var1[15]=75;//NI最小size
    param.var1[16]=190;//NI最大size
    param.var1[17]=11;//NI最小边长
    param.var1[18]=70;//NI最大边长

    param.var2[0]=0;//角度偏移
    param.var2[1]=2;//长宽比例
    param.var2[2]=2.2;//NI长宽比例
    param.var2[3]=3;//NI面积比例 暂时不用
    Rect roi(360,180,550,550);
    //Rect roi(0,0,1280,960);
    QTime t;
    t.start();


    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);


    if(!ui->checkBox_dll->isChecked())
    {
        Mat m5,m6;
        medianBlur(g_dstImage1(roi), m5, 3);

        int offset = 10;
        int Absolute_offset = offset > 0 ? offset : -offset;
        Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
        if( offset < 0 )
            morphologyEx(m5, m6, MORPH_TOPHAT , element);
        else
            morphologyEx(m5, m6, MORPH_BLACKHAT, element);



        pro3->zt_GLineGN5(m6,g_dstImage2,m5,roi,param);
//        if(param.var2[19]==360)
//        {
//            qDebug()<<"calc NI";
//            pro3->zt_GLineNI(m6,g_dstImage2,roi,param);
//        }

//        if(param.var2[19]==360)
//        {
//            qDebug()<<"zt_GLineGN5_2";
//            pro3->zt_GLineGN5_2(m5,g_dstImage2,roi,param);
//        }

    }
    else
    {
       // call dll
        detectWork(g_dstImage1,roi,param,13);
        qDebug()<<"call dll";
    }


    imshow("rgb",g_dstImage2);
    qDebug()<<"angle:"<<param.var2[19]<<"Time elapsed:"<<t.elapsed()<<"ms";
    ui->label_3->setText(QString("angle:%1  Time elapsed:%2 ms").arg(param.var2[19]).arg(t.elapsed()));

}

void MainWindow::on_pushButton_93_clicked()
{
    btnNext=ui->pushButton_93;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    params param;
    //652,511,150,380,700,254,25,0,190,198
    //625,511,150,380,700,254,25,0,190,198

    param.var1[0]=652;//740;//圆心X
    param.var1[1]=511;//圆心Y
    param.var1[2]=150;//圆心与字距离
    param.var1[3]=380;//大半圆size
    param.var1[4]=500;//大半圆size
    param.var1[5]=254;//二值
    param.var1[6]=25;//计算NI step值
    param.var1[7]=198;//外接矩形长
    param.var1[8]=228;//外接矩形长
    param.var1[9]=40;//圆心与字距离最大偏差
    param.var2[0]=0;//角度偏差

    //Rect r1(0,0,1280,960);
    Rect roi(347,236,941-347,816-236);

    QTime t;
    t.start();

    if(!ui->checkBox_dll->isChecked())
        pro1->zt_2ndLine(g_dstImage1,roi,param);
    else
        detectWork(g_dstImage1,roi,param,17);

    common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[2],param.var2[19],90,Scalar(255,0,0));

    imshow("show",g_dstImage2);
     qDebug()<<"angle:"<<param.var2[19]<<"耗时:"<<t.elapsed();
}

void MainWindow::on_pushButton_94_clicked()
{
    btnNext=ui->pushButton_94;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    pro3->similarity();



}
void createList()
{
    std::vector<int> aaa;
    aaa.push_back(0);
    aaa.push_back(1);
    aaa.push_back(2);
    qDebug()<<"aaa";
}
void MainWindow::on_pushButton_95_clicked()
{

    btnNext=ui->pushButton_95;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    namedWindow("pushButton_95", 0 );
    resizeWindow("pushButton_95",1200,800);
    Rect roi(0,0,g_dstImage1.cols,g_dstImage1.rows);
    params param;
    param.var1[0]=6000;//size范围
    param.var1[1]=240;//二值
    param.var1[2]=25;//step
    pro3->clothEdge(g_dstImage1,roi,param);
    imshow("pushButton_95",g_dstImage2);

}

void MainWindow::on_pushButton_96_clicked()
{
    QString path=QDir::currentPath()+"/image";//获取程序当前目录
    path.replace("/","\\");//将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
    QProcess::startDetached("explorer "+path);//打开上面获取的目录
}

void MainWindow::on_pushButton_97_clicked()
{
    btnNext=ui->pushButton_97;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    pro2->hd_gap();
}

void MainWindow::on_pushButton_98_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
     Rect roi(470,220,400,400);
     g_dstImage1=imread(s1,0);//(roi);
     g_dstImage2=imread(s1,1);//(roi);
medianBlur(g_dstImage1, g_dstImage3,medianBlurVal);
//    Mat s2=imread(s1,0);
//    Mat s3=imread(s1,1);
//    Rect r(894,860,1477-894,1448-880);
//    g_dstImage1=s2(r);
//    g_dstImage2=s3(r);
    useBlackH=true;
    pro2->min_rect1();
}

void MainWindow::on_pushButton_99_clicked()
{


    btnNext=ui->pushButton_99;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);


    IplImage *image=cvLoadImage(ui->comboBox->currentText().toLocal8Bit().data());//打开图像源图像
    pro3->rgbPic(image,1);

}

void MainWindow::on_pushButton_100_clicked()
{
    double d1=0;
    int n=0;
    for(int i=0;i<fileList.size();i++)
    {
        QString s=fileList[i];
        s.remove(0,16);
        s.split('.');
        double angle=s.split('.').at(0).toDouble();
        if(abs(angle-d1)>1)
        {
            d1=angle;
            n=0;
        }
        else
        {
            n++;
        }
        if(n>2)
        {
            QFile::remove(loadPath+fileList[i]);
            qDebug()<<loadPath+fileList[i];
        }

    }
}

void MainWindow::on_pushButton_101_clicked()
{
    btnNext=ui->pushButton_101;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    params param;
    //562,513,103,15,90,300,0,200,280,550,1100,254,280,325,0
//103,15,90,300,0
    param.var1[0]=562;//569;//629;//圆心X
    param.var1[1]=513;//514;//圆心Y
    param.var1[2]=200;//圆心与字距离
    param.var1[3]=280;//圆心与字距离
    param.var1[4]=550;//大半圆size
    param.var1[5]=1100;//大半圆size
    param.var1[6]=254;//152;//二值
    param.var1[7]=280;//205;//外接矩形长
    param.var1[8]=325;//外接矩形长    
    param.var2[0]=0;//角度偏差

    param.var1[29]=82000;//计算标识


    //Rect r1(0,0,1000,800);
    Rect roi(206,146,800,650);
    QTime t;
    t.start();
    if(!ui->checkBox_dll->isChecked())
        pro1->zt_2ndLine(g_dstImage1,roi,param);
    else
        detectWork(g_dstImage1,roi,param,17);

    common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[2],param.var2[19],90,Scalar(255,0,0));

    imshow("show",g_dstImage2);
    ui->label_3->setText(QString("angle:%1 耗时%2").arg(param.var2[19]).arg(t.elapsed()));

}

void MainWindow::on_pushButton_103_clicked()
{
    btnNext=ui->pushButton_103;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    namedWindow("pushButton_95", 0 );
    resizeWindow("pushButton_95",1200,800);
    Rect roi(0,0,g_dstImage1.cols,g_dstImage1.rows);
    params param;
    param.var1[0]=6000;//size范围
    param.var1[1]=150;//二值
    param.var1[2]=35;//step
    pro3->clothEdge(g_dstImage1,roi,param);
    imshow("pushButton_95",g_dstImage2);
}



void MainWindow::on_pushButton_105_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    Mat img1;

    threshold(g_dstImage1,img1,198,255,THRESH_BINARY);
    imwrite("image/temp1.png",img1);
}



void MainWindow::on_pushButton_106_clicked()
{
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage2=imread(s1,1);
    pro3->splitRGB(g_dstImage2);
}

void MainWindow::on_pushButton_107_clicked()
{
    btnNext=ui->pushButton_107;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);
    IplImage *image=cvLoadImage(ui->comboBox->currentText().toLocal8Bit().data());//打开图像源图像
    pro3->rgbPic(image,0);
}

void MainWindow::on_pushButton_108_clicked()
{
    btnNext=ui->pushButton_108;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);


    Point pos;
    Point leftUp(84,172),rightDown(359,265);
    qDebug()<<"area:"<<pro1->icecream(g_dstImage1,leftUp,rightDown,200,pos);
    imshow("rgb",g_dstImage2);
}

void MainWindow::on_pushButton_109_clicked()
{

    btnNext=ui->pushButton_109;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);

    params param;
    //590,530,108,168,100,500,30,22,0
    //590,530,100,14,90,108,0,108,168,100,500,30,22,0
    param.var1[0]=590;//中心点X
    param.var1[1]=530;//中心点Y
    param.var1[2]=130;//字体到中心点距离
    param.var1[3]=215;
    param.var1[4]=120;//检测字体最小值
    param.var1[5]=500;//检测字体最大值
    param.var1[6]=30;//二值
    param.var1[7]=30;//小三角形max len
    param.var2[0]=0;//角度偏移

    param.var1[28]=10000;//得到首次检测到NI坐标
    param.var1[29]=10000;
    param.var2[19]=360;//角度



    Rect roi(0,0,1280,960);
    QTime t;
    t.start();

    pro2->zt_ZDK(g_dstImage1,g_dstImage2,roi,param);
    ui->label_3->setText(QString::number(param.var2[19]));
    common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[2],param.var2[19],90,Scalar(0,0,255));
    imshow("rgb",g_dstImage2);
    qDebug("Time elapsed: %d ms", t.elapsed());
    qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];
}




void MainWindow::on_checkBox_stateChanged(int arg1)
{
    //ui->checkBox->setChecked(arg1);
    if(arg1==0)
        updateXml(xmlName,"checkBox","0");
    else
        updateXml(xmlName,"checkBox","1");
}








void MainWindow::on_pushButton_117_clicked()
{
    btnNext=ui->pushButton_117;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);
    g_dstImage2=imread(s1,1);


    pro2->hd_vision4();

}


void MainWindow::on_pushButton_119_clicked()
{
    string s2=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage2=imread(s2,0);

}
void MainWindow::on_pushButton_121_clicked()
{
    string s3=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage3=imread(s3,0);
}
void MainWindow::on_pushButton_120_clicked()
{
    btnNext=ui->pushButton_120;
    string s1=ui->comboBox->currentText().toLocal8Bit();
    g_dstImage1=imread(s1,0);

    if(g_dstImage2.cols==0||g_dstImage3.cols==0)
        return;
    Rect roi(0,0,640,480);
    pro3->cmpPicPixel(g_dstImage1,g_dstImage2,g_dstImage3,roi,30);



}




void MainWindow::abc()
{
    qDebug()<<"abc";
}





void MainWindow::on_pushButton_134_clicked()
{
    //M线三角形
        btnNext=ui->pushButton_134;
        string s1=ui->comboBox->currentText().toLocal8Bit();
        g_dstImage1=imread(s1,0);
        g_dstImage2=imread(s1,1);
        //606,480,1000,1,90,150,2,-1,-1,80,230,210,280,50,230,17,60,360,20,0,10,1,1000,80,23,-1,-1,0

        params param;
        param.var1[0]=606;//中心点X
        param.var1[1]=480;//中心点Y
        param.var1[2]=80;//检测字体最小值
        param.var1[3]=230;//检测字体最大值
        param.var1[4]=210;//字体到中心点距离
        param.var1[5]=280;//字体到中心点距离
        param.var1[6]=50;//二值
        param.var1[7]=230;//二值
        param.var1[8]=17;//外接矩形len
        param.var1[9]=60;//外接矩形len
        param.var1[10]=360;//外接矩形角度默认360   <45保留 or >45保留
        param.var1[11]=20;//角度范围20  标识0附近保留  标识90附近保留
        param.var1[12]=0;//范围标识0 90
        param.var1[13]=5;//i+=step;
        param.var1[14]=1;//1黑色  0白色
        param.var1[15]=1000;//噪点
        param.var1[16]=80;//相似度
        param.var1[17]=23;//fg
        param.var1[18]=2;//method
        param.var2[0]=-1;//长度默认-1    <100 <过滤  or >100  >过滤  长/宽
        param.var2[1]=-1;//面积默认-1     <100 <过滤  or >100  >过滤  area/rectArea

        param.var2[18]=0;//角度偏移


        param.var2[19]=360;//角度


        Rect roi(250,300,750,380);
        QTime t;
        t.start();

        if(ui->checkBox_dll->isChecked())
        {
            detectWork(g_dstImage1,roi,param,102);
        }
        else
        {
            Mat m5;
            medianBlur(g_dstImage1(roi), m5, 3);

            pro3->commonMatch(m5,g_dstImage2,roi,param);

        }

        if(DEBUG)
        {
            qDebug("Time elapsed: %d ms", t.elapsed());
            qDebug()<<"x y"<<param.var1[28]+roi.x<<param.var1[29]+roi.y<<"angle:"<<param.var2[19];

            namedWindow("rgb",0);
            resizeWindow("rgb",800,600);
            common::drawRect(g_dstImage2,Point(param.var1[0],param.var1[1]),param.var1[5],param.var2[19],90,Scalar(0,0,255));
            imshow("rgb",g_dstImage2);
            ui->label_3->setText(QString("angle:%1 matchVal:%2").arg(param.var2[19]).arg(param.var1[27]));
        }


        paramRet=param.var2[19];
}
