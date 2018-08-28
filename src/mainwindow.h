#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QSettings>
#include <QComboBox>
#include <QTreeView>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void aboutToQuit();
    void open();

private slots:

    void cbDriveLeftCurrentIndexChanged(int index);
    void leftPaneModelDirectoryLoaded(const QString &path);
    void on_pbRootLeft_clicked();
    void on_pbUpLeft_clicked();
    void on_treeViewLeft_entered(const QModelIndex &index);
    void on_treeViewLeft_activated(const QModelIndex &index);

    void cbDriveRightCurrentIndexChanged(int index);
    void rightPaneModelDirectoryLoaded(const QString &path);
    void on_pbRootRight_clicked();
    void on_pbUpRight_clicked();
    void on_treeViewRight_entered(const QModelIndex &index);
    void on_treeViewRight_activated(const QModelIndex &index);

//    void on_treeViewLeft_customContextMenuRequested(const QPoint &pos);
//    void on_leftPane_openFile(bool checked = false);
//    void on_treeViewRight_customContextMenuRequested(const QPoint &pos);

private:
    void setupMenuBar();
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);
    void setPaneRoot(const QString &root, QComboBox *driveList, QTreeView *treeView, QLineEdit *leDriveInfo);
    void getDriveInfo(const QString &drive, QString &name, QString &type, QString &size, QString &free);
    void LoadSettings();
    void SaveSettings();

    QModelIndex curSelectedIndex;
    QSettings appSettings;
    Ui::MainWindow *ui;
    QStringList drives;
    QFileSystemModel* fileModelLeft;
    QFileSystemModel* fileModelRight;

    QSet<Qt::Key> m_keysPressed;
};

#endif // MAINWINDOW_H
