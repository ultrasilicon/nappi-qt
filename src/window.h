#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QUdpSocket>

class Window : public QWidget
{
  Q_OBJECT

public:
  Window(QWidget *parent = nullptr);
  ~Window();

private:
  QMenu *tray_icon_menu;
  QAction *wake_action;
  QAction *show_action;
  QAction *quit_action;
  QSystemTrayIcon *tray_icon;

  QUdpSocket* udp_socket;
  quint16 port = 65282;
};
#endif // WIDGET_H
