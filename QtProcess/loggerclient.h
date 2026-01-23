#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <memory>

#include <QtGrpc/QGrpcCallReply>
#include <QtGrpc/QGrpcHttp2Channel>

#include "logger_client.grpc.qpb.h"

class LoggerClient : public QObject
{
    Q_OBJECT

public:
    explicit LoggerClient(const QUrl& endpoint, QObject* parent = nullptr);
    void requestPluginInfo();

signals:
    void callFailed(int grpcStatus, const QString& errorString);
    void receivedPluginInfo(const QString& info);

private:
    std::shared_ptr<QGrpcHttp2Channel> m_channel;
    std::unique_ptr<logger::v1::LoggerPlugin::Client> m_client;
};
