#ifndef DIRSYNCTREEMODEL_H
#define DIRSYNCTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QFileInfo>

class DirSyncTreeItem;

class DirSyncTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum SyncFlag { SubDir          = 0x01,
                   ByContent        = 0x02,
                   IgnoreDate       = 0x04,
                   IgnoreExtension  = 0x08,
                   IgnoreSize       = 0x16,
                   NoFilter         = -1
                 };
    Q_DECLARE_FLAGS(SyncFlags, SyncFlag)

    explicit DirSyncTreeModel(QObject *parent = nullptr);
    ~DirSyncTreeModel() override;

    void setLeftAndRightTopDirs(QString &leftTopDir, QString &righTopDir, SyncFlags syncFlags = NoFilter);
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
    void addDirSiblings(DirSyncTreeItem *parent, QFileInfo &topDir, bool isLeft);
    void compareFiles(DirSyncTreeItem *parent, QFileInfo& leftFile, QFileInfo& righFile);
    void compareDirs(DirSyncTreeItem *parent, const QString &leftRootPath, const QString &rightRootPath);
    QString formatSize(qint64 fileSize);

    DirSyncTreeItem *rootItem;
    QString leftTopDir;
    QString rightTopDir;
    SyncFlags syncFlags;
};

#endif // DIRSYNCTREEMODEL_H
