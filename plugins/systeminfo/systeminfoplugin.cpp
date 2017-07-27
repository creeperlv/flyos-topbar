#include "systeminfoplugin.h"

systeminfoPlugin::systeminfoPlugin(QObject *parent)
    : QObject(parent)
{
    widget = new systeminfowidget;
}

const QString systeminfoPlugin::pluginName() const
{
    return "系统信息";
}

QWidget *systeminfoPlugin::itemWidget(const QString &itemKey)
{
    return widget;
}

QWidget *systeminfoPlugin::itemTipsWidget(const QString &itemKey)
{
    QLabel *label =new QLabel("systeminfo");
    label->setStyleSheet("color:white");
    return label;
}

void systeminfoPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;

    proxyInter->itemAdded(this, "");
}

const QString systeminfoPlugin::itemCommand(const QString &itemKey)
{
    return "";
}

const QString systeminfoPlugin::itemContextMenu(const QString &itemKey)
{
    return "";
}

void systeminfoPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked)
{

}


void systeminfoPlugin::requestContextMenu(const QString &itemKey)
{
    m_proxyInter->requestContextMenu(this, itemKey);
}
