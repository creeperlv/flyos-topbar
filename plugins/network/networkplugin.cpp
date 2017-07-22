#include "networkplugin.h"
#include "item/wireditem.h"
#include "item/wirelessitem.h"

#define WIRED_ITEM      "wired"
#define WIRELESS_ITEM   "wireless"

NetworkPlugin::NetworkPlugin(QObject *parent)
    : QObject(parent),

      m_networkManager(NetworkManager::instance(this)),
      m_refershTimer(new QTimer(this))
{
}

const QString NetworkPlugin::pluginName() const
{
    return "network";
}

void NetworkPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;

    m_refershTimer->setInterval(100);
    m_refershTimer->setSingleShot(true);

    connect(m_networkManager, &NetworkManager::networkStateChanged, this, &NetworkPlugin::networkStateChanged);
    connect(m_networkManager, &NetworkManager::deviceTypesChanged, this, &NetworkPlugin::deviceTypesChanged);
    connect(m_networkManager, &NetworkManager::deviceAdded, this, &NetworkPlugin::deviceAdded);
    connect(m_networkManager, &NetworkManager::deviceRemoved, this, &NetworkPlugin::deviceRemoved);
    connect(m_networkManager, &NetworkManager::deviceChanged, m_refershTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(m_refershTimer, &QTimer::timeout, this, &NetworkPlugin::refershDeviceItemVisible);

    m_networkManager->init();
}

void NetworkPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked)
{
    Q_UNUSED(checked)

    for (auto item : m_deviceItemList)
        if (item->path() == itemKey)
            return item->invokeMenuItem(menuId);

    Q_UNREACHABLE();
}

void NetworkPlugin::refershIcon(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    for (auto *item : m_deviceItemList)
        item->refreshIcon();
}

const QString NetworkPlugin::itemCommand(const QString &itemKey)
{
    for (auto deviceItem : m_deviceItemList)
        if (deviceItem->path() == itemKey)
            return deviceItem->itemCommand();

    Q_UNREACHABLE();
    return QString();
}

const QString NetworkPlugin::itemContextMenu(const QString &itemKey)
{
    for (auto item : m_deviceItemList)
        if (item->path() == itemKey)
            return item->itemContextMenu();

    Q_UNREACHABLE();
    return QString();
}

QWidget *NetworkPlugin::itemWidget(const QString &itemKey)
{
    for (auto deviceItem : m_deviceItemList)
        if (deviceItem->path() == itemKey)
        {
            return deviceItem;
        }

    return nullptr;
}

QWidget *NetworkPlugin::itemTipsWidget(const QString &itemKey)
{
    for (auto deviceItem : m_deviceItemList)
        if (deviceItem->path() == itemKey)
            return deviceItem->itemPopup();

    return nullptr;
}

QWidget *NetworkPlugin::itemPopupApplet(const QString &itemKey)
{
    for (auto deviceItem : m_deviceItemList)
        if (deviceItem->path() == itemKey)
            return deviceItem->itemApplet();

    return nullptr;
}

void NetworkPlugin::deviceAdded(const NetworkDevice &device)
{
    DeviceItem *item = nullptr;
    switch (device.type())
    {
    case NetworkDevice::Wired:      item = new WiredItem(device.path());        break;
    case NetworkDevice::Wireless:   item = new WirelessItem(device.path());     break;
    default:;
    }

    if (!item)
        return;
    connect(item, &DeviceItem::requestContextMenu, this, &NetworkPlugin::contextMenuRequested);

    m_deviceItemList.append(item);
    m_refershTimer->start();
}

void NetworkPlugin::deviceRemoved(const NetworkDevice &device)
{
    const auto item = std::find_if(m_deviceItemList.begin(), m_deviceItemList.end(),
                                   [&] (DeviceItem *dev) {return device == dev->path();});

    if (item == m_deviceItemList.cend())
        return;

    m_proxyInter->itemRemoved(this, (*item)->path());
    (*item)->deleteLater();
    m_deviceItemList.erase(item);
}

void NetworkPlugin::networkStateChanged(const NetworkDevice::NetworkTypes &states)
{
    Q_UNUSED(states)

    m_refershTimer->start();
}

void NetworkPlugin::deviceTypesChanged(const NetworkDevice::NetworkTypes &types)
{
    Q_UNUSED(types)

    m_refershTimer->start();
}

void NetworkPlugin::refershDeviceItemVisible()
{
    const NetworkDevice::NetworkTypes types = m_networkManager->types();
    const bool hasWiredDevice = types.testFlag(NetworkDevice::Wired);
    const bool hasWirelessDevice = types.testFlag(NetworkDevice::Wireless);

//    qDebug() << hasWiredDevice << hasWirelessDevice;

    for (auto item : m_deviceItemList)
    {
        switch (item->type())
        {
        case NetworkDevice::Wireless:
            m_proxyInter->itemAdded(this, item->path());
            break;

        case NetworkDevice::Wired:
            if (hasWiredDevice && (item->state() == NetworkDevice::Activated || !hasWirelessDevice))
                m_proxyInter->itemAdded(this, item->path());
            else
                m_proxyInter->itemRemoved(this, item->path());
            break;

        default:
            Q_UNREACHABLE();
        }
    }
}

void NetworkPlugin::contextMenuRequested()
{
    DeviceItem *item = qobject_cast<DeviceItem *>(sender());
    Q_ASSERT(item);

    m_proxyInter->requestContextMenu(this, item->path());
}
