#ifndef SYSTEMINFOPLUGIN_H
#define SYSTEMINFOPLUGIN_H

#include "pluginsiteminterface.h"
#include "systeminfowidget.h"

#include <QLabel>

#define BATTERY_DISCHARED   2
#define BATTERY_FULL        4

class systeminfoPlugin : public QObject, PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "systeminfo.json")

public:
    explicit systeminfoPlugin(QObject *parent = 0);

    const QString pluginName() const;
    void init(PluginProxyInterface *proxyInter);

    QWidget *itemWidget(const QString &itemKey);
    QWidget *itemTipsWidget(const QString &itemKey);
    const QString itemCommand(const QString &itemKey);
    const QString itemContextMenu(const QString &itemKey);
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked);

private:
    void requestContextMenu(const QString &itemKey);

private:
    systeminfowidget *widget;

};

#endif // SYSTEMINFOPLUGIN_H
