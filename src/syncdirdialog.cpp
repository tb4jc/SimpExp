#include <QCompleter>
#include <QFileDialog>

#include "syncdirdialog.h"
#include "ui_syncdirdialog.h"

SyncDirDialog::SyncDirDialog(QSettings *settings, QString &leftRoot, QString &rightRoot, QWidget *parent) :
    // not setting parent to get own window
    ui(new Ui::SyncDirDialog),
    appSettings(settings)
{
    Q_UNUSED(parent);
    ui->setupUi(this);

    LoadSettings();

    fsmLeft.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
    fsmRight.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
    ui->treeView->sortByColumn(0, Qt::AscendingOrder);
    ui->treeView->setModel(&dirSyncTreeModel);
    showMaximized();
    setAttribute(Qt::WA_DeleteOnClose);

    setPaneRoots(leftRoot, rightRoot);
}

SyncDirDialog::~SyncDirDialog()
{
    SaveSettings();
    delete ui;
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

void SyncDirDialog::LoadSettings()
{
    appSettings->beginGroup("SyncDirDialog");
    QRect defaultRect(200,200,1000, 800);
    QVariant defNameLeftWidth(400);
    QVariant defExtLeftWidth(50);
    QVariant defSizeLeftWidth(100);
    QVariant defDateLeftWidth(120);
    QVariant defNameRightWidth(400);
    QVariant defExtRightWidth(50);
    QVariant defSizeRightWidth(100);
    QVariant defDateRightWidth(120);

    if (!restoreGeometry(appSettings->value("geometry").toByteArray()))
    {
        setGeometry(defaultRect);
    }
    ui->treeView->setColumnWidth(0, appSettings->value("left/name-col-width", defNameLeftWidth).toInt());
    ui->treeView->setColumnWidth(1, appSettings->value("left/ext-col-width", defExtLeftWidth).toInt());
    ui->treeView->setColumnWidth(2, appSettings->value("left/size-col-width", defSizeLeftWidth).toInt());
    ui->treeView->setColumnWidth(3, appSettings->value("left/date-col-width", defDateLeftWidth).toInt());
    ui->treeView->setColumnWidth(5, appSettings->value("right/name-col-width", defNameRightWidth).toInt());
    ui->treeView->setColumnWidth(6, appSettings->value("right/ext-col-width", defExtRightWidth).toInt());
    ui->treeView->setColumnWidth(7, appSettings->value("right/size-col-width", defSizeRightWidth).toInt());
    ui->treeView->setColumnWidth(8, appSettings->value("right/date-col-width", defDateRightWidth).toInt());
    appSettings->endGroup();
}


void SyncDirDialog::SaveSettings()
{
    appSettings->beginGroup("SyncDirDialog");
    appSettings->setValue("geometry", saveGeometry());
    appSettings->setValue("left/name-col-width", ui->treeView->columnWidth(0));
    appSettings->setValue("left/ext-col-width", ui->treeView->columnWidth(1));
    appSettings->setValue("left/size-col-width", ui->treeView->columnWidth(2));
    appSettings->setValue("left/date-col-width", ui->treeView->columnWidth(3));
    appSettings->setValue("right/name-col-width", ui->treeView->columnWidth(0));
    appSettings->setValue("right/ext-col-width", ui->treeView->columnWidth(1));
    appSettings->setValue("right/size-col-width", ui->treeView->columnWidth(2));
    appSettings->setValue("right/date-col-width", ui->treeView->columnWidth(3));
    appSettings->endGroup();
}

void SyncDirDialog::on_pushButtonCompare_released()
{
    // get left and right top directories
    QString leftTopDir = ui->lineEditLeftPath->text();
    QString rightTopDir = ui->lineEditRightPath->text();
    // set them to the dir sync tree model
    DirSyncTreeModel::SyncFlags syncFlags;
    syncFlags.setFlag(DirSyncTreeModel::ByContent, ui->checkBoxByContent->isChecked());
    syncFlags.setFlag(DirSyncTreeModel::IgnoreDate, ui->checkBoxIgnoreDate->isChecked());
    syncFlags.setFlag(DirSyncTreeModel::IgnoreSize, ui->checkBoxIgnoreSize->isChecked());
    syncFlags.setFlag(DirSyncTreeModel::IgnoreExtension, ui->checkBoxIgnoreExt->isChecked());
    dirSyncTreeModel.setLeftAndRightTopDirs(leftTopDir, rightTopDir, syncFlags);

    // model should update the tree view
//    ui->treeView->update();
}
