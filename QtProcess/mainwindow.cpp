#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "echoclient.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_echoClient = new EchoClient(QUrl("http://127.0.0.1:50051"), this);

    QObject::connect(m_echoClient, &EchoClient::echoSucceeded, [this](const QString& reply) {
        qInfo() << "Reply:" << reply;
        QCoreApplication::quit();
    });

    QObject::connect(m_echoClient, &EchoClient::echoFailed, [this](int status, const QString& error) {
        qWarning() << "RPC failed:" << status << error;
        QCoreApplication::quit();
    });

    m_echoClient->echo("Hello from modular Qt gRPC client");
}

MainWindow::~MainWindow()
{
    delete ui;
    // m_echoClient is deleted automatically as QObject child of this MainWindow
}
