#ifndef DIRSYNCTREEMODEL_H
#define DIRSYNCTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class DirSyncTreeItem;

class DirSyncTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DirSyncTreeModel(const QString &data, QObject *parent = nullptr);
    ~DirSyncTreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    void setupModelData(const QStringList &lines, DirSyncTreeItem *parent);

    DirSyncTreeItem *root;
};

#endif // DIRSYNCTREEMODEL_H
