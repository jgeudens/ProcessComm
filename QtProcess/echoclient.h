#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QObject>
#include <memory>

#include <QtGrpc/QGrpcCallReply>
#include <QtGrpc/QGrpcHttp2Channel>

#include "echo_client.grpc.qpb.h"

class EchoClient : public QObject
{
    Q_OBJECT

public:
    explicit EchoClient(const QUrl& endpoint, QObject* parent = nullptr);

    void echo(const QString& message);

signals:
    void echoSucceeded(const QString& response);
    void echoFailed(int grpcStatus, const QString& errorString);

private:
    std::shared_ptr<QGrpcHttp2Channel> m_channel;
    std::unique_ptr<echo::EchoService::Client> m_client;
};

#endif // ECHOCLIENT_H
