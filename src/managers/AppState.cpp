#include "AppState.h"

#include "BoardItemsModel.h"
#include "BoardsListModel.h"
#include "CacheManager.h"

#include <QAbstractItemModel>
#include <QDateTime>
#include <QDir>
#include <QFont>
#include <QFontMetricsF>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QPainter>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QSettings>
#include <QTransform>
#include <QTimeZone>
#include <QUrl>
#include <array>
#include <algorithm>
#include <cmath>

const QString AppState::kApiBaseUrl = QStringLiteral("https://imagoref.ru/api");

namespace {

QString valueAsString(const QJsonObject &object, const QStringList &keys)
{
    for (const QString &key : keys) {
        const QJsonValue value = object.value(key);
        if (value.isString()) {
            const QString text = value.toString().trimmed();
            if (!text.isEmpty()) {
                return text;
            }
        }
    }

    return {};
}

int valueAsInt(const QJsonObject &object, const QStringList &keys, int fallback = 0)
{
    for (const QString &key : keys) {
        const QJsonValue value = object.value(key);
        if (value.isDouble()) {
            return value.toInt();
        }
        if (value.isString()) {
            bool ok = false;
            const int parsed = value.toString().toInt(&ok);
            if (ok) {
                return parsed;
            }
        }
    }

    return fallback;
}

qreal valueAsReal(const QJsonObject &object, const QStringList &keys, qreal fallback = 0.0)
{
    for (const QString &key : keys) {
        const QJsonValue value = object.value(key);
        if (value.isDouble()) {
            return value.toDouble();
        }
        if (value.isString()) {
            bool ok = false;
            const qreal parsed = value.toString().toDouble(&ok);
            if (ok) {
                return parsed;
            }
        }
    }

    return fallback;
}

QJsonArray extractArray(const QJsonDocument &document, const QStringList &topLevelKeys, const QStringList &nestedKeys = {})
{
    if (document.isArray()) {
        return document.array();
    }

    if (!document.isObject()) {
        return {};
    }

    const QJsonObject root = document.object();
    for (const QString &key : topLevelKeys) {
        if (root.value(key).isArray()) {
            return root.value(key).toArray();
        }
    }

    for (const QString &key : nestedKeys) {
        if (root.value("data").isObject()) {
            const QJsonObject data = root.value("data").toObject();
            if (data.value(key).isArray()) {
                return data.value(key).toArray();
            }
        }
    }

    return {};
}

QJsonObject extractObject(const QJsonDocument &document)
{
    if (document.isObject()) {
        return document.object();
    }

    return {};
}

QString formatDateTime(const QDateTime &dateTime)
{
    if (!dateTime.isValid()) {
        return QStringLiteral("Синхронизировано с облаком");
    }

    return QStringLiteral("Обновлено %1").arg(dateTime.toLocalTime().toString("dd.MM.yyyy HH:mm"));
}

}

AppState::AppState(QObject *parent)
    : QObject(parent)
    , m_boardsModel(new BoardsListModel(this))
    , m_boardItemsModel(new BoardItemsModel(this))
{
    restoreSession();
    if (loggedIn()) {
        refreshProfile();
        loadBoards();
    } else {
        updateStatus("Войдите, чтобы открыть облачные доски на мобильном устройстве.");
    }
}

bool AppState::loggedIn() const
{
    return !m_token.isEmpty();
}

bool AppState::authenticating() const
{
    return m_authenticating;
}

bool AppState::loadingBoards() const
{
    return m_loadingBoards;
}

bool AppState::loadingBoard() const
{
    return m_loadingBoard;
}

QString AppState::userEmail() const
{
    return m_userEmail;
}

QString AppState::userNickname() const
{
    return m_userNickname;
}

QString AppState::userDisplayName() const
{
    return m_userNickname.isEmpty() ? m_userEmail : m_userNickname;
}

QString AppState::userAvatarUrl() const
{
    if (!m_userAvatarUrl.isEmpty()) {
        return m_userAvatarUrl;
    }

    return m_userAvatarHash.isEmpty()
        ? QString()
        : QStringLiteral("https://imagoref.s3.timeweb.com/%1").arg(m_userAvatarHash);
}

QString AppState::selectedBoardId() const
{
    return m_selectedBoardId;
}

QString AppState::selectedBoardName() const
{
    return m_selectedBoardName;
}

QString AppState::syncStatus() const
{
    return m_syncStatus;
}

QString AppState::errorMessage() const
{
    return m_errorMessage;
}

qreal AppState::boardCanvasWidth() const
{
    return m_boardCanvasWidth;
}

qreal AppState::boardCanvasHeight() const
{
    return m_boardCanvasHeight;
}

qreal AppState::boardOriginX() const
{
    return m_boardOriginX;
}

qreal AppState::boardOriginY() const
{
    return m_boardOriginY;
}

QAbstractItemModel *AppState::boardsModel() const
{
    return m_boardsModel;
}

QAbstractItemModel *AppState::boardItemsModel() const
{
    return m_boardItemsModel;
}

void AppState::login(const QString &email, const QString &password)
{
    const QString trimmedEmail = email.trimmed();
    if (trimmedEmail.isEmpty() || password.isEmpty()) {
        updateError("Введите email и пароль.");
        return;
    }

    updateError({});
    setAuthenticating(true);
    updateStatus("Выполняется вход...");

    QJsonObject payload;
    payload["email"] = trimmedEmail;
    payload["password"] = password;

    QNetworkRequest request(QUrl(kApiBaseUrl + "/auth/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_networkManager.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        setAuthenticating(false);

        if (reply->error() != QNetworkReply::NoError) {
            updateError(extractErrorMessage(reply, "Не удалось выполнить вход."));
            updateStatus("Не удалось войти в аккаунт.");
            return;
        }

        const QJsonObject object = extractObject(QJsonDocument::fromJson(reply->readAll()));
        m_token = valueAsString(object, {"access_token", "token"});
        applyUserResponse(object, false);

        if (m_token.isEmpty()) {
            updateError("Сервер не вернул токен доступа.");
            updateStatus("Не удалось войти в аккаунт.");
            return;
        }

        persistSession();
        emit authChanged();
        updateStatus(QString("Вы вошли как %1. Загружаем доски...").arg(userDisplayName()));
        loadBoards();
    });
}

void AppState::logout()
{
    clearSession();
    m_boardsModel->clear();
    m_boardItemsModel->clear();
    m_selectedBoardId.clear();
    m_selectedBoardName.clear();
    m_boardCanvasWidth = 1200;
    m_boardCanvasHeight = 800;
    m_boardOriginX = 0;
    m_boardOriginY = 0;
    emit boardSceneChanged();
    emit selectedBoardChanged();
    emit authChanged();
    updateError({});
    updateStatus("Вы вышли из аккаунта. Войдите снова, чтобы открыть синхронизированные доски.");
}

void AppState::loadBoards()
{
    if (!loggedIn()) {
        return;
    }

    updateError({});
    setLoadingBoards(true);

    QNetworkReply *reply = m_networkManager.get(authorizedRequest("/boards/"));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        setLoadingBoards(false);

        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401) {
            handleUnauthorized();
            return;
        }

        if (reply->error() != QNetworkReply::NoError) {
            updateError(extractErrorMessage(reply, "Не удалось загрузить список досок."));
            updateStatus("Не удалось загрузить облачные доски.");
            return;
        }

        parseBoardsResponse(reply->readAll());
    });
}

void AppState::openBoard(const QString &boardId, const QString &boardTitle)
{
    if (!loggedIn() || boardId.isEmpty()) {
        return;
    }

    m_selectedBoardId = boardId;
    m_selectedBoardName = boardTitle;
    emit selectedBoardChanged();

    m_boardItemsModel->clear();
    updateError({});
    setLoadingBoard(true);
    updateStatus(QString("Загружаем доску «%1»...").arg(boardTitle));

    QNetworkReply *reply = m_networkManager.get(authorizedRequest("/boards/" + boardId + "/metadata"));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        setLoadingBoard(false);

        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401) {
            handleUnauthorized();
            return;
        }

        if (reply->error() != QNetworkReply::NoError) {
            updateError(extractErrorMessage(reply, "Не удалось открыть доску."));
            updateStatus("Не удалось загрузить данные доски.");
            return;
        }

        parseBoardMetadata(reply->readAll());
    });
}

void AppState::closeBoard()
{
    m_selectedBoardId.clear();
    m_selectedBoardName.clear();
    m_boardItemsModel->clear();
    m_boardCanvasWidth = 1200;
    m_boardCanvasHeight = 800;
    m_boardOriginX = 0;
    m_boardOriginY = 0;
    emit boardSceneChanged();
    emit selectedBoardChanged();
    updateError({});
    updateStatus("Выберите доску, чтобы открыть синхронизированные референсы.");
}

void AppState::refreshCurrentBoard()
{
    if (!m_selectedBoardId.isEmpty()) {
        openBoard(m_selectedBoardId, m_selectedBoardName);
    } else {
        loadBoards();
    }
}

void AppState::restoreSession()
{
    QSettings settings("ImagoRef", "ImagoRefMobile");
    m_token = settings.value("auth/token").toString();
    m_userEmail = settings.value("auth/email").toString();
    m_userNickname = settings.value("auth/nickname").toString();
    m_userAvatarHash = settings.value("auth/avatar_hash").toString();
}

void AppState::persistSession() const
{
    QSettings settings("ImagoRef", "ImagoRefMobile");
    settings.setValue("auth/token", m_token);
    settings.setValue("auth/email", m_userEmail);
    settings.setValue("auth/nickname", m_userNickname);
    settings.setValue("auth/avatar_hash", m_userAvatarHash);
}

void AppState::clearSession()
{
    m_token.clear();
    m_userEmail.clear();
    m_userNickname.clear();
    m_userAvatarHash.clear();
    m_userAvatarUrl.clear();

    QSettings settings("ImagoRef", "ImagoRefMobile");
    settings.remove("auth");
}

void AppState::refreshProfile()
{
    if (!loggedIn()) {
        m_userAvatarUrl.clear();
        return;
    }

    QNetworkReply *reply = m_networkManager.get(authorizedRequest("/users/me"));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401) {
            handleUnauthorized();
            return;
        }

        if (reply->error() != QNetworkReply::NoError) {
            return;
        }

        applyUserResponse(extractObject(QJsonDocument::fromJson(reply->readAll())), true);
        emit authChanged();
    });
}

void AppState::updateStatus(const QString &message)
{
    if (m_syncStatus == message) {
        return;
    }

    m_syncStatus = message;
    emit statusChanged();
}

void AppState::updateError(const QString &message)
{
    if (m_errorMessage == message) {
        return;
    }

    m_errorMessage = message;
    emit statusChanged();
}

void AppState::setAuthenticating(bool value)
{
    if (m_authenticating == value) {
        return;
    }

    m_authenticating = value;
    emit busyChanged();
}

void AppState::setLoadingBoards(bool value)
{
    if (m_loadingBoards == value) {
        return;
    }

    m_loadingBoards = value;
    emit busyChanged();
}

void AppState::setLoadingBoard(bool value)
{
    if (m_loadingBoard == value) {
        return;
    }

    m_loadingBoard = value;
    emit busyChanged();
}

void AppState::handleUnauthorized()
{
    logout();
    updateError("Сессия истекла. Пожалуйста, войдите снова.");
}

QNetworkRequest AppState::authorizedRequest(const QString &path) const
{
    QNetworkRequest request(QUrl(kApiBaseUrl + path));
    request.setRawHeader("Authorization", QByteArray("Bearer ") + m_token.toUtf8());
    return request;
}

QString AppState::extractErrorMessage(QNetworkReply *reply, const QString &fallback) const
{
    const QByteArray body = reply->readAll();
    const QJsonObject object = extractObject(QJsonDocument::fromJson(body));

    QString message = valueAsString(object, {"detail", "message", "error"});
    if (!message.isEmpty()) {
        return message;
    }

    return fallback;
}

void AppState::applyUserResponse(const QJsonObject &object, bool preserveExisting)
{
    const QString email = valueAsString(object, {"email"});
    const QString nickname = valueAsString(object, {"nickname", "name"});
    const QString avatarHash = valueAsString(object, {"avatar_hash", "avatarHash"});
    const QString avatarUrl = valueAsString(object, {"avatar_url", "avatarUrl"});

    if (!email.isEmpty() || !preserveExisting) {
        m_userEmail = email;
    }

    if (!nickname.isEmpty() || !preserveExisting || object.contains("nickname") || object.contains("name")) {
        m_userNickname = nickname;
    }

    if (!avatarHash.isEmpty() || !preserveExisting || object.contains("avatar_hash") || object.contains("avatarHash")) {
        m_userAvatarHash = avatarHash;
    }

    if (!avatarUrl.isEmpty() || !preserveExisting || object.contains("avatar_url") || object.contains("avatarUrl")) {
        m_userAvatarUrl = avatarUrl;
    }
}

void AppState::parseBoardsResponse(const QByteArray &data)
{
    const QJsonDocument document = QJsonDocument::fromJson(data);
    const QJsonArray boardsArray = extractArray(document, {"boards", "data"}, {"boards"});

    QVector<BoardEntry> boards;
    boards.reserve(boardsArray.size());

    for (const QJsonValue &value : boardsArray) {
        const QJsonObject object = value.toObject();
        BoardEntry entry;
        entry.id = valueAsString(object, {"id", "board_id"});
        entry.title = valueAsString(object, {"title", "name"});
        entry.subtitle = formatBoardSubtitle(object);
        entry.updatedLabel = formatUpdatedLabel(object);
        entry.itemCount = valueAsInt(object, {"items_count", "item_count", "images_count"}, 0);
        entry.previewSource = valueAsString(object.value("data").toObject(), {"preview_url", "previewUrl"});

        if (!entry.id.isEmpty()) {
            if (entry.title.isEmpty()) {
                entry.title = "Без названия";
            }
            if (entry.subtitle.isEmpty()) {
                entry.subtitle = "Синхронизированная доска";
            }
            boards.append(entry);
        }
    }

    m_boardsModel->setBoards(boards);
    updateStatus(boards.isEmpty()
                     ? "Для этого аккаунта пока нет облачных досок."
                     : QString("Загружено досок из облака: %1.").arg(boards.size()));

    for (const BoardEntry &board : boards) {
        if (board.previewSource.isEmpty()) {
            fetchBoardPreview(board.id);
        }
    }
}

void AppState::parseBoardMetadata(const QByteArray &data)
{
    const QJsonDocument document = QJsonDocument::fromJson(data);
    const QJsonObject root = extractObject(document);
    QJsonObject dataObject = root;
    if (root.value("data").isObject()) {
        dataObject = root.value("data").toObject();
    }

    QJsonArray itemsArray;
    if (dataObject.value("items").isArray()) {
        itemsArray = dataObject.value("items").toArray();
    } else if (root.value("items").isArray()) {
        itemsArray = root.value("items").toArray();
    }

    QJsonObject downloadUrls = root.value("download_urls").toObject();
    if (downloadUrls.isEmpty() && dataObject.value("download_urls").isObject()) {
        downloadUrls = dataObject.value("download_urls").toObject();
    }

    QVector<BoardItemEntry> items;
    items.reserve(itemsArray.size());

    for (const QJsonValue &value : itemsArray) {
        const QJsonObject object = value.toObject();
        const QJsonObject payload = object.value("payload").toObject();

        BoardItemEntry entry;
        entry.id = valueAsString(object, {"id", "item_id"});
        entry.imageHash = valueAsString(payload, {"imageHash", "image_hash"});
        entry.x = valueAsReal(object, {"x", "pos_x"});
        entry.y = valueAsReal(object, {"y", "pos_y"});
        entry.width = qMax<qreal>(valueAsReal(object, {"width"}, 240.0), 32.0);
        entry.height = qMax<qreal>(valueAsReal(object, {"height"}, 240.0), 32.0);
        entry.rotation = valueAsReal(payload, {"rotation"});
        entry.opacity = valueAsReal(payload, {"opacity"}, 1.0);
        entry.cropX = valueAsReal(payload, {"cropX", "crop_x"});
        entry.cropY = valueAsReal(payload, {"cropY", "crop_y"});
        entry.cropWidth = valueAsReal(payload, {"cropWidth", "crop_width"});
        entry.cropHeight = valueAsReal(payload, {"cropHeight", "crop_height"});
        entry.zIndex = valueAsInt(object, {"z_index", "zValue"});
        entry.label = valueAsString(payload, {"label"});

        const QString downloadUrl = downloadUrls.value(entry.imageHash).toString();
        entry.source = resolveImageSource(entry.imageHash, downloadUrl);

        if (!entry.imageHash.isEmpty() && !downloadUrl.isEmpty()) {
            downloadImage(entry.imageHash, downloadUrl);
        }

        if (!entry.id.isEmpty()) {
            items.append(entry);
        }
    }

    m_boardItemsModel->setItems(items);
    computeBoardScene(items);
    updateStatus(items.isEmpty()
                     ? QString("Доска «%1» пока пустая.").arg(m_selectedBoardName)
                     : QString("Доска «%1» загружена. Изображений: %2.").arg(m_selectedBoardName).arg(items.size()));
}

void AppState::computeBoardScene(const QVector<BoardItemEntry> &items)
{
    if (items.isEmpty()) {
        m_boardCanvasWidth = 1200;
        m_boardCanvasHeight = 800;
        m_boardOriginX = 0;
        m_boardOriginY = 0;
        emit boardSceneChanged();
        return;
    }

    auto updateBounds = [](qreal left, qreal top, qreal right, qreal bottom,
                           qreal &minX, qreal &minY, qreal &maxX, qreal &maxY, bool &hasBounds) {
        if (!hasBounds) {
            minX = left;
            minY = top;
            maxX = right;
            maxY = bottom;
            hasBounds = true;
            return;
        }

        minX = qMin(minX, left);
        minY = qMin(minY, top);
        maxX = qMax(maxX, right);
        maxY = qMax(maxY, bottom);
    };

    QFont labelFont;
    labelFont.setPixelSize(14);
    const QFontMetricsF labelMetrics(labelFont);
    const qreal labelHorizontalPadding = 14.0;
    const qreal labelVerticalPadding = 8.0;
    const qreal labelGap = 4.0;
    constexpr qreal pi = 3.14159265358979323846;

    qreal minX = 0;
    qreal minY = 0;
    qreal maxX = 0;
    qreal maxY = 0;
    bool hasBounds = false;

    for (const BoardItemEntry &item : items) {
        const qreal centerX = item.x + item.width / 2.0;
        const qreal centerY = item.y + item.height / 2.0;
        const qreal radians = item.rotation * pi / 180.0;
        const qreal cosValue = std::cos(radians);
        const qreal sinValue = std::sin(radians);

        struct CornerOffset {
            qreal x = 0;
            qreal y = 0;
        };

        const qreal halfWidth = item.width / 2.0;
        const qreal halfHeight = item.height / 2.0;
        const std::array<CornerOffset, 4> corners = {{
            {-halfWidth * cosValue + halfHeight * sinValue, -halfWidth * sinValue - halfHeight * cosValue},
            {halfWidth * cosValue + halfHeight * sinValue, halfWidth * sinValue - halfHeight * cosValue},
            {halfWidth * cosValue - halfHeight * sinValue, halfWidth * sinValue + halfHeight * cosValue},
            {-halfWidth * cosValue - halfHeight * sinValue, -halfWidth * sinValue + halfHeight * cosValue}
        }};

        qreal itemMinSceneX = centerX + corners[0].x;
        qreal itemMaxSceneX = itemMinSceneX;
        qreal itemMinSceneY = centerY + corners[0].y;
        qreal itemMaxSceneY = itemMinSceneY;

        for (const CornerOffset &corner : corners) {
            itemMinSceneX = qMin(itemMinSceneX, centerX + corner.x);
            itemMaxSceneX = qMax(itemMaxSceneX, centerX + corner.x);
            itemMinSceneY = qMin(itemMinSceneY, centerY + corner.y);
            itemMaxSceneY = qMax(itemMaxSceneY, centerY + corner.y);
        }

        updateBounds(itemMinSceneX, itemMinSceneY, itemMaxSceneX, itemMaxSceneY,
                     minX, minY, maxX, maxY, hasBounds);

        if (!item.label.isEmpty()) {
            const qreal labelWidth = labelMetrics.horizontalAdvance(item.label) + labelHorizontalPadding;
            const qreal labelHeight = labelMetrics.height() + labelVerticalPadding;
            const qreal labelLeft = centerX + (itemMinSceneX - centerX);
            const qreal labelTop = centerY + (itemMinSceneY - centerY) - labelHeight - labelGap;

            updateBounds(labelLeft, labelTop, labelLeft + labelWidth, labelTop + labelHeight,
                         minX, minY, maxX, maxY, hasBounds);
        }
    }

    const qreal padding = 64.0;
    m_boardOriginX = minX - padding;
    m_boardOriginY = minY - padding;
    m_boardCanvasWidth = qMax<qreal>((maxX - minX) + padding * 2.0, 640.0);
    m_boardCanvasHeight = qMax<qreal>((maxY - minY) + padding * 2.0, 420.0);
    emit boardSceneChanged();
}

QString AppState::resolveImageSource(const QString &hash, const QString &downloadUrl)
{
    if (!hash.isEmpty() && CacheManager::instance().isCached(hash)) {
        return QUrl::fromLocalFile(CacheManager::instance().cacheFilePath(hash)).toString();
    }

    return downloadUrl;
}

void AppState::downloadImage(const QString &hash, const QString &downloadUrl)
{
    if (hash.isEmpty() || downloadUrl.isEmpty() || CacheManager::instance().isCached(hash)) {
        return;
    }

    QNetworkReply *reply = m_networkManager.get(QNetworkRequest(QUrl(downloadUrl)));
    connect(reply, &QNetworkReply::finished, this, [this, reply, hash]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            return;
        }

        const QByteArray imageData = reply->readAll();
        CacheManager::instance().saveToCache(hash, imageData);
        const QString localSource = QUrl::fromLocalFile(CacheManager::instance().cacheFilePath(hash)).toString();
        m_boardItemsModel->updateSourceForHash(hash, localSource);
        m_boardsModel->updatePreviewItemSourceForHash(hash, localSource);
    });
}

void AppState::fetchBoardPreview(const QString &boardId)
{
    if (!loggedIn() || boardId.isEmpty()) {
        return;
    }

    QNetworkReply *reply = m_networkManager.get(authorizedRequest("/boards/" + boardId + "/metadata"));
    connect(reply, &QNetworkReply::finished, this, [this, reply, boardId]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            return;
        }

        const QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject root = extractObject(document);
        QJsonObject dataObject = root;
        if (root.value("data").isObject()) {
            dataObject = root.value("data").toObject();
        }

        QJsonArray itemsArray;
        if (dataObject.value("items").isArray()) {
            itemsArray = dataObject.value("items").toArray();
        } else if (root.value("items").isArray()) {
            itemsArray = root.value("items").toArray();
        }

        QJsonObject downloadUrls = root.value("download_urls").toObject();
        if (downloadUrls.isEmpty() && dataObject.value("download_urls").isObject()) {
            downloadUrls = dataObject.value("download_urls").toObject();
        }

        struct PreviewItemData {
            QString id;
            QString hash;
            QString source;
            qreal x = 0;
            qreal y = 0;
            qreal width = 0;
            qreal height = 0;
            qreal rotation = 0;
            qreal opacity = 1.0;
            int z = 0;
        };

        QVector<PreviewItemData> previewData;
        previewData.reserve(itemsArray.size());

        QString previewSource;
        qreal minX = 0;
        qreal minY = 0;
        qreal maxX = 0;
        qreal maxY = 0;
        bool hasBounds = false;

        for (const QJsonValue &value : itemsArray) {
            const QJsonObject itemObject = value.toObject();
            const QJsonObject payload = itemObject.value("payload").toObject();
            const QString hash = valueAsString(payload, {"imageHash", "image_hash"});
            const QString downloadUrl = downloadUrls.value(hash).toString();
            const QString source = resolveImageSource(hash, downloadUrl);

            if (previewSource.isEmpty() && !source.isEmpty()) {
                previewSource = source;
            }

            if (!hash.isEmpty() && !downloadUrl.isEmpty()) {
                downloadImage(hash, downloadUrl);
            }

            if (source.isEmpty()) {
                continue;
            }

            PreviewItemData item;
            item.id = valueAsString(itemObject, {"id", "item_id"});
            item.hash = hash;
            item.source = source;
            item.x = valueAsReal(itemObject, {"x", "pos_x"});
            item.y = valueAsReal(itemObject, {"y", "pos_y"});
            item.width = qMax<qreal>(valueAsReal(itemObject, {"width"}, 240.0), 24.0);
            item.height = qMax<qreal>(valueAsReal(itemObject, {"height"}, 240.0), 24.0);
            item.rotation = valueAsReal(payload, {"rotation"});
            item.opacity = valueAsReal(payload, {"opacity"}, 1.0);
            item.z = valueAsInt(itemObject, {"z_index", "zValue"});

            constexpr qreal pi = 3.14159265358979323846;
            const qreal centerX = item.x + item.width / 2.0;
            const qreal centerY = item.y + item.height / 2.0;
            const qreal radians = item.rotation * pi / 180.0;
            const qreal cosValue = std::cos(radians);
            const qreal sinValue = std::sin(radians);
            const qreal halfWidth = item.width / 2.0;
            const qreal halfHeight = item.height / 2.0;

            const std::array<QPointF, 4> corners = {{
                QPointF(-halfWidth, -halfHeight),
                QPointF(halfWidth, -halfHeight),
                QPointF(halfWidth, halfHeight),
                QPointF(-halfWidth, halfHeight)
            }};

            qreal itemMinX = 0;
            qreal itemMaxX = 0;
            qreal itemMinY = 0;
            qreal itemMaxY = 0;
            bool firstCorner = true;

            for (const QPointF &corner : corners) {
                const qreal rotatedX = centerX + corner.x() * cosValue - corner.y() * sinValue;
                const qreal rotatedY = centerY + corner.x() * sinValue + corner.y() * cosValue;

                if (firstCorner) {
                    itemMinX = rotatedX;
                    itemMaxX = rotatedX;
                    itemMinY = rotatedY;
                    itemMaxY = rotatedY;
                    firstCorner = false;
                } else {
                    itemMinX = qMin(itemMinX, rotatedX);
                    itemMaxX = qMax(itemMaxX, rotatedX);
                    itemMinY = qMin(itemMinY, rotatedY);
                    itemMaxY = qMax(itemMaxY, rotatedY);
                }
            }

            if (!hasBounds) {
                minX = itemMinX;
                minY = itemMinY;
                maxX = itemMaxX;
                maxY = itemMaxY;
                hasBounds = true;
            } else {
                minX = qMin(minX, itemMinX);
                minY = qMin(minY, itemMinY);
                maxX = qMax(maxX, itemMaxX);
                maxY = qMax(maxY, itemMaxY);
            }

            previewData.append(item);
        }

        std::sort(previewData.begin(), previewData.end(), [](const PreviewItemData &left, const PreviewItemData &right) {
            if (left.z == right.z) {
                return left.id < right.id;
            }
            return left.z < right.z;
        });

        QVariantList previewItems;
        previewItems.reserve(previewData.size());
        for (const PreviewItemData &item : previewData) {
            QVariantMap map;
            map.insert("id", item.id);
            map.insert("hash", item.hash);
            map.insert("source", item.source);
            map.insert("x", item.x);
            map.insert("y", item.y);
            map.insert("width", item.width);
            map.insert("height", item.height);
            map.insert("rotation", item.rotation);
            map.insert("opacity", item.opacity);
            map.insert("z", item.z);
            previewItems.append(map);
        }

        qreal previewOriginX = 0;
        qreal previewOriginY = 0;
        qreal previewCanvasWidth = 112;
        qreal previewCanvasHeight = 112;
        if (hasBounds) {
            const qreal margin = qMax<qreal>((maxX - minX) * 0.1, 24.0);
            minX -= margin;
            minY -= margin;
            maxX += margin;
            maxY += margin;
            const qreal side = qMax(maxX - minX, maxY - minY);
            const qreal centerX = (minX + maxX) / 2.0;
            const qreal centerY = (minY + maxY) / 2.0;
            previewOriginX = centerX - side / 2.0;
            previewOriginY = centerY - side / 2.0;
            previewCanvasWidth = side;
            previewCanvasHeight = side;
        }

        m_boardsModel->updatePreview(boardId,
                                     previewSource,
                                     previewItems,
                                     previewCanvasWidth,
                                     previewCanvasHeight,
                                     previewOriginX,
                                     previewOriginY,
                                     itemsArray.size());
    });
}

QString AppState::formatUpdatedLabel(const QJsonObject &boardObject) const
{
    const QJsonValue updatedValue = boardObject.value("updated_at");
    if (updatedValue.isDouble()) {
        return formatDateTime(QDateTime::fromSecsSinceEpoch(updatedValue.toInteger()));
    }

    if (updatedValue.isString()) {
        const QDateTime parsed = parseApiDateTime(updatedValue.toString());
        if (parsed.isValid()) {
            return formatDateTime(parsed);
        }
    }

    return QStringLiteral("Синхронизировано с настольной версией");
}

QString AppState::formatBoardSubtitle(const QJsonObject &boardObject) const
{
    const QString owner = valueAsString(boardObject, {"owner_email", "owner", "user_email"});
    const int itemCount = valueAsInt(boardObject, {"items_count", "item_count", "images_count"}, -1);

    if (!owner.isEmpty() && itemCount >= 0) {
        return QString("%1 изображений • %2").arg(itemCount).arg(owner);
    }

    if (itemCount >= 0) {
        return QString("%1 изображений синхронизировано").arg(itemCount);
    }

    if (!owner.isEmpty()) {
        return QString("Владелец: %1").arg(owner);
    }

    return QStringLiteral("Синхронизированная доска референсов");
}

QDateTime AppState::parseApiDateTime(const QString &value) const
{
    if (value.isEmpty()) {
        return {};
    }

    QDateTime parsed = QDateTime::fromString(value, Qt::ISODateWithMs);
    if (!parsed.isValid()) {
        parsed = QDateTime::fromString(value, Qt::ISODate);
    }

    if (!parsed.isValid()) {
        return {};
    }

    static const QRegularExpression timezoneSuffix(R"((Z|[+\-]\d{2}:\d{2})$)");
    if (!timezoneSuffix.match(value).hasMatch()) {
        parsed.setTimeZone(QTimeZone::UTC);
    }

    return parsed;
}
