#include "mainwindow.h"
#include <QApplication>
#include <QObject>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("thonojato");
    a.setOrganizationDomain("thonojato.ch");
    a.setApplicationName("SimpleExplorer");
    QString appDirPath(QApplication::applicationDirPath());
    QSettings appSettings(QApplication::applicationDirPath() + "/SimpExp.ini", QSettings::IniFormat);

    MainWindow w(&appSettings);
    a.connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(aboutToQuit()), Qt::DirectConnection);
    w.show();

    return a.exec();
}
