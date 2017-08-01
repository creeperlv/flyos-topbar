//
// Created by linux on 17-8-1.
//

#ifndef FLYOS_TOPBAR_SHADOW_H
#define FLYOS_TOPBAR_SHADOW_H


#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QFrame>

class shadow : public QFrame{
public:
    shadow();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QGraphicsDropShadowEffect *effect;

};


#endif //FLYOS_TOPBAR_SHADOW_H
