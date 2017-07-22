#include "datetimewidget.h"
#include "constants.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QSvgRenderer>
#include <QMouseEvent>

DatetimeWidget::DatetimeWidget(QWidget *parent)
        : QWidget(parent),

          m_settings("deepin", "dde-dock-datetime"),

          m_24HourFormat(m_settings.value("24HourFormat", true).toBool()) {

}

void DatetimeWidget::toggleHourFormat() {
    m_24HourFormat = !m_24HourFormat;

    this->setFixedWidth(m_24HourFormat ? 100 : 120);

    m_settings.setValue("24HourFormat", m_24HourFormat);

    m_cachedTime.clear();
    update();

    emit requestUpdateGeometry();
}

QSize DatetimeWidget::sizeHint() const {
    QFontMetrics fm(qApp->font());

    if (m_24HourFormat)
        return fm.boundingRect("88:88").size() + QSize(20, 10);
    else
        return fm.boundingRect("88:88 A.A.").size() + QSize(20, 20);
}

void DatetimeWidget::resizeEvent(QResizeEvent *e) {
    m_cachedTime.clear();

    QWidget::resizeEvent(e);
}

void DatetimeWidget::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);

    const QDateTime current = QDateTime::currentDateTime();

    QPainter painter(this);

    QString format;
    if (m_24HourFormat)
        format = "MM月dd日 hh:mm";
    else {
        format = "MM月dd日 hh:mm AP";
    }

    this->setFixedWidth(m_24HourFormat ? 100 : 120);

    painter.setPen(QColor(44, 48, 53));
    painter.drawText(rect(), Qt::AlignCenter, current.currentDateTime().toString(format));
    return;
}

void DatetimeWidget::mousePressEvent(QMouseEvent *e) {
    if (e->button() != Qt::RightButton)
        return QWidget::mousePressEvent(e);

    const QPoint p(e->pos() - rect().center());
    if (p.manhattanLength() < std::min(width(), height()) * 0.8 * 0.5) {
        emit requestContextMenu();
        return;
    }

    QWidget::mousePressEvent(e);
}

const QPixmap DatetimeWidget::loadSvg(const QString &fileName, const QSize size) {
    QPixmap pixmap(size);
    QSvgRenderer renderer(fileName);
    pixmap.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pixmap);
    renderer.render(&painter);
    painter.end();

    return pixmap;
}
