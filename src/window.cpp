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
  , offline_timer(new QTimer(this))
{
  wake_action->setCheckable(true);
  wake_action->setChecked(false);
//  tray_icon_menu->addAction(wake_action);
  tray_icon_menu->addAction(show_action);
  tray_icon_menu->addSeparator();
  tray_icon_menu->addAction(quit_action);

  tray_icon->setToolTip("nappi");
  tray_icon->setContextMenu(tray_icon_menu);
  this->refreshTrayIcon();
  tray_icon->setVisible(true);
  tray_icon->show();

  udp_socket->bind(QHostAddress("0.0.0.0"), port);

  offline_timer->setInterval(1000);
  offline_timer->start();

  connect(quit_action, &QAction::triggered, this, &QCoreApplication::quit);
  connect(show_action, &QAction::triggered, this, &QWidget::show);
  connect(udp_socket, &QUdpSocket::readyRead, this, &Window::onMessage);
  connect(offline_timer, &QTimer::timeout, [&](){
      using namespace std::chrono;
      int64_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
      if(now - offline_timestamp > offline_timeout) {
          qDebug()<<"bad";
          pi_state = Offline;
          refreshTrayIcon();
        }
    });


  QTimer* wakeTimer = new QTimer(this);
  connect(wakeTimer, &QTimer::timeout, [&](){
      udp_socket->writeDatagram("{\"type\": \"wake\", \"version\": \"0.0.1\"}", server_address, port);
    });
  wakeTimer->setInterval(300);
  wakeTimer->start();

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
        if(message["type"].toString() == "heartbeat") {
          if(pi_state != Online) {
            pi_state = Online;
            refreshTrayIcon();
          }
          using namespace std::chrono;
          offline_timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        }
      }
    }
}

void Window::refreshTrayIcon()
{
  QIcon icon(pi_state == Online ? ":/img/img/tray_online_light.png" : ":/img/img/tray_offline_light.png");
  icon.setIsMask(true);
  tray_icon->setIcon(icon);
}

