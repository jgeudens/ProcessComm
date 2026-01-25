#include "loggerclient.h"
#include "grpcunarycall.h"

LoggerClient::LoggerClient(const QUrl& endpoint, QObject* parent) : QObject(parent)
{
    _channel = std::make_shared<QGrpcHttp2Channel>(endpoint);
    _client = std::make_unique<logger::v1::LoggerPlugin::Client>();
    _client->attachChannel(_channel);
}

void LoggerClient::requestPluginInfo()
{
    logger::v1::GetPluginInfoRequest request;

    auto* reply = _client->GetPluginInfo(request).release();

    executeUnaryRpc<logger::v1::GetPluginInfoResponse>(
      this, reply,
      [this](const logger::v1::GetPluginInfoResponse& response) {
          PluginVersion _pluginVersion(response.version(), response.type(), response.author(), response.description());
          emit receivedPluginInfo(_pluginVersion);
      },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}
