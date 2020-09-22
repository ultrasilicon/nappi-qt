#include "window.h"

#include <QTimer>
#include <QJsonDocument>
#include <QDebug>
#include <QCoreApplication>
#include <QNetworkDatagram>


Window::Window(Config* config, QWidget *parent)
  : QWidget(parent)
  , tray_icon_menu(new QMenu(this))
  , wake_action(new QAction(tr("Stay &Awake"), this))
  , show_action(new QAction(tr("&Settings"), this))
  , quit_action(new QAction(tr("&Quit"), this))
  , tray_icon(new QSystemTrayIcon(this))
{
  wake_action->setCheckable(true);
  wake_action->setChecked(false);
//  tray_icon_menu->addAction(wake_action);
  tray_icon_menu->addAction(show_action);
  tray_icon_menu->addSeparator();
  tray_icon_menu->addAction(quit_action);

  this->refreshTrayIcon(false);
  tray_icon->setToolTip("nappi");
  tray_icon->setContextMenu(tray_icon_menu);
  tray_icon->setVisible(true);
  tray_icon->show();

  connect(quit_action, &QAction::triggered, this, &QCoreApplication::quit);
  connect(show_action, &QAction::triggered, this, &QWidget::show);

  this->setWindowTitle("nappi");
}

Window::~Window()
{
}

void Window::onPiStateChanged(PiState state)
{
  refreshTrayIcon(state == PiState::Online);
}

void Window::onMessage()
{

}

void Window::refreshTrayIcon(const bool& online)
{
  QIcon icon(online ? ":/img/img/tray_online_light.png" : ":/img/img/tray_offline_light.png");
  icon.setIsMask(true);
  tray_icon->setIcon(icon);
}

