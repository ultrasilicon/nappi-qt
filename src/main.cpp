#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setQuitOnLastWindowClosed(false);
  Window w;
  return a.exec();
}
