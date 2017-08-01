//
// Created by linux on 17-7-25.
//

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "systeminfowidget.h"

#include <QDebug>


systeminfowidget::systeminfowidget(QWidget *parent) {
    this->setFixedSize(50, 30);

    memorySize = new DCircleProgress(this);
    memorySize->setGeometry(0, 3, 26, 26);
    memorySize->setValue(0);
    memorySize->setStyleSheet("color:#2C3035");
    memorySize->setChunkColor(QColor("#2C3035"));
    memorySize->setBackgroundColor(QColor("#888888"));

    memorySizeTitle= new QLabel(this);
    memorySizeTitle->setGeometry(30, 0,30,30);
    memorySizeTitle->setStyleSheet("color:#2C3035;"
                                           "font-size:14px");

    upload = new QLabel(this);
    upload->move(65, 0);
    upload->setStyleSheet("color:#2C3035;"
                                  "font-size:12px");

    download = new QLabel(this);
    download->move(65, 15);
    download->setStyleSheet("color:#2C3035;"
                                    "font-size:12px");

    startTimer(1000);
}

void systeminfowidget::timerEvent(QTimerEvent *event) {
    getMenInfo();
    getNetInfo();
    int len1= download->fontMetrics().width(download->text());
    int len2= upload->fontMetrics().width(upload->text());

    int len = len1 > len2 ? len1 : len2;
    len += 65;

    download->setFixedWidth(len);
    upload->setFixedWidth(len);
    this->setFixedWidth(len);
}

void systeminfowidget::setDownload(long downloadData) {
    char danwei = 'b';
    float tmp;
    if (downloadData >= 1024 * 1024) {
        danwei = 'm';
        tmp = (float) downloadData / 1024 / 1024;
    } else if (downloadData >= 1024) {
        danwei = 'k';
        tmp = (float) downloadData / 1024;
    }
    QString str = QString("⇩：%1 %2/s").arg(tmp, 0, 'f', 2).arg(danwei);
    download->setText(str);
}

void systeminfowidget::setUpload(long uploadData) {
    char danwei = 'b';
    float tmp;
    if (uploadData >= 1024 * 1024) {
        tmp = (float) uploadData / 1024 / 1024;
        danwei = 'm';
    } else if (uploadData >= 1024) {
        tmp = (float) uploadData / 1024;
        danwei = 'k';
    }
    QString str = QString("⇧：%1 %2/s").arg(tmp, 0, 'f', 2).arg(danwei);
    upload->setText(str);
}

void systeminfowidget::getNetInfo() {
    QFile netdev("/proc/net/dev");
    if (netdev.open(QIODevice::ReadOnly)) {
        QTextStream in(&netdev);
        QString tmp;
        QList<long> list;
        in.readLine();
        in.readLine();
        long tmp1 = 0;
        long tmp2 = 0;
        while ((tmp = in.readLine()) != NULL) {
            QStringList list = tmp.split(":");
            list[0]= list[0].trimmed();
            if (list[0] != "lo") {
                QTextStream text(&list[1]);
                long tmp3;
                text >> tmp3;
                tmp1+=tmp3;
                text >> tmp3;
                text >> tmp3;
                text >> tmp3;
                text >> tmp3;
                text >> tmp3;
                text >> tmp3;
                text >> tmp3;
                text >> tmp3;
                tmp2+=tmp3;
            }

        }
        setDownload(tmp1 - lastDownloadData);
        setUpload(tmp2 - lastUploadData);
        lastDownloadData =tmp1;
        lastUploadData =tmp2;

    }

}

void systeminfowidget::getMenInfo() {
    QFile meminfo("/proc/meminfo");
    if (meminfo.open(QIODevice::ReadOnly)) {
        QTextStream in(&meminfo);
        QString tmp;
        QList<long> list;
        while ((tmp = in.readLine()) != NULL) {
            QStringList info;
            tmp = tmp.simplified();
            info = tmp.split(":");
            info[1] = info[1].split("kB")[0];
            if (info[0] == "MemTotal") {
                list << info[1].toLong();
            } else if (info[0] == "MemAvailable") {
                list << info[1].toLong();
            }
        }
        setMemSize(list[0], list[0] - list[1]);
        meminfo.close();
    }

}

void systeminfowidget::setMemSize(long Total, long Available) {
    int mem = (float) Available / Total * 100;
    memorySize->setValue(mem);
    QString tmp = QString("%1%").arg(mem);
    memorySizeTitle->setText(tmp);
}
