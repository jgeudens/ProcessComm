#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "echoclient.h"
#include "loggerclient.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectEcho();
    connectLogger();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectEcho()
{
    m_echoClient = new EchoClient(QUrl("http://127.0.0.1:50051"), this);

    connect(m_echoClient, &EchoClient::echoSucceeded, [this](const QString& reply) { qInfo() << "Reply:" << reply; });

    connect(m_echoClient, &EchoClient::echoFailed,
            [this](int status, const QString& error) { qWarning() << "RPC failed:" << status << error; });

    m_echoClient->echo("Hello from modular Qt gRPC client");
}

void MainWindow::connectLogger()
{
    m_loggerClient = new LoggerClient(QUrl("http://127.0.0.1:50051"), this);

    connect(m_loggerClient, &LoggerClient::callFailed,
            [this](int status, const QString& error) { qWarning() << "RPC failed:" << status << error; });

    connect(m_loggerClient, &LoggerClient::receivedPluginInfo, [this](const QString& info) {
        qInfo() << "Logger Plugin Info:\n" << info;
    });

    m_loggerClient->requestPluginInfo();
}
