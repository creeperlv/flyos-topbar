#include "pluginloader.h"
#include "pluginscontroller.h"

#include <QApplication>
#include <QDebug>

PluginLoader::PluginLoader(QObject *parent)
        : QThread(parent) {

}

void PluginLoader::run() {
#ifdef QT_DEBUG
    const QDir pluginsDir(QApplication::applicationDirPath() + "/plugins");
#else
    const QDir pluginsDir("../lib/flyos-topbar/plugins");
#endif
    const QStringList plugins = pluginsDir.entryList(QDir::Files);
    for (const QString file : plugins) {
        if (!QLibrary::isLibrary(file))
            continue;

        emit pluginFounded(pluginsDir.absoluteFilePath(file));

        msleep(500);
    }

    emit finished();
}
