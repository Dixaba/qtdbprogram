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
    void setup(DB DBtype, const QString &database, const QString &IP = "",
               int port = 0, const QString &user = "", const QString &pass = "");

  private:
    QString IP, user, pass, database;
    int port;
    QSqlDatabase db;
    DB DBtype;
};

#endif // DBREQUEST_H
