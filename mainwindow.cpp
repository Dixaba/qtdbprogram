// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QVector2D>
#include <QtMath>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFileDialog>

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
  setWindowTitle(windowTitle());
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
      dbr.setup(
        DBtype,
        (DBtype == DB::SQLITE) ? ui->SQLiteFile->text() : QStringLiteral("qtdbprogram"),
        ui->serverIP->text(),
        ui->serverPort->text().toInt(),
        ui->serverUser->text(),
        ui->serverPass->text()
      );

      if (dbr.connect())
        {
          updateDBLabel();
          ui->pages->setCurrentIndex(1);
        }
      else
        {
          QMessageBox msgBox;
          msgBox.setText(
            QStringLiteral("Возможно, указаны неверное имя пользователя или пароль, перепроверьте"));
          msgBox.setWindowTitle(
            QStringLiteral("Не удалось подключиться к БД"));
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setDetailedText(QStringLiteral(
                                   "Также есть вероятность, что база данных повреждена. Для исправления обратитесь к системному администратору и покажите это сообщение\n\n"
                                   "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY, login VARCHAR UNIQUE NOT NULL, pass VARCHAR NOT NULL, name VARCHAR NOT NULL, surname VARCHAR NOT NULL);\n"
                                   "CREATE TABLE IF NOT EXISTS points(id INTEGER PRIMARY KEY, latitude DECIMAL(9,6) NOT NULL, longitude DECIMAL(9,6) NOT NULL, pointtype INTEGER NOT NULL);\n"
                                   "DROP TABLE IF EXISTS pointtypes;\n"
                                   "CREATE TABLE pointtypes(id INTEGER PRIMARY KEY NOT NULL, name VARCHAR NOT NULL);\n"
                                   "INSERT INTO pointtypes (id, name) VALUES (0, 'Ведущая');\n"
                                   "INSERT INTO pointtypes (id, name) VALUES (1, 'Ведомая');\n"
                                 ));
          msgBox.exec();
        }
    }
  else
    { QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Указаны неверные параметры подключения")); }
}

void MainWindow::on_SQLiteSelectFile_clicked()
{
  QString filename = QFileDialog::getOpenFileName(this,
                     QStringLiteral("Открыть файл БД"), nullptr,
                     QStringLiteral("Базы данных (*.db)"));
  ui->SQLiteFile->setText(filename);
}

void MainWindow::on_buttonLogin_clicked()
{
  if (
    ui->loginUser->hasAcceptableInput()
    && ui->loginPass->text().length() > 0
  )
    {
      DBUser = ui->loginUser->text();
      DBPass = QString(
                 QCryptographicHash::hash(
                   ui->loginPass->text().toUtf8(),
                   QCryptographicHash::Md5
                 ).toHex()
               );

      if (dbr.login(DBUser, DBPass))
        {
          updateUserLabel(false);
          setupModel();
          ui->pages->setCurrentIndex(2);
        }
      else
        { QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Указано неверное имя пользователя или пароль")); }
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
      DBUser = ui->registerUser->text();
      DBPass = QString(
                 QCryptographicHash::hash(
                   ui->registerPass->text().toUtf8(),
                   QCryptographicHash::Md5
                 ).toHex()
               );
      DBName = ui->registerName->text();
      DBSurname = ui->registerSurname->text();

      if (dbr.regIster(DBUser, DBPass, DBName, DBSurname))
        {
          updateUserLabel(true);
          setupModel();
          ui->pages->setCurrentIndex(2);
        }
      else
        { QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Не удалось зарегистрировать пользователя")); }
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
      DBdisconnect();
      ui->pages->setCurrentIndex(0);
    }
}

void MainWindow::on_actionLogout_triggered()
{
  if (QMessageBox::question(this, QStringLiteral("Эээ?"),
                            QStringLiteral("Выйти из системы?")) == QMessageBox::Yes)
    {
      DBlogout();
      ui->pages->setCurrentIndex(1);
    }
}

void MainWindow::DBlogout()
{
  DBUser.clear();
  DBPass.clear();
  DBName.clear();
  DBSurname.clear();
  updateUserLabel(true);

  if (model != nullptr)
    {
      delete model;
      model = nullptr;
    }

  if (proxyModel != nullptr)
    {
      delete proxyModel;
      proxyModel = nullptr;
    }
}

void MainWindow::DBdisconnect()
{
  DBtype = DB::NOTCONNECTED;
  DBlogout();
  dbr.disconnect();
  updateDBLabel();
}

void MainWindow::updateDBLabel()
{
  label_dbtype->setText(dbr.getDBname());
  label_dbtype->setToolTip(dbr.getDBconnection());
}

void MainWindow::updateUserLabel(bool local)
{
  if (!local)
    {
      DBName = dbr.getName();
      DBSurname = dbr.getSurname();
    }

  label_user->setText(QStringLiteral("%1 %2").arg(DBSurname, DBName));
}

void MainWindow::setupModel()
{
  model = dbr.getModel();

  if (!model->select())
    {
      QTextStream(stdout) << model->lastError().text();
      return;
    }

  model->setParent(ui->tableView);
  proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(model);
  ui->tableView->setModel(model);
  ui->tableView->setItemDelegate(new DBDelegate(ui->tableView));
  ui->tableView->setColumnHidden(0, true);
  ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tableView->setCurrentIndex(model->index(0, 0));
}

void MainWindow::closeEvent(QCloseEvent */*event*/)
{
  DBdisconnect();
}

void MainWindow::on_buttonAddPoint_clicked()
{
  if (dbr.addPoint(
        ui->pointLatitude->value(),
        ui->pointLongitude->value(),
        ui->pointType->currentIndex()
      ))
    {
      model->select();
    }
  else
    { QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Не удалось добавить запись")); }
}

void MainWindow::on_buttonDelete_clicked()
{
  if (ui->tableView->selectionModel()->selectedRows().count() > 0)
    {
      model->removeRow(ui->tableView->selectionModel()->selectedRows()[0].row());
      model->select();
    }
}
