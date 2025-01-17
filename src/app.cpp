#include "app.h"

App::App(int &argc, char **argv)
  : QApplication(argc, argv)
  , config(new Config())
  , service(new Service(config))
  , window(new Window(config))
{
  service->start();
  this->setQuitOnLastWindowClosed(false);

  qRegisterMetaType<PiState>("PiState");
  connect(service, &Service::piStateChanged, window, &Window::onPiStateChanged);
}

App::~App()
{
  service->quit();
  service->deleteLater();
  if(window)
    window->deleteLater();
}
