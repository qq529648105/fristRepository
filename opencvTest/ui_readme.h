/********************************************************************************
** Form generated from reading UI file 'readme.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_README_H
#define UI_README_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_readMe
{
public:
    QWidget *centralwidget;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *readMe)
    {
        if (readMe->objectName().isEmpty())
            readMe->setObjectName(QString::fromUtf8("readMe"));
        readMe->resize(800, 600);
        centralwidget = new QWidget(readMe);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(20, 10, 751, 531));
        readMe->setCentralWidget(centralwidget);
        menubar = new QMenuBar(readMe);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 30));
        readMe->setMenuBar(menubar);
        statusbar = new QStatusBar(readMe);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        readMe->setStatusBar(statusbar);

        retranslateUi(readMe);

        QMetaObject::connectSlotsByName(readMe);
    } // setupUi

    void retranslateUi(QMainWindow *readMe)
    {
        readMe->setWindowTitle(QApplication::translate("readMe", "A\347\272\277\346\255\243\345\217\215\350\257\264\346\230\216", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("readMe", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">A\347\272\277\346\255\243\345\217\215\346\257\224\345\257\271\351\207\207\347\224\250\346\250\241\346\235\277\345\214\271\351\205\215\357\274\214\346\255\243\351\235\242\345\222\214\345\217\215\351\235\242\345\210\206\345\210\253\350\277\233\350\241\214\347\233\270\344\274\274\345\272\246\346\257\224\350\276\203\357\274\214\345\222\214\346\211\200\346\234\211\346\250\241\346\235\277\345\233\276\346\257\224\345\257\271\357\274\214\350\277\224\345\233\236\344\270\200\344\270\252\345\214\271\351\205\215\347\273\223\346\236\234\346\234\200"
                        "\345\245\275\347\232\204\345\200\274(\345\200\274\350\266\212\345\260\217\347\233\270\344\274\274\345\272\246\350\266\212\351\253\230)\357\274\214\346\234\200\347\273\210\346\255\243\351\235\242\345\222\214\345\217\215\351\235\242\345\257\271\346\257\224\357\274\214\345\200\274\350\276\203\345\260\217\347\232\204\346\230\257\346\234\211\345\255\227\344\270\200\351\235\242\343\200\202\345\234\250\345\210\266\344\275\234\346\250\241\346\235\277\346\227\266\357\274\214\344\277\235\346\214\201\346\234\211\345\255\227\344\270\200\351\235\242\350\277\224\345\233\236\345\200\274&lt;=300,\346\227\240\345\255\227\344\270\200\351\235\242\350\277\224\345\233\236\345\200\274&gt;300</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">\346\250\241\346\235\277\345\233\276\344\275\215\347\275\256\357\274\232D:\\DzicsRobot\\temp\\</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:"
                        "0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">1.\345\244\215\345\210\266\347\216\260\345\234\272\345\233\276\347\211\207\357\274\214\351\207\214\351\235\242\345\214\205\345\220\253\344\272\206\346\255\243\345\217\215\346\211\200\346\234\211\345\233\276\347\211\207\343\200\202\345\234\250\350\276\223\345\205\245\346\241\206\346\212\212\350\267\257\345\276\204\345\244\215\345\210\266\350\277\233\345\216\273\357\274\214\347\202\271\345\207\273[1\345\210\240\351\231\244\345\244\232\344\275\231\345\233\276\347\211\207]\357\274\214\345\217\257\344\273\245\344\277\235\350\257\201\345\233\276\347\211\207\346\255\243\351\235\242\345\222\214\345\217\215\351\235\242\346\225\260\351\207\217\344\270\200\350\207\264\343\200\202</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0"
                        "px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">2.\347\202\271\345\207\273[2\345\217\215\350\275\254\345\214\271\351\205\215\345\210\206\347\246\273]\357\274\214\344\274\232\346\212\212\346\255\243\345\217\215\345\233\276\347\211\207\345\210\206\347\246\273\345\207\272\346\235\245\343\200\202\344\277\235\345\255\230\345\234\250\345\275\223\345\211\215\347\250\213\345\272\217\350\267\257\345\276\204\\image\\\345\275\223\345\211\215\346\227\245\346\234\237\\\343\200\202\345\246\202\346\236\234\344\270\244\344\270\252\346\226\207\344\273\266\345\244\271\346\226\207\344\273\266\346\225\260\351\207\217\344\270\215\344\270\200\346\240\267\343\200\202\350\257\264\346\230\216\346\234\211\345\255\227\344\270\200\351\235\242\347\232\204\345\200\274\346\234\211\345\244\247\344\272\216300\346\210\226\350\200\205"
                        "\346\227\240\345\255\227\344\270\200\351\235\242\347\232\204\345\200\274\345\260\217\344\272\216300</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">\343\200\202</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">3.\345\210\206\345\210\253\351\200\211\346\213\251\344\270\244\344\270\252\346\226\207\344\273\266\345\244\271\345\271\266\346\212\212\350\267\257\345\276\204\345\244\215\345\210\266\345\210\260\350\276\223\345\205\245\346\241\206\343\200\202\347\202\271\345\207\273[3\345\217\215\350\275\254\347\233\270\350\277\236]\357\274\214\344\274\232\346\212\212\346\226\207\344\273\266\345\244\271\346\255\243\345\217\215\346\267\267\345\220\210\345\234\250\344\270\200\350\265\267\347\232\204\345\233\276\347\211\207\345\210\240\351\231\244\345\271\266\345\244\215\345\210\266\345\210"
                        "\260image\\\344\270\213\343\200\202</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">4.\345\217\257\345\257\271\346\217\220\345\217\226\345\207\272\346\235\245\347\232\204\345\233\276\347\211\207\350\277\233\350\241\214\346\250\241\346\235\277\345\210\266\344\275\234,\345\205\210\347\202\271\345\207\273[loadImages]\344\270\200\346\254\241\343\200\202\347\202\271\345\207\273[\345\267\245\344\273\266\350\277\224\350\275\254]\345\217\257\344\273\245\346\265\213\350\257\225\357\274\214\345\233\276\347\211\207\344\270\212\344\274\232\346\230\276\347\244\272\345\214\271\351\205\215\345\200\274\343\200\202</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-inden"
                        "t:0; text-indent:0px; font-size:12pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">5.\346\234\200\345\220\216\350\246\201\344\277\235\350\257\201\346\211\200\346\234\211\346\227\240\345\255\227\344\270\200\351\235\242\345\214\271\351\205\215\345\272\246\344\270\215\350\203\275\344\275\216\344\272\216300\343\200\202\345\244\215\345\210\266\346\227\240\345\255\227\344\270\200\351\235\242\346\226\207\344\273\266\345\244\271\350\267\257\345\276\204\350\207\263\350\276\223\345\205\245\346\241\206\343\200\202\347\202\271\345\207\273\345\217\215\350\275\254\345\214\271\351\205\215&lt;=300\343\200\202\345\246\202\346\236\234\345\217\263\344\270\212\350\247\222*\345\217\267\345\217\230\346\210\220\345\233\276\347\211\207\345\220\215(xxx.png)\345\210\231\350\257\245\346\250\241\346\235\277\351\234\200\350\246\201\351\207\215\346\226\260\345\244\204\347\220\206\343\200\202\346\226\271\346\263\2251"
                        "\357\274\232\345\210\240\346\216\211\343\200\202\346\226\271\346\263\2252\357\274\232\345\234\250\346\250\241\346\235\277\345\233\276\344\270\212\346\211\213\345\212\250\347\224\273\345\255\227\344\275\223\343\200\202</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt;\"><br /></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class readMe: public Ui_readMe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_README_H
