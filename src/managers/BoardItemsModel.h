#pragma once

#include <QAbstractListModel>
#include <QVector>

struct BoardItemEntry {
    QString id;
    QString imageHash;
    QString source;
    qreal x = 0;
    qreal y = 0;
    qreal width = 0;
    qreal height = 0;
    qreal rotation = 0;
    qreal opacity = 1.0;
    qreal cropX = 0;
    qreal cropY = 0;
    qreal cropWidth = 0;
    qreal cropHeight = 0;
    int zIndex = 0;
    QString label;
};

class BoardItemsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Role {
        ItemIdRole = Qt::UserRole + 1,
        ImageHashRole,
        SourceRole,
        XRole,
        YRole,
        WidthRole,
        HeightRole,
        RotationRole,
        OpacityRole,
        CropXRole,
        CropYRole,
        CropWidthRole,
        CropHeightRole,
        ZIndexRole,
        LabelRole
    };

    explicit BoardItemsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setItems(QVector<BoardItemEntry> items);
    void clear();
    void updateSourceForHash(const QString &hash, const QString &source);

signals:
    void countChanged();

private:
    QVector<BoardItemEntry> m_items;
};
