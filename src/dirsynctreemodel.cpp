#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDateTime>

#include "dirsynctreeitem.h"
#include "dirsynctreemodel.h"


DirSyncTreeModel::DirSyncTreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      syncFlags(NoFilter)
{
    QList<QVariant> rootData;
    rootData << "Name" << "Ext" << "Size" << "Date" << "<=>" << "Name" << "Ext" << "Size" << "Date";
    rootItem = new DirSyncTreeItem(rootData);
}

DirSyncTreeModel::~DirSyncTreeModel()
{
    delete rootItem;
}

void DirSyncTreeModel::addDirSiblings(DirSyncTreeItem *parent, QFileInfo &topDir, bool isLeft)
{
    // TODO: loop over siblings
    QList<QVariant> columnData;
    if (isLeft)
    {
        columnData << topDir.absoluteFilePath() << "" << "" << "" << "->" << "" << "" << "" << "";
    }
    else
    {
        columnData << "" << "" << "" << "" << "" << topDir.absoluteFilePath() << "" << "" << "";
    }

    parent->appendChild(new DirSyncTreeItem(columnData, parent));
}

void DirSyncTreeModel::compareFiles(DirSyncTreeItem *parent, QFileInfo &leftFile, QFileInfo &rightFile)
{
    QString leftCompareName;
    QString rightCompareName;
    QString leftFilename = leftFile.fileName();
    QString rightFilename = rightFile.fileName();
    QString leftFileBaseName = leftFile.baseName();
    QString rightFileBaseName = rightFile.baseName();
    QString leftFileCompleteBaseName = leftFile.completeBaseName();
    QString rightFileCompleteBaseName = rightFile.completeBaseName();
    QString leftFileExt = leftFile.suffix();
    QString rightFileExt = rightFile.suffix();
    qint64 leftFileSize = leftFile.size();
    qint64 rightFileSize = rightFile.size();

    // both iterators reference files
    if (!syncFlags.testFlag(IgnoreExtension))
    {
        leftCompareName = leftFilename;
        rightCompareName = rightFilename;
    }
    else
    {

        leftCompareName = leftFileCompleteBaseName;
        rightCompareName= rightFileCompleteBaseName;
    }

    QList<QVariant> columnData;
    if (leftCompareName < rightCompareName)
    {
        columnData << leftFileCompleteBaseName << leftFileExt << leftFileSize << leftFile.fileTime(QFile::FileModificationTime);
        columnData << "=>" << ""<< "" << "" << "";

    }
    else if (leftCompareName > rightCompareName)
    {
        columnData << "" << "" << "" << "";
        columnData << "<=" << rightFileCompleteBaseName << rightFileExt << rightFileSize << rightFile.fileTime(QFile::FileModificationTime);
    }
    else
    {
        // file names (with or without extension) are equal
        int compVal = 0;
        bool fileEqual = false;
        if (syncFlags.testFlag(ByContent))
        {
            fileEqual = false;
        }

        if (!syncFlags.testFlag(IgnoreSize))
        {
            qint64 leftFileSize = leftFile.size();
            qint64 rightFileSize = rightFile.size();
            fileEqual = (leftFileSize != rightFileSize);
        }

        if (!syncFlags.testFlag(IgnoreDate))
        {
            QDateTime leftFileATime = leftFile.fileTime(QFile::FileAccessTime);
            QDateTime rightFileATime = rightFile.fileTime(QFile::FileAccessTime);
            if (leftFileATime < rightFileATime) compVal = -1;
            else if (leftFileATime > rightFileATime) compVal = 1;
        }
        QList<QString> cmpSign;
        cmpSign << "<=" << "=" << "=>";
        columnData << leftFileCompleteBaseName << leftFileExt << leftFileSize << leftFile.fileTime(QFile::FileModificationTime);
        columnData << cmpSign.at(compVal+1) << rightFileCompleteBaseName << rightFileExt << rightFileSize << rightFile.fileTime(QFile::FileModificationTime);
    }
    parent->appendChild(new DirSyncTreeItem(columnData, parent));
}

void DirSyncTreeModel::compareDirs(DirSyncTreeItem *parent, const QString &leftRootPath, const QString &rightRootPath)
{
    QDir leftRootDir(leftRootPath);
    QDir rightRootDir(rightRootPath);

    QFileInfoList leftFileList(leftRootDir.entryInfoList(QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Dirs|QDir::Files, QDir::DirsFirst|QDir::Name));
    QFileInfoList rightFileList(rightRootDir.entryInfoList(QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Dirs|QDir::Files, QDir::DirsFirst|QDir::Name));

    QFileInfoList::iterator iteratorLeftList = leftFileList.begin();
    QFileInfoList::iterator iteratorRightList = rightFileList.begin();

    while ((iteratorLeftList != leftFileList.end()) || (iteratorRightList != rightFileList.end()))
    {
        if ((iteratorLeftList != leftFileList.end()) && (iteratorRightList != rightFileList.end()))
        {
            if ((*iteratorLeftList).isDir() && (*iteratorRightList).isDir())
            {
                QString leftDir = (*iteratorLeftList).fileName();
                QString rightDir = (*iteratorRightList).fileName();
                if (leftDir < rightDir)
                {
                    // left dir is before right -> work all files within as copy to right
                    addDirSiblings(parent, *iteratorLeftList, true);
                    iteratorLeftList++;
                }
                else if (leftDir > rightDir)
                {
                    // left dir is before right -> work all files within as copy to right
                    addDirSiblings(parent, *iteratorLeftList, true);
                    iteratorRightList++;
                }
                else
                {
                    // same directory - compare siblings if enabled
                    if (syncFlags.testFlag(SubDir))
                    {
                        compareDirs(parent, (*iteratorLeftList).absoluteFilePath(), (*iteratorRightList).absoluteFilePath());
                        iteratorLeftList++;
                        iteratorRightList++;
                    }
                }
            }
            else if ((*iteratorLeftList).isDir())
            {
                addDirSiblings(parent, *iteratorLeftList, true);
                iteratorLeftList++;
            }
            else if ((*iteratorRightList).isDir())
            {
                addDirSiblings(parent, *iteratorRightList, false);
                iteratorRightList++;
            }
            else
            {
                compareFiles(parent, *iteratorLeftList, *iteratorRightList);
                iteratorLeftList++;
                iteratorRightList++;
            }
        }
        else if (iteratorLeftList != leftFileList.end())
        {
            if ((*iteratorLeftList).isDir())
            {
                 addDirSiblings(parent, *iteratorLeftList, true);
            }
            else
            {
                 QList<QVariant> columnData;
                 columnData << (*iteratorLeftList).completeBaseName() << (*iteratorLeftList).suffix() << (*iteratorLeftList).size() << (*iteratorLeftList).fileTime(QFile::FileModificationTime);
                 columnData << "=>" << ""<< "" << "" << "";
                 parent->appendChild(new DirSyncTreeItem(columnData, parent));
            }
            iteratorLeftList++;
        }
        else if  (iteratorRightList != rightFileList.end())
        {
            if ((*iteratorRightList).isDir())
            {
                addDirSiblings(parent, *iteratorLeftList, true);
            }
            else
            {
                QList<QVariant> columnData;
                columnData << ""<< "" << "" << "" << "<=";
                columnData << (*iteratorRightList).completeBaseName() << (*iteratorRightList).suffix() << (*iteratorRightList).size() << (*iteratorRightList).fileTime(QFile::FileModificationTime);
                parent->appendChild(new DirSyncTreeItem(columnData, parent));
            }
            iteratorRightList++;
        }
    }
}

void DirSyncTreeModel::setLeftAndRightTopDirs(QString &leftTopDir, QString &rightTopDir, SyncFlags syncFlags)
{
    this->leftTopDir = leftTopDir;
    this->rightTopDir = rightTopDir;
    this->syncFlags = syncFlags;

    beginInsertRows(QModelIndex(), 0, 9999999);
    compareDirs(rootItem, leftTopDir, rightTopDir);
    endInsertRows();
}

int DirSyncTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<DirSyncTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant DirSyncTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DirSyncTreeItem *item = static_cast<DirSyncTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags DirSyncTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant DirSyncTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex DirSyncTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DirSyncTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DirSyncTreeItem*>(parent.internalPointer());

    DirSyncTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DirSyncTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DirSyncTreeItem *childItem = static_cast<DirSyncTreeItem*>(index.internalPointer());
    DirSyncTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DirSyncTreeModel::rowCount(const QModelIndex &parent) const
{
    DirSyncTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DirSyncTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
