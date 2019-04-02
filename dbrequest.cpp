// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "dbrequest.h"

DBRequest::DBRequest() = default;

DBRequest::~DBRequest() = default;

bool DBRequest::connect()
{
  QSqlDatabase::removeDatabase(connection);
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

  db = QSqlDatabase::addDatabase(driver, connection);
  db.setDatabaseName(database);
  db.setHostName(IP);
  db.setPort(port);
  db.setUserName(user);
  db.setPassword(pass);

  if (!db.open())
    { return false; }

  QStringList tables = db.tables();
  return true;
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

QString DBRequest::getDBname()
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

