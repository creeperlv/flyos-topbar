//
// Created by linux on 17-7-17.
//

#ifndef FLYOS_TOPBAR_MAINFRAME_H
#define FLYOS_TOPBAR_MAINFRAME_H

#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE

class mainFrame : public DBlurEffectWidget {

    Q_OBJECT

public:
    mainFrame(QWidget *parent = 0);

    void registerDockType();


};


#endif //FLYOS_TOPBAR_MAINFRAME_H
