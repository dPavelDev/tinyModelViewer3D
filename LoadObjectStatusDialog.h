#pragma once

#include <QBasicTimer>
#include <QDialog>
#include <QFuture>
#include "SceneStat.h"
#include "Scene.h"

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

    QString _path;
    SceneStat* sceneStat = nullptr;
    QFuture<Scene*> f;
    Scene** _resultScene = nullptr;

    QBasicTimer _timer;

    bool _isStartLoading = false;

    void _startLoading();
    void _stopLoading();
};
