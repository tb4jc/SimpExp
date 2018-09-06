#include "syndirdialog.h"
#include "ui_syndirdialog.h"

SynDirDialog::SynDirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SynDirDialog)
{
    ui->setupUi(this);
}

SynDirDialog::~SynDirDialog()
{
    delete ui;
}
