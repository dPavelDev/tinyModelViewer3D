#ifndef LOADOBJECTSTATUSDIALOG_H
#define LOADOBJECTSTATUSDIALOG_H

#include <QDialog>

namespace Ui {
class LoadObjectStatusDialog;
}

class LoadObjectStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadObjectStatusDialog(QWidget *parent = nullptr);
    ~LoadObjectStatusDialog();

private:
    Ui::LoadObjectStatusDialog *ui;
};

#endif // LOADOBJECTSTATUSDIALOG_H
