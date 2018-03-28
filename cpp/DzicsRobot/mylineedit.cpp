#include "mylineedit.h"

mylineedit::mylineedit(QWidget  *parent) :
    QLineEdit(parent)
{
}

void mylineedit::focusInEvent (QFocusEvent *e)
{

   emit clicked(index);
   QLineEdit::focusInEvent(e);

}

void mylineedit::focusOutEvent(QFocusEvent *e)
{
    emit focusOut();
    QLineEdit::focusOutEvent(e);
}
