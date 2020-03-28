#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setQuitOnLastWindowClosed(false);
  Widget w;
  return a.exec();
}
