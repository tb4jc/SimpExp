#include <QCompleter>

#include "syncdirdialog.h"
#include "ui_syncdirdialog.h"

SyncDirDialog::SyncDirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncDirDialog)
{
    ui->setupUi(this);
    leftFileSystem.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
    rightFileSystem.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
    leftFileSystem.setRootPath("");
    rightFileSystem.setRootPath("");
    ui->treeViewLeft->setModel(&leftFileSystem);
    ui->treeViewRight->setModel(&rightFileSystem);
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
        ui->treeViewLeft->setColumnWidth(col, colSizes[col]);
        ui->treeViewRight->setColumnWidth(col, colSizes[col]);
    }
}

void SyncDirDialog::setPaneRoots(QString &leftRoot, QString &rightRoot)
{
    QCompleter *leftCompleter = new QCompleter(&leftFileSystem, this);
    QCompleter *rightCompleter = new QCompleter(&rightFileSystem, this);
    leftCompleter->setCompletionMode(QCompleter::PopupCompletion);
    leftCompleter->setCaseSensitivity(Qt::CaseSensitive);
    rightCompleter->setCompletionMode(QCompleter::PopupCompletion);
    rightCompleter->setCaseSensitivity(Qt::CaseSensitive);

    ui->lineEditLeft->setCompleter(leftCompleter);
    ui->lineEditLeft->setText(leftRoot);
    ui->treeViewLeft->setRootIndex(leftFileSystem.index(leftRoot));
    ui->lineEditRight->setCompleter(rightCompleter);
    ui->lineEditRight->setText(rightRoot);
    ui->treeViewRight->setRootIndex(rightFileSystem.index(rightRoot));
}
