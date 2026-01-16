#include "echoclient.h"
#include "grpcunarycall.h"

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

    executeUnaryRpc<echo::EchoReply>(
      this, reply, [this](const echo::EchoReply& response) { emit echoSucceeded(response.message()); },
      [this](int code, const QString& error) { emit echoFailed(code, error); });
}
