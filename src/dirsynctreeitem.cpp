#include "dirsynctreeitem.h"

DirSyncTreeItem::DirSyncTreeItem(const QList<QVariant> &data, DirSyncTreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

DirSyncTreeItem::~DirSyncTreeItem()
{
    qDeleteAll(m_childItems);
}

void DirSyncTreeItem::appendChild(DirSyncTreeItem *item)
{
    m_childItems.append(item);
}

DirSyncTreeItem *DirSyncTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int DirSyncTreeItem::childCount() const
{
    return m_childItems.count();
}

int DirSyncTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant DirSyncTreeItem::data(int column) const
{
    return m_itemData.value(column);
}

DirSyncTreeItem *DirSyncTreeItem::parentItem()
{
    return m_parentItem;
}

int DirSyncTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<DirSyncTreeItem*>(this));

    return 0;
}

void DirSyncTreeItem::resetItem()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}
