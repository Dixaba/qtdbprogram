// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QTranslator qtTranslator;
  qtTranslator.load("qt_" + QLocale::system().name(),
                    QStringLiteral(":/translations"));
  QApplication::installTranslator(&qtTranslator);
  MainWindow w;
  w.show();
  return QApplication::exec();
}
