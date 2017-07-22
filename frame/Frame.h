//
// Created by linux on 17-7-17.
//

#ifndef FLYOS_TOPBAR_FRAME_H
#define FLYOS_TOPBAR_FRAME_H


#include <QtWidgets/QFrame>

class Frame : public QFrame
{
    Q_OBJECT
public:
    Frame(QWidget *parent = 0);

    void registerDesktop();
};


#endif //FLYOS_TOPBAR_FRAME_H
