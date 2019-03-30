#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QRegExpValidator>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_serverDB_currentIndexChanged(int index);
    void on_buttonConnect_clicked();

    void on_SQLiteSelectFile_clicked();

  private:
    Ui::MainWindow *ui;
    QLabel *dbtype;
    QLabel *user;

};

#endif // MAINWINDOW_H
