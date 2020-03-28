#include "widget.h"

#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent)
  : QWidget(parent)
  , tray_icon_menu(new QMenu(this))
  , wake_action(new QAction(tr("Stay &Awake"), this))
  , show_action(new QAction(tr("&Settings"), this))
  , quit_action(new QAction(tr("&Quit"), this))
  , tray_icon(new QSystemTrayIcon(this))
  , udp_socket(new QUdpSocket(this))
{
  wake_action->setCheckable(true);
  wake_action->setChecked(false);
//  tray_icon_menu->addAction(wake_action);
//  tray_icon_menu->addAction(show_action);
//  tray_icon_menu->addSeparator();
  tray_icon_menu->addAction(quit_action);
  tray_icon->setIcon(QIcon(":/img/img/icon_tray.png"));
  tray_icon->setToolTip("nappi");
  tray_icon->setContextMenu(tray_icon_menu);
  tray_icon->setVisible(true);
  tray_icon->show();

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, [&](){
      udp_socket->writeDatagram("{\"type\": \"wake\", \"version\": \"0.0.1\"}", QHostAddress("10.10.10.47"), port);
    });
  timer->setInterval(300);
  timer->start();

}

Widget::~Widget()
{
}

