// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "dbrequest.h"

#include <QMessageBox>

DBRequest::DBRequest() = default;

DBRequest::~DBRequest() = default;

bool DBRequest::connect()
{
  QString driver;

  switch (DBtype)
    {
      case DB::SQLITE:
      {
        driver = QStringLiteral("QSQLITE");
        break;
      }

      case DB::POSTGRESQL:
      {
        driver = QStringLiteral("QPSQL");
        break;
      }

      default:
      {
        return false;
      }
    }

  QSqlDatabase db = QSqlDatabase::addDatabase(driver, connection);
  db.setDatabaseName(database);
  db.setHostName(IP);
  db.setPort(port);
  db.setUserName(user);
  db.setPassword(pass);

  if (!db.open())
    { return false; }

  QStringList tables = db.tables();
  bool success = true;
  success &= tables.contains("users");
  success &= tables.contains("points");
  success &= tables.contains("pointtypes");
  return success;
}

void DBRequest::disconnect()
{
  {
    QSqlDatabase db = QSqlDatabase::database(connection);
    db.close();
  }
  this->DBtype = DB::NOTCONNECTED;
  QSqlDatabase::removeDatabase(connection);
}

void DBRequest::setup(DB DBtype, const QString &database, const QString &IP,
                      int port, const QString &user, const QString &pass)
{
  this->DBtype = DBtype;
  this->database = database;
  this->IP = IP;
  this->port = port;
  this->user = user;
  this->pass = pass;
}

QString DBRequest::getDBname() const
{
  switch (DBtype)
    {
      case DB::SQLITE:
      {
        return QStringLiteral("SQLite");
      }

      case DB::POSTGRESQL:
      {
        return QStringLiteral("PostgreSQL");
      }

      default:
      {
        return QString();
      }
    }
}

QString DBRequest::getDBconnection()
{
  switch (DBtype)
    {
      case DB::SQLITE:
      {
        return "Имя файла: " + database;
      }

      case DB::POSTGRESQL:
      {
        QString dbname(
          "Адрес сервера: %1\n"
          "Порт сервера: %2\n"
          "Имя пользователя: %3"
        );
        return dbname.arg(
                 IP,
                 QString::number(port),
                 user
               );
      }

      default:
      {
        return QString();
      }
    }
}

QSqlRelationalTableModel *DBRequest::getModel()
{
  auto model = new QSqlRelationalTableModel(nullptr,
      QSqlDatabase::database(connection));
  model->setEditStrategy(QSqlTableModel::OnRowChange);
  model->setTable("points");
  model->setHeaderData(model->fieldIndex("longitude"), Qt::Horizontal,
                       "Широта");
  model->setHeaderData(model->fieldIndex("latitude"), Qt::Horizontal,
                       "Долгота");
  model->setHeaderData(model->fieldIndex("ismaster"), Qt::Horizontal,
                       "Ведущая");
  model->setRelation(model->fieldIndex("ismaster"), QSqlRelation("pointtypes",
                     "id", "name"));
  return model;
}

bool DBRequest::login(const QString &login, const QString &pass)
{
  QSqlQuery q = QSqlQuery(QSqlDatabase::database(connection));

  if (q.prepare(
        QLatin1String("SELECT name, surname FROM users WHERE login=? AND pass=?")))
    {
      q.addBindValue(login);
      q.addBindValue(pass);

      if (q.exec() && q.next())
        {
          QSqlRecord rec = q.record();
          name = rec.value("name").toString();
          surname = rec.value("surname").toString();
          return true;
        }
    }

  return false;
}

bool DBRequest::regIster(const QString &login, const QString &pass,
                         const QString &name, const QString &surname)
{
  QSqlQuery q = QSqlQuery(QSqlDatabase::database(connection));

  if (q.prepare(
        QLatin1String("INSERT INTO users (id, login, pass, name, surname) VALUES ((SELECT COUNT(id) FROM users)+1, ?, ?, ?, ?)")))
    {
      q.addBindValue(login);
      q.addBindValue(pass);
      q.addBindValue(name);
      q.addBindValue(surname);
      return q.exec();
    }

  return false;
}

QString DBRequest::getName() const
{
  return name;
}

QString DBRequest::getSurname() const
{
  return surname;
}
