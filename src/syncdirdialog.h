#ifndef SYNCDIRDIALOG_H
#define SYNCDIRDIALOG_H

#include <QWidget>
#include <QSettings>
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
    explicit SyncDirDialog(QSettings *settings, QString &leftRoot, QString &rightRoot, QWidget *parent = nullptr);
    ~SyncDirDialog();
    void setPaneRoots(QString &leftRoot, QString &rightRoot);

private slots:
    void on_pushButtonClose_released();

    void on_pushButtonLeftPathSelector_released();

    void on_pushButtonRightPathSelector_released();

    void on_pushButtonCompare_released();

private:
    void LoadSettings();
    void SaveSettings();

    Ui::SyncDirDialog *ui;
    QSettings *appSettings;
    FileSystemModel fsmLeft;
    FileSystemModel fsmRight;
    DirSyncTreeModel dirSyncTreeModel;
};

#endif // SYNCDIRDIALOG_H
