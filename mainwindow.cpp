#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QVector2D>
#include <QtMath>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_toolBox_currentChanged(int index)
{
  ui->stackedWidget->setCurrentIndex(index);
}
