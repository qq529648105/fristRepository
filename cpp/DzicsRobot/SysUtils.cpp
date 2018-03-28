#include "SysUtils.h"

#include <QSqlQuery>

#include <vector>
#include <QFile>
#include <QRegExp>
#include <QCryptographicHash>
#include <QApplication>

#include <GL/glew.h>
#include <math.h>

#include <windows.h>
#include <winuser.h>
#include <QDir>
#include <QString>
#include <QDate>
//#define uint    unsigned int

namespace SysUtils {

    static const uint CurrentDbVersion = 11;

/******************** 私有函数 ********************/

/***** 标准数据类型 *****
    NULL
    INTEGER
    REAL
    TEXT
    BLOB
******** 也接受如下的数据类型：
    smallint 16 位元的整数。
    interger 32 位元的整数。
    decimal(p,s) p 精确值和 s 大小的十进位整数，精确值p是指全部有几个数(digits)大小值，s是指小数点後有几位数。如果没有特别指定，则系统会设为 p=5; s=0 。
    float   32位元的实数。
    double   64位元的实数。
    char(n)   n 长度的字串，n不能超过 254。
    varchar(n) 长度不固定且其最大长度为 n 的字串，n不能超过 4000。
    graphic(n) 和 char(n) 一样，不过其单位是两个字元 double-bytes， n不能超过127。这个形态是为了支持两个字元长度的字体，例如中文字。
    vargraphic(n) 可变长度且其最大长度为 n 的双字元字串，n不能超过 2000
    date   包含了 年份、月份、日期。
    time   包含了 小时、分钟、秒。
    timestamp 包含了 年、月、日、时、分、秒、千分之一秒。
******/

    bool upgradeDatabase(uint lastVersion, std::vector<double> a=std::vector<double>(), int t=0) {
        QSqlQuery query;
        switch(lastVersion) {
            case 0:
                addSysParam("softVersion", "1.0.1028");
                addSysParam("language", "zh_CN");
                addSysParam("initPage", 1);
				addSysParam("basePath", "D:\\DzicsRobot");
                addSysParam("lastStationId", "");

                //                addSysParam("machineType", 11);  // 机床类型，1:车床，2:铣床，3:加工中心，11:六轴机械手，21:摆臂机械手，22:龙门机械手
                // 文件库
                //query.exec("CREATE TABLE File(id CHAR(24) NOT NULL PRIMARY KEY, name VARCHAR(500), content BLOB, creationTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP))");

                // 模型
//                query.exec("CREATE TABLE Module(id CHAR(24) NOT NULL PRIMARY KEY, code VARCHAR(125), name VARCHAR(250) NOT NULL, type INTEGER, fileId CHAR(24), bindAxis CHAR(24), x REAL, y REAL, z REAL, rotateX REAL, rotateY REAL, rotateZ REAL, rotateAngle REAL, material INTEGER, color INTEGER, parentId CHAR(24), isUse INTEGER, isLock INTEGER, creationTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");    // 模型类型(type)，1：机床本体，2：联动轴，3：辅助轴，4：毛坯夹(夹具)，5：毛坯，6：刀架，7：刀
//                query.exec("CREATE INDEX Module_parentId_index ON Module(parentId)");
//                query.exec("CREATE INDEX Module_code_index ON Module(code)");
//                query.exec("CREATE INDEX Module_type_index ON Module(type)");
//                query.exec("CREATE INDEX Module_bindAxis_index ON Module(bindAxis)");

                // 坐标轴
//                query.exec("CREATE TABLE Axis(id CHAR(24) NOT NULL PRIMARY KEY, sign CHAR(10), name VARCHAR(250) NOT NULL, type INTEGER, startX REAL, startY REAL, startZ REAL, endX REAL, endY REAL, endZ REAL, symbol INTEGER, originOffset REAL, minValue REAL, maxValue REAL, motorVar REAL, offsetVar REAL, adjustVar REAL, backVar REAL, isMain INTEGER, channel INTEGER, displayOrder INTEGER, parentId CHAR(24), isUse INTEGER, isLock INTEGER, creationTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");    // 轴类型(type)，1：平移轴，2：旋转轴，3：摆臂轴； 符号(symbol)，1：正，-1：负
//                query.exec("CREATE INDEX Axis_parentId_index ON Axis(parentId)");
//                query.exec("CREATE INDEX Axis_type_index ON Axis(type)");
//                query.exec("CREATE INDEX Axis_channel_index ON Axis(channel)");

                // 产品信息
//                query.exec("CREATE TABLE Product(id CHAR(24) NOT NULL PRIMARY KEY, name VARCHAR(250) NOT NULL, shortName VARCHAR(250) NOT NULL, codeText TEXT, roughId CHAR(24), roughChuckId CHAR(24), knifeId CHAR(24), knifeShelfId CHAR(24), processTime TIME, totalProduce INTEGER, description TEXT, creationTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");

                // 毛坯夹(夹具)
//                query.exec("CREATE TABLE RoughChuck(id CHAR(24) NOT NULL PRIMARY KEY, code VARCHAR(125), name VARCHAR(250), moduleId CHAR(24), specification VARCHAR(1000), description TEXT, creationTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");

//                // 毛坯
//                query.exec("CREATE TABLE Rough(id CHAR(24) NOT NULL PRIMARY KEY, code VARCHAR(125), name VARCHAR(250), moduleId CHAR(24), specification VARCHAR(1000), description TEXT, creationTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
//
//                // 刀架
//                query.exec("CREATE TABLE KnifeShelf");
//
//                // 刀片
//                query.exec("CREATE TABLE Blade");
//
//                // 刀
//                query.exec("CREATE TABLE Knife");
//
//                // 加工配置
//                query.exec("CREATE TABLE WorkConfig");




//                QUuid::createUuid().toString().toAscii().data()

            case 10:
                addSysParam("sys", "sys");

//            case 11:
//                query.exec("create table ParameterLibrary(id char(24) not null,variable char(24),value varchar(128),type int, postionX  int,postionY int,primary key (id))");
//                query.exec("create table ParameterLibrary(id char(24) not null,variable char(24),value varchar(128),type int, postionX  int,postionY int,primary key (id))");
//                query.exec("create table ParameterLibrary(id char(24) not null,variable char(24),value varchar(128),type int, postionX  int,postionY int,primary key (id))");
//                query.exec("create table ParameterLibrary(id char(24) not null,variable char(24),value varchar(128),type int, postionX  int,postionY int,primary key (id))");

/*            default:

*/

        }

        return true;
    }





/******************** 公共函数 ********************/

    QVariant getSysParam(QString paramCode) {
        QSqlQuery query;
        if (query.exec(QString("SELECT value FROM SysParam WHERE code='%1'").arg(paramCode))) {
            if (query.next())
                return query.value(0);
        }
        return NULL;
    }

    bool setSysParam(QString paramCode, QVariant paramValue) {
        QSqlQuery query;
        return query.exec(QString("UPDATE SysParam SET value='%2' WHERE code='%1'").arg(paramCode, paramValue.toString()));
    }

    bool addSysParam(QString paramCode, QVariant paramValue) {
        QSqlQuery query;
        return query.exec(QString("INSERT INTO SysParam(code, value) VALUES('%1', '%2')").arg(paramCode, paramValue.toString()));
    }

    bool addFile(QString id, QByteArray content, QString name) {
        QSqlQuery query;
        query.prepare("INSERT INTO File(id, name, content) VALUES(?,?,?)");
        query.addBindValue(id);
        query.addBindValue(name);
        query.addBindValue(content, QSql::Binary);
        return query.exec();
    }

    bool setFile(QString id, QByteArray content) {
        QSqlQuery query;
        query.prepare("UPDATE File SET content=? WHERE id=?");
        query.addBindValue(content, QSql::Binary);
        query.addBindValue(id);
        return query.exec();
    }

    QByteArray getFile(QString id) {
        QSqlQuery query;
        if (query.exec(QString("SELECT content FROM File WHERE id='%1'").arg(id))) {
            if (query.next())
                return query.value(0).toByteArray();
        }
        return NULL;
    }

    QString text(QString s) {
        int i=6;
        QString r=s.left(6);
        while (i<30) {
            r += "-"+s.mid(i, 6);
            i += 6;
        }
        return r;
    }

    QString put() {
        QString lpRootName = "C";
        LPTSTR lpVolumeNameBuffer=new TCHAR[12];
        DWORD nVolumeNameSize=12;
        DWORD VolumeSerialNumber;
        DWORD MaximumComponentLength;
        lpRootName+=":";
        LPTSTR lpFileSystemNameBuffer=new TCHAR[10];
        DWORD nFileSystemNameSize=10;
        DWORD FileSystemFlags;
        lpRootName+="\\";
        GetVolumeInformation((LPTSTR)lpRootName.utf16(),
          lpVolumeNameBuffer, nVolumeNameSize,
          &VolumeSerialNumber, &MaximumComponentLength,
          &FileSystemFlags,
          lpFileSystemNameBuffer, nFileSystemNameSize);
        QByteArray aa,bb;
        aa.append(VolumeSerialNumber + 8566);
        aa.append(aa.at(1));
        aa.append("Dzics.c0m");
        bb = QCryptographicHash::hash(aa, QCryptographicHash::Sha1 );
        bb.append(bb.at(8));
        bb.append(bb.at(5));
        bb.append(QCryptographicHash::hash(bb, QCryptographicHash::Md5 ));
        bb.append(bb.at(16));
        bb.append(bb.at(06));
        return QCryptographicHash::hash( bb, QCryptographicHash::Md5 ).toHex();
    }

    bool get() {
        QString lpRootName = "In";
        lpRootName += "fo.";
        lpRootName += "in";
        lpRootName += "i";
        if (QFile(lpRootName).exists()) {
            QString c = readFile(lpRootName);
            if (!c.endsWith('D')) {
                if (c.remove('\r').remove('\n').remove(' ').toLower() == text(put().mid(2))) {
                    return false;
                } else {
                    writeFile(lpRootName, text(put().toUpper().mid(2)).toLocal8Bit());
                    return false;
                }
            }
            setSysParam("sys", uuid().left(17)+c.remove('-').toLower().left(30)+uuid().left(19));
        }
        QByteArray aa,bb;
        aa.append("DZics");
        aa.append(".C0M");
        aa.append(put().mid(2));
        aa.append(aa.at(18));
        aa.append(aa.at(7));
        aa.append(getSysParam("basePath").toString());
        aa.append(aa.at(26));
        bb = QCryptographicHash::hash(aa, QCryptographicHash::Sha1 );
        bb.append(bb.at(18));
        if (bb.at(8)>60) {
            bb.append(bb.at(16));
        }
        aa.append(aa.at(26));
        bb.append(bb.at(7));
        bb.append(getSysParam("basePath").toString());
        bb.append(bb.at(26));
        if (text(QCryptographicHash::hash(bb, QCryptographicHash::Md5 ).toHex()) == text(getSysParam("sys").toString().mid(17,30))) return QFile(lpRootName).remove()?true:true;
            aa.append(aa.at(18));
        aa.append(aa.at(7));
        aa.append(aa.at(26));
        bb.append(bb.at(18));
        bb.append(bb.at(7));
        bb.append(bb.at(26));
        writeFile(lpRootName, text(put().toUpper().mid(2)).toLocal8Bit());
        return false;
    }

    bool setFileName(QString id, QString name) {
        QSqlQuery query;
        return query.exec(QString("UPDATE File SET name='%2' WHERE id='%1'").arg(id, name));
    }


    QString getFileName(QString id) {
        QSqlQuery query;
        if (query.exec(QString("SELECT name FROM File WHERE id='%1'").arg(id))) {
            if (query.next())
                return query.value(0).toString();
        }
        return NULL;
    }

    QString idToFile(QString id) {
        return id.replace(QRegExp("[\\/:*?\"<>|=]"), "D");
    }

    bool checkDatabase() {

        QSqlQuery query;
        uint dbVersion = getSysParam("dbVersion").toUInt();
        if (dbVersion == 0) {
            QSqlDatabase db = QSqlDatabase::database();
            db.transaction();
            if (!query.exec("CREATE TABLE SysParam(code CHAR(32) NOT NULL PRIMARY KEY, value VARCHAR(256) NOT NULL)")) {
//                db.rollback();
//                return false;
            }
            if (!addSysParam("dbVersion", 1)) {
                db.rollback();
                return false;
            }
            db.commit();
        }
        if (dbVersion < CurrentDbVersion) {
            QSqlDatabase db = QSqlDatabase::database();
            db.transaction();
            if (!upgradeDatabase(dbVersion)) {
                db.rollback();
                return false;
            }
            if (!setSysParam("dbVersion", CurrentDbVersion)) {
                db.rollback();
                return false;
            }
            db.commit();
        }
        return get();
    }


    QByteArray readFile(QString fileName) {
        QFile file(fileName);
        QByteArray data;
        if (!file.open(QFile::ReadOnly)) return data;
        data = file.readAll();
        file.close();
        return data;
    }

    bool writeFile(QString fileName, QByteArray data) {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly)) return false;
        file.write(data);
        file.close();
        return true;
    }

    QString uuid() {
        return uuid(QUuid::createUuid());
    }

    QString uuid(QUuid uuid) {
        QByteArray byte;
        QDataStream stream(&byte, QIODevice::WriteOnly);
        stream << uuid.data1 << uuid.data2 << uuid.data3;
        stream.writeRawData((char *)uuid.data4, 6);
//        printf("%s, %s, %s\n", byte.toBase64().data(), QByteArray::fromBase64(byte.toBase64()).toHex().data(), uuid.toString().toLatin1().data());
        return QString(byte.toBase64().data()).replace(QRegExp("[\\/:*?\"<>|+=]"), "D");
    }

	void glRotate(char axis, double center[3], double angle) {
		static double matrix[16]={
			1., 0., 0., 0.,
			0., 1., 0., 0.,
			0., 0., 1., 0.,
			0., 0., 0., 1.
		};
		switch(axis){
			case X:
				matrix[5]=cos(angle);
				matrix[6]=sin(angle);
				matrix[9]=-sin(angle);
				matrix[10]=cos(angle);
				break;
			case Y:
				matrix[0]=cos(angle);
				matrix[2]=-sin(angle);
				matrix[8]=sin(angle);
				matrix[10]=cos(angle);
				break;
			case Z:
				matrix[0]=cos(angle);
				matrix[1]=sin(angle);
				matrix[4]=-sin(angle);
				matrix[5]=cos(angle);
				break;
		}
		matrix[12]=center[X];
		matrix[13]=center[Y];
		matrix[14]=center[Z];
		glMultMatrixd(matrix);
		glTranslated(-center[X], -center[Y], -center[Z]);
	}

    // 外部打标
    void externalPrint(bool isShow)
    {
        HWND hwnd = FindWindow(NULL, QString("ThorX6 - Robot").utf16());

        if (hwnd) {
            if (isShow) {
                ShowWindow(hwnd, SW_SHOWMAXIMIZED);
                SetForegroundWindow(hwnd);
            }
        } else {
            WinExec("D:\\ThorX6\\X6.exe D:\\ThorX6\\Robot.Tx6", isShow ? SW_SHOWMAXIMIZED : SW_SHOWNOACTIVATE);
        }
    }

    // 远程协助
    void remoteHelp(bool isShow)
    {
        HWND hwnd = FindWindow(NULL, QString("大正远程协助系统").utf16());

        if (hwnd) {
            if (isShow) {
                ShowWindow(hwnd, SW_NORMAL);
                SetForegroundWindow(hwnd);
            }
        } else {
            WinExec("D:\\DzicsRobot\\DzicsHelp.exe", isShow ? SW_NORMAL : SW_SHOWNOACTIVATE);
        }
    }

    // 计算条件表达式
    bool evalCondition(QString valueLeft, QString valueOperator, QString valueRight) {
        bool checkResult = false;
        if (valueLeft.isEmpty() || valueRight.isEmpty())
            return false;
        float v1 = valueLeft.toDouble(), v2 = valueRight.toDouble();
        if (valueOperator == "=") {
//            if (v1 == v2) {
            if (fabs(v1 - v2) < 1e-6) {
                checkResult = true;
            }
        } else if(valueOperator == "<>") {
//            if (v1 != v2) {
            if (fabs(v1 - v2) >= 1e-6) {
                checkResult = true;
            }
        } else if(valueOperator == ">") {
            if (v1 > v2) {
                checkResult = true;
            }
        } else if(valueOperator == "<") {
            if (v1 < v2) {
                checkResult = true;
            }
        } else if(valueOperator == ">=") {
//            if (v1 >= v2) {
            if ( (v1 > v2) || (fabs(v1 - v2)<1e-6) ) {
                checkResult = true;
            }
        } else if(valueOperator == "<=") {
//            if (v1 <= v2) {
            if ( (v1 < v2) || (fabs(v1 - v2)<1e-6) ) {
                checkResult = true;
            }
        }
        return checkResult;
    }


}

namespace common {

bool getCircleCenter(Point p1,Point p2,Point p3,Point& p)
{
    bool flag=false;

    if(p1.x==p2.x)
        p2.x++;
    if(p1.x==p3.x)
        p3.x++;

    double u=(p1.x*p1.x-p2.x*p2.x+ p1.y*p1.y-p2.y*p2.y)/(2*p1.x-2*p2.x);
    double v=(p1.x*p1.x-p3.x*p3.x+ p1.y*p1.y-p3.y*p3.y)/(2*p1.x-2*p3.x);
    double k1=(p1.y-p2.y)*1.0/(p1.x-p2.x);
    double k2=(p1.y-p3.y)*1.0/(p1.x-p3.x);
    if(abs(k1-k2)<0.000001)
        return false;
    p.y=(u-v)/(k1-k2);
    p.x=v-(u-v)*k2/(k1-k2);

    if(p.x>0&&p.y>0&&p.x<2600&&p.y<2000)
    {
        flag=true;
    }
    else
    {
        p.x=0;
        p.y=0;
    }
    return flag;

}

double mathAngle(double a,double b)
{
    double c=sqrt(a*a*1.0+b*b*1.0);
    return acos(b*1.0/c)*180/3.1415;
}

double calcAngle(Point p1,Point p2)
{
    if(p1.x==p2.x)
        return 90;

    if(p1.y==p2.y)
        return 0;

    if(p1.x>p2.x)
    {
        Point p3=p1;
        p1=p2;
        p2=p3;
    }
    if(p2.y<p1.y)
        return mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x))*-1.0;
    else
        return mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x));

}
int getLineLen(Point p1,Point p2)
{
    return (int)sqrt((p1.x-p2.x*1.0)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

//振挺using
double calcAngle2(Point p1,Point p2)//p1中心点 p2物体坐标
{
    if(p2.x>p1.x)
    {
        if(p2.y<p1.y)
        {
            return common::mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x))-180;
        }else
        {
            return 180-common::mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x));
        }
    }
    else
    {
        if(p2.y<p1.y)
        {

            return common::mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x))*-1;
        }else
        {
            return common::mathAngle(abs(p2.y-p1.y),abs(p2.x-p1.x));
        }
    }



}

int linesAngle(double angle1, double angle2,bool sign)
{
    int dif=angle1-angle2;
    if(dif>90)
        dif=180-dif;
    if(dif<-90)
        dif=180+dif;

    if(!sign)
        dif=abs(dif);

    return dif;

}

Point xyCalibration(double angle,Point p1,Point p2)//p1中心点
{
    if(p1==p2)
        return p1;
    double angle1=mathAngle(abs(p1.y-p2.y),abs(p1.x-p2.x));
    int len=getLineLen(p1,p2);
    int signX,signY;
    double angle3=angle1+angle;

    if(p2.x<p1.x&&p2.y<p1.y||p2.x>p1.x&&p2.y>p1.y)
        angle3=angle1+angle*-1;

    if(p1.x==p2.x)
    {
        if(p2.y<p1.y)
        { signY=-1; signX=angle>0?-1:1;}
        else
        {signY=1;signX=angle>0?1:-1;}
    }
    else if(p1.y==p2.y)
    {
        if(p2.x<p1.x)
        { signX=-1; signY=angle>0?1:-1;}
        else
        { signX=1;signY=angle>0?-1:1;}
    }
    else
    {
        signX=(p2.x>p1.x?1:-1);
        signY=(p2.y>p1.y?1:-1);
        if(angle3>90) signX= signX>0?-1:1;
        if(angle3<0)  signY= signY>0?-1:1;
    }

    int x=signX*len*abs(cos(angle3*3.1415/180))+p1.x;//xOffset
    int y=signY*len*abs(sin(angle3*3.1415/180))+p1.y;//yOffset
    //qDebug()<<"X:"<<x<<"Y:"<<y<<"|"<<sin((angle1+angle)*3.1415/180)<<"p2.x"<<p2.x<<"p2.y"<<p2.y<<"angle"<<angle<<"p1.x"<<p1.x<<"p1.y"<<p1.y;
    return Point(x,y);
}


int circleFeature(std::vector<Point> &contours,Point cen,int step,int move,int offset)
{
    int size=contours.size();
    int num=0;
    Point p;
    bool success;

    for(int i=0;i+step*2<size;i+=move)
    {
        success= getCircleCenter(contours[i],contours[step+i],contours[step*2+i],p);
        if(success)
        {
            //qDebug()<<"aaa"<<p.x<<p.y<<"cen"<<cen.x<<cen.y;
            if(abs(p.x-cen.x)<offset&&abs(p.y-cen.y)<offset)
                 ++num;
        }
    }

    return num;
}

int trangleCalc2(Mat &rgb,std::vector<Point> &contours,int sideLen,int step,Rect &roi)
{
    int size=contours.size();
    int i2,j2,i3,j3;
    int j=0;
    int num=0;



    for(int i=0;i<size;i+=step)
    {

        i2=i+step;
        j2=j-step;
        i3=i+2*step;
        j3=j-2*step;
        if(i>=size) i-=size;
        if(i2>=size) i2-=size;
        if(i3>=size) i3-=size;
        if(j<0) j+=size;
        if(j2<0) j2+=size;
        if(j3<0) j3+=size;

       // qDebug()<<"trangle"<<abs(common::calcAngle(contours[i],contours[i2])-common::calcAngle(contours[i2],contours[i3]))<<abs(common::calcAngle(contours[j],contours[j2])-common::calcAngle(contours[j2],contours[j3]));
   // circle( rgb, Point(contours[i].x+roi.x,contours[i].y+roi.y ), 3, Scalar(0,255,0), 1, 16);
         if(abs(common::calcAngle(contours[i],contours[i2])-common::calcAngle(contours[i2],contours[i3]))<10)
              ++num;
         if(abs(common::calcAngle(contours[j],contours[j2])-common::calcAngle(contours[j2],contours[j3]))<10)
             ++num;


         j-=step;


    }

    //qDebug()<<"jjj"<<i<<i2<<i3<<j<<j2<<j3;
    //qDebug()<<"jjj"<<contours[i].x<<contours[i2].x<<contours[i3].x<<contours[j].x<<contours[j2].x<<contours[j3].x;
    return num;
}

int trangleCalc(Mat &rgb,std::vector<Point> &contours,int sideLen,int index,Rect &roi)
{
    int size=contours.size();
    int i=index,j=index;
    int i2,j2,i3,j3;
    int k=0,num=0;
    int times=5;
    int step=5;//size/4/times;
    if(step>size/4)
        return 0;
    //qDebug()<<"size"<<size<<step<<sideLen;
    while(1)
    {
        if(k>times)
            break;

        i2=i+step;
        j2=j-step;
        i3=i+2*step;
        j3=j-2*step;
        if(i>=size) i-=size;
        if(i2>=size) i2-=size;
        if(i3>=size) i3-=size;
        if(j<0) j+=size;
        if(j2<0) j2+=size;
        if(j3<0) j3+=size;
       // qDebug()<<"trangle"<<abs(common::calcAngle(contours[i],contours[i2])-common::calcAngle(contours[i2],contours[i3]))<<abs(common::calcAngle(contours[j],contours[j2])-common::calcAngle(contours[j2],contours[j3]));
   // circle( rgb, Point(contours[i].x+roi.x,contours[i].y+roi.y ), 3, Scalar(0,255,0), 1, 16);
                if(abs(common::calcAngle(contours[i],contours[i2])-common::calcAngle(contours[i2],contours[i3]))<12)
                    ++num;

                if(abs(common::calcAngle(contours[j],contours[j2])-common::calcAngle(contours[j2],contours[j3]))<12)
                    ++num;

        i+=step;
        j-=step;
        k++;
    }

    //qDebug()<<"jjj"<<i<<i2<<i3<<j<<j2<<j3;
    //qDebug()<<"jjj"<<contours[i].x<<contours[i2].x<<contours[i3].x<<contours[j].x<<contours[j2].x<<contours[j3].x;
    return num;
}
void createCpk3(QStringList list)
{
    QString fileName="D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk";
    if(QFile::exists(fileName))
    {
        return;
    }

    int cols=list.size();
    QString str;
    for(int i=0;i<cols;i++)
    {
        str+=QString("<Item R0=\"%2\">C%1</Item>").arg(i).arg(list[i]);
    }

    QString s=QString("<?xml version=\"1.0\" encoding=\"utf-8\"?><TyCPKToolsFileData><Config USL_E=\"-1\" USL_V=\"0.5\" Target_E=\"-1\" Target_V=\"0\" LSL_E=\"-1\" LSL_V=\"-0.5\" SubGroupSize=\"16\" DataGroupSize=\"0\" SampleSize=\"8\" DecimalSize=\"2\" CalculateSTDKind=\"0\"/><Cells><ColCount>%1</ColCount><RowCount>1</RowCount>%2</Cells></TyCPKToolsFileData>").arg(cols).arg(str);
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);

    f.write(s.toUtf8().data());
    f.close();
}
void clearCpk3()
{
    QDir dir("D:/DzicsRobot/CPK/");
    dir.mkdir("D:/DzicsRobot/CPK/");
    dir.setFilter(QDir::Files);
    QFileInfoList list3=dir.entryInfoList();
    if(list3.size()>20)
    {
        for(int i=0;i<list3.size();i++)
        {
            if(list3[i].fileName().right(3)=="cpk"&&list3[i].fileName().length()>10)
            {

                if(i<list3.size()/2)
                    QFile::remove("D:/DzicsRobot/CPK/"+list3[i].fileName());
            }
        }
    }
}



void addCpk3(QStringList title,QStringList list)
{
    createCpk3(title);


    QFile f1("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk");
    f1.open(QIODevice::ReadOnly);
    QRegExp rx("\\d+</RowCount");
    QRegExp rx1("\\d+");

    QString txt=QString::fromUtf8(f1.readAll());

    f1.close();
    QFile f("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk");
    f.open(QIODevice::WriteOnly);

    rx.indexIn(txt,0);

    rx1.indexIn(rx.cap(0),0);
    int count=rx1.cap(0).toInt();


    for(int i=list.size()-1;i>=0;i--)
    {

     txt=txt.replace(QString(">C%1<").arg(i),QString(" R%1=\"%2\">C%1<").arg(i).arg(list[i]));

    }
     txt=txt.replace(QString(">%1</RowCount").arg(count),QString(">%1</RowCount").arg(count+1));

    f.write(txt.toUtf8().data());
    f.close();


    clearCpk3();
}

double anlgeOffset(double angle,double offset)
{

    if(offset+angle>180)
        angle=offset+angle-360;
    else if(offset+angle<-180)
        angle=offset+angle+360;
    else
        angle+=offset;
    return angle;
}

}
