#include "CacheManager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

CacheManager &CacheManager::instance()
{
    static CacheManager instance;
    return instance;
}

CacheManager::CacheManager(QObject *parent)
    : QObject(parent)
{
    m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/image_cache";
    QDir().mkpath(m_cacheDir);
}

bool CacheManager::isCached(const QString &hash) const
{
    return !hash.isEmpty() && QFileInfo::exists(cacheFilePath(hash));
}

void CacheManager::saveToCache(const QString &hash, const QByteArray &data)
{
    if (hash.isEmpty() || data.isEmpty()) {
        return;
    }

    QFile file(cacheFilePath(hash));
    if (file.exists()) {
        return;
    }

    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    }
}

QString CacheManager::cacheFilePath(const QString &hash) const
{
    return m_cacheDir + "/" + hash + ".png";
}
