#include "pluginscontroller.h"
#include "pluginsiteminterface.h"
#include "itemcontroller.h"
#include "pluginloader.h"

#include <QDebug>
#include <QDir>

PluginsController::PluginsController(ItemController *itemControllerInter)
    : QObject(itemControllerInter),
      m_itemControllerInter(itemControllerInter)
{
    qApp->installEventFilter(this);

    QTimer::singleShot(1, this, &PluginsController::startLoader);
}

PluginsController::~PluginsController()
{
}

void PluginsController::itemAdded(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    // check if same item added
    if (m_pluginList.contains(itemInter))
        if (m_pluginList[itemInter].contains(itemKey))
            return;

    PluginsItem *item = new PluginsItem(itemInter, itemKey);
    item->setVisible(false);

    m_pluginList[itemInter][itemKey] = item;

    emit pluginItemInserted(item);
}

void PluginsController::itemUpdate(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    PluginsItem *item = pluginItemAt(itemInter, itemKey);

    Q_ASSERT(item);

    item->update();

    emit pluginItemUpdated(item);
}

void PluginsController::itemRemoved(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    PluginsItem *item = pluginItemAt(itemInter, itemKey);

    if (!item)
        return;

    item->detachPluginWidget();

    emit pluginItemRemoved(item);

    m_pluginList[itemInter].remove(itemKey);
    QTimer::singleShot(1, item, &PluginsItem::deleteLater);
}

//void DockPluginsController::requestRefershWindowVisible()
//{
//    for (auto list : m_pluginList.values())
//    {
//        for (auto item : list.values())
//        {
//            Q_ASSERT(item);
//            emit item->requestRefershWindowVisible();
//            return;
//        }
//    }
//}

void PluginsController::requestContextMenu(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    PluginsItem *item = pluginItemAt(itemInter, itemKey);
    Q_ASSERT(item);

}

//void DockPluginsController::requestPopupApplet(PluginsItemInterface * const itemInter, const QString &itemKey)
//{
//    PluginsItem *item = pluginItemAt(itemInter, itemKey);

//    Q_ASSERT(item);
//    item->showPopupApplet();
//}

void PluginsController::startLoader()
{
    PluginLoader *loader = new PluginLoader(this);

    connect(loader, &PluginLoader::finished, loader, &PluginLoader::deleteLater, Qt::QueuedConnection);
    connect(loader, &PluginLoader::pluginFounded, this, &PluginsController::loadPlugin, Qt::QueuedConnection);

    QTimer::singleShot(1, loader, [=] { loader->start(QThread::LowestPriority); });
}

void PluginsController::loadPlugin(const QString &pluginFile)
{
    qDebug() << "load plugin: " << pluginFile;

    QPluginLoader *pluginLoader = new QPluginLoader(pluginFile, this);
    PluginsItemInterface *interface = qobject_cast<PluginsItemInterface *>(pluginLoader->instance());
    if (!interface)
    {
        qWarning() << "load plugin failed!!!" << pluginLoader->errorString() << pluginFile;
        pluginLoader->unload();
        pluginLoader->deleteLater();
        return;
    }

    m_pluginList.insert(interface, QMap<QString, PluginsItem *>());
    qDebug() << "init plugin: " << interface->pluginName();
    interface->init(this);
    qDebug() << "init plugin finished: " << interface->pluginName();
}

PluginsItem *PluginsController::pluginItemAt(PluginsItemInterface * const itemInter, const QString &itemKey) const
{
    if (!m_pluginList.contains(itemInter))
        return nullptr;

    return m_pluginList[itemInter][itemKey];
}
