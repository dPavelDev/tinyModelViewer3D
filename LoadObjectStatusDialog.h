#ifndef LOADOBJECTSTATUSDIALOG_H
#define LOADOBJECTSTATUSDIALOG_H

#include <QBasicTimer>
#include <QDialog>
#include <QFuture>
#include "SceneStat.h"
#include "scene.h"

#include <QCloseEvent>

namespace Ui
{
	class LoadObjectStatusDialog;
}

class LoadObjectStatusDialog : public QDialog
{
	Q_OBJECT

public:
    explicit LoadObjectStatusDialog(QWidget* parent, Scene** resultScene, const QString& path);
	~LoadObjectStatusDialog();

	void setSceneStat(SceneStat* value);

protected:
	void timerEvent(QTimerEvent*) override;
    void closeEvent(QCloseEvent * e);

private slots:
	void on_cancelButton_clicked();

private:
    Ui::LoadObjectStatusDialog* ui = nullptr;

    SceneStat* sceneStat = nullptr;
	QFuture<Scene*> f;
    Scene** _resultScene = nullptr;

	QBasicTimer _timer;
};

#endif // LOADOBJECTSTATUSDIALOG_H


