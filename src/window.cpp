#include "window.h"
#include "version.h"

#include <QTimer>
#include <QJsonDocument>
#include <QDebug>
#include <QCoreApplication>
#include <QNetworkDatagram>


Window::Window(QWidget *parent)
  : QWidget(parent)
  , tray_icon_menu(new QMenu(this))
  , wake_action(new QAction(tr("Stay &Awake"), this))
  , show_action(new QAction(tr("&Settings"), this))
  , quit_action(new QAction(tr("&Quit"), this))
  , tray_icon(new QSystemTrayIcon(this))
  , server_address(QHostAddress("10.10.10.47"))
  , udp_socket(new QUdpSocket(this))
{
  wake_action->setCheckable(true);
  wake_action->setChecked(false);
//  tray_icon_menu->addAction(wake_action);
  tray_icon_menu->addAction(show_action);
  tray_icon_menu->addSeparator();
  tray_icon_menu->addAction(quit_action);

  QIcon icon(":/img/img/icon_tray_white.png");
  icon.setIsMask(true);
  tray_icon->setIcon(icon);
  tray_icon->setToolTip("nappi");
  tray_icon->setContextMenu(tray_icon_menu);
  tray_icon->setVisible(true);
  tray_icon->show();

  udp_socket->bind(QHostAddress("0.0.0.0"), port);

  connect(quit_action, &QAction::triggered, this, &QCoreApplication::quit);
  connect(show_action, &QAction::triggered, this, &QWidget::show);
  connect(udp_socket, &QUdpSocket::readyRead, this, &Window::onMessage);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, [&](){
      udp_socket->writeDatagram("{\"type\": \"wake\", \"version\": \"0.0.1\"}", server_address, port);
    });
  timer->setInterval(300);
  timer->start();

  this->setWindowTitle("nappi");
}

Window::~Window()
{
}

void Window::onMessage()
{
  while (udp_socket->hasPendingDatagrams()) {
      QByteArray data = udp_socket->receiveDatagram().data();
      QJsonParseError jsonErr;
      QJsonDocument message = QJsonDocument::fromJson(data, &jsonErr);
      if(jsonErr.error == QJsonParseError::NoError && message.isObject()) {
        if(message["version"].toString() != VERSION_STR)
          return;
        if(message["type"].toString() == "heartbeat")
          qDebug() << "heart beat";
      }
  }
}

