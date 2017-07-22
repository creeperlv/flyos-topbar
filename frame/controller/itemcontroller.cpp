#include "itemcontroller.h"

#include <QDebug>

ItemController *ItemController::INSTANCE = nullptr;

ItemController *ItemController::instance(QObject *parent) {
    if (!INSTANCE)
        INSTANCE = new ItemController(parent);

    return INSTANCE;
}

ItemController::~ItemController() {
    qDeleteAll(m_itemList);
}

const QList<Item *> ItemController::itemList() const {
    return m_itemList;
}

ItemController::ItemController(QObject *parent)
        : QObject(parent),

          m_updatePluginsOrderTimer(new QTimer(this)),

          m_pluginsInter(new PluginsController(this)),
          m_placeholderItem(new StretchItem) {
//    m_placeholderItem->hide();

    m_updatePluginsOrderTimer->setSingleShot(true);
    m_updatePluginsOrderTimer->setInterval(1000);


    connect(m_pluginsInter, &PluginsController::pluginItemInserted, this, &ItemController::pluginItemInserted,
            Qt::QueuedConnection);
    connect(m_pluginsInter, &PluginsController::pluginItemRemoved, this, &ItemController::pluginItemRemoved,
            Qt::QueuedConnection);

    QMetaObject::invokeMethod(this, "refershItemsIcon", Qt::QueuedConnection);
}

void ItemController::pluginItemInserted(PluginsItem *item) {
    // check item is in container
    if (item->allowContainer() && item->isInContainer())
    {
        emit itemManaged(item);
        return itemDroppedIntoContainer(item);
    }

    // find first plugins item position
    int firstPluginPosition = -1;
    for (int i(0); i != m_itemList.size(); ++i) {
        if (m_itemList[i]->itemType() != Item::Plugins)
            continue;

        firstPluginPosition = i;
        break;
    }
    if (firstPluginPosition == -1)
        firstPluginPosition = m_itemList.size();

    // find insert position
    int insertIndex = 0;
    const int itemSortKey = item->itemSortKey();
    if (itemSortKey == -1 || firstPluginPosition == -1) {
        insertIndex = m_itemList.size();
    } else if (itemSortKey == 0) {
        insertIndex = firstPluginPosition;
    } else {
        insertIndex = m_itemList.size();
        for (int i(firstPluginPosition); i != m_itemList.size(); ++i) {
            PluginsItem *pItem = static_cast<PluginsItem *>(m_itemList[i]);
            Q_ASSERT(pItem);

            if (itemSortKey <= pItem->itemSortKey())
                continue;
            insertIndex = i;
            break;
        }
    }

//    qDebug() << insertIndex << item;

    m_itemList.insert(insertIndex, item);
    emit itemInserted(insertIndex, item);
}

void ItemController::pluginItemRemoved(PluginsItem *item) {
    emit itemRemoved(item);

    m_itemList.removeOne(item);
}

void ItemController::itemDroppedIntoContainer(Item *const item) {
    Q_ASSERT(item->itemType() == Item::Plugins);

    PluginsItem *pi = static_cast<PluginsItem *>(item);

    if (!pi->allowContainer())
        return;

//    qDebug() << "drag into container" << item;

    // remove from main panel
    emit itemRemoved(item);
    m_itemList.removeOne(item);

    // add to container
    pi->setInContainer(true);
}
