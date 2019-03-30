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
  label_dbtype = new QLabel(this);
  label_dbtype->setMinimumSize(80, 0);
  label_dbtype->setIndent(15);
  label_dbtype->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,
                                          QSizePolicy::Preferred));
  label_dbtype->addAction(ui->actionDisconnect);
  label_dbtype->setContextMenuPolicy(Qt::ActionsContextMenu);
  label_user = new QLabel(this);
  label_user->addAction(ui->actionLogout);
  label_user->setContextMenuPolicy(Qt::ActionsContextMenu);
  ui->statusBar->addPermanentWidget(label_user);
  ui->statusBar->addPermanentWidget(label_dbtype);
  QString IPBlock = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  ui->serverIP->setValidator(new QRegExpValidator(QRegExp(
                               IPBlock + "\\." + IPBlock + "\\." + IPBlock + "\\." + IPBlock
                             )));
  auto validator = new QRegExpValidator(QRegExp(
                                          "[a-z][a-z0-9]*",
                                          Qt::CaseInsensitive
                                        ));
  ui->serverUser->setValidator(validator);
  ui->loginUser->setValidator(validator);
  ui->registerUser->setValidator(validator);
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
  DBtype = static_cast<DB>(ui->serverDB->currentIndex());

  if (
    (
      DBtype == DB::SQLITE
      && ui->SQLiteFile->text().length() > 0
    ) || (
      DBtype != DB::SQLITE
      && ui->serverIP->hasAcceptableInput()
      && ui->serverUser->hasAcceptableInput()
      && ui->serverPass->text().length() > 0
    )
  )
    {
      QMessageBox::information(this, "Всё норм",
                               "Типа пытаюсь подключиться");
      serverIP = ui->serverIP->text();
      serverPort = ui->serverPort->text();
      serverUser = ui->serverUser->text();
      updateDBLabel();
      ui->pages->setCurrentIndex(1);
    }
  else
    { QMessageBox::critical(this, "Ошибка", "Указаны неверные параметры подключения"); }
}

void MainWindow::on_SQLiteSelectFile_clicked()
{
  QMessageBox::information(this, "Всё норм",
                           "Типа выбор файла");
  ui->SQLiteFile->setText("Типа имя файла");
}

void MainWindow::on_buttonLogin_clicked()
{
  if (
    ui->loginUser->hasAcceptableInput()
    && ui->loginPass->text().length() > 0
  )
    {
      QMessageBox::information(this, "Всё норм",
                               "Типа вошёл в систему");
      label_user->setText("Обналиченный Воробушек");
      DBUser = ui->loginUser->text();
      DBPass = ui->loginPass->text();
      DBName = "Воробушек";
      DBSurname = "Накормленный";
      updateUserLabel();
      ui->pages->setCurrentIndex(2);
    }
  else
    { QMessageBox::critical(this, "Ошибка", "Не указано имя пользователя или пароль"); }
}

void MainWindow::on_buttonRegister_clicked()
{
  if (
    ui->registerUser->hasAcceptableInput()
    && ui->registerPass->text().length() > 0
    && ui->registerPass->text() == ui->registerPassRepeat->text()
    && ui->registerName->text().length() > 0
    && ui->registerSurname->text().length() > 0
  )
    {
      QMessageBox::information(this, "Всё норм",
                               "Типа зарегистрировался и вошёл");
      DBUser = ui->registerUser->text();
      DBPass = ui->registerPass->text();
      DBName = ui->registerName->text();
      DBSurname = ui->registerSurname->text();
      updateUserLabel();
      ui->pages->setCurrentIndex(2);
    }
  else
    { QMessageBox::critical(this, "Ошибка", "Заполнены не все поля или пароли не совпадают"); }
}

void MainWindow::on_actionDisconnect_triggered()
{
  if (QMessageBox::question(this, "Эээ?",
                            "Отключиться от базы данных?") == QMessageBox::Yes)
    {
      QMessageBox::information(this, "Всё норм",
                               "Типа отключился от базы данных");
      logout();
      DBtype = DB::NOTCONNECTED;
      updateDBLabel();
      ui->pages->setCurrentIndex(0);
    }
}

void MainWindow::on_actionLogout_triggered()
{
  if (QMessageBox::question(this, "Эээ?",
                            "Выйти из системы?") == QMessageBox::Yes)
    {
      QMessageBox::information(this, "Всё норм",
                               "Типа вышел из системы");
      logout();
      ui->pages->setCurrentIndex(1);
    }
}

void MainWindow::updateDBLabel()
{
  QString DBText, DBToolTip;

  switch (DBtype)
    {
      case DB::SQLITE:
      {
        DBText = "SQLite";
        DBToolTip = "Имя файла: " + ui->SQLiteFile->text();
        break;
      }

      case DB::POSTGRESQL:
      {
        DBText = "PostgreSQL";
        QString dbname(
          "Адрес сервера: %1\n"
          "Порт сервера: %2\n"
          "Имя пользователя: %3"
        );
        DBToolTip = dbname.arg(
                      serverIP,
                      serverPort,
                      serverUser
                    );
        break;
      }

      case DB::NOTCONNECTED:
      {
        DBText = "";
        DBToolTip = "";
        break;
      }
    }

  label_dbtype->setText(DBText);
  label_dbtype->setToolTip(DBToolTip);
}

void MainWindow::logout()
{
  DBUser = DBPass = DBName = DBSurname = "";
  updateUserLabel();
}

void MainWindow::updateUserLabel()
{
  label_user->setText(QString("%1 %2").arg(DBSurname, DBName));
}
