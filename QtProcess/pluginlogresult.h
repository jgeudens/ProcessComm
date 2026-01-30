#pragma once

#include <QMetaType>
#include <QString>

class PluginLogResult
{
public:
    PluginLogResult() = default;

    PluginLogResult(const QString& dataPointName, int value, int status, const QString& errorMessage)
        : _dataPointName(dataPointName), _value(value), _status(status), _errorMessage(errorMessage)
    {
    }

    PluginLogResult(const PluginLogResult& other)
        : _dataPointName(other._dataPointName),
          _value(other._value),
          _status(other._status),
          _errorMessage(other._errorMessage)
    {
    }

    const QString& dataPointName() const
    {
        return _dataPointName;
    }

    int value() const
    {
        return _value;
    }

    int status() const
    {
        return _status;
    }

    const QString& errorMessage() const
    {
        return _errorMessage;
    }

private:
    QString _dataPointName;
    int _value;
    int _status;
    QString _errorMessage;
};

Q_DECLARE_METATYPE(PluginLogResult)
