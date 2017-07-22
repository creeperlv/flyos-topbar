
#include "item.h"

#include <QMouseEvent>
#include <QJsonObject>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusPendingReply>
#include <frame/dbus/dbusmenu.h>

//Position DockItem::DockPosition = Position::Top;
DisplayMode Item::displayMode = DisplayMode::Efficient;
std::unique_ptr<ItemPopupWindow> Item::PopupWindow(nullptr);

Item::Item(QWidget *parent)
    : QWidget(parent),
      m_hover(false),
      m_popupShown(false),

      m_popupTipsDelayTimer(new QTimer(this)),
      m_popupAdjustDelayTimer(new QTimer(this)),

      m_menuManagerInter(new DBusMenuManager(this))
{
    if (!PopupWindow.get())
    {
        ItemPopupWindow *arrowRectangle = new ItemPopupWindow(nullptr);
        arrowRectangle->setShadowBlurRadius(20);
//        arrowRectangle->setBorderWidth(0);
        arrowRectangle->setRadius(6);
        arrowRectangle->setShadowDistance(0);
        arrowRectangle->setShadowYOffset(2);
        arrowRectangle->setShadowXOffset(0);
        arrowRectangle->setArrowWidth(18);
        arrowRectangle->setArrowHeight(10);
        PopupWindow.reset(arrowRectangle);
    }

    m_popupTipsDelayTimer->setInterval(500);
    m_popupTipsDelayTimer->setSingleShot(true);

    m_popupAdjustDelayTimer->setInterval(100);
    m_popupAdjustDelayTimer->setSingleShot(true);

    connect(m_popupTipsDelayTimer, &QTimer::timeout, this, &Item::showHoverTips);
    connect(m_popupAdjustDelayTimer, &QTimer::timeout, this, &Item::updatePopupPosition);
}



void Item::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
}

void Item::moveEvent(QMoveEvent *e)
{
    QWidget::moveEvent(e);

    m_popupAdjustDelayTimer->start();
}


void Item::enterEvent(QEvent *e)
{
    m_hover = true;
    m_popupTipsDelayTimer->start();

    update();

    return QWidget::enterEvent(e);
}

void Item::leaveEvent(QEvent *e)
{
    QWidget::leaveEvent(e);

    m_hover = false;
    m_popupTipsDelayTimer->stop();

    // auto hide if popup is not model window
    if (m_popupShown && !PopupWindow->model())
        hidePopup();

    update();
}

const QRect Item::perfectIconRect() const
{
    const QRect itemRect = rect();
    const int iconSize = std::min(itemRect.width(), itemRect.height()) * 0.8;

    QRect iconRect;
    iconRect.setWidth(iconSize);
    iconRect.setHeight(iconSize);
    iconRect.moveTopLeft(itemRect.center() - iconRect.center());

    return iconRect;
}


void Item::showHoverTips()
{
    // another model popup window is alread exists
    if (PopupWindow->isVisible() && PopupWindow->model())
        return;

    QWidget * const content = popupTips();
    if (!content)
        return;

    showPopupWindow(content);
}

void Item::showPopupWindow(QWidget * const content, const bool model)
{
    m_popupShown = true;

    if (model)
        emit requestWindowAutoHide(false);

    ItemPopupWindow *popup = PopupWindow.get();
    QWidget *lastContent = popup->getContent();
    if (lastContent)
        lastContent->setVisible(false);
    popup->setArrowDirection(ItemPopupWindow::ArrowTop);
    popup->resize(content->sizeHint());
    popup->setContent(content);

    const QPoint p = popupMarkPoint();
    if (!popup->isVisible())
        QMetaObject::invokeMethod(popup, "show", Qt::QueuedConnection, Q_ARG(QPoint, p), Q_ARG(bool, model));
    else
        popup->show(p, model);

    connect(popup, &ItemPopupWindow::accept, this, &Item::popupWindowAccept, Qt::UniqueConnection);
}

void Item::popupWindowAccept()
{
    if (!PopupWindow->isVisible())
        return;
    disconnect(PopupWindow.get(), &ItemPopupWindow::accept, this, &Item::popupWindowAccept);

    hidePopup();

    emit requestWindowAutoHide(true);
}

QWidget *Item::popupTips()
{
    return nullptr;
}

const QPoint Item::popupMarkPoint()
{
    QPoint p;
    QWidget *w = this;
    do {
        p += w->pos();
        w = qobject_cast<QWidget *>(w->parent());
    } while (w);

    const QRect r = rect();
    const int offset = 2;
    p += QPoint(r.width() / 2, r.height() + offset);
    return p;
}

void Item::hidePopup()
{
    m_popupTipsDelayTimer->stop();
    m_popupShown = false;
    PopupWindow->hide();

    emit requestWindowAutoHide(true);
}

void Item::setDockDisplayMode(const DisplayMode mode)
{
    displayMode = mode;
}

void Item::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton)
        return showContextMenu();
}

void Item::showContextMenu() {
    const QString menuJson = contextMenu();
    if (menuJson.isEmpty())
        return;

    QDBusPendingReply<QDBusObjectPath> result = m_menuManagerInter->RegisterMenu();

    result.waitForFinished();
    if (result.isError())
    {
        qWarning() << result.error();
        return;
    }

    const QPoint p = popupMarkPoint();

    QJsonObject menuObject;
    menuObject.insert("x", QJsonValue(p.x()));
    menuObject.insert("y", QJsonValue(p.y()));
    menuObject.insert("isDockMenu", QJsonValue(true));
    menuObject.insert("menuJsonContent", QJsonValue(menuJson));


    menuObject.insert("direction", "top");

    const QDBusObjectPath path = result.argumentAt(0).value<QDBusObjectPath>();
    DBusMenu *menuInter = new DBusMenu(path.path(), this);

    connect(menuInter, &DBusMenu::ItemInvoked, this, &Item::invokedMenuItem);
    connect(menuInter, &DBusMenu::MenuUnregistered, this, &Item::requestRefershWindowVisible);
    connect(menuInter, &DBusMenu::MenuUnregistered, menuInter, &DBusMenu::deleteLater, Qt::QueuedConnection);

    menuInter->ShowMenu(QString(QJsonDocument(menuObject).toJson()));
    hidePopup();
}

void Item::invokedMenuItem(const QString &itemId, const bool checked)
{
    Q_UNUSED(itemId)
    Q_UNUSED(checked)
}

const QString Item::contextMenu() const
{
    return QString();
}

void Item::showPopupApplet(QWidget *const applet) {
    // another model popup window is alread exists
    if (PopupWindow->isVisible() && PopupWindow->model())
        return;

    showPopupWindow(applet, true);
}

void Item::updatePopupPosition() {
    Q_ASSERT(sender() == m_popupAdjustDelayTimer);

    if (!m_popupShown || !PopupWindow->isVisible())
        return;

    const QPoint p = popupMarkPoint();
    PopupWindow->show(p, PopupWindow->model());
}

Item::~Item() {
    if (m_popupShown)
        popupWindowAccept();
}
