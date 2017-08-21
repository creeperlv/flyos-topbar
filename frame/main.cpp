#include <DApplication>
#include <zconf.h>
#include "mainframe.h"
#include "shadow.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char **argv) {
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    if (app.setSingleInstance(QString("flyos-topbar_%1").arg(getuid()))) {
        app.setTheme("light");
        app.loadTranslator();
        app.setApplicationDisplayName("FlyOS标题栏");
        app.setApplicationName("flyos-topbar");
        app.setApplicationVersion("0.1.6");
        app.setOrganizationDomain("fly-os.xyz");
        app.setApplicationDescription(DApplication::translate("MainWindow", "FlyOS标题栏") + "\n");

        shadow *shadow1 = new shadow;
        shadow1->show();

        mainFrame *mainFrame1 = new mainFrame;
        mainFrame1->registerDockType();
        mainFrame1->activateWindow();
        mainFrame1->show();

        return app.exec();
    }

    return 0;
}
