#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QRegExpValidator>

#include "dbrequest.h"
#include "dbdelegate.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private slots:
    void on_serverDB_currentIndexChanged(int index);
    void on_buttonConnect_clicked();
    void on_SQLiteSelectFile_clicked();
    void on_buttonLogin_clicked();
    void on_buttonRegister_clicked();
    void on_actionDisconnect_triggered();
    void on_actionLogout_triggered();
    void on_buttonAddPoint_clicked();
    void on_buttonDelete_clicked();

  private:
    void DBlogout();
    void DBdisconnect();
    void updateDBLabel();
    void updateUserLabel(bool local);
    void setupModel();

    Ui::MainWindow *ui;
    QLabel *label_dbtype;
    QLabel *label_user;
    QRegExpValidator *nameValidator;
    QRegExpValidator *IPValidator;
    DB DBtype = DB::NOTCONNECTED;
    QString DBUser, DBPass, DBName, DBSurname;
    DBRequest dbr; // DBRequest is needed for database requests
    QSqlRelationalTableModel *model = nullptr;
    QSortFilterProxyModel *proxyModel = nullptr;


  protected:
    void closeEvent(QCloseEvent * /*event*/) override;
};

#endif // MAINWINDOW_H
