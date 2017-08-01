//
// Created by linux on 17-8-1.
//

#include <QApplication>
#include <QDesktopWidget>
#include <xcb/xcb_ewmh.h>
#include <QtX11Extras/QX11Info>
#include <QtGui/QPainter>
#include <QtGui/QtGui>
#include "shadow.h"

shadow::shadow() {
    this->setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    setFixedSize(qApp->desktop()->size().width(), 45);

    xcb_ewmh_connection_t m_ewmh_connection;
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, cookie, NULL);

    xcb_atom_t atoms[1];
    atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DESKTOP;
    xcb_ewmh_set_wm_window_type(&m_ewmh_connection, winId(), 1, atoms);
}

void shadow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawLine(0,31+i,this->width(),31+i);
    }
}
