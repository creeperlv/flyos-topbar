#include "tipswidget.h"
#include "traywidget.h"

TrayApplet::TrayApplet(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(new QHBoxLayout)
{
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);

    setLayout(m_mainLayout);
    setFixedHeight(26);
}

void TrayApplet::clear()
{
    QLayoutItem *item = nullptr;
    while ((item = m_mainLayout->takeAt(0)) != nullptr)
    {
        if (item->widget())
            item->widget()->setParent(nullptr);
        delete item;
    }
}

void TrayApplet::addWidgets(QList<TrayWidget *> widgets)
{
    for (auto w : widgets)
    {
        w->setVisible(true);
        m_mainLayout->addWidget(w);
    }
    setFixedWidth(widgets.size() * 26);
}
