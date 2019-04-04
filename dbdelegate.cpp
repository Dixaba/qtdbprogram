// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "dbdelegate.h"

#include <QDoubleSpinBox>

DBDelegate::DBDelegate(QObject *parent) : QSqlRelationalDelegate(parent) {}

QWidget *DBDelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  switch (index.column())
    {
      case 1:
      {
        auto *sb = new QDoubleSpinBox(parent);
        sb->setMaximum(180);
        sb->setMinimum(-180);
        sb->setDecimals(6);
        return sb;
      }

      case 2:
      {
        auto *sb = new QDoubleSpinBox(parent);
        sb->setMaximum(90);
        sb->setMinimum(-90);
        sb->setDecimals(6);
        return sb;
      }
    }

  return QSqlRelationalDelegate::createEditor(parent, option, index);
}
