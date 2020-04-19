#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QUdpSocket>

#include <chrono>

class Window : public QWidget
{
  Q_OBJECT

  enum PiState {
    Online = 0,
    Offline,
  };

public:
  Window(QWidget *parent = nullptr);
  ~Window();

private:
  void onMessage();
  inline void refreshTrayIcon();

  QMenu *tray_icon_menu;
  QAction *wake_action;
  QAction *show_action;
  QAction *quit_action;
  QSystemTrayIcon *tray_icon;

  QHostAddress server_address;
  QUdpSocket* udp_socket;
  quint16 port = 65282;

  PiState pi_state = Offline;
  const int offline_timeout = 1000;
  int64_t offline_timestamp;
  QTimer* offline_timer;
};
#endif // WIDGET_H
