#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scaleSlider->setValue(70); // set default scale
    ui->rotationSpeedSlider->setValue(10); // set default rotation speed
    ui->rotationSpeedCheckBox->setCheckState(Qt::Checked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
