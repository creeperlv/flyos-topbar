#ifndef STRETCHITEM_H
#define STRETCHITEM_H

#include "item.h"

class StretchItem : public Item
{
    Q_OBJECT

public:
    explicit StretchItem(QWidget *parent = 0);

    inline ItemType itemType() const {return Stretch;}

private:
    void mousePressEvent(QMouseEvent *e);
};

#endif // STRETCHITEM_H
