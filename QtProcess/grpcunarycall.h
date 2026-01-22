#pragma once

#include "grpcreplyguard.h"
#include <QObject>
#include <QPointer>
#include <QtGrpc/QGrpcCallReply>
#include <QtGrpc/QGrpcStatus>
#include <utility>

template<typename ReplyT, typename SuccessFn, typename ErrorFn>
inline void executeUnaryRpc(QObject* context, QGrpcCallReply* rawReply, SuccessFn&& onSuccess, ErrorFn&& onError)
{
    GrpcReplyGuard guard(rawReply);

    if (!guard)
    {
        onError(-1, QStringLiteral("Failed to start RPC"));
        return;
    }

    QGrpcCallReply* reply = guard.get();

    // Tie reply to context for Qt-side lifetime safety
    reply->setParent(context);

    QPointer<QObject> safeContext(context);

    QObject::connect(
      reply, &QGrpcCallReply::finished, reply,
      [reply, safeContext, guard = std::move(guard), onSuccess = std::forward<SuccessFn>(onSuccess),
       onError = std::forward<ErrorFn>(onError)](const QGrpcStatus& status) mutable {
          // Context already destroyed
          if (!safeContext)
              return;

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
      },
      Qt::SingleShotConnection);
}
