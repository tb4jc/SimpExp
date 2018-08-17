#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QModelIndex>
#include <QDir>
#include <QFileInfoList>
#include <QStorageInfo>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    appSettings(QApplication::applicationDirPath() + "/SimpExp.ini", QSettings::IniFormat),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up treeViewLeft/Right first before setting combox
//    QString root = "C:";
    fileModelLeft = new QFileSystemModel(this);
    fileModelLeft->setFilter(QDir::NoDot|QDir::AllEntries);
    ui->treeViewLeft->setModel(fileModelLeft);

    fileModelRight = new QFileSystemModel(this);
    fileModelRight->setFilter(QDir::NoDot|QDir::AllEntries);
    ui->treeViewRight->setModel(fileModelRight);

    connect(fileModelLeft, SIGNAL(directoryLoaded(QString)), this, SLOT(leftPaneModelDirectoryLoaded(QString))/*, Qt::DirectConnection*/);
    connect(fileModelRight, SIGNAL(directoryLoaded(QString)), this, SLOT(rightPaneModelDirectoryLoaded(QString))/*, Qt::DirectConnection*/);
    fileModelLeft->setRootPath("");
    fileModelRight->setRootPath("");

    QFileSystemModel* cbDriveLeftModel = new QFileSystemModel(this);
    cbDriveLeftModel->setFilter(QDir::Drives);
    QFileInfoList driveFileInfos = QDir::drives();
    foreach(const QFileInfo& drive, driveFileInfos)
    {
        QString absDrivePath = drive.absolutePath();
//        qDebug() << "Drive: " << absDrivePath;
        QModelIndex driveIdx = cbDriveLeftModel->index(drive.absolutePath());
        QString driveName = driveIdx.data(Qt::DisplayRole).toString();
        if (!driveName.isEmpty())
        {
            drives.append(absDrivePath);
            QIcon driveIcon = cbDriveLeftModel->fileIcon(driveIdx);
            QString cbItemText;
            if (driveName.contains('('))
            {
                QString driveLetter = driveName.section('(',1,1).section(')', 0, 0);
                cbItemText = driveLetter;
            }
            else
            {
                cbItemText = driveName;
            }
            ui->cbDriveLeft->addItem(driveIcon, cbItemText);
            ui->cbDriveRight->addItem(driveIcon, cbItemText);
        }
    }

    connect(ui->cbDriveLeft, SIGNAL(currentIndexChanged(int)), this, SLOT(cbDriveLeftCurrentIndexChanged(int)) /*, Qt::DirectConnection*/);
    connect(ui->cbDriveRight, SIGNAL(currentIndexChanged(int)), this, SLOT(cbDriveRightCurrentIndexChanged(int)) /*, Qt::DirectConnection*/);

    LoadSettings();

    // root directories not loaded yet - manul selected drive C:
//    cbDriveLeftCurrentIndexChanged(0);
//    cbDriveRightCurrentIndexChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileModelLeft;
    delete fileModelRight;
}

void MainWindow::setPaneRoot(const QString &root, QComboBox *driveList, QTreeView *treeView, QLineEdit *leDriveInfo)
{
    QFileSystemModel* model = dynamic_cast<QFileSystemModel*>(treeView->model());
    int currentIndex = driveList->currentIndex();
    QString drive = root.split('/').at(0);
    int foundIndex = driveList->findText(drive);
    if ( foundIndex != -1 ) { // -1 for not found
        if (currentIndex != foundIndex)
        {
            driveList->setCurrentIndex(foundIndex);
            treeView->setRootIndex(model->index(root));
        }
        else
        {
            QString driveName = ui->cbDriveLeft->currentText();
            QString size, free, partitionName, type;
            getDriveInfo(driveName, partitionName, type, size, free);
            QString driveInfo = QString("[%1] %2 GB of %3 GB free").arg(partitionName).arg(free).arg(size);
            leDriveInfo->setText(driveInfo);
            treeView->setRootIndex(model->index(root));
        }
    }
}

void MainWindow::LoadSettings()
{
    appSettings.beginGroup("MainWindow");
    QRect defaultRect(200,200,1000, 800);
    QString defLeftPaneRoot("C:");
    QString defRightPaneRoot("C:");
    QVariant defNameWidth(400);
    QVariant defSizeWidth(100);
    QVariant defTypeWidth(130);
    QVariant defDateWidth(120);

    if (!restoreGeometry(appSettings.value("geometry").toByteArray()))
    {
        setGeometry(defaultRect);
    }
    ui->treeViewLeft->setColumnWidth(0, appSettings.value("left/name-col-width", defNameWidth).toInt());
    ui->treeViewLeft->setColumnWidth(1, appSettings.value("left/size-col-width", defSizeWidth).toInt());
    ui->treeViewLeft->setColumnWidth(2, appSettings.value("left/type-col-width", defTypeWidth).toInt());
    ui->treeViewLeft->setColumnWidth(3, appSettings.value("left/date-col-width", defDateWidth).toInt());
    ui->treeViewRight->setColumnWidth(0, appSettings.value("right/name-col-width", defNameWidth).toInt());
    ui->treeViewRight->setColumnWidth(1, appSettings.value("right/size-col-width", defSizeWidth).toInt());
    ui->treeViewRight->setColumnWidth(2, appSettings.value("right/type-col-width", defTypeWidth).toInt());
    ui->treeViewRight->setColumnWidth(3, appSettings.value("right/date-col-width", defDateWidth).toInt());
    appSettings.endGroup();

    appSettings.beginGroup("LeftPane");
    QString leftPaneRoot = appSettings.value("rootDir", defLeftPaneRoot).toString();
    if (leftPaneRoot.isEmpty()) leftPaneRoot = defLeftPaneRoot;
    setPaneRoot(leftPaneRoot, ui->cbDriveLeft, ui->treeViewLeft, ui->leDriveInfoLeft);
    appSettings.endGroup();

    appSettings.beginGroup("RightPane");
    QString rightPaneRoot = appSettings.value("rootDir", defRightPaneRoot).toString();
    if (rightPaneRoot.isEmpty()) rightPaneRoot = defRightPaneRoot;
    setPaneRoot(rightPaneRoot, ui->cbDriveRight, ui->treeViewRight, ui->leDriveInfoRight);
    appSettings.endGroup();
}

void MainWindow::SaveSettings()
{
    appSettings.beginGroup("MainWindow");
    appSettings.setValue("geometry", saveGeometry());
    appSettings.setValue("left/name-col-width", ui->treeViewLeft->columnWidth(0));
    appSettings.setValue("left/size-col-width", ui->treeViewLeft->columnWidth(1));
    appSettings.setValue("left/type-col-width", ui->treeViewLeft->columnWidth(2));
    appSettings.setValue("left/date-col-width", ui->treeViewLeft->columnWidth(3));
    appSettings.setValue("right/name-col-width", ui->treeViewRight->columnWidth(0));
    appSettings.setValue("right/size-col-width", ui->treeViewRight->columnWidth(1));
    appSettings.setValue("right/type-col-width", ui->treeViewRight->columnWidth(2));
    appSettings.setValue("right/date-col-width", ui->treeViewRight->columnWidth(3));
    appSettings.endGroup();

    appSettings.beginGroup("LeftPane");
    QString leftRootDir = ui->treeViewLeft->rootIndex().data(QFileSystemModel::FilePathRole).toString();
    appSettings.setValue("rootDir", leftRootDir);
    appSettings.endGroup();

    appSettings.beginGroup("RightPane");
    QString rightRootDir = ui->treeViewRight->rootIndex().data(QFileSystemModel::FilePathRole).toString();
    appSettings.setValue("rootDir", rightRootDir);
    appSettings.endGroup();
}

void MainWindow::aboutToQuit()
{
    SaveSettings();
}


// private slots

// Left Pane Functions
void MainWindow::cbDriveLeftCurrentIndexChanged(int /*index*/)
{
    QString driveName = ui->cbDriveLeft->currentText();
    QString size;
    QString free;
    QString partitionName;
    QString type;
    getDriveInfo(driveName, partitionName, type, size, free);
    QFileSystemModel* model = dynamic_cast<QFileSystemModel*>(ui->treeViewLeft->model());
    if (model)
    {
        QString driveInfo = QString("[%1] %2 GB of %3 GB free").arg(partitionName).arg(free).arg(size);
        ui->leDriveInfoLeft->setText(driveInfo);
        ui->treeViewLeft->setRootIndex(model->index(driveName));
    }
}

void MainWindow::leftPaneModelDirectoryLoaded(const QString &path)
{
    ui->lePathLeft->setText(QDir::toNativeSeparators(QFileInfo(path).absoluteFilePath()));
}

void MainWindow::on_pbRootLeft_clicked()
{
    QString root = ui->cbDriveLeft->currentText();
    ui->treeViewLeft->setRootIndex(fileModelLeft->setRootPath(root));
}

void MainWindow::on_pbUpLeft_clicked()
{
    QModelIndex rootIdx = ui->treeViewLeft->rootIndex();
    QString rootIndexPath(rootIdx.data(QFileSystemModel::FilePathRole).toString());
    if (!drives.contains(rootIndexPath))
    {
        ui->treeViewLeft->setRootIndex(rootIdx.parent());
    }
}

void MainWindow::on_treeViewLeft_entered(const QModelIndex &index)
{
    qDebug() << "Entered left pane's item " << fileModelLeft->fileName(index);
}

void MainWindow::on_treeViewLeft_activated(const QModelIndex &index)
{
    QString sPath = fileModelLeft->fileInfo(index).absoluteFilePath();
//    ui->treeViewLeft->setRootIndex(fileModelLeft->setRootPath(sPath));
    ui->treeViewLeft->setRootIndex(fileModelLeft->index(sPath));
}


// Right Pane Functions
void MainWindow::cbDriveRightCurrentIndexChanged(int /*index*/)
{
    QString driveName = ui->cbDriveRight->currentText();
    QString size;
    QString free;
    QString partitionName;
    QString type;
    getDriveInfo(driveName, partitionName, type, size, free);
    QFileSystemModel* model = dynamic_cast<QFileSystemModel*>(ui->treeViewRight->model());
    if (model)
    {
        QString driveInfo= QString("[%1] %2 GB of %3 GB free").arg(partitionName).arg(free).arg(size);
        ui->leDriveInfoRight->setText(driveInfo);
        ui->treeViewRight->setRootIndex(model->index(driveName));
    }
}

void MainWindow::rightPaneModelDirectoryLoaded(const QString &path)
{
    ui->lePathRight->setText(QDir::toNativeSeparators(QFileInfo(path).absoluteFilePath()));
}

void MainWindow::on_pbRootRight_clicked()
{
    QString root = ui->cbDriveLeft->currentText();
//    ui->treeViewRight->setRootIndex(fileModelRight->setRootPath(root));
    ui->treeViewRight->setRootIndex(fileModelRight->index(root));
}

void MainWindow::on_pbUpRight_clicked()
{
    QModelIndex rootIdx = ui->treeViewRight->rootIndex();
    QString rootIndexPath(rootIdx.data(QFileSystemModel::FilePathRole).toString());
    if (!drives.contains(rootIndexPath))
    {
        ui->treeViewRight->setRootIndex(rootIdx.parent());
    }
}

void MainWindow::on_treeViewRight_entered(const QModelIndex &index)
{
    qDebug() << "Entered right pane's item " << fileModelRight->fileName(index);
}

void MainWindow::on_treeViewRight_activated(const QModelIndex &index)
{
    QString sPath = fileModelRight->fileInfo(index).absoluteFilePath();
//    ui->treeViewRight->setRootIndex(fileModelRight->setRootPath(sPath));
    ui->treeViewRight->setRootIndex(fileModelRight->index(sPath));
}


// private functions
void MainWindow::getDriveInfo(const QString &drive, QString &name, QString &type, QString &size, QString &free)
{
    QStorageInfo storage(drive);
//    if (storage.isReadOnly())
//        qDebug() << "isReadOnly:" << storage.isReadOnly();

    name = storage.name();
    type = storage.fileSystemType();
    double sizeGB = double(storage.bytesTotal())/1073741824;
    double freeGB = double(storage.bytesAvailable())/1073741824;
    size.sprintf("%.3f", sizeGB);
    free.sprintf("%.3f", freeGB);
}
