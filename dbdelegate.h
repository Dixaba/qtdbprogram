#ifndef DBDELEGATE_H
#define DBDELEGATE_H

#include <QSqlRelationalDelegate>

class DBDelegate : public QSqlRelationalDelegate {
  public:
    explicit DBDelegate(QObject *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
};

#endif // DBDELEGATE_H
