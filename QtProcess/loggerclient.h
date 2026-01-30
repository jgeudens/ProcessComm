#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <memory>

#include <QtGrpc/QGrpcCallReply>
#include <QtGrpc/QGrpcHttp2Channel>

#include "logger_client.grpc.qpb.h"

#include "pluginlogresult.h"
#include "pluginversion.h"

class LoggerClient : public QObject
{
    Q_OBJECT

public:
    explicit LoggerClient(const QUrl& endpoint, QObject* parent = nullptr);
    void requestPluginInfo();
    void getAvailableDataPoints();
    void startLogging();
    void stopLogging();
    void isLoggingActive();
    void readLog(const QStringList& availableDataPoints);

signals:
    void callFailed(int grpcStatus, const QString& errorString);
    void receivedPluginInfo(const PluginVersion& pluginVersion);
    void receivedAvailableDataPoints(const QStringList& dataPoints);
    void loggingStarted();
    void loggingStatusReceived(bool isActive);
    void loggingStopped();
    void logDataReceived(const QList<PluginLogResult>& logResult);

private:
    std::shared_ptr<QGrpcHttp2Channel> _channel;
    std::unique_ptr<logger::v1::LoggerPlugin::Client> _client;

    PluginVersion _pluginVersion;
};
