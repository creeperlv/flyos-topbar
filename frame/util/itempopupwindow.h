#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <darrowrectangle.h>
#include <DWindowManagerHelper>
#include <frame/dbus/dbusxmousearea.h>
#include <frame/dbus/dbusdisplay.h>

DWIDGET_USE_NAMESPACE

class ItemPopupWindow : public Dtk::Widget::DArrowRectangle
{
    Q_OBJECT

public:
    explicit ItemPopupWindow(QWidget *parent = 0);
    ~ItemPopupWindow();

    bool model() const;

public slots:
    void show(const QPoint &pos, const bool model = false);
    void show(const int x, const int y);

signals:
    void accept() const;

protected:
    void showEvent(QShowEvent *e);
    void enterEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private slots:
    void compositeChanged();
    void globalMouseRelease(int button, int x, int y, const QString &id);
    void registerMouseEvent();
    void unRegisterMouseEvent();


private:
    bool m_model;
    QPoint m_lastPoint;
    QString m_mouseAreaKey;

    QTimer *m_acceptDelayTimer;

    DBusXMouseArea *m_mouseInter;
    DBusDisplay *m_displayInter;
    DWindowManagerHelper *m_wmHelper;

};

#endif // POPUPWINDOW_H
