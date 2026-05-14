#pragma once

#include <QObject>
#include <QString>

class CacheManager : public QObject
{
    Q_OBJECT

public:
    static CacheManager &instance();

    bool isCached(const QString &hash) const;
    void saveToCache(const QString &hash, const QByteArray &data);
    QString cacheFilePath(const QString &hash) const;

private:
    explicit CacheManager(QObject *parent = nullptr);

    QString m_cacheDir;
};
