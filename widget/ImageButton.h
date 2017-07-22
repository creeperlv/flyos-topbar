//
// Created by linux on 17-7-22.
//

#ifndef FLYOS_TOPBAR_IMAGEBUTTON_H
#define FLYOS_TOPBAR_IMAGEBUTTON_H


#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

class ImageButton : public QPushButton{
public:
    ImageButton(QWidget *parent = 0);
    ImageButton(QString IconPath, QWidget *parent = 0);

private:
    QLabel *Icon;
public:

    void setIcon(QString IconPath);

    void setIcon(QPixmap Icon);
};


#endif //FLYOS_TOPBAR_IMAGEBUTTON_H
