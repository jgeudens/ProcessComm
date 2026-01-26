#include "mainwindow.h"

#include "pluginversion.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<PluginVersion>("PluginVersion");

    MainWindow w;
    w.show();
    return a.exec();
}
