#ifndef DOCKITEM_H
#define DOCKITEM_H

#include "constants.h"
#include "frame/util/itempopupwindow.h"
#include <QFrame>
#include <memory>
#include <frame/dbus/dbusmenumanager.h>

using namespace Dock;
//class DBusMenuManager;
class Item : public QWidget
{
    Q_OBJECT

public:
    enum ItemType {
        Stretch,
        Plugins,
        Container,
    };

public:
    explicit Item(QWidget *parent = nullptr);
    ~Item();


    inline virtual ItemType itemType() const {Q_UNREACHABLE(); return Plugins;}

    void setDockDisplayMode(const DisplayMode mode);


public slots:
    virtual void refershIcon() {}

signals:
    void dragStarted() const;
    void itemDropped(QObject *destination) const;
    void requestWindowAutoHide(const bool autoHide) const;
    void requestRefershWindowVisible() const;

protected:
    void paintEvent(QPaintEvent *e);
    void moveEvent(QMoveEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

    const QRect perfectIconRect() const;
    const QPoint popupMarkPoint();

    void hidePopup();
    void showPopupApplet(QWidget * const applet);
    void showPopupWindow(QWidget * const content, const bool model = false);
    virtual void showHoverTips();
    virtual const QString contextMenu() const;
    virtual void invokedMenuItem(const QString &itemId, const bool checked);
    virtual QWidget *popupTips();

protected slots:
    void showContextMenu();

private:
    void updatePopupPosition();
protected:
    bool m_hover;
    bool m_popupShown;

    QTimer *m_popupTipsDelayTimer;
    QTimer *m_popupAdjustDelayTimer;

    DBusMenuManager *m_menuManagerInter;

    static DisplayMode displayMode;
    static std::unique_ptr<ItemPopupWindow> PopupWindow;

    void popupWindowAccept();

    virtual void mousePressEvent(QMouseEvent *event);

};

#endif // DOCKITEM_H
