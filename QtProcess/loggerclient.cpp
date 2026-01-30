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
          PluginVersion pluginVersion(response.version(), response.type(), response.author(), response.description());
          _pluginVersion = pluginVersion;
          emit receivedPluginInfo(pluginVersion);
      },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}

void LoggerClient::GetAvailableDataPoints()
{
    logger::v1::GetAvailableDataPointsRequest request;

    auto* reply = _client->GetAvailableDataPoints(request).release();

    executeUnaryRpc<logger::v1::GetAvailableDataPointsResponse>(
      this, reply,
      [this](const logger::v1::GetAvailableDataPointsResponse& response) {
          emit receivedAvailableDataPoints(response.dataPoint());
      },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}

void LoggerClient::StartLogging()
{
    logger::v1::StartLoggingRequest request;

    auto* reply = _client->StartLogging(request).release();

    executeUnaryRpc<logger::v1::StartLoggingResponse>(
      this, reply, [this](const logger::v1::StartLoggingResponse& response) { emit loggingStarted(); },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}

void LoggerClient::StopLogging()
{
    logger::v1::StopLoggingRequest request;

    auto* reply = _client->StopLogging(request).release();

    executeUnaryRpc<logger::v1::StopLoggingResponse>(
      this, reply, [this](const logger::v1::StopLoggingResponse& response) { emit loggingStopped(); },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}

void LoggerClient::IsLoggingActive()
{
    logger::v1::IsLoggingActiveRequest request;

    auto* reply = _client->IsLoggingActive(request).release();

    executeUnaryRpc<logger::v1::IsLoggingActiveResponse>(
      this, reply,
      [this](const logger::v1::IsLoggingActiveResponse& response) {
          bool isActive = response.isActive();
          emit loggingStatusReceived(isActive);
      },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}

void LoggerClient::ReadLog(const QStringList& availableDataPoints)
{
    logger::v1::ReadLogRequest request;

    request.setDataPoint(availableDataPoints);

    auto* reply = _client->ReadLog(request).release();

    executeUnaryRpc<logger::v1::ReadLogResponse>(
      this, reply,
      [this](const logger::v1::ReadLogResponse& response) {
          // Process log results
          QList<PluginLogResult> logData;
          for (const auto& result : response.results())
          {
              logData.append(PluginLogResult(result.dataPointName(), result.value(), static_cast<int>(result.status()),
                                             result.errorMessage()));
          }
          emit logDataReceived(logData);
      },
      [this](int code, const QString& error) { emit callFailed(code, error); });
}
