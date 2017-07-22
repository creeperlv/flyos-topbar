#include "stretchitem.h"

#include <QPaintEvent>

StretchItem::StretchItem(QWidget *parent)
    : Item(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void StretchItem::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}
