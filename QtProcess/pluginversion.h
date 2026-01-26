#pragma once

#include <QMetaType>
#include <QString>

class PluginVersion
{
public:
    PluginVersion() = default;

    PluginVersion(const QString& version, const QString& type, const QString& author, const QString& description)
        : _version(version), _type(type), _author(author), _description(description)
    {
    }

    const QString& version() const
    {
        return _version;
    }

    const QString& type() const
    {
        return _type;
    }

    const QString& author() const
    {
        return _author;
    }

    const QString& description() const
    {
        return _description;
    }

private:
    QString _version;
    QString _type;
    QString _author;
    QString _description;
};

Q_DECLARE_METATYPE(PluginVersion)