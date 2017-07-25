//
// Created by linux on 17-7-25.
//

#ifndef FLYOS_TOPBAR_SYSTEMINFOWIDGET_H
#define FLYOS_TOPBAR_SYSTEMINFOWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <dcircleprogress.h>

DWIDGET_USE_NAMESPACE

class systeminfowidget : public QWidget{
public:
    systeminfowidget(QWidget *parent = 0);

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    QLabel *download;
    QLabel *upload;
    DCircleProgress *memorySize;
    QLabel *memorySizeTitle;


    long lastDownloadData;

    long lastUploadData;

    void getNetInfo();

    void getMenInfo();

    void setDownload(long downloadData);
    void setUpload(long uploadData);
    void setMemSize(long Total,long Available);

};


#endif //FLYOS_TOPBAR_SYSTEMINFOWIDGET_H
