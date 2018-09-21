#ifndef DIRSYNCTREEITEM_H
#define DIRSYNCTREEITEM_H


#include <QList>
#include <QVariant>


class DirSyncTreeItem
{
public:
    explicit DirSyncTreeItem(const QList<QVariant> &data, DirSyncTreeItem *parentItem = nullptr);
    ~DirSyncTreeItem();

    void appendChild(DirSyncTreeItem *child);

    DirSyncTreeItem *parentItem();
    DirSyncTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    void resetItem();

private:
    DirSyncTreeItem *m_parentItem;
    QList<DirSyncTreeItem*> m_childItems;
    QList<QVariant> m_itemData;
};

#endif // DIRSYNCTREEITEM_H
