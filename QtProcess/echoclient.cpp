#include "echoclient.h"
#include "grpcunarycall.h"

EchoClient::EchoClient(const QUrl& endpoint, QObject* parent) : QObject(parent)
{
    m_channel = std::make_shared<QGrpcHttp2Channel>(endpoint);
    m_client = std::make_unique<echo::v1::EchoService::Client>();
    m_client->attachChannel(m_channel);
}

void EchoClient::echo(const QString& message)
{
    echo::v1::EchoRequest request;
    request.setMessage(message);

    auto* reply = m_client->Echo(request).release();

    executeUnaryRpc<echo::v1::EchoReply>(
      this, reply, [this](const echo::v1::EchoReply& response) { emit echoSucceeded(response.message()); },
      [this](int code, const QString& error) { emit echoFailed(code, error); });
}
