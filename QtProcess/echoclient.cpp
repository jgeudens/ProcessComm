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

    auto* reply = m_client->Echo(request).release();

    connect(
      reply, &QGrpcCallReply::finished, this,
      [this, reply](const QGrpcStatus& status) {
          if (status.isOk())
          {
              const auto response = reply->read<echo::EchoReply>();
              if (response.has_value())
              {
                  emit echoSucceeded(response->message());
              }
              else
              {
                  emit echoFailed(-1, QStringLiteral("Failed to read response"));
              }
          }
          else
          {
              emit echoFailed(static_cast<int>(status.code()), status.message());
          }

          reply->deleteLater();
      },
      Qt::SingleShotConnection);
}
