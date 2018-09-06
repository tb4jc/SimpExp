#ifndef SYNCDIRDIALOG_H
#define SYNCDIRDIALOG_H

#include <QWidget>
#include <QFileInfo>
#include "filesystemmodel.h"
#include "dirsynctreemodel.h"

namespace Ui {
class SyncDirDialog;
}

class SyncDirDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SyncDirDialog(QWidget *parent = nullptr);
    ~SyncDirDialog();
    void setPaneRoots(QString &leftRoot, QString &rightRoot);
    void setPaneColWidths(QList<int> &colSizes);

private slots:
    void on_pushButtonClose_released();

    void on_pushButtonLeftPathSelector_released();

    void on_pushButtonRightPathSelector_released();

private:
    Ui::SyncDirDialog *ui;
    FileSystemModel fsmLeft;
    FileSystemModel fsmRight;
    DirSyncTreeModel dirSyncTreeModel;
};

#endif // SYNCDIRDIALOG_H
