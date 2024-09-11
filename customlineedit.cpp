#include "customlineedit.h"
#include <QMouseEvent>

CustomLineEdit::CustomLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

void CustomLineEdit::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
    emit clicked(); // Emit the clicked signal
}

void CustomLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    emit clicked(); // Emit the clicked signal
}
