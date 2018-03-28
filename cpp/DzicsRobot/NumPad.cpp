#include "NumPad.h"
#include "ui_NumPad.h"


#ifndef WIN32
    #include <X11/extensions/XTest.h>
    #include <X11/keysym.h>
#endif

#include <QKeyEvent>


NumPad::NumPad(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::NumPad)
{
    m_ui->setupUi(this);
    infoMap.clear();
    butonMap.clear();

    #ifndef WIN32
        Display* disp = XOpenDisplay(NULL);
        if(!getNumlockState(disp)) {
            XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, XK_Num_Lock), True, 0);
            XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, XK_Num_Lock), False, 0);
        }
        XCloseDisplay(disp);
    #endif
}

NumPad::~NumPad()
{

    delete m_ui;
}

void NumPad::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NumPad::keyPressEvent ( QKeyEvent * e )
{
    if (e->key() == Qt::Key_NumLock) {
        if (!m_ui->numLock->isEnabled()) return;
        m_ui->numLock->setDown(true);
        return;
    }
    if (e->modifiers() == Qt::KeypadModifier) {     // 小键盘按钮
        QPushButton *optButton = getCurrentButton((Qt::Key)e->key());
        if(!optButton)
            return;
        if(!optButton->isEnabled())
            return;
        optButton->setDown(true);
    }
}


#ifndef WIN32
    int NumPad::getNumlockState(Display* disp)
    {
        int i;
        int numlock_mask = 0;
        Window dummy1, dummy2;
        int dummy3, dummy4, dummy5, dummy6;
        unsigned int mask;
        XModifierKeymap* map = XGetModifierMapping( disp );

        KeyCode numlock_keycode = XKeysymToKeycode( disp, XK_Num_Lock );
        if( numlock_keycode == NoSymbol )
            return 0;
        for( i = 0; i < 8; ++i ) {
            if( map->modifiermap[map->max_keypermod * i ] == numlock_keycode)
                numlock_mask = 1 <<i;
        }
        XQueryPointer( disp,DefaultRootWindow( disp ), &dummy1,&dummy2,
            &dummy3, &dummy4, &dummy5, &dummy6,&mask );
        XFreeModifiermap( map );
        return mask & numlock_mask;
    }
#endif

void NumPad::keyReleaseEvent (QKeyEvent * e )
{
    if (e->key() == Qt::Key_NumLock) {
         #ifndef WIN32
             Display* disp = XOpenDisplay(NULL);
             if(!getNumlockState(disp)) {
                 XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, XK_Num_Lock), True, 0);
                 XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, XK_Num_Lock), False, 0);

                 XCloseDisplay(disp);
                 return;
             }
             XCloseDisplay(disp);
         #endif

         if (!m_ui->numLock->isEnabled()) return;
         m_ui->numLock->setDown(false);
         m_ui->numLock->click();
         return;
     }
     if (e->modifiers() == Qt::KeypadModifier) { // 小键盘按钮
         QPushButton *optButton = getCurrentButton((Qt::Key)e->key());
         if(!optButton)
             return;
         if(!optButton->isEnabled())
             return;
         optButton->setDown(false);
         optButton->click();
     }
}

void NumPad::on_num0_clicked() { emit processKey(getKeyId(Qt::Key_0)); }
void NumPad::on_num1_clicked() { emit processKey(getKeyId(Qt::Key_1)); }
void NumPad::on_num2_clicked() { emit processKey(getKeyId(Qt::Key_2)); }
void NumPad::on_num3_clicked() { emit processKey(getKeyId(Qt::Key_3)); }
void NumPad::on_num4_clicked() { emit processKey(getKeyId(Qt::Key_4)); }
void NumPad::on_num5_clicked() { emit processKey(getKeyId(Qt::Key_5)); }
void NumPad::on_num6_clicked() { emit processKey(getKeyId(Qt::Key_6)); }
void NumPad::on_num7_clicked() { emit processKey(getKeyId(Qt::Key_7)); }
void NumPad::on_num8_clicked() { emit processKey(getKeyId(Qt::Key_8)); }
void NumPad::on_num9_clicked() { emit processKey(getKeyId(Qt::Key_9)); }
void NumPad::on_numLock_clicked() { emit processKey(getKeyId(Qt::Key_NumLock)); }
void NumPad::on_numSlash_clicked() { emit processKey(getKeyId(Qt::Key_Slash)); }
void NumPad::on_numAsterisk_clicked() { emit processKey(getKeyId(Qt::Key_Asterisk)); }
void NumPad::on_numMinus_clicked() { emit processKey(getKeyId(Qt::Key_Minus)); }
void NumPad::on_numPlus_clicked() { emit processKey(getKeyId(Qt::Key_Plus)); }
void NumPad::on_numEnter_clicked() { emit processKey(getKeyId(Qt::Key_Enter)); }
void NumPad::on_numPeriod_clicked() { emit processKey(getKeyId(Qt::Key_Period)); }



void NumPad::registerKey(int id, Qt::Key key, QString text, QColor color, bool isShow)
{
    KEYINFO idInfo;
    idInfo.key = key;
    idInfo.text = text;
    idInfo.color = color;
    idInfo.enabled = true;
    infoMap.insert(id, idInfo);
    if (isShow) {
        showKey(id);
    }
}


void NumPad::removeKey(int id)
{
    if(infoMap.contains(id))
        infoMap.remove(id);

}

void NumPad::showKey(int id)
{
    if(!infoMap.contains(id))
        return;
    KEYINFO info = infoMap[id];
    butonMap.insert(info.key, id);
    QPushButton*optButton = getCurrentButton(info.key);
    if(optButton)
    {
        optButton->setText(info.text);
        QPalette pal;
        pal.setColor(QPalette::ButtonText, info.color);
        optButton->setPalette(pal);
        optButton->setEnabled(info.enabled);
    }
}

void NumPad::enableKey(int id, bool enabled)
{
    if(!infoMap.contains(id))
        return;
    Qt::Key key = infoMap[id].key;
    infoMap[id].enabled = enabled;
    if(!butonMap.contains(key))
        return;
    if(butonMap[key]!= id)
        return;
    QPushButton *optButton = getCurrentButton(key);
    if(optButton)
        optButton->setEnabled(enabled);
}


bool NumPad::isKeyEnable(int id)
{
    if(!infoMap.contains(id))
        return false;
    bool enabled = infoMap[id].enabled;
    return enabled;
}

bool NumPad::isKeyShow(int id)
{
    Qt::Key key;
    int curString;
    if(!infoMap.contains(id))
        return false;
    key =  infoMap[id].key;
    if(!butonMap.contains(key))
        return false;
    curString = butonMap[key];
    if(curString == id)
        return true;
    else
        return false;
}


void NumPad::hideAllKey()
{
    for(int i = 0; i<butonMap.keys().count(); ++i)
    {
        QPushButton *optButton = getCurrentButton(butonMap.keys().at(i));
        optButton->setText("");
        optButton->setEnabled(false);
    }
    butonMap.clear();
}



void NumPad::hideKey(int id)
{
    if(!infoMap.contains(id))
        return;
    Qt::Key key = infoMap[id].key;
    if(butonMap[key] != id)
        return;
    butonMap.remove(key);
    QPushButton*optButton = getCurrentButton(key);
    if(optButton)
    {
        optButton->setText("");
        optButton->setEnabled(false);
    }
}

int NumPad::getKeyId(Qt::Key key)
{
    return butonMap[key];

}

QString NumPad::getKeyText(Qt::Key key)
{
    QPushButton *optButton = getCurrentButton(key);
    if(optButton)
        return optButton->text();
    else
        return "";
}

void NumPad::lockKey()
{

    //m_ui->numSlash->setEnabled(false);
    //m_ui->numAsterisk->setEnabled(false);
    //m_ui->numMinus->setEnabled(false);
    //m_ui->numPlus->setEnabled(false);
    //m_ui->numPeriod->setEnabled(false);
    //m_ui->num0->setEnabled(false);
    m_ui->num1->setEnabled(false);
    m_ui->num2->setEnabled(false);
    //m_ui->num3->setEnabled(false);
    m_ui->num4->setEnabled(false);
    m_ui->num7->setEnabled(false);
    m_ui->num8->setEnabled(false);

}

void NumPad::unLockKey()
{

    m_ui->numSlash->setEnabled(true);
    m_ui->numAsterisk->setEnabled(true);
    m_ui->numMinus->setEnabled(true);
    m_ui->numPlus->setEnabled(true);
    m_ui->numPeriod->setEnabled(true);
    m_ui->num0->setEnabled(true);
    m_ui->num1->setEnabled(true);
    m_ui->num2->setEnabled(true);
    m_ui->num3->setEnabled(true);
    m_ui->num4->setEnabled(true);
    m_ui->num7->setEnabled(true);
    m_ui->num8->setEnabled(true);

}


QPushButton* NumPad::getCurrentButton(Qt::Key key)
{
    switch(key)
    {
    case Qt::Key_NumLock:
        return m_ui->numLock;
    case Qt::Key_Slash:
        return m_ui->numSlash;
    case Qt::Key_Asterisk:
        return m_ui->numAsterisk;
    case Qt::Key_Minus:
        return m_ui->numMinus;
    case Qt::Key_Plus:
        return m_ui->numPlus;
    case Qt::Key_Enter:
        return m_ui->numEnter;
    case Qt::Key_Period:
    case Qt::Key_Delete:
        return m_ui->numPeriod;
    case Qt::Key_0:
    case Qt::Key_Insert:
        return m_ui->num0;
    case Qt::Key_End:
    case Qt::Key_1:
        return m_ui->num1;
    case Qt::Key_Down:
    case Qt::Key_2:
        return m_ui->num2;
    case Qt::Key_PageDown:
    case Qt::Key_3:
        return m_ui->num3;
    case Qt::Key_Left:
    case Qt::Key_4:
        return m_ui->num4;
    case Qt::Key_Clear:
    case Qt::Key_5:
        return m_ui->num5;
    case Qt::Key_Right:
    case Qt::Key_6:
        return m_ui->num6;
    case Qt::Key_Home:
    case Qt::Key_7:
        return m_ui->num7;
    case Qt::Key_Up:
    case Qt::Key_8:
        return  m_ui->num8;
    case Qt::Key_PageUp:
    case Qt::Key_9:
        return  m_ui->num9;
    default:
        return NULL;
    }

}

