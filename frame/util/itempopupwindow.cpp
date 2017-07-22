#include "itempopupwindow.h"

#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

DWIDGET_USE_NAMESPACE

const int MOUSE_BUTTON(1 << 1);

ItemPopupWindow::ItemPopupWindow(QWidget *parent)
    : DArrowRectangle(ArrowBottom, parent),
      m_model(false),

      m_acceptDelayTimer(new QTimer(this)),

      m_mouseInter(new DBusXMouseArea(this)),
      m_displayInter(new DBusDisplay(this))
{
    m_acceptDelayTimer->setSingleShot(true);
    m_acceptDelayTimer->setInterval(100);

    m_wmHelper = DWindowManagerHelper::instance();

    compositeChanged();

    setBackgroundColor(DBlurEffectWidget::DarkColor);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_InputMethodEnabled, false);
    setFocusPolicy(Qt::StrongFocus);

    connect(m_acceptDelayTimer, &QTimer::timeout, this, &ItemPopupWindow::accept);
    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &ItemPopupWindow::compositeChanged);
}

ItemPopupWindow::~ItemPopupWindow()
{
}

bool ItemPopupWindow::model() const
{
    return m_model;
}

void ItemPopupWindow::show(const QPoint &pos, const bool model)
{
    m_model = model;
    m_lastPoint = pos;

    show(pos.x(), pos.y());

    if (!model && !m_mouseAreaKey.isEmpty())
        unRegisterMouseEvent();

    if (model && m_mouseAreaKey.isEmpty())
        registerMouseEvent();
}

void ItemPopupWindow::show(const int x, const int y)
{
    m_lastPoint = QPoint(x, y);

    DArrowRectangle::show(x, y);
}

void ItemPopupWindow::showEvent(QShowEvent *e)
{
    DArrowRectangle::showEvent(e);

    QTimer::singleShot(1, this, [&] {
        raise();
        if (!m_model)
            return;
        activateWindow();
        setFocus(Qt::ActiveWindowFocusReason);
    });
}

void ItemPopupWindow::enterEvent(QEvent *e)
{
    DArrowRectangle::enterEvent(e);

    raise();
    if (!m_model)
        return;
    activateWindow();
    setFocus(Qt::ActiveWindowFocusReason);
}

void ItemPopupWindow::mousePressEvent(QMouseEvent *e)
{
    DArrowRectangle::mousePressEvent(e);

//    if (e->button() == Qt::LeftButton)
//        m_acceptDelayTimer->start();
}

bool ItemPopupWindow::eventFilter(QObject *o, QEvent *e)
{
    if (o != getContent() || e->type() != QEvent::Resize)
        return false;

    // FIXME: ensure position move after global mouse release event
    QTimer::singleShot(100, this, [this] {if (isVisible()) show(m_lastPoint, m_model);});

    return false;
}


void ItemPopupWindow::compositeChanged()
{
    if (m_wmHelper->hasComposite())
        setBorderColor(QColor(255, 255, 255, 255 * 0.05));
    else
        setBorderColor(QColor("#2C3238"));
}

void ItemPopupWindow::unRegisterMouseEvent()
{
    if (m_mouseAreaKey.isEmpty())
        return;

    disconnect(m_mouseInter, &DBusXMouseArea::ButtonRelease, this, &ItemPopupWindow::globalMouseRelease);

    m_mouseInter->UnregisterArea(m_mouseAreaKey);
    m_mouseAreaKey.clear();
}

void ItemPopupWindow::globalMouseRelease(int button, int x, int y, const QString &id) {
// button_left
    if (button != 1)
        return;

    if (id != m_mouseAreaKey)
        return;

    Q_ASSERT(m_model);

    const QRect rect = QRect(pos(), size());
    const QPoint pos = QPoint(x, y);

    if (rect.contains(pos))
        return;

    emit accept();

    unRegisterMouseEvent();
}

void ItemPopupWindow::registerMouseEvent() {
    if (!m_mouseAreaKey.isEmpty())
        return;

    // only regist mouse button event
    m_mouseAreaKey = m_mouseInter->RegisterArea(0, 0, m_displayInter->screenWidth(), m_displayInter->screenHeight(), MOUSE_BUTTON);

    connect(m_mouseInter, &DBusXMouseArea::ButtonRelease, this, &ItemPopupWindow::globalMouseRelease, Qt::QueuedConnection);
}
