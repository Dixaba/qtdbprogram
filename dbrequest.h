#ifndef DBREQUEST_H
#define DBREQUEST_H

#include <QObject>
#include <QtSql>

enum DB
{
  SQLITE,
  POSTGRESQL,
  NOTCONNECTED
};

class DBRequest {
  public:
    DBRequest();
    ~DBRequest();
    bool connect();

  private:
    QString DBUser, DBPass, DBName, DBSurname;
    QSqlDatabase db;
};

#endif // DBREQUEST_H
