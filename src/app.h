#ifndef APP_H
#define APP_H

#include "window.h"
#include "service.h"


#include <QApplication>


class App : public QApplication
{
  Q_OBJECT

public:
  App(int &argc, char **argv);
  ~App();

private:
  Config* config;
  Service* service;
  Window* window;
};

#endif // APP_H
