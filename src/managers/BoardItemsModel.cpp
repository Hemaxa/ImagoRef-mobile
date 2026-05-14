#include "BoardItemsModel.h"

#include <algorithm>

BoardItemsModel::BoardItemsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int BoardItemsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_items.size();
}

QVariant BoardItemsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return {};
    }

    const BoardItemEntry &item = m_items.at(index.row());

    switch (role) {
    case ItemIdRole:
        return item.id;
    case ImageHashRole:
        return item.imageHash;
    case SourceRole:
        return item.source;
    case XRole:
        return item.x;
    case YRole:
        return item.y;
    case WidthRole:
        return item.width;
    case HeightRole:
        return item.height;
    case RotationRole:
        return item.rotation;
    case OpacityRole:
        return item.opacity;
    case CropXRole:
        return item.cropX;
    case CropYRole:
        return item.cropY;
    case CropWidthRole:
        return item.cropWidth;
    case CropHeightRole:
        return item.cropHeight;
    case ZIndexRole:
        return item.zIndex;
    case LabelRole:
        return item.label;
    default:
        return {};
    }
}

QHash<int, QByteArray> BoardItemsModel::roleNames() const
{
    return {
        {ItemIdRole, "itemId"},
        {ImageHashRole, "imageHash"},
        {SourceRole, "itemSource"},
        {XRole, "itemX"},
        {YRole, "itemY"},
        {WidthRole, "itemWidth"},
        {HeightRole, "itemHeight"},
        {RotationRole, "itemRotation"},
        {OpacityRole, "itemOpacity"},
        {CropXRole, "cropX"},
        {CropYRole, "cropY"},
        {CropWidthRole, "cropWidth"},
        {CropHeightRole, "cropHeight"},
        {ZIndexRole, "itemZ"},
        {LabelRole, "itemLabel"}
    };
}

void BoardItemsModel::setItems(QVector<BoardItemEntry> items)
{
    std::sort(items.begin(), items.end(), [](const BoardItemEntry &left, const BoardItemEntry &right) {
        if (left.zIndex == right.zIndex) {
            return left.id < right.id;
        }
        return left.zIndex < right.zIndex;
    });

    beginResetModel();
    m_items = std::move(items);
    endResetModel();
    emit countChanged();
}

void BoardItemsModel::clear()
{
    setItems({});
}

void BoardItemsModel::updateSourceForHash(const QString &hash, const QString &source)
{
    if (hash.isEmpty() || source.isEmpty()) {
        return;
    }

    for (int row = 0; row < m_items.size(); ++row) {
        if (m_items[row].imageHash == hash && m_items[row].source != source) {
            m_items[row].source = source;
            const QModelIndex modelIndex = index(row);
            emit dataChanged(modelIndex, modelIndex, {SourceRole});
        }
    }
}
