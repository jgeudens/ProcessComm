#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "echoclient.h"
#include "loggerclient.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    connectEcho();
    connectLogger();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::connectEcho()
{
    _echoClient = new EchoClient(QUrl("http://127.0.0.1:50051"), this);

    connect(_echoClient, &EchoClient::echoSucceeded, [this](const QString& reply) { qInfo() << "Reply:" << reply; });

    connect(_echoClient, &EchoClient::echoFailed,
            [this](int status, const QString& error) { qWarning() << "RPC failed:" << status << error; });

    _echoClient->echo("Hello from modular Qt gRPC client");
}

void MainWindow::connectLogger()
{
    _loggerClient = new LoggerClient(QUrl("http://127.0.0.1:50051"), this);

    connect(_loggerClient, &LoggerClient::callFailed,
            [this](int status, const QString& error) { qWarning() << "RPC failed:" << status << error; });

    connect(_loggerClient, &LoggerClient::receivedPluginInfo, [this](const PluginVersion& pluginVersion) {
        qInfo() << "Logger Plugin Info:\n"
                << pluginVersion.version() << "\n"
                << pluginVersion.type() << "\n"
                << pluginVersion.author() << "\n"
                << pluginVersion.description();
    });

    _loggerClient->requestPluginInfo();
}
