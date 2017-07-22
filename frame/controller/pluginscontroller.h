#ifndef PLUGINSCONTROLLER_H
#define PLUGINSCONTROLLER_H

#include "../item/pluginsitem.h"
#include "pluginproxyinterface.h"

#include <QPluginLoader>
#include <QList>
#include <QMap>

class ItemController;
class PluginsItemInterface;
class PluginsController : public QObject, PluginProxyInterface
{
    Q_OBJECT

public:
    explicit PluginsController(ItemController *itemControllerInter = 0);
    ~PluginsController();

    // implements PluginProxyInterface
    void itemAdded(PluginsItemInterface * const itemInter, const QString &itemKey);
    void itemUpdate(PluginsItemInterface * const itemInter, const QString &itemKey);
    void itemRemoved(PluginsItemInterface * const itemInter, const QString &itemKey);
    void requestContextMenu(PluginsItemInterface * const itemInter, const QString &itemKey);

signals:
    void pluginItemInserted(PluginsItem *pluginItem) const;
    void pluginItemRemoved(PluginsItem *pluginItem) const;
    void pluginItemUpdated(PluginsItem *pluginItem) const;

private slots:
    void startLoader();
    void loadPlugin(const QString &pluginFile);

private:
    PluginsItem *pluginItemAt(PluginsItemInterface * const itemInter, const QString &itemKey) const;

private:
//    QList<PluginsItemInterface *> m_pluginsInterfaceList;
//    QList<QPluginLoader *> m_pluginLoaderList;
    QMap<PluginsItemInterface *, QMap<QString, PluginsItem *>> m_pluginList;
    ItemController *m_itemControllerInter;
};

#endif // PLUGINSCONTROLLER_H
