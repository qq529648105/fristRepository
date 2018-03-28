#ifndef NUMPAD_H
#define NUMPAD_H

#include <QtGui/QWidget>
#include <QMap>
#include <QColor>
#include <QString>
#include <QPushButton>

typedef struct _KEYINFO
{
    Qt::Key  key;
    QString  text;
    QColor  color;
    bool   enabled;
}KEYINFO;

namespace Ui {
    class NumPad;
}

class NumPad : public QWidget {
    Q_OBJECT
public:
    NumPad(QWidget *parent = 0);
    ~NumPad();
    void keyPressEvent( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );

    void registerKey(int id, Qt::Key key, QString text, QColor color = Qt::black, bool isShow = true);
    void removeKey(int id );
    void showKey(int id);
    bool isKeyShow(int id);
    void enableKey(int id, bool enabled);
    bool isKeyEnable(int id);
    void hideKey(int id);
    void hideAllKey();
    int getKeyId(Qt::Key key);
    QString getKeyText(Qt::Key key);
    void lockKey();
    void unLockKey();

protected:
    void changeEvent(QEvent *e);


private:
    Ui::NumPad *m_ui;
    QPushButton *getCurrentButton(Qt::Key);

    QMap<int, KEYINFO> infoMap;
    QMap<Qt::Key, int> butonMap;

#ifndef WIN32
    int getNumlockState(Display* disp);
#endif

private slots:
   void on_num0_clicked();
    void on_num1_clicked();
    void on_num2_clicked();
    void on_num3_clicked();
    void on_num4_clicked();
    void on_num5_clicked();
    void on_num6_clicked();
    void on_num7_clicked();
    void on_num8_clicked();
    void on_num9_clicked();
    void on_numLock_clicked();
    void on_numSlash_clicked();
    void on_numAsterisk_clicked();
    void on_numMinus_clicked();
    void on_numPlus_clicked();
    void on_numEnter_clicked();
    void on_numPeriod_clicked();

signals:
    void processKey(int id);

};

#endif // NUMPAD_H
