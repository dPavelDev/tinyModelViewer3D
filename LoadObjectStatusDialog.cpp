#include "LoadObjectStatusDialog.h"
#include "ui_loadobjectstatusdialog.h"

LoadObjectStatusDialog::LoadObjectStatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadObjectStatusDialog)
{
    ui->setupUi(this);
}

LoadObjectStatusDialog::~LoadObjectStatusDialog()
{
    delete ui;
}
