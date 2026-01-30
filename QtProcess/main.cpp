#include "mainwindow.h"

#include "pluginlogresult.h"
#include "pluginversion.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<PluginVersion>("PluginVersion");
    qRegisterMetaType<PluginLogResult>("PluginLogResult");

    MainWindow w;
    w.show();
    return a.exec();
}
