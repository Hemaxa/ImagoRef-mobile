#include "BoardsListModel.h"

BoardsListModel::BoardsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int BoardsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_boards.size();
}

QVariant BoardsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_boards.size()) {
        return {};
    }

    const BoardEntry &board = m_boards.at(index.row());

    switch (role) {
    case BoardIdRole:
        return board.id;
    case TitleRole:
        return board.title;
    case SubtitleRole:
        return board.subtitle;
    case UpdatedLabelRole:
        return board.updatedLabel;
    case PreviewSourceRole:
        return board.previewSource;
    case PreviewItemsRole:
        return board.previewItems;
    case PreviewCanvasWidthRole:
        return board.previewCanvasWidth;
    case PreviewCanvasHeightRole:
        return board.previewCanvasHeight;
    case PreviewOriginXRole:
        return board.previewOriginX;
    case PreviewOriginYRole:
        return board.previewOriginY;
    case ItemCountRole:
        return board.itemCount;
    default:
        return {};
    }
}

QHash<int, QByteArray> BoardsListModel::roleNames() const
{
    return {
        {BoardIdRole, "boardId"},
        {TitleRole, "title"},
        {SubtitleRole, "subtitle"},
        {UpdatedLabelRole, "updatedLabel"},
        {PreviewSourceRole, "previewSource"},
        {PreviewItemsRole, "previewItems"},
        {PreviewCanvasWidthRole, "previewCanvasWidth"},
        {PreviewCanvasHeightRole, "previewCanvasHeight"},
        {PreviewOriginXRole, "previewOriginX"},
        {PreviewOriginYRole, "previewOriginY"},
        {ItemCountRole, "itemCount"}
    };
}

void BoardsListModel::setBoards(const QVector<BoardEntry> &boards)
{
    beginResetModel();
    m_boards = boards;
    endResetModel();
    emit countChanged();
}

void BoardsListModel::clear()
{
    setBoards({});
}

BoardEntry BoardsListModel::boardAt(int row) const
{
    if (row < 0 || row >= m_boards.size()) {
        return {};
    }

    return m_boards.at(row);
}

int BoardsListModel::indexOfId(const QString &boardId) const
{
    for (int i = 0; i < m_boards.size(); ++i) {
        if (m_boards.at(i).id == boardId) {
            return i;
        }
    }

    return -1;
}

void BoardsListModel::updatePreview(const QString &boardId,
                                    const QString &previewSource,
                                    const QVariantList &previewItems,
                                    qreal previewCanvasWidth,
                                    qreal previewCanvasHeight,
                                    qreal previewOriginX,
                                    qreal previewOriginY,
                                    int itemCount)
{
    const int row = indexOfId(boardId);
    if (row < 0) {
        return;
    }

    QVector<int> changedRoles;

    if (!previewSource.isEmpty() && m_boards[row].previewSource != previewSource) {
        m_boards[row].previewSource = previewSource;
        changedRoles.append(PreviewSourceRole);
    }

    if (!previewItems.isEmpty() && m_boards[row].previewItems != previewItems) {
        m_boards[row].previewItems = previewItems;
        changedRoles.append(PreviewItemsRole);
    }

    if (previewCanvasWidth > 0 && !qFuzzyCompare(m_boards[row].previewCanvasWidth, previewCanvasWidth)) {
        m_boards[row].previewCanvasWidth = previewCanvasWidth;
        changedRoles.append(PreviewCanvasWidthRole);
    }

    if (previewCanvasHeight > 0 && !qFuzzyCompare(m_boards[row].previewCanvasHeight, previewCanvasHeight)) {
        m_boards[row].previewCanvasHeight = previewCanvasHeight;
        changedRoles.append(PreviewCanvasHeightRole);
    }

    if (!qFuzzyCompare(m_boards[row].previewOriginX + 1.0, previewOriginX + 1.0)) {
        m_boards[row].previewOriginX = previewOriginX;
        changedRoles.append(PreviewOriginXRole);
    }

    if (!qFuzzyCompare(m_boards[row].previewOriginY + 1.0, previewOriginY + 1.0)) {
        m_boards[row].previewOriginY = previewOriginY;
        changedRoles.append(PreviewOriginYRole);
    }

    if (itemCount >= 0 && m_boards[row].itemCount != itemCount) {
        m_boards[row].itemCount = itemCount;
        changedRoles.append(ItemCountRole);
    }

    if (!changedRoles.isEmpty()) {
        const QModelIndex modelIndex = index(row);
        emit dataChanged(modelIndex, modelIndex, changedRoles);
    }
}
