#include "LoadObjectStatusDialog.h"
#include "ui_LoadObjectStatusDialog.h"
#include "ObjReader.h"

#include <QtConcurrent/QtConcurrent>

LoadObjectStatusDialog::LoadObjectStatusDialog(QWidget* parent, Scene** resultScene, const QString& path) :
    QDialog(parent),
    ui(new Ui::LoadObjectStatusDialog)
{
    ui->setupUi(this);

    sceneStat = new SceneStat();
    _resultScene = resultScene;

    _path = path;

    _timer.start(100, this);
}

LoadObjectStatusDialog::~LoadObjectStatusDialog()
{
    delete ui;
    delete sceneStat;
}

void LoadObjectStatusDialog::timerEvent(QTimerEvent*)
{
    if (!_isStartLoading)
        _startLoading();

    if (f.isCanceled())
        return;

    if (f.isFinished())
    {
        ui->cancelButton->setText("Закрыть");

        if (_resultScene != nullptr)
            *_resultScene = f.result();
    }

    ui->materialsCount->setText(QString::number(sceneStat->materialsCount()));
    ui->texturesCount->setText(QString::number(sceneStat->texturesCount()));

    ui->verticiesCount->setText(QString::number(sceneStat->verticiesCount()));
    ui->normalsCount->setText(QString::number(sceneStat->normalsCount()));
    ui->texCoordsCount->setText(QString::number(sceneStat->texVerticiesCount()));
    ui->facesCount->setText(QString::number(sceneStat->facesCount()));

    if (sceneStat->totalBytes() != 0) {
        int currentPercent = 100 * sceneStat->readBytes() / sceneStat->totalBytes();

        ui->progressBar->setValue(currentPercent);
    }
}

void LoadObjectStatusDialog::closeEvent(QCloseEvent *e)
{
    if (! (f.isCanceled() || f.isFinished()) )
        e->ignore();
}

void LoadObjectStatusDialog::setSceneStat(SceneStat* value)
{
    sceneStat = value;
}

void LoadObjectStatusDialog::on_cancelButton_clicked()
{
    _stopLoading();

    close();
}

void LoadObjectStatusDialog::_startLoading()
{
    f = QtConcurrent::run(ObjReader::loadScene, _path, sceneStat);
    _isStartLoading = true;
}

void LoadObjectStatusDialog::_stopLoading()
{
    sceneStat->setCanceled();
    f.cancel();
    f.waitForFinished();
}

