//
// Created by linux on 17-7-22.
//

#include <QtCore/QFile>
#include "ImageButton.h"

ImageButton::ImageButton(QWidget *parent) :
        QPushButton(parent),
        Icon(new QLabel(this)){

    QFile css(":/resources/image_button.css");
    css.open(QIODevice::ReadOnly);
    setStyleSheet(css.readAll());
    this->setFixedSize(45, 30);
    Icon->setGeometry(0, 0, 45, 30);
    Icon->setAlignment(Qt::AlignCenter);

}

ImageButton::ImageButton(QString IconPath, QWidget *parent) :
        QPushButton(parent),
        Icon(new QLabel(this)) {
    QFile css(":/resources/image_button.css");
    css.open(QIODevice::ReadOnly);
    setStyleSheet(css.readAll());

    this->setFixedSize(45, 30);
    Icon->setPixmap(QPixmap(IconPath).scaled(30, 30));
    Icon->setGeometry(0, 0, 45, 30);
    Icon->setAlignment(Qt::AlignCenter);

}

void ImageButton::setIcon(QPixmap Icon) {
    ImageButton::Icon->setPixmap(Icon);
    ImageButton::Icon->setAlignment(Qt::AlignCenter);

}

void ImageButton::setIcon(QString IconPath) {
    Icon->setPixmap(QPixmap(IconPath).scaled(30, 30, Qt::KeepAspectRatioByExpanding));
}
