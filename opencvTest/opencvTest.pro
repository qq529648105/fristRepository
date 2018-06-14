#-------------------------------------------------
#
# Project created by QtCreator 2016-07-11T09:57:10
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvTest
DESTDIR = ./
TEMPLATE = app

INCLUDEPATH += E:\opencv2.4.9\opencv\build\include
INCLUDEPATH += E:\opencv2.4.9\opencv\build\include\opencv
INCLUDEPATH += E:\opencv2.4.9\opencv\build\include\opencv2

#INCLUDEPATH += E:\opencv\build\include
#INCLUDEPATH += E:\opencv\build\include\opencv
#INCLUDEPATH += E:\opencv\build\include\opencv2

#Debug:LIBS += -LE:\opencv\build\x86\vc9\lib
#LIBS += opencv_calib3d249d.lib
#LIBS += opencv_contrib249d.lib
#LIBS += opencv_core249d.lib
#LIBS += opencv_features2d249d.lib
#LIBS += opencv_flann249d.lib
#LIBS += opencv_gpu249d.lib
#LIBS += opencv_highgui249d.lib
#LIBS += opencv_imgproc249d.lib
#LIBS += opencv_legacy249d.lib
#LIBS += opencv_ml249d.lib
#LIBS += opencv_objdetect249d.lib
#LIBS += opencv_ts249d.lib
#LIBS += opencv_video249d.lib


#LIBS += opencv_ml249d.lib
#LIBS += opencv_calib3d249d.lib
#LIBS += opencv_contrib249d.lib
#LIBS += opencv_core249d.lib
#LIBS += opencv_features2d249d.lib
#LIBS += opencv_flann249d.lib
#LIBS += opencv_gpu249d.lib
#LIBS += opencv_highgui249d.lib
#LIBS += opencv_imgproc249d.lib
#LIBS += opencv_legacy249d.lib
#LIBS += opencv_objdetect249d.lib
#LIBS += opencv_ts249d.lib
#LIBS += opencv_video249d.lib
#LIBS += opencv_nonfree249d.lib
#LIBS += opencv_ocl249d.lib
#LIBS += opencv_photo249d.lib
#LIBS += opencv_stitching249d.lib
#LIBS += opencv_superres249d.lib
#LIBS += opencv_videostab249d.lib

#Release:LIBS += -LE:\opencv\build\x86\vc9\lib
#LIBS += opencv_calib3d249.lib
#LIBS += opencv_contrib249.lib
#LIBS += opencv_core249.lib
#LIBS += opencv_features2d249.lib
#LIBS += opencv_flann249.lib
#LIBS += opencv_gpu249.lib
#LIBS += opencv_highgui249.lib
#LIBS += opencv_imgproc249.lib
#LIBS += opencv_legacy249.lib
#LIBS += opencv_ml249.lib
#LIBS += opencv_objdetect249.lib
#LIBS += opencv_ts249.lib
#LIBS += opencv_video249.lib
LIBS += -L. -lVisionDll -lopencv300dll
LIBS += -LE:\opencv2.4.9\opencv\build\x86\vc10\lib
LIBS += opencv_objdetect249.lib
LIBS += opencv_ts249.lib
LIBS += opencv_video249.lib
LIBS += opencv_nonfree249.lib
LIBS += opencv_ocl249.lib
LIBS += opencv_photo249.lib
LIBS += opencv_stitching249.lib
LIBS += opencv_superres249.lib
LIBS += opencv_videostab249.lib
LIBS += opencv_calib3d249.lib
LIBS += opencv_contrib249.lib
LIBS += opencv_core249.lib
LIBS += opencv_features2d249.lib
LIBS += opencv_flann249.lib
LIBS += opencv_gpu249.lib
LIBS += opencv_highgui249.lib
LIBS += opencv_imgproc249.lib
LIBS += opencv_legacy249.lib
LIBS += opencv_ml249.lib
SOURCES += main.cpp\
        mainwindow.cpp \
    project.cpp \
    common.cpp \
    project2.cpp \
    project3.cpp \
    readme.cpp \
    project1.cpp \
    mainwindow_2.cpp \
    project4.cpp

HEADERS  += mainwindow.h \
    visiondll.h \
    project.h \
    common.h \
    project2.h \
    project3.h \
    readme.h \
    project1.h \
    Global.h \
    opencv300.h \
    mainwindow_all.h \
    project4.h

FORMS    += mainwindow.ui \
    readme.ui

