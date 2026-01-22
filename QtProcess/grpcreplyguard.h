#pragma once

#include <QtGrpc/QGrpcCallReply>

class GrpcReplyGuard
{
public:
    explicit GrpcReplyGuard(QGrpcCallReply* reply = nullptr) : m_reply(reply)
    {
    }

    GrpcReplyGuard(const GrpcReplyGuard&) = delete;
    GrpcReplyGuard& operator=(const GrpcReplyGuard&) = delete;

    GrpcReplyGuard(GrpcReplyGuard&& other) noexcept : m_reply(other.m_reply)
    {
        other.m_reply = nullptr;
    }

    GrpcReplyGuard& operator=(GrpcReplyGuard&& other) noexcept
    {
        if (this != &other)
        {
            reset();
            m_reply = other.m_reply;
            other.m_reply = nullptr;
        }
        return *this;
    }

    ~GrpcReplyGuard()
    {
        reset();
    }

    QGrpcCallReply* get() const noexcept
    {
        return m_reply;
    }

    explicit operator bool() const noexcept
    {
        return m_reply != nullptr;
    }

private:
    void reset()
    {
        if (m_reply)
        {
            m_reply->deleteLater();
        }
        m_reply = nullptr;
    }

    QGrpcCallReply* m_reply = nullptr;
};
