//
// Created by linux on 17-7-17.
//
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <QtX11Extras/QX11Info>
#include <DApplication>
#include <QDesktopWidget>
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
    logoButton->move(30, 0);

    QString distroid;
    QFile file("/etc/os-release");
    if(file.open(QIODevice::ReadOnly)){
        distroid = file.readLine();
    }
    if (distroid.contains("Deepin")) {
        logoButton->setIcon(QIcon::fromTheme("dde").pixmap(26,26));
    }
    else {
        logoButton->setIcon(QPixmap(":/resources/logo.png").scaled(QSize(26, 26), Qt::KeepAspectRatioByExpanding));
    }


    title = new QLabel("flyos-topbar", this);
    title->setStyleSheet("color:#2C3035;"
                                 "text-align:center;"
                                 "font-size:16px;"
                                 "font-weight:bold;");
    title->setGeometry(100, 0, 100, 30);

    ImageButton *settingButton = new ImageButton(":/resources/setting.png", this);

    QFrame *pluginsFrame = new QFrame(this);
    pluginsFrame->setGeometry(100, 0, qApp->desktop()->size().width() - 100, 30);


    QHBoxLayout *pluginsLayout = new QHBoxLayout(pluginsFrame);
    pluginsLayout->setContentsMargins(0, 0, 0, 0);
    pluginsFrame->setLayout(pluginsLayout);

    pluginsLayout->addStretch();
    pluginsLayout->addSpacing(10);
    pluginsLayout->addWidget(settingButton, 0, Qt::AlignVCenter);


    dBusDock = new DBusDock(this);

    connect(dBusDock, &DBusDock::EntryAdded, this, &mainFrame::EntryAdded);
    connect(dBusDock, &DBusDock::EntryRemoved, this, &mainFrame::EntryRemoved);

    for (QDBusObjectPath entryPath : dBusDock->entries()) {
        DBusDockEntry *entry = new DBusDockEntry(entryPath.path());
        entryList << entry;
        connect(entry, &DBusDockEntry::ActiveChanged, this, &mainFrame::ActiveChanged, Qt::UniqueConnection);

    }

//  加载插件
    ItemController *controller = ItemController::instance(this);


    connect(logoButton, &QPushButton::clicked, this, [] {
        QProcess::startDetached("dde-control-center", QStringList() << "-s" << "systeminfo");
    });

    connect(settingButton, &QPushButton::clicked, this, [] {
        QProcess::startDetached("dde-control-center", QStringList() << "show");
    });

    connect(controller, &ItemController::itemInserted, this, [=](const int index, Item *item) {
        item->setVisible(true);
        item->setParent(this);
        item->setDockDisplayMode(Dock::DisplayMode::Efficient);

        pluginsLayout->insertWidget(index + 1, item);
    }, Qt::DirectConnection);
    connect(controller, &ItemController::itemRemoved, this, [=](Item *item) {
        pluginsLayout->removeWidget(item);
    }, Qt::DirectConnection);

}


void mainFrame::EntryAdded(const QDBusObjectPath &entryPath, const int index) {
    DBusDockEntry *entry = new DBusDockEntry(entryPath.path());
    entryList << entry;
    connect(entry, &DBusDockEntry::ActiveChanged, this, &mainFrame::ActiveChanged, Qt::UniqueConnection);

    ActiveChanged();
}

void mainFrame::EntryRemoved(const QString &entryId) {
    for (int i = 0; i < entryList.count(); i++) {
        if (entryList[i]->id() == entryId) {
            DBusDockEntry *entry = entryList[i];
            entryList.removeAt(i);
            entry->disconnect();
            entry->deleteLater();
            break;
        }
    }
    ActiveChanged();

}

void mainFrame::ActiveChanged() {
    QString name = " FlyOS";
    for (DBusDockEntry *entry : entryList) {
        if (entry->active()) {
            name = entry->name();
            title->setText(name);
            title->setFixedWidth(title->fontMetrics().width(name));
            return;
        }
    }

    title->setText(name);
    title->setFixedWidth(title->fontMetrics().width(name));
}

void mainFrame::registerDockType() {
    QRect screen = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen());
    resize(screen.width(), 30);
    move(screen.x(), 0);

    xcb_ewmh_connection_t m_ewmh_connection;
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, cookie, NULL);

    xcb_atom_t atoms[2];
    atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DOCK;
    atoms[1] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DESKTOP;
    xcb_ewmh_set_wm_window_type(&m_ewmh_connection, winId(), 1, atoms);

    xcb_ewmh_wm_strut_partial_t partial;
    memset(&partial, 0, sizeof(xcb_ewmh_wm_strut_partial_t));

    const QPoint p(screen.x(), 0);
    const QRect r = QRect(p, size());

    partial.top = r.bottom() + 2;
    partial.top_start_x = r.left();
    partial.top_end_x = r.right();

    xcb_ewmh_set_wm_strut_partial(&m_ewmh_connection, winId(), partial);
}


