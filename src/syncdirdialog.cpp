#include "syncdirdialog.h"
#include "ui_syncdirdialog.h"

SyncDirDialog::SyncDirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncDirDialog)
{
    ui->setupUi(this);
}

SyncDirDialog::~SyncDirDialog()
{
    delete ui;
}
