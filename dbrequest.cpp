#include "dbrequest.h"

DBRequest::DBRequest()
{
  QTextStream o(stdout);

  foreach (QString s, QSqlDatabase::drivers())
    { o << s << '\n'; }
}

DBRequest::~DBRequest()
{
}

bool DBRequest::connect()
{
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(":memory:");

  if (!db.open())
    { return false; }

  QStringList tables = db.tables();

  if (
    !tables.contains("users", Qt::CaseInsensitive)
    || !tables.contains("points", Qt::CaseInsensitive)
  )
    { return false; }
}
