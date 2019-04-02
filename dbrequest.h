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
    void disconnect();
    void setup(DB DBtype, const QString &database, const QString &IP = "",
               int port = 0, const QString &user = "", const QString &pass = "");
    QString getDBname();
    QString getDBconnection();

  private:
    QString IP, user, pass, database;
    int port = 0;
    DB DBtype = DB::NOTCONNECTED;

    const QString connection = QStringLiteral("connection1");
};

#endif // DBREQUEST_H
