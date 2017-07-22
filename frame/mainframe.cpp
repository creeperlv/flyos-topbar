//
// Created by linux on 17-7-17.
//
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <QtX11Extras/QX11Info>
#include <DApplication>
#include <QDesktopWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <frame/controller/itemcontroller.h>
#include <widget/ImageButton.h>

#include "mainframe.h"


mainFrame::mainFrame(QWidget *parent) :
        DBlurEffectWidget(parent) {
    setFixedSize(qApp->desktop()->size().width(), 30);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setMaskColor(DBlurEffectWidget::LightColor);

    ImageButton *logoButton = new ImageButton(this);
    logoButton->setIcon(QPixmap(":/resources/logo.png").scaled(QSize(26,26), Qt::KeepAspectRatioByExpanding));
    logoButton->move(40, 0);

    QLabel *title = new QLabel("flyos-topbar",this);

    title->move(100, 7);

    ImageButton *settingButton = new ImageButton(":/resources/setting.png", this);

    QFrame *pluginsFrame = new QFrame(this);
    pluginsFrame->setGeometry(100, 0, qApp->desktop()->size().width() - 100, 30);


    QHBoxLayout *pluginsLayout = new QHBoxLayout(pluginsFrame);
    pluginsLayout->setContentsMargins(0,0,0,0);
    pluginsFrame->setLayout(pluginsLayout);

    pluginsLayout->addStretch();
    pluginsLayout->addSpacing(10);
    pluginsLayout->addWidget(settingButton, 0, Qt::AlignVCenter);

    ItemController *controller= ItemController::instance(this);



    connect(logoButton, &QPushButton::clicked, this, [] {
        QProcess::startDetached("dde-control-center", QStringList() << "-s" << "systeminfo");
    });

    connect(settingButton, &QPushButton::clicked, this, [] {
        QProcess::startDetached("dde-control-center", QStringList() << "show");
    });

    connect(controller, &ItemController::itemInserted, this, [=](const int index, Item *item){
        item->setVisible(true);
        item->setParent(this);
        item->setDockDisplayMode(Dock::DisplayMode::Efficient);

        pluginsLayout->insertWidget(index+1, item);
    }, Qt::DirectConnection);
    connect(controller, &ItemController::itemRemoved, this, [=](Item *item){
        pluginsLayout->removeWidget(item);
    }, Qt::DirectConnection);

}

void mainFrame::registerDockType()
{
    xcb_ewmh_connection_t m_ewmh_connection;
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, cookie, NULL);

    xcb_atom_t atoms[1];
    atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DOCK;
    xcb_ewmh_set_wm_window_type(&m_ewmh_connection, winId(), 1, atoms);
}