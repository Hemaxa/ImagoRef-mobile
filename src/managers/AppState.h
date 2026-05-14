#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "BoardItemsModel.h"

class QAbstractItemModel;
class QJsonArray;
class QJsonObject;
class QNetworkReply;

class BoardsListModel;
struct BoardItemEntry;

class AppState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY authChanged)
    Q_PROPERTY(bool authenticating READ authenticating NOTIFY busyChanged)
    Q_PROPERTY(bool loadingBoards READ loadingBoards NOTIFY busyChanged)
    Q_PROPERTY(bool loadingBoard READ loadingBoard NOTIFY busyChanged)
    Q_PROPERTY(QString userEmail READ userEmail NOTIFY authChanged)
    Q_PROPERTY(QString userNickname READ userNickname NOTIFY authChanged)
    Q_PROPERTY(QString userDisplayName READ userDisplayName NOTIFY authChanged)
    Q_PROPERTY(QString userAvatarUrl READ userAvatarUrl NOTIFY authChanged)
    Q_PROPERTY(QString selectedBoardId READ selectedBoardId NOTIFY selectedBoardChanged)
    Q_PROPERTY(QString selectedBoardName READ selectedBoardName NOTIFY selectedBoardChanged)
    Q_PROPERTY(QString syncStatus READ syncStatus NOTIFY statusChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY statusChanged)
    Q_PROPERTY(qreal boardCanvasWidth READ boardCanvasWidth NOTIFY boardSceneChanged)
    Q_PROPERTY(qreal boardCanvasHeight READ boardCanvasHeight NOTIFY boardSceneChanged)
    Q_PROPERTY(qreal boardOriginX READ boardOriginX NOTIFY boardSceneChanged)
    Q_PROPERTY(qreal boardOriginY READ boardOriginY NOTIFY boardSceneChanged)
    Q_PROPERTY(QAbstractItemModel *boardsModel READ boardsModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel *boardItemsModel READ boardItemsModel CONSTANT)

public:
    explicit AppState(QObject *parent = nullptr);

    bool loggedIn() const;
    bool authenticating() const;
    bool loadingBoards() const;
    bool loadingBoard() const;
    QString userEmail() const;
    QString userNickname() const;
    QString userDisplayName() const;
    QString userAvatarUrl() const;
    QString selectedBoardId() const;
    QString selectedBoardName() const;
    QString syncStatus() const;
    QString errorMessage() const;
    qreal boardCanvasWidth() const;
    qreal boardCanvasHeight() const;
    qreal boardOriginX() const;
    qreal boardOriginY() const;
    QAbstractItemModel *boardsModel() const;
    QAbstractItemModel *boardItemsModel() const;

    Q_INVOKABLE void login(const QString &email, const QString &password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void loadBoards();
    Q_INVOKABLE void openBoard(const QString &boardId, const QString &boardTitle);
    Q_INVOKABLE void closeBoard();
    Q_INVOKABLE void refreshCurrentBoard();

signals:
    void authChanged();
    void busyChanged();
    void selectedBoardChanged();
    void statusChanged();
    void boardSceneChanged();

private:
    void applyUserResponse(const QJsonObject &object, bool preserveExisting = true);
    void restoreSession();
    void persistSession() const;
    void clearSession();
    void refreshProfile();
    void updateStatus(const QString &message);
    void updateError(const QString &message);
    void setAuthenticating(bool value);
    void setLoadingBoards(bool value);
    void setLoadingBoard(bool value);
    void handleUnauthorized();

    QNetworkRequest authorizedRequest(const QString &path) const;
    QString extractErrorMessage(QNetworkReply *reply, const QString &fallback) const;
    void parseBoardsResponse(const QByteArray &data);
    void parseBoardMetadata(const QByteArray &data);
    void computeBoardScene(const QVector<BoardItemEntry> &items);
    QString resolveImageSource(const QString &hash, const QString &downloadUrl);
    void downloadImage(const QString &hash, const QString &downloadUrl);
    void fetchBoardPreview(const QString &boardId);
    QString formatUpdatedLabel(const QJsonObject &boardObject) const;
    QString formatBoardSubtitle(const QJsonObject &boardObject) const;
    QDateTime parseApiDateTime(const QString &value) const;

    BoardsListModel *m_boardsModel;
    BoardItemsModel *m_boardItemsModel;
    QNetworkAccessManager m_networkManager;
    QString m_token;
    QString m_userEmail;
    QString m_userNickname;
    QString m_userAvatarHash;
    QString m_userAvatarUrl;
    QString m_selectedBoardId;
    QString m_selectedBoardName;
    QString m_syncStatus;
    QString m_errorMessage;
    bool m_authenticating = false;
    bool m_loadingBoards = false;
    bool m_loadingBoard = false;
    qreal m_boardCanvasWidth = 1200;
    qreal m_boardCanvasHeight = 800;
    qreal m_boardOriginX = 0;
    qreal m_boardOriginY = 0;

    static const QString kApiBaseUrl;
};
