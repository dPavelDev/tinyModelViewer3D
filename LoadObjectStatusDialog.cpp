#include "LoadObjectStatusDialog.h"
#include "ui_LoadObjectStatusDialog.h"

#include <QtConcurrent/QtConcurrent>

LoadObjectStatusDialog::LoadObjectStatusDialog(QWidget* parent, Scene** resultScene, const QString& path) :
	QDialog(parent),
	ui(new Ui::LoadObjectStatusDialog)
{
	ui->setupUi(this);

	sceneStat = new SceneStat();
	_resultScene = resultScene;

	f = QtConcurrent::run(Scene::loadObj, path, sceneStat);

	_timer.start(100, this);
}

LoadObjectStatusDialog::~LoadObjectStatusDialog()
{
	delete ui;
	delete sceneStat;
}

void LoadObjectStatusDialog::timerEvent(QTimerEvent*)
{
	if (f.isCanceled())
		return;

	if (f.isFinished())
	{
        ui->cancelButton->setText("Закрыть");

        *_resultScene = f.result();
	}

	ui->materialsCount->setText(QString::number(sceneStat->materialsCount()));
	ui->texturesCount->setText(QString::number(sceneStat->texturesCount()));

	ui->verticiesCount->setText(QString::number(sceneStat->verticiesCount()));
	ui->normalsCount->setText(QString::number(sceneStat->normalsCount()));
	ui->texCoordsCount->setText(QString::number(sceneStat->texVerticiesCount()));
	ui->facesCount->setText(QString::number(sceneStat->facesCount()));

    if (sceneStat->totalBytes() != 0)
        ui->progressBar->setValue(100 * sceneStat->readBytes() / sceneStat->totalBytes());
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
    sceneStat->setCanceled();
    f.cancel();
	f.waitForFinished();

	close();
}

