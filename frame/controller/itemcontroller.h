#ifndef DOCKITEMCONTROLLER_H
#define DOCKITEMCONTROLLER_H

#include "pluginscontroller.h"
#include "pluginsiteminterface.h"
#include "../item/item.h"
#include "../item/stretchitem.h"

#include <QObject>

class ItemController : public QObject
{
    Q_OBJECT

public:
    static ItemController *instance(QObject *parent);
    ~ItemController();

    const QList<Item *> itemList() const;

signals:
    void itemInserted(const int index, Item *item) const;
    void itemRemoved(Item *item) const;
    void itemManaged(Item *item) const;

public slots:
    void itemDroppedIntoContainer(Item * const item);
private:
    explicit ItemController(QObject *parent = 0);
    void pluginItemInserted(PluginsItem *item);
    void pluginItemRemoved(PluginsItem *item);

private:
    QList<Item *> m_itemList;

    QTimer *m_updatePluginsOrderTimer;

    PluginsController *m_pluginsInter;
    StretchItem *m_placeholderItem;

    static ItemController *INSTANCE;
};

#endif // DOCKITEMCONTROLLER_H
