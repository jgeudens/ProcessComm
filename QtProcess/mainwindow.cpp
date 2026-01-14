#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "echoclient.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    EchoClient client(QUrl("http://127.0.0.1:50051"));

    QObject::connect(&client, &EchoClient::echoSucceeded, [&](const QString& reply) {
        qInfo() << "Reply:" << reply;
        QCoreApplication::quit();
    });

    QObject::connect(&client, &EchoClient::echoFailed, [&](int status, const QString& error) {
        qWarning() << "RPC failed:" << status << error;
        QCoreApplication::quit();
    });

    client.echo("Hello from modular Qt gRPC client");
}

MainWindow::~MainWindow()
{
    delete ui;
}
