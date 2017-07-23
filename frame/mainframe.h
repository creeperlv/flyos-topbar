//
// Created by linux on 17-7-17.
//

#ifndef FLYOS_TOPBAR_MAINFRAME_H
#define FLYOS_TOPBAR_MAINFRAME_H

#include <DBlurEffectWidget>
#include <frame/dbus/dbusdock.h>
#include <frame/dbus/dbusdockentry.h>
#include <QtWidgets/QLabel>

DWIDGET_USE_NAMESPACE

class mainFrame : public DBlurEffectWidget {

    Q_OBJECT

public:
    mainFrame(QWidget *parent = 0);

    void registerDockType();

private:
    DBusDock *dBusDock;
    QLabel *title;

    QList<DBusDockEntry *> entryList;

private slots:
    void EntryAdded(const QDBusObjectPath &entryPath, const int index);
    void EntryRemoved(const QString &entryId);

    void ActiveChanged();
};


#endif //FLYOS_TOPBAR_MAINFRAME_H
