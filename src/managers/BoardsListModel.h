#pragma once

#include <QAbstractListModel>
#include <QVariantList>
#include <QVector>

struct BoardEntry {
    QString id;
    QString title;
    QString subtitle;
    QString updatedLabel;
    QString previewSource;
    QVariantList previewItems;
    qreal previewCanvasWidth = 0;
    qreal previewCanvasHeight = 0;
    qreal previewOriginX = 0;
    qreal previewOriginY = 0;
    int itemCount = 0;
};

class BoardsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Role {
        BoardIdRole = Qt::UserRole + 1,
        TitleRole,
        SubtitleRole,
        UpdatedLabelRole,
        PreviewSourceRole,
        PreviewItemsRole,
        PreviewCanvasWidthRole,
        PreviewCanvasHeightRole,
        PreviewOriginXRole,
        PreviewOriginYRole,
        ItemCountRole
    };

    explicit BoardsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setBoards(const QVector<BoardEntry> &boards);
    void clear();
    BoardEntry boardAt(int row) const;
    int indexOfId(const QString &boardId) const;
    void updatePreview(const QString &boardId,
                       const QString &previewSource,
                       const QVariantList &previewItems = {},
                       qreal previewCanvasWidth = 0,
                       qreal previewCanvasHeight = 0,
                       qreal previewOriginX = 0,
                       qreal previewOriginY = 0,
                       int itemCount = -1);
    void updatePreviewItemSourceForHash(const QString &hash, const QString &source);

signals:
    void countChanged();

private:
    QVector<BoardEntry> m_boards;
};
