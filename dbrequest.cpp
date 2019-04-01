#include "dbrequest.h"

DBRequest::DBRequest()
{
}

DBRequest::~DBRequest()
{
}

bool DBRequest::connect()
{
  QSqlDatabase::removeDatabase("connection1");
  QString driver;

  switch (DBtype)
    {
      case DB::SQLITE:
      {
        driver = "QSQLITE";
        break;
      }

      case DB::POSTGRESQL:
      {
        driver = "QPSQL";
        break;
      }

      default:
      {
        return false;
      }
    }

  db = QSqlDatabase::addDatabase(driver, "connection1");
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

