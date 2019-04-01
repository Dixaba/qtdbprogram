#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QRegExpValidator>

#include "dbrequest.h"

//TODO DB wrapper


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

  private:
    Ui::MainWindow *ui;
    QLabel *label_dbtype;
    QLabel *label_user;
    QRegExpValidator *nameValidator;
    QRegExpValidator *IPValidator;
    DB DBtype = DB::NOTCONNECTED;
    QString serverIP, serverPort, serverUser, serverPass, SQLiteFile;
    QString DBUser, DBPass, DBName, DBSurname;
    DBRequest dbr;

    void logout();
    void disconnect();
    void updateDBLabel();
    void updateUserLabel();

  protected:
    void closeEvent(QCloseEvent * /*event*/) override;
};

#endif // MAINWINDOW_H
