#include "loggerclient.h"
#include "grpcunarycall.h"

LoggerClient::LoggerClient(const QUrl& endpoint, QObject* parent) : QObject(parent)
{
    m_channel = std::make_shared<QGrpcHttp2Channel>(endpoint);
    m_client = std::make_unique<logger::v1::LoggerPlugin::Client>();
    m_client->attachChannel(m_channel);
}

void LoggerClient::requestPluginInfo()
{
    logger::v1::GetPluginInfoRequest request;

    auto* reply = m_client->GetPluginInfo(request).release();

    executeUnaryRpc<logger::v1::GetPluginInfoResponse>(
      this, reply,
      [this](const logger::v1::GetPluginInfoResponse& response) {
          QString info = QString("version: %1\ntype: %2\nauthor: %3\ndescription: %4")
                           .arg(response.version())
                           .arg(response.type())
                           .arg(response.author())
                           .arg(response.description());
          emit receivedPluginInfo(info);
      },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}
