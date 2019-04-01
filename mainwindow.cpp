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
  label_dbtype->setSizePolicy(QSizePolicy(
                                QSizePolicy::Fixed,
                                QSizePolicy::Preferred
                              ));
  label_dbtype->addAction(ui->actionDisconnect);
  label_dbtype->setContextMenuPolicy(Qt::ActionsContextMenu);
  label_user = new QLabel(this);
  label_user->addAction(ui->actionLogout);
  label_user->setContextMenuPolicy(Qt::ActionsContextMenu);
  ui->statusBar->addPermanentWidget(label_user);
  ui->statusBar->addPermanentWidget(label_dbtype);
  QString IPBlock = QStringLiteral("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])");
  IPValidator = new QRegExpValidator(QRegExp(
                                       IPBlock + "\\." + IPBlock + "\\." + IPBlock + "\\." + IPBlock
                                     ), this);
  ui->serverIP->setValidator(IPValidator);
  nameValidator = new QRegExpValidator(QRegExp(
                                         "[a-z][a-z0-9]*",
                                         Qt::CaseInsensitive
                                       ), this);
  ui->serverUser->setValidator(nameValidator);
  ui->loginUser->setValidator(nameValidator);
  ui->registerUser->setValidator(nameValidator);
  on_serverDB_currentIndexChanged(0);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_serverDB_currentIndexChanged(int index)
{
  bool isSQLite = index == 0;
  ui->label_serverIP->setVisible(!isSQLite);
  ui->label_serverPort->setVisible(!isSQLite);
  ui->label_serverUser->setVisible(!isSQLite);
  ui->label_serverPass->setVisible(!isSQLite);
  ui->serverIP->setVisible(!isSQLite);
  ui->serverPort->setVisible(!isSQLite);
  ui->serverUser->setVisible(!isSQLite);
  ui->serverPass->setVisible(!isSQLite);
  ui->label_SQLite->setVisible(isSQLite);
  ui->SQLiteFile->setVisible(isSQLite);
  ui->SQLiteSelectFile->setVisible(isSQLite);
}

void MainWindow::on_buttonConnect_clicked()
{
  DBtype = static_cast<DB>(ui->serverDB->currentIndex());

  if (
    (DBtype == DB::SQLITE
     && ui->SQLiteFile->text().length() > 0)
    ||
    (DBtype != DB::SQLITE
     && ui->serverIP->hasAcceptableInput()
     && ui->serverUser->hasAcceptableInput()
     && ui->serverPass->text().length() > 0)
  )
    {
      serverIP = ui->serverIP->text();
      serverPort = ui->serverPort->text();
      serverUser = ui->serverUser->text();
      updateDBLabel();
      ui->pages->setCurrentIndex(1);
    }
  else
    { QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Указаны неверные параметры подключения")); }
}

void MainWindow::on_SQLiteSelectFile_clicked()
{
  ui->SQLiteFile->setText("Типа имя файла");
  //TODO open file
}

void MainWindow::on_buttonLogin_clicked()
{
  if (
    ui->loginUser->hasAcceptableInput()
    && ui->loginPass->text().length() > 0
  )
    {
      //TODO DB login
      DBUser = ui->loginUser->text();
      DBPass = ui->loginPass->text();
      DBName = "Воробушек";
      DBSurname = "Накормленный";
      updateUserLabel();
      ui->pages->setCurrentIndex(2);
    }
  else
    { QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Не указано имя пользователя или пароль")); }
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
      //TODO DB regiter user
      DBUser = ui->registerUser->text();
      DBPass = ui->registerPass->text();
      DBName = ui->registerName->text();
      DBSurname = ui->registerSurname->text();
      updateUserLabel();
      ui->pages->setCurrentIndex(2);
    }
  else
    { QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Заполнены не все поля или пароли не совпадают")); }
}

void MainWindow::on_actionDisconnect_triggered()
{
  if (QMessageBox::question(this, QStringLiteral("Эээ?"),
                            QStringLiteral("Отключиться от базы данных?")) ==
      QMessageBox::Yes)
    {
      logout();
      disconnect();
      ui->pages->setCurrentIndex(0);
    }
}

void MainWindow::on_actionLogout_triggered()
{
  if (QMessageBox::question(this, QStringLiteral("Эээ?"),
                            QStringLiteral("Выйти из системы?")) == QMessageBox::Yes)
    {
      logout();
      ui->pages->setCurrentIndex(1);
    }
}

void MainWindow::logout()
{
  //TODO logout
  DBUser.clear();
  DBPass.clear();
  DBName.clear();
  DBSurname.clear();
  updateUserLabel();
}

void MainWindow::disconnect()
{
  //TODO disconnect
  DBtype = DB::NOTCONNECTED;
  updateDBLabel();
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
        DBText.clear();
        DBToolTip.clear();
        break;
      }
    }

  label_dbtype->setText(DBText);
  label_dbtype->setToolTip(DBToolTip);
}

void MainWindow::updateUserLabel()
{
  label_user->setText(QString("%1 %2").arg(DBSurname, DBName));
}

void MainWindow::closeEvent(QCloseEvent * /*event*/)
{
  logout();
}
