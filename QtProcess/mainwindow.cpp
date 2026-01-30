#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "echoclient.h"
#include "loggerclient.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    connectEcho();
    connectLogger();

    connect(_ui->btnPluginInfo, &QPushButton::clicked, [this]() { _loggerClient->requestPluginInfo(); });
    connect(_ui->btnStartLogging, &QPushButton::clicked, [this]() { _loggerClient->startLogging(); });
    connect(_ui->btnStopLogging, &QPushButton::clicked, [this]() { _loggerClient->stopLogging(); });
    connect(_ui->btnAvailableDatapoints, &QPushButton::clicked, [this]() {
        _loggerClient->getAvailableDataPoints();
        _availableDataPoints.clear();
    });
    connect(_ui->btnReadlog, &QPushButton::clicked, [this]() { _loggerClient->readLog(_availableDataPoints); });
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

    connect(_loggerClient, &LoggerClient::callFailed, [this](int status, const QString& error) {
        qWarning() << "RPC failed:" << status << error;
        _ui->statusbar->showMessage("Logger RPC failed: " + error, 5000);
    });

    connect(_loggerClient, &LoggerClient::loggingStarted, [this]() {
        qInfo() << "Logging started";
        _ui->statusbar->showMessage("Logging started", 5000);
    });

    connect(_loggerClient, &LoggerClient::loggingStopped, [this]() {
        qInfo() << "Logging stopped";
        _ui->statusbar->showMessage("Logging stopped", 5000);
    });

    connect(_loggerClient, &LoggerClient::receivedPluginInfo, [this](const PluginVersion& pluginVersion) {
        qInfo() << "Logger Plugin Info:\n"
                << pluginVersion.version() << "\n"
                << pluginVersion.type() << "\n"
                << pluginVersion.author() << "\n"
                << pluginVersion.description();
        _ui->txtPluginInfo->setPlainText(QString("Version: %1\nType: %2\nAuthor: %3\nDescription: %4")
                                           .arg(pluginVersion.version())
                                           .arg(pluginVersion.type())
                                           .arg(pluginVersion.author())
                                           .arg(pluginVersion.description()));
    });

    connect(_loggerClient, &LoggerClient::receivedAvailableDataPoints, [this](const QStringList& dataPoints) {
        qInfo() << "Available Data Points:" << dataPoints;
        _availableDataPoints = dataPoints;
        _ui->txtAvailableDatapoint->setPlainText(dataPoints.join("\n"));
    });
    connect(_loggerClient, &LoggerClient::logDataReceived, [this](const QList<PluginLogResult>& logData) {
        qInfo() << "Log data received";
        _ui->statusbar->showMessage("Log data received", 5000);
        _ui->txtLog->clear();
        for (const PluginLogResult& result : logData)
        {
            qInfo() << "Data Point:" << result.dataPointName() << "Value:" << result.value()
                    << "Status:" << result.status() << "Error Message:" << result.errorMessage();
            _ui->txtLog->appendPlainText(QString("Data Point: %1, Value: %2, Status: %3, Error Message: %4")
                                           .arg(result.dataPointName())
                                           .arg(result.value())
                                           .arg(result.status())
                                           .arg(result.errorMessage()));
        }
    });
}
