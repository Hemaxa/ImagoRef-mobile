#pragma once

#include <QObject>
#include <QVariantMap>

class ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentTheme READ currentTheme NOTIFY themeChanged)
    Q_PROPERTY(QVariantMap colors READ colors NOTIFY themeChanged)
    Q_PROPERTY(QVariantMap icons READ icons NOTIFY themeChanged)

public:
    explicit ThemeManager(QObject *parent = nullptr);

    Q_INVOKABLE void applyTheme(const QString &themeName);

    QString currentTheme() const;
    QVariantMap colors() const;
    QVariantMap icons() const;

signals:
    void themeChanged();

private:
    void resetToDefaults();
    void loadTheme(const QString &themeName);

    QString m_currentTheme;
    QVariantMap m_colors;
    QVariantMap m_icons;
};
