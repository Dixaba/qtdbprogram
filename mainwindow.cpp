#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QVector2D>
#include <QtMath>
#include <QRandomGenerator>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  dbtype = new QLabel(this);
  dbtype->setMinimumSize(80, 0);
  dbtype->setIndent(15);
  dbtype->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
  user = new QLabel(this);
  ui->statusBar->addPermanentWidget(user);
  ui->statusBar->addPermanentWidget(dbtype);
  QString IPBlock = QStringLiteral("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])");
  ui->serverIP->setValidator(new QRegExpValidator(QRegExp(
                               IPBlock + "\\." + IPBlock + "\\." + IPBlock + "\\." + IPBlock
                             )));
  ui->serverUser->setValidator(new QRegExpValidator(QRegExp(
                                 "[a-z][a-z0-9]+", Qt::CaseInsensitive
                               )));
  on_serverDB_currentIndexChanged(0);
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_serverDB_currentIndexChanged(int index)
{
  if (index == 0)
    {
      ui->label_serverIP->hide();
      ui->label_serverPort->hide();
      ui->label_serverUser->hide();
      ui->label_serverPass->hide();
      ui->serverIP->hide();
      ui->serverPort->hide();
      ui->serverUser->hide();
      ui->serverPass->hide();
      ui->label_SQLite->show();
      ui->SQLiteFile->show();
      ui->SQLiteSelectFile->show();
    }
  else
    {
      ui->label_serverIP->show();
      ui->label_serverPort->show();
      ui->label_serverUser->show();
      ui->label_serverPass->show();
      ui->serverIP->show();
      ui->serverPort->show();
      ui->serverUser->show();
      ui->serverPass->show();
      ui->label_SQLite->hide();
      ui->SQLiteFile->hide();
      ui->SQLiteSelectFile->hide();
    }
}

void MainWindow::on_buttonConnect_clicked()
{
  if (
    (
      ui->serverDB->currentIndex() == 0
      && ui->SQLiteFile->text().length() > 0
    ) || (
      ui->serverDB->currentIndex() > 0
      && ui->serverIP->hasAcceptableInput()
      && ui->serverUser->hasAcceptableInput()
      && ui->serverPass->text().length() > 0
    )
  )
    { QMessageBox::information(this, "Всё норм", "Типа пытаюсь подключиться"); }
  else
    { QMessageBox::critical(this, "Ошибка", "Указаны неверные параметры подключения"); }
}

void MainWindow::on_SQLiteSelectFile_clicked()
{
  QMessageBox::information(this, "Всё норм",
                           "Типа выбор файла");
}
