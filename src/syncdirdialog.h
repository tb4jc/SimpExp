#ifndef SYNCDIRDIALOG_H
#define SYNCDIRDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include "filesystemmodel.h"

namespace Ui {
class SyncDirDialog;
}

class SyncDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SyncDirDialog(QWidget *parent = nullptr);
    ~SyncDirDialog();
    void setPaneRoots(QString &leftRoot, QString &rightRoot);
    void setPaneColWidths(QList<int> &colSizes);

private:
    Ui::SyncDirDialog *ui;
    FileSystemModel leftFileSystem;
    FileSystemModel rightFileSystem;
};

#endif // SYNCDIRDIALOG_H
