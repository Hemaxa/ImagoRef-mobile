#pragma once

#include <QObject>
#include <QVariantList>

class AppState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList accounts READ accounts NOTIFY accountsChanged)
    Q_PROPERTY(QVariantList boards READ boards NOTIFY boardsChanged)
    Q_PROPERTY(QString selectedAccountId READ selectedAccountId NOTIFY selectedAccountChanged)
    Q_PROPERTY(QString selectedAccountName READ selectedAccountName NOTIFY selectedAccountChanged)
    Q_PROPERTY(QString selectedBoardId READ selectedBoardId NOTIFY selectedBoardChanged)
    Q_PROPERTY(QString selectedBoardName READ selectedBoardName NOTIFY selectedBoardChanged)
    Q_PROPERTY(QString syncStatus READ syncStatus NOTIFY syncStatusChanged)

public:
    explicit AppState(QObject *parent = nullptr);

    QVariantList accounts() const;
    QVariantList boards() const;
    QString selectedAccountId() const;
    QString selectedAccountName() const;
    QString selectedBoardId() const;
    QString selectedBoardName() const;
    QString syncStatus() const;

    Q_INVOKABLE void selectAccount(const QString &accountId);
    Q_INVOKABLE void selectBoard(const QString &boardId);
    Q_INVOKABLE void refreshBoards();

signals:
    void accountsChanged();
    void boardsChanged();
    void selectedAccountChanged();
    void selectedBoardChanged();
    void syncStatusChanged();

private:
    void loadMockData();
    void rebuildBoards();
    QVariantMap boardById(const QString &boardId) const;

    QVariantList m_accounts;
    QVariantList m_boards;
    QString m_selectedAccountId;
    QString m_selectedBoardId;
    QString m_syncStatus;
};
