#include "ThemeManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QColor>
#include <QDebug>

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
{
    resetToDefaults();
}

void ThemeManager::applyTheme(const QString &themeName)
{
    loadTheme(themeName);
    m_currentTheme = themeName;
    emit themeChanged();
}

QString ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

QVariantMap ThemeManager::colors() const
{
    return m_colors;
}

QVariantMap ThemeManager::icons() const
{
    return m_icons;
}

void ThemeManager::resetToDefaults()
{
    m_colors.clear();
    m_icons.clear();

    m_colors["backgroundColor"] = QColor("#1e1e2f");
    m_colors["textColor"] = QColor("#f7f0e8");
    m_colors["accentColor"] = QColor("#E67E22");
    m_colors["accentHoverColor"] = QColor("#F39C12");
    m_colors["accentPressedColor"] = QColor("#D35400");
    m_colors["iconColor"] = QColor("#E67E22");
    m_colors["gridColor"] = QColor("#3c3c3c");
    m_colors["borderColor"] = QColor("#181818");
    m_colors["panelColor"] = QColor("#2c2130");
    m_colors["controlBackground"] = QColor("#241d2d");
    m_colors["welcomeBgColor"] = QColor("#ff5e16");
    m_colors["welcomeBtnNewGradientStart"] = QColor("#FF69B4");
    m_colors["welcomeBtnNewGradientEnd"] = QColor("#00CED1");
    m_colors["welcomeBtnOpenColor"] = QColor("#ADFF2F");
    m_colors["welcomeTextDark"] = QColor("#141414");
    m_colors["welcomeAccentYellow"] = QColor("#FFE135");
}

void ThemeManager::loadTheme(const QString &themeName)
{
    resetToDefaults();

    const QString themesPath = QCoreApplication::applicationDirPath() + "/themes";
    const QString themeDir = themesPath + "/" + themeName;
    QFile file(themeDir + "/theme.json");

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open theme file:" << file.fileName();
        return;
    }

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Unable to parse theme file:" << error.errorString();
        return;
    }

    const QJsonObject root = document.object();
    const QJsonObject colorsObject = root.value("colors").toObject();
    for (auto it = colorsObject.begin(); it != colorsObject.end(); ++it) {
        m_colors[it.key()] = QColor(it.value().toString());
    }

    const QJsonObject iconsObject = root.value("icons").toObject();
    for (auto it = iconsObject.begin(); it != iconsObject.end(); ++it) {
        m_icons[it.key()] = "file://" + themeDir + "/" + it.value().toString();
    }
}
