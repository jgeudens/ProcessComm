#include "echoclient.h"

EchoClient::EchoClient(const QUrl& endpoint, QObject* parent) : QObject(parent)
{
    m_channel = std::make_shared<QGrpcHttp2Channel>(endpoint);
    m_client = std::make_unique<echo::EchoService::Client>();
    m_client->attachChannel(m_channel);
}

void EchoClient::echo(const QString& message)
{
    echo::EchoRequest request;
    request.setMessage(message);

    std::unique_ptr<QGrpcCallReply> reply = m_client->Echo(request);
    const auto* replyPtr = reply.get();

    QObject::connect(
      replyPtr, &QGrpcCallReply::finished, replyPtr,
      [this, reply = std::move(reply)](const QGrpcStatus& status) {
          if (status.isOk())
          {
              const auto response = reply->read<echo::EchoReply>();

              if (response.has_value())
              {
                  emit echoSucceeded(response.value().message());
              }
              else
              {
                  emit echoFailed(0, QStringLiteral("Failed to read response"));
              }
          }
          else
          {
              qDebug() << "Client (UnaryCall) failed:" << status;
          }
      },
      Qt::SingleShotConnection);
}
