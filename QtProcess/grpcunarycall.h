#pragma once

#include <QObject>
#include <QtGrpc/QGrpcCallReply>
#include <QtGrpc/QGrpcStatus>

template<typename ReplyT, typename SuccessFn, typename ErrorFn>
inline void executeUnaryRpc(QObject* context, QGrpcCallReply* reply, SuccessFn&& onSuccess, ErrorFn&& onError)
{
    QObject::connect(
      reply, &QGrpcCallReply::finished, context,
      [reply, onSuccess = std::forward<SuccessFn>(onSuccess),
       onError = std::forward<ErrorFn>(onError)](const QGrpcStatus& status) {
          if (status.isOk())
          {
              const auto response = reply->template read<ReplyT>();
              if (response.has_value())
              {
                  onSuccess(*response);
              }
              else
              {
                  onError(-1, QStringLiteral("Failed to decode response"));
              }
          }
          else
          {
              onError(static_cast<int>(status.code()), status.message());
          }

          reply->deleteLater();
      },
      Qt::SingleShotConnection);
}
