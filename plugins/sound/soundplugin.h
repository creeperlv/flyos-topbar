#ifndef SOUNDPLUGIN_H
#define SOUNDPLUGIN_H

#include "pluginsiteminterface.h"
#include "sounditem.h"

#include <QObject>

class SoundPlugin : public QObject, PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "sound.json")

public:
    explicit SoundPlugin(QObject *parent = 0);

    const QString pluginName() const;
    void init(PluginProxyInterface *proxyInter);

    QWidget *itemWidget(const QString &itemKey);
    QWidget *itemTipsWidget(const QString &itemKey);
    QWidget *itemPopupApplet(const QString &itemKey);

    const QString itemContextMenu(const QString &itemKey);
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked);

private:
    SoundItem *m_soundItem;
};

#endif // SOUNDPLUGIN_H
