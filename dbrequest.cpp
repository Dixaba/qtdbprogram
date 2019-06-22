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
  success &= tables.contains(QStringLiteral("users"));
  success &= tables.contains(QStringLiteral("points"));
  success &= tables.contains(QStringLiteral("pointtypes"));
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
        return u8"Имя файла: " + database;
      }

      case DB::POSTGRESQL:
      {
        QString dbname = u8"Адрес сервера: %1\n"
                         "Порт сервера: %2\n"
                         "Имя пользователя: %3";
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
  model->setEditStrategy(QSqlTableModel::OnFieldChange);
  model->setTable(QStringLiteral("points"));
  model->setRelation(model->fieldIndex(QStringLiteral("pointtype")),
                     QSqlRelation(QStringLiteral("pointtypes"),
                                  QStringLiteral("id"), QStringLiteral("name")));
  model->setHeaderData(model->fieldIndex(QStringLiteral("longitude")),
                       Qt::Horizontal,
                       u8"Долгота");
  model->setHeaderData(model->fieldIndex(QStringLiteral("latitude")),
                       Qt::Horizontal,
                       u8"Широта");
  model->setHeaderData(model->fieldIndex(QStringLiteral("pointtype")),
                       Qt::Horizontal,
                       u8"Тип точки");
  return model;
}

bool DBRequest::login(const QString &login, const QString &pass)
{
  QSqlQuery q = QSqlQuery(QSqlDatabase::database(connection));

  if (q.prepare(
        QStringLiteral("SELECT name, surname FROM users WHERE login=? AND pass=?")))
    {
      q.addBindValue(login);
      q.addBindValue(pass);

      if (q.exec() && q.next())
        {
          QSqlRecord rec = q.record();
          name = rec.value(QStringLiteral("name")).toString();
          surname = rec.value(QStringLiteral("surname")).toString();
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
        QStringLiteral("INSERT INTO users (id, login, pass, name, surname) VALUES ((SELECT COUNT(id) FROM users)+1, ?, ?, ?, ?)")))
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
  return name + "DBRequest";
}

QString DBRequest::getSurname() const
{
  return surname;
}

bool DBRequest::addPoint(double latitude, double longitude, int type)
{
  QSqlQuery q = QSqlQuery(QSqlDatabase::database(connection));
  int id = -1;

  if (q.exec(QStringLiteral("SELECT COUNT(id) AS c FROM points")) && q.next())
    {
      QSqlRecord rec = q.record();
      id = rec.value(QStringLiteral("c")).toInt();

      if (id == 0)
        { id++; }
      else
        {
          if (q.exec(QStringLiteral("SELECT id FROM points ORDER BY id DESC LIMIT 1"))
              && q.next())
            {
              QSqlRecord rec = q.record();
              id = rec.value(QStringLiteral("id")).toInt() + 1;
            }
          else
            {
              return false;
            }
        }
    }

  if (q.prepare(
        QStringLiteral("INSERT INTO points (id, latitude, longitude, pointtype) VALUES (?, ?, ?, ?)")))
    {
      q.addBindValue(id);
      q.addBindValue(latitude);
      q.addBindValue(longitude);
      q.addBindValue(type);
      return q.exec();
    }

  return false;
}
