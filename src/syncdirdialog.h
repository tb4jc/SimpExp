#ifndef SYNCDIRDIALOG_H
#define SYNCDIRDIALOG_H

#include <QDialog>

namespace Ui {
class SyncDirDialog;
}

class SyncDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SyncDirDialog(QWidget *parent = nullptr);
    ~SyncDirDialog();

private:
    Ui::SyncDirDialog *ui;
};

#endif // SYNCDIRDIALOG_H
