#ifndef PROJECT2_H
#define PROJECT2_H

#include <QObject>
#include "Global.h"
#include <QSharedMemory>

class project2 : public QObject
{
    Q_OBJECT
public:
    explicit project2(QObject *parent = 0);
    void hd_vision1();
    void hd_vision2();
    void hd_vision3();
    void hd_vision4();
    void hd_vision1_2(Mat &src, Rect roi, params &param);
    void hd_vision3_angle(Mat &src, Rect roi, params &param);
    //void hd_vision3_flaw(Mat &src, Rect roi, params &param);
    void hd_maxDiameter(Mat &src,Rect roi,params &param);
    void hd_maxDiameter2(Mat &src,Rect roi,params &param);
    void hd_gap();
    void hd_smallWork(Mat &src, Rect roi, params &param);
    void hd_smallWork_other(Mat &src, Rect roi, params &param);
    void min_rect1();
    void zt_ZT01(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_ZT01_2(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_ZDK(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_5thLine(Mat &src,Mat &rgb,Rect roi,params& param);
    void test1(IplImage *pSrcImage);
    void sendUdp(QString str);
    void sz_cloth(Mat &src,Mat &rgb,Rect roi,params& param);
    void getSharedMem();

    void wx_direction0(Mat &src, Mat &rgb, params& param);
    void wx_direction1(Mat &src, Mat &rgb, Rect roi, params& param);
    void wx_defect(Mat &src, Rect roi, params& param);
    void testUdp();
    void armPic();
    int gap1(Mat &src,Rect roi,int minWidth,int difVal);

signals:
    void addLog(QString str);
public slots:

private slots:
    void processUdp();
private:
    QUdpSocket *m_client;
    IplImage *udpImg,*rgbImg;
    int pic_count,pic_sum;
    QSharedMemory *sharememory;

};

#endif // PROJECT2_H
