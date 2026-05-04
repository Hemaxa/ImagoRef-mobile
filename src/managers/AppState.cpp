#include "AppState.h"

#include <QVariantMap>

namespace {

QVariantMap makeAccount(const QString &id,
                        const QString &name,
                        const QString &handle,
                        const QString &accent)
{
    QVariantMap account;
    account["id"] = id;
    account["name"] = name;
    account["handle"] = handle;
    account["accent"] = accent;
    account["initials"] = name.left(1).toUpper();
    return account;
}

QVariantMap makeBoard(const QString &id,
                      const QString &name,
                      const QString &subtitle,
                      const QString &accentA,
                      const QString &accentB,
                      int collaborators)
{
    QVariantMap board;
    board["id"] = id;
    board["name"] = name;
    board["subtitle"] = subtitle;
    board["accentA"] = accentA;
    board["accentB"] = accentB;
    board["collaborators"] = collaborators;
    board["state"] = "Synced";
    board["updatedLabel"] = "Updated from desktop";
    return board;
}

}

AppState::AppState(QObject *parent)
    : QObject(parent)
{
    loadMockData();
}

QVariantList AppState::accounts() const
{
    return m_accounts;
}

QVariantList AppState::boards() const
{
    return m_boards;
}

QString AppState::selectedAccountId() const
{
    return m_selectedAccountId;
}

QString AppState::selectedAccountName() const
{
    for (const QVariant &item : m_accounts) {
        const QVariantMap accountMap = item.toMap();
        if (accountMap.value("id").toString() == m_selectedAccountId) {
            return accountMap.value("name").toString();
        }
    }

    return QString();
}

QString AppState::selectedBoardId() const
{
    return m_selectedBoardId;
}

QString AppState::selectedBoardName() const
{
    return boardById(m_selectedBoardId).value("name").toString();
}

QString AppState::syncStatus() const
{
    return m_syncStatus;
}

void AppState::selectAccount(const QString &accountId)
{
    if (accountId.isEmpty() || accountId == m_selectedAccountId) {
        return;
    }

    m_selectedAccountId = accountId;
    rebuildBoards();
    emit selectedAccountChanged();
}

void AppState::selectBoard(const QString &boardId)
{
    if (boardId.isEmpty()) {
        return;
    }

    m_selectedBoardId = boardId;
    const QString boardName = selectedBoardName();
    m_syncStatus = QString("%1 is ready for synced viewing on mobile. Editing stays on desktop.")
                       .arg(boardName);
    emit selectedBoardChanged();
    emit syncStatusChanged();
}

void AppState::refreshBoards()
{
    const QString accountName = selectedAccountName();
    m_syncStatus = QString("Latest desktop changes for %1 were synced just now.")
                       .arg(accountName.isEmpty() ? "your workspace" : accountName);
    emit syncStatusChanged();
    emit boardsChanged();
}

void AppState::loadMockData()
{
    m_accounts = {
        makeAccount("artist", "Sergei", "@sergei", "#FFE135"),
        makeAccount("team", "Studio Team", "@imagoref-team", "#00CED1"),
        makeAccount("guest", "Guest View", "@guest-sync", "#FF69B4")
    };

    m_selectedAccountId = "artist";
    rebuildBoards();
    m_selectedBoardId = "moodboard";
    m_syncStatus = "Latest desktop board is available on mobile for browsing and sync status.";
}

void AppState::rebuildBoards()
{
    if (m_selectedAccountId == "team") {
        m_boards = {
            makeBoard("campaign", "Spring Campaign", "Shared references for launch art", "#7C3AED", "#00CED1", 5),
            makeBoard("ui-kit", "Mobile UI Board", "App screens and interaction references", "#FF7A18", "#FFE135", 4),
            makeBoard("materials", "Material Study", "Fabric, metal and glass inspirations", "#1D976C", "#93F9B9", 3)
        };
    } else if (m_selectedAccountId == "guest") {
        m_boards = {
            makeBoard("review", "Review Board", "Read-only synced selection", "#3A1C71", "#D76D77", 2),
            makeBoard("favorites", "Favorite Frames", "Pinned results from desktop boards", "#0F2027", "#2C5364", 1)
        };
    } else {
        m_boards = {
            makeBoard("moodboard", "Character Moodboard", "Color, silhouettes and costume references", "#FF4E50", "#F9D423", 3),
            makeBoard("lighting", "Lighting Notes", "Evening ambience and cinematic warmups", "#614385", "#516395", 2),
            makeBoard("props", "Prop Details", "Small forms, wear patterns and materials", "#F7971E", "#FFD200", 2),
            makeBoard("shots", "Shot Planning", "Storyboard references synced from desktop", "#0575E6", "#00F260", 4)
        };
    }

    if (m_boards.isEmpty()) {
        m_selectedBoardId.clear();
    } else if (boardById(m_selectedBoardId).isEmpty()) {
        m_selectedBoardId = m_boards.first().toMap().value("id").toString();
    }

    emit boardsChanged();
    emit selectedBoardChanged();
}

QVariantMap AppState::boardById(const QString &boardId) const
{
    for (const QVariant &item : m_boards) {
        const QVariantMap board = item.toMap();
        if (board.value("id").toString() == boardId) {
            return board;
        }
    }

    return {};
}
