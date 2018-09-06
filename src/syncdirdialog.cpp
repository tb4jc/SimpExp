#include <QCompleter>
#include <QFileDialog>

#include "syncdirdialog.h"
#include "ui_syncdirdialog.h"

SyncDirDialog::SyncDirDialog(QWidget* /*parent*/) :
    // not setting parent to get own window
    ui(new Ui::SyncDirDialog)
{
    ui->setupUi(this);
    fsmLeft.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
    fsmRight.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
    ui->treeView->sortByColumn(0, Qt::AscendingOrder);
    ui->treeView->setModel(&dirSyncTreeModel);
    showMaximized();
}

SyncDirDialog::~SyncDirDialog()
{
    delete ui;
}

void SyncDirDialog::setPaneColWidths(QList<int> &colSizes)
{
    for(int col = 0; col < colSizes.size(); col++)
    {
        ui->treeView->setColumnWidth(col, colSizes[col]);
    }
}

void SyncDirDialog::setPaneRoots(QString &leftRoot, QString &rightRoot)
{
    QCompleter *leftCompleter = new QCompleter(&fsmLeft, this);
    QCompleter *rightCompleter = new QCompleter(&fsmRight, this);
    leftCompleter->setCompletionMode(QCompleter::PopupCompletion);
    leftCompleter->setCaseSensitivity(Qt::CaseSensitive);
    rightCompleter->setCompletionMode(QCompleter::PopupCompletion);
    rightCompleter->setCaseSensitivity(Qt::CaseSensitive);

    fsmLeft.setRootPath(leftRoot);
    ui->lineEditLeftPath->setCompleter(leftCompleter);
    ui->lineEditLeftPath->setText(leftRoot);
    fsmRight.setRootPath(leftRoot);
    ui->lineEditRightPath->setCompleter(rightCompleter);
    ui->lineEditRightPath->setText(rightRoot);
}

void SyncDirDialog::on_pushButtonClose_released()
{
    this->close();
}

void SyncDirDialog::on_pushButtonLeftPathSelector_released()
{
    QFileDialog fileDialog(this, "Select Left Direcotry", ui->lineEditLeftPath->text());
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    if (fileDialog.exec())
    {
        ui->lineEditLeftPath->setText(fileDialog.selectedFiles()[0]);
    }
}

void SyncDirDialog::on_pushButtonRightPathSelector_released()
{
    QFileDialog fileDialog(this, "Select Right Direcotry", ui->lineEditRightPath->text());
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    if (fileDialog.exec())
    {
        ui->lineEditRightPath->setText(fileDialog.selectedFiles()[0]);
    }
}
