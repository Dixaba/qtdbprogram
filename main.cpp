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
